"""Parse Westberry CMSIS + StdPeriph headers into structured data.

Trust level: peer ground truth #1. When this disagrees with other sources, the
header wins. The header file is uniformly formatted by Westberry's tooling so a
regex-based parser is robust enough and avoids the preprocessor headaches of
running pycparser on a vendor SDK.
"""

from __future__ import annotations

import re
from pathlib import Path

from wb32_data.models import (
    HeaderData,
    Interrupt,
    MemoryRegion,
    PeripheralInstance,
    Register,
    RegisterBlock,
    RegisterField,
)


# ---------------------------------------------------------------------------
# Top-level entry
# ---------------------------------------------------------------------------


def parse_cmsis_header(path: Path) -> HeaderData:
    """Parse wb32f10x.h or equivalent into a HeaderData bundle."""
    text = path.read_text(encoding="utf-8", errors="replace")

    data = HeaderData()
    data.core_info = _parse_core_info(text)
    data.constants = _parse_top_level_constants(text)
    data.core_exceptions, data.interrupts = _parse_irq_enum(text)
    base_addresses = _parse_base_addresses(text)
    data.memory = _parse_memory_regions(base_addresses)
    data.register_blocks = _parse_register_blocks(text)
    data.peripherals = _parse_peripheral_instances(text, base_addresses)
    return data


# ---------------------------------------------------------------------------
# Core / constants
# ---------------------------------------------------------------------------


_CORE_DIRECTIVES = {
    "__CM3_REV":           ("core_revision_raw", _hex_or_int := lambda s: int(s, 0)),
    "__MPU_PRESENT":       ("mpu_present",       lambda s: bool(int(s, 0))),
    "__VTOR_PRESENT":      ("vtor_present",      lambda s: bool(int(s, 0))),
    "__NVIC_PRIO_BITS":    ("nvic_prio_bits",    lambda s: int(s, 0)),
    "__Vendor_SysTickConfig": ("vendor_systick_config", lambda s: bool(int(s, 0))),
}


def _parse_core_info(text: str) -> dict[str, str | int | bool]:
    """Extract Cortex-M3 core revision and NVIC bits from #defines."""
    out: dict[str, str | int | bool] = {}
    for macro, (key, conv) in _CORE_DIRECTIVES.items():
        m = re.search(rf"^\s*#define\s+{re.escape(macro)}\s+(\S+)", text, re.MULTILINE)
        if m:
            try:
                # strip C-style suffixes (U, UL, L)
                raw = re.sub(r"[ULul]+$", "", m.group(1))
                out[key] = conv(raw)
            except (ValueError, TypeError):
                out[key] = m.group(1)

    # Cortex-M3 revision is a 16-bit value like 0x0200 => r2p0
    if "core_revision_raw" in out:
        rev = int(out["core_revision_raw"])  # type: ignore[arg-type]
        out["core_revision"] = f"r{(rev >> 8) & 0xff}p{rev & 0xff}"
    return out


def _parse_top_level_constants(text: str) -> dict[str, int]:
    """Parse HSE_VALUE, MHSI_VALUE, FHSI_VALUE and similar oscillator constants."""
    out: dict[str, int] = {}
    for macro in ("HSE_VALUE", "MHSI_VALUE", "FHSI_VALUE", "HSE_STARTUP_TIMEOUT"):
        m = re.search(rf"^\s*#define\s+{macro}\s+\(?\s*([0-9xXa-fA-F]+)", text, re.MULTILINE)
        if m:
            try:
                out[macro] = int(m.group(1), 0)
            except ValueError:
                pass
    return out


# ---------------------------------------------------------------------------
# IRQs
# ---------------------------------------------------------------------------


_IRQ_LINE_RE = re.compile(
    r"^\s*([A-Za-z_][A-Za-z0-9_]*)_IRQn\s*=\s*(-?\d+)\s*,\s*(?:/\*\s*(.*?)\s*\*/)?",
    re.MULTILINE,
)


def _parse_irq_enum(text: str) -> tuple[list[Interrupt], list[Interrupt]]:
    """Return (core_exceptions, vendor_irqs).

    Core exceptions have negative numbers; vendor IRQs are >= 0.
    """
    # Restrict to the IRQn typedef enum block.
    m = re.search(r"typedef\s+enum\s+IRQn\s*\{(.*?)\}\s*IRQn_Type\s*;", text, re.DOTALL)
    if not m:
        return [], []
    body = m.group(1)

    core: list[Interrupt] = []
    vendor: list[Interrupt] = []
    for match in _IRQ_LINE_RE.finditer(body):
        name = match.group(1)
        number = int(match.group(2))
        desc = match.group(3) or ""
        # strip leading IRQ counter notes like "2 Non Maskable Interrupt"
        desc = re.sub(r"^\s*\d+\s+", "", desc)
        irq = Interrupt(number=number, name=name, description=desc.strip())
        if number < 0:
            core.append(irq)
        else:
            vendor.append(irq)
    vendor.sort(key=lambda i: i.number)
    core.sort(key=lambda i: i.number)
    return core, vendor


# ---------------------------------------------------------------------------
# Base addresses
# ---------------------------------------------------------------------------


_BASE_DEFINE_RE = re.compile(
    r"^\s*#define\s+([A-Za-z_][A-Za-z0-9_]*_BASE)\s+(.*?)\s*(?://.*|/\*.*?\*/)?\s*$",
    re.MULTILINE,
)

_BLOCK_COMMENT_RE = re.compile(r"/\*.*?\*/", re.DOTALL)
_LINE_COMMENT_RE = re.compile(r"//.*?$", re.MULTILINE)


def _strip_c_comments(s: str) -> str:
    s = _BLOCK_COMMENT_RE.sub(" ", s)
    s = _LINE_COMMENT_RE.sub("", s)
    return s


def _parse_base_addresses(text: str) -> dict[str, int]:
    """Resolve every #define ending in _BASE to a numeric address.

    Handles indirection like `APB1PERIPH_BASE = PERIPH_BASE + 0x08000`.
    """
    raw: dict[str, str] = {}
    for m in _BASE_DEFINE_RE.finditer(text):
        # Strip any trailing block or line comments from the captured RHS.
        value = _strip_c_comments(m.group(2)).strip()
        if value:
            raw[m.group(1)] = value

    resolved: dict[str, int] = {}

    def resolve(name: str, seen: set[str]) -> int | None:
        if name in resolved:
            return resolved[name]
        if name in seen or name not in raw:
            return None
        seen.add(name)
        expr = raw[name].strip()
        # Strip surrounding parentheses and cast like ((uint32_t)0x40000000UL)
        expr = re.sub(r"\(\s*\(?\s*uint32_t\s*\)?\s*", "(", expr)
        expr = expr.strip()
        # Strip integer suffixes
        expr = re.sub(r"\b(0[xX][0-9a-fA-F]+|\d+)([ULul]+)\b", r"\1", expr)
        # Substitute referenced base symbols.
        for ref in re.findall(r"[A-Za-z_][A-Za-z_0-9]*", expr):
            if ref in raw:
                ref_val = resolve(ref, seen)
                if ref_val is None:
                    return None
                expr = re.sub(rf"\b{re.escape(ref)}\b", str(ref_val), expr)
        try:
            value = int(eval(expr, {"__builtins__": {}}, {}))  # noqa: S307 — restricted env
        except Exception:
            return None
        resolved[name] = value
        return value

    for name in raw:
        resolve(name, set())
    return resolved


# ---------------------------------------------------------------------------
# Memory regions
# ---------------------------------------------------------------------------


_MEMORY_LABELS = {
    "FLASH_BASE":      ("flash",          "Internal flash"),
    "SRAM_BASE":       ("sram",           "Internal SRAM"),
    "PERIPH_BASE":     ("peripherals",    "Peripheral region"),
    "SRAM_BB_BASE":    ("sram_bitband",   "SRAM bit-band alias region"),
    "PERIPH_BB_BASE":  ("peripheral_bitband", "Peripheral bit-band alias region"),
}


def _parse_memory_regions(bases: dict[str, int]) -> list[MemoryRegion]:
    out = []
    for sym, (name, desc) in _MEMORY_LABELS.items():
        if sym in bases:
            out.append(MemoryRegion(name=name, base=bases[sym], description=desc))
    out.sort(key=lambda r: r.base)
    return out


# ---------------------------------------------------------------------------
# Register-block typedefs
# ---------------------------------------------------------------------------


_TYPEDEF_RE = re.compile(
    r"typedef\s+struct\s*\{\s*(.*?)\s*\}\s*([A-Za-z_][A-Za-z0-9_]*_TypeDef)\s*;",
    re.DOTALL,
)

# Match register lines like:
#   __IOM uint32_t MODER;          /*!< description, Address offset: 0x000 */
#   __IM  uint16_t CRH;            /*!< desc Address offset: 0x000 */
# Also handle array forms:
#   __IM  uint32_t DOUT[8];        /*!< desc Address offset: 0x008 - 0x024 */
_REG_LINE_RE = re.compile(
    r"""
    ^\s*
    (?:__(?P<access>IM|OM|IOM)\s+)?     # optional access qualifier
    (?:volatile\s+)?
    (?:const\s+)?
    (?P<ctype>uint\d+_t|int\d+_t)\s+
    (?P<name>[A-Za-z_][A-Za-z0-9_]*)
    (?:\[(?P<count>\d+)\])?              # optional array size
    \s*;\s*
    (?:/\*!?<?\s*(?P<comment>.*?)\s*\*/)?
    """,
    re.VERBOSE | re.MULTILINE,
)

# Catch "Address offset: 0x123" or "0x004 - 0x024" in trailing comment
_OFFSET_RE = re.compile(r"(?:Address\s+offset:\s*)?0x([0-9A-Fa-f]+)")

# RESERVED slots like:
#   uint32_t RESERVED0;
#   uint32_t RESERVED0[39];
_RESERVED_RE = re.compile(
    r"""
    ^\s*
    (?:volatile\s+)?
    (?:const\s+)?
    (?P<ctype>uint\d+_t|int\d+_t)\s+
    (?P<name>RESERVED\d*)
    (?:\[(?P<count>\d+)\])?
    \s*;
    """,
    re.VERBOSE | re.MULTILINE,
)


_CTYPE_SIZES = {
    "uint8_t":  8,
    "uint16_t": 16,
    "uint32_t": 32,
    "int8_t":   8,
    "int16_t":  16,
    "int32_t":  32,
}


def _access(qualifier: str | None) -> str:
    if qualifier == "IM":
        return "r"
    if qualifier == "OM":
        return "w"
    return "rw"


def _parse_register_blocks(text: str) -> dict[str, RegisterBlock]:
    """Find every `typedef struct { ... } X_TypeDef;` and parse the registers.

    Offsets are computed positionally: we walk fields in declaration order and
    accumulate bytes, validating against the `Address offset: 0xNNN` comment
    when present.
    """
    blocks: dict[str, RegisterBlock] = {}
    for m in _TYPEDEF_RE.finditer(text):
        body, name = m.group(1), m.group(2)
        registers, _final_offset = _parse_struct_body(body, typedef_name=name)
        blocks[name] = RegisterBlock(name=name, registers=tuple(registers))
    return blocks


def _parse_struct_body(body: str, *, typedef_name: str) -> tuple[list[Register], int]:
    """Walk a struct body line-by-line, tracking byte offset.

    Returns (registers, final_offset). Reserved slots advance the offset but
    are not emitted as registers. Anonymous unions are partially handled — we
    take the first member's offset as authoritative.
    """
    registers: list[Register] = []
    offset = 0

    union_depth = 0
    union_base: int | None = None
    union_max_size = 0

    for raw_line in body.splitlines():
        line = raw_line.strip()
        if not line:
            continue

        if line.startswith("union"):
            union_depth += 1
            union_base = offset
            union_max_size = 0
            continue
        if line.startswith("}") and union_depth > 0:
            # closing union
            union_depth -= 1
            offset = (union_base or offset) + union_max_size
            union_base = None
            union_max_size = 0
            continue

        # RESERVED slot — advance offset but do not emit.
        rm = _RESERVED_RE.match(raw_line)
        if rm:
            size = _ctype_size_bytes(rm.group("ctype"))
            count = int(rm.group("count") or "1")
            advance = size * count
            if union_depth > 0:
                union_max_size = max(union_max_size, advance)
            else:
                offset += advance
            continue

        rmatch = _REG_LINE_RE.match(raw_line)
        if not rmatch:
            continue

        ctype = rmatch.group("ctype")
        name = rmatch.group("name")
        count = int(rmatch.group("count") or "1")
        comment = rmatch.group("comment") or ""
        size_bytes = _ctype_size_bytes(ctype)
        size_bits = _CTYPE_SIZES[ctype]
        total = size_bytes * count

        # Parse address-offset hint from the comment for sanity-checking.
        hinted_offset = _offset_hint(comment)

        current_offset = union_base if union_depth > 0 else offset
        if hinted_offset is not None and hinted_offset != current_offset:
            # Trust the comment — sometimes structs include __packed/aligned
            # quirks the linear walk misses.
            current_offset = hinted_offset

        registers.append(
            Register(
                name=name,
                offset=current_offset,
                size=size_bits,
                access=_access(rmatch.group("access")),
                description=_clean_description(comment),
            )
        )

        if union_depth > 0:
            union_max_size = max(union_max_size, total)
        else:
            offset = current_offset + total

    return registers, offset


def _ctype_size_bytes(ctype: str) -> int:
    return _CTYPE_SIZES[ctype] // 8


def _offset_hint(comment: str) -> int | None:
    m = _OFFSET_RE.search(comment)
    if not m:
        return None
    try:
        return int(m.group(1), 16)
    except ValueError:
        return None


def _clean_description(comment: str) -> str:
    # Strip the trailing ", Address offset: 0xNNN" part.
    cleaned = re.sub(r",?\s*Address\s+offset:\s*0x[0-9A-Fa-f]+.*$", "", comment).strip()
    cleaned = re.sub(r"\s+", " ", cleaned)
    return cleaned


# ---------------------------------------------------------------------------
# Peripheral instances
# ---------------------------------------------------------------------------


_INSTANCE_RE = re.compile(
    r"""
    ^\s*\#define\s+
    (?P<name>[A-Za-z_][A-Za-z0-9_]*)\s+
    \(\s*\(\s*(?P<typedef>[A-Za-z_][A-Za-z0-9_]*_TypeDef)\s*\*\s*\)\s*
    (?P<base>[A-Za-z_][A-Za-z0-9_]*_BASE)\s*\)
    """,
    re.VERBOSE | re.MULTILINE,
)


def _parse_peripheral_instances(
    text: str,
    bases: dict[str, int],
) -> dict[str, PeripheralInstance]:
    out: dict[str, PeripheralInstance] = {}
    for m in _INSTANCE_RE.finditer(text):
        name = m.group("name")
        base_sym = m.group("base")
        if base_sym not in bases:
            continue
        out[name] = PeripheralInstance(
            name=name,
            base_address=bases[base_sym],
            typedef=m.group("typedef"),
        )
    return out


# ---------------------------------------------------------------------------
# StdPeriph driver headers — bitfield definitions
# ---------------------------------------------------------------------------


# Bit-field defines come in pairs:
#   #define GPIO_MODER_MODER0_Pos        (0U)
#   #define GPIO_MODER_MODER0_Msk        (0x3U << GPIO_MODER_MODER0_Pos)
_BITFIELD_POS_RE = re.compile(
    r"""
    ^\s*\#define\s+
    (?P<periph>[A-Z][A-Z0-9]*)_(?P<reg>[A-Z][A-Z0-9_]*?)_(?P<field>[A-Z][A-Z0-9_]*)_Pos
    \s+\(?\s*(?P<value>\d+)\s*U?\s*\)?
    """,
    re.VERBOSE | re.MULTILINE,
)

_BITFIELD_MSK_RE = re.compile(
    r"""
    ^\s*\#define\s+
    (?P<periph>[A-Z][A-Z0-9]*)_(?P<reg>[A-Z][A-Z0-9_]*?)_(?P<field>[A-Z][A-Z0-9_]*)_Msk
    \s+\(?\s*0[xX](?P<mask>[0-9A-Fa-f]+)\s*U?\s*<<\s*[A-Z0-9_]+_Pos\s*\)?
    """,
    re.VERBOSE | re.MULTILINE,
)


def parse_bitfield_definitions(text: str) -> dict[tuple[str, str], list[RegisterField]]:
    """Return {(periph, reg): [RegisterField, ...]} from a StdPeriph header.

    Both header files (`wb32f10x.h` and `wb32f10x_<periph>.h`) use the same
    convention. Caller is expected to associate the keys with their register
    blocks — multiple keys may apply to the same physical register depending
    on which mode union member is in use (e.g., UART's RBR/THR/DLL).
    """
    positions: dict[tuple[str, str, str], int] = {}
    masks: dict[tuple[str, str, str], int] = {}

    for m in _BITFIELD_POS_RE.finditer(text):
        key = (m.group("periph"), m.group("reg"), m.group("field"))
        positions[key] = int(m.group("value"))
    for m in _BITFIELD_MSK_RE.finditer(text):
        key = (m.group("periph"), m.group("reg"), m.group("field"))
        masks[key] = int(m.group("mask"), 16)

    fields_by_reg: dict[tuple[str, str], list[RegisterField]] = {}
    for key in sorted(positions):
        periph, reg, field_name = key
        if key not in masks:
            continue
        pos = positions[key]
        mask = masks[key]
        # bit_width = number of contiguous low bits set in the unshifted mask
        width = 0
        m = mask
        while m & 1:
            width += 1
            m >>= 1
        if m != 0:
            # non-contiguous mask — emit as-is with width=1 to avoid lying
            width = 1
        fields_by_reg.setdefault((periph, reg), []).append(
            RegisterField(
                name=field_name,
                bit_offset=pos,
                bit_width=width,
            )
        )
    return fields_by_reg

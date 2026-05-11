"""Parse the ChibiOS-Contrib WB32 HAL port into structured data.

Trust level: peer ground truth #2 (alongside vendor headers). Covers
WB32FQ95xx and WB32F3G71xx only; the WB32F10x family has no port and yields
an empty ChibiosData bundle.

Sources within the port:

  os/hal/ports/WB32/<chip>/wb32_registry.h  — capability flags
  os/hal/ports/WB32/<chip>/wb32_isr.h        — IRQ vector + number mappings
  os/hal/ports/WB32/<chip>/wb32_rcc.h        — clock-enable masks + helpers
  os/hal/ports/WB32/LLD/DMAv1/wb32_dma.h    — DMA peripheral handshake numbers
  os/hal/ports/WB32/LLD/*/hal_*_lld.h        — default IRQ priorities + extra bitfields
"""

from __future__ import annotations

import re
from pathlib import Path

from wb32_data.models import (
    AhbPeripheralMask,
    ChibiosData,
    ChibiosCapability,  # noqa: F401 — re-exported for callers
    ClockEnable,
    DmaHandshake,
    IrqPriority,
    IsrVector,
)
from wb32_data.sources.headers import parse_bitfield_definitions


# ---------------------------------------------------------------------------
# Top-level entry
# ---------------------------------------------------------------------------


def parse_chibios_port(port_root: Path, chip_family: str) -> ChibiosData:
    """Parse the ChibiOS-Contrib WB32 port for a given chip family.

    ``port_root`` should point at ``os/hal/ports/WB32/`` inside a ChibiOS-Contrib
    checkout. ``chip_family`` is the directory name (e.g. ``WB32FQ95xx``).
    Returns an empty ``ChibiosData`` if the port doesn't exist for that family.
    """
    chip_dir = port_root / chip_family
    if not chip_dir.is_dir():
        return ChibiosData(chip_family=chip_family)

    data = ChibiosData(chip_family=chip_family)

    registry = chip_dir / "wb32_registry.h"
    if registry.exists():
        data.capabilities = _parse_registry(registry.read_text(encoding="utf-8", errors="replace"))

    isr = chip_dir / "wb32_isr.h"
    if isr.exists():
        data.isr_vectors = _parse_isr(isr.read_text(encoding="utf-8", errors="replace"))

    rcc = chip_dir / "wb32_rcc.h"
    if rcc.exists():
        rcc_text = rcc.read_text(encoding="utf-8", errors="replace")
        data.clock_enables = _parse_clock_enables(rcc_text)
        data.ahb_peripheral_masks = _parse_ahb_peripheral_masks(rcc_text)

    lld_root = port_root / "LLD"
    if lld_root.is_dir():
        dma_h = lld_root / "DMAv1" / "wb32_dma.h"
        if dma_h.exists():
            data.dma_handshakes = _parse_dma_handshakes(dma_h.read_text(encoding="utf-8", errors="replace"))

        data.irq_priorities = _collect_irq_priorities(lld_root)
        data.extra_register_fields = _collect_extra_bitfields(lld_root)

    return data


# ---------------------------------------------------------------------------
# wb32_registry.h
# ---------------------------------------------------------------------------


_REGISTRY_RE = re.compile(
    r"^\s*#define\s+(WB32_[A-Z][A-Z0-9_]*)\s+(TRUE|FALSE|-?\d+)\b",
    re.MULTILINE,
)


def _parse_registry(text: str) -> dict[str, str]:
    """Return {macro: value} for every `WB32_*` capability define."""
    return {m.group(1): m.group(2) for m in _REGISTRY_RE.finditer(text)}


# ---------------------------------------------------------------------------
# wb32_isr.h
# ---------------------------------------------------------------------------


_VECTOR_RE = re.compile(
    r"^\s*#define\s+WB32_(?P<name>[A-Za-z][A-Za-z0-9_]*)_IRQ_VECTOR\s+Vector(?P<hex>[0-9A-Fa-f]+)\b",
    re.MULTILINE,
)
_NUMBER_RE = re.compile(
    r"^\s*#define\s+WB32_(?P<name>[A-Za-z][A-Za-z0-9_]*)_NUMBER\s+(?P<num>\d+)\b",
    re.MULTILINE,
)


def _parse_isr(text: str) -> list[IsrVector]:
    vectors = {m.group("name"): int(m.group("hex"), 16) for m in _VECTOR_RE.finditer(text)}
    numbers = {m.group("name"): int(m.group("num")) for m in _NUMBER_RE.finditer(text)}
    common = sorted(vectors.keys() & numbers.keys(), key=lambda n: numbers[n])
    return [IsrVector(name=n, vector_offset=vectors[n], irq_number=numbers[n]) for n in common]


# ---------------------------------------------------------------------------
# wb32_rcc.h — clock-enable helpers + AHB peripheral masks
# ---------------------------------------------------------------------------


# Two forms appear in the file:
#   #define rccEnableQSPI() rccEnableAPB1(RCC_APB1ENR_QSPIEN)
#   #define rccEnableBKP() do { rccEnableAHB2(RCC_AHBPeriph_BKP >> 24); } while ...
_CLOCK_ENABLE_SIMPLE_RE = re.compile(
    r"^\s*#define\s+rccEnable(?P<periph>[A-Za-z0-9_]+)\(\)\s+"
    r"rccEnable(?P<bus>AHB2|AHB|APB1|APB2)\((?P<field>[A-Z][A-Z0-9_]*)\)",
    re.MULTILINE,
)

# Multi-line `do { ... } while (false)` form — find rccEnable<BUS>(<FIELD>) inside.
_CLOCK_ENABLE_DOWHILE_RE = re.compile(
    r"#define\s+rccEnable(?P<periph>[A-Za-z0-9_]+)\(\)\s+do\s*\{(?P<body>.*?)\}\s*while\s*\(\s*false\s*\)\s*;?",
    re.DOTALL,
)
_BODY_ENABLE_RE = re.compile(
    r"rccEnable(?P<bus>AHB2|AHB|APB1|APB2)\((?P<field>[A-Z][A-Za-z0-9_]*)",
)


def _parse_clock_enables(text: str) -> list[ClockEnable]:
    out: dict[str, ClockEnable] = {}

    for m in _CLOCK_ENABLE_SIMPLE_RE.finditer(text):
        periph = m.group("periph")
        out[periph] = ClockEnable(
            peripheral=periph,
            bus=m.group("bus"),     # type: ignore[arg-type]
            field=m.group("field"),
        )

    # Multi-line do-while form — only capture the first rccEnable<BUS>(...) inside,
    # which is the primary clock-gate for the peripheral.
    for m in _CLOCK_ENABLE_DOWHILE_RE.finditer(text):
        periph = m.group("periph")
        if periph in out:
            continue
        bm = _BODY_ENABLE_RE.search(m.group("body"))
        if not bm:
            continue
        out[periph] = ClockEnable(
            peripheral=periph,
            bus=bm.group("bus"),    # type: ignore[arg-type]
            field=bm.group("field"),
        )

    return sorted(out.values(), key=lambda c: (c.bus, c.peripheral))


_AHB_PERIPH_MASK_RE = re.compile(
    r"^\s*#define\s+RCC_AHBPeriph_(?P<name>[A-Za-z][A-Za-z0-9_]*)\s+"
    r"\(\(uint32_t\)\s*0x(?P<mask>[0-9A-Fa-f]+)\)",
    re.MULTILINE,
)


def _parse_ahb_peripheral_masks(text: str) -> list[AhbPeripheralMask]:
    out = []
    for m in _AHB_PERIPH_MASK_RE.finditer(text):
        out.append(AhbPeripheralMask(name=m.group("name"), mask=int(m.group("mask"), 16)))
    return sorted(out, key=lambda x: x.mask)


# ---------------------------------------------------------------------------
# LLD/DMAv1/wb32_dma.h — DMA peripheral handshake interface numbers
# ---------------------------------------------------------------------------


_HWHIF_RE = re.compile(
    r"^\s*#define\s+WB32_DMAC_HWHIF_(?P<sig>[A-Za-z][A-Za-z0-9_]*)\s+(?P<num>\d+)\b",
    re.MULTILINE,
)
_DMAC_SECTION_RE = re.compile(r"only used for DMAC(?P<n>[12])", re.IGNORECASE)
_DIR_SUFFIX_RE = re.compile(r"^(?P<base>.+?)_(?P<dir>RX|TX)$")


def _parse_dma_handshakes(text: str) -> list[DmaHandshake]:
    """Walk the file, tracking which DMAC section we're in based on the comment markers."""
    # Find section markers — every "only used for DMACn" comment.
    sections: list[tuple[int, str]] = [(0, "DMAC1")]  # default before first marker
    for m in _DMAC_SECTION_RE.finditer(text):
        sections.append((m.start(), f"DMAC{m.group('n')}"))

    def controller_at(pos: int) -> str:
        # Find the last section marker that started before this position.
        active = "DMAC1"
        for start, ctrl in sections:
            if start <= pos:
                active = ctrl
            else:
                break
        return active

    out: list[DmaHandshake] = []
    for m in _HWHIF_RE.finditer(text):
        sig = m.group("sig")
        dir_match = _DIR_SUFFIX_RE.match(sig)
        if dir_match:
            base, direction = dir_match.group("base"), dir_match.group("dir")
        else:
            base, direction = sig, None
        out.append(
            DmaHandshake(
                controller=controller_at(m.start()),  # type: ignore[arg-type]
                peripheral=base,
                direction=direction,             # type: ignore[arg-type]
                handshake_number=int(m.group("num")),
            )
        )
    return out


# ---------------------------------------------------------------------------
# LLD/*/hal_*_lld.h — default IRQ priorities + extra register bit-fields
# ---------------------------------------------------------------------------


# Matches "#define WB32_<DRIVER>_<INSTANCE>_IRQ_PRIORITY 5" where <DRIVER> is
# the driver short name (ADC/I2C/UART/SPI/GPT) and <INSTANCE> is the peripheral
# name (ADC1/I2C1/UART2/QSPI/TIM3/etc.).
_IRQ_PRIORITY_RE = re.compile(
    r"^\s*#define\s+WB32_(?P<driver>[A-Z]+)_(?P<instance>[A-Z0-9]+)_IRQ_PRIORITY\s+(?P<prio>\d+)\b",
    re.MULTILINE,
)


def _collect_irq_priorities(lld_root: Path) -> list[IrqPriority]:
    out: dict[tuple[str, str], int] = {}
    for hdr in sorted(lld_root.glob("*/hal_*_lld.h")):
        text = hdr.read_text(encoding="utf-8", errors="replace")
        for m in _IRQ_PRIORITY_RE.finditer(text):
            key = (m.group("driver"), m.group("instance"))
            # First occurrence wins (subsequent ones are typically the same default).
            out.setdefault(key, int(m.group("prio")))
    return [
        IrqPriority(driver=d, instance=i, priority=p)
        for (d, i), p in sorted(out.items())
    ]


def _collect_extra_bitfields(lld_root: Path):
    """Walk all LLD headers (driver + helper) looking for _Pos/_Msk pairs.

    These supplement the sparse vendor-header bitfield definitions, particularly
    for TIM/SPI/I2C/USB peripherals that the vendor omits.
    """
    out: dict[tuple[str, str], list] = {}
    for hdr in sorted(lld_root.glob("*/*.h")):
        text = hdr.read_text(encoding="utf-8", errors="replace")
        fields_by_reg = parse_bitfield_definitions(text)
        for key, fields in fields_by_reg.items():
            out.setdefault(key, []).extend(fields)
    return out

"""Extract register bit-fields from the WB32FQ95xx reference manual PDF.

The vendor C header only defines `_Pos`/`_Msk` pairs for GPIO/PWR/FMC/UART;
the 388-page reference manual documents every register's bit-fields in clean
4-column tables (Bits | Fields | R/W | Description). This extractor walks the
manual recipe-driven, identifies register headings of the form
``15.5.1. Control register 1 (TIMx_CR1)``, and associates each bit-field
table with its register.

Output shape matches ``parse_bitfield_definitions`` in ``headers.py`` so the
two sources merge identically into ``data/peripherals/<typedef>.yaml``.
"""

from __future__ import annotations

import re
from dataclasses import dataclass, field
from pathlib import Path
from typing import Iterable

import pdfplumber
import yaml

from wb32_data.models import RegisterField


@dataclass
class ManualBitFields:
    """Output of parsing the reference manual."""

    fields_by_reg: dict[tuple[str, str], list[RegisterField]] = field(default_factory=dict)
    """{(peripheral_prefix, register_name): [RegisterField, ...]}.

    Peripheral prefix is normalized to the form used by the vendor C header
    (e.g. ``TIM`` not ``TIMx``, ``UART`` not ``UART_x``).
    """

    unmatched_tables: list[tuple[int, str]] = field(default_factory=list)
    """Pages where a bit-field table was found but no register heading could
    be associated. Useful for iterating the recipe."""


# ---------------------------------------------------------------------------
# Recipe loader
# ---------------------------------------------------------------------------


def load_recipe(path: Path) -> dict:
    return yaml.safe_load(path.read_text(encoding="utf-8")) or {}


# ---------------------------------------------------------------------------
# Top-level entry
# ---------------------------------------------------------------------------


def extract_reference_manual(
    pdf_path: Path,
    recipe: dict,
    repo_root: Path | None = None,
) -> ManualBitFields:
    """Apply a recipe to the reference manual and return all parsed bit-fields."""
    out = ManualBitFields()
    sections = recipe.get("sections", {}) or {}

    with pdfplumber.open(pdf_path) as pdf:
        for _section_name, spec in sections.items():
            pages = spec.get("pages") or []
            for fields in _extract_section(pdf, pages):
                key = fields["key"]
                existing_names = {f.name for f in out.fields_by_reg.get(key, [])}
                for f in fields["fields"]:
                    if f.name in existing_names:
                        continue
                    out.fields_by_reg.setdefault(key, []).append(f)
                    existing_names.add(f.name)

            for page_no, table_idx in _find_orphan_tables(pdf, pages):
                out.unmatched_tables.append((page_no, f"table {table_idx}"))

    return out


# ---------------------------------------------------------------------------
# Section-level walk
# ---------------------------------------------------------------------------


# Match register-name section headings. Two formats appear in the manual:
#
#   1. "15.5.1. Control register 1 (TIMx_CR1)" — name in parens (TIM/UART chapters)
#   2. "24.15.1. I2C_CON"                       — name follows the number (I2C chapter)
#
# Both require a leading section number to avoid inline parenthetical mentions.
_REG_HEADING_PAREN_RE = re.compile(
    r"^\s*\d+\.\d+(?:\.\d+)?\.?\s+.*?\((?P<id>[A-Z][A-Za-z0-9_]*?)_(?P<reg>[A-Z][A-Z0-9_]+)\)",
    re.MULTILINE,
)
_REG_HEADING_BARE_RE = re.compile(
    r"^\s*\d+\.\d+(?:\.\d+)?\.?\s+(?P<id>[A-Z][A-Za-z0-9_]*?)_(?P<reg>[A-Z][A-Z0-9_]+)\s*$",
    re.MULTILINE,
)


def _iter_register_headings(text: str):
    for m in _REG_HEADING_PAREN_RE.finditer(text):
        yield m.start(), m.group("id"), m.group("reg")
    for m in _REG_HEADING_BARE_RE.finditer(text):
        # The bare-form regex matches greedily across underscores, so it can
        # mis-split names like "CLR_SMBUS_INTR" into periph="CLR" /
        # reg="SMBUS_INTR". Only accept bare matches whose periph is a known
        # WB32 peripheral name.
        periph_raw = m.group("id")
        known = set(_PERIPH_NORM.values()) | set(_PERIPH_NORM.keys())
        if periph_raw in known:
            yield m.start(), periph_raw, m.group("reg")


def _extract_section(pdf, pages: list[int]) -> Iterable[dict]:
    """Walk pages in order, tracking the most-recent register heading.

    Within a single page we need to associate each table with the heading
    that immediately precedes it (not just the last heading on the page),
    so we iterate headings + tables together by character/object position.
    Tables that span pages without a re-heading inherit the last-seen
    context from the previous page.
    """
    current_reg_key: tuple[str, str] | None = None

    for page_no in pages:
        if not (1 <= page_no <= len(pdf.pages)):
            continue
        page = pdf.pages[page_no - 1]
        text = page.extract_text() or ""

        # Find heading positions (character offsets) on this page.
        headings: list[tuple[int, tuple[str, str]]] = []
        for pos, periph_raw, reg in _iter_register_headings(text):
            periph = _normalize_peripheral(periph_raw)
            headings.append((pos, (periph, reg)))

        # Tables: pdfplumber returns them in document order. We approximate
        # their position by finding the table header text in the page text.
        tables = page.extract_tables()

        # Approach: build an ordered timeline of (position, kind, payload).
        # Headings give exact char offsets; tables we approximate by their
        # first row of cells joined and searched in text.
        timeline: list[tuple[int, str, object]] = []
        for pos, key in headings:
            timeline.append((pos, "heading", key))

        # Anchor each table at a position after the previous table's anchor —
        # otherwise repeated bit-range strings cause all tables to anchor at
        # the first occurrence.
        cursor = 0
        for tbl in tables:
            if not _is_bitfield_table(tbl):
                continue
            anchor = _table_anchor_pos(tbl, text, start_from=cursor)
            timeline.append((anchor, "table", tbl))
            cursor = anchor + 1

        timeline.sort(key=lambda t: (t[0], 0 if t[1] == "heading" else 1))

        for _pos, kind, payload in timeline:
            if kind == "heading":
                current_reg_key = payload   # type: ignore[assignment]
            else:
                if current_reg_key is None:
                    continue
                fields = _parse_bitfield_table(payload)
                if fields:
                    yield {"key": current_reg_key, "fields": fields}


def _table_anchor_pos(tbl, page_text: str, start_from: int = 0) -> int:
    """Best-effort: find where this table appears in page text.

    Anchors on the first non-empty Fields-column value, which is usually
    unique within a page (bit ranges like "31:9" repeat across tables).
    Falls back to the first bit-range cell, then to ``start_from`` if nothing
    matches.
    """
    if not tbl or not tbl[0]:
        return start_from
    # First pass: try field names (column 1), since they're typically unique.
    for row in tbl[1:6]:
        if not row or len(row) < 2:
            continue
        name = (row[1] or "").strip()
        if name and name.lower() != "reserved":
            # Strip optional [n:m] bit notation that the manual sometimes uses.
            name = re.sub(r"\[\d+(?::\d+)?\]", "", name).strip()
            if len(name) >= 3:
                idx = page_text.find(name, start_from)
                if idx >= 0:
                    return idx
    # Fallback: search for the bits cell.
    for row in tbl[1:4]:
        if not row:
            continue
        first = (row[0] or "").strip()
        if first:
            idx = page_text.find(first, start_from)
            if idx >= 0:
                return idx
    return start_from


def _find_orphan_tables(pdf, pages: list[int]):
    """Yield (page_no, table_idx) pairs for bit-field tables that appear
    before any register heading was seen."""
    saw_heading = False
    for page_no in pages:
        if not (1 <= page_no <= len(pdf.pages)):
            continue
        page = pdf.pages[page_no - 1]
        text = page.extract_text() or ""
        if next(_iter_register_headings(text), None) is not None:
            saw_heading = True
        if saw_heading:
            continue
        for ti, tbl in enumerate(page.extract_tables()):
            if _is_bitfield_table(tbl):
                yield page_no, ti


# ---------------------------------------------------------------------------
# Peripheral name normalization
# ---------------------------------------------------------------------------


# The manual writes TIMx_CR1 where x is a placeholder for the instance number.
# Normalize to the bare peripheral prefix matching the C header typedef.
_PERIPH_NORM = {
    "TIMx":     "TIM",
    "USARTx":   "UART",       # WB32 calls them UART; some manual text uses USART
    "UARTx":    "UART",
    "I2Cx":     "I2C",
    "SPIx":     "SPI",
    "GPIOx":    "GPIO",
    "DMACx":    "DMAC",
    "EXTI":     "EXTI",
    "ADC":      "ADC",
    "RTC":      "RTC",
    "BKP":      "BKP",
    "IWDG":     "IWDG",
    "WWDG":     "WWDG",
    "RCC":      "RCC",
    "PWR":      "PWR",
    "AFIO":     "AFIO",
    "CRC":      "CRC",
    "FMC":      "FMC",
    "USB":      "USB",
    "I2S":      "I2S",
    "LED":      "LED",
    "DBGMCU":   "DBGMCU",
    "ANCTL":    "ANCTL",
    "SFM":      "SFM",
    "ISO":      "ISO",
    "CACHE":    "CACHE",
    "RNG":      "RNG",
    "SYS":      "SYS",
}


def _normalize_peripheral(raw: str) -> str:
    return _PERIPH_NORM.get(raw, raw)


# ---------------------------------------------------------------------------
# Table parsing
# ---------------------------------------------------------------------------


def _is_bitfield_table(tbl) -> bool:
    """Header signature: ['Bits', 'Fields' or 'Field', 'R/W' or 'RW' or 'Access',
    'Description' or 'description' or 'Function'] (case-insensitive)."""
    if not tbl or not tbl[0]:
        return False
    header = [(c or "").strip().lower() for c in tbl[0]]
    if len(header) < 4:
        return False
    return (
        header[0] in {"bits", "bit"}
        and header[1] in {"fields", "field"}
        and header[2] in {"r/w", "rw", "access", "type"}
        and header[3] in {"description", "function", "descriptions"}
    )


_BIT_RANGE_RE = re.compile(r"^\s*\[?(?P<hi>\d+)(?:[:\-](?P<lo>\d+))?\]?\s*$")
_FIELD_NAME_RE = re.compile(r"^(?P<name>[A-Za-z_][A-Za-z0-9_]*)(?:\[\d+(?::\d+)?\])?")


def _parse_bitfield_table(tbl) -> list[RegisterField]:
    """Parse rows after the header into RegisterField records.

    Skips reserved rows (empty Fields cell or "Reserved" text).
    """
    out: list[RegisterField] = []
    for row in tbl[1:]:
        if not row or len(row) < 4:
            continue
        bits_cell = (row[0] or "").strip()
        field_cell = (row[1] or "").strip()
        if not field_cell or field_cell.lower() == "reserved":
            continue

        bit_lo, bit_width = _parse_bit_range(bits_cell)
        if bit_lo is None:
            continue
        name_match = _FIELD_NAME_RE.match(field_cell)
        if not name_match:
            continue
        out.append(
            RegisterField(
                name=name_match.group("name"),
                bit_offset=bit_lo,
                bit_width=bit_width,
            )
        )
    return out


def _parse_bit_range(s: str) -> tuple[int | None, int]:
    """Parse `31:16`, `[9:8]`, `7`, single-bit etc. Returns (bit_offset, width).

    Returns (None, 0) when the input doesn't parse.
    """
    s = s.strip()
    m = _BIT_RANGE_RE.match(s)
    if not m:
        return None, 0
    hi = int(m.group("hi"))
    lo_str = m.group("lo")
    lo = int(lo_str) if lo_str is not None else hi
    if hi < lo:
        hi, lo = lo, hi
    return lo, (hi - lo + 1)

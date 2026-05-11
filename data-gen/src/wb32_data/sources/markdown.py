"""Parse the repository's markdown documentation for pin/AF claims.

Trust level: lowest. Every claim extracted here is subject to validation
against vendor headers, ChibiOS-Contrib, and the datasheet PDF. The
generator never inserts markdown facts directly into output YAML unless no
higher-trust source covers the field.

Currently parses ``docs/chip-overview.md``. The tables are GitHub-flavored
markdown, structured per peripheral kind (UART, I2C, SPI/QSPI, Timers,
I2S, ADC, LED Controller, etc.). Each table has a header row, a separator
row, and one PinSignal claim per data row.
"""

from __future__ import annotations

import re
from pathlib import Path

from wb32_data.models import MarkdownData, PinSignal


# ---------------------------------------------------------------------------
# Parser
# ---------------------------------------------------------------------------


_HEADING_RE = re.compile(r"^(?P<level>#{2,4})\s+(?P<title>.+?)\s*$")
_TABLE_ROW_RE = re.compile(r"^\s*\|(.+)\|\s*$")
_TABLE_SEPARATOR_RE = re.compile(r"^\s*\|[\s\-:|]+\|\s*$")
_AF_NUMBER_RE = re.compile(r"AF(\d+)", re.IGNORECASE)
_PIN_NAME_RE = re.compile(r"^P[A-D]\d{1,2}$")
_LQFP64_NOTE_RE = re.compile(r"\(\s*64\s*\)|LQFP64", re.IGNORECASE)


def parse_chip_overview(path: Path) -> MarkdownData:
    """Parse docs/chip-overview.md tables into pin/AF claims."""
    text = path.read_text(encoding="utf-8", errors="replace")
    lines = text.splitlines()

    data = MarkdownData()
    section_stack: list[str] = []
    i = 0
    while i < len(lines):
        line = lines[i]
        heading = _HEADING_RE.match(line)
        if heading:
            level = len(heading.group("level"))
            title = heading.group("title").strip()
            # Maintain a stack so we know what section a later table belongs to.
            section_stack = section_stack[: level - 2]
            section_stack.append(title)
            i += 1
            continue

        if _TABLE_ROW_RE.match(line):
            # Found a table. Consume header + separator + data rows.
            header_cells = _split_row(line)
            if i + 1 < len(lines) and _TABLE_SEPARATOR_RE.match(lines[i + 1]):
                i += 2
                while i < len(lines) and _TABLE_ROW_RE.match(lines[i]):
                    data_cells = _split_row(lines[i])
                    _maybe_emit_pin_signal(data, header_cells, data_cells, section_stack)
                    i += 1
                continue
        i += 1

    return data


def _split_row(line: str) -> list[str]:
    return [c.strip() for c in line.strip().strip("|").split("|")]


def _maybe_emit_pin_signal(
    data: MarkdownData,
    header: list[str],
    row: list[str],
    section_stack: list[str],
) -> None:
    """If ``row`` looks like a Pin/Function/AF table row, extract a PinSignal.

    The chip-overview format is:
      | Pin | Function | AF | Notes |
    Some tables omit the AF column (e.g., USB analog pins, oscillator).
    """
    if not row:
        return

    # Find columns by header name (case-insensitive).
    cols = {h.lower(): i for i, h in enumerate(header)}
    pin_col = _first_col(cols, ["pin"])
    func_col = _first_col(cols, ["function"])
    af_col = _first_col(cols, ["af"])
    notes_col = _first_col(cols, ["notes"])

    if pin_col is None or func_col is None:
        return
    if pin_col >= len(row) or func_col >= len(row):
        return

    pin = _strip_bold(row[pin_col]).split()[0] if row[pin_col].strip() else ""
    if not _PIN_NAME_RE.match(pin):
        return

    func = _strip_bold(row[func_col])
    af: int | None = None
    af_kind: str = "af"
    if af_col is not None and af_col < len(row):
        af_raw = row[af_col]
        af_match = _AF_NUMBER_RE.search(af_raw)
        if af_match:
            af = int(af_match.group(1))
        elif "default" in af_raw.lower():
            af_kind = "default"

    notes = row[notes_col] if notes_col is not None and notes_col < len(row) else ""
    only_package = "LQFP64" if _LQFP64_NOTE_RE.search(notes) or _LQFP64_NOTE_RE.search(pin) else None
    peripheral = _peripheral_from_signal(func)

    # If the func column is an analog channel (ADC_CH0, OSC_IN, USBDM, etc.)
    # we record it with af=None and kind="analog".
    if _looks_analog(func):
        af_kind = "analog"
        af = None

    data.pin_signals.append(
        PinSignal(
            pin=pin,
            af=af,
            signal=func,
            peripheral=peripheral,
            kind=af_kind,                    # type: ignore[arg-type]
            notes=notes,
            only_package=only_package,
        )
    )


def _first_col(cols: dict[str, int], candidates: list[str]) -> int | None:
    for name in candidates:
        if name in cols:
            return cols[name]
    return None


def _strip_bold(s: str) -> str:
    return s.strip().strip("*").strip("`")


def _looks_analog(func: str) -> bool:
    f = func.upper()
    return (
        f.startswith("ADC_")
        or f.startswith("OSC")
        or f in {"USBDM", "USBDP", "USB D-", "USB D+"}
    )


def _peripheral_from_signal(func: str) -> str:
    """Best-effort peripheral name from a signal like 'TIM2_CH1' → 'TIM2'."""
    f = func.strip()
    # Normalize spaced names.
    f = re.sub(r"\s+", "_", f)
    # Match leading "TIMn", "UARTn", "I2Cn", "SPISn", "SPIMn", "ADC", "QSPI", etc.
    m = re.match(r"([A-Za-z]+\d*)", f)
    return m.group(1) if m else f

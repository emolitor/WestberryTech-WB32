"""Test the chip-overview.md table parser."""

from __future__ import annotations

from pathlib import Path

from wb32_data.sources.markdown import parse_chip_overview


_BASE = """\
# Chip Overview

<!-- applies-to: WB32F104xx, WB32F105xx -->
### LED Controller

**Group 1: PA8-PA11 (AF4) -- LED0-LED3**

| Pin | Function | AF |
|-----|----------|----|
| PA8 | LED0 | AF4 |
| PA9 | LED1 | AF4 |

### Unscoped Section

| Pin | Function | AF |
|-----|----------|----|
| PB0 | TIM3_CH3 | AF2 |
"""


def test_pin_af_extracted(tmp_path: Path) -> None:
    f = tmp_path / "chip-overview.md"
    f.write_text(_BASE)
    data = parse_chip_overview(f)
    pins = {(s.pin, s.af, s.signal) for s in data.pin_signals}
    assert ("PA8", 4, "LED0") in pins
    assert ("PA9", 4, "LED1") in pins
    assert ("PB0", 2, "TIM3_CH3") in pins


def test_applies_to_attached(tmp_path: Path) -> None:
    """LED pins should carry applies-to (WB32F104xx, F105xx); the unscoped
    PB0 row in a different `###` heading should NOT inherit."""
    f = tmp_path / "chip-overview.md"
    f.write_text(_BASE)
    data = parse_chip_overview(f)
    led_pins = [s for s in data.pin_signals if s.signal.startswith("LED")]
    assert led_pins, "expected LED rows to be parsed"
    for s in led_pins:
        assert "WB32F104xx" in s.applies_to
        assert "WB32F105xx" in s.applies_to

    unscoped = [s for s in data.pin_signals if s.pin == "PB0"]
    assert unscoped and unscoped[0].applies_to == ()


def test_empty_doc(tmp_path: Path) -> None:
    f = tmp_path / "empty.md"
    f.write_text("# Just a title\n\nNo tables here.\n")
    data = parse_chip_overview(f)
    assert data.pin_signals == []

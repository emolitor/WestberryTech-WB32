"""Test the pin_defs.h parser used by the cross-source validator."""

from __future__ import annotations

from pathlib import Path

from wb32_data.sources.pin_validation import parse_pin_defs


_SAMPLE = """\
/*
    Complete AF mapping table from DS004 Table 3.4-2 (WB32F104xx).
*/

static const af_test_entry_t af_table[] = {
  { GPIOA,  5, 5, "PA5_AF5_QSPI_SCK" },
  { GPIOA,  8, 4, "PA8_AF4_LED0" },
  { GPIOC, 12, 4, "PC12_AF4_LED6" },
};
"""


def test_parses_entries(tmp_path: Path) -> None:
    f = tmp_path / "pin_defs.h"
    f.write_text(_SAMPLE)
    data = parse_pin_defs(f)
    assert len(data.entries) == 3
    e0, e1, e2 = data.entries
    assert (e0.port, e0.pin, e0.af) == ("GPIOA", 5, 5)
    assert e0.pin_name == "PA5"
    assert e0.signal == "QSPI_SCK"
    assert e1.signal == "LED0"
    assert (e2.port, e2.pin, e2.af, e2.signal) == ("GPIOC", 12, 4, "LED6")


def test_source_chip_extracted(tmp_path: Path) -> None:
    f = tmp_path / "pin_defs.h"
    f.write_text(_SAMPLE)
    data = parse_pin_defs(f)
    assert data.source_chip.startswith("WB32F104")


def test_signal_with_trailing_underscores(tmp_path: Path) -> None:
    """`PA0_AF1_TIM2_CH1_ETR` should produce signal `TIM2_CH1_ETR`."""
    f = tmp_path / "pin_defs.h"
    f.write_text('{ GPIOA, 0, 1, "PA0_AF1_TIM2_CH1_ETR" },\n')
    data = parse_pin_defs(f)
    assert data.entries[0].signal == "TIM2_CH1_ETR"


def test_ignores_malformed_entries(tmp_path: Path) -> None:
    text = """
    { GPIOA, 5, 5, "PA5_AF5_QSPI_SCK" },
    not_an_entry_line
    /* { GPIOX, 9, 9, "fake" }, */
    { GPIOB, 12, 4, "PB12_AF4_LED4" },
    """
    f = tmp_path / "pin_defs.h"
    f.write_text(text)
    data = parse_pin_defs(f)
    pins = sorted(e.pin_name for e in data.entries)
    # GPIOX is rejected by the regex (port group restricts to GPIO[A-D]).
    assert pins == ["PA5", "PB12"]

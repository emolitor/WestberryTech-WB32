"""Regression tests for the AF matrix parser.

DS004/DS005 page 16 ships with pdfplumber clipping the leading "P" off the
first column (header reads `ort`, rows read `A0`, `A1`, ...). The parser
must accept these truncated cells and re-prefix the port name. These tests
guard against regression of that fix.
"""

from __future__ import annotations

from wb32_data.sources.pdf import _is_pin_af_table, _parse_pin_af_table


_HEALTHY_HEADER = ["Port", "AF0", "AF1", "AF2", "AF3", "AF4", "AF5", "AF6", "AF7"]
_TRUNCATED_HEADER = ["ort", "AF0", "AF1", "AF2", "AF3", "AF4", "AF5", "AF6", "AF7"]


def test_healthy_table_recognised() -> None:
    tbl = [
        _HEALTHY_HEADER,
        [None, "SYS", "TIM1/2", "TIM3/4", "I2S", "I2C/LED", "SPI(M)", "SPI(S)", "UART"],
        ["PA0", "WKUP", "TIM2_CH1_ETR", "", "", "", "", "", "UART2_CTS"],
    ]
    assert _is_pin_af_table(tbl) is True
    entries = _parse_pin_af_table(tbl)
    pins_afs = {(e.pin, e.af, e.signal) for e in entries}
    assert ("PA0", 0, "WKUP") in pins_afs
    assert ("PA0", 1, "TIM2_CH1_ETR") in pins_afs
    assert ("PA0", 7, "UART2_CTS") in pins_afs


def test_truncated_table_recognised_and_renormalised() -> None:
    """DS004 page 16 strips the leading P; the parser must still emit PA0."""
    tbl = [
        _TRUNCATED_HEADER,
        [None, "SYS", "TIM1/2", "TIM3/4", "I2S", "I2C/LED", "SPI(M)", "SPI(S)", "UART"],
        ["A0", "WKUP", "TIM2_CH1_ETR", "", "", "", "", "", "UART2_CTS"],
        ["A1", "", "TIM2_CH2", "", "", "", "", "", "UART2_RTS"],
    ]
    assert _is_pin_af_table(tbl) is True
    entries = _parse_pin_af_table(tbl)
    pins = {e.pin for e in entries}
    assert pins == {"PA0", "PA1"}, f"expected port name to be re-prefixed; got {pins}"
    sample = next(e for e in entries if e.pin == "PA0" and e.af == 7)
    assert sample.signal == "UART2_CTS"


def test_non_af_table_rejected() -> None:
    tbl = [
        ["Foo", "Bar", "Baz"],
        ["x", "y", "z"],
    ]
    assert _is_pin_af_table(tbl) is False


def test_misaligned_af_columns_rejected() -> None:
    """A header that starts with AF1 (not AF0) should not be parsed — it's
    probably a malformed table rather than the operating AF mux."""
    tbl = [
        ["Port", "AF1", "AF2", "AF3", "AF4", "AF5", "AF6", "AF7", "AF8"],
        ["PA0", "x", "y", "", "", "", "", "", ""],
    ]
    assert _is_pin_af_table(tbl) is False


def test_blank_first_column_skipped() -> None:
    """Continuation rows where the port cell is empty should be ignored."""
    tbl = [
        _HEALTHY_HEADER,
        [None, "SYS", "TIM1/2", "TIM3/4", "I2S", "I2C/LED", "SPI(M)", "SPI(S)", "UART"],
        ["PA0", "WKUP", "", "", "", "", "", "", ""],
        ["", "", "extra", "", "", "", "", "", ""],
    ]
    entries = _parse_pin_af_table(tbl)
    assert all(e.pin == "PA0" for e in entries)


def test_non_port_first_column_skipped() -> None:
    """A row whose first cell isn't a port name should be skipped (e.g. a
    stray text row that snuck into the table)."""
    tbl = [
        _HEALTHY_HEADER,
        [None, "SYS", "TIM1/2", "TIM3/4", "I2S", "I2C/LED", "SPI(M)", "SPI(S)", "UART"],
        ["Note:", "this row is junk", "", "", "", "", "", "", ""],
        ["PA0", "WKUP", "", "", "", "", "", "", ""],
    ]
    entries = _parse_pin_af_table(tbl)
    assert {e.pin for e in entries} == {"PA0"}

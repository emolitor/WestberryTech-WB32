"""Tests for the YAML emitter (hex representer, key ordering)."""

from __future__ import annotations

from pathlib import Path

from wb32_data.emit import emit_yaml


def _emit(data: dict, tmp_path: Path) -> str:
    target = tmp_path / "out.yaml"
    emit_yaml(data, target)
    return target.read_text(encoding="utf-8")


def test_hex_keys_render_in_hex(tmp_path: Path) -> None:
    text = _emit(
        {
            "name": "X",
            "base": 0x40010800,
            "offset": 0x20,
            "usb_vid": 0x342D,
            "rom_base_address": 0x1FFFF000,
            "rom_size": 0x1000,
        },
        tmp_path,
    )
    assert "base: 0x40010800" in text
    assert "offset: 0x20" in text
    assert "usb_vid: 0x342D" in text
    assert "rom_base_address: 0x1FFFF000" in text
    assert "rom_size: 0x1000" in text


def test_non_hex_keys_render_as_decimal(tmp_path: Path) -> None:
    text = _emit({"flash_size": 131072, "ram_size": 36864, "count": 7}, tmp_path)
    assert "flash_size: 131072" in text
    assert "ram_size: 36864" in text
    assert "count: 7" in text


def test_key_order_preserved(tmp_path: Path) -> None:
    text = _emit(
        {
            "name": "A",
            "family": "B",
            "package": "LQFP48",
            "flash_size": 128,
            "ram_size": 32,
        },
        tmp_path,
    )
    lines = [ln for ln in text.splitlines() if ":" in ln and not ln.startswith("#")]
    keys = [ln.split(":", 1)[0].strip() for ln in lines]
    assert keys == ["name", "family", "package", "flash_size", "ram_size"]


def test_nested_hex_keys(tmp_path: Path) -> None:
    text = _emit(
        {
            "peripheral": {"name": "GPIOA", "base": 0x40000000},
            "list": [{"name": "x", "offset": 0x0C}],
        },
        tmp_path,
    )
    assert "base: 0x40000000" in text
    assert "offset: 0xC" in text


def test_bool_is_not_treated_as_hex(tmp_path: Path) -> None:
    # bool is a subclass of int; without a guard the representer would crash
    # or coerce True/False into 0x1/0x0.
    text = _emit({"rom_dfu": True, "rom_size": 0x1000}, tmp_path)
    assert "rom_dfu: true" in text
    assert "rom_size: 0x1000" in text

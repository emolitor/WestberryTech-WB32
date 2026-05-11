"""Light sanity tests for IR dataclasses — catches field-rename regressions."""

from __future__ import annotations

from dataclasses import asdict

from wb32_data.models import (
    Interrupt,
    PdfData,
    PdfElectrical,
    PdfPackage,
    PdfPinAfEntry,
    Register,
    RegisterField,
)


def test_interrupt_round_trip() -> None:
    irq = Interrupt(number=11, name="DMAC1", description="DMAC1 IRQ")
    assert asdict(irq) == {"number": 11, "name": "DMAC1", "description": "DMAC1 IRQ"}


def test_register_with_fields() -> None:
    fields = (RegisterField(name="MODER0", bit_offset=0, bit_width=2),)
    reg = Register(name="MODER", offset=0, size=32, access="rw", fields=fields)
    assert reg.fields[0].bit_offset == 0
    assert reg.fields[0].bit_width == 2


def test_pin_af_entry_immutable() -> None:
    p = PdfPinAfEntry(pin="PA5", af=5, signal="QSPI_SCK")
    assert (p.pin, p.af, p.signal) == ("PA5", 5, "QSPI_SCK")


def test_pdf_data_defaults_empty() -> None:
    d = PdfData(pdf_path="x.pdf", sha256="abc")
    assert d.pin_af_matrix == []
    assert d.packages == []
    assert d.electrical is None
    assert d.raw_text_by_page == {}


def test_pdf_electrical_defaults_none() -> None:
    e = PdfElectrical()
    assert e.vdd_min is None
    assert e.temperature_max is None


def test_pdf_package_optional_fields() -> None:
    pkg = PdfPackage(name="LQFP48", body_mm=(7.0, 7.0), pin_count=48)
    assert pkg.pitch_mm is None
    assert pkg.body_mm == (7.0, 7.0)

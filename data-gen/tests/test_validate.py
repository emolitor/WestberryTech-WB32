"""Cross-source validator unit tests — feed handcrafted models, assert findings."""

from __future__ import annotations

from pathlib import Path

from wb32_data.models import (
    HeaderData,
    MarkdownData,
    PdfData,
    PdfPinAfEntry,
    PinSignal,
)
from wb32_data.sources.pin_validation import PinDefEntry, PinValidationData
from wb32_data.validate import validate


def _pdf_with(entries: list[tuple[str, int, str]]) -> PdfData:
    return PdfData(
        pdf_path="fake.pdf",
        sha256="0" * 64,
        pin_af_matrix=[PdfPinAfEntry(pin=p, af=a, signal=s) for p, a, s in entries],
    )


def test_markdown_disagreement_with_pdf_is_error() -> None:
    header = HeaderData()
    pdf = _pdf_with([("PA5", 5, "QSPI_SCK")])
    markdown = MarkdownData(pin_signals=[
        PinSignal(pin="PA5", af=4, signal="QSPI_SCK", peripheral="QSPI"),    # wrong AF
    ])
    report = validate(chip="WB32F104RC", header=header, chibios=None, pdf=pdf, markdown=markdown)
    assert len(report.errors) == 1
    e = report.errors[0]
    assert "PA5" in e.message and "QSPI_SCK" in e.message
    assert "markdown" in e.sources and "pdf" in e.sources


def test_markdown_agreement_with_pdf_no_finding() -> None:
    pdf = _pdf_with([("PA5", 5, "QSPI_SCK")])
    markdown = MarkdownData(pin_signals=[
        PinSignal(pin="PA5", af=5, signal="QSPI_SCK", peripheral="QSPI"),
    ])
    report = validate(chip="WB32F104RC", header=HeaderData(), chibios=None, pdf=pdf, markdown=markdown)
    assert report.errors == []


def test_pin_defs_disagreement_with_pdf_is_error() -> None:
    pdf = _pdf_with([("PA8", 4, "LED0")])
    pin_val = PinValidationData(
        source_chip="WB32F104xx",
        entries=[PinDefEntry(port="GPIOA", pin=8, af=5, descriptor="PA8_AF5_LED0")],
    )
    report = validate(
        chip="WB32F104RC", header=HeaderData(),
        chibios=None, pdf=pdf, markdown=None,
        pin_validation=pin_val,
    )
    # Disagreement against PDF is an error per validate._validate_pin_validation_against_pdf.
    assert any("pin_defs" in f.category.lower() or "pin_defs" in (s.lower() for s in f.sources)
               for f in report.errors), f"got: {[(f.category, f.message) for f in report.errors]}"


def test_markdown_signal_not_in_pdf_is_warning() -> None:
    """If the markdown asserts a pin/signal that the PDF AF matrix doesn't
    contain, that's a warning (PDF parsing may be incomplete), not an error."""
    pdf = _pdf_with([("PA5", 5, "QSPI_SCK")])  # PDF has only this row
    markdown = MarkdownData(pin_signals=[
        PinSignal(pin="PC3", af=5, signal="SPIM2_MO", peripheral="SPIM2"),
    ])
    report = validate(chip="WB32F104RC", header=HeaderData(), chibios=None, pdf=pdf, markdown=markdown)
    assert report.errors == []
    assert len(report.warnings) >= 1


def test_chip_family_filters_markdown() -> None:
    """A markdown signal scoped to family A should not produce findings on chip
    of family B."""
    pdf = _pdf_with([("PA5", 5, "QSPI_SCK")])
    markdown = MarkdownData(pin_signals=[
        PinSignal(pin="PA5", af=4, signal="QSPI_SCK", peripheral="QSPI",
                  applies_to=("WB32F104xx",)),
    ])
    # Validate against an unrelated family — the signal should be filtered out
    # and produce no errors despite the AF=4 vs PDF AF=5 mismatch.
    report = validate(
        chip="WB32FQ95xC", header=HeaderData(),
        chibios=None, pdf=pdf, markdown=markdown,
        chip_family="WB32FQ95xx",
    )
    assert report.errors == []


def test_report_to_markdown_has_counts() -> None:
    pdf = _pdf_with([("PA5", 5, "QSPI_SCK")])
    markdown = MarkdownData(pin_signals=[
        PinSignal(pin="PA5", af=4, signal="QSPI_SCK", peripheral="QSPI"),
    ])
    report = validate(chip="X", header=HeaderData(), chibios=None, pdf=pdf, markdown=markdown)
    md = report.to_markdown()
    assert "# Validation report: X" in md
    assert "Errors: **1**" in md

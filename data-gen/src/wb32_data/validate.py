"""Cross-source validation.

The trust hierarchy (vendor headers + ChibiOS-Contrib peer ground truth; PDFs
secondary; markdown lowest) is enforced here. Disagreements never silently
overwrite higher-trust facts — they're emitted as graded findings in a
markdown report under ``data/validation-reports/<chip>.md``.

Severity:
  error    — generator should fail; markdown / overlay must be corrected.
  warning  — likely a documentation gap, not a correctness problem.
  info     — informational, no action expected.
"""

from __future__ import annotations

from dataclasses import dataclass, field
from typing import Literal

from wb32_data.models import (
    ChibiosData,
    HeaderData,
    MarkdownData,
    PdfData,
)


Severity = Literal["error", "warning", "info"]


@dataclass(frozen=True)
class Finding:
    severity: Severity
    category: str
    message: str
    sources: tuple[str, ...] = field(default_factory=tuple)


@dataclass
class ValidationReport:
    chip: str
    findings: list[Finding] = field(default_factory=list)

    def add(self, severity: Severity, category: str, message: str, sources: tuple[str, ...] = ()) -> None:
        self.findings.append(Finding(severity=severity, category=category, message=message, sources=sources))

    @property
    def errors(self) -> list[Finding]:
        return [f for f in self.findings if f.severity == "error"]

    @property
    def warnings(self) -> list[Finding]:
        return [f for f in self.findings if f.severity == "warning"]

    @property
    def infos(self) -> list[Finding]:
        return [f for f in self.findings if f.severity == "info"]

    def to_markdown(self) -> str:
        lines = [
            f"# Validation report: {self.chip}",
            "",
            f"Errors: **{len(self.errors)}** &middot; Warnings: {len(self.warnings)} &middot; Info: {len(self.infos)}",
            "",
        ]
        for sev_label, items in [
            ("Errors", self.errors),
            ("Warnings", self.warnings),
            ("Info", self.infos),
        ]:
            lines.append(f"## {sev_label} ({len(items)})")
            if not items:
                lines.append("_None._")
                lines.append("")
                continue
            buckets: dict[str, list[Finding]] = {}
            for f in items:
                buckets.setdefault(f.category, []).append(f)
            for category, findings in buckets.items():
                lines.append(f"### {category}")
                for f in findings:
                    src = f" _(sources: {', '.join(f.sources)})_" if f.sources else ""
                    lines.append(f"- {f.message}{src}")
                lines.append("")
        return "\n".join(lines)


# ---------------------------------------------------------------------------
# Validators
# ---------------------------------------------------------------------------


def validate(
    chip: str,
    header: HeaderData,
    chibios: ChibiosData | None,
    pdf: PdfData | None,
    markdown: MarkdownData | None,
) -> ValidationReport:
    report = ValidationReport(chip=chip)

    if chibios is not None:
        _validate_irqs_against_chibios(header, chibios, report)
        _validate_capabilities_against_header(header, chibios, report)

    if pdf is not None and markdown is not None:
        _validate_markdown_af_against_pdf(markdown, pdf, report)

    if markdown is not None:
        _validate_markdown_peripherals_against_header(header, markdown, report)
        if chibios is not None:
            _validate_markdown_peripherals_against_chibios(chibios, markdown, report)

    if pdf is not None:
        _validate_pin_descriptions_against_header(header, pdf, report)

    return report


def _validate_irqs_against_chibios(
    header: HeaderData,
    chibios: ChibiosData,
    report: ValidationReport,
) -> None:
    """Every IRQ name in the header should appear in ChibiOS' ISR map and vice versa.

    The header uses names like ``TIM1_BRK``; ChibiOS uses the same. ChibiOS
    sometimes renames (``USB`` → ``USB1``, ``USBP_WKUP`` → ``USBP1_WKUP``);
    treat those as known equivalences.
    """
    aliases = {
        "USB1": "USB",
        "USB1_DMA": "USB_DMA",
        "USBP1_WKUP": "USBP_WKUP",
    }

    by_num_chibios = {v.irq_number: aliases.get(v.name, v.name) for v in chibios.isr_vectors}
    by_num_header = {i.number: i.name for i in header.interrupts}

    for num, hname in by_num_header.items():
        cname = by_num_chibios.get(num)
        if cname is None:
            report.add(
                "warning",
                "IRQ coverage",
                f"IRQ #{num} ({hname}) appears in vendor header but not in ChibiOS ISR table",
                ("header", "chibios"),
            )
            continue
        if cname != hname:
            report.add(
                "error",
                "IRQ name mismatch",
                f"IRQ #{num}: header says `{hname}`, ChibiOS says `{cname}`",
                ("header", "chibios"),
            )

    for num, cname in by_num_chibios.items():
        if num not in by_num_header:
            report.add(
                "warning",
                "IRQ coverage",
                f"IRQ #{num} ({cname}) appears in ChibiOS ISR table but not in vendor header",
                ("header", "chibios"),
            )


def _validate_capabilities_against_header(
    header: HeaderData,
    chibios: ChibiosData,
    report: ValidationReport,
) -> None:
    """If ChibiOS says WB32_HAS_FOO=TRUE, the header should expose a peripheral
    instance reachable from that name."""
    # Umbrella flags that summarize a family (e.g. WB32_HAS_TIM means "has any
    # timer", implied by TIM1/TIM2/...) — skip them entirely.
    UMBRELLAS = {"DMAC", "TIM", "I2C", "SPI", "UART", "USB"}
    # Capabilities that map to lines / signals rather than peripheral instances.
    SIGNAL_LIKE = {
        "PVD", "TAMPER",
        "EXTI0", "EXTI1", "EXTI2", "EXTI3", "EXTI4",
        "EXTI9_5", "EXTI15_10",
        "USB1_DMA", "USB1_WKUP",
    }

    for key, value in chibios.capabilities.items():
        if not key.startswith("WB32_HAS_"):
            continue
        if value != "TRUE":
            continue
        name = key.removeprefix("WB32_HAS_")
        if name in UMBRELLAS or name in SIGNAL_LIKE:
            continue

        # Name → expected peripheral instance.
        if name == "USB1":
            target = "USB"
        else:
            target = name

        if target not in header.peripherals:
            report.add(
                "warning",
                "Peripheral coverage",
                f"ChibiOS claims `WB32_HAS_{name}` but no `{target}` instance in vendor header",
                ("chibios", "header"),
            )


def _validate_markdown_af_against_pdf(
    markdown: MarkdownData,
    pdf: PdfData,
    report: ValidationReport,
) -> None:
    """Compare every (pin, signal, AF) claim from markdown against the
    datasheet's Port × AF mux matrix. The PDF is the authority."""
    pdf_lookup: dict[tuple[str, str], int] = {}
    for entry in pdf.pin_af_matrix:
        # Cells like "TIM2_CH1\nTIM2_ETR" carry two signals; record both.
        for sig in (s.strip() for s in entry.signal.split("\n") if s.strip()):
            pdf_lookup[(entry.pin, sig)] = entry.af

    for ps in markdown.pin_signals:
        if ps.af is None or ps.kind != "af":
            continue
        signal = _canonicalize_signal(ps.signal)
        af_pdf = pdf_lookup.get((ps.pin, signal))
        if af_pdf is None:
            # No exact match — try common-prefix matches before warning.
            candidates = [
                af for (pin, sig), af in pdf_lookup.items()
                if pin == ps.pin and sig.replace("_", "").upper() == signal.replace("_", "").upper()
            ]
            if not candidates:
                report.add(
                    "warning",
                    "Markdown AF not in datasheet",
                    f"{ps.pin} {signal} (AF{ps.af} per markdown) not found in datasheet AF matrix",
                    ("markdown", "pdf"),
                )
                continue
            af_pdf = candidates[0]
        if af_pdf != ps.af:
            report.add(
                "error",
                "Markdown AF disagreement",
                f"{ps.pin} {signal}: markdown says AF{ps.af}, datasheet says AF{af_pdf}",
                ("markdown", "pdf"),
            )


def _canonicalize_signal(s: str) -> str:
    """Normalize a markdown signal name to match the datasheet matrix.

    Markdown uses 'QSPI_MI (IO1)' or 'QSPI_NSS0' or 'SPIM2_MI'; the datasheet
    uses 'QSPI_MI_IO1', 'QSPI_NSS0', 'SPIM2_MI'. Strip parenthetical aliases
    and collapse whitespace.
    """
    s = s.strip()
    # Replace " (X)" with "_X" — captures QSPI_MI (IO1) → QSPI_MI_IO1.
    import re as _re
    s = _re.sub(r"\s*\(\s*([^)]+?)\s*\)", lambda m: "_" + m.group(1).replace(" ", "_"), s)
    s = _re.sub(r"\s+", "_", s)
    return s


def _validate_markdown_peripherals_against_header(
    header: HeaderData,
    markdown: MarkdownData,
    report: ValidationReport,
) -> None:
    """Every peripheral the markdown mentions in an AF column should be a
    known instance in the vendor header."""
    known = set(header.peripherals.keys())
    for ps in markdown.pin_signals:
        if ps.kind == "analog":
            continue
        if ps.peripheral.startswith("UART") or ps.peripheral.startswith("TIM"):
            if ps.peripheral not in known:
                report.add(
                    "info",
                    "Markdown peripheral coverage",
                    f"Markdown references `{ps.peripheral}` (pin {ps.pin} signal {ps.signal}) but it is not in the vendor header",
                    ("markdown", "header"),
                )


def _validate_markdown_peripherals_against_chibios(
    chibios: ChibiosData,
    markdown: MarkdownData,
    report: ValidationReport,
) -> None:
    """For peripherals the markdown mentions, check whether the ChibiOS port
    even claims to support them."""
    if not chibios.capabilities:
        return
    supported = {
        key.removeprefix("WB32_HAS_")
        for key, value in chibios.capabilities.items()
        if key.startswith("WB32_HAS_") and value == "TRUE"
    }
    seen_missing: set[str] = set()
    for ps in markdown.pin_signals:
        p = ps.peripheral
        if p in seen_missing or ps.kind == "analog":
            continue
        if p in {"USBDM", "USBDP", "XTAL", "ADC"}:
            continue
        if p not in supported and not any(s.startswith(p) for s in supported):
            seen_missing.add(p)
            report.add(
                "info",
                "Markdown peripheral not in ChibiOS port",
                f"Markdown references `{p}` but ChibiOS WB32 port has no WB32_HAS_{p}",
                ("markdown", "chibios"),
            )


def _validate_pin_descriptions_against_header(
    header: HeaderData,
    pdf: PdfData,
    report: ValidationReport,
) -> None:
    """Sanity check — every GPIO pin in the PDF pin description should belong
    to a GPIO port present in the header."""
    available_ports = {name[-1] for name in header.peripherals if name.startswith("GPIO")}
    for pin in pdf.pin_descriptions:
        if not pin.pin_name.startswith("P"):
            continue
        if len(pin.pin_name) < 2:
            continue
        port = pin.pin_name[1]
        if port not in available_ports:
            report.add(
                "warning",
                "Pin without GPIO port",
                f"Datasheet lists pin `{pin.pin_name}` but vendor header has no GPIO{port}",
                ("pdf", "header"),
            )

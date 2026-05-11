"""Command-line entry point for wb32-data."""

from __future__ import annotations

import json
import sys
from pathlib import Path

import click
from rich.console import Console
from rich.table import Table

from wb32_data.sources.chibios import parse_chibios_port
from wb32_data.sources.headers import parse_cmsis_header
from wb32_data.sources.markdown import parse_chip_overview
from wb32_data.sources.pdf import PdfExtractor, load_recipe
from wb32_data.validate import validate as run_validation

console = Console()


@click.group()
def main() -> None:
    """wb32-data: machine-readable data for WB32 microcontrollers."""


# ---------------------------------------------------------------------------
# inspect-headers — diagnostic only, prints what the header parser sees
# ---------------------------------------------------------------------------


@main.command("inspect-headers")
@click.option(
    "--vendor-lib",
    type=click.Path(exists=True, file_okay=False, path_type=Path),
    required=True,
)
@click.option("--json", "as_json", is_flag=True, help="Emit JSON instead of rich tables")
def inspect_headers(vendor_lib: Path, as_json: bool) -> None:
    """Parse the vendor CMSIS header and print what we extract."""
    header = vendor_lib / "Libraries" / "CMSIS" / "Device" / "WB" / "WB32F10x" / "wb32f10x.h"
    if not header.exists():
        console.print(f"[red]ERROR[/]: cmsis header not found at {header}")
        sys.exit(2)

    data = parse_cmsis_header(header)

    if as_json:
        # Convert to a JSON-serializable form.
        out = {
            "core_info": data.core_info,
            "constants": data.constants,
            "memory": [
                {"name": r.name, "base": f"0x{r.base:08X}", "description": r.description}
                for r in data.memory
            ],
            "core_exceptions": [
                {"number": i.number, "name": i.name, "description": i.description}
                for i in data.core_exceptions
            ],
            "interrupts": [
                {"number": i.number, "name": i.name, "description": i.description}
                for i in data.interrupts
            ],
            "peripherals": {
                name: {
                    "base": f"0x{p.base_address:08X}",
                    "typedef": p.typedef,
                }
                for name, p in sorted(data.peripherals.items())
            },
            "register_blocks": {
                name: [
                    {
                        "name": r.name,
                        "offset": f"0x{r.offset:03X}",
                        "size": r.size,
                        "access": r.access,
                        "description": r.description,
                    }
                    for r in block.registers
                ]
                for name, block in sorted(data.register_blocks.items())
            },
        }
        click.echo(json.dumps(out, indent=2))
        return

    # Pretty print.
    console.rule("[bold]Core info")
    for k, v in data.core_info.items():
        console.print(f"  {k}: {v}")

    console.rule("[bold]Top-level constants")
    for k, v in data.constants.items():
        console.print(f"  {k}: {v}")

    console.rule("[bold]Memory regions")
    mem = Table(show_header=True, header_style="bold")
    mem.add_column("name")
    mem.add_column("base")
    mem.add_column("description")
    for r in data.memory:
        mem.add_row(r.name, f"0x{r.base:08X}", r.description)
    console.print(mem)

    console.rule(f"[bold]Interrupts (vendor: {len(data.interrupts)}, core: {len(data.core_exceptions)})")
    irq = Table(show_header=True, header_style="bold")
    irq.add_column("number", justify="right")
    irq.add_column("name")
    irq.add_column("description")
    for i in data.interrupts:
        irq.add_row(str(i.number), i.name, i.description)
    console.print(irq)

    console.rule(f"[bold]Peripheral instances ({len(data.peripherals)})")
    peri = Table(show_header=True, header_style="bold")
    peri.add_column("name")
    peri.add_column("base")
    peri.add_column("typedef")
    for name, p in sorted(data.peripherals.items()):
        peri.add_row(name, f"0x{p.base_address:08X}", p.typedef)
    console.print(peri)

    console.rule(f"[bold]Register blocks ({len(data.register_blocks)})")
    for name, block in sorted(data.register_blocks.items()):
        console.print(f"[cyan]{name}[/] — {len(block.registers)} registers")


# ---------------------------------------------------------------------------
# inspect-chibios — diagnostic for the ChibiOS-Contrib parser
# ---------------------------------------------------------------------------


@main.command("inspect-chibios")
@click.option(
    "--chibios-contrib",
    type=click.Path(exists=True, file_okay=False, path_type=Path),
    required=True,
)
@click.option(
    "--chip-family",
    type=str,
    default="WB32FQ95xx",
    show_default=True,
)
def inspect_chibios(chibios_contrib: Path, chip_family: str) -> None:
    """Parse the ChibiOS-Contrib WB32 port and summarize extracted data."""
    port = chibios_contrib / "os" / "hal" / "ports" / "WB32"
    if not port.is_dir():
        console.print(f"[red]ERROR[/]: port directory not found at {port}")
        sys.exit(2)

    data = parse_chibios_port(port, chip_family)

    console.rule(f"[bold]Chip family: {data.chip_family}")
    console.print(
        f"capabilities: {len(data.capabilities)} | "
        f"isr_vectors: {len(data.isr_vectors)} | "
        f"clock_enables: {len(data.clock_enables)} | "
        f"ahb_masks: {len(data.ahb_peripheral_masks)} | "
        f"dma_handshakes: {len(data.dma_handshakes)} | "
        f"irq_priorities: {len(data.irq_priorities)} | "
        f"extra_bitfield_regs: {len(data.extra_register_fields)}"
    )

    console.rule("[bold]ISR vectors (first 8)")
    t = Table(show_header=True, header_style="bold")
    t.add_column("name"); t.add_column("irq#", justify="right"); t.add_column("vector_offset")
    for v in data.isr_vectors[:8]:
        t.add_row(v.name, str(v.irq_number), f"0x{v.vector_offset:02X}")
    console.print(t)

    console.rule("[bold]Clock-enable helpers")
    t = Table(show_header=True, header_style="bold")
    t.add_column("peripheral"); t.add_column("bus"); t.add_column("ENR field")
    for c in data.clock_enables:
        t.add_row(c.peripheral, c.bus, c.field)
    console.print(t)

    console.rule("[bold]DMA handshakes")
    t = Table(show_header=True, header_style="bold")
    t.add_column("controller"); t.add_column("peripheral"); t.add_column("dir"); t.add_column("hwhif#", justify="right")
    for h in data.dma_handshakes:
        t.add_row(h.controller, h.peripheral, h.direction or "", str(h.handshake_number))
    console.print(t)

    console.rule("[bold]Default IRQ priorities")
    t = Table(show_header=True, header_style="bold")
    t.add_column("driver"); t.add_column("instance"); t.add_column("priority", justify="right")
    for p in data.irq_priorities:
        t.add_row(p.driver, p.instance, str(p.priority))
    console.print(t)

    console.rule("[bold]AHB peripheral masks")
    t = Table(show_header=True, header_style="bold")
    t.add_column("name"); t.add_column("mask")
    for m in data.ahb_peripheral_masks:
        t.add_row(m.name, f"0x{m.mask:08X}")
    console.print(t)


# ---------------------------------------------------------------------------
# inspect-refman — diagnostic for the reference-manual bit-field extractor
# ---------------------------------------------------------------------------


@main.command("inspect-refman")
@click.option("--recipe", type=click.Path(exists=True, dir_okay=False, path_type=Path), required=True)
@click.option("--repo-root", type=click.Path(exists=True, file_okay=False, path_type=Path), required=True)
@click.option("--section", type=str, default=None, help="Inspect only this recipe section")
def inspect_refman(recipe: Path, repo_root: Path, section: str | None) -> None:
    """Extract bit-fields from the reference manual using a recipe and summarize."""
    from wb32_data.sources.reference_manual import extract_reference_manual, load_recipe

    rec = load_recipe(recipe)
    pdf_path = (repo_root / rec["pdf"]).resolve()
    if not pdf_path.exists():
        console.print(f"[red]ERROR[/]: pdf not found at {pdf_path}")
        sys.exit(2)

    if section is not None:
        rec = {**rec, "sections": {section: rec["sections"][section]}}

    result = extract_reference_manual(pdf_path, rec, repo_root)

    console.rule(f"[bold]{pdf_path.name}")
    console.print(
        f"register groups: {len(result.fields_by_reg)} | "
        f"total fields: {sum(len(v) for v in result.fields_by_reg.values())} | "
        f"orphan tables: {len(result.unmatched_tables)}"
    )
    t = Table(show_header=True, header_style="bold")
    t.add_column("peripheral"); t.add_column("register"); t.add_column("fields", justify="right"); t.add_column("sample")
    for (periph, reg), fields in sorted(result.fields_by_reg.items()):
        sample = ", ".join(f.name for f in fields[:4]) + ("..." if len(fields) > 4 else "")
        t.add_row(periph, reg, str(len(fields)), sample)
    console.print(t)

    if result.unmatched_tables:
        console.rule("[yellow]Orphan tables (no register context)")
        for p, name in result.unmatched_tables[:10]:
            console.print(f"  page {p}: {name}")


# ---------------------------------------------------------------------------
# inspect-pdf — diagnostic for the PDF extractor
# ---------------------------------------------------------------------------


@main.command("inspect-pdf")
@click.option("--recipe", type=click.Path(exists=True, dir_okay=False, path_type=Path), required=True)
@click.option("--repo-root", type=click.Path(exists=True, file_okay=False, path_type=Path), required=True)
@click.option("--save-images/--no-save-images", default=False)
def inspect_pdf(recipe: Path, repo_root: Path, save_images: bool) -> None:
    """Apply a PDF recipe and print what was extracted."""
    rec = load_recipe(recipe)
    pdf_path = (repo_root / rec["pdf"]).resolve()
    if not pdf_path.exists():
        console.print(f"[red]ERROR[/]: pdf not found at {pdf_path}")
        sys.exit(2)

    cache_dir = repo_root / "data-gen" / ".pdf-cache"
    extractor = PdfExtractor(pdf_path, repo_root, cache_dir)

    image_dir = None
    if save_images:
        chip = rec.get("chip", "unknown")
        image_dir = repo_root / "data" / "diagrams" / chip

    data = extractor.extract(rec, image_out_dir=image_dir)

    console.rule(f"[bold]{rec.get('chip', '?')}  ({pdf_path.name})")
    console.print(
        f"sha256: {data.sha256[:16]}…\n"
        f"pin_descriptions: {len(data.pin_descriptions)}\n"
        f"pin_af_matrix entries: {len(data.pin_af_matrix)}\n"
        f"packages: {len(data.packages)}\n"
        f"ordering_variants: {len(data.ordering_variants)}\n"
        f"saved_images: {len(data.saved_images)}\n"
        f"raw_text pages: {sorted(data.raw_text_by_page.keys())}\n"
        f"notes: {data.notes}"
    )

    if data.pin_descriptions:
        console.rule("[bold]Pin descriptions (first 8)")
        t = Table(show_header=True, header_style="bold")
        for col in ["LQFP48", "LQFP64", "Name", "Type", "Main", "AlternateFn", "Analog"]:
            t.add_column(col)
        for p in data.pin_descriptions[:8]:
            t.add_row(
                str(p.pin_number_lqfp48 or ""),
                str(p.pin_number_lqfp64 or ""),
                p.pin_name, p.pin_type, p.main_function,
                ", ".join(p.alternate_functions),
                ", ".join(p.analog_functions),
            )
        console.print(t)

    if data.pin_af_matrix:
        console.rule(f"[bold]Pin AF matrix (PA0–PA7)")
        # filter to PA0–PA7
        targets = {f"PA{i}" for i in range(8)}
        rows = [e for e in data.pin_af_matrix if e.pin in targets]
        t = Table(show_header=True, header_style="bold")
        t.add_column("pin"); t.add_column("af", justify="right"); t.add_column("signal")
        for e in rows[:32]:
            t.add_row(e.pin, str(e.af), e.signal.replace("\n", " / "))
        console.print(t)

    if data.packages:
        console.rule("[bold]Packages")
        for pkg in data.packages:
            console.print(f"  {pkg.name}: body={pkg.body_mm}, pitch={pkg.pitch_mm}, pins={pkg.pin_count}")


# ---------------------------------------------------------------------------
# generate / validate — stubs for now
# ---------------------------------------------------------------------------


def _load_parts_registry(repo_root: Path) -> list[dict]:
    """Load data-gen/recipes/parts.yaml."""
    import yaml
    parts_yaml = repo_root / "data-gen" / "recipes" / "parts.yaml"
    if not parts_yaml.exists():
        return []
    raw = yaml.safe_load(parts_yaml.read_text(encoding="utf-8")) or {}
    return raw.get("parts", []) or []


def _collect_sources(
    part: dict,
    *,
    vendor_lib: Path,
    vendor_docs: Path,
    chibios_contrib: Path | None,
    repo_root: Path,
):
    """Run every relevant source parser for a single part entry."""
    from wb32_data.sources.markdown import parse_chip_overview as _parse_md

    cmsis = vendor_lib / "Libraries" / "CMSIS" / "Device" / "WB" / "WB32F10x" / "wb32f10x.h"
    header = parse_cmsis_header(cmsis)

    chibios = None
    chibios_chip = part.get("chibios_chip")
    if chibios_chip and chibios_contrib is not None:
        port_root = chibios_contrib / "os" / "hal" / "ports" / "WB32"
        chibios = parse_chibios_port(port_root, chibios_chip)

    pdf_data = None
    recipes = part.get("pdf_recipes") or []
    if recipes:
        # For now, apply the first recipe; future versions could merge multiple.
        recipe_path = repo_root / "data-gen" / "recipes" / recipes[0]
        if recipe_path.exists():
            rec = load_recipe(recipe_path)
            pdf_path = (repo_root / rec["pdf"]).resolve()
            if pdf_path.exists():
                extractor = PdfExtractor(
                    pdf_path,
                    repo_root,
                    repo_root / "data-gen" / ".pdf-cache",
                )
                pdf_data = extractor.extract(rec)

    md_path = repo_root / "docs" / "chip-overview.md"
    markdown = _parse_md(md_path) if md_path.exists() else None

    from wb32_data.sources.pin_validation import parse_pin_defs
    pin_defs = repo_root / "examples" / "pin-validation" / "pin_defs.h"
    pin_val = parse_pin_defs(pin_defs) if pin_defs.exists() else None

    return header, chibios, pdf_data, markdown, pin_val


@main.command()
@click.option("--vendor-lib", type=click.Path(exists=True, file_okay=False, path_type=Path), required=True)
@click.option("--vendor-docs", type=click.Path(exists=True, file_okay=False, path_type=Path), required=True)
@click.option("--chibios-contrib", type=click.Path(file_okay=False, path_type=Path), required=False)
@click.option("--repo-root", type=click.Path(exists=True, file_okay=False, path_type=Path), required=True)
@click.option("--out", type=click.Path(file_okay=False, path_type=Path), required=True)
@click.option("--chip", type=str, default=None, help="Generate only this chip")
def generate(
    vendor_lib: Path,
    vendor_docs: Path,
    chibios_contrib: Path | None,
    repo_root: Path,
    out: Path,
    chip: str | None,
) -> None:
    """Generate per-chip YAML files for every part in recipes/parts.yaml."""
    from wb32_data.emit import emit_chip_yaml
    from wb32_data.merge import merge_chip

    parts = _load_parts_registry(repo_root)
    if chip:
        parts = [p for p in parts if p.get("name") == chip]
    if not parts:
        console.print(f"[red]ERROR[/]: no matching parts in registry")
        sys.exit(2)

    out.mkdir(parents=True, exist_ok=True)
    chips_dir = out / "chips"
    chips_dir.mkdir(exist_ok=True)

    # Shared register-block and interrupt YAMLs depend only on the vendor
    # header, so emit them once up front rather than per-chip.
    _emit_shared(out, vendor_lib, repo_root)

    for part in parts:
        name = part["name"]
        console.print(f"[bold]{name}[/]: gathering sources...")
        header, chibios, pdf_data, markdown, pin_val = _collect_sources(
            part,
            vendor_lib=vendor_lib,
            vendor_docs=vendor_docs,
            chibios_contrib=chibios_contrib,
            repo_root=repo_root,
        )
        chip_data = merge_chip(part, header, chibios, pdf_data, markdown)
        # Apply any user-curated overlay.
        override_path = out / "_overrides" / f"{name}.yaml"
        if override_path.exists():
            import yaml as _yaml
            overlay = _yaml.safe_load(override_path.read_text(encoding="utf-8")) or {}
            _deep_merge(chip_data, overlay)
            console.print(f"  applied override: {override_path.relative_to(repo_root)}")
        target = chips_dir / f"{name}.yaml"
        emit_chip_yaml(chip_data, target)
        console.print(f"  [green]wrote[/] {target.relative_to(repo_root)}")


def _emit_shared(out: Path, vendor_lib: Path, repo_root: Path) -> None:
    """Emit data/peripherals/<block>.yaml and data/interrupts/WB32F10x.yaml."""
    from wb32_data.emit import emit_yaml
    from wb32_data.sources.headers import parse_bitfield_definitions

    cmsis = vendor_lib / "Libraries" / "CMSIS" / "Device" / "WB" / "WB32F10x" / "wb32f10x.h"
    header = parse_cmsis_header(cmsis)

    # ---- Peripheral register blocks ----
    drv_dir = vendor_lib / "Libraries" / "WB32F10x_StdPeriph_Driver" / "inc"
    bitfields_by_reg: dict[tuple[str, str], list] = {}
    bitfield_sources: dict[tuple[str, str, str], str] = {}   # (periph, reg, field_name) -> source name

    if cmsis.exists():
        for key, fields in parse_bitfield_definitions(
            cmsis.read_text(encoding="utf-8", errors="replace")
        ).items():
            bitfields_by_reg.setdefault(key, []).extend(fields)
            for f in fields:
                bitfield_sources[(*key, f.name)] = "header"

    if drv_dir.exists():
        for hdr in sorted(drv_dir.glob("wb32f10x_*.h")):
            for key, fields in parse_bitfield_definitions(
                hdr.read_text(encoding="utf-8", errors="replace")
            ).items():
                for f in fields:
                    if (*key, f.name) not in bitfield_sources:
                        bitfields_by_reg.setdefault(key, []).append(f)
                        bitfield_sources[(*key, f.name)] = "header"

    # ---- Layer on reference-manual bit-fields ----
    refman_recipe = repo_root / "data-gen" / "recipes" / "EN_RM2905025_WB32FQ95xx.yaml"
    refman_disagreements: list[str] = []
    if refman_recipe.exists():
        try:
            from wb32_data.sources.reference_manual import extract_reference_manual, load_recipe as _load_rm_recipe
            rec = _load_rm_recipe(refman_recipe)
            refman_pdf = (repo_root / rec["pdf"]).resolve()
            if refman_pdf.exists():
                manual = extract_reference_manual(refman_pdf, rec, repo_root)
                for key, fields in manual.fields_by_reg.items():
                    existing = {f.name: f for f in bitfields_by_reg.get(key, [])}
                    for f in fields:
                        if f.name in existing:
                            # Cross-check: if header and manual disagree on bit
                            # offset/width, log it but keep the header version.
                            e = existing[f.name]
                            if (e.bit_offset, e.bit_width) != (f.bit_offset, f.bit_width):
                                refman_disagreements.append(
                                    f"{key[0]}_{key[1]}.{f.name}: header={e.bit_offset}+{e.bit_width}, "
                                    f"manual={f.bit_offset}+{f.bit_width}"
                                )
                            continue
                        bitfields_by_reg.setdefault(key, []).append(f)
                        bitfield_sources[(*key, f.name)] = "reference_manual"
                console.print(
                    f"  reference manual: {len(manual.fields_by_reg)} register groups, "
                    f"{sum(len(v) for v in manual.fields_by_reg.values())} fields"
                    + (f", [yellow]{len(refman_disagreements)} disagreements with header[/]"
                       if refman_disagreements else "")
                )
        except Exception as e:
            console.print(f"  [yellow]reference manual extraction failed:[/] {e}")

    peripherals_dir = out / "peripherals"
    peripherals_dir.mkdir(parents=True, exist_ok=True)
    for typedef, block in header.register_blocks.items():
        registers = []
        for r in block.registers:
            entry: dict = {
                "name": r.name,
                "offset": r.offset,
                "size": r.size,
                "access": r.access,
            }
            if r.description:
                entry["description"] = r.description
            # Attach any bit-fields parsed from the same peripheral prefix.
            # Block "GPIO_TypeDef" → prefix "GPIO" → look up (GPIO, MODER) etc.
            prefix = typedef.removesuffix("_TypeDef")
            fields = bitfields_by_reg.get((prefix, r.name)) or []
            if fields:
                entry["fields"] = []
                for f in fields:
                    src = bitfield_sources.get((prefix, r.name, f.name), "header")
                    field_entry: dict = {
                        "name": f.name,
                        "bits": [f.bit_offset, f.bit_width],
                    }
                    if src != "header":
                        field_entry["source"] = src
                    entry["fields"].append(field_entry)
            registers.append(entry)
        emit_yaml(
            {
                "name": typedef,
                "applies_to": ["WB32F10x", "WB32FQ95xx", "WB32F3G71xx"],
                "registers": registers,
            },
            peripherals_dir / f"{typedef}.yaml",
            header="Generated by wb32-data — shared peripheral register block.",
        )
    console.print(f"  [green]wrote[/] {len(header.register_blocks)} peripheral block(s) to data/peripherals/")
    if refman_disagreements:
        console.print("  [yellow]Header/manual bit-field disagreements (header kept):[/]")
        for d in refman_disagreements[:10]:
            console.print(f"    {d}")
        if len(refman_disagreements) > 10:
            console.print(f"    ... +{len(refman_disagreements) - 10} more")

    # ---- Interrupt family table ----
    interrupts_dir = out / "interrupts"
    interrupts_dir.mkdir(parents=True, exist_ok=True)
    emit_yaml(
        {
            "family": "WB32F10x",
            "applies_to": [
                "WB32F101xx", "WB32F102xx", "WB32F103xx", "WB32F104xx",
                "WB32F105xx", "WB32FQ95xx", "WB32F3G71xx",
            ],
            "core_exceptions": [
                {"number": i.number, "name": i.name, "description": i.description}
                for i in header.core_exceptions
            ],
            "vendor_irqs": [
                {"number": i.number, "name": i.name, "description": i.description}
                for i in header.interrupts
            ],
        },
        interrupts_dir / "WB32F10x.yaml",
        header="Generated by wb32-data — shared interrupt table for all WB32 families.",
    )
    console.print(f"  [green]wrote[/] data/interrupts/WB32F10x.yaml")


def _deep_merge(target: dict, overlay: dict) -> None:
    """Recursively merge ``overlay`` into ``target`` in-place.

    Lists in the overlay replace the target list outright. Dicts merge by key.
    Scalars in the overlay overwrite the target.
    """
    for key, value in overlay.items():
        if isinstance(value, dict) and isinstance(target.get(key), dict):
            _deep_merge(target[key], value)
        else:
            target[key] = value


@main.command()
@click.option("--vendor-lib", type=click.Path(exists=True, file_okay=False, path_type=Path), required=True)
@click.option("--vendor-docs", type=click.Path(exists=True, file_okay=False, path_type=Path), required=True)
@click.option("--chibios-contrib", type=click.Path(file_okay=False, path_type=Path), required=False)
@click.option("--repo-root", type=click.Path(exists=True, file_okay=False, path_type=Path), required=True)
@click.option("--data", "data_dir", type=click.Path(file_okay=False, path_type=Path), required=True)
@click.option("--chip", type=str, default=None, help="Validate only this chip")
@click.option("--fail-on-error/--no-fail-on-error", default=True)
def validate(
    vendor_lib: Path,
    vendor_docs: Path,
    chibios_contrib: Path | None,
    repo_root: Path,
    data_dir: Path,
    chip: str | None,
    fail_on_error: bool,
) -> None:
    """Cross-source validation. Writes a Markdown report per chip and exits non-zero if any errors are found."""
    parts = _load_parts_registry(repo_root)
    if chip:
        parts = [p for p in parts if p.get("name") == chip]
    if not parts:
        console.print(f"[red]ERROR[/]: no matching parts in registry")
        sys.exit(2)

    reports_dir = data_dir / "validation-reports"
    reports_dir.mkdir(parents=True, exist_ok=True)
    any_errors = False
    for part in parts:
        name = part["name"]
        console.print(f"[bold]{name}[/]: validating...")
        header, chibios, pdf_data, markdown, pin_val = _collect_sources(
            part,
            vendor_lib=vendor_lib,
            vendor_docs=vendor_docs,
            chibios_contrib=chibios_contrib,
            repo_root=repo_root,
        )
        report = run_validation(
            name, header, chibios, pdf_data, markdown, pin_val,
            chip_family=part.get("family"),
        )
        out_path = reports_dir / f"{name}.md"
        out_path.write_text(report.to_markdown(), encoding="utf-8")
        console.print(
            f"  errors: {len(report.errors)}, "
            f"warnings: {len(report.warnings)}, "
            f"info: {len(report.infos)} → {out_path.relative_to(repo_root)}"
        )
        if report.errors:
            any_errors = True

    if any_errors and fail_on_error:
        sys.exit(1)

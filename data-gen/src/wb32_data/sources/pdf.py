"""Extract structured data from Westberry vendor PDFs.

Trust level: secondary (below vendor headers and ChibiOS-Contrib port, above
the repository markdown). All vendor PDFs are text-based (LaTeX or Foxit
print-to-PDF), so `pdfplumber.extract_tables()` plus targeted text scans
handle them without OCR. Diagrams (block diagram, clock tree, package
drawings) are saved as PNG images via `pymupdf` for downstream reference.

Extraction is recipe-driven: a YAML file under `data-gen/recipes/` maps each
PDF to the chip(s) it documents and the per-section page ranges + extractors.
Results are cached under `data-gen/.pdf-cache/<sha256>.json` so iterating on
the recipe doesn't pay the parse cost twice.
"""

from __future__ import annotations

import hashlib
import json
import re
from dataclasses import asdict
from pathlib import Path
from typing import Any

import pdfplumber
import yaml

from wb32_data.models import (
    PdfData,
    PdfOrderingVariant,
    PdfPackage,
    PdfPinAfEntry,
    PdfPinDescription,
)


# ---------------------------------------------------------------------------
# Recipe schema
# ---------------------------------------------------------------------------


class Recipe(dict):
    """Thin wrapper around the parsed recipe dict.

    Schema (informal):

      pdf: <repo-relative-path>
      chip: <chip-name>
      sections:
        <name>:
          pages: [int, ...]     # 1-indexed
          extractor: <kind>     # optional: pin_description_table |
                                #           pin_af_matrix |
                                #           ordering_variants |
                                #           package_dimensions |
                                #           text
          save_images: bool     # optional
    """


def load_recipe(path: Path) -> Recipe:
    with path.open("r", encoding="utf-8") as fh:
        return Recipe(yaml.safe_load(fh))


# ---------------------------------------------------------------------------
# Extraction
# ---------------------------------------------------------------------------


class PdfExtractor:
    """Cached PDF extractor. One instance per (PDF, repo-root)."""

    def __init__(self, pdf_path: Path, repo_root: Path, cache_dir: Path):
        self.pdf_path = pdf_path.resolve()
        self.repo_root = repo_root.resolve()
        self.cache_dir = cache_dir
        self.cache_dir.mkdir(parents=True, exist_ok=True)
        self._sha = _hash_file(self.pdf_path)

    @property
    def sha256(self) -> str:
        return self._sha

    @property
    def cache_file(self) -> Path:
        return self.cache_dir / f"{self._sha}.json"

    def extract(self, recipe: Recipe, *, image_out_dir: Path | None = None) -> PdfData:
        """Apply a recipe to this PDF and return structured data."""
        cached = self._load_cache(recipe)
        if cached is not None:
            return cached

        try:
            rel_path = str(self.pdf_path.relative_to(self.repo_root))
        except ValueError:
            rel_path = str(self.pdf_path)
        data = PdfData(pdf_path=rel_path, sha256=self._sha)

        sections: dict[str, dict[str, Any]] = recipe.get("sections", {}) or {}

        with pdfplumber.open(self.pdf_path) as pdf:
            for section_name, spec in sections.items():
                pages = spec.get("pages") or []
                extractor = spec.get("extractor")
                save_images = bool(spec.get("save_images"))

                page_objs = [pdf.pages[p - 1] for p in pages if 1 <= p <= len(pdf.pages)]
                if not page_objs:
                    continue

                # Always retain raw text for downstream debugging.
                for p, page in zip(pages, page_objs):
                    data.raw_text_by_page[p] = page.extract_text() or ""

                if extractor == "pin_description_table":
                    data.pin_descriptions.extend(_extract_pin_descriptions(page_objs))
                elif extractor == "pin_af_matrix":
                    data.pin_af_matrix.extend(_extract_pin_af_matrix(page_objs))
                elif extractor == "ordering_variants":
                    data.ordering_variants.extend(_extract_ordering_variants(page_objs))
                elif extractor == "package_dimensions":
                    data.packages.extend(_extract_package_dimensions(page_objs))
                elif extractor in (None, "text"):
                    pass     # raw text already captured
                else:
                    data.notes.append(f"unknown extractor '{extractor}' for section '{section_name}'")

                if save_images and image_out_dir is not None:
                    saved = _save_page_images(self.pdf_path, pages, image_out_dir)
                    data.saved_images.extend(str(p) for p in saved)

        self._save_cache(recipe, data)
        return data

    # -----------------------------------------------------------------------
    # Cache helpers
    # -----------------------------------------------------------------------

    def _cache_key(self, recipe: Recipe) -> str:
        """Hash the recipe so we re-extract when the recipe changes."""
        recipe_json = json.dumps(recipe, sort_keys=True)
        return hashlib.sha256(recipe_json.encode("utf-8")).hexdigest()[:16]

    def _load_cache(self, recipe: Recipe) -> PdfData | None:
        key = self._cache_key(recipe)
        path = self.cache_dir / f"{self._sha}.{key}.json"
        if not path.exists():
            return None
        try:
            raw = json.loads(path.read_text("utf-8"))
        except (OSError, json.JSONDecodeError):
            return None
        return _pdf_data_from_dict(raw)

    def _save_cache(self, recipe: Recipe, data: PdfData) -> None:
        key = self._cache_key(recipe)
        path = self.cache_dir / f"{self._sha}.{key}.json"
        path.write_text(json.dumps(_pdf_data_to_dict(data), indent=2))


def _hash_file(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as fh:
        for chunk in iter(lambda: fh.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def _pdf_data_to_dict(d: PdfData) -> dict:
    return {
        "pdf_path": d.pdf_path,
        "sha256": d.sha256,
        "pin_descriptions": [asdict(p) for p in d.pin_descriptions],
        "pin_af_matrix": [asdict(p) for p in d.pin_af_matrix],
        "packages": [asdict(p) for p in d.packages],
        "ordering_variants": [asdict(p) for p in d.ordering_variants],
        "raw_text_by_page": {str(k): v for k, v in d.raw_text_by_page.items()},
        "saved_images": list(d.saved_images),
        "notes": list(d.notes),
    }


def _pdf_data_from_dict(d: dict) -> PdfData:
    return PdfData(
        pdf_path=d["pdf_path"],
        sha256=d["sha256"],
        pin_descriptions=[PdfPinDescription(**p) for p in d.get("pin_descriptions", [])],
        pin_af_matrix=[PdfPinAfEntry(**p) for p in d.get("pin_af_matrix", [])],
        packages=[PdfPackage(
            name=p["name"],
            body_mm=tuple(p["body_mm"]) if p.get("body_mm") else None,
            pitch_mm=p.get("pitch_mm"),
            pin_count=p.get("pin_count"),
        ) for p in d.get("packages", [])],
        ordering_variants=[PdfOrderingVariant(**p) for p in d.get("ordering_variants", [])],
        raw_text_by_page={int(k): v for k, v in d.get("raw_text_by_page", {}).items()},
        saved_images=list(d.get("saved_images", [])),
        notes=list(d.get("notes", [])),
    )


# ---------------------------------------------------------------------------
# Pin Description Table
#
# Format observed in WB32FQ95xC datasheet (page 12+13):
#   ['PinNo.', None, 'Pin\nName', 'Type', 'Main\nFunction', 'AlternateFunction', 'Analog\nFunction']
#   ['LQFP48', 'LQFP64', None, None, None, None, None]
#   ['1', '1', 'VBAT', 'S', 'VBAT', '', '']
#   ['2', '2', 'PC13', 'I/O', 'PC13', 'TAMPER/RTC', '']
# ---------------------------------------------------------------------------


_PIN_DESCRIPTION_HEADER_TOKENS = {"PinNo.", "Pin", "Name", "Type", "Main", "Function"}


def _extract_pin_descriptions(pages) -> list[PdfPinDescription]:
    rows: list[PdfPinDescription] = []
    for page in pages:
        for tbl in page.extract_tables():
            if not _is_pin_description_table(tbl):
                continue
            rows.extend(_parse_pin_description_table(tbl))
    return rows


def _is_pin_description_table(tbl: list[list[str | None]]) -> bool:
    if not tbl or not tbl[0]:
        return False
    flat = " ".join(cell or "" for cell in tbl[0])
    return "PinNo" in flat or "Pin\nName" in flat or all(
        token in flat for token in ("Pin", "Function")
    )


def _parse_pin_description_table(tbl: list[list[str | None]]) -> list[PdfPinDescription]:
    """Skip the two header rows (which span two physical rows in the datasheet)
    and parse the remaining rows as Pin No. (LQFP48, LQFP64) | Pin Name | Type
    | Main Function | Alternate Function | Analog Function.
    """
    rows: list[PdfPinDescription] = []
    # Find the first data row — the one whose first cell looks like an integer.
    for r in tbl:
        if not r or not r[0]:
            continue
        first = (r[0] or "").strip()
        if not first.isdigit():
            continue
        # Expect 7 columns: LQFP48, LQFP64, Name, Type, Main, AF, Analog
        if len(r) < 7:
            continue
        try:
            pin48 = int((r[0] or "").strip())
        except ValueError:
            pin48 = None
        try:
            pin64 = int((r[1] or "").strip())
        except ValueError:
            pin64 = None
        rows.append(
            PdfPinDescription(
                pin_number_lqfp48=pin48,
                pin_number_lqfp64=pin64,
                pin_name=(r[2] or "").strip(),
                pin_type=(r[3] or "").strip(),
                main_function=(r[4] or "").strip(),
                alternate_functions=_split_slash((r[5] or "").strip()),
                analog_functions=_split_slash((r[6] or "").strip()),
            )
        )
    return rows


def _split_slash(s: str) -> list[str]:
    if not s:
        return []
    return [p.strip() for p in re.split(r"[/\n]", s) if p.strip()]


# ---------------------------------------------------------------------------
# Pin × AF mux matrix
#
# Format observed (WB32FQ95xC pages 13-14):
#   ['Port', 'AF0', 'AF1', 'AF2', 'AF3', 'AF4', 'AF5', 'AF6', 'AF7']
#   [None,   'SYS', 'TIM1/2', 'TIM3/4', 'I2S', 'I2C', 'SPI(M)', 'SPI(S)', 'UART']
#   ['PA0',  'WKUP', 'TIM2_CH1\nTIM2_ETR', '', '', '', '', '', 'UART2_CTS']
# ---------------------------------------------------------------------------


def _extract_pin_af_matrix(pages) -> list[PdfPinAfEntry]:
    out: list[PdfPinAfEntry] = []
    for page in pages:
        for tbl in page.extract_tables():
            if not _is_pin_af_table(tbl):
                continue
            out.extend(_parse_pin_af_table(tbl))
    return out


def _is_pin_af_table(tbl: list[list[str | None]]) -> bool:
    if not tbl or not tbl[0]:
        return False
    header = [(c or "").strip() for c in tbl[0]]
    return header[:2] == ["Port", "AF0"] and "AF7" in header


def _parse_pin_af_table(tbl: list[list[str | None]]) -> list[PdfPinAfEntry]:
    out: list[PdfPinAfEntry] = []
    af_columns = [(c or "").strip() for c in tbl[0]]
    for row in tbl[1:]:
        if not row or not row[0]:
            continue
        port = (row[0] or "").strip()
        if not port.startswith("P") or len(port) < 3:
            continue
        # row[1..8] correspond to AF0..AF7
        for af_index in range(8):
            col_idx = af_index + 1
            if col_idx >= len(row):
                break
            cell = (row[col_idx] or "").strip()
            if not cell:
                continue
            # The AF column header is at af_columns[col_idx] = "AF<n>"
            # — sanity check that we're aligned, otherwise skip.
            try:
                if af_columns[col_idx] != f"AF{af_index}":
                    continue
            except IndexError:
                continue
            out.append(PdfPinAfEntry(pin=port, af=af_index, signal=cell))
    return out


# ---------------------------------------------------------------------------
# Ordering variants (Tab 7.0-1) — sparse table with one column per part suffix.
# ---------------------------------------------------------------------------


def _extract_ordering_variants(pages) -> list[PdfOrderingVariant]:
    """Best-effort extraction.

    The WB32FQ95xC ordering page combines a single-row decode diagram with a
    matrix that's hard to parse cleanly. We capture raw text from the page so
    downstream code can extract specific facts; the structured rows return
    empty unless a clear table is found.
    """
    out: list[PdfOrderingVariant] = []
    for page in pages:
        for tbl in page.extract_tables():
            if not tbl or not tbl[0]:
                continue
            header = [(c or "").strip() for c in tbl[0]]
            # A real variant table will have at least one cell starting with "WB32".
            for row in tbl[1:]:
                first = (row[0] or "").strip() if row else ""
                if not first.upper().startswith("WB32"):
                    continue
                attributes = {
                    header[i] or f"col{i}": (row[i] or "").strip()
                    for i in range(min(len(header), len(row)))
                }
                out.append(PdfOrderingVariant(part_number=first, raw_attributes=attributes))
    return out


# ---------------------------------------------------------------------------
# Package dimensions (lightweight — captures package name + pin count + pitch).
# ---------------------------------------------------------------------------


_PACKAGE_LINE_RE = re.compile(
    r"LQFP(?P<pins>\d{2,3}?)(?P<a>\d{1,2}(?:\.\d+)?)\s*[xX]\s*(?P<b>\d{1,2}(?:\.\d+)?)\s*mm",
)
_PITCH_RE = re.compile(r"(?P<pitch>\d+\.\d+|0?\.\d+)\s*mm\s*pitch", re.IGNORECASE)


def _extract_package_dimensions(pages) -> list[PdfPackage]:
    """Parse 'Fig 6.1-1 LQFP64 10x10mm, 0.5mm pitch ...' style lines."""
    out: dict[str, PdfPackage] = {}
    for page in pages:
        text = page.extract_text() or ""
        for line in text.split("\n"):
            m = _PACKAGE_LINE_RE.search(line)
            if not m:
                continue
            pins = int(m.group("pins"))
            body = (float(m.group("a")), float(m.group("b")))
            pitch_m = _PITCH_RE.search(line)
            pitch = float(pitch_m.group("pitch")) if pitch_m else None
            name = f"LQFP{pins}"
            if name not in out:
                out[name] = PdfPackage(name=name, body_mm=body, pitch_mm=pitch, pin_count=pins)
    return list(out.values())


# ---------------------------------------------------------------------------
# Image extraction via pymupdf
# ---------------------------------------------------------------------------


def _save_page_images(pdf_path: Path, pages: list[int], out_dir: Path) -> list[Path]:
    """Render each requested page to a PNG using pymupdf. We rasterize the
    whole page rather than picking out embedded image objects: the vendor's
    diagrams are vector drawings, not raster images, so embedded-image
    extraction returns nothing useful.
    """
    import fitz                   # pymupdf

    out_dir.mkdir(parents=True, exist_ok=True)
    saved: list[Path] = []
    with fitz.open(str(pdf_path)) as doc:
        for page_no in pages:
            if not (1 <= page_no <= doc.page_count):
                continue
            page = doc[page_no - 1]
            pix = page.get_pixmap(dpi=200)
            target = out_dir / f"{pdf_path.stem}-p{page_no:03d}.png"
            pix.save(str(target))
            saved.append(target)
    return saved

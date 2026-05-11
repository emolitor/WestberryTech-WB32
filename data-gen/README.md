# wb32-data

Generator that produces machine-readable data for Westberry Tech WB32
microcontroller families. Inspired by
[stm32-data](https://github.com/embassy-rs/stm32-data) and
[ch32-data](https://github.com/ch32-rs/ch32-data).

## Sources, in trust order

1. **Vendor CMSIS + StdPeriph headers** — `vendor-lib/WB32F10x_StdPeriph_Lib_V0.1.10/`
2. **ChibiOS-Contrib WB32 port** — external checkout, default
   `$HOME/Development/Personal/WB32/ChibiOS-Contrib`, override with
   `CHIBIOS_CONTRIB=...`. Covers `WB32FQ95xx` and `WB32F3G71xx` only.
3. **Vendor PDFs** — `vendor-docs/datasheets/`, `vendor-docs/reference-manual/`,
   `vendor-docs/application-notes/`. Extracted with `pdfplumber` +
   `pymupdf`, with Claude Sonnet vision fallback for hard pages.
4. **Markdown docs** — `docs/chip-overview.md`. Validated against 1–3, never
   overrides them.

Hand-curated overlays live at `data/_overrides/<chip>.yaml` and patch the
generated output for facts that no source captures cleanly.

## Output

```
data/
├── chips/            per-chip YAML (the deliverable)
├── peripherals/      shared peripheral register/bitfield definitions
├── interrupts/       interrupt tables shared across chips
├── diagrams/         PNG images extracted from PDFs
├── _overrides/       hand-curated overlays
└── validation-reports/  cross-source disagreement reports
```

## Usage

```bash
make venv          # create Python venv
make install       # install deps
make data          # generate all chips
make data CHIP=WB32FQ95xC   # one chip
make validate      # cross-source validation report
```

Requires `ANTHROPIC_API_KEY` in the environment for PDF vision fallback. If
unset the generator skips the vision step and emits a warning.

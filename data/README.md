# WB32 Data

Machine-readable data for the Westberry Tech WB32 microcontroller families,
emitted by the generator in [`../data-gen/`](../data-gen/).

## Structure

```
data/
├── chips/                 per-chip YAML — the primary deliverable
├── peripherals/           shared peripheral register/bitfield definitions
├── interrupts/            interrupt tables shared across chips
├── diagrams/<chip>/       PNGs extracted from PDFs, with captions.yaml
├── _overrides/            hand-curated overlays applied on top of generated data
└── validation-reports/    Markdown reports listing cross-source disagreements
```

## Schema

See [`schema.md`](schema.md) for the full per-chip YAML schema.

## Regenerating

From the repository root:

```bash
make -C data-gen data
```

Re-running is idempotent; chips/, peripherals/, and interrupts/ are
regenerated from source. Files in `_overrides/` are user-authored and never
overwritten.

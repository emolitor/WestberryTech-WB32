"""Golden snapshot tests.

For each chip in recipes/parts.yaml, regenerate the chip YAML and validation
report into a temp directory, then diff against the checked-in files under
``data/``. The committed files in the repo ARE the snapshots — when a
deliberate change lands, the developer re-runs ``make data && make validate``
and commits the new outputs alongside the code change.

Catches:
- Parser regressions silently changing extracted fields.
- Merge order or key-shape changes.
- Validator false-positive / false-negative regressions.

Skipped when ChibiOS-Contrib is unavailable — the per-chip merge step reads
chibios capability flags for FQ95xx / F3G71xx and the validator compares
header IRQs against the ChibiOS ISR table.
"""

from __future__ import annotations

import subprocess
import sys
from pathlib import Path

import pytest
import yaml


_PARTS = [
    "WB32F101xC",
    "WB32F102xC",
    "WB32F103xC",
    "WB32F104RC",
    "WB32F105RC",
    "WB32FQ95xC",
    "WB32F3G71xx",
]


def _strip_volatile_fields(chip_dict: dict) -> dict:
    """Drop fields that change every run (timestamp, in-tree path hashes)."""
    sources = chip_dict.get("sources")
    if isinstance(sources, dict):
        sources.pop("generated_at", None)
    return chip_dict


@pytest.fixture(scope="module")
def regenerated(repo_root: Path, chibios_contrib_path: Path | None, tmp_path_factory) -> Path:
    """Run ``generate`` + ``validate`` once for the whole module."""
    if chibios_contrib_path is None:
        pytest.skip("ChibiOS-Contrib WB32 port not on disk")

    tmp = tmp_path_factory.mktemp("wb32-golden")
    common = [
        sys.executable, "-m", "wb32_data",
        "--vendor-lib", str(repo_root / "vendor-lib" / "WB32F10x_StdPeriph_Lib_V0.1.10"),
        "--vendor-docs", str(repo_root / "vendor-docs"),
        "--chibios-contrib", str(chibios_contrib_path),
        "--repo-root", str(repo_root),
    ]
    # Generate
    gen = subprocess.run(
        [sys.executable, "-m", "wb32_data", "generate",
         "--vendor-lib", str(repo_root / "vendor-lib" / "WB32F10x_StdPeriph_Lib_V0.1.10"),
         "--vendor-docs", str(repo_root / "vendor-docs"),
         "--chibios-contrib", str(chibios_contrib_path),
         "--repo-root", str(repo_root),
         "--out", str(tmp)],
        capture_output=True, text=True, cwd=str(repo_root / "data-gen"),
    )
    assert gen.returncode == 0, f"generate failed: {gen.stderr}\n{gen.stdout}"

    # Re-populate the _overrides folder in tmp so the same overlays apply.
    src_overrides = repo_root / "data" / "_overrides"
    dst_overrides = tmp / "_overrides"
    if src_overrides.is_dir():
        dst_overrides.mkdir(exist_ok=True)
        for f in src_overrides.glob("*.yaml"):
            (dst_overrides / f.name).write_text(f.read_text(encoding="utf-8"), encoding="utf-8")
        # Regenerate so overrides actually get applied to the chip dicts.
        gen2 = subprocess.run(
            [sys.executable, "-m", "wb32_data", "generate",
             "--vendor-lib", str(repo_root / "vendor-lib" / "WB32F10x_StdPeriph_Lib_V0.1.10"),
             "--vendor-docs", str(repo_root / "vendor-docs"),
             "--chibios-contrib", str(chibios_contrib_path),
             "--repo-root", str(repo_root),
             "--out", str(tmp)],
            capture_output=True, text=True, cwd=str(repo_root / "data-gen"),
        )
        assert gen2.returncode == 0, f"generate (with overrides) failed: {gen2.stderr}\n{gen2.stdout}"

    # Validate (writes reports under tmp/validation-reports/).
    val = subprocess.run(
        [sys.executable, "-m", "wb32_data", "validate",
         "--vendor-lib", str(repo_root / "vendor-lib" / "WB32F10x_StdPeriph_Lib_V0.1.10"),
         "--vendor-docs", str(repo_root / "vendor-docs"),
         "--chibios-contrib", str(chibios_contrib_path),
         "--repo-root", str(repo_root),
         "--data", str(tmp),
         "--no-fail-on-error"],
        capture_output=True, text=True, cwd=str(repo_root / "data-gen"),
    )
    assert val.returncode == 0, f"validate failed: {val.stderr}\n{val.stdout}"
    return tmp


@pytest.mark.requires_chibios
@pytest.mark.parametrize("chip", _PARTS)
def test_chip_yaml_matches_golden(repo_root: Path, regenerated: Path, chip: str) -> None:
    fresh_path = regenerated / "chips" / f"{chip}.yaml"
    golden_path = repo_root / "data" / "chips" / f"{chip}.yaml"
    assert fresh_path.exists(), f"missing fresh output: {fresh_path}"
    assert golden_path.exists(), f"missing golden: {golden_path}"

    fresh = _strip_volatile_fields(yaml.safe_load(fresh_path.read_text("utf-8")))
    golden = _strip_volatile_fields(yaml.safe_load(golden_path.read_text("utf-8")))
    assert fresh == golden, (
        f"{chip} YAML drifted from golden. Run `make data` and commit the diff."
    )


@pytest.mark.requires_chibios
@pytest.mark.parametrize("chip", _PARTS)
def test_validation_report_matches_golden(repo_root: Path, regenerated: Path, chip: str) -> None:
    fresh_path = regenerated / "validation-reports" / f"{chip}.md"
    golden_path = repo_root / "data" / "validation-reports" / f"{chip}.md"
    assert fresh_path.exists(), f"missing fresh report: {fresh_path}"
    assert golden_path.exists(), f"missing golden report: {golden_path}"

    fresh = fresh_path.read_text("utf-8")
    golden = golden_path.read_text("utf-8")
    assert fresh == golden, (
        f"{chip} validation report drifted from golden. "
        f"Run `make validate` and commit the diff."
    )

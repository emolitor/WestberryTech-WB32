"""Shared pytest fixtures for wb32-data tests."""

from __future__ import annotations

import os
from pathlib import Path

import pytest


@pytest.fixture(scope="session")
def repo_root() -> Path:
    """Absolute path to the repo root (parent of data-gen/)."""
    return Path(__file__).resolve().parents[2]


@pytest.fixture(scope="session")
def chibios_contrib_path() -> Path | None:
    """Resolve the ChibiOS-Contrib path, honouring $CHIBIOS_CONTRIB, else None."""
    env = os.environ.get("CHIBIOS_CONTRIB")
    if env:
        p = Path(env)
        return p if p.is_dir() else None
    default = Path.home() / "Development" / "Personal" / "WB32" / "ChibiOS-Contrib"
    return default if default.is_dir() else None


def pytest_collection_modifyitems(config, items) -> None:
    """Auto-skip tests marked `requires_chibios` when the port isn't on disk."""
    env = os.environ.get("CHIBIOS_CONTRIB")
    if env:
        present = Path(env).is_dir()
    else:
        present = (Path.home() / "Development" / "Personal" / "WB32" / "ChibiOS-Contrib").is_dir()
    if present:
        return
    skip_marker = pytest.mark.skip(reason="ChibiOS-Contrib WB32 port not on disk")
    for item in items:
        if "requires_chibios" in item.keywords:
            item.add_marker(skip_marker)

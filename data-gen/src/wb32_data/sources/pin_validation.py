"""Parse examples/pin-validation/pin_defs.h — the in-tree silicon-validation AF table.

Trust level: documentation-equivalent (its claims are hand-written by the same
author who wrote the markdown, but the file is checked against real silicon
via the validation test rig). Both this file and the markdown are *audited*
against the vendor datasheets, never trusted above them.
"""

from __future__ import annotations

import re
from dataclasses import dataclass, field
from pathlib import Path


@dataclass(frozen=True)
class PinDefEntry:
    """One row of the af_table[] array in pin_defs.h."""

    port: str        # "GPIOA" → port letter "A"
    pin: int         # 0..15
    af: int          # 0..7
    descriptor: str  # the "PA5_AF4_QSPI_SCK" style label

    @property
    def pin_name(self) -> str:
        return f"P{self.port[-1]}{self.pin}"

    @property
    def signal(self) -> str:
        """Extract the trailing signal portion of the descriptor.

        ``PA5_AF4_QSPI_SCK`` → ``QSPI_SCK``
        ``PA0_AF1_TIM2_CH1_ETR`` → ``TIM2_CH1_ETR``
        """
        # The first two segments are pin name and AF identifier.
        parts = self.descriptor.split("_", 2)
        if len(parts) == 3:
            return parts[2]
        return self.descriptor


@dataclass
class PinValidationData:
    """Output of parsing pin_defs.h."""

    source_chip: str = ""        # what the file claims to document (e.g. "WB32F104xx")
    entries: list[PinDefEntry] = field(default_factory=list)


_ENTRY_RE = re.compile(
    r"""
    \{\s*
      (?P<port>GPIO[A-D])\s*,\s*
      (?P<pin>\d{1,2})\s*,\s*
      (?P<af>\d)\s*,\s*
      "(?P<desc>[^"]+)"
    \s*\}
    """,
    re.VERBOSE,
)
_SOURCE_RE = re.compile(r"from\s+(?P<doc>DS\d+|tab[a-z\s]*\d[.\-\d]*).*?\((?P<chip>WB32[A-Z0-9]+x*)\)", re.IGNORECASE)


def parse_pin_defs(path: Path) -> PinValidationData:
    text = path.read_text(encoding="utf-8", errors="replace")
    data = PinValidationData()

    src = _SOURCE_RE.search(text)
    if src is not None:
        data.source_chip = src.group("chip")

    for m in _ENTRY_RE.finditer(text):
        data.entries.append(
            PinDefEntry(
                port=m.group("port"),
                pin=int(m.group("pin")),
                af=int(m.group("af")),
                descriptor=m.group("desc"),
            )
        )
    return data

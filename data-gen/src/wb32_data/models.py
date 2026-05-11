"""Dataclass models for the intermediate representation.

These are the structures emitted by source parsers (headers/chibios/pdf/md) and
merged into the final per-chip schema. They do NOT include every field of the
output YAML schema — they're the union of facts a source can produce.
"""

from __future__ import annotations

from dataclasses import dataclass, field
from typing import Literal, Optional


# ---------------------------------------------------------------------------
# Interrupts
# ---------------------------------------------------------------------------


@dataclass(frozen=True)
class Interrupt:
    number: int
    name: str
    description: str = ""


# ---------------------------------------------------------------------------
# Registers
# ---------------------------------------------------------------------------


@dataclass(frozen=True)
class RegisterField:
    name: str
    bit_offset: int
    bit_width: int
    description: str = ""


@dataclass(frozen=True)
class Register:
    name: str
    offset: int                  # bytes from peripheral base
    size: int                    # bits (32, 16, 8)
    access: Literal["r", "w", "rw"]
    description: str = ""
    fields: tuple[RegisterField, ...] = field(default_factory=tuple)


@dataclass(frozen=True)
class RegisterBlock:
    """A register layout (typedef struct) shared by one or more peripheral instances."""

    name: str                    # e.g. "GPIO_TypeDef" or block id "GPIO_v1"
    registers: tuple[Register, ...]


# ---------------------------------------------------------------------------
# Peripherals
# ---------------------------------------------------------------------------


@dataclass(frozen=True)
class PeripheralInstance:
    """A concrete instance like GPIOA, TIM1, UART2."""

    name: str
    base_address: int
    typedef: str                 # the C TypeDef name; resolves to a RegisterBlock


# ---------------------------------------------------------------------------
# Memory map
# ---------------------------------------------------------------------------


@dataclass(frozen=True)
class MemoryRegion:
    name: str
    base: int
    description: str = ""


# ---------------------------------------------------------------------------
# Chip capabilities (from ChibiOS registry)
# ---------------------------------------------------------------------------


@dataclass(frozen=True)
class ChibiosCapability:
    """A WB32_HAS_* or WB32_*_NUM_* style fact from wb32_registry.h."""

    key: str                     # e.g. "WB32_HAS_USB1_DMA" or "WB32_DMAC1_NUM_CHANNELS"
    value: str                   # e.g. "TRUE", "FALSE", "3"


# ---------------------------------------------------------------------------
# Pin AF mapping (from markdown, validated against headers/chibios/PDF)
# ---------------------------------------------------------------------------


@dataclass(frozen=True)
class PinSignal:
    pin: str                     # e.g. "PA0"
    af: Optional[int]            # 0-7, or None for analog
    signal: str                  # e.g. "TIM2_CH1_ETR"
    peripheral: str              # e.g. "TIM2"
    kind: Literal["af", "analog", "default", "alternate"] = "af"
    notes: str = ""
    only_package: Optional[str] = None   # e.g. "LQFP64" if pin only present on bigger package
    applies_to: tuple[str, ...] = ()     # chip families this signal applies to; empty = all


# ---------------------------------------------------------------------------
# Source bundles — what each parser returns
# ---------------------------------------------------------------------------


@dataclass
class HeaderData:
    """Output of parsing vendor CMSIS + StdPeriph headers."""

    interrupts: list[Interrupt] = field(default_factory=list)
    core_exceptions: list[Interrupt] = field(default_factory=list)
    register_blocks: dict[str, RegisterBlock] = field(default_factory=dict)
    peripherals: dict[str, PeripheralInstance] = field(default_factory=dict)
    memory: list[MemoryRegion] = field(default_factory=list)
    core_info: dict[str, str | int | bool] = field(default_factory=dict)
    constants: dict[str, int] = field(default_factory=dict)


@dataclass(frozen=True)
class IsrVector:
    """ISR name → NVIC vector-table offset + IRQ number, from wb32_isr.h."""

    name: str                    # e.g. "WWDG", "TIM1_UP", "USB1_DMA"
    vector_offset: int           # bytes from start of vector table (e.g. 0x40)
    irq_number: int              # NVIC IRQ index (e.g. 0)


@dataclass(frozen=True)
class ClockEnable:
    """A peripheral's clock-enable bit, derived from rccEnable<Name>() macros."""

    peripheral: str              # e.g. "QSPI", "I2C1", "TIM2"
    bus: Literal["AHB", "AHB2", "APB1", "APB2"]
    field: str                   # ENR field symbol, e.g. "RCC_APB1ENR_QSPIEN"


@dataclass(frozen=True)
class AhbPeripheralMask:
    """Raw RCC_AHBPeriph_<NAME> mask from wb32_rcc.h."""

    name: str                    # e.g. "USB", "DMAC1Bridge"
    mask: int


@dataclass(frozen=True)
class DmaHandshake:
    """A DMA peripheral handshake interface number, from LLD/DMAv1/wb32_dma.h."""

    controller: Literal["DMAC1", "DMAC2"]
    peripheral: str              # e.g. "UART1", "ADC_Regular", "TIM2_CH1"
    direction: Optional[Literal["RX", "TX"]]    # None for non-directional (e.g. TIM channels)
    handshake_number: int


@dataclass(frozen=True)
class IrqPriority:
    """Default ChibiOS IRQ priority for a peripheral instance."""

    driver: str                  # e.g. "ADC", "UART", "GPT", "SPI", "I2C"
    instance: str                # e.g. "ADC1", "UART1", "TIM2"
    priority: int


@dataclass
class ChibiosData:
    """Output of parsing ChibiOS-Contrib WB32 port."""

    chip_family: str             # e.g. "WB32FQ95xx"
    capabilities: dict[str, str] = field(default_factory=dict)
    pin_signals: list[PinSignal] = field(default_factory=list)
    isr_vectors: list[IsrVector] = field(default_factory=list)
    clock_enables: list[ClockEnable] = field(default_factory=list)
    ahb_peripheral_masks: list[AhbPeripheralMask] = field(default_factory=list)
    dma_handshakes: list[DmaHandshake] = field(default_factory=list)
    irq_priorities: list[IrqPriority] = field(default_factory=list)
    extra_register_fields: dict[tuple[str, str], list[RegisterField]] = field(default_factory=dict)


@dataclass
class MarkdownData:
    """Output of parsing repo markdown docs."""

    pin_signals: list[PinSignal] = field(default_factory=list)


# ---------------------------------------------------------------------------
# PDF-sourced records
# ---------------------------------------------------------------------------


@dataclass(frozen=True)
class PdfPinDescription:
    """One row from the datasheet's Pin Description table."""

    pin_number_lqfp48: Optional[int]
    pin_number_lqfp64: Optional[int]
    pin_name: str
    pin_type: str               # "I/O", "I", "S" (supply), "Reset", etc.
    main_function: str
    alternate_functions: list[str]    # split from "TIM4_CH1/I2C1_SCL/..."
    analog_functions: list[str]


@dataclass(frozen=True)
class PdfPinAfEntry:
    """One Port×AF cell from the datasheet AF mux matrix."""

    pin: str                    # "PA0"
    af: int                     # 0..7
    signal: str                 # cell text; may contain newlines for paired signals


@dataclass(frozen=True)
class PdfPackage:
    name: str                   # "LQFP64", "LQFP48"
    body_mm: Optional[tuple[float, float]] = None    # (length, width)
    pitch_mm: Optional[float] = None
    pin_count: Optional[int] = None


@dataclass(frozen=True)
class PdfOrderingVariant:
    """One row of the ordering variant matrix."""

    part_number: str            # e.g. "WB32FQ95RCT6"
    raw_attributes: dict[str, str] = field(default_factory=dict)


@dataclass
class PdfData:
    pdf_path: str                                          # repo-relative
    sha256: str
    pin_descriptions: list[PdfPinDescription] = field(default_factory=list)
    pin_af_matrix: list[PdfPinAfEntry] = field(default_factory=list)
    packages: list[PdfPackage] = field(default_factory=list)
    ordering_variants: list[PdfOrderingVariant] = field(default_factory=list)
    raw_text_by_page: dict[int, str] = field(default_factory=dict)
    saved_images: list[str] = field(default_factory=list)  # repo-relative paths
    notes: list[str] = field(default_factory=list)

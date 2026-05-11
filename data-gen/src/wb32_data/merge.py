"""Merge source bundles into a single chip dict, ready for YAML emission.

Trust order (higher → lower): vendor headers + ChibiOS-Contrib (peer ground
truth) > PDF > markdown. The merge logic here is:

* Memory map, IRQ list, peripheral base addresses, and register-block typedef
  names come from the vendor header.
* DMA handshake numbers, IRQ vector mapping, peripheral clock-enable info,
  and IRQ priorities come from the ChibiOS port (if available).
* Pin descriptions, AF mux entries, and package dimensions come from the PDF.
* Markdown is never inserted into output — its facts are validated separately
  and reported on disagreement.
"""

from __future__ import annotations

from collections import defaultdict
from dataclasses import asdict
from datetime import datetime, timezone
from typing import Any

from wb32_data import __version__
from wb32_data.models import (
    ChibiosData,
    HeaderData,
    MarkdownData,
    PdfData,
)


def merge_chip(
    part: dict,
    header: HeaderData,
    chibios: ChibiosData | None,
    pdf: PdfData | None,
    markdown: MarkdownData | None,
) -> dict[str, Any]:
    """Compose the final dict representing one chip.

    The dict shape mirrors ``data/schema.md``. The serializer is responsible
    only for turning this into YAML — no further transformation.
    """
    name = part["name"]
    family = part["family"]
    package = part.get("package", "")
    flash_bytes = part.get("flash_bytes")
    ram_bytes = part.get("ram_bytes")

    out: dict[str, Any] = {
        "schema_version": 1,
        "name": name,
        "family": family,
        "package": package,
        "flash_size": flash_bytes,
        "ram_size": ram_bytes,
        "core": _core_block(header),
        "sources": _sources_block(part, header, chibios, pdf),
        "memory": _memory_block(header, flash_bytes, ram_bytes),
        "interrupts": _interrupts_block(header, chibios),
        "dma": _dma_block(header, chibios),
        "peripherals": _peripherals_block(header, chibios),
        "pins": _pins_block(pdf, package),
        "clocks": _clocks_block(header),
        "flash": _flash_block(header),
    }
    return out


# ---------------------------------------------------------------------------
# Top-level blocks
# ---------------------------------------------------------------------------


def _core_block(h: HeaderData) -> dict[str, Any]:
    core = {
        "name": "cortex-m3",
        "revision": h.core_info.get("core_revision", ""),
        "mpu": bool(h.core_info.get("mpu_present", False)),
        "fpu": False,
        "nvic_prio_bits": int(h.core_info.get("nvic_prio_bits", 4)),
    }
    return core


def _sources_block(part: dict, h: HeaderData, c: ChibiosData | None, p: PdfData | None) -> dict[str, Any]:
    inputs = []
    inputs.append({"kind": "header", "name": "vendor CMSIS wb32f10x.h"})
    if c is not None and c.capabilities:
        inputs.append({"kind": "chibios", "chip_family": c.chip_family})
    if p is not None:
        inputs.append({"kind": "pdf", "path": p.pdf_path, "sha256": p.sha256[:16]})
    return {
        "generated_by": f"wb32-data {__version__}",
        "generated_at": datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ"),
        "inputs": inputs,
    }


def _memory_block(h: HeaderData, flash_bytes: int | None, ram_bytes: int | None) -> list[dict[str, Any]]:
    out: list[dict[str, Any]] = []
    by_name = {r.name: r for r in h.memory}
    flash = by_name.get("flash")
    if flash is not None:
        out.append({
            "name": "flash",
            "base": flash.base,
            "size": flash_bytes,
            "kind": "flash",
            "access": "rx",
        })
    sram = by_name.get("sram")
    if sram is not None:
        out.append({
            "name": "sram",
            "base": sram.base,
            "size": ram_bytes,
            "kind": "ram",
            "access": "rwx",
        })
    for extra_name in ("sram_bitband", "peripheral_bitband", "peripherals"):
        r = by_name.get(extra_name)
        if r is not None:
            out.append({
                "name": r.name,
                "base": r.base,
                "kind": "alias" if "bitband" in r.name else "mmio",
            })
    return out


def _interrupts_block(h: HeaderData, c: ChibiosData | None) -> list[dict[str, Any]]:
    # ChibiOS gives us the vector-table offset; the header gives us names + numbers.
    vector_by_num: dict[int, int] = {}
    if c is not None:
        for v in c.isr_vectors:
            vector_by_num[v.irq_number] = v.vector_offset
    return [
        {
            "number": i.number,
            "name": i.name,
            "description": i.description,
            **({"vector_offset": vector_by_num[i.number]} if i.number in vector_by_num else {}),
        }
        for i in h.interrupts
    ]


def _dma_block(h: HeaderData, c: ChibiosData | None) -> dict[str, Any]:
    controllers: list[dict[str, Any]] = []
    for name, peripheral in sorted(h.peripherals.items()):
        if name in ("DMAC1", "DMAC2"):
            controllers.append({
                "name": name,
                "base": peripheral.base_address,
                "num_channels": _from_chibios_int(c, f"WB32_{name}_NUM_CHANNELS", default=3),
                "irq": name,
            })
    if "USB" in h.peripherals:
        controllers.append({
            "name": "USB_DMA",
            "base": h.peripherals["USB"].base_address,
            "num_channels": 0,
            "irq": "USB_DMA",
            "notes": "Dedicated USB DMA master, separate from DMAC1/2",
        })

    handshakes_by_controller: dict[str, list[dict[str, Any]]] = defaultdict(list)
    if c is not None:
        for h_hand in c.dma_handshakes:
            row: dict[str, Any] = {
                "peripheral": h_hand.peripheral,
                "handshake": h_hand.handshake_number,
            }
            if h_hand.direction is not None:
                row["direction"] = h_hand.direction
            handshakes_by_controller[h_hand.controller].append(row)

    out: dict[str, Any] = {
        "controllers": controllers,
        "features": {
            "lli_supported": False,
            "notes": [
                "DMA peripheral handshake mapping differs from STM32F10x",
                "LLP (linked-list pointer) register is reserved on WB32 — use auto-reload or ISR chaining",
            ],
        },
    }
    if handshakes_by_controller:
        out["handshakes"] = {k: v for k, v in sorted(handshakes_by_controller.items())}
    return out


def _from_chibios_int(c: ChibiosData | None, key: str, default: int) -> int:
    if c is None:
        return default
    raw = c.capabilities.get(key)
    try:
        return int(raw) if raw is not None else default
    except ValueError:
        return default


def _peripherals_block(h: HeaderData, c: ChibiosData | None) -> list[dict[str, Any]]:
    """Emit one entry per peripheral instance, joined with ChibiOS-derived
    extras where available."""
    clock_enable_by_periph = {ce.peripheral: ce for ce in c.clock_enables} if c is not None else {}
    irq_priority_by_instance = {p.instance: p for p in c.irq_priorities} if c is not None else {}

    # Collect IRQ vectors per peripheral name prefix.
    irqs_by_peripheral: dict[str, list[dict[str, Any]]] = defaultdict(list)
    for i in h.interrupts:
        # Map IRQ name to peripheral instance heuristically.
        target = _irq_to_peripheral(i.name, h.peripherals)
        if target:
            irqs_by_peripheral[target].append({
                "name": i.name,
                "number": i.number,
                **({"signal": _irq_signal(i.name)} if _irq_signal(i.name) else {}),
            })

    out: list[dict[str, Any]] = []
    for name, peripheral in sorted(h.peripherals.items()):
        entry: dict[str, Any] = {
            "name": name,
            "kind": _peripheral_kind(name, peripheral.typedef),
            "block": peripheral.typedef,
            "base": peripheral.base_address,
        }
        ce = clock_enable_by_periph.get(name)
        if ce is not None:
            entry["clock"] = {"bus": ce.bus, "field": ce.field}
        ip = irq_priority_by_instance.get(name)
        if ip is not None:
            entry["default_irq_priority"] = ip.priority
        if name in irqs_by_peripheral:
            entry["interrupts"] = irqs_by_peripheral[name]
        out.append(entry)
    return out


def _peripheral_kind(name: str, typedef: str) -> str:
    """Heuristic mapping from instance name / typedef to a kind classifier."""
    if name.startswith("GPIO"):
        return "gpio"
    if name.startswith("TIM"):
        return "timer"
    if name.startswith("UART"):
        return "uart"
    if name.startswith("I2C"):
        return "i2c"
    if name.startswith("SPI") or name == "QSPI":
        return "spi"
    if name.startswith("DMAC"):
        return "dma"
    if name == "USB":
        return "usb"
    if name == "ADC":
        return "adc"
    if name == "RTC":
        return "rtc"
    if name == "FMC":
        return "flash_controller"
    if name == "RCC":
        return "rcc"
    if name == "PWR":
        return "power"
    if name == "EXTI":
        return "exti"
    if name in ("WWDG", "IWDG"):
        return "watchdog"
    if name == "CRC":
        return "crc"
    if name == "BKP":
        return "backup_registers"
    if name == "RNG":
        return "rng"
    if name == "LED":
        return "led_controller"
    if name == "AFIO":
        return "afio"
    return typedef.removesuffix("_TypeDef").lower()


def _irq_to_peripheral(name: str, peripherals: dict) -> str | None:
    # Most IRQ names map directly to a peripheral instance.
    if name in peripherals:
        return name
    # TIM1_BRK / TIM1_UP / TIM1_TRG_COM / TIM1_CC → TIM1
    if name.startswith("TIM1_"):
        return "TIM1" if "TIM1" in peripherals else None
    if name.startswith("EXTI"):
        return "EXTI" if "EXTI" in peripherals else None
    if name == "USB_DMA" or name == "USBP_WKUP":
        return "USB" if "USB" in peripherals else None
    if name == "RTCAlarm":
        return "RTC" if "RTC" in peripherals else None
    return None


def _irq_signal(name: str) -> str | None:
    """Extract a sub-signal like BRK/UP/CC from a multi-IRQ peripheral name."""
    if name.startswith("TIM1_"):
        return name[len("TIM1_"):]
    if name == "USB_DMA":
        return "DMA"
    if name == "USBP_WKUP":
        return "WKUP"
    if name == "RTCAlarm":
        return "Alarm"
    return None


def _pins_block(p: PdfData | None, package: str) -> list[dict[str, Any]]:
    if p is None:
        return []
    by_pin: dict[str, dict[str, Any]] = {}

    # Seed from pin descriptions — gives us pin number + type + main function.
    for pd in p.pin_descriptions:
        if package == "LQFP48":
            pin_no = pd.pin_number_lqfp48
        elif package == "LQFP64":
            pin_no = pd.pin_number_lqfp64
        else:
            pin_no = pd.pin_number_lqfp48 or pd.pin_number_lqfp64
        if pin_no is None:
            continue
        entry: dict[str, Any] = {
            "number": pin_no,
            "name": pd.pin_name,
            "type": pd.pin_type,
            "main_function": pd.main_function,
            "signals": [],
        }
        if pd.alternate_functions:
            for sig in pd.alternate_functions:
                entry["signals"].append({"kind": "alternate_list", "signal": sig})
        if pd.analog_functions:
            for sig in pd.analog_functions:
                entry["signals"].append({"kind": "analog", "signal": sig})
        by_pin[pd.pin_name] = entry

    # Overlay AF mux info on the same pin entries (when port name matches).
    for af_entry in p.pin_af_matrix:
        ent = by_pin.get(af_entry.pin)
        if ent is None:
            ent = {"name": af_entry.pin, "signals": []}
            by_pin[af_entry.pin] = ent
        for sig in (s.strip() for s in af_entry.signal.split("\n") if s.strip()):
            ent.setdefault("signals", []).append({
                "kind": "af",
                "af": af_entry.af,
                "signal": sig,
            })

    pins = list(by_pin.values())
    pins.sort(key=lambda e: (e.get("number") or 9999, e.get("name", "")))
    return pins


def _clocks_block(h: HeaderData) -> dict[str, Any]:
    return {
        "hse": {
            "default_hz": h.constants.get("HSE_VALUE"),
            "startup_timeout": h.constants.get("HSE_STARTUP_TIMEOUT"),
        },
        "mhsi": {"hz": h.constants.get("MHSI_VALUE")},
        "fhsi": {
            "hz": h.constants.get("FHSI_VALUE"),
            "notes": "Internal high-speed oscillator, WB32-specific (not present in STM32F10x)",
        },
        "lsi": {"hz": 40000},
        "lse": {"hz": 32768},
    }


def _flash_block(h: HeaderData) -> dict[str, Any]:
    """Flash controller facts that don't change between WB32 variants."""
    return {
        "controller": "FMC",
        "sector_size": 256,
        "page_program_size": 256,
        "notes": "Different register layout than STM32 FPEC controller",
    }

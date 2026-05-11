# WB32 Data Schema

Per-chip YAML lives at `data/chips/<part>.yaml`. Shared register definitions
live at `data/peripherals/<block>_v<n>.yaml` and are referenced by name from
chip files (a `GPIO_v1` block may be shared across all WB32F10x and WB32FQ95
variants, for example).

This schema is heavily inspired by [stm32-data](https://github.com/embassy-rs/stm32-data)
but adapted to WB32-specific concepts (alternate-function multiplexer, USB DMA
master, FMC register naming, two-level GPIO speed encoding).

## Top-level: `data/chips/<part>.yaml`

```yaml
name: WB32FQ95xC                 # canonical part name
family: WB32FQ95xx               # chip family
package: LQFP48                  # LQFP48 | LQFP64 | ...
flash_size: 131072               # bytes
ram_size: 36864                  # bytes
core:
  name: cortex-m3
  revision: r2p0
  max_frequency: 96000000        # Hz; some variants 128 MHz
  fpu: false
  mpu: true
  nvic_prio_bits: 4

sources:                         # provenance for this file
  generated_by: wb32-data 0.1.0
  generated_at: 2026-05-11T12:34:56Z
  inputs:
    - {kind: header,  path: vendor-lib/.../wb32f10x.h,       sha256: ...}
    - {kind: chibios, path: ChibiOS-Contrib/.../wb32_registry.h, sha256: ...}
    - {kind: pdf,     path: vendor-docs/datasheets/EN_DS1104041_WB32FQ95xC_V01.pdf, sha256: ...}
    - {kind: md,      path: docs/chip-overview.md,           sha256: ...}

memory:
  - {name: flash,  base: 0x08000000, size: 0x00020000, kind: flash, access: rx}
  - {name: sram,   base: 0x20000000, size: 0x00009000, kind: ram,   access: rwx}
  - {name: sram_bb,base: 0x22000000, size: 0x00120000, kind: bitband_ram}

interrupts:                      # ordered by NVIC number
  - {number:  0, name: WWDG,        description: "Window WatchDog Interrupt"}
  - {number:  1, name: PVD,         description: "PVD through EXTI Line detection"}
  - ...

dma:
  controllers:
    - {name: DMAC1, base: 0x40007C00, num_channels: 3, irq: DMAC1}
    - {name: DMAC2, base: 0x4000FC00, num_channels: 3, irq: DMAC2}
    - {name: USB_DMA, base: 0x40014000, num_channels: 0, irq: USB_DMA,
       notes: "Dedicated USB DMA master, separate from DMAC1/2"}
  features:
    lli_supported: false         # LLP register reserved; auto-reload or ISR chaining only
    notes:
      - "DMA peripheral handshake mapping differs from STM32F10x"

peripherals:
  - name: GPIOA
    kind: gpio
    block: GPIO_v1               # references data/peripherals/GPIO_v1.yaml
    base: 0x40000000
    bus: APB1
    clock: GPIOA                 # RCC clock-enable bit identifier
    interrupts: []
    pins:                        # 16 pins, sparse if package limits
      - {pin: PA0, package_pin: 14}
      - ...

  - name: TIM1
    kind: timer
    block: TIM_ADV_v1
    base: 0x40001C00
    bus: APB1
    clock: TIM1
    interrupts:
      - {name: TIM1_BRK,     signal: BRK}
      - {name: TIM1_UP,      signal: UP}
      - {name: TIM1_TRG_COM, signal: TRG_COM}
      - {name: TIM1_CC,      signal: CC}
    channels: 4
    is_32bit: false

  - name: UART1
    kind: uart
    block: UART_v1
    base: 0x40003800
    bus: APB1
    clock: UART1
    interrupts:
      - {name: UART1, signal: global}

  - name: USB
    kind: usb
    block: USB_v1
    base: 0x40014000
    bus: AHB
    clock: USB
    interrupts:
      - {name: USB,      signal: global}
      - {name: USB_DMA,  signal: dma}
      - {name: USBP_WKUP,signal: wakeup}
    has_dedicated_dma: true

  # ... ADC, I2C1, I2C2, SPI*, RTC, BKP, WWDG, IWDG, FMC, ANCTL, PWR, LED, etc.

pins:                            # ordered by package pin number
  - number: 1
    name: PA0
    port: A
    bit: 0
    package_position: top-left
    signals:                     # alternate-function multiplexer entries
      - {af: 0, signal: GPIO,      kind: default}
      - {af: 1, signal: TIM2_CH1_ETR, peripheral: TIM2}
      - {af: 7, signal: UART2_CTS, peripheral: UART2}
      - {kind: analog, signal: ADC_IN0, peripheral: ADC, channel: 0}
    notes: []
  - number: 2
    name: PA1
    ...

# Pins not exposed in this package omitted; LQFP48 vs LQFP64 difference is
# encoded by whether the pin appears here at all.

clocks:
  hse:
    default_hz: 8000000
    startup_timeout: 48000
  mhsi:
    hz: 8000000
  fhsi:                          # WB32-specific internal 48MHz
    hz: 48000000
    notes: "Internal high-speed oscillator, not present in STM32F10x"
  lsi: {hz: 40000}
  lse: {hz: 32768}
  pll:
    sources: [HSE, MHSI, FHSI]
    max_output: 96000000

flash:
  controller: FMC                # NOT FPEC like STM32; different register layout
  sector_size: 256
  sectors_per_bank: 1024
  banks: 1
  page_program_size: 256

bootloader:
  rom_dfu: true
  vid_pid: [0x342D, 0xDFA0]      # Westberry USB DFU
  notes:
    - "Entered via boot pin held at reset"

electrical:                      # populated from datasheet, may be partial
  vdd_min: 2.0
  vdd_max: 5.5
  temperature_range: [-40, 85]
  package_dimensions:
    package: LQFP48
    body: [7.0, 7.0]
    pitch: 0.5

errata:                          # references docs/errata.md
  - id: dma-lli-reserved
    severity: high
    summary: "DMA Linked-List-Item (LLP) register is reserved despite STM32-similar struct"
    workaround: "Use auto-reload or ISR-chained transfers"
```

## Shared peripheral blocks: `data/peripherals/<block>_v<n>.yaml`

```yaml
name: GPIO_v1
description: "WB32 GPIO controller with 8-entry AF mux per pin"
applies_to: [WB32F10x, WB32FQ95xx]   # informational
registers:
  - name: MODER
    offset: 0x00
    size: 32
    reset: 0x00000000
    description: "Port mode register"
    fields:
      - {name: MODER0, bits: [1, 0], description: "Pin 0 mode (00=input, 01=output, 10=AF, 11=analog)"}
      - {name: MODER1, bits: [3, 2]}
      - ...
  - name: OTYPER
    offset: 0x04
    ...
  - name: AFRL
    offset: 0x20
    description: "Alternate function low register (pins 0-7)"
    fields:
      - {name: AFR0, bits: [3, 0]}
      - ...
```

Speed encoding for WB32 is **two levels (LOW / HIGH)** and the encoding is
inverted relative to STM32 — this MUST be captured in the block YAML.

## Interrupts: `data/interrupts/<family>.yaml`

```yaml
family: WB32FQ95xx
core_exceptions:                 # negative IRQ numbers
  - {number: -14, name: NonMaskableInt, description: "Non Maskable Interrupt"}
  - ...
vendor_irqs:                     # 0-based, from IRQn enum
  - {number:  0, name: WWDG}
  - {number:  1, name: PVD}
  - ...
```

## Hand-curated overlays: `data/_overrides/<chip>.yaml`

Same structure as the chip YAML but partial. Applied via deep merge on top of
generated data. Use for:

- Hardware facts that no source covers (e.g., physical package dimensions
  pulled from a third-party measurement).
- Correcting known vendor doc errors after silicon validation.
- Adding informational fields not yet populated by the generator.

Each override entry MUST include a `_reason:` field at the leaf node level
explaining why the override exists.

## Validation reports: `data/validation-reports/<chip>.md`

Generated when `make validate` runs. Lists every disagreement between
sources, ranked by severity. Markdown items conflicting with higher-trust
sources are flagged as **markdown-error** and should be fixed in
`docs/chip-overview.md`.

## Schema versioning

The top-level chip YAML carries `schema_version: 1`. Incompatible changes
bump the version; consumers should error on unknown versions.

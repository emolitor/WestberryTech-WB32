# WestberryTech WB32 — Community Documentation

Comprehensive open-source documentation, drivers, and tools for Westberry Technology WB32F10x and WB32FQ95xx ARM Cortex-M3 microcontrollers.

WB32 chips are pin-compatible STM32F10x alternatives used in keyboards from GMMK, Akko, MonsGeek, Neo, Shortcut Studio, and others. Despite widespread use, public documentation is scarce. This repository consolidates about a year of reverse engineering, hardware testing, and driver development into a single reference.

## Documentation

| Document | Description |
|----------|-------------|
| [Chip Overview](docs/chip-overview.md) | WB32 family, architecture, STM32 comparison |
| [Getting Started](docs/getting-started.md) | Toolchain setup, first project, flashing |
| [Flashing](docs/flashing.md) | OpenOCD and USB DFU flashing guide |
| [Errata](docs/errata.md) | Defects discovered |
| [Bootloader](docs/bootloader.md) | ROM DFU bootloader, flashing tools, recovery |
| [ChibiOS Port](docs/chibios-port.md) | Driver inventory, setup, known limitations |
| [ChibiOS Bug Fixes](docs/chibios-bug-fixes.md) | Various fixes not all upstreamed yet |
| [WS2812 GPIO DMA Driver](docs/ws2812-gpio-dma-driver.md) | Architecture, timing, validation |
| [QMK Integration](docs/qmk-integration.md) | Submodule config, build commands |
| [CH582F Protocol](docs/wireless/ch582f-protocol.md) | Wireless module UART protocol reference |

## Examples

| Project | Description | Requirements |
|---------|-------------|--------------|
| [blink](examples/blink/) | Bare metal LED blink | arm-none-eabi-gcc |
| [systick-blink](examples/systick-blink/) | SysTick timer blink | arm-none-eabi-gcc |
| [dma-multi-transfer](examples/dma-multi-transfer/) | DMA ISR chaining test | ChibiOS + ChibiOS-Contrib |
| [spi-flash](examples/spi-flash/) | SPI flash test | ChibiOS + ChibiOS-Contrib |
| [ws2812-gpio-dma](examples/ws2812-gpio-dma/) | WS2812 RGB LED driver | QMK firmware |

## OpenOCD Flash Driver

OpenOCD flash driver and debug for WB32. Supports sector erase, mass erase, page program, and verify on all WB32F10x and WB32FQ95xx variants.

The driver is maintained in a dedicated OpenOCD fork: [emolitor/openocd](https://github.com/emolitor/openocd). See the fork for build instructions, usage, and FMC register reference.

## Structured Data

Machine-readable YAML data for every WB32 variant, inspired by
[stm32-data](https://github.com/embassy-rs/stm32-data) and
[ch32-data](https://github.com/ch32-rs/ch32-data). Per-chip files capture
memory map, peripheral instances, register bit-fields, interrupts, DMA
peripheral handshake numbers, pin AF mappings, clock-enable bits, and IRQ
priorities — drawn from the vendor CMSIS header, the ChibiOS-Contrib WB32
port, the vendor datasheets and reference manual, and the repository
markdown (in that trust order).

| Path | Contents |
|------|----------|
| [`data/chips/`](data/chips/) | Per-chip YAML — WB32F101xC, F102xC, F103xC, F104RC, F105RC, FQ95xC, F3G71xx |
| [`data/peripherals/`](data/peripherals/) | Shared register-block definitions (GPIO, TIM, UART, I2C, SPI, ADC, …) with bit-fields from the reference manual |
| [`data/interrupts/`](data/interrupts/) | Family-level interrupt tables |
| [`data/_overrides/`](data/_overrides/) | Hand-curated overlays for facts no source captures (bootloader VID/PID, errata) |
| [`data/validation-reports/`](data/validation-reports/) | Per-chip cross-source disagreement reports |
| [`data/schema.md`](data/schema.md) | Full per-chip YAML schema |
| [`data-gen/`](data-gen/) | Python generator that produces the above from `vendor-lib/`, `vendor-docs/`, ChibiOS-Contrib, and the repo's markdown |

To regenerate:

```bash
make -C data-gen data       # all chips
make -C data-gen validate   # cross-source validation, non-zero exit on errors
```

The generator is reproducible — running it should produce identical output
given the same inputs.

## Vendor Documentation

The [`vendor-docs/`](vendor-docs/) directory contains datasheets, the reference manual, application notes, and a reference schematic.

See [vendor-docs/README.md](vendor-docs/README.md) for a complete index.

## Related Projects

| Project | Description |
|---------|-------------|
| [emolitor/openocd](https://github.com/emolitor/openocd) | OpenOCD fork with WB32 flash driver |
| [emolitor/ChibiOS-Contrib](https://github.com/emolitor/ChibiOS-Contrib) | ChibiOS-Contrib fork with WB32 fixes (branch `em-wb32-improvements`) |
| [WestberryTech/wb32-dfu-updater](https://github.com/WestberryTech/wb32-dfu-updater) | Official DFU flash tool |
| [WestberryTech GitHub](https://github.com/WestberryTech/) | Vendor SDK, tools, examples |
| [qmk/ChibiOS-Contrib](https://github.com/qmk/ChibiOS-Contrib) | Upstream ChibiOS-Contrib (WB32 port) |
| [QMK Firmware](https://github.com/qmk/qmk_firmware) | Keyboard firmware with WB32 support |

## Development Hardware

You can source WB32F104 development boards from the [Westberry Taobao store](https://world.taobao.com/dianpu/471165391.htm?spm=a21wu.12321156-tw/qwen.shop-area.1.59ad6dc0ANXkNx).
For the WB32FQ95 you need to rework the board to replace the WB32F104 with a
WB32FQ95. It's possible to do this with either a hotplate or hot-air rework
station.

| Device | Flash | SRAM | Debug Probe |
|--------|-------|------|-------------|
| WB32F104RC | 256 KB | 36 KB | CMSIS-DAP |
| WB32FQ95xC | 256 KB | 36 KB | CMSIS-DAP |

## Repository Structure

```
WestberryTech-WB32/
├── docs/                    # Technical documentation
│   └── wireless/            # CH582F protocol reference
├── vendor-docs/             # Official Westberry documentation
│   ├── datasheets/          # DS001-DS005 + WB32FQ95xC
│   ├── reference-manual/    # PDF
│   ├── application-notes/   # AN001-AN005
│   └── schematics/          # Reference board schematic
├── vendor-lib/              # WB32F10x Standard Peripheral Library
├── data/                    # Machine-readable YAML (stm32-data inspired)
│   ├── chips/               # Per-chip files
│   ├── peripherals/         # Shared register-block definitions
│   ├── interrupts/          # Family interrupt tables
│   ├── _overrides/          # Hand-curated overlays
│   ├── validation-reports/  # Cross-source disagreements
│   └── schema.md            # YAML schema
├── data-gen/                # Python generator for data/
├── examples/                # Bare metal and ChibiOS examples
└── debug/                   # GDB scripts and J-Link config
```

## License

This project is licensed under the [GNU General Public License v2.0](LICENSE).

The vendor peripheral library ([`vendor-lib/`](vendor-lib/)) and vendor documentation ([`vendor-docs/`](vendor-docs/)) are provided by Westberry Technology and are subject to their respective licenses.

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines on reporting bugs, submitting patches, and testing requirements.

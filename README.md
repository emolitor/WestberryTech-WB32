# WestberryTech WB32 — Community Documentation & Tools

Comprehensive open-source documentation, drivers, and tools for Westberry Technology WB32F10x and WB32FQ95xx ARM Cortex-M3 microcontrollers.

WB32 chips are pin-compatible STM32F10x alternatives used in keyboards from GMMK, Akko, MonsGeek, Inland, Shortcut Studio, and others. Despite widespread use, public documentation is scarce. This repository consolidates about a year of reverse engineering, hardware testing, and driver development into a single reference.

## Key Findings

| Discovery | Impact | Documentation |
|-----------|--------|---------------|
| DMA BLOCK_TS limited to 9 bits (max 511) | DMA transfers >511 silently fail | [Silicon Errata](docs/silicon-errata.md) |
| Flash erases to 0x00 (inverted bit model) | Standard NOR flash assumptions break | [Silicon Errata](docs/silicon-errata.md) |
| GPIO OSPEEDR polarity inverted from STM32 | STM32 speed settings give opposite result | [Silicon Errata](docs/silicon-errata.md) |
| 11 ChibiOS-Contrib HAL bugs | DMA, PWM, USB, ADC drivers affected | [Bug Fixes](docs/chibios-bug-fixes.md) |
| PWM+DMA unusable for WS2812 multi-chunk | Timer generates stale pulses at boundaries | [WS2812 Driver](docs/ws2812-gpio-dma-driver.md) |
| ChibiOS DMA ISR priority must be >= 3 | Priority 1-2 causes kernel crash/deadlock | [Debug War Stories](docs/debug-war-stories.md) |
| CH582F wireless module UART protocol | Undocumented protocol used by many keyboards | [CH582F Protocol](docs/wireless/ch582f-protocol.md) |

## Quick Start

```bash
# Build a bare metal blink example
cd examples/blink
make

# Flash via OpenOCD (requires custom driver — see https://github.com/emolitor/openocd)
openocd -f interface/cmsis-dap.cfg -f target/wb32fq95x.cfg \
  -c "init; halt; flash probe 0" \
  -c "flash write_image erase build/Template.bin 0x08000000" \
  -c "verify_image build/Template.bin 0x08000000" \
  -c "reset run; shutdown"

# Flash via DFU (keyboards)
wb32-dfu-updater_cli -t -s 0x8000000 -D firmware.bin
```

## Documentation

| Document | Description |
|----------|-------------|
| [Chip Overview](docs/chip-overview.md) | WB32 family, architecture, STM32 comparison |
| [Getting Started](docs/getting-started.md) | Toolchain setup, first project, flashing |
| [Silicon Errata](docs/silicon-errata.md) | 5 hardware bugs discovered through testing |
| [Bootloader](docs/bootloader.md) | ROM DFU bootloader, flashing tools, recovery |
| [ChibiOS Port](docs/chibios-port.md) | Driver inventory, setup, known limitations |
| [ChibiOS Bug Fixes](docs/chibios-bug-fixes.md) | All 11 fixes with before/after code |
| [WS2812 GPIO DMA Driver](docs/ws2812-gpio-dma-driver.md) | Architecture, 8 bugs, timing, validation |
| [QMK Integration](docs/qmk-integration.md) | Submodule config, build commands |
| [Debug War Stories](docs/debug-war-stories.md) | 3 resolved bugs with root cause analysis |
| [CH582F Protocol](docs/wireless/ch582f-protocol.md) | Wireless module UART protocol reference |
| [Validation Results](docs/validation/) | Hardware test results for all drivers |

## Examples

| Project | Description | Requirements |
|---------|-------------|--------------|
| [blink](examples/blink/) | Bare metal LED blink | arm-none-eabi-gcc |
| [systick-blink](examples/systick-blink/) | SysTick timer blink | arm-none-eabi-gcc |
| [dma-multi-transfer](examples/dma-multi-transfer/) | DMA ISR chaining test | ChibiOS + ChibiOS-Contrib |
| [spi-flash](examples/spi-flash/) | SPI flash test | ChibiOS + ChibiOS-Contrib |
| [ws2812-gpio-dma](examples/ws2812-gpio-dma/) | WS2812 RGB LED driver | QMK firmware |

## OpenOCD Flash Driver

The only open-source flash driver for WB32. Supports sector erase, mass erase, page program, and verify on all WB32F10x and WB32FQ95xx variants.

The driver is maintained in a dedicated OpenOCD fork: [emolitor/openocd](https://github.com/emolitor/openocd). See the fork for build instructions, usage, and FMC register reference.

## Vendor Documentation

The `vendor-docs/` directory contains datasheets, the reference manual (with searchable text extracts), application notes, and a reference schematic.

```bash
# Search the reference manual
grep -r "DMA_CTLH" vendor-docs/reference-manual/chapters/
```

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

## Hardware Tested

| Device | Flash | SRAM | Debug Probe |
|--------|-------|------|-------------|
| WB32F104RC | 256 KB | 36 KB | CMSIS-DAP |
| WB32FQ95xC | 256 KB | 36 KB | CMSIS-DAP |

## Repository Structure

```
WestberryTech-WB32/
├── docs/                    # Technical documentation
│   ├── validation/          # Hardware test results
│   └── wireless/            # CH582F protocol reference
├── vendor-docs/             # Official Westberry documentation
│   ├── datasheets/          # DS001-DS005 + WB32FQ95xC
│   ├── reference-manual/    # PDF + searchable text chapters
│   ├── application-notes/   # AN001-AN005
│   └── schematics/          # Reference board schematic
├── vendor-lib/              # WB32F10x Standard Peripheral Library
├── examples/                # Bare metal and ChibiOS examples
└── debug/                   # GDB scripts and J-Link config
```

## License

This project is licensed under the [GNU General Public License v2.0](LICENSE).

The vendor peripheral library (`vendor-lib/`) and vendor documentation (`vendor-docs/`) are provided by Westberry Technology and are subject to their respective licenses.

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines on reporting bugs, submitting patches, and testing requirements.

# Example Projects

Bare metal and ChibiOS example projects for WB32F10x and WB32FQ95xx
microcontrollers.

## Projects

### blink/

Bare metal LED blink for WB32F104. The simplest possible WB32 program --
configures a GPIO pin and toggles it in a delay loop. No RTOS, no interrupts.

Uses the vendor standard peripheral library (`vendor-lib/`).

### systick-blink/

SysTick timer-driven LED blink. Demonstrates the ARM Cortex-M3 SysTick
interrupt handler for periodic timing instead of a busy-wait delay loop.

Uses the vendor standard peripheral library (`vendor-lib/`).

### dma-multi-transfer/

ChibiOS DMA validation project. Tests multi-chunk ISR-chained DMA transfers as
a workaround for the WB32 BLOCK_TS silicon errata (9-bit field limits each DMA
block to 511 elements). Verified by hardware testing on a WB32FQ95xC
development board.

Requires ChibiOS and ChibiOS-Contrib.

### spi-flash/

SPI flash read/write test using the ChibiOS SPI driver. Demonstrates SPI
peripheral initialization and basic flash memory communication.

Requires ChibiOS and ChibiOS-Contrib.

### ws2812-gpio-dma/

WS2812 RGB LED driver extracted from QMK firmware. Uses timer-triggered DMA to
write GPIO BSRR values directly, with ISR-chained block transfers and
double-buffered encoding. Handles up to 127 LEDs within the WB32 DMA hardware
constraints.

This is a QMK custom driver source file, not a standalone buildable project.
See [docs/ws2812-gpio-dma-driver.md](../docs/ws2812-gpio-dma-driver.md) for
full architecture documentation and the
[ws2812-gpio-dma/README.md](ws2812-gpio-dma/README.md) integration guide.

## Prerequisites

**All projects** require an ARM cross-compiler:

- `arm-none-eabi-gcc` (and associated binutils)

**ChibiOS projects** (`dma-multi-transfer/`, `spi-flash/`) additionally require:

- [ChibiOS](https://github.com/ChibiOS/ChibiOS) (21.11.x branch)
- [ChibiOS-Contrib](https://github.com/ChibiOS/ChibiOS-Contrib) (chibios-21.11.x branch)

These are not included in this repository. Clone them from upstream or use the
patched fork at https://github.com/emolitor/ChibiOS-Contrib (branch
`em-wb32-improvements`) which includes bug fixes for the WB32 port. See
[patches/](../patches/) for pre-generated patch files.

## Building

Bare metal projects:

```bash
cd examples/blink
make
```

ChibiOS projects (adjust paths to your ChibiOS installation):

```bash
cd examples/dma-multi-transfer
make
```

To specify a custom toolchain location:

```bash
make GCC_PATH=/path/to/arm-none-eabi/bin
```

Build output is placed in `build/` within each project directory.

## Vendor Library Path

The bare metal examples (`blink/`, `systick-blink/`) reference the vendor
standard peripheral library at `../../vendor-lib/` relative to the example
directory. This path resolves to the top-level `vendor-lib/` directory in this
repository.

## Flashing

Use OpenOCD with a CMSIS-DAP, J-Link, or WB-Link debug probe:

```bash
openocd -f interface/cmsis-dap.cfg -f target/wb32fq95x.cfg \
  -c "init; halt; flash probe 0" \
  -c "flash write_image erase build/Template.bin 0x08000000" \
  -c "verify_image build/Template.bin 0x08000000" \
  -c "reset run; shutdown"
```

See [emolitor/openocd](https://github.com/emolitor/openocd) for OpenOCD setup
with the custom WB32 flash driver.

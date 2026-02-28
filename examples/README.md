# Example Projects

Bare metal and ChibiOS example projects for WB32F10x and WB32FQ95xx
microcontrollers.

## Projects

### [blink/](blink/)

Bare metal LED blink for WB32F104. The simplest possible WB32 program --
configures a GPIO pin and toggles it in a delay loop. No RTOS, no interrupts.

Uses the vendor standard peripheral library ([`vendor-lib/`](../../vendor-lib/)).

### [systick-blink/](systick-blink/)

SysTick timer-driven LED blink. Demonstrates the ARM Cortex-M3 SysTick
interrupt handler for periodic timing instead of a busy-wait delay loop.

Uses the vendor standard peripheral library ([`vendor-lib/`](../../vendor-lib/)).

### [dma-multi-transfer/](dma-multi-transfer/)

ChibiOS DMA validation project. Tests multi-chunk ISR-chained DMA transfers as
a workaround for the WB32 BLOCK_TS silicon errata (9-bit field limits each DMA
block to 511 elements). Verified by hardware testing on a WB32FQ95xC
development board.

Requires ChibiOS and ChibiOS-Contrib.

### [spi-flash/](spi-flash/)

SPI flash read/write test using the ChibiOS SPI driver. Demonstrates SPI
peripheral initialization and basic flash memory communication.

Requires ChibiOS and ChibiOS-Contrib.

### [pin-validation/](pin-validation/)

Bare metal AF pin mapping validation test suite for WB32F104RC and WB32FQ95 (LQFP64).
Validates all 148 alternate function entries from the datasheet (DS004, Table 3.4-2)
via register read-back, plus functional tests for UART loopback, timer counters, ADC
channels, SPI/I2C init, LED controller, and USB clock access. No jumper wires
required -- all tests use internal hardware features. Results are reported via
UART1 serial and stored in a GDB/OpenOCD-inspectable SRAM struct. Both chips
produce identical results (162 pass, 0 fail, 8 skip). Build with `make CHIP=FQ95`
for the WB32FQ95 (128KB flash) or plain `make` for the WB32F104RC (256KB flash).

Uses the vendor standard peripheral library ([`vendor-lib/`](../../vendor-lib/)).

### [ws2812-gpio-dma/](ws2812-gpio-dma/)

Custom QMK WS2812 RGB LED driver. Uses timer-triggered DMA to write GPIO BSRR
values directly, with ISR-chained block transfers. Handles up to 127 LEDs 
127 LEDs within the WB32 DMA hardware constraints.

This is a QMK custom driver source file, not a standalone buildable project.
See [docs/ws2812-gpio-dma-driver.md](../docs/ws2812-gpio-dma-driver.md) for
full architecture documentation and the
[ws2812-gpio-dma/README.md](ws2812-gpio-dma/README.md) integration guide.

## Prerequisites

**All projects** require an ARM cross-compiler:

- `arm-none-eabi-gcc` (and associated binutils)

**ChibiOS projects** ([`dma-multi-transfer/`](dma-multi-transfer/), [`spi-flash/`](spi-flash/)) additionally require:

- [ChibiOS](https://github.com/ChibiOS/ChibiOS) (21.11.x branch)
- [ChibiOS-Contrib](https://github.com/ChibiOS/ChibiOS-Contrib) (chibios-21.11.x branch)

These are not included in this repository. Clone them from upstream or use the
fork at https://github.com/emolitor/ChibiOS-Contrib (branch
`em-wb32-improvements`) which includes bug fixes for the WB32 port.

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

The bare metal examples ([`blink/`](blink/), [`systick-blink/`](systick-blink/), [`pin-validation/`](pin-validation/)) reference the vendor
standard peripheral library at `../../vendor-lib/` relative to the example
directory. This path resolves to the top-level [`vendor-lib/`](../../vendor-lib/) directory in this
repository.

## Flashing

Use OpenOCD with a CMSIS-DAP, J-Link, or WB-Link debug probe:

```bash
openocd -f interface/cmsis-dap.cfg -f target/wb32.cfg \
  -c "init; halt; flash probe 0" \
  -c "flash write_image erase build/Template.bin 0x08000000" \
  -c "verify_image build/Template.bin 0x08000000" \
  -c "reset run; shutdown"
```

See [emolitor/openocd](https://github.com/emolitor/openocd) for OpenOCD setup
with the custom WB32 flash driver.

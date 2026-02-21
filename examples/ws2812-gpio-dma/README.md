# WS2812 GPIO DMA Driver

A QMK custom WS2812 driver for WB32FQ95xx microcontrollers. Uses
timer-triggered DMA to write GPIO BSRR values directly, with ISR-chained block
transfers and double-buffered encoding.

This is a driver source file for integration into QMK keyboard firmware, not a
standalone project.

## Why This Driver Exists

Two WB32 hardware constraints prevent standard WS2812 approaches from working
at scale:

1. **DMA BLOCK_TS limit**: The WB32 DMA controller's block transfer size field
   is 9 bits (max 511 transfers), not 12 bits as documented. A full 81-LED
   frame requires 5832 transfers, so single-block DMA is impossible.

2. **PWM+DMA glitching**: The timer continues running during ISR chunk
   transitions. Stale CCR values produce incorrect pulse widths that corrupt
   LED data at block boundaries.

The GPIO DMA approach solves both problems: GPIO pins hold their last written
state during ISR transitions, and the block-chained architecture handles
arbitrarily long LED strips within the 511-transfer limit.

See [docs/ws2812-gpio-dma-driver.md](../../docs/ws2812-gpio-dma-driver.md) for
the full architecture documentation and
[docs/silicon-errata.md](../../docs/silicon-errata.md) for details on the
BLOCK_TS hardware erratum.

## Key Features

- Supports up to 127 LEDs (hardware limit from 3-phase encoding and 511
  BLOCK_TS)
- Double buffer: 2 x 504 phases = 4,032 bytes total DMA buffer
- Branchless bit encoding (~40 us per block, 7x faster than branching
  approach)
- 8 hardware bugs identified and fixed during development
- Validated flicker-free on Bridge75 ANSI keyboard (81 LEDs)

## Configuration

| Define | Default | Description |
|--------|---------|-------------|
| `WS2812_LED_COUNT` | (required) | Number of LEDs in the strip |
| `WS2812_PHASE_TICKS` | 27 | Timer ticks per WS2812 phase (375 ns at 72 MHz) |
| `WS2812_DI_PIN` | (required) | QMK pin identifier for the data line |
| `WS2812_DMA_STREAM` | (required) | ChibiOS DMA stream identifier |
| `WS2812_PWM_DRIVER` | (required) | ChibiOS PWM driver for timer triggering |

## QMK Integration

1. Copy `ws2812_custom.c` into your keyboard directory.

2. In your keyboard's `rules.mk`:

   ```makefile
   WS2812_DRIVER = custom
   ```

3. In your keyboard's `config.h`, define the required configuration:

   ```c
   #define WS2812_LED_COUNT    81
   #define WS2812_DI_PIN       B0
   #define WS2812_DMA_STREAM   STM32_DMA_STREAM_ID(1, 3)
   #define WS2812_PWM_DRIVER   PWMD3
   ```

4. The driver provides the standard QMK WS2812 interface: `ws2812_init()`,
   `ws2812_setleds()`, and `ws2812_write()`.

## Hardware Bugs Fixed

Eight bugs were found and fixed during development and hardware validation:

1. ISR priority too low for reliable DMA callback timing
2. Wrong DMA completion flag constant
3. Missing null pointer checks on DMA stream
4. PWM timing corruption from stale CCR values (led to GPIO DMA approach)
5. `dmaStreamDisable()` clears all interrupt masks (must re-enable before each
   block)
6. Stale UG event triggers spurious DMA transfer at frame start
7. Stale UIF flag at block boundaries
8. ChEnReg not auto-cleared after non-circular transfer (stale handshake
   requests)

## Validation

- **GDB register validation**: 8 phases, 13 test points, all passing
- **Keyboard validation**: Flicker-free rainbow animation on Bridge75 ANSI (81
  LEDs) across extended operation with concurrent USB and UART traffic

See [docs/validation/ws2812-dma.md](../../docs/validation/ws2812-dma.md) for
the full test report.

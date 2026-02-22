# WS2812 GPIO DMA Driver

A custom WS2812 LED driver for WB32 microcontrollers that uses timer-triggered DMA to write
GPIO BSRR values directly, with ISR-chained block transfers and double-buffered encoding.
Designed for ChibiOS/QMK firmware on WB32FQ95xx and WB32F3G71xx targets.

Source: [`examples/ws2812-gpio-dma/ws2812_custom.c`](../examples/ws2812-gpio-dma/ws2812_custom.c)

---

## Background

Two hardware constraints on the WB32 drove the design of this driver.

### DMA BLOCK_TS Limited to 511

The WB32 DMA controller's CTLH register BLOCK_TS field is 9 bits wide (max 511), not 12 bits
as documented in the reference manual. A full 81-LED WS2812 frame requires 5832 DMA transfers
(81 LEDs x 24 bits x 3 phases per bit). Transferring the entire frame in one DMA block is
impossible. See [errata.md](errata.md) for full details on this hardware
erratum.

### PWM+DMA Approach Failed

The natural approach for WS2812 on a timer-equipped MCU is to use PWM output with DMA
updating the CCR (capture/compare) register each cycle. This was attempted and abandoned
because the timer continues running during ISR chunk transitions. When the ISR fires to chain
the next DMA block, the timer generates one or more additional PWM cycles using the stale CCR
value from the previous block's final transfer. These extra cycles produce incorrect pulse
widths that corrupt LED data.

### GPIO DMA Solution

Instead of modulating the PWM duty cycle, this driver writes directly to the GPIO BSRR
(bit set/reset register) via DMA, using the timer's update event as the DMA trigger. Each
WS2812 bit is encoded as three BSRR writes (see 3-Phase Bit Encoding below).

The key advantage: GPIO pins hold their last written state indefinitely. When the ISR fires
between blocks, the pin stays in whatever state the last DMA write set it to (always LOW,
since each bit ends with a RESET phase). The pause between blocks becomes an extended low
period, which is within WS2812 protocol tolerance (the reset threshold is approximately 9
microseconds, and the ISR pause is under 10 microseconds).

---

## Architecture

### Timer

- **Peripheral**: TIM3 (configurable via `WS2812_GPIO_DMA_TIMER`)
- **Clock**: 72 MHz, PSC=0 (no prescaler)
- **Phase period**: Configurable via `WS2812_PHASE_TICKS` (`#ifndef` guard, default 27)
- **Phase duration**: 27 ticks x 13.89 ns = 375 ns at 72 MHz
- **DMA trigger**: Timer update event (UDE bit in DIER)
- **Mode**: Continuous, no output compare — the timer is used purely as a DMA request
  generator

### DMA

- **Direction**: Memory-to-peripheral
- **Transfer width**: WORD (32-bit, matching BSRR register width)
- **Source increment**: Enabled (walks through the phase buffer)
- **Destination**: Fixed (`GPIOx->BSRR`)
- **Hardware handshake**: TIM3_UP (`WB32_DMAC_HWHIF_TIM3_UP`)
- **Circular mode**: Disabled — non-circular with ISR block chaining
- **IRQ priority**: 3 (`CORTEX_MAX_KERNEL_PRIORITY`, the minimum safe priority under ChibiOS)

### Double Buffer

Two buffers of 504 phases each (4,032 bytes total) form a double-buffer scheme:

```
buf[0]: [504 phases]  ← DMA reads from one buffer
buf[1]: [504 phases]  ← CPU fills the other buffer
```

The ISR chains the next DMA block from the pre-filled buffer while the CPU fills the
just-released buffer with the block after that. With a branchless fill taking approximately
40 microseconds and each DMA block playing back in approximately 189 microseconds (at 27
ticks per phase), the CPU has roughly 149 microseconds of margin per block.

This replaced an earlier quad-buffer design (4 x 504 = 8,064 bytes). The original per-phase
state machine fill took 282 microseconds per block, exceeding the 210 microsecond DMA playback
time and requiring four buffers to avoid data races. The branchless fill reduced fill time by
7x, making double buffering safe and saving 4 KB of SRAM.

### Block Layout

For 81 WS2812 LEDs (5832 total phases):

| Block | Phases | Buffer |
|-------|--------|--------|
| 0 | 504 | buf[0] |
| 1 | 504 | buf[1] |
| 2 | 504 | buf[0] |
| ... | ... | ... |
| 10 | 504 | buf[0] |
| 11 | 288 (remainder) | buf[1] |

12 blocks total: 11 x 504 + 1 x 288 = 5832 phases.

Block size 504 was chosen as the largest value that is less than or equal to the 511 hardware
maximum and is also divisible by 72 (phases per LED), ensuring every block starts on an LED
boundary. This eliminates the need for mid-LED state tracking across blocks.

### ISR Block Chaining

The DMA transfer-complete (TFR) interrupt fires after each block. The ISR:

1. Increments the block index.
2. If more blocks remain: configures DMA with the next buffer and block size, resets the timer
   counter and status register, and re-enables DMA.
3. If all blocks are sent: disables DMA, stops the timer, drives the pin LOW, and clears the
   transfer-active flag.

### Transfer Flow

```
ws2812_flush()
  ├── Wait for previous transfer (with timeout)
  ├── Pre-fill buf[0] and buf[1]
  ├── Configure DMA for block 0
  ├── Re-enable TFR/ERR interrupt masks
  ├── Start timer with UDE suppression sequence
  ├── Enable DMA
  └── Feed loop: fill next buffer as ISR advances
        ├── Block 0 completes → ISR starts block 1 from buf[1]
        ├── CPU fills buf[0] with block 2 data
        ├── Block 1 completes → ISR starts block 2 from buf[0]
        ├── CPU fills buf[1] with block 3 data
        ├── ...
        └── Block 11 completes → ISR stops timer, clears active flag
  └── Sleep 300µs for WS2812 reset pulse
```

---

## 3-Phase Bit Encoding

Each WS2812 bit is encoded as three timer-period phases. Data is sent in GRB order, MSB
first, matching the WS2812 wire protocol.

| Phase | Bit = 0 | Bit = 1 |
|-------|---------|---------|
| Phase 0 | BSRR_SET (pin HIGH) | BSRR_SET (pin HIGH) |
| Phase 1 | BSRR_RESET (pin LOW) | BSRR_NOP (pin stays HIGH) |
| Phase 2 | BSRR_NOP (pin stays LOW) | BSRR_RESET (pin LOW) |

Where:
- `BSRR_SET = (1 << pin)` — sets the GPIO pin high
- `BSRR_RESET = (1 << (pin + 16))` — resets the GPIO pin low
- `BSRR_NOP = 0x00000000` — no change to pin state

This produces:

- **Bit 0**: One phase high, two phases low (T0H = 1 phase, T0L = 2 phases)
- **Bit 1**: Two phases high, one phase low (T1H = 2 phases, T1L = 1 phase)

At 27 ticks per phase (375 ns): T0H = 375 ns, T1H = 750 ns, bit period = 1125 ns. All values
fall within the WS2812B datasheet tolerances.

### Branchless Encoding

The `ws2812_encode_byte()` inline function encodes one color byte (8 bits) into 24 phases
using arithmetic instead of conditionals:

```c
p[0] = BSRR_SET;
p[1] = (1 - bv) * BSRR_RESET;   /* RESET if bit=0, NOP(0) if bit=1 */
p[2] = bv * BSRR_RESET;          /* NOP(0) if bit=0, RESET if bit=1 */
```

On Cortex-M3, the compiler emits `MVN` + `AND` + `LSL` (all single-cycle instructions)
instead of branch instructions. This eliminates pipeline stalls under `-Os` optimization and
achieves approximately 40 microseconds per 504-phase block (roughly 2,900 cycles at 72 MHz),
a 7x improvement over the original per-phase state machine approach (282 microseconds).

---

## Comparison with Bitbang Driver

| Aspect | Bitbang | GPIO DMA (double buffer) |
|--------|---------|--------------------------|
| Transfer time (81 LEDs) | ~2.4 ms | ~2.5 ms |
| CPU during transfer | 100% (NOP loops) | ~100% (fill loop) |
| **Interrupts blocked** | **Entire 2.7 ms** (data + reset) | **~5 us total** (375 ns x 12 boundaries) |
| SRAM usage | 243 bytes (LED array only) | ~4,275 bytes (LED array + 2 x 2,016 byte buffers) |
| Timing source | CPU cycle-counting NOPs | Hardware timer + DMA |
| Clock sensitivity | Breaks if interrupted | Hardware-timed, interrupt-tolerant |

The critical advantage is **interrupt latency**. The bitbang driver disables all interrupts
for the entire 2.7 ms frame duration (81 ms per second at 30 fps). During this window, USB
packets, wireless protocol messages, and RTOS scheduling are all blocked. The GPIO DMA driver
allows interrupts to fire freely during 99.8% of the transfer, with interrupts blocked only
for approximately 375 ns at each of the 12 block boundaries. This is essential for wireless
keyboard responsiveness and USB stability.

## Key Lessons Learned

### Stale DMA Handshake State

`dmaStreamEnable()` can fire a spurious transfer from stale handshake requests accumulated
while the channel was idle. Timer update events continue to generate DMA requests even after
the transfer completes, and these requests are not discarded when the channel is re-enabled.

**Rule**: Always enable DMA after timer setup with UDE disabled. Never enable DMA with UDE
already active — the accumulated requests will fire immediately.

### WB32 Timer SR Register Is Read-Write

Unlike STM32 timers where the SR register uses rc_w0 (read-clear-write-0) semantics, the WB32
timer SR register is plain read-write. Writing `SR = ~FLAG` to clear a specific flag will set
all other bit positions to 1, potentially triggering spurious interrupts or DMA requests.

**Rule**: Always use `SR = 0` to clear timer status flags on WB32.

### WS2812 T0H Threshold Varies by LED Batch

Testing across two LED variations (ANSI and ISO keyboard boards) revealed different T0H
thresholds. The ANSI board LEDs accepted T0H = 417 ns (30 ticks), while the ISO board LEDs
interpreted T0H = 417 ns as a 1-bit, producing all-white output.

**Rule**: Keep T0H below approximately 420 ns for compatibility. The WS2812B datasheet
specifies T0H = 250-550 ns, but practical tolerance varies by LED manufacturer.

---

## Timing Analysis

Phase duration is configurable via `WS2812_PHASE_TICKS` (default 27, guarded by `#ifndef` so
keyboards can override it).

| Ticks | Phase Duration | T0H (1 phase) | T1H (2 phases) | T0L (2 phases) | T1L (1 phase) | Bit Period |
|-------|----------------|----------------|-----------------|-----------------|----------------|------------|
| 25 | 347 ns | 347 ns | 694 ns | 694 ns | 347 ns | 1042 ns |
| 27 | 375 ns | 375 ns | 750 ns | 750 ns | 375 ns | 1125 ns |
| 28 | 389 ns | 389 ns | 778 ns | 778 ns | 389 ns | 1167 ns |
| 30 | 417 ns | 417 ns | 833 ns | 833 ns | 417 ns | 1250 ns |

WS2812B datasheet tolerances (typical):
- T0H: 250-550 ns
- T1H: 650-950 ns
- T0L: 700-1000 ns
- T1L: 250-550 ns

All rows in the table above fall within specification. The default of 27 ticks provides
comfortable margin on both the T0H upper bound and the T1H lower bound.

### Frame Timing (81 LEDs)

| Parameter | Value |
|-----------|-------|
| Total phases | 5832 |
| Blocks | 12 (11 x 504 + 1 x 288) |
| DMA playback per block | ~189 us (504 x 375 ns) |
| Total DMA time | ~2.2 ms |
| ISR overhead (12 transitions) | ~60 us total |
| Reset pulse | 300 us |
| **Total frame time** | **~2.5 ms** |

---

## Integration

### File Placement

For QMK keyboards, place the driver source in the keyboard directory:

```
keyboards/<vendor>/<keyboard>/
├── ws2812_custom.c          ← driver source
├── config.h                 ← WS2812 pin and timing configuration
├── halconf.h                ← ChibiOS HAL configuration
├── mcuconf.h                ← MCU peripheral configuration
└── rules.mk                 ← build configuration
```

A reference copy is available at
[`examples/ws2812-gpio-dma/ws2812_custom.c`](../examples/ws2812-gpio-dma/ws2812_custom.c).

### rules.mk

```makefile
WS2812_DRIVER = custom
```

### config.h

```c
/* Required: data pin connected to WS2812 DIN */
#define WS2812_DI_PIN B10

/* Optional: override phase timing (default 27 ticks = 375ns at 72MHz) */
/* #define WS2812_PHASE_TICKS 28 */

/* Optional: override timer and DMA assignments */
/* #define WS2812_GPIO_DMA_TIMER   GPTD3 */
/* #define WS2812_GPIO_DMA_STREAM  WB32_DMA1_STREAM1 */
/* #define WS2812_GPIO_DMA_TIM_HWHIF WB32_DMAC_HWHIF_TIM3_UP */
```

### mcuconf.h

Ensure TIM3 and GPT are enabled:

```c
#define WB32_GPT_USE_TIM3   TRUE
```

### halconf.h

Ensure GPT is enabled:

```c
#define HAL_USE_GPT   TRUE
```

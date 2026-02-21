# WS2812 GPIO DMA Driver — Post-Fix Hardware Validation

**Date**: 2026-02-05
**Firmware**: `shortcut_bridge75_ansi_devtest.elf` (40960 bytes)
**Target**: WB32FQ95xx via CMSIS-DAP, OpenOCD 0.12.0+dev
**Tool**: gdb-multiarch batch scripts (8 phases)

## Context

The WS2812 GPIO DMA driver was rewritten to fix 4 hardware-confirmed bugs, then a 5th bug
was discovered and fixed: `dmaStreamDisable()` clears all DMA interrupt masks internally,
so subsequent frames never received TFR interrupts. The fix adds
`dmaStreamEnableInterrupt(WB32_DMAC_IT_TFR)` and `dmaStreamEnableInterrupt(WB32_DMAC_IT_ERR)`
calls before `dmaStreamEnable()` in `ws2812_flush()`.

## Results Summary

| Phase | Test | Result |
|-------|------|--------|
| 1 | Firmware boots, DMA stream allocated | **PASS** |
| 2 | DAR, CTLL, CFGH, PSC, DIER, GPIO correct | **PASS** |
| 3 | 3-phase encoding, GRB order, MSB-first | **PASS** |
| 4 | Frame 1 completes (12 blocks) | **PASS** |
| 4 | Frame 2 completes (mask fix verified) | **PASS** |
| 4 | Frame 3 completes (repeated success) | **PASS** |
| 4 | No DMA errors across 3 frames | **PASS** |
| 5 | Masks enabled during transfer (blocks 0-11) | **PASS** |
| 5 | Masks cleared after dmaStreamDisable | **PASS** |
| 5 | Masks re-enabled at next frame start | **PASS** |
| 6 | 0/6 stuck, 0/6 errors over 30s | **PASS** |
| 7 | 8/8 reset cycles pass | **PASS** |
| 8 | All registers match expected values | **PASS** |

**Overall: ALL 13 TESTS PASS**

---

## Phase 1: Flash and Boot Verification

Firmware programmed and verified. After 2s run:

| Check | Value | Result |
|-------|-------|--------|
| PC | 0x0800657e (chSysPolledDelayX) | PASS — in flash |
| xPSR | 0x21000000 | PASS — Thread mode, no fault |
| ws2812_dma_stream | 0x08008b34 | PASS — non-NULL |
| Initial SP | 0x20000400 | Valid SRAM |
| Reset vector | 0x080001b9 | Valid flash (Thumb) |

## Phase 2: Static Peripheral Configuration

| Register | Value | Check | Result |
|----------|-------|-------|--------|
| DAR | 0x40000418 | GPIOB->BSRR | PASS |
| CTLL | 0x02100125 | M2P\|WORD\|WORD\|MINC\|INT_EN | PASS |
| CFGH | 0x00003004 | DST_PER=6 (TIM3_UP) | PASS |
| CFGL | 0x00000060 | — | OK |
| TIM3_PSC | 0x00000000 | No prescaler | PASS |
| TIM3_DIER | 0x00000100 | UDE=1 | PASS |
| GPIOB_MODER B10 | mode=1 | Output | PASS |
| GPIOB_OTYPER B10 | OT=0 | Push-pull | PASS |
| DmaCfgReg | 0x00000001 | DMAC enabled | PASS |

## Phase 3: Buffer Content Verification

LED 0 colors: Green=0x2F, Red=0x45

| Check | Value | Result |
|-------|-------|--------|
| Phase 0 (SET) | 0x00000400 | PASS — BSRR_SET for B10 |
| Phase 1 (Green MSB=0) | 0x04000000 | PASS — bit=0 -> RESET |
| Phase 2 (Green MSB=0) | 0x00000000 | PASS — bit=0 -> NOP |
| Phase 24 (Red byte start) | 0x00000400 | PASS — byte boundary SET |
| Phase 72 (LED 1 start) | 0x00000400 | PASS — LED boundary SET |
| First 7 LEDs phase 0 | 7/7 = 0x00000400 | PASS |

## Phase 4: DMA ISR and Block Chaining

### First Callback
- flags = 0x00000001 (TFR=1, ERR=0) — **PASS**
- block_idx = 0 (pre-increment) — correct
- TIM3_ARR = 0x1d (29 = 30 ticks) — **PASS**
- TIM3_CR1 CEN=1 — timer running
- MaskTfr Ch0=1, MaskErr Ch0=1 — **PASS**

### Three-Frame Sequence

| Frame | block_idx | active | Timer | Result |
|-------|-----------|--------|-------|--------|
| 1 | 12 | 0 | stopped | PASS |
| 2 | 12 | 0 | — | PASS |
| 3 | 12 | 0 | — | PASS |

Post-frame masks: MaskTfr=0, MaskErr=0 — **PASS** (cleared by dmaStreamDisable)
RawErr = 0x00000000 — **PASS** (no DMA errors)

## Phase 5: Interrupt Mask Lifecycle (KEY TEST)

### During Chaining (blocks 0-11)

All 12 callbacks: MaskTfr=1, MaskErr=1, ChEn=0 — masks stay enabled throughout entire frame.

```
callback #0:  block_idx=0   MaskTfr=1  MaskErr=1  ChEn=0
callback #1:  block_idx=1   MaskTfr=1  MaskErr=1  ChEn=0
callback #2:  block_idx=2   MaskTfr=1  MaskErr=1  ChEn=0
callback #3:  block_idx=3   MaskTfr=1  MaskErr=1  ChEn=0
callback #4:  block_idx=4   MaskTfr=1  MaskErr=1  ChEn=0
callback #5:  block_idx=5   MaskTfr=1  MaskErr=1  ChEn=0
callback #6:  block_idx=6   MaskTfr=1  MaskErr=1  ChEn=0
callback #7:  block_idx=7   MaskTfr=1  MaskErr=1  ChEn=0
callback #8:  block_idx=8   MaskTfr=1  MaskErr=1  ChEn=0
callback #9:  block_idx=9   MaskTfr=1  MaskErr=1  ChEn=0
callback #10: block_idx=10  MaskTfr=1  MaskErr=1  ChEn=0
callback #11: block_idx=11  MaskTfr=1  MaskErr=1  ChEn=0
```

### Next Frame Start

First callback of subsequent frame: MaskTfr=1, MaskErr=1 — **PASS**

**Conclusion**: The `dmaStreamEnableInterrupt()` fix in `ws2812_flush()` correctly
re-enables interrupt masks before each new frame, compensating for `dmaStreamDisable()`
clearing them at frame end.

## Phase 6: Multi-Frame Stability (30s Extended Run)

6 samples over 30 seconds with halt/resume cycles:

| Sample | Time | active | block_idx | CEN | RawErr | State |
|--------|------|--------|-----------|-----|--------|-------|
| 1 | 5s | 1 | 5 | 1 | 0 | Mid-transfer |
| 2 | 10s | 0 | 12 | 0 | 0 | Idle (complete) |
| 3 | 15s | 0 | 12 | 0 | 0 | Idle (complete) |
| 4 | 20s | 0 | 12 | 0 | 0 | Idle (complete) |
| 5 | 25s | 0 | 12 | 0 | 0 | Idle (complete) |
| 6 | 30s | 1 | 6 | 1 | 0 | Mid-transfer |

- Stuck samples: **0/6**
- Error samples: **0/6**
- Completed (idle) samples: 4/6
- Mid-transfer samples: 2/6

Backtraces show normal QMK operation: `chSysPolledDelayX` -> `matrix_output_unselect_delay` -> `matrix_scan`

## Phase 7: Stress Test (8 Reset Cycles)

| Cycle | Duration | active | block_idx | RawErr | stream | Result |
|-------|----------|--------|-----------|--------|--------|--------|
| 1 | 3s | 0 | 12 | 0 | 0x08008b34 | PASS |
| 2 | 3s | 0 | 12 | 0 | 0x08008b34 | PASS |
| 3 | 3s | 0 | 12 | 0 | 0x08008b34 | PASS |
| 4 | 3s | 0 | 12 | 0 | 0x08008b34 | PASS |
| 5 | 3s | 0 | 12 | 0 | 0x08008b34 | PASS |
| 6 (ext) | 8s | 0 | 12 | 0 | 0x08008b34 | PASS |
| 7 (ext) | 8s | 0 | 12 | 0 | 0x08008b34 | PASS |
| 8 (ext) | 8s | 0 | 12 | 0 | 0x08008b34 | PASS |

**8/8 PASS** — Driver re-initializes correctly after every reset.

## Phase 8: Full Register Dump (Active Transfer)

Captured at block_idx=0, mid-transfer:

### TIM3
| Register | Value | Decoded |
|----------|-------|---------|
| CR1 | 0x00000085 | CEN=1, ARPE=1, OPM=0 |
| CR2 | 0x00000000 | — |
| DIER | 0x00000100 | UDE=1, UIE=0 |
| SR | 0x00000001 | UIF=1 |
| CNT | 0x0000000d | Counter mid-count |
| PSC | 0x00000000 | **PASS** |
| ARR | 0x0000001d | **PASS** (29 = 30 ticks) |

### DMAC1 Channel 0
| Register | Value | Decoded |
|----------|-------|---------|
| SAR | 0x20002bd4 | buf[1] base (next block pre-loaded) |
| DAR | 0x40000418 | **PASS** (GPIOB->BSRR) |
| CTLL | 0x02100125 | **PASS** |
| CTLH | 0x000001f8 | **PASS** (504 phases) |
| CFGL | 0x00000060 | — |
| CFGH | 0x00003004 | **PASS** (DST_PER=6 = TIM3_UP) |

### DMAC1 Global
| Register | Value | Key bits |
|----------|-------|----------|
| DmaCfgReg | 0x00000001 | EN=1 |
| ChEnReg | 0x00000000 | Ch0=0 (transfer just completed) |
| RawTfr | 0x00000001 | Ch0=1 (TFR pending) |
| RawBlock | 0x00000001 | Ch0=1 |
| RawErr | 0x00000000 | **PASS** (no errors) |
| StatusTfr | 0x00000001 | Ch0=1 (unmasked TFR) |
| StatusErr | 0x00000000 | No errors |
| MaskTfr | 0x00000001 | **PASS** (Ch0 enabled) |
| MaskErr | 0x00000001 | **PASS** (Ch0 enabled) |

---

## Conclusion (Devboard Validation — 2026-02-05)

All 8 validation phases pass. The WS2812 GPIO DMA driver with the
`dmaStreamEnableInterrupt()` fix is fully operational:

1. **Peripheral configuration** matches design exactly
2. **Buffer encoding** correctly implements 3-phase WS2812 protocol (GRB, MSB-first)
3. **ISR block chaining** completes all 12 blocks per frame
4. **Multi-frame operation** works reliably (the fix is confirmed)
5. **Interrupt mask lifecycle** proven at register level: enabled during transfer, cleared by dmaStreamDisable, re-enabled by flush fix
6. **30+ second stability** with no stuck states or errors
7. **8 reset cycles** all successful — initialization is robust
8. **Full register snapshot** serves as reference baseline

---

## Phase 9: Real Keyboard Validation (2026-02-06)

After devboard validation confirmed ISR/DMA correctness, further testing on the real Bridge75 keyboard revealed an animation flicker issue (not visible with solid colors). Root cause analysis and final fix documented below.

### Animation Flicker Root Cause: CPU/DMA Data Race

The fill function takes **282µs** per 504-phase block (measured via DWT cycle counter at 72MHz), but DMA plays each block in **210µs**. The fill is slower than DMA playback, so it gradually falls behind and eventually writes to a buffer that DMA is simultaneously reading — producing visible corruption during color-changing animations (solid colors are unaffected because the data being written is identical to what's already in the buffer).

Key system discoveries that informed the analysis:
- `CH_CFG_TIME_QUANTUM = 0` — ChibiOS round-robin preemption is disabled
- No ChibiOS threads exist for wireless — `wireless_task()` is cooperative polling in the main thread
- `chThdYield()` returns immediately (no other same-priority threads)
- The fill loop is a busy-wait spin, not a preemption victim

### Buffer Count Progression

| Buffers | Runway before reuse | Margin vs 282µs fill | Observed result |
|---------|--------------------|-----------------------|-----------------|
| 2 (double) | 210µs (1 period) | -72µs (race!) | Flicker from ~U/I keys (block ~5) |
| 3 (triple) | 420µs (2 periods) | +138µs avg, race at block 8 | Flicker from ~<> keys (block ~8) |
| 4 (quad) | 630µs (3 periods) | +348µs, all 12 blocks safe | **No flicker** |

Mathematical proof: with 4 buffers, fill for block n finishes at `(n-3) × 282µs`, DMA starts block n at `n × 210µs`. Race occurs when `(n-3) × 282 ≥ n × 210`, i.e. `n ≥ 11.75`. All 12 blocks (0-11) are safe. Worst-case margin at block 11: 54µs.

### Failed Approaches

| Approach | Result | Why |
|----------|--------|-----|
| Increase timer ticks (30→37) | All-white LEDs | T0H=514ns exceeds WS2812 0/1 threshold (~500ns) |
| LED-at-a-time fill (~22µs) | Keyboard lockup | Works on devboard, fails on real keyboard (unknown interaction) |
| LUT-based fill | Devboard hang | CPU stuck at `__port_exit_from_isr` with corrupted LR after 1 frame |
| HIGHPRIO thread boost | Flicker shifted | No other threads to starve; `chThdYield()` is a no-op anyway |

### WS2812 T0H Timing Discovery

These WS2812 chips reject T0H=514ns (37 ticks at 72MHz) as a 1-bit, producing all-white output. The 0/1 threshold is below ~500ns for this hardware. Original T0H=417ns (30 ticks) works correctly and is well within the WS2812B datasheet spec of 250-550ns.

### Final Configuration

```c
#define WS2812_PHASE_TICKS   30U    /* 30 ticks = 417ns (T0H within 250-550ns spec) */
static uint32_t ws2812_buf[4][504]; /* Quad buffer: 3-period runway > 282µs fill */
```

### Test Results

| Test | Target | Result |
|------|--------|--------|
| 30-second GDB stress test | Devboard | PASS (0 errors, 54/81 LEDs active in rainbow) |
| Rainbow animation visual | Real keyboard | **PASS — flicker-free across all 81 LEDs** |
| Solid color visual | Real keyboard | PASS (no artifacts) |

### Comparison with Bitbang Driver

| Aspect | Bitbang | GPIO DMA (quad buffer) |
|--------|---------|----------------------|
| Transfer time (81 LEDs) | ~2.4 ms | ~2.5 ms |
| CPU during transfer | 100% (NOP loops) | ~100% (fill loop) |
| **Interrupts disabled** | **Entire 2.7ms** (data + reset) | **~5µs total** (417ns × 12 boundaries) |
| SRAM usage | 243 bytes | 8,307 bytes |
| Timing source | CPU cycle-counting NOPs | Hardware timer + DMA |

The primary advantage is interrupt latency: bitbang blocks all interrupts for 2.7ms per frame, while GPIO DMA allows interrupts during 99.8% of the transfer. Critical for wireless keyboard responsiveness.

# ChibiOS-Contrib WB32 Bug Fixes

## Overview

This document catalogs bugs found in the ChibiOS-Contrib WB32FQ95xx HAL port. Fixes for a
subset of these are available on the `em-wb32-improvements` branch:

- **Repository**: https://github.com/emolitor/ChibiOS-Contrib
- **Branch**: `em-wb32-improvements`

The fixes cover DMA controller issues, incorrect register definitions, and corrections to
the ADC and USB drivers.

---

## Applying the Fixes

Use the fork branch directly:

```bash
cd ChibiOS-Contrib
git remote add emolitor https://github.com/emolitor/ChibiOS-Contrib.git
git fetch emolitor em-wb32-improvements
git checkout emolitor/em-wb32-improvements
```

---

## Fix 1: DMA Block Transfer Size Mask (Critical -- Silicon Errata)

**File**: `LLD/DMAv1/wb32_dma.h:588`

The reference manual documents the CTLH register BLOCK_TS field as bits [11:0], implying a
maximum single-block transfer size of 4095. Hardware testing confirms only bits [8:0] are
writable -- the actual maximum is 511. See [silicon-errata.md](silicon-errata.md) for full
details.

The original `0x1FF` mask was numerically correct but used as an unexplained magic number.
The fix adds a named constant and a runtime assertion:

**Before:**
```c
(dmastp)->dmac->Ch[(dmastp)->channel].CTLH = (uint32_t)((size) & 0x1FF);
```

**After:**
```c
osalDbgAssert((size) <= 511U, "DMA BLOCK_TS max 511");
(dmastp)->dmac->Ch[(dmastp)->channel].CTLH = (uint32_t)((size) & WB32_DMA_CHCFG_SIZE_MASK);
```

The same correction was applied to `dmaWaitCompletion()` for consistency.

**Impact**: Without the assertion, transfers configured with more than 511 elements are
silently truncated to the lower 9 bits. A request for 512 elements transfers only 0; a
request for 1024 elements also transfers 0. No error is generated.

---

## Fix 2: DMA ISR Conditional Checks

**File**: `LLD/DMAv1/wb32_dma.c:191-196`

The DMAC2 interrupt handler conditionals were all comparing against `> 0`, meaning stream
handlers for channels 2 and 3 could execute even when those channels were not allocated.

**Before:**
```c
if (WB32_DMAC2_NUM_CHANNELS > 0) { /* Stream 1 handler */ }
if (WB32_DMAC2_NUM_CHANNELS > 0) { /* Stream 2 handler */ }
if (WB32_DMAC2_NUM_CHANNELS > 0) { /* Stream 3 handler */ }
```

**After:**
```c
if (WB32_DMAC2_NUM_CHANNELS > 0) { /* Stream 1 handler */ }
if (WB32_DMAC2_NUM_CHANNELS > 1) { /* Stream 2 handler */ }
if (WB32_DMAC2_NUM_CHANNELS > 2) { /* Stream 3 handler */ }
```

**Impact**: Could invoke uninitialized callback pointers if DMAC2 was configured with fewer
than 3 channels.

---

## Fix 3: dmaInit() Reset Conditional

**File**: `LLD/DMAv1/wb32_dma.c:218`

The DMAC1 reset block incorrectly checked `WB32_DMAC2_NUM_CHANNELS` instead of
`WB32_DMAC1_NUM_CHANNELS`.

**Before:**
```c
if (WB32_DMAC2_NUM_CHANNELS > 0) {
    /* DMAC1 reset code */
}
```

**After:**
```c
if (WB32_DMAC1_NUM_CHANNELS > 0) {
    /* DMAC1 reset code */
}
```

**Impact**: DMAC1 would not be reset during initialization if DMAC2 had zero channels
configured, leaving DMAC1 in an undefined state after a soft reset.

---

## Fix 4: DST_AUTO_RELOAD_DIS Bit Position

**File**: `LLD/DMAv1/wb32_dma.h:294`

The destination auto-reload disable constant used bit position 30 instead of the correct
bit position 31.

**Before:**
```c
#define WB32_DMA_CHCFG_DST_AUTO_RELOAD_DIS  (0x0U << 30)
```

**After:**
```c
#define WB32_DMA_CHCFG_DST_AUTO_RELOAD_DIS  (0x0U << 31)
```

**Impact**: Inconsistency with the corresponding `_EN` variant. Because the value being
shifted is `0x0`, the practical effect is cosmetic, but the incorrect bit position could
cause bugs if the define were used in read-modify-write operations or if the value were
changed.

---

## Fix 5: ADC Comment Typos

**File**: `LLD/ADCv1/hal_adc_lld.h:113, 121`

Two comment typos corrected.

**Before:**
```c
/* ... when usbed in ... */
```

**After:**
```c
/* ... when used in ... */
```

**Impact**: Documentation only.

---

## Fix 6: USB Macro Name Typo

**Files**: `LLD/USBv1/hal_usb_lld.h:38, 51` and `LLD/USBv1/hal_usb_lld.c:737`

The USB endpoint count macro was misspelled in three locations.

**Before:**
```c
#define USB_ENDOPOINTS_NUMBER   3
```

**After:**
```c
#define USB_ENDPOINTS_NUMBER    3
```

**Impact**: Code referencing the correctly-spelled `USB_ENDPOINTS_NUMBER` would fail to
compile or fall through to a different default. The typo was consistent across all three
usage sites, so the driver compiled and functioned correctly in isolation, but external code
using the standard spelling would not see the WB32-specific value.

---

## Fix 7: PWM Driver BDTR Register Access (Critical)

**Files**: `LLD/TIMv1/hal_pwm_lld.c:435-444, 463-471`

The Break and Dead-Time Register (BDTR) was unconditionally written for all timer instances
when `WB32_PWM_USE_TIM1` was defined. Only TIM1 (the advanced timer) has a BDTR register.
TIM2-4 register maps end at CCR4 (offset 0x40); writing to offset 0x44 (BDTR) accesses
undefined memory.

**Before (in `pwm_lld_start()`):**
```c
#if WB32_PWM_USE_TIM1
  pwmp->tim->BDTR = WB32_TIM_BDTR_MOE;
#endif
```

**After:**
```c
#if WB32_PWM_USE_TIM1
  if (&PWMD1 == pwmp) {
    pwmp->tim->BDTR = WB32_TIM_BDTR_MOE;
  }
#endif
```

**Before (in `pwm_lld_stop()`):**
```c
#if WB32_PWM_USE_TIM1 || WB32_PWM_USE_TIM8 || WB32_PWM_USE_TIM20
    pwmp->tim->BDTR = 0;
#endif
```

**After:**
```c
#if WB32_PWM_USE_TIM1
    if (&PWMD1 == pwmp) {
      pwmp->tim->BDTR = 0;
    }
#endif
```

The stop path also removed references to `TIM8` and `TIM20`, which do not exist on the
WB32FQ95xx.

**Impact**: Using PWM on TIM2-4 while TIM1 was also enabled could cause undefined behavior
from writes to unmapped register space.

---

## Fix 8: PWM OC5M/OC6M Macro Bit Shift

**File**: `LLD/TIMv1/wb32_tim.h:358-359, 367-368`

The output compare mode macros for channels 5 and 6 used `>> 2` for the high bit
extraction, inconsistent with channels 1-4 which use `>> 3`.

**Before:**
```c
#define WB32_TIM_CCMR3_OC5M(n) ((((n) & 7) << 4) | (((n) >> 2) << 16))
#define WB32_TIM_CCMR3_OC6M(n) ((((n) & 7) << 12) | (((n) >> 2) << 24))
```

**After:**
```c
#define WB32_TIM_CCMR3_OC5M(n) ((((n) & 7) << 4) | (((n) >> 3) << 16))
#define WB32_TIM_CCMR3_OC6M(n) ((((n) & 7) << 12) | (((n) >> 3) << 24))
```

**Impact**: Output compare modes 8-15 (which use the 4th bit) would be incorrectly
configured on channels 5 and 6. Modes 0-7 are unaffected.

---

## Fix 9: GPIO OSPEEDR Polarity Documentation

**File**: `LLD/GPIOv1/hal_pal_lld.h`

Added documentation comment clarifying that the WB32 GPIO speed register uses inverted
polarity compared to STM32:

```c
/* WB32 GPIO OSPEEDR polarity is inverted from STM32:
 *   0 = HIGH speed
 *   1 = LOW speed
 * Default PAL_MODE_OUTPUT_PUSHPULL uses high speed (OSPEEDR=0). */
```

**Impact**: Documentation only. Prevents incorrect assumptions when porting STM32 code that
explicitly sets GPIO speed values.

---

## Fix 10: Timer Counter Width Documentation

**Files**: `WB32FQ95xx/wb32_registry.h`, `WB32F3G71xx/wb32_registry.h`,
`LLD/TIMv1/hal_gpt_lld.h`

The registry defined `WB32_TIMx_IS_32BITS = FALSE` for all timers without documenting that
WB32 timers have 20-bit counters (not 16-bit as `IS_32BITS = FALSE` might imply). Added
documentation and a max counter constant:

```c
/* Note: WB32 timers have 20-bit counters (CNT[19:0]), not 16-bit or 32-bit.
 * IS_32BITS = FALSE is correct as they are not full 32-bit timers.
 * Max counter value: 0xFFFFF (1,048,575) */
#define WB32_TIM_CNT_MAX    0xFFFFFU
```

A corresponding note was added to the `gptcnt_t` typedef in `hal_gpt_lld.h`.

**Impact**: Documentation only. Prevents incorrect assumptions about timer overflow behavior.
The 20-bit counter allows periods up to 1,048,575 ticks before overflow, which at 96 MHz
gives approximately 10.9 ms maximum period.

---

## Fix 11: ADC/USB/DMA Consistency Improvements

Several minor corrections were included alongside the above fixes:

- `dmaWaitCompletion()` block size mask updated to use `WB32_DMA_CHCFG_SIZE_MASK` (matching
  Fix 1)
- Consistent use of named constants throughout the DMA driver where magic numbers were
  previously used

---

## WWDG Driver Implementation

In addition to bug fixes, a complete Window Watchdog (WWDG) driver was implemented following
the ChibiOS WDG HAL interface.

### New Files

| File | Description |
|------|-------------|
| `LLD/xWDGv1/hal_wdg_lld.h` | Driver header with configuration structures |
| `LLD/xWDGv1/hal_wdg_lld.c` | Driver implementation |
| `LLD/xWDGv1/driver.mk` | Build system integration |

### Modified Files

| File | Change |
|------|--------|
| `WB32FQ95xx/platform.mk` | Added xWDGv1 driver include |
| `WB32F3G71xx/platform.mk` | Added xWDGv1 driver include |

### Features

- Configurable counter value (0x40 to 0x7F)
- Configurable window value for windowed refresh operation
- Prescaler selection: DIV1, DIV2, DIV4, DIV8
- Early wakeup interrupt support
- Standard ChibiOS WDG API: `wdg_lld_init()`, `wdg_lld_start()`, `wdg_lld_stop()`,
  `wdg_lld_reset()`

### Enabling the Driver

In `halconf.h`:

```c
#define HAL_USE_WDG                         TRUE
```

In `mcuconf.h`:

```c
#define WB32_WDG_USE_WWDG                   TRUE
#define WB32_WDG_WWDG_IRQ_PRIORITY          3   /* Must be >= CORTEX_MAX_KERNEL_PRIORITY */
```

### Configuration Example

```c
static const WDGConfig wdgcfg = {
    .cnt   = 0x7F,                          /* Counter value (max timeout) */
    .win   = 0x7F,                          /* Window value (no windowing) */
    .wdgtb = WB32_WWDG_CFR_WDGTB_DIV8,     /* Prescaler /8 */
    .ewi   = false                          /* No early wakeup interrupt */
};

wdgStart(&WDGD1, &wdgcfg);

/* In main loop: */
wdgReset(&WDGD1);
```

### Key Registers

| Register | Offset | Description |
|----------|--------|-------------|
| WWDG_CR | 0x00 | Counter value [6:0], enable bit WDGA [7] |
| WWDG_CFR | 0x04 | Window value [6:0], timer prescaler [8:7], early wakeup IE [9] |
| WWDG_SR | 0x08 | Early wakeup interrupt flag [0] |

### Timeout Calculation

The WWDG timeout period is:

```
t_WWDG = t_PCLK2 * 4096 * 2^PSC * (CNT[5:0] + 1)
```

Where:
- `t_PCLK2` = 1 / PCLK2 frequency
- `PSC` = prescaler exponent (0, 1, 2, or 3 for DIV1/2/4/8)
- `CNT[5:0]` = counter bits [5:0] (counter range 0x40-0x7F gives 0-63)

**Example**: PCLK2 = 48 MHz, prescaler = DIV8, counter = 0x7F (63):

```
t_WWDG = (1/48000000) * 4096 * 8 * 64 = ~43.7 ms
```

The counter must be refreshed after it enters the window (below the window value) but before
it reaches 0x40, at which point the watchdog triggers a system reset.

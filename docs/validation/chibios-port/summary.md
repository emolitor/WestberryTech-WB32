# Validation Summary

Overall status of ChibiOS-Contrib WB32FQ95 port validation.


## Overall Assessment

| Phase | Component | Status | Notes |
|-------|-----------|--------|-------|
| 1 | Clock Configuration | Correct | LSI uses max value (conservative) |
| 2 | Peripheral Bus | Correct | WWDG now implemented in xWDGv1 |
| 3 | GPIO Driver | Correct | Schmitt trigger not exposed |
| 4 | USB Driver | Fixed | Typo fixed, PIO mode only |
| 5 | Timer Driver | Fixed | 20-bit counter width documented |
| 6 | DMA Driver | Fixed | All 4 bugs fixed |
| 7 | ADC Driver | Fixed | Typo fixed, injected channels not supported |
| 8 | Flash Driver | Validated | Erase=0x00 confirmed by hardware test, mass erase not in ChibiOS |
| 9 | OpenOCD Flash Driver | Fixed | PRE_OP per-op bug fixed, erase=0x00 confirmed |
| 10 | WS2812 GPIO DMA Driver | **Keyboard validated** | 8 bugs fixed (incl. DMA ISR priority root cause), quad buffer, flicker-free on Bridge75. 32x frame rate improvement under UART stress (2026-02-07) |

**Conclusion:** The port is production-ready for most applications.

## Phase Details

See individual phase files for detailed validation results:

- `phase1-clock.md` - Clock tree, PLL, oscillators
- `phase2-bus.md` - APB1/APB2/AHB peripheral mapping
- `phase3-gpio.md` - GPIO modes, registers, drive strength
- `phase4-usb.md` - USB device, endpoints, FIFO
- `phase5-timer.md` - Timer counter width, PWM modes
- `phase6-dma.md` - DMA channels, handshake interfaces
- `phase7-adc.md` - ADC channels, sampling, triggers
- `phase8-flash.md` - Flash operations, erase patterns
- `../openocd-flash.md` - OpenOCD flash driver hardware validation
- `../ws2812-dma.md` - WS2812 GPIO DMA driver post-fix validation (8 phases, 13 tests)

## Bug Fixes Applied

All identified bugs have been fixed in branch `em-wb32-improvements` (commit `225c26e4`):

### ChibiOS-Contrib HAL Fixes

| Bug | File | Status |
|-----|------|--------|
| DMA BLOCK_TS mask + assert | `wb32_dma.h` | Fixed — uses `WB32_DMA_CHCFG_SIZE_MASK` (0x1FF) |
| DMA `dmaWaitCompletion` mask | `wb32_dma.h` | Fixed — consistent with BLOCK_TS mask |
| DMA ISR conditionals | `wb32_dma.c:191-196` | Fixed |
| dmaInit() reset conditional | `wb32_dma.c:218` | Fixed |
| DST_AUTO_RELOAD_DIS bit position | `wb32_dma.h:294` | Fixed |
| ADC comment typos | `hal_adc_lld.h:113,121` | Fixed |
| USB macro typo | `hal_usb_lld.h/c` | Fixed |
| PWM BDTR register access | `hal_pwm_lld.c:435-471` | Fixed |
| PWM OC5M/OC6M macro shift | `wb32_tim.h:358-368` | Fixed |
| GPIO OSPEEDR polarity doc | `hal_pal_lld.h` | Added (WB32: 0=HIGH, inverted from STM32) |

### Bridge75 Firmware Fixes

| Bug | File | Status |
|-----|------|--------|
| NVIC pending interrupt `<` → `<<` | `lp_sleep.c`, `lpwr_wb32.c` | Fixed — validated on keyboard |
| PWR register magic numbers | `lp_sleep.c`, `lpwr_wb32.c` | Named defines + bit field docs |
| PRE_LP/POST_LP binary blobs | `lp_sleep.c`, `lpwr_wb32.c` | Disassembly documented |
| USBSLP visual feedback | `bridge75.c` | 1-second escape LED flash |
| WS2812 GPIO DMA (8 bugs) | `ws2812_custom.c` | Validated on real keyboard (root cause: DMA ISR priority 2→3) |

See `../chibios-bug-fixes.md` for implementation details.

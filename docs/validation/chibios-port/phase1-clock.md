# Phase 1: Clock Configuration Validation


## Summary

| Parameter | Port Value | Datasheet Value | Status |
|-----------|------------|-----------------|--------|
| LSI frequency | 40 kHz | 20-40 kHz (range) | Uses max value |
| HSE range | 4-16 MHz | 4-16 MHz | Correct |
| LSE range | 32.768 kHz - 1 MHz | 32.768 kHz | Max seems high |
| PLL input range | 2-16 MHz | 1-16 MHz | Conservative |
| PLL output range | 48-96 MHz | 12-96 MHz | Correct (max) |
| PLL multipliers | 12, 16, 20, 24 | 12, 16, 20, 24 | Correct |
| APB1/APB2 max | 96 MHz | 96 MHz | Correct |
| USB prescaler | DIV1, DIV1P5, DIV2, DIV3 | DIV2, DIV1.5, DIV3 (+ bypass) | Correct |
| RTC sources | LSE, LSI, HSEDIV | LSE, LSI, HSE/128 | Correct |

## Detailed Findings

### LSI Clock Frequency
- **Port:** `WB32_LSICLK = 40000` (40 kHz)
- **Datasheet:** Min 20 kHz, Max 40 kHz
- **Impact:** Port uses maximum value. If actual LSI runs near minimum (20 kHz), IWDG and other LSI-dependent timing could be off by 2x.
- **Recommendation:** Consider adding a comment warning users about LSI variability.

### HSE Oscillator Range
- **Port:** `WB32_HSECLK_MIN = 4 MHz`, `WB32_HSECLK_MAX = 16 MHz`
- **Datasheet:** 4-16 MHz external crystal confirmed
- **Status:** Matches datasheet exactly

### LSE Oscillator Range
- **Port:** `WB32_LSECLK_MIN = 32768`, `WB32_LSECLK_MAX = 1000000`
- **Datasheet:** 32.768 kHz for RTC
- **Issue:** Maximum of 1 MHz seems excessive for "low-speed external" oscillator.
- **Recommendation:** Consider reducing max to 32768.

### PLL Input/Output
- **Port:** `WB32_PLLIN_MIN = 2 MHz`, `WB32_PLLOUT_MAX = 96 MHz`
- **Datasheet:** PLL input 1-16 MHz, PLL output max 96 MHz
- **Note:** Reference manual mentions 128 MHz max PLL output, but datasheet confirms system max is 96 MHz
- **Status:** Port correctly limits to 96 MHz

### PLL Multiplier Values
- **Port:** Accepts 12, 16, 20, 24 (validated at `hal_lld.h:418-419`)
- **Reference Manual (ANCTL_PLLCR):** 00=x24, 01=x20, 10=x16, 11=x12
- **Status:** All values correct

### USB Prescaler Values
- **Port (`hal_lld.h:144-149`):**
  - `WB32_USBPRE_DIV1 = 0x0` (DIVEN=0, divider bypassed)
  - `WB32_USBPRE_DIV1P5 = 0x5` (DIVEN=1, RATIO=10)
  - `WB32_USBPRE_DIV2 = 0x1` (DIVEN=1, RATIO=00)
  - `WB32_USBPRE_DIV3 = 0x3` (DIVEN=1, RATIO=01)
- **Reference Manual (RCC_USBPRE):** RATIO: 00=div2, 10=div1.5, others=div3; DIVEN for enable
- **Status:** Correct

### RTC Clock Sources
- **Port (`hal_lld.h:155-160`):** NOCLOCK, LSE, LSI, HSEDIV (bits 8-9)
- **Reference Manual (BKP_BDCR RTCSEL):** 00=none, 01=LSE, 10=LSI, 11=HSE/128
- **Status:** Matches exactly

## Key Clarifications

1. **128 MHz vs 96 MHz:** Reference manual states PLL can output up to 128 MHz, but datasheet confirms system clock maximum is 96 MHz. The port correctly limits to 96 MHz.

2. **FHSI (Fast HSI):** The 48 MHz internal oscillator is available and supported by the port (`WB32_MAINCLKSRC_FHSI`).

3. **Flash Wait States:** At 96 MHz, appropriate wait states are configured in `hal_lld.c`.

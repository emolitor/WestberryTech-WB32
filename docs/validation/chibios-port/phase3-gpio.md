# Phase 3: GPIO Driver Validation


## Summary

| Feature | Port Implementation | Reference Manual | Status |
|---------|---------------------|------------------|--------|
| MODER (mode) | 2-bit: 0-3 | 00-11: Input/Output/AF/Analog | Correct |
| OTYPER (output type) | 1-bit: PP/OD | 0/1: Push-pull/Open-drain | Correct |
| OSPEEDR (speed) | 2-bit: HIGH/LOW | 00/01: High/Low speed | Correct |
| PUPDR (pull) | 2-bit: Float/PU/PD | 00/01/10: None/Up/Down | Correct |
| AFRL/AFRH (alt func) | 4-bit (0-15) | AF0-AF15 | Correct |
| CURRENT (drive) | 2-bit (0-3) | 4/8/12/16 mA | Correct |
| Port width | 16 bits | 16 pins per port | Correct |
| SMIT (Schmitt) | Not exposed | Per-pin enable | Missing |
| CFGMSK | Used internally | Config mask | Used |

## Register Encoding Validation

### GPIOx_MODER (Mode Register)

| Value | Port Constant | Reference Manual | Match |
|-------|---------------|------------------|-------|
| 00 | `PAL_WB32_MODE_INPUT` | Input mode | Correct |
| 01 | `PAL_WB32_MODE_OUTPUT` | GP output mode | Correct |
| 10 | `PAL_WB32_MODE_ALTERNATE` | Alternate function | Correct |
| 11 | `PAL_WB32_MODE_ANALOG` | Analog mode | Correct |

### GPIOx_OTYPER (Output Type)

| Value | Port Constant | Reference Manual | Match |
|-------|---------------|------------------|-------|
| 0 | `PAL_WB32_OTYPE_PUSHPULL` | Push-pull | Correct |
| 1 | `PAL_WB32_OTYPE_OPENDRAIN` | Open-drain | Correct |

### GPIOx_OSPEEDR (Speed)

| Value | Port Constant | Reference Manual | Match |
|-------|---------------|------------------|-------|
| 00 | `PAL_WB32_OSPEED_HIGH` | High speed | Correct |
| 01 | `PAL_WB32_OSPEED_LOW` | Low speed | Correct |
| 10, 11 | Not exposed | Reserved | Correct |

**Note:** Reference manual explicitly states "Others: Reserved" - port correctly exposes only valid values.

### GPIOx_PUPDR (Pull-up/Pull-down)

| Value | Port Constant | Reference Manual | Match |
|-------|---------------|------------------|-------|
| 00 | `PAL_WB32_PUPDR_FLOATING` | No pull | Correct |
| 01 | `PAL_WB32_PUPDR_PULLUP` | Pull-up | Correct |
| 10 | `PAL_WB32_PUPDR_PULLDOWN` | Pull-down | Correct |
| 11 | Not exposed | Reserved | Correct |

### GPIOx_CURRENT (Drive Strength)

| Value | Port Constant | Reference Manual | Match |
|-------|---------------|------------------|-------|
| 00 | `PAL_WB32_CURRENT_LEVEL0` | 4mA | Correct |
| 01 | `PAL_WB32_CURRENT_LEVEL1` | 8mA | Correct |
| 10 | `PAL_WB32_CURRENT_LEVEL2` | 12mA | Correct |
| 11 | `PAL_WB32_CURRENT_LEVEL3` | 16mA | Correct |

## Register Offset Verification

| Register | Offset | Port Uses | Match |
|----------|--------|-----------|-------|
| MODER | 0x00 | Yes | Correct |
| OTYPER | 0x04 | Yes | Correct |
| OSPEEDR | 0x08 | Yes | Correct |
| PUPDR | 0x0C | Yes | Correct |
| IDR | 0x10 | Yes | Correct |
| ODR | 0x14 | Yes | Correct |
| BSRR | 0x18 | Yes | Correct |
| LCKR | 0x1C | Via struct | Correct |
| AFRL | 0x20 | Yes | Correct |
| AFRH | 0x24 | Yes | Correct |
| SMIT | 0x28 | Not used | Missing |
| CURRENT | 0x2C | Yes | Correct |
| CFGMSK | 0x30 | Yes | Correct |

## Missing Features

### Schmitt Trigger (GPIOx_SMIT)

Reference manual (section 8.4.11) documents GPIOx_SMIT register:
- Per-pin Schmitt trigger enable/disable
- Reset value: 0xFFFF (all enabled)

Port does not expose this feature. Users cannot disable Schmitt trigger for analog or special applications.

**Recommendation:** Add `PAL_WB32_SMIT_ENABLE` / `PAL_WB32_SMIT_DISABLE` options.

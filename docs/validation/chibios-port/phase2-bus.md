# Phase 2: Peripheral Bus Assignments Validation


## Summary

| Bus | Port Peripherals | Reference Manual | Status |
|-----|------------------|------------------|--------|
| APB1 | TIM1-4, UART1, QSPI, SPIS1, ADC, EXTI, AFIO, DMAC1, GPIO | Same | Correct |
| APB2 | I2C1-2, UART2-3, SPIM2, SPIS2, DMAC2, WWDG | Same | Correct |
| AHB | USB, FLASH, CACHE, SYS, CRC/SFM, ISO, BKP, DMA bridges | Same | Correct |

## APB1 Peripheral Assignments

**Reference Manual (RCC_APB1ENR at offset 0x048):**

| Bit | Peripheral | Port Support | Status |
|-----|------------|--------------|--------|
| 15 | BMX1EN (bus matrix) | Used in hal_lld.c | Correct |
| 14 | UART1 | rccEnableUART1() | Correct |
| 13 | SPIS1 | rccEnableSPIS1() | Correct |
| 12 | QSPI | rccEnableQSPI() | Correct |
| 11 | ADC | rccEnableADC() | Correct |
| 10 | AFIO | rccEnableEXTI() | Correct |
| 9 | EXTI | rccEnableEXTI() | Correct |
| 8-5 | GPIOD-A | hal_lld.c direct | Correct |
| 4-1 | TIM4-1 | rccEnableTIMx() | Correct |
| 0 | DMAC1 | rccEnableDMAC1() | Correct |

## APB2 Peripheral Assignments

**Reference Manual (RCC_APB2ENR at offset 0x04C):**

| Bit | Peripheral | Port Support | Status |
|-----|------------|--------------|--------|
| 11 | BMX2EN (bus matrix) | Used implicitly | Correct |
| 8 | I2C2 | rccEnableI2C2() | Correct |
| 7 | I2C1 | rccEnableI2C1() | Correct |
| 5 | SPIS2 | rccEnableSPIS2() | Correct |
| 4 | SPIM2 | rccEnableSPIM2() | Correct |
| 3 | UART3 | rccEnableUART3() | Correct |
| 2 | UART2 | rccEnableUART2() | Correct |
| 1 | WWDG | rccEnableWWDG() | Correct (now implemented) |
| 0 | DMAC2 | rccEnableDMAC2() | Correct |

## AHB Peripheral Assignments

**Reference Manual (RCC_AHBENR1 at offset 0x040):**

| Bit | Peripheral | Port Support | Status |
|-----|------------|--------------|--------|
| 8 | CRC/SFM | RCC_AHBPeriph_CRC_SFM | Correct |
| 7 | DMAC2 Bridge | rccEnableDMAC2() | Correct |
| 6 | DMAC1 Bridge | rccEnableDMAC1() | Correct |
| 5 | SYS | RCC_AHBPeriph_SYS | Correct |
| 4 | CACHE | RCC_AHBPeriph_CACHE | Correct |
| 3 | FLASH | RCC_AHBPeriph_FLASH | Correct |
| 2 | ISO7816 | RCC_AHBPeriph_ISO | Correct |
| 1 | USB | RCC_AHBPeriph_USB | Correct |

## Special Cases

### IWDG (Independent Watchdog)
- **Reference Manual:** Has dedicated register `RCC_IWDGCLKENR` at offset 0x064
- **Port:** Defines `RCC_AHBPeriph_IWDG` but no driver implementation
- **Status:** Not fully implemented

### DMA Controllers
- **Finding:** DMA requires enabling both APB clock (DMAC1EN/DMAC2EN) AND AHB bridge clock (DMAC1BREN/DMAC2BREN)
- **Port:** Correctly handles this with compound macros in `rccEnableDMAC1()`/`rccEnableDMAC2()`
- **Status:** Correctly implemented

### GPIO
- **Finding:** GPIO (GPIOA-D) are on APB1, not AHB as some MCUs have
- **Port:** Correctly enables via APB1ENR in `hal_lld.c`
- **Status:** Correctly implemented

## Memory Map Validation

Cross-referenced peripheral base addresses from reference manual Table 2-2:

| Peripheral | Address Range | Bus | Port Matches |
|------------|---------------|-----|--------------|
| GPIOA | 0x4000_0000 | APB1 | Correct |
| TIM1 | 0x4000_1C00 | APB1 | Correct |
| ADC | 0x4000_3C00 | APB1 | Correct |
| UART2 | 0x4000_8000 | APB2 | Correct |
| I2C1 | 0x4000_8800 | APB2 | Correct |
| USB | 0x4001_1000 | AHB | Correct |
| RCC | 0x4001_0800 | AHB | Correct |

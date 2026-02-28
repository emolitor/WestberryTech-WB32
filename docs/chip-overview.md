# WB32 Microcontroller Family Overview

## Company Background

Westberry Technology (ChangZhou) Corp., Ltd. is a fabless semiconductor company based in
Changzhou, China, specializing in low-power, high-performance reconfigurable microcontrollers.
Their product line targets motor control, wearables, automotive peripherals, consumer
electronics, and gaming keyboards.

The gaming keyboard market has become Westberry's primary commercial segment, with WB32
parts appearing in boards from several major manufacturers.

---

## Product Families

| Family | Description | Target Applications |
|--------|-------------|---------------------|
| WB32F101xx | Entry-level | Basic embedded control |
| WB32F102xx | Mid-range with USB | USB peripherals |
| WB32F103xx | Feature-rich | General purpose |
| WB32F104xx | Enhanced | Enhanced peripherals |
| WB32F105xx | Extended temperature range | Industrial |
| WB32FQ95xx | USB keyboard focus | Gaming keyboards (QMK/VIA) |
| WB32F3G71xx | Newer variant | Advanced applications |

### WB32FQ95xx

The WB32FQ95xx is the most widely used variant in the mechanical keyboard community and the
primary target for open-source firmware development:

- **Flash**: Up to 128 KB
- **SRAM**: Up to 36 KB
- **USB**: Full-speed device with ROM DFU bootloader
- **Package**: LQFP48 (pin-compatible with STM32F10x)

---

## Technical Architecture

### Core Specifications

- **CPU Core**: ARM Cortex-M3 (r2p0)
- **Process**: 55nm
- **Max Frequency**: 96 MHz (some variants support 128 MHz)
- **STM32 Compatibility**: Pin-compatible with STM32F10x in LQFP48 package

### Bus Architecture

The WB32 uses a multi-master bus matrix with five masters and three passive units:

```
+----------------------------------------------------------------+
|                       BUS MASTERS (5)                          |
+----------------------------------------------------------------+
|  Cortex-M3  |  DCode  |  System  |  DMA1/DMA2  |   USB DMA   |
|    Core     |   Bus   |   Bus    |             |              |
+-------------+---------+----------+-------------+--------------+
                              |
                              v
                     +------------------+
                     |    Bus Matrix    |
                     +------------------+
                              |
                              v
+----------------------------------------------------------------+
|                     PASSIVE UNITS (3)                           |
+----------------------------------------------------------------+
|     Internal SRAM    |  Internal Flash  |   AHB-APB Bridges    |
+----------------------+------------------+----------------------+
```

A notable architectural feature is the dedicated USB DMA master, which operates independently
from the general-purpose DMA1 and DMA2 controllers.

### Key Differences from STM32F10x

While the WB32FQ95xx is pin-compatible with the STM32F10x, the internal architecture diverges
in several important ways. STM32 code cannot be directly ported; driver adaptation is required.

| Area | STM32F10x | WB32FQ95xx |
|------|-----------|------------|
| DMA Handshake | Fixed peripheral mapping | Different channel assignments |
| DMA LLI | Supported | Not supported (LLP register reserved); use auto-reload or ISR chaining |
| USB DMA | Shared with DMA1/2 | Separate USB DMA master |
| Clock Tree | Single PLL, HSE/HSI | Additional FHSI (48 MHz) option |
| Flash Controller | FPEC registers | FMC registers (different layout) |
| GPIO Speed | 4 levels | 2 levels (LOW/HIGH, inverted encoding) |

---

## Pin Mappings (LQFP48/64)

Pin assignments for all peripherals on the WB32F10x / WB32FQ95xx. All AF mappings below
have been validated against real silicon on both the WB32F104RC and WB32FQ95 (both LQFP64)
using the register-level test suite in `examples/pin-validation/` (148 AF entries, 162
total tests, all passing on both chips -- identical AF mappings confirmed). The WB32 uses
an alternate-function multiplexer (AF0-AF7) rather than the STM32F1-style remap registers.
Each pin's function is selected by its AF number.

Pins marked **(64)** are only available on the LQFP64 package.

### SWD (Serial Wire Debug)

| Pin | Function | AF | Notes |
|-----|----------|----|-------|
| PA13 | SWDIO | Default | Active after reset; also QSPI_NSS1 (AF4) |
| PA14 | SWCLK | Default | Active after reset; also QSPI_NSS2 (AF4) |

### USB

| Pin | Function | Notes |
|-----|----------|-------|
| PA11 | USB D- (USBDM) | Dedicated analog function (not AF-muxed) |
| PA12 | USB D+ (USBDP) | Dedicated analog function (not AF-muxed) |

USB uses a dedicated analog connection, not the AF multiplexer. When USB is not in use,
PA11 and PA12 can be used for other AF functions (TIM1, LED, UART1).

### UART

| Pin | Function | AF | Notes |
|-----|----------|----|-------|
| PA9 | UART1 TX | AF7 | Default UART1 pins |
| PA10 | UART1 RX | AF7 | |
| PA8 | UART1 CK | AF7 | |
| PA11 | UART1 CTS | AF7 | Also TIM1_CH4, LED3 |
| PA12 | UART1 RTS | AF7 | Also TIM1_ETR |
| PB6 | UART1 TX | AF7 | Alternate UART1 pins |
| PB7 | UART1 RX | AF7 | |
| PB8 | UART1 CTS | AF7 | |
| PB9 | UART1 RTS | AF7 | |
| PA2 | UART2 TX | AF7 | |
| PA3 | UART2 RX | AF7 | |
| PA0 | UART2 CTS | AF7 | |
| PA1 | UART2 RTS | AF7 | |
| PA4 | UART2 CK | AF7 | |
| PB10 | UART3 TX | AF7 | Default UART3 pins |
| PB11 | UART3 RX | AF7 | |
| PB12 | UART3 CK | AF7 | **(64)** |
| PB13 | UART3 CTS | AF7 | **(64)** |
| PB14 | UART3 RTS | AF7 | **(64)** |
| PC10 | UART3 TX | AF7 | **(64)** Alternate UART3 pins |
| PC11 | UART3 RX | AF7 | **(64)** |
| PC12 | UART3 CK | AF7 | **(64)** |

### I2C

| Pin | Function | AF | Notes |
|-----|----------|----|-------|
| PB6 | I2C1 SCL | AF5 | Default I2C1 pins |
| PB7 | I2C1 SDA | AF3 | |
| PB8 | I2C1 SCL | AF3 | Alternate I2C1 pins |
| PB9 | I2C1 SDA | AF3 | |
| PB10 | I2C2 SCL | AF3 | Also UART3_TX, TIM2_CH3 |
| PB11 | I2C2 SDA | AF3 | Also UART3_RX, TIM2_CH4 |
| PB5 | I2C1 SMBAI | AF6 | SMBus alert |
| PA15 | I2C SMBAI | AF3 | SMBus alert |

### SPI / QSPI

The WB32 has three SPI-capable peripherals: QSPI (quad-capable), SPIM2 (master), and two
slave peripherals (SPIS1, SPIS2). The QSPI peripheral can be mapped to two different pin
groups.

**QSPI (Port A -- default)**

| Pin | Function | AF | Notes |
|-----|----------|----|-------|
| PA4 | QSPI_NSS0 | AF4 | Also SPIS1_NSS (AF5), ADC_IN4 |
| PA5 | QSPI_SCK | AF4 | Also SPIS1_SCK (AF5), ADC_IN5 |
| PA6 | QSPI_MI (IO1) | AF4 | Also SPIS1_SO (AF5), TIM3_CH1 |
| PA7 | QSPI_MO (IO0) | AF4 | Also SPIS1_SI (AF5), TIM3_CH2 |

**QSPI (Port B -- alternate)**

| Pin | Function | AF | Notes |
|-----|----------|----|-------|
| PA15 | QSPI_NSS0 | AF4 | Also SPIS1_NSS (AF5), TIM2_CH1_ETR |
| PB3 | QSPI_SCK | AF4 | Also SPIS1_SCK (AF5), SWO, TIM2_CH2 |
| PB4 | QSPI_MI (IO1) | AF4 | Also SPIS1_SO (AF5), TIM3_CH1 |
| PB5 | QSPI_MO (IO0) | AF4 | Also SPIS1_SI (AF5), TIM3_CH2, I2S_SD1 |
| PB0 | QSPI_IO2 | AF4 | Quad mode only |
| PB1 | QSPI_IO3 | AF4 | Quad mode only |

**QSPI Chip Selects**

| Pin | Function | AF | Notes |
|-----|----------|----|-------|
| PA13 | QSPI_NSS1 | AF4 | Also SWDIO (default) |
| PA14 | QSPI_NSS2 | AF4 | Also SWCLK (default) |
| PB6 | QSPI_NSS1 | AF4 | |
| PB10 | QSPI_NSS2 | AF4 | |

**SPIM2 (Port B -- LQFP64)**

| Pin | Function | AF | Notes |
|-----|----------|----|-------|
| PB12 | SPIM2_NSS0 | AF5 | **(64)** Also SPIS2_NSS (AF6) |
| PB13 | SPIM2_SCK | AF5 | **(64)** Also SPIS2_SCK (AF6) |
| PB14 | SPIM2_MI | AF5 | **(64)** Also SPIS2_SO (AF6) |
| PB15 | SPIM2_MO | AF5 | **(64)** Also SPIS2_SI (AF6) |

**SPIM2 (Port C -- LQFP64)**

| Pin | Function | AF | Notes |
|-----|----------|----|-------|
| PC0 | SPIM2_NSS0 | AF4 | **(64)** Also SPIS2_NSS (AF5) |
| PC1 | SPIM2_SCK | AF4 | **(64)** Also SPIS2_SCK (AF5) |
| PC2 | SPIM2_MI | AF4 | **(64)** Also SPIS2_SO (AF5) |
| PC3 | SPIM2_MO | AF4 | **(64)** Also SPIS2_SI (AF5) |

**SPIM2 Additional Chip Selects**

| Pin | Function | AF | Notes |
|-----|----------|----|-------|
| PB7 | SPIM2_NSS1 | AF4 | |
| PB8 | SPIM2_NSS2 | AF4 | |
| PB11 | SPIM2_NSS1 | AF4 | |
| PC5 | SPIM2_NSS2 | AF4 | **(64)** |

### Timers

**TIM1 (Advanced Timer)**

| Pin | Function | AF | Notes |
|-----|----------|----|-------|
| PA8 | TIM1_CH1 | AF1 | |
| PA9 | TIM1_CH2 | AF1 | Also UART1_TX (AF7) |
| PA10 | TIM1_CH3 | AF1 | Also UART1_RX (AF7) |
| PA11 | TIM1_CH4 | AF1 | Also USBDM |
| PA12 | TIM1_ETR | AF1 | Also USBDP |
| PA6 | TIM1_BKIN | AF1 | |
| PB12 | TIM1_BKIN | AF1 | **(64)** Alternate BKIN |
| PA7 | TIM1_CH1N | AF1 | Complementary output |
| PB0 | TIM1_CH2N | AF1 | Complementary output |
| PB1 | TIM1_CH3N | AF1 | Complementary output |
| PB13 | TIM1_CH1N | AF1 | **(64)** Alt complementary |
| PB14 | TIM1_CH2N | AF1 | **(64)** Alt complementary |
| PB15 | TIM1_CH3N | AF1 | **(64)** Alt complementary |

**TIM2 (General Purpose)**

| Pin | Function | AF | Notes |
|-----|----------|----|-------|
| PA0 | TIM2_CH1_ETR | AF1 | Also ADC_IN0 |
| PA1 | TIM2_CH2 | AF1 | Also ADC_IN1 |
| PA2 | TIM2_CH3 | AF1 | Also UART2_TX (AF7) |
| PA3 | TIM2_CH4 | AF1 | Also UART2_RX (AF7) |
| PA15 | TIM2_CH1_ETR | AF1 | Alternate CH1/ETR |
| PB3 | TIM2_CH2 | AF1 | Alternate CH2 |
| PB10 | TIM2_CH3 | AF1 | Alternate CH3 |
| PB11 | TIM2_CH4 | AF1 | Alternate CH4 |

**TIM3 (General Purpose)**

| Pin | Function | AF | Notes |
|-----|----------|----|-------|
| PA6 | TIM3_CH1 | AF2 | Also QSPI_MI (AF4) |
| PA7 | TIM3_CH2 | AF2 | Also QSPI_MO (AF4) |
| PB0 | TIM3_CH3 | AF2 | |
| PB1 | TIM3_CH4 | AF2 | |
| PB4 | TIM3_CH1 | AF2 | Alternate CH1 |
| PB5 | TIM3_CH2 | AF2 | Alternate CH2 |
| PC6 | TIM3_CH1 | AF2 | **(64)** Alternate CH1 |
| PC7 | TIM3_CH2 | AF2 | **(64)** Alternate CH2 |
| PC8 | TIM3_CH3 | AF2 | **(64)** Alternate CH3 |
| PC9 | TIM3_CH4 | AF2 | **(64)** Alternate CH4 |
| PD2 | TIM3_ETR | AF2 | **(64)** |

**TIM4 (General Purpose)**

| Pin | Function | AF | Notes |
|-----|----------|----|-------|
| PB6 | TIM4_CH1 | AF2 | Also I2C1_SCL (AF5) |
| PB7 | TIM4_CH2 | AF2 | Also I2C1_SDA (AF3) |
| PB8 | TIM4_CH3 | AF2 | |
| PB9 | TIM4_CH4 | AF2 | |
| PB10 | TIM4_CH1 | AF2 | Alternate CH1 |
| PC12 | TIM4_ETR | AF2 | **(64)** |

### I2S (Inter-IC Sound)

| Pin | Function | AF | Notes |
|-----|----------|----|-------|
| PA15 | I2S_WS | AF2 | Word select |
| PB3 | I2S_SCLK | AF2 | Serial clock |
| PB0 | I2S_MCLK | AF3 | Master clock |
| PB5 | I2S_SD1 | AF3 | Serial data 1 |
| PB12 | I2S_WS | AF2 | **(64)** Alt word select |
| PB13 | I2S_SCLK | AF2 | **(64)** Alt serial clock |
| PB15 | I2S_SD0 | AF2 | **(64)** Serial data 0 |
| PC0 | I2S_WS | AF2 | **(64)** Alt word select |
| PC1 | I2S_SCLK | AF2 | **(64)** Alt serial clock |
| PC2 | I2S_SD0 | AF2 | **(64)** Alt serial data 0 |
| PC3 | I2S_SD1 | AF2 | **(64)** Alt serial data 1 |
| PC6 | I2S_MCLK | AF3 | **(64)** Alt master clock |
| PC7 | I2S_MCLK | AF3 | **(64)** Alt master clock |

### ADC

The ADC requires enabling the ANCTL SAR ADC circuitry before conversions work:
`PWR_UnlockANA()`, `ANCTL_SARADCCmd(ENABLE)`, `PWR_LockANA()`. Additionally,
`ADC_ExternalTrigConvCmd(ENABLE)` is required even for software-triggered conversions.

| Pin | Channel | Notes |
|-----|---------|-------|
| PA0 | ADC_CH0 | |
| PA1 | ADC_CH1 | |
| PA2 | ADC_CH2 | |
| PA3 | ADC_CH3 | |
| PA4 | ADC_CH4 | |
| PA5 | ADC_CH5 | |
| PA6 | ADC_CH6 | |
| PA7 | ADC_CH7 | |
| PB0 | ADC_CH8 | |
| PB1 | ADC_CH9 | |
| PC0 | ADC_CH10 | **(64)** |
| PC1 | ADC_CH11 | **(64)** |
| PC2 | ADC_CH12 | **(64)** |
| PC3 | ADC_CH13 | **(64)** |
| PC4 | ADC_CH14 | **(64)** |
| PC5 | ADC_CH15 | **(64)** |
| - | ADC_CH16 | Internal temperature sensor |
| - | ADC_CH17 | Internal Vref |

### Oscillator

| Pin | Function | Notes |
|-----|----------|-------|
| PD0 | XTAL_IN | External crystal input (12 MHz typical) |
| PD1 | XTAL_OUT | External crystal output |

### LED Controller

The WB32 has a dedicated hardware LED controller. LED outputs can be mapped to multiple
pin groups. The segment code API (`LED_SetSegmentCode`/`LED_GetSegmentCode`) applies an
internal bit-reordering transform.

**Group 1: PA8-PA11 (AF5) -- LED0-LED3**

| Pin | Function | AF |
|-----|----------|----|
| PA8 | LED0 | AF5 |
| PA9 | LED1 | AF5 |
| PA10 | LED2 | AF5 |
| PA11 | LED3 | AF5 |

**Group 2: PB12-PB15 (AF4) -- LED4-LED7 (LQFP64)**

| Pin | Function | AF |
|-----|----------|----|
| PB12 | LED4 | AF4 |
| PB13 | LED5 | AF4 |
| PB14 | LED6 | AF4 |
| PB15 | LED7 | AF4 |

**Group 3: PC6-PC9 (AF5) -- LED0-LED3 (LQFP64)**

| Pin | Function | AF |
|-----|----------|----|
| PC6 | LED0 | AF5 |
| PC7 | LED1 | AF5 |
| PC8 | LED2 | AF5 |
| PC9 | LED3 | AF5 |

**Group 4: PC10-PC12, PD2 (AF5) -- LED4-LED7 (LQFP64)**

| Pin | Function | AF |
|-----|----------|----|
| PC10 | LED4 | AF5 |
| PC11 | LED5 | AF5 |
| PC12 | LED6 | AF5 |
| PD2 | LED7 | AF5 |

### Alternate Function Summary

| AF | Function |
|----|----------|
| AF0 | System (default after reset) |
| AF1 | TIM1 / TIM2 |
| AF2 | TIM3 / TIM4 / I2S (WS, SCLK, SD0, SD1) |
| AF3 | I2C1 (SDA, alt SCL) / I2C2 / I2S (MCLK, SD1) / I2C SMBAI |
| AF4 | QSPI / SPIM2 (Port C) / LED (PB12-PB15 only) |
| AF5 | SPIS1 / SPIM2 (Port B) / SPIS2 (Port C) / LED (PA, PC, PD pins) / I2C1 SCL (PB6) |
| AF6 | SPIS2 (Port B) / I2C1 SMBAI (PB5) |
| AF7 | UART1 / UART2 / UART3 |

---

## STM32 Comparison

| Feature | WB32FQ95xx | STM32F103xx |
|---------|------------|-------------|
| Core | Cortex-M3 | Cortex-M3 |
| Max Clock | 96-128 MHz | 72 MHz |
| Flash | Up to 128 KB | Up to 512 KB |
| SRAM | Up to 36 KB | Up to 64 KB |
| USB | Full-speed | Full-speed |
| DMA Channels | 6 (2 controllers) | 7-12 |
| Package | LQFP48/64 | LQFP48/64/100 |

---

## Commercial Users

WB32 microcontrollers are used in production keyboards from several manufacturers:

- **GMMK** (Glorious)
- **Akko**
- **MonsGeek**
- **Inland**
- **Shortcut Studio** (Bridge75 uses WB32FQ95 + CH582F for wireless)
- **QK/Neo** (Neo Gen 1 boards use WB32FQ95 + CH582F for wireless)
- **Polygon** (PG7 uses WB32FQ95 + CH582F for wireless)

---

## Software Ecosystem

### Firmware Frameworks

| Framework | WB32 Support |
|-----------|-------------|
| QMK Firmware | Official support for WB32FQ95xx and WB32F3G71xx |
| ChibiOS | Community port in ChibiOS-Contrib |
| Bare Metal | Vendor Standard Peripheral Library |

### Development Tools

| Tool | Purpose |
|------|---------|
| `wb32-dfu-updater` | Official USB DFU flash tool (bundled with QMK MSYS) |
| OpenOCD | Debug and flash via SWD (custom driver required) |
| J-Link | SWD debugging |
| WB-Link PRO | Official Westberry debug probe |

---

## Resources

### Official
- [Westberry Technology Website](https://www.westberrytech.com/)
- [WestberryTech GitHub](https://github.com/WestberryTech/)
- [wb32-dfu-updater](https://github.com/WestberryTech/wb32-dfu-updater)

### Community
- [QMK Compatible Microcontrollers](https://docs.qmk.fm/compatible_microcontrollers)
- [qmk/ChibiOS-Contrib WB32 Port](https://github.com/qmk/ChibiOS-Contrib/tree/master/os/hal/ports/WB32)

### Sourcing Chips and Devboards
- [Westberry Taobao](https://world.taobao.com/dianpu/471165391.htm?spm=a21wu.12321156-tw/qwen.shop-area.1.59ad6dc0ANXkNx)

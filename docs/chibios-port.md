# ChibiOS-Contrib WB32FQ95 Port

## Overview

The ChibiOS-Contrib project includes a community-maintained HAL port for the Westberry
WB32FQ95xx ARM Cortex-M3 microcontroller family. The port provides a full set of low-level
drivers for the WB32's peripherals, adapted to ChibiOS's Hardware Abstraction Layer (HAL)
conventions.

The port lives in the ChibiOS-Contrib tree at `os/hal/ports/WB32/`.

---

## Port Directory Structure

```
ChibiOS-Contrib/os/hal/ports/WB32/
├── WB32FQ95xx/               # Platform-specific HAL
│   ├── hal_lld.c/h           # HAL low-level driver (clock init)
│   ├── wb32_registry.h       # Peripheral capability flags
│   ├── wb32_rcc.h            # RCC helper macros
│   ├── wb32_isr.c/h          # Interrupt handlers
│   └── hal_efl_lld.c/h       # Embedded flash driver
└── LLD/                      # Shared low-level drivers
    ├── GPIOv1/               # GPIO/PAL driver
    ├── UARTv1/               # UART serial driver
    ├── SPIv1/                # SPI driver
    ├── I2Cv1/                # I2C driver
    ├── USBv1/                # USB full-speed device
    ├── ADCv1/                # 12-bit ADC
    ├── DMAv1/                # DMA controller
    ├── TIMv1/                # Timers (PWM, GPT, ICU)
    ├── RTCv1/                # Real-time clock
    ├── EXTIv1/               # External interrupts
    └── xWDGv1/               # Window watchdog driver
```

---

## Implemented Drivers

| Driver | Peripherals | Notes |
|--------|-------------|-------|
| GPIO/PAL | GPIOA-D | 4 ports, 16 pins each, alternate functions |
| UART | UART1-3 | Hardware FIFO, flow control (CTS/RTS/AFC) |
| SPI | QSPI, SPIM2, SPIS1/2 | Master and slave modes |
| I2C | I2C1-2 | Standard/fast mode, busy timeout |
| USB | USB1 | Full-speed device, 3 endpoints, PIO mode, remote wakeup |
| ADC | ADC | 16 external channels + internal temp sensor + Vref |
| DMA | DMAC1-2 | 3 channels per controller (6 total), max 511 transfers per block (see [silicon-errata.md](silicon-errata.md)) |
| Timer | TIM1-4 | PWM, GPT, ICU modes, 4 channels per timer, 20-bit counters |
| RTC | RTC | Alarm support, LSE/LSI/HSE-divided clock sources |
| EXTI | EXTI0-18 | 19 interrupt lines, configurable edge detection |
| Flash | EFL | 256-byte sector granularity, erases to 0x00 |
| WDG | WWDG | Window watchdog with early wakeup interrupt |

---

## Architecture Notes

### Bus Architecture

The WB32 uses a multi-master bus matrix:

- **5 Bus Masters**: Cortex-M3 core, DCode bus, System bus, DMA1/DMA2, USB DMA
- **3 Passive Units**: Internal SRAM, Internal Flash, AHB-to-APB bridges

The dedicated USB DMA master is a significant architectural difference from STM32, where USB
shares the general-purpose DMA controllers.

### Why STM32 Assumptions Do Not Apply

The WB32FQ95xx is pin-compatible with the STM32F10x in LQFP48 packages, but the internal
architecture diverges in ways that prevent direct code porting:

| Area | STM32F10x | WB32FQ95xx |
|------|-----------|------------|
| DMA Handshake | Fixed peripheral mapping | Different channel assignments |
| USB DMA | Shared with DMA1/2 | Separate USB DMA master |
| Clock Tree | Single PLL, HSE/HSI | Additional FHSI (48 MHz) oscillator |
| Flash Controller | FPEC registers | FMC registers (different layout, inverted bit model) |
| GPIO Speed | 4 levels (0=low, 3=high) | 2 levels (0=high, 1=low -- inverted encoding) |

### ChibiOS Driver Adaptations

The LLD (Low-Level Driver) layer handles these hardware differences:

- **DMAv1**: Custom handshake mapping table for WB32 peripheral-to-channel assignments. DMA
  block transfers are limited to 511 elements per the hardware errata; drivers requiring
  larger transfers must implement ISR-based block chaining.
- **USBv1**: Uses PIO (Programmed I/O) mode exclusively. The WB32's USB controller has a
  dedicated 1024-byte FIFO and its own DMA master, but the controller does not expose
  general-purpose DMA integration. All USB data movement goes through CPU-driven FIFO
  load/unload.
- **hal_lld.c**: WB32-specific clock initialization sequence, including support for the FHSI
  (48 MHz Fast HSI) oscillator and the WB32's PLL configuration registers.
- **GPIOv1**: Simplified speed settings (2 levels instead of STM32's 4), with inverted
  polarity -- OSPEEDR=0 means high speed on WB32.

---

## Setup

### Enabling the Port

In your project's `halconf.h`, enable the drivers you need:

```c
#define HAL_USE_PAL                 TRUE
#define HAL_USE_SERIAL              TRUE
#define HAL_USE_USB                 TRUE
/* ... other drivers as needed ... */
```

In `mcuconf.h`, configure the MCU-specific settings:

```c
#define WB32_PLLMF_VALUE            8       /* PLL multiplier */
#define WB32_USBPRE                 WB32_USBPRE_DIV1P5
#define WB32_SERIAL_USE_UART1       TRUE
/* ... other peripheral configurations ... */
```

### Demo Project

A ready-to-build demo is available at:

```
ChibiOS-Contrib/demos/WB32/RT-WB32FQ95-GENERIC/
```

Build:

```bash
cd ChibiOS-Contrib/demos/WB32/RT-WB32FQ95-GENERIC
make
```

This produces an ELF binary that can be flashed via SWD (OpenOCD, J-Link) or USB DFU.

---

## Known Limitations

1. **Clock Configuration**: The port configures up to 96 MHz system clock. 128 MHz operation
   (supported by some WB32 variants) has not been verified and may require additional PLL
   validation.

2. **SPI Circular Mode**: Disabled (`SPI_SUPPORTS_CIRCULAR = FALSE`). Non-circular transfers
   work correctly.

3. **USB Endpoint Count**: Fixed at 3 endpoints. The hardware may support additional
   endpoints, but the driver has not been extended beyond 3.

4. **Missing Peripheral Drivers**:
   - IWDG (Independent Watchdog) -- registry declares `WB32_HAS_IWDG = TRUE` but no driver
     exists
   - CRC hardware accelerator -- no driver
   - I2S audio interface -- no driver

5. **Power Management**: Sleep, Stop, and Standby modes are not fully exposed through the
   HAL. Low-power entry/exit requires direct register access.

---

## Bug Fixes

Several bugs have been identified and fixed in the upstream ChibiOS-Contrib WB32 port code.
See [chibios-bug-fixes.md](chibios-bug-fixes.md) for the complete list of 11 fixes with
code-level details.

Fixes are available on the `em-wb32-improvements` branch at
`https://github.com/emolitor/ChibiOS-Contrib`.

---

## Validation

All port validation phases have been completed and passed. The port has been tested in
production firmware (Bridge75 keyboard, 81 WS2812 LEDs, wireless operation) and is
considered production-ready for typical embedded applications.

See [`docs/validation/`](validation/) for individual phase results covering clock
configuration, GPIO, UART, SPI, I2C, USB, ADC, DMA, timers, and flash operations.

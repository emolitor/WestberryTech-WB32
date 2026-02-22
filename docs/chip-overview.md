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
| USB DMA | Shared with DMA1/2 | Separate USB DMA master |
| Clock Tree | Single PLL, HSE/HSI | Additional FHSI (48 MHz) option |
| Flash Controller | FPEC registers | FMC registers (different layout) |
| GPIO Speed | 4 levels | 2 levels (LOW/HIGH, inverted encoding) |

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

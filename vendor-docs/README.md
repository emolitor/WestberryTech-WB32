# Vendor Documentation

Westberry Technology documentation for the WB32F10x and WB32FQ95xx
microcontroller families.

## Directory Structure

```
vendor-docs/
├── datasheets/              Datasheet PDFs for each WB32 variant
├── reference-manual/        WB32FQ95xx reference manual
│   ├── *.pdf                Full reference manual
│   └── chapters/            Searchable text extracts (26 chapters)
├── application-notes/       Official application notes
└── schematics/              Reference board schematic
```

## Datasheets

| Document | MCU Family |
|----------|------------|
| DS001_WB32F101xx_DataSheet_EN_V4.0.pdf | WB32F101xx -- Entry-level, up to 64 KB flash, 20 KB SRAM |
| DS002_WB32F102xx_DataSheet_EN_V4.0.pdf | WB32F102xx -- Mid-range with USB |
| DS003_WB32F103xx_DataSheet_EN_V4.0.pdf | WB32F103xx -- Feature-rich with USB |
| DS004_WB32F104xx_DataSheet_EN_V4.0.pdf | WB32F104xx -- Enhanced features |
| DS005_WB32F105xx_DataSheet_EN_V4.0.pdf | WB32F105xx -- Extended temperature range |
| EN_DS1104041_WB32FQ95xC_V01.pdf | WB32FQ95xC -- High-performance, 128 KB flash, USB |

## Reference Manual

The WB32FQ95xx reference manual is provided both as the original PDF and as 26
searchable plain-text chapter extracts in `reference-manual/chapters/`.

### Chapter Index

| File | Topic | Key Content |
|------|-------|-------------|
| `00_Front_Matter.txt` | TOC, Figure/Table Lists | Navigation reference |
| `01_Documentation_Conventions.txt` | Abbreviations | Register notation |
| `02_System_and_Memory_Architecture.txt` | CPU, Memory Map | ARM Cortex-M3, bit-banding, flash/SRAM layout |
| `03_System_Configuration_SYS.txt` | SYS Registers | Security, write protection, boot config |
| `04_Cyclic_Redundancy_Check_CRC.txt` | CRC Unit | Hardware CRC calculation |
| `05_Power_Control_PWR.txt` | Power Management | Sleep/Stop/Standby modes, PVD, voltage regulator |
| `06_Backup_Registers_BKP.txt` | Backup Domain | RTC calibration, tamper detection |
| `07_Reset_and_Clock_Control_RCC.txt` | Clock Tree | HSE/HSI/PLL/LSE/LSI, clock security |
| `08_GPIO_and_AFIO.txt` | GPIO Configuration | Pin modes, alternate functions, EXTI mapping |
| `09_NVIC.txt` | Interrupt Controller | Priority levels, vector table |
| `10_EXTI_Interrupt_Event_Controller.txt` | External Interrupts | Edge detection, wakeup |
| `11_Analog_Control_ANCTL.txt` | Analog Peripherals | Comparators, internal references |
| `12_Special_Function_Macro_SFM.txt` | Special Functions | Hardware accelerators |
| `13_DMA_Controller.txt` | DMA | Channels, triggers, memory-to-peripheral |
| `14_ADC.txt` | ADC | 12-bit SAR ADC, channels, conversion modes |
| `15_Advanced_Timer_TIM1.txt` | TIM1 | PWM, capture/compare, dead-time, motor control |
| `16_General_Purpose_Timers_TIM2-TIM4.txt` | TIM2-4 | General timers, encoder interface |
| `17_Real_Time_Clock_RTC.txt` | RTC | Time keeping, alarms, calibration |
| `18_Watchdog_Timer_IWDG.txt` | Independent WDG | System reset on timeout |
| `19_Window_Watchdog_Timer_WWDG.txt` | Window WDG | Windowed watchdog |
| `20_Flash_Controller_FMC.txt` | Flash Programming | Erase/program operations |
| `21_USB_Full_Speed_Device.txt` | USB Device | Full-speed USB, endpoints, descriptors |
| `22_SPI.txt` | SPI | Master/slave, DMA support, QSPI |
| `24_I2C.txt` | I2C | Master/slave, SMBus, addressing modes |
| `25_UART.txt` | UART | Async serial, baud rate, DMA |
| `26_Device_Electronic_Signature.txt` | Device ID | Unique ID, flash size |
| `27_Debug_DBG.txt` | Debug Interface | SWD, debug registers |

## Application Notes

| Document | Topic |
|----------|-------|
| AN001_WB32F10x_12Bit_ADC_Application_Note_EN_V1.1.pdf | 12-bit ADC usage and calibration |
| AN002_WB32F10x_Getting_Started_Development_EN_V1.2.pdf | Getting started with WB32 development |
| AN003_WB32F10x_PCB_Layout_Suggestions_EN_V1.4.pdf | PCB layout guidelines for WB32 |
| AN004_WB32F10x_Program_Download_Guide_EN_V1.3.pdf | Firmware programming procedures |
| AN005_WB32F10x_Set_Up_A_GCC_Development_Environment_EN_V1.2.pdf | GCC toolchain setup |

## Schematics

| Document | Description |
|----------|-------------|
| Schematic_wb32f10x_48pin_Mini_v1.1.pdf | WB32F10x 48-pin Mini reference board schematic |

## Searching the Reference Manual

The plain-text chapter files in `reference-manual/chapters/` support fast
searching with standard tools.

### Register Definitions

```bash
# Search for a specific register
grep -r "RCC_CR" reference-manual/chapters/

# All registers for a peripheral
grep -r "PWR_" reference-manual/chapters/ | head -20
```

### Bit Field Definitions

```bash
# Find bit field definitions
grep -r "Bits \[" reference-manual/chapters/

# Find reset values
grep -ri "reset value" reference-manual/chapters/
```

### Alternate Functions and Pin Assignments

```bash
# GPIO alternate function mappings
grep -ri "alternate function" reference-manual/chapters/08_GPIO_and_AFIO.txt

# Pin assignments for a specific pin
grep -ri "PA[0-9]" reference-manual/chapters/08_GPIO_and_AFIO.txt
```

### Peripheral Base Addresses

```bash
grep -ri "base address" reference-manual/chapters/02_System_and_Memory_Architecture.txt
```

## External Resources

### Official Sources

- [Westberry Technology](https://www.westberrytech.com/) -- Product pages and documentation
- [WestberryTech GitHub](https://github.com/WestberryTech/) -- SDK, tools, firmware examples
- [wb32-dfu-updater](https://github.com/WestberryTech/wb32-dfu-updater) -- Official DFU flash tool
- [qmk_westberry](https://github.com/WestberryTech/qmk_westberry) -- Westberry's QMK fork

### QMK and ChibiOS Community

- [QMK Compatible Microcontrollers](https://docs.qmk.fm/compatible_microcontrollers) -- WB32 support status
- [QMK Flashing Documentation](https://docs.qmk.fm/flashing) -- Includes WB32-DFU instructions
- [ChibiOS-Contrib WB32 Port](https://github.com/qmk/ChibiOS-Contrib/tree/master/os/hal/ports/WB32) -- Upstream ChibiOS port

### Component Distributors

- [JLCPCB](https://jlcpcb.com/) -- Search for WB32FQ95 for part availability and pricing

### Chinese Technical Sources

- CSDN Blog: Search "WB32库开发教程" for library development tutorials
- 21ic电子技术开发论坛: WB32 discussion threads

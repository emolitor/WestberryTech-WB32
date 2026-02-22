# Vendor Documentation

Westberry Technology documentation for the WB32F10x and WB32FQ95xx
microcontroller families.

## Directory Structure

```
vendor-docs/
├── datasheets/              Datasheet PDFs for each WB32 variant
├── reference-manual/        WB32FQ95xx reference manual
│   └── *.pdf                Full reference manual
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

The WB32FQ95xx reference manual is provided as the original PDF.

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

### Chinese Technical Sources

- CSDN Blog: Search "WB32库开发教程" for library development tutorials
- 21ic电子技术开发论坛: WB32 discussion threads

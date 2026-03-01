# Getting Started with WB32 Development

This guide covers toolchain setup, building bare metal firmware, flashing, and debugging
for WB32F10x and WB32FQ95xx microcontrollers.

---

## Prerequisites

| Tool | Purpose |
|------|---------|
| `arm-none-eabi-gcc` | ARM cross-compiler toolchain |
| `make` | Build system |
| OpenOCD (with WB32 patch) | Flash and debug via SWD |
| `wb32-dfu-updater` | Flash via USB DFU (no debug probe required) |
| Debug probe | CMSIS-DAP, J-Link, or WB-Link PRO (for SWD access) |

---

## Toolchain Setup

### Ubuntu / Debian

```bash
sudo apt install gcc-arm-none-eabi binutils-arm-none-eabi make
```

### macOS (Homebrew)

```bash
brew install --cask gcc-arm-embedded
```

### Windows

Download the [GNU Arm Embedded Toolchain](https://developer.arm.com/downloads/-/gnu-rm)
installer and add the `bin/` directory to your PATH. Alternatively, use the toolchain bundled
with QMK MSYS.

### Verify Installation

```bash
arm-none-eabi-gcc --version
```

---

## First Bare Metal Project (Blink)

The [`examples/blink/`](../examples/blink/) directory contains a minimal LED blink project that demonstrates the
bare metal build setup.

### Project Files

| File | Description |
|------|-------------|
| `main.c` | Application entry point |
| `wb32f10x_it.c` | Interrupt service routines |
| `wb32f10x_conf.h` | Peripheral library configuration |
| `WB32F10x_FLASH.ld` | Linker script (memory layout) |
| `Makefile` | Build configuration |

### Building

```bash
cd examples/blink
make
```

The Makefile expects the vendor Standard Peripheral Library at [`vendor-lib/`](../vendor-lib/). Output files
are placed in `build/`:

- `build/Template.elf` -- ELF with debug symbols
- `build/Template.hex` -- Intel HEX format
- `build/Template.bin` -- Raw binary for flashing

To use a toolchain installed in a non-standard location:

```bash
make GCC_PATH=/path/to/arm-none-eabi/bin
```

---

## Architecture Overview

WB32 bare metal projects follow a layered architecture:

```
Application (main.c)
        |
        v
Standard Peripheral Library (vendor-lib/)
        |
        v
CMSIS Layer (vendor-lib/.../CMSIS/)
        |
        v
ARM Cortex-M3 Hardware
```

### Key Files in Every Project

| File | Role |
|------|------|
| `main.c` | Application logic and peripheral initialization |
| `wb32f10x_it.c` | Interrupt handlers (SysTick, DMA, UART, etc.) |
| `wb32f10x_conf.h` | Selects which peripheral library headers to include |
| `WB32F10x_FLASH.ld` | Linker script defining flash and SRAM regions |

### Critical Build Defines

These preprocessor defines must be set in the Makefile or compiler flags:

| Define | Value | Purpose |
|--------|-------|---------|
| `USE_STDPERIPH_DRIVER` | (defined) | Enables the vendor peripheral library |
| `HSE_VALUE` | `12000000` | External crystal frequency (12 MHz) |
| `MAINCLK_FREQ_96MHz` | (defined) | System clock target (options: 32/48/64/72/96 MHz) |

---

## Flashing

Two methods are available: **OpenOCD** via an SWD debug probe, and **USB DFU** using the ROM bootloader. See [flashing.md](flashing.md) for the complete guide including backup, verification, mass erase, helper commands, and troubleshooting.

### Quick Reference: OpenOCD

```bash
openocd -f interface/cmsis-dap.cfg -f target/wb32.cfg \
  -c "init; halt; flash probe 0" \
  -c "flash write_image erase firmware.bin 0x08000000" \
  -c "verify_image firmware.bin 0x08000000" \
  -c "reset run; shutdown"
```

Requires the [emolitor/openocd](https://github.com/emolitor/openocd) fork with WB32 flash driver.

### Quick Reference: USB DFU

```bash
wb32-dfu-updater_cli -t -s 0x8000000 -D firmware.bin
```

Put the device in DFU mode first (QK_BOOT keycode, RESET button, or software reset). Tool available from [WestberryTech/wb32-dfu-updater](https://github.com/WestberryTech/wb32-dfu-updater).

---

## Debug Setup

### OpenOCD + GDB

Start the OpenOCD server:

```bash
openocd -f interface/cmsis-dap.cfg -f target/wb32.cfg
```

In a separate terminal, connect with GDB:

```bash
arm-none-eabi-gdb build/Template.elf
(gdb) target remote :3333
(gdb) monitor halt
(gdb) load
(gdb) break main
(gdb) continue
```

### J-Link

Connect via J-Link GDB Server:

```bash
JLinkGDBServer -device WB32F103 -if SWD -speed 4000
```

Then connect GDB to port 2331.

### Linux udev Rules

To allow non-root access to Westberry USB devices, create
`/etc/udev/rules.d/50-wb32.rules`:

```
# Westberry WB32 DFU
SUBSYSTEM=="usb", ATTR{idVendor}=="342d", MODE="0666"
```

Reload rules:

```bash
sudo udevadm control --reload-rules
```

---

## ChibiOS Projects

ChibiOS RTOS support for WB32 is provided through the community ChibiOS-Contrib repository.
Building a ChibiOS project requires both the main ChibiOS source tree and ChibiOS-Contrib.

See [chibios-port.md](chibios-port.md) for driver inventory and port details.

A generic demo is available:

```bash
cd ChibiOS-Contrib/demos/WB32/RT-WB32FQ95-GENERIC
make
```

---

## Memory Map

| Region | Start Address | End Address | Size | Notes |
|--------|---------------|-------------|------|-------|
| Flash | `0x08000000` | `0x0801FFFF` | Up to 256 KB | User application code and data |
| SRAM | `0x20000000` | `0x20008FFF` | Up to 36 KB | Variables, stack, heap |
| ROM Bootloader | `0x1FFFF000` | `0x1FFFFFFF` | 4 KB | Factory DFU bootloader (read-only) |

Flash and SRAM sizes vary by device variant. The ROM bootloader is always present and cannot
be erased, providing a guaranteed recovery path.

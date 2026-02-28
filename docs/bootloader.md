# WB32 DFU Bootloader

## Overview

WB32 microcontrollers include a ROM-based DFU bootloader that does not consume user flash space and is always available for firmware updates and recovery.

## Key Features

- **No flash required** -- the bootloader is stored in ROM and cannot be erased or overwritten
- **USB DFU protocol** -- uses the standard USB Device Firmware Upgrade protocol
- **QMK compatible** -- works with QMK Toolbox and the official `wb32-dfu-updater` tool
- **Always recoverable** -- even with completely corrupted firmware, the device can always be reflashed via the ROM bootloader

## Entering DFU Mode

### Method 1: QK_BOOT Keycode (Keyboards)

For QMK keyboards, press the `QK_BOOT` keycode. This is typically mapped to a key combination defined in the keyboard's keymap.

### Method 2: RESET Button

Hold the RESET/BOOT button while either:
1. Powering on the device, or
2. Pressing the hardware reset button

### Method 3: Software Reset

From running firmware, trigger a reset into the bootloader:
```c
NVIC_SystemReset();  // With bootloader flag set
```

### Verification

When in DFU mode, the device enumerates as a USB device with:
- **VID**: 0x342D (Westberry Technology)
- **PID**: Varies by device

Verify with `lsusb` on Linux or Device Manager on Windows.

## Flashing Tools

### wb32-dfu-updater (Official)

The official Westberry flashing tool, available from [WestberryTech/wb32-dfu-updater](https://github.com/WestberryTech/wb32-dfu-updater). Also bundled with QMK MSYS on Windows.

**Basic flash command:**
```bash
wb32-dfu-updater_cli -t -s 0x8000000 -D firmware.bin
```

**Command options:**

| Option | Description |
|--------|-------------|
| `-t` | Transfer mode |
| `-s 0x8000000` | Start address (flash base) |
| `-D <file>` | Download (flash) file to device |
| `-U <file>` | Upload (read) from device to file |

### QMK Toolbox

QMK Toolbox automatically detects WB32 devices in DFU mode and can flash `.bin` or `.hex` files.

1. Open QMK Toolbox
2. Select the firmware file
3. Put the keyboard in DFU mode
4. Click "Flash"

### dfu-util (Generic)

The standard `dfu-util` tool may work, but `wb32-dfu-updater` is recommended for reliability with WB32 devices.

## QMK Configuration

### info.json Bootloader Setting

```json
{
    "bootloader": "wb32-dfu"
}
```

### Processor Values

```json
{
    "processor": "WB32FQ95"
}
```

Or for newer variants:
```json
{
    "processor": "WB32F3G71"
}
```

## OpenOCD Alternative

For development and debugging, OpenOCD with an SWD debug probe provides more control than USB DFU. See the [emolitor/openocd](https://github.com/emolitor/openocd) fork for WB32-specific configuration and build instructions.

**Flash via OpenOCD:**
```bash
openocd -f interface/cmsis-dap.cfg -f target/wb32.cfg \
  -c "init; halt; flash probe 0" \
  -c "flash write_image erase firmware.bin 0x08000000" \
  -c "verify_image firmware.bin 0x08000000" \
  -c "reset run; shutdown"
```

**Advantages of OpenOCD over DFU:**
- Full debug access (breakpoints, watchpoints, memory inspection)
- Flash verification after write
- Works even if USB is non-functional on the target
- Faster iteration during development

## Troubleshooting

### Device Not Detected on Linux

Add a udev rule for Westberry devices:

```
# /etc/udev/rules.d/50-wb32.rules
SUBSYSTEM=="usb", ATTR{idVendor}=="342d", MODE="0666"
```

Reload rules: `sudo udevadm control --reload-rules`

## Memory Map

| Region | Start Address | End Address | Size |
|--------|---------------|-------------|------|
| Flash | 0x08000000 | 0x0801FFFF | 128 KB (WB32FQ95) |
| SRAM | 0x20000000 | 0x20008FFF | 36 KB |
| ROM (Bootloader) | 0x1FFFF000 | 0x1FFFFFFF | 4 KB |

The ROM bootloader resides in system memory, separate from and independent of user flash.

## Resources

- [wb32-dfu-updater Repository](https://github.com/WestberryTech/wb32-dfu-updater)
- [QMK Flashing Documentation](https://docs.qmk.fm/flashing)
- [QMK Toolbox](https://github.com/qmk/qmk_toolbox)

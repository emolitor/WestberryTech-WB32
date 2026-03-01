# Flashing WB32 Firmware

Two methods are available for flashing WB32 microcontrollers: **OpenOCD** via an SWD debug probe, and **USB DFU** using the ROM bootloader. This guide covers both.

## Choosing a Method

| | OpenOCD (SWD) | USB DFU |
|---|---|---|
| **Hardware** | Debug probe (CMSIS-DAP, J-Link, WB-Link) | USB cable only |
| **Debug access** | Full (breakpoints, memory, registers) | None |
| **Flash verification** | Built-in `verify_image` | Not available |
| **Speed** | Slower (host-side register writes) | Faster |
| **Recovery** | Works even with non-functional USB | Requires USB enumeration |
| **Best for** | Development and debugging | Production flashing, keyboards |

---

## OpenOCD

WB32 requires a custom flash driver not yet in upstream OpenOCD. Use the [emolitor/openocd](https://github.com/emolitor/openocd) fork.

### Building OpenOCD

```bash
git clone https://github.com/emolitor/openocd.git
cd openocd
git checkout wb32
./bootstrap
./configure --enable-cmsis-dap --enable-cmsis-dap-v2 \
            --enable-internal-jimtcl --enable-internal-libjaylink
make
```

The binary is at `src/openocd`. Run `./configure --help` for additional adapter flags (J-Link, ST-Link, etc.).

### Debug Probe Configuration

The unified `target/wb32.cfg` works for all WB32 families. Pair it with the interface config for your probe:

| Probe | Interface Config |
|-------|-----------------|
| CMSIS-DAP | `interface/cmsis-dap.cfg` |
| J-Link | `interface/jlink.cfg` |
| WB-Link PRO | `interface/cmsis-dap.cfg` |

### Device Identification

Connect and query the device before flashing:

```bash
openocd -f interface/cmsis-dap.cfg -f target/wb32.cfg \
  -c "init; halt; wb32_info; shutdown"
```

This reads `SYS_ID` and `SYS_MEMSZ` registers and displays the chip family, flash size, and SRAM size.

### Backup Existing Firmware

Always back up before flashing new firmware:

```bash
# Dump first 4 KB (default)
openocd -f interface/cmsis-dap.cfg -f target/wb32.cfg \
  -c "init; halt; wb32_dump_flash backup.bin; shutdown"

# Dump entire flash (auto-detects size)
openocd -f interface/cmsis-dap.cfg -f target/wb32.cfg \
  -c "init; halt; wb32_dump_all_flash backup.bin; shutdown"

# Dump specific length
openocd -f interface/cmsis-dap.cfg -f target/wb32.cfg \
  -c "init; halt; wb32_dump_flash backup.bin 0x40000; shutdown"
```

### Flash Firmware

```bash
openocd -f interface/cmsis-dap.cfg -f target/wb32.cfg \
  -c "init; halt; flash probe 0" \
  -c "flash write_image erase firmware.bin 0x08000000" \
  -c "verify_image firmware.bin 0x08000000" \
  -c "reset run; shutdown"
```

The `flash write_image erase` command erases only the sectors needed before writing. The `verify_image` step reads back the flash and compares it against the source file.

### Verify Without Flashing

```bash
openocd -f interface/cmsis-dap.cfg -f target/wb32.cfg \
  -c "init; halt; wb32_verify_flash firmware.bin; shutdown"
```

### Mass Erase

Erase the entire flash (all sectors):

```bash
openocd -f interface/cmsis-dap.cfg -f target/wb32.cfg \
  -c "init; halt; flash probe 0" \
  -c "wb32f10x mass_erase 0" \
  -c "shutdown"
```

Note: WB32 flash erases to `0x00`, not `0xFF` like STM32.

### Restore a Backup

```bash
openocd -f interface/cmsis-dap.cfg -f target/wb32.cfg \
  -c "init; halt; flash probe 0" \
  -c "flash write_image erase backup.bin 0x08000000" \
  -c "verify_image backup.bin 0x08000000" \
  -c "reset run; shutdown"
```

### Interactive Session

For iterative development, start OpenOCD as a server and connect via telnet:

```bash
# Terminal 1: Start OpenOCD server
openocd -f interface/cmsis-dap.cfg -f target/wb32.cfg

# Terminal 2: Connect via telnet
telnet localhost 4444
> halt
> flash probe 0
> flash write_image erase firmware.bin 0x08000000
> verify_image firmware.bin 0x08000000
> reset run
```

This avoids the connection/disconnection overhead of one-shot commands during repeated flash cycles.

### Helper Commands

The `wb32_common.tcl` helpers are available in any OpenOCD session using `target/wb32.cfg`:

| Command | Description |
|---------|-------------|
| `wb32_info` | Display chip family, flash size, SRAM size |
| `wb32_dump_flash <file> [length]` | Dump flash to file (default: 4 KB) |
| `wb32_dump_all_flash <file>` | Dump entire flash (auto-detects size) |
| `wb32_verify_flash <file> [offset]` | Verify flash against file |
| `wb32_load_ram <file> [address]` | Load image to RAM (default: `0x20000000`) |

### Troubleshooting

**"Error connecting DP"** -- Check SWD wiring (SWDIO, SWCLK, GND). Ensure the target is powered. Try reducing adapter speed:

```bash
openocd -f interface/cmsis-dap.cfg -f target/wb32.cfg \
  -c "adapter speed 500" -c "init"
```

**"target halted due to debug-request, current mode: Handler HardFault"** -- The device has a corrupted vector table. The flash driver handles this automatically by clearing the exception state via `VECTCLRACTIVE`. Proceed with flashing normally.

**Probe not detected on Linux** -- Add a udev rule. See [Troubleshooting](#linux-udev-rules) below.

---

## USB DFU

WB32 microcontrollers include a ROM-based DFU bootloader at `0x1FFFF000`. It cannot be erased and provides a guaranteed recovery path even with completely corrupted firmware. See [bootloader.md](bootloader.md) for full details on the bootloader.

### Entering DFU Mode

| Method | Procedure |
|--------|-----------|
| QK_BOOT keycode | Press the QMK `QK_BOOT` key combination (keyboards only) |
| RESET button | Hold BOOT/RESET while powering on or pressing hardware reset |
| Software reset | Call `NVIC_SystemReset()` with the bootloader flag set |

When in DFU mode, the device enumerates with VID `0x342D` (Westberry Technology). Verify with `lsusb` (Linux), `system_profiler SPUSBDataType` (macOS), or Device Manager (Windows).

### Flash with wb32-dfu-updater

The official Westberry flashing tool. Available from [WestberryTech/wb32-dfu-updater](https://github.com/WestberryTech/wb32-dfu-updater) and bundled with QMK MSYS on Windows.

```bash
wb32-dfu-updater_cli -t -s 0x8000000 -D firmware.bin
```

| Option | Description |
|--------|-------------|
| `-t` | Transfer mode |
| `-s 0x8000000` | Start address (flash base) |
| `-D <file>` | Download (write) file to device |
| `-U <file>` | Upload (read) from device to file |

#### Read Back Firmware via DFU

```bash
wb32-dfu-updater_cli -t -s 0x8000000 -U readback.bin
```

### Flash with QMK Toolbox

1. Open [QMK Toolbox](https://github.com/qmk/qmk_toolbox)
2. Select the firmware file (`.bin` or `.hex`)
3. Put the device into DFU mode
4. Click "Flash"

QMK Toolbox automatically detects WB32 devices in DFU mode.

---

## Linux udev Rules

For non-root access to both DFU and debug probe devices, create `/etc/udev/rules.d/50-wb32.rules`:

```
# Westberry WB32 DFU
SUBSYSTEM=="usb", ATTR{idVendor}=="342d", MODE="0666"
```

Reload rules:

```bash
sudo udevadm control --reload-rules
```

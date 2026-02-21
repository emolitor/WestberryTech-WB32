# QMK Firmware Integration

QMK firmware fork using the fixed WB32 ChibiOS-Contrib port for Bridge75 wireless keyboard
variants.

---

## Overview

The QMK firmware for WB32-based keyboards depends on a fixed ChibiOS-Contrib HAL port that
fixes 11 bugs in the upstream WB32 drivers (DMA, PWM, USB, ADC, GPIO, and timer subsystems)
and adds a WWDG driver. The firmware also includes a custom WS2812 GPIO DMA driver that
replaces QMK's default bitbang implementation.

The fork is maintained on the `wireless-2.0` branch.

---

## ChibiOS-Contrib Submodule

The QMK build system pulls ChibiOS-Contrib as a git submodule at `lib/chibios-contrib`. For
WB32 targets, you can point to the fixed branch:

| Setting | Value |
|---------|-------|
| Repository | `https://github.com/emolitor/ChibiOS-Contrib` |
| Branch | `em-wb32-improvements` |

---

## Updating the Submodule

To sync the ChibiOS-Contrib submodule to the latest `em-wb32-improvements` branch:

```bash
cd emolitor-qmk_firmware

# Point submodule at the fork and fetch
git submodule sync lib/chibios-contrib
cd lib/chibios-contrib
git remote set-url origin https://github.com/emolitor/ChibiOS-Contrib.git
git fetch origin
git checkout em-wb32-improvements
cd ../..

# Verify
git submodule status lib/chibios-contrib

# Stage and commit the submodule update
git add .gitmodules lib/chibios-contrib
git commit -m "Update ChibiOS-Contrib submodule to latest em-wb32-improvements"
```

---

## Building QMK

All builds are run from the QMK firmware root directory.

### Bridge75 Variants

```bash
cd emolitor-qmk_firmware

# ANSI layout (default keymap, wireless enabled)
make shortcut/bridge75/ansi:default

# ANSI layout (devtest keymap, wireless disabled, SWD-safe)
make shortcut/bridge75/ansi:devtest WIRELESS_ENABLE=no

# ISO layout
make shortcut/bridge75/iso:default

# ANSI layout without RGB
make shortcut/bridge75/ansi_norgb:default
```

The `devtest` keymap disables the `DEVBOARD_NO_USB_POWER` guard that protects PA14 (SWCLK)
from being reconfigured as a GPIO output. Use `devtest` when debugging via SWD on a
development board.

### Flashing

Via USB DFU (no debug probe required):

```bash
wb32-dfu-updater_cli -t -s 0x8000000 -D shortcut_bridge75_ansi_default.bin
```

Via OpenOCD with a CMSIS-DAP probe:

```bash
openocd -f interface/cmsis-dap.cfg -f target/wb32fq95x.cfg \
  -c "init; halt; flash probe 0" \
  -c "flash write_image erase shortcut_bridge75_ansi_default.bin 0x08000000" \
  -c "verify_image shortcut_bridge75_ansi_default.bin 0x08000000" \
  -c "reset run; shutdown"
```

---

## What the Fork Fixes

The `em-wb32-improvements` branch contains fixes to the ChibiOS-Contrib WB32 port plus a new WWDG
driver. Summary of the fixes:

| # | Fix | Severity |
|---|-----|----------|
| 1 | DMA BLOCK_TS mask: named constant + runtime assert for 9-bit hardware limit | Critical |
| 2 | DMA ISR conditionals: DMAC2 stream handler guard thresholds corrected | High |
| 3 | dmaInit() reset: wrong channel count constant for DMAC1 initialization | High |
| 4 | DST_AUTO_RELOAD_DIS: bit position corrected from 30 to 31 | Low |
| 5 | ADC comment typos: "usbed" corrected to "used" | Cosmetic |
| 6 | USB macro name: `USB_ENDOPOINTS_NUMBER` corrected to `USB_ENDPOINTS_NUMBER` | Medium |
| 7 | PWM BDTR register: guarded to TIM1 only (TIM2-4 lack BDTR, write hits undefined memory) | Critical |
| 8 | PWM OC5M/OC6M macros: bit shift corrected from `>> 2` to `>> 3` | Medium |
| 9 | GPIO OSPEEDR polarity: documentation added (WB32: 0=HIGH, 1=LOW, inverted from STM32) | Documentation |
| 10 | Timer counter width: 20-bit counter documented, `WB32_TIM_CNT_MAX` constant added | Documentation |
| 11 | DMA consistency: `dmaWaitCompletion()` mask updated to named constant | Low |

See [chibios-bug-fixes.md](chibios-bug-fixes.md) for code-level details of each fix.

To apply these fixes to other ChibiOS-Contrib trees, use the
[emolitor/ChibiOS-Contrib](https://github.com/emolitor/ChibiOS-Contrib) fork
(`em-wb32-improvements` branch).

---

## WS2812 Custom Driver

The Bridge75 keyboard uses a custom WS2812 GPIO DMA driver instead of QMK's default bitbang
or PWM drivers. The driver uses timer-triggered DMA to write GPIO BSRR values directly, with
ISR-chained block transfers to work around the WB32 DMA BLOCK_TS 511-element hardware limit.

Key properties:

- Interrupts blocked for only ~5 microseconds per frame (vs. 2.7 ms for bitbang)
- Double-buffered with branchless encoding (~40 microseconds per block fill)
- Validated on Bridge75 ANSI: flicker-free across all 81 LEDs
- 8 bugs found and fixed during development, including a DMA ISR priority violation that
  caused scheduler deadlock under wireless UART stress

See [ws2812-gpio-dma-driver.md](ws2812-gpio-dma-driver.md) for the full technical
documentation.

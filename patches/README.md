# Patches

Pre-generated patches for ChibiOS-Contrib.

## ChibiOS-Contrib Patches

Bug fixes for the ChibiOS-Contrib WB32FQ95xx HAL port.
Generated from the `em-wb32-improvements` branch of
https://github.com/emolitor/ChibiOS-Contrib (base: `chibios-21.11.x`).

Fixes DMA ISR conditionals, DMA init reset guard, DST_AUTO_RELOAD_DIS bit
position, ADC comment typos, and USB endpoint macro name typo. See
[docs/chibios-bug-fixes.md](../docs/chibios-bug-fixes.md) for detailed
descriptions of each fix.

### Patch Files

```
chibios-contrib/
└── 0001-fix-Fix-various-minor-defects-in-WB32-port.patch
```

### Applying with git am

```bash
cd /path/to/ChibiOS-Contrib
git checkout chibios-21.11.x
git am /path/to/patches/chibios-contrib/*.patch
```

### Using the Branch Directly

```bash
cd /path/to/ChibiOS-Contrib
git remote add emolitor https://github.com/emolitor/ChibiOS-Contrib.git
git fetch emolitor em-wb32-improvements
git checkout emolitor/em-wb32-improvements
```

## OpenOCD Flash Driver

The WB32F10x flash driver is maintained in a dedicated OpenOCD fork:
[emolitor/openocd](https://github.com/emolitor/openocd). See the fork for
build instructions, driver documentation, and FMC register reference.

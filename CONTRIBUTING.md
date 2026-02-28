# Contributing

Contributions to WestberryTech-WB32 are welcome. This document covers how to report bugs, submit patches, and test changes.

## Reporting Bugs

Open a GitHub issue with:

1. **Device**: Which WB32 variant (WB32F10x, WB32FQ95xx, etc.)
2. **Context**: What you were trying to do (OpenOCD flash, ChibiOS driver, bare metal, etc.)
3. **Expected behavior**: What should have happened
4. **Actual behavior**: What happened instead
5. **Steps to reproduce**: Minimal reproduction case
6. **Environment**: Toolchain version, debug probe, OS

For silicon errata discoveries, include the register-level validation method (e.g., OpenOCD `mww`/`mdw` commands or GDB register dumps).

## Submitting Patches

### Documentation Fixes

For typos, broken links, or clarifications — open a pull request directly. No testing required.

### Code Changes

For changes to example projects or GDB scripts:

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test on actual hardware (see [Testing](#testing))
5. Open a pull request with:
   - What changed and why
   - Which device(s) you tested on
   - Test results (pass/fail for affected operations)

### ChibiOS-Contrib Fixes

ChibiOS-Contrib fixes should go to the [emolitor/ChibiOS-Contrib](https://github.com/emolitor/ChibiOS-Contrib) fork on the `em-wb32-improvements` branch.

## Testing

### OpenOCD Driver

Test on actual hardware with a debug probe (CMSIS-DAP, J-Link, or WB-Link):

```bash
# Minimum test: probe + erase + write + verify
openocd -f interface/cmsis-dap.cfg -f target/wb32.cfg \
  -c "init; halt; flash probe 0" \
  -c "flash write_image erase test.bin 0x08000000" \
  -c "verify_image test.bin 0x08000000" \
  -c "reset run; shutdown"
```

Verify:
- Device detection succeeds
- Sector erase completes without timeout
- Write + verify passes
- Device runs correctly after reset

### Bare Metal Examples

```bash
cd examples/blink
make clean && make
# Flash and verify LED blinks
```

### Silicon Errata

New errata should include:
- Register-level validation commands (OpenOCD or GDB)
- Expected vs. actual register values
- Which device(s) were tested
- Impact on existing drivers/applications

## Code Style

- C code: Follow existing style in each project (vendor lib style for examples, ChibiOS style for ChibiOS drivers)
- Markdown: Standard GitHub-flavored markdown, no trailing whitespace
- Commit messages: Imperative mood, concise subject line

## What We're Looking For

Contributions in these areas are particularly valuable:

- Testing on WB32 variants not yet validated (WB32F101xx, WB32F102xx, WB32F3G71xx)
- Additional silicon errata discoveries
- OpenOCD driver improvements at [emolitor/openocd](https://github.com/emolitor/openocd) (especially a RAM-resident flash algorithm for faster writes)
- ChibiOS driver improvements (IWDG, CRC, I2S, USB DMA)
- Better documentation of the WB32 Gaming Keyboard SoC

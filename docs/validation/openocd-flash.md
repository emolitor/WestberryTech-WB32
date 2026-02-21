# OpenOCD WB32 Flash Driver Validation Results

**Date**: 2026-02-05
**Target**: WB32FQ95xC (Device ID 0x3A50E980, 256 KB flash, 36 KB SRAM)
**Probe**: Raspberry Pi Debug Probe (CMSIS-DAP v2, VID:PID 2e8a:000c)
**Driver**: `src/flash/nor/wb32f10x.c` in [emolitor/openocd](https://github.com/emolitor/openocd) (direct register write approach)
**OpenOCD**: v0.12.0+dev-g22a7bda-dirty

## Summary

All 10 validation phases passed. One critical bug was found and fixed during Phase 2 (PRE_OP frequency), and the erase pattern ambiguity (Issue #2) was resolved.

| Phase | Description | Result |
|-------|-------------|--------|
| 0 | Build Preparation | PASS |
| 1 | Probe Connectivity & Device ID | PASS |
| 2 | Erase Pattern Determination | PASS (found PRE_OP bug, fixed) |
| 3 | PRE_OP Calibration Verification | PASS |
| 4 | Sector Erase Validation | PASS |
| 5 | Write + Verify (Small, 2 KB) | PASS |
| 6 | Write + Verify (Large, 18 KB) | PASS |
| 7 | Mass Erase | PASS |
| 8 | Stress Test | PASS |
| 9 | Edge Cases | PASS |
| 10 | GDB Test | PASS |

## Critical Finding: PRE_OP Must Run Before Every Operation

**Bug**: The driver originally called PRE_OP once per erase/write session. Writes silently failed — data read back as zeros.

**Fix**: Moved PRE_OP call into `wb32f10x_flash_op_direct()` so it runs before every individual erase and program operation (matching vendor library and ChibiOS behavior). Clear latch operations skip PRE_OP.

**Root cause**: Flash controller timing calibration expires or becomes invalid. Without fresh calibration before each operation, the FMC accepts the command but doesn't actually program the flash cells. The WR bit clears normally (no timeout), but data is not written.

## Erase Pattern: 0x00 (Issue #2 Resolved)

WB32 flash uses an **inverted bit model** compared to standard NOR:

| Property | WB32 | Standard NOR |
|----------|------|--------------|
| Erased state | 0x00 | 0xFF |
| Programming | Sets bits (0→1) | Clears bits (1→0) |
| Overwrite without erase | OR'd corruption | AND'd corruption |
| Partial page padding | 0x00 | 0xFF |

Confirmed by erasing sectors 59-63 and reading back: all 4096 bytes per sector read as 0x00000000. Consistent across start, middle, and end of each sector.

## Phase Details

### Phase 0: Build Preparation

- Backed up old driver (1043 lines), copied new driver (915 lines)
- Built with `--enable-cmsis-dap --enable-cmsis-dap-v2 --disable-werror`
- Required `libhidapi-dev` for CMSIS-DAP HID support

### Phase 1: Probe Connectivity & Device Identification

| Register | Value | Meaning |
|----------|-------|---------|
| SYS_ID (0x40016400) | 0x3A50E980 | WB32F104 family |
| SYS_MEMSZ (0x40016404) | 0x00040003 | 256 KB flash, 36 KB SRAM |
| FMC_CON (0x40017800) | 0x00000800 | Idle, WR clear |
| FMC_STAT (0x40017808) | 0x00000000 | No errors |
| FHSISR (0x4001043C) | 0x00000001 | FHSI ready |
| Factory cal (0x1FFFF000) | Non-zero | PRE_OP data present |

- Flash probed: 64 sectors x 4096 bytes = 256 KB
- SRAM verified: wrote/read 0xDEADBEEF at 0x20000000
- Full flash backup saved (262144 bytes at 62.9 KiB/s)

### Phase 2: Erase Pattern Determination

1. Erased sector 63 (was already zeros — inconclusive)
2. Erased sector 61 (had existing data) → all 0x00000000 across entire sector
3. Confirmed across sectors 59-63 at multiple offsets
4. Attempted write to verify programming works → **write failed silently**
5. Diagnosed PRE_OP frequency bug, applied fix
6. After fix: write + verify succeeded

### Phase 3: PRE_OP Verbose Verification (-d3)

Debug trace confirmed:
- PRE_OP code (240 bytes) loaded to 0x20000000
- BLX wrapper at 0x20000000, PRE_OP body at 0x20000008
- CPU executed PRE_OP, halted at BKPT (0x20000004)
- FMC register sequence matched vendor library exactly
- FHSISR bit 0 = 1 (FHSI ready) after PRE_OP

### Phase 4: Sector Erase Validation

| Test | Sector(s) | Time | Result |
|------|-----------|------|--------|
| Single sector | 0 | 33 ms | PASS |
| Multi-sector | 1-3 | 75 ms | PASS |
| Mid-flash | 32 | 25 ms | PASS |

All erased sectors verified as 0x00000000.

### Phase 5: Small Firmware Write (Blink, 2016 bytes)

- `flash write_image erase` succeeded
- `verify_image` passed
- Vector table: SP=0x20009000, Reset=0x08000499 (matched binary)
- Functional test: LEDs on PB13/PB14 blink at ~1 Hz after BOOT0 jumper correction

**Note**: Board had BOOT0 jumper set for bootloader ROM. CPU booted to 0x1FFFED8E instead of flash. After moving jumper to flash boot position, firmware ran correctly.

### Phase 6: Large Firmware Write (DMA Test, 18400 bytes)

- Wrote 18400 bytes across 5 sectors in 2.5 seconds (~8.0 KiB/s)
- `verify_image` passed
- Sector boundary data verified at 0x08000FF0/0x08001000 and 0x08003FF0/0x08004000
- Tail region verified (partial page padding correct)

### Phase 7: Mass Erase

- `wb32f10x mass_erase 0` completed successfully
- Verified at start (0x08000000), middle (0x08020000), near end (0x0803F000), and last page (0x0803FF00) — all 0x00000000
- `flash erase_check 0` reported "Bank is erased"

### Phase 8: Stress Test

| Test | Iterations | Result |
|------|-----------|--------|
| Blink write/verify | 5/5 | PASS |
| DMA write/verify | 3/3 | PASS |
| Alternating firmware | 3/3 | PASS |
| Rapid sector erase (sector 63) | 10/10 | PASS (26-33 ms each) |
| Final functional boot | 1/1 | PASS (LEDs blinking) |

### Phase 9: Edge Cases

| Test | Result | Notes |
|------|--------|-------|
| Overwrite without erase | Expected failure | verify_image reports mismatches (OR'd corruption) |
| Single page write (256 bytes) | PASS | Erased sector 63, wrote/verified 256 bytes |
| Cross-sector boundary | PASS | Wrote across sector 31/32 boundary, verified both sides |
| Diagnostic register dump | PASS | FMC_CON, FMC_STAT, FHSISR, PCLKENR all normal |

### Phase 10: GDB Test

- Connected via `gdb-multiarch` to OpenOCD GDB server on port 3333
- Halted in `Delay()` at `main.c:22` (Blink firmware)
- Backtrace: `Delay(1000)` called from `main()` at `main.c:60`
- Register dump, memory inspection, source-level debug all working
- Resume: LEDs continued blinking after `continue`

## Driver Changes Made During Validation

1. **PRE_OP per-operation** (`wb32f10x_flash_op_direct()`): Added PRE_OP call before every erase/program operation, skip for clear_latch
2. **Removed standalone PRE_OP calls**: From `erase()`, `write()`, and `mass_erase()` — now handled by `flash_op_direct()`
3. **Erase check for 0x00 only**: Removed dual-pattern check, only checks for 0x00000000
4. **Partial page padding 0x00**: Changed from 0xFF to 0x00 (erased value)

## Performance

| Operation | Measurement |
|-----------|-------------|
| Sector erase | 25-33 ms |
| Flash backup (256 KB read) | 62.9 KiB/s |
| Small write (2 KB) | ~2 KiB/s |
| Large write (18 KB) | ~8 KiB/s |
| Mass erase | < 5 seconds |

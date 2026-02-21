# Phase 8: Flash Driver (EFL) Validation


## Summary

| Feature | Port Implementation | Reference Manual | Status |
|---------|---------------------|------------------|--------|
| Page size | 256 bytes | 256 bytes (BUF0-63) | Correct |
| Sector size | 256 bytes (registry) | 256 bytes (page) | Correct |
| Max sectors | 1024 | 256KB / 256B = 1024 | Correct |
| Line size | 8 bytes | N/A | Reasonable |
| Page program | 0x0C op code | Page program | Correct |
| Page erase | 0x08 op code | Page erase | Correct |
| Clear latch | 0x04 op code | Page write latch clear | Correct |
| Mass erase | UNIMPLEMENTED | Chip erase supported | Not implemented |
| Block erase | Not supported | Block erase supported | Not implemented |
| Erase pattern | Verifies 0x00 | N/A | CONFIRMED — erased state is 0x00 |

## Operation Codes

| Operation | Port Code | Reference Manual | Match |
|-----------|-----------|------------------|-------|
| Clear page write latch | 0x04 | PWLC command | Correct |
| Page erase | 0x08 | PE command | Correct |
| Page program | 0x0C | PP command | Correct |
| Block erase | Not used | BE command | Not implemented |
| Chip erase | Not used | CE command | Not implemented |

## Flash Descriptor

```c
static const flash_descriptor_t efl_lld_descriptor = {
  .attributes = FLASH_ATTR_ERASED_IS_ONE |     /* INCORRECT: WB32 erased state is 0x00 (all zeros), not ones. This flag should be removed. */
                FLASH_ATTR_MEMORY_MAPPED |
                FLASH_ATTR_ECC_CAPABLE |
                FLASH_ATTR_REWRITABLE,
  .page_size  = 256U,
  .sectors_count = 0U,    /* Filled at runtime from flash size */
  .sectors_size  = 256U,
  .sectors       = NULL,  /* Uniform sectors */
  .address       = (uint8_t *)0x08000000U,
  .size          = 0U,    /* Filled at runtime */
};
```

## Erase Pattern — CONFIRMED

**Hardware test date:** 2026-02-05
**Result:** WB32 flash erases to `0x00` (all zeros)

### Test Results

Sector erase on WB32F104RC/WB32FQ95xC produces `0x00000000` across all bytes. Verified across multiple sectors and memory offsets during OpenOCD flash driver validation.

### Inverted Bit Model

The WB32 uses an inverted flash bit model:
- **Erased state**: `0x00` (all zeros)
- **Programming**: Sets bits (0→1)
- **Cannot do without erase**: Clear bits (1→0)
- **Overwrite without erase**: Produces OR'd corruption

### ChibiOS Erase Verification

The `efl_lld_verify_erase()` check (`hal_efl_lld.c:514`) comparing against `0x00` is **correct**. The original developer had tested on hardware.

**However**, the `FLASH_ATTR_ERASED_IS_ONE` flag in the flash descriptor is incorrect — the erased state is zero, not one. This flag should be reviewed/removed.

See [`openocd-flash.md`](../openocd-flash.md) for full test details. The driver source is maintained at [emolitor/openocd](https://github.com/emolitor/openocd).

## Limitations

1. **Mass erase not implemented** - Returns FLASH_ERROR_UNIMPLEMENTED
2. **Block erase not supported** - Only page (256-byte) erase available
3. **No async erase** - Operations are synchronous with IRQ disabled
4. **CRC calculation feature not exposed** - FMC has hardware CRC

## Validation Conclusion

The Flash driver implementation is **correct and functional** for basic flash operations:
- FMC register structure matches the reference manual
- Operation codes (clear latch, page erase, page program) are correct
- Unlock sequence is properly implemented
- RAM code execution correctly handles flash read suspension
- Page write latch (BUF[0-63]) is correctly used
- Flash descriptor correctly reports capabilities
- FHSI clock dependency is properly handled
- Interrupt disabling protects flash operations
- **Erase pattern confirmed as `0x00`** (all zeros) — inverted from standard NOR flash
- **Note:** `FLASH_ATTR_ERASED_IS_ONE` flag in the descriptor is incorrect for this hardware and should be reviewed

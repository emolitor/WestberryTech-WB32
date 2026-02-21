# Phase 6: DMA Driver Validation


## Summary

| Feature | Port Implementation | Reference Manual | Status |
|---------|---------------------|------------------|--------|
| DMA controllers | 2 (DMAC1, DMAC2) | 2 (DMAC1, DMAC2) | Correct |
| Channels per controller | 3 | 3 | Correct |
| Hardware handshake interfaces | 16 per DMAC | 16 per DMAC | Correct |
| Transfer types | M2M, M2P, P2M, P2P | M2M, M2P, P2M, P2P | Correct |
| Block size limit | 9-bit (511) | 12-bit documented, 9-bit actual | Silicon errata |
| Scatter/Gather | Supported | Supported | Correct |

## Critical: Silicon Errata

**The reference manual documents CTLH.BLOCK_TS as 12 bits (4095 max), but hardware only implements 9 bits (511 max).**

See `../silicon-errata.md` for details and workarounds.

## Hardware Handshake Interface Validation

All DMAC1 and DMAC2 handshake mappings verified against reference manual:

### DMAC1 Handshake Interfaces
| Interface | Peripheral | Verified |
|-----------|------------|----------|
| 0 | TIM1_CH1 | Correct |
| 1 | TIM1_CH2 | Correct |
| 2 | TIM1_CH3 | Correct |
| 3 | TIM1_CH4 | Correct |
| 4 | TIM1_UP | Correct |
| 5 | TIM1_TRIG | Correct |
| 6 | TIM1_COM | Correct |
| 7 | UART1_RX | Correct |
| 8 | UART1_TX | Correct |
| 9 | ADC | Correct |
| 10-15 | Reserved | Correct |

### DMAC2 Handshake Interfaces
| Interface | Peripheral | Verified |
|-----------|------------|----------|
| 0 | TIM2_CH1 | Correct |
| 1 | TIM2_CH2 | Correct |
| 2-6 | TIM2-4 channels | Correct |
| 7 | I2C1_RX | Correct |
| 8 | I2C1_TX | Correct |
| 9 | I2C2_RX | Correct |
| 10 | I2C2_TX | Correct |
| 11-15 | SPI/UART | Correct |

## Bugs Fixed

### 1. Block Transfer Size Mask
**File:** `wb32_dma.h:588`
```c
// Original (bug - limited to 511):
(dmastp)->dmac->Ch[(dmastp)->channel].CTLH = (uint32_t)((size) & 0x1FF);

// Note: Changed to 0xFFF but then REVERTED due to silicon errata
// Hardware only supports 9 bits regardless of mask value
```

### 2. ISR Handler Conditional Checks
**File:** `wb32_dma.c:191-196`
- Fixed DMAC2 stream handler conditionals from all `> 0` to proper `> 0`, `> 1`, `> 2`

### 3. dmaInit() Reset Conditional
**File:** `wb32_dma.c:218`
- Changed incorrect `WB32_DMAC2_NUM_CHANNELS` to `WB32_DMAC1_NUM_CHANNELS` for DMAC1 reset

### 4. DST_AUTO_RELOAD_DIS Bit Position
**File:** `wb32_dma.h:294`
- Changed from `(0x0U << 30)` to `(0x0U << 31)` for consistency with EN variant

## Validation Conclusion

The DMA driver implementation is **correct** after bug fixes:
- All handshake interface mappings match the reference manual exactly
- All register bit positions are correctly defined
- Transfer types and flow control modes are correct
- Interrupt handling is complete

**Limitation:** Maximum single-block transfer is 511 items due to silicon errata. Use multi-transfer ISR chaining for larger buffers.

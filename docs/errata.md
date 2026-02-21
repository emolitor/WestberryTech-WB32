# WB32 Errata

Known errata and driver-level issues for WB32F10x and WB32FQ95xx microcontrollers, discovered through register-level hardware testing and production firmware validation.

---

## WB32-E001: DMA BLOCK_TS Field Limited to 9 Bits

### Description

The reference manual (Chapter 13, Section 13.7.5) documents the CTLH register BLOCK_TS field as bits [11:0], implying a maximum single-block DMA transfer size of 4095 elements. Hardware only implements bits [8:0], limiting the actual maximum to **511 elements**. Bits [9:11] are not writable and always read as zero.

### Impact

Any DMA transfer configured with a block size greater than 511 is silently truncated to the lower 9 bits. This affects WS2812 LED strips (typically requiring thousands of transfers per frame), large SPI/UART DMA buffers, and any peripheral DMA operation exceeding 511 elements. No error is generated -- the transfer simply processes fewer elements than requested.

| LED Count | DMA Transfers Required | Status |
|-----------|------------------------|--------|
| 6 | 368 | Works (< 511) |
| 11 | 488 | Works (< 511) |
| 12 | 512 | Fails (> 511) |
| 82 | 2192 | Fails without workaround |

### Workaround

Use ISR block chaining: split large transfers into chunks of 511 elements or fewer, then chain them via the DMA transfer-complete (TFR) interrupt. The ISR reconfigures and starts the next chunk immediately upon completion.

Example for 81 WS2812 LEDs (5832 phases total): split into 12 blocks of 504 phases each (504 chosen for 3-phase-per-bit alignment), with the final block containing the remainder.

```c
// In DMA TFR interrupt callback:
if (++block_idx < total_blocks) {
    // Configure next chunk source address and block size
    dmaStreamSetMemory(stream, &buffer[block_idx * CHUNK_SIZE]);
    dmaStreamSetTransactionSize(stream, next_chunk_size);  // <= 511
    dmaStreamEnableInterrupt(stream, WB32_DMAC_IT_TFR);
    dmaStreamEnableInterrupt(stream, WB32_DMAC_IT_ERR);
    dmaStreamEnable(stream);
}
```

### Validation

Confirmed via direct register access on all 3 DMA channels:

```tcl
# OpenOCD register test
mww 0x40007C1C 0xFFF   # Write 4095 to DMAC1 Ch0 CTLH
mdw 0x40007C1C          # Reads back 0x1FF (511)
```

Multi-transfer chaining validated with 424 DMA callbacks across 100 stress-test cycles, zero errors. ISR completes in approximately 42 cycles (90 available at bus clock). Production-validated on Bridge75 keyboard with 81 WS2812 LEDs.

---

## WB32-E002: Flash Erases to 0x00 (Inverted Bit Model)

### Description

Standard NOR flash erases to 0xFF (all bits set) and programs by clearing bits (1 to 0). The WB32 flash controller operates with an inverted bit model:

- **Erased state**: 0x00 (all bits cleared)
- **Programming**: Sets bits (0 to 1)
- **Cannot clear bits** (1 to 0) without erasing the containing sector first

This is undocumented in the reference manual and contradicts standard flash programming assumptions.

### Impact

- **Overwriting without erase** produces OR-corrupted data (new data OR'd with existing data) rather than a write error.
- **Partial page padding** must use 0x00 (the erased value), not the conventional 0xFF. Flash programming tools and drivers that assume 0xFF padding will corrupt data.
- **Erase verification** must check for 0x00000000, not 0xFFFFFFFF.
- Any flash driver, bootloader, or file system ported from STM32 or other NOR flash platforms requires modification.

### Workaround

All flash programming tools and drivers must account for the inverted bit model:

1. Pad partial pages with 0x00.
2. Verify erase by checking for 0x00000000.
3. Always erase before reprogramming -- overwriting without erase silently corrupts data.

### Validation

Confirmed by hardware test on WB32F104RC and WB32FQ95xC (2026-02-05). After sector erase, all bytes read as 0x00. Programming sets bits from 0 to 1. Attempting to clear a set bit without erase leaves it unchanged, producing OR'd corruption.

---

## WB32-E003: ChibiOS dmaStreamDisable() Clears All Interrupt Masks

### Description

The ChibiOS WB32 DMA driver's `dmaStreamDisable()` internally calls `dmaStreamDisableInterruptAll()`, which clears all interrupt mask registers for the channel: MaskTfr, MaskBlock, and MaskErr. The `dmaStreamEnable()` function does not restore these masks. Initial mask configuration only occurs during `dmaStreamSetMode()`.

This is a driver-level issue rather than a silicon erratum, but it affects all WB32 ChibiOS applications using multi-frame DMA.

### Impact

Any driver that completes a DMA transfer (calling `dmaStreamDisable()`) and later starts a new transfer on the same stream (calling `dmaStreamEnable()`) will find that TFR and ERR interrupts never fire on the second and subsequent transfers.

Symptoms:
- First DMA frame completes normally (masks set during `dmaStreamSetMode()`)
- All subsequent frames hang silently -- the transfer completes at the hardware level, but the completion interrupt is masked
- No DMA errors are reported

### Workaround

Re-enable the required interrupt masks before every `dmaStreamEnable()` call:

```c
dmaStreamEnableInterrupt(stream, WB32_DMAC_IT_TFR);
dmaStreamEnableInterrupt(stream, WB32_DMAC_IT_ERR);
dmaStreamEnable(stream);
```

This must be done before every `dmaStreamEnable()`, not just the first.

### Validation

Confirmed at register level during WS2812 GPIO DMA driver validation:
- During transfer: MaskTfr=1, MaskErr=1
- After `dmaStreamDisable()`: MaskTfr=0, MaskErr=0
- After applying workaround: MaskTfr=1, MaskErr=1 restored before each enable

---

## WB32-E004: GPIO OSPEEDR Polarity Inverted from STM32

### Description

The GPIO output speed register (OSPEEDR) uses inverted polarity compared to the STM32 convention:

| OSPEEDR Value | STM32 Behavior | WB32 Behavior |
|---------------|----------------|---------------|
| 0 | Low speed | **High speed** |
| 1 | Low speed | **Low speed** |
| 2 | Medium speed | N/A |
| 3 | High speed | N/A |

The default `PAL_MODE_OUTPUT_PUSHPULL` in ChibiOS uses high speed (OSPEEDR=0) on WB32, which is correct.

### Impact

Code ported from STM32 that explicitly sets OSPEEDR values for speed control will get the opposite behavior. Setting OSPEEDR=0 expecting low speed will get high speed, and vice versa. This can cause signal integrity issues on high-speed interfaces or unnecessary EMI on pins that should be configured for low speed.

### Workaround

Use the ChibiOS PAL macros, which handle the polarity difference internally. When setting speed directly at the register level, verify the polarity against the WB32 reference manual rather than assuming STM32-compatible values.

### Validation

Confirmed by register inspection on WB32FQ95 hardware. Default GPIO configuration (OSPEEDR=0) produces high-speed output transitions consistent with documentation.

---

## WB32-E005: DMA ChEnReg Not Auto-Cleared After Non-Circular Transfer

### Description

After a non-circular DMA transfer completes and fires the TFR interrupt, the channel remains in an "enabled but idle" state in the Channel Enable Register (ChEnReg). The hardware does not automatically clear the channel enable bit upon transfer completion.

While the channel is in this state, hardware handshake requests from the associated peripheral (such as timer update events) continue to accumulate. These stale requests are not discarded.

### Impact

The next call to `dmaStreamEnable()` may fire a spurious DMA transfer from the stale handshake state before the intended transfer begins. This is particularly problematic for timer-triggered DMA (such as PWM-based LED drivers), where timer update events continue to generate DMA requests between frames.

Symptoms include unexpected single-element transfers, corrupted output at the start of a DMA frame, or data appearing one element shifted.

### Workaround

Manually disable the channel by writing `(ch_mask << 8)` to ChEnReg before reconfiguring for the next transfer. This clears the channel enable bit and discards stale handshake requests while preserving interrupt mask configuration.

```c
// Disable channel without clearing interrupt masks
// (unlike dmaStreamDisable() which clears all masks -- see WB32-E003)
DMAC1->ChEnReg = ((1 << channel) << 8);
```

Using `dmaStreamDisable()` also clears the channel, but additionally clears all interrupt masks (WB32-E003), requiring them to be re-enabled.

### Validation

Confirmed during WS2812 GPIO DMA driver development. Without the workaround, the first phase of each DMA frame contained stale data from the previous transfer. Adding the ChEnReg clear before reconfiguration eliminated the spurious transfer.

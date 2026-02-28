# WB32FQ95 Multi-Transfer DMA Validation Demo

This demo validates the multi-chunk DMA transfer mechanism needed to work around the WB32FQ95xx silicon errata where DMA BLOCK_TS is limited to 9 bits (max 511) instead of the documented 12 bits (max 4095).

**Key Metrics:**
- 424 total DMA callbacks with 0 errors
- ISR completes in ~42 cycles (90 available per PWM period)
- Chunk sizes: 511, 511, 511, 475 (2008 total bits for 82 LEDs)

## Hardware Requirements

- WB32FQ95 development board
- CMSIS-DAP debug adapter (e.g., DAPLink, WB-Link, Pico Debug Probe)
- Optional: Oscilloscope to observe PWM output on PA7

## Quick Start

### 1. Build the Demo

```bash
cd WB32FQ95-DMA-Test

# Build single-transfer test (default - validates basic DMA works)
make

# Build multi-chunk test (validates ISR chaining for >511 elements)
make MAIN=main_dma_test.c

# Build simple blink (validates ChibiOS + GPIO)
make MAIN=main.c
```

### 2. Flash and Debug

**Terminal 1 - Start OpenOCD:**
```bash
openocd -f interface/cmsis-dap.cfg -f target/wb32.cfg
```

**Terminal 2 - Run GDB:**
```bash
gdb-multiarch -x debug.gdb build/dma_test.elf
```

### 3. Run Tests

In GDB:
```
(gdb) run_tests
```

This will:
1. Blink LED twice to indicate ready
2. Run Test 1: Chunk calculations
3. Run Test 2: Single multi-chunk transfer
4. Run Test 3: Consecutive transfers
5. Run Test 4: Stress test (100 transfers)

### 4. Inspect Results

```
(gdb) dma_info
```

Output should show:
```
=== DMA Transfer Status ===
transfer_active:    0
transfer_complete:  1
chunk_index:        4
callback_count:     424  (4 chunks * 106 transfers)
error_count:        0

=== Chunk Status ===
Chunk 0: size=511, complete=1
Chunk 1: size=511, complete=1
Chunk 2: size=511, complete=1
Chunk 3: size=475, complete=1

=== Test Results ===
Test 1 (chunk calculations): PASS
Test 2 (single transfer):    PASS
Test 3 (consecutive):        PASS
Test 4 (stress test):        PASS
```

## LED Indications

| Pattern | Meaning |
|---------|---------|
| 2 blinks | Ready |
| 4 quick blinks | Running tests |
| Fast blink (100ms) | All tests passed |
| Slow blink (500ms) | Test failure |

## Debug Variables

Inspect these in GDB:

| Variable | Description |
|----------|-------------|
| `dma_callback_count` | Total DMA callbacks received |
| `dma_error_count` | DMA errors encountered |
| `dma_chunk_index` | Current chunk being transferred |
| `dma_transfer_active` | True during transfer |
| `dma_transfer_complete` | True when all chunks done |
| `dma_chunk_complete[4]` | Per-chunk completion status |
| `test_results[5]` | 0=not run, 1=pass, 2=fail |

## GDB Commands

| Command | Description |
|---------|-------------|
| `dma_status` | Show transfer state variables |
| `dma_chunks` | Show chunk completion status |
| `test_results` | Show test pass/fail status |
| `dma_info` | Show all debug info |
| `dma_regs` | Show DMA hardware registers |
| `run_tests` | Continue execution |

## Test Details

### Single-Transfer Tests (main_single_dma_test.c)

Validates that basic DMA+PWM works before attempting multi-chunk transfers.

| Test | Description | Validation |
|------|-------------|------------|
| Test 0 | DMA allocation | Stream allocated successfully |
| Test 1 | Small buffer (100 elements) | Callback received, no errors |
| Test 2 | Max buffer (511 elements) | Callback received, no errors |
| Test 3 | Repeated transfers (10x) | 10 callbacks, consistent operation |

**Debug Variables:**
- `dma_callback_count` - Total callbacks (should be 12 after all tests)
- `dma_error_count` - Should be 0
- `test_results[4]` - All should be 1 (pass)

### Multi-Chunk Tests (main_dma_test.c)

Validates ISR-based chaining for transfers >511 elements.

### Test 1: Chunk Calculations

Verifies compile-time constants:
- `DMA_CHUNK_COUNT = 4` (2008 bits / 511 = 3.93)
- Chunk sizes: 511, 511, 511, 475

### Test 2: Single Transfer

Starts one multi-chunk transfer and verifies:
- All 4 chunks complete
- Correct callback count (4)
- No DMA errors

### Test 3: Consecutive Transfers

Runs 5 back-to-back transfers and verifies:
- 20 total callbacks (5 * 4)
- No errors

### Test 4: Stress Test

Runs 100 transfers rapidly and verifies:
- 400 total callbacks (100 * 4)
- No errors
- Proper state management

## Mapping to WS2812 Driver

This demo validates the same multi-transfer mechanism used in `ws2812_pwm.c`:

| Demo | WS2812 Driver |
|------|---------------|
| `dma_callback()` | `ws2812_dma_callback()` |
| `start_transfer()` | `ws2812_start_transfer()` |
| `wait_for_transfer()` | Logic in `ws2812_flush()` |
| `DMA_CHUNK_SIZE=511` | `WS2812_DMA_CHUNK_SIZE=511` |
| `frame_buffer[2008]` | `ws2812_frame_buffer[WS2812_BIT_N]` |

## Known Issues

### BOOT0 Pin Configuration

On the WB32F104RC → WB32FQ95 dev board, the chip boots to ROM (PC=0x1fffe090) instead of flash (PC=0x08000xxx) depending upon
the boot pin configuration. You can either change the boot pin jumper or just manually set PC/SP.

**Symptoms:**
- After `reset run`, chip enters ROM code instead of user flash
- Vector table at 0x08000000 is correctly programmed

**Workaround:**
After flashing, manually set PC/SP from the vector table:
```tcl
# In OpenOCD
halt
set sp [mrw 0x08000000]
set pc [mrw 0x08000004]
reg sp $sp
reg pc $pc
resume
```

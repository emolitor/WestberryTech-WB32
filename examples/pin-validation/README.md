# WB32F104RC Pin Mapping Validation Test Suite

Bare-metal test suite that validates the AF (Alternate Function) pin mappings
from the WB32F104xx datasheet (DS004, Table 3.4-2) against real hardware.

## Hardware Setup

- **Target**: WB32F104RC (LQFP64, 256KB flash, 36KB SRAM)
- **Debug probe**: Pico debug probe (CMSIS-DAP SWD + UART passthrough)
- **SWD**: PA13 (SWDIO), PA14 (SWCLK)
- **Console**: Pico UART connected to WB32 UART1 -- PA9 (TX), PA10 (RX)

## Build

```bash
cd examples/pin-validation
make
```

Produces `build/PinValidation.elf`, `.hex`, and `.bin`.

## Flash

```bash
make flash
```

Or manually:

```bash
openocd -f interface/cmsis-dap.cfg -f target/wb32.cfg \
    -c "program build/PinValidation.bin 0x08000000 verify reset exit"
```

## Capture Output

Connect to the Pico debug probe's UART at 115200 baud:

```bash
# macOS
screen /dev/tty.usbmodem* 115200

# Linux
picocom -b 115200 /dev/ttyACM0
```

## Automated Test Run

```bash
make test
```

Or:

```bash
openocd -f interface/cmsis-dap.cfg -f target/wb32.cfg \
    -f flash_and_test.tcl
```

**Note**: After the first build, update `TEST_RESULTS_ADDR` in
`flash_and_test.tcl` with the actual address from the map file:

```bash
grep test_results build/PinValidation.map
```

## GDB Inspection

```bash
arm-none-eabi-gdb build/PinValidation.elf
(gdb) target remote :3333
(gdb) monitor reset halt
(gdb) load
(gdb) continue
# Wait for tests to complete, then Ctrl-C
(gdb) print test_results
```

## Test Phases

| Phase | Description | Tests | Jumpers? |
|-------|-------------|-------|----------|
| 1 | GPIO AF register validation | ~150 | No |
| 2 | UART internal loopback | 3 | No |
| 3 | Timer counter running | 4 | No |
| 4 | ADC internal + external channels | ~18 | No |
| 5 | SPI register status check | 2 | No |
| 6 | I2C peripheral init | 2 | No |
| 7 | LED controller read/write | 1 | No |
| 8 | USB clock enable | 1 | No |

## Reserved Pins (Cannot Test)

| Pin | Reason |
|-----|--------|
| PA9, PA10 | UART1 console |
| PA13, PA14 | SWD debug |
| PD0, PD1 | HSE oscillator |

## Optional Jumper Wires

All jumper-dependent tests are skippable. Without jumpers, ~180 tests still run.

| Jumper | From | To | Test |
|--------|------|----|------|
| J1 | PA0 (TIM2_CH1) | PB8 (TIM4_CH3) | Timer cross-capture |
| J2 | PA7 (QSPI_MO) | PA6 (QSPI_MI) | QSPI loopback |
| J3 | PB15 (SPIM2_MO) | PB14 (SPIM2_MI) | SPIM2 loopback |

## Serial Output Format

```
[PASS] PA0_AF1_TIM2_CH1_ETR: reg=0x1 exp=0x1
[FAIL] PA5_AF3_I2S_SCLK: reg=0x0 exp=0x3
[SKIP] PA9_AF7_UART1_TX: reserved for console/SWD/HSE
```

## LED Indicator

After tests complete:
- **Slow blink** (1 Hz): All tests passed
- **Fast blink** (10 Hz): One or more tests failed

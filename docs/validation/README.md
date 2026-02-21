# Validation Results

The WB32 ChibiOS port and associated drivers were validated through systematic
hardware testing on WB32F104RC and WB32FQ95xC development boards using CMSIS-DAP
debug probes.

## Methodology

Each validation phase examines a specific hardware subsystem through
register-level inspection via GDB/OpenOCD, cross-referenced against the
WB32FQ95xx reference manual. Tests progress from basic connectivity through
increasingly complex peripheral interactions, with each phase building on the
results of the previous one.

Validation artifacts include GDB session transcripts, register dumps, and
before/after comparisons for every bug fix.

## ChibiOS Port Validation

Eight phases covering the complete WB32FQ95xx ChibiOS-Contrib HAL port:

| Phase | Subsystem | Result |
|-------|-----------|--------|
| 1 | Clock Configuration | Correct |
| 2 | Peripheral Bus Mapping | Correct |
| 3 | GPIO Driver | Correct |
| 4 | USB Driver | Fixed |
| 5 | Timer Driver | Fixed |
| 6 | DMA Driver | Fixed |
| 7 | ADC Driver | Fixed |
| 8 | Flash Driver | Validated |

All phases passed. The port is production-ready for most applications.

See [chibios-port/summary.md](chibios-port/summary.md) for the overview and
individual phase files (`phase1-clock.md` through `phase8-flash.md`) for
detailed register-level results.

## OpenOCD Flash Driver Validation

Ten-phase hardware test of the custom OpenOCD WB32F10x flash driver:

| Phase | Description | Result |
|-------|-------------|--------|
| 0 | Build Preparation | PASS |
| 1 | Probe Connectivity and Device ID | PASS |
| 2 | Erase Pattern Determination | PASS |
| 3 | PRE_OP Calibration Verification | PASS |
| 4 | Sector Erase | PASS |
| 5 | Write and Verify (Small, 2 KB) | PASS |
| 6 | Write and Verify (Large, 18 KB) | PASS |
| 7 | Mass Erase | PASS |
| 8 | Stress Test (5+3+3 erase/write cycles) | PASS |
| 9 | Edge Cases | PASS |
| 10 | GDB Flash Test | PASS |

One critical bug was found and fixed during validation: PRE_OP must execute
before every individual erase and program operation, not once per session.

See [openocd-flash.md](openocd-flash.md) for the full test report.

## WS2812 GPIO DMA Driver Validation

Two-stage validation of the WS2812 GPIO DMA driver:

**Stage 1 -- GDB Register Validation** (8 phases, 13 test points):

| Phase | Test | Result |
|-------|------|--------|
| 1 | Firmware boot, DMA stream allocation | PASS |
| 2 | DMA register configuration (DAR, CTLL, CFGH) | PASS |
| 3 | 3-phase encoding, GRB order, MSB-first | PASS |
| 4 | Multi-frame completion (3 consecutive frames) | PASS |
| 5 | Interrupt mask lifecycle (enable/clear/re-enable) | PASS |
| 6 | 30-second sustained operation (0 stuck, 0 errors) | PASS |
| 7 | Error flag monitoring | PASS |
| 8 | Block transition timing | PASS |

**Stage 2 -- Visual Keyboard Validation**:

Flicker-free rainbow animation across all 81 LEDs on a Bridge75 ANSI keyboard.
Validated across extended operation with concurrent USB and UART traffic.

See [ws2812-dma.md](ws2812-dma.md) for the full test report.

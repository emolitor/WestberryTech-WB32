# Debug Tools

GDB scripts and J-Link configuration for debugging WB32 firmware via OpenOCD
and J-Link.

## GDB Scripts

Curated GDB scripts for register-level debugging of WB32 firmware through
OpenOCD's GDB server. Located in [`gdb/`](gdb/).

| Script | Purpose |
|--------|---------|
| [`debug_fault_info.gdb`](gdb/debug_fault_info.gdb) | HardFault analyzer. Dumps CFSR, HFSR, DFSR, MMFAR, BFAR and the exception stack frame (R0-R3, R12, LR, PC, xPSR). |
| [`debug_psp_check.gdb`](gdb/debug_psp_check.gdb) | Dual-stack (MSP/PSP) fault analysis for ChibiOS and other RTOS targets. Determines which stack was active at fault time. |
| [`debug_dma2.gdb`](gdb/debug_dma2.gdb) | DMA register inspector. Dumps all DMAC1 channel registers, interrupt status/mask registers, and TIM2 configuration. |
| [`debug_halt_check.gdb`](gdb/debug_halt_check.gdb) | Run-and-inspect diagnostic. Checks whether firmware is running or has crashed, and reports the current PC and fault status. |
| [`debug_interactive.gdb`](gdb/debug_interactive.gdb) | Interactive session starter with pre-configured breakpoints on DMA callback and HardFault_Handler. |
| [`debug_ws2812.gdb`](gdb/debug_ws2812.gdb) | WS2812 DMA driver debugger with callback tracking, buffer inspection, and block transition monitoring. |

### Usage

Start OpenOCD in one terminal:

```bash
openocd -f interface/cmsis-dap.cfg -f target/wb32.cfg
```

In another terminal, connect GDB with a script:

```bash
arm-none-eabi-gdb -x debug/gdb/debug_fault_info.gdb build/firmware.elf
```

Or load a script from within an existing GDB session:

```
(gdb) source debug/gdb/debug_dma2.gdb
```

## J-Link Configuration

J-Link device definition and flash algorithm for WB32 targets. Located in
[`jlink/`](jlink/).

| File | Purpose |
|------|---------|
| [`JLinkDevices.xml`](jlink/JLinkDevices.xml) | WB32 device definition for J-Link. Defines flash base address, size, sector layout, and flash algorithm path. |
| [`WB32F10x_256.FLM`](jlink/WB32F10x_256.FLM) | Flash algorithm (256 KB variant). Binary loaded by J-Link to perform flash erase and program operations. |

### Installation

Copy both files to your J-Link installation directory:

```bash
cp debug/jlink/JLinkDevices.xml /path/to/JLink/
cp debug/jlink/WB32F10x_256.FLM /path/to/JLink/Devices/Westberry/
```

Consult the SEGGER J-Link documentation for the correct device database
directory on your platform.

## See Also

- [emolitor/openocd](https://github.com/emolitor/openocd) -- OpenOCD fork with
  WB32 flash driver, build instructions, and configuration

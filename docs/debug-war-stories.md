# Debug War Stories

Three resolved debug cases from WB32 firmware development, each involving a non-obvious root cause
that required systematic investigation to diagnose.

---

## Case 1: DMA ISR Priority Crash

**Date**: 2026-01-27

### Symptom

A WS2812 PWM+DMA driver crashes after successfully completing all 5 DMA chunk transfers. The
program counter becomes `0x00000000`. All 5 DMA callbacks execute correctly with `flags=0x1`
(transfer complete). After the final callback sets `transfer_active = false`, the CPU jumps to
address zero.

The crash only occurs at full speed. Single-stepping through the same code path in GDB works
perfectly every time.

### Investigation

The DMA transfer-complete ISR was allocated with priority 1 via `dmaStreamAlloc()`. All five
callbacks fired in the correct order with the expected flags. No DMA errors were reported. The
crash always occurred immediately after the final callback returned, suggesting the problem was
in the ISR epilogue rather than the application logic.

The fact that GDB single-stepping masked the problem pointed toward a timing-dependent
interaction -- the debugger's halt-step-resume cycle serializes execution, preventing the race
condition that causes the crash at full speed.

### Root Cause

On ChibiOS/ARMv7-M, interrupt priorities 0-1 are classified as "fast" interrupts. They can
preempt the ChibiOS kernel itself, including its context switch and scheduler critical sections.
The DMA ISR used `OSAL_IRQ_PROLOGUE()` and `OSAL_IRQ_EPILOGUE()`, which manipulate the process
stack pointer (PSP) as part of ChibiOS's cooperative context switching.

When a priority-1 ISR preempts a kernel critical section that is also manipulating PSP, the
two stack pointer modifications conflict, corrupting the saved exception frame. On return from
the ISR, the corrupted frame loads `0x00000000` into PC.

ChibiOS priority levels on Cortex-M3:

| Priority | Classification | Safe for OSAL macros? |
|----------|----------------|----------------------|
| 0-1 | Fast (preempts kernel) | No |
| 2 | CORTEX_PRIORITY_SVCALL | No |
| 3+ | Kernel-managed (CORTEX_MAX_KERNEL_PRIORITY) | Yes |

Priority 2 was later (2026-02-07) also confirmed unsafe. Under UART interrupt stress, a
priority-2 DMAC1 ISR preempted `_port_irq_epilogue()`, pushed a second fake exception frame,
and caused `SVC_Handler` to return to an infinite loop (`.L1: b .L1`). The kernel's BASEPRI
lock value (`0x30`) only masks priorities >= 3, so priority 2 (equal to SVCall) sits above the
lock threshold. Fixing the DMA ISR to priority 3 eliminated the crash and produced a 32x frame
rate improvement by removing the stalls caused by the infinite loop recovery.

### Fix

Changed the DMA stream allocation priority from 1 to 3:

```c
// Before (crashes at full speed):
dmaStreamAlloc(stream, 1, callback, param);

// After (stable):
dmaStreamAlloc(stream, 3, callback, param);
```

### Lessons Learned

- **"Works with debugger, crashes at full speed" = ISR priority violation.** The debugger
  serializes ISR delivery by halting the CPU between steps, which prevents the preemption
  race. This is a strong diagnostic signal.
- **Any ISR that uses ChibiOS OSAL macros (`OSAL_IRQ_PROLOGUE`, `OSAL_IRQ_EPILOGUE`) must use
  a priority >= `CORTEX_MAX_KERNEL_PRIORITY` (3 on default ChibiOS configurations).** Priorities
  0-2 can preempt kernel internals and corrupt the process stack.
- **Priority 2 is not safe either.** It equals `CORTEX_PRIORITY_SVCALL` and is not masked by
  the kernel's BASEPRI lock. Under sustained interrupt load, it will eventually preempt a
  critical section.

---

## Case 2: PWM+DMA LED Glitching at Chunk Boundaries

**Date**: 2026-01-30

### Symptom

A WS2812 PWM+DMA driver passes standalone DMA validation testing (424 callbacks across stress
cycles, zero errors) but produces visible LED glitches starting around LED 20 when driving
actual WS2812 LEDs in QMK firmware.

The first ~20 LEDs display correct colors. LEDs beyond that point show corrupted or wrong
colors, with the corruption worsening further down the strip.

### Investigation

The DMA mechanism was thoroughly validated in isolation:

- Block sizes were correct (504 phases per chunk, 12 chunks total for 81 LEDs)
- All 424 transfer-complete callbacks fired with correct flags
- Zero DMA error callbacks
- Source addresses and transfer counts matched expectations at every chunk boundary

The glitch was not visible in DMA register state or callback telemetry. It was an electrical
timing issue on the WS2812 data line that only manifested when driving real LEDs.

### Root Cause

The WB32 timer continues running during ISR chunk transitions. When the DMA transfer-complete
ISR fires to reconfigure and start the next chunk, the timer's PWM output continues generating
cycles using the stale CCR (capture/compare register) value from the previous chunk's final
transfer.

These extra PWM cycles produce pulses with incorrect widths. The WS2812 protocol encodes bits
as pulse widths (short pulse = 0, long pulse = 1), so even one or two extra cycles with the
wrong duty cycle corrupt the bit stream. Since WS2812 LEDs shift data serially, corruption at
a chunk boundary propagates to all downstream LEDs.

Why the standalone DMA test passed: the test validated that DMA transfers completed correctly
(right number of callbacks, right flags, no errors) but did not connect to actual WS2812 LEDs.
The extra PWM cycles between chunks were electrically present but invisible to software
instrumentation. DMA worked perfectly -- the problem was the timer generating unwanted output
during the gap.

This is a fundamental limitation of PWM+DMA for multi-chunk WS2812 on the WB32. The timer
cannot be paused and restarted without introducing its own glitches, and the ISR latency
(even at sub-microsecond levels) allows enough stray pulses to corrupt downstream data.

### Fix

Replaced the PWM+DMA approach entirely with a GPIO DMA driver. Instead of modulating the PWM
duty cycle, the new driver writes directly to the GPIO BSRR (bit set/reset register) via DMA,
using the timer purely as a DMA request trigger (no PWM output).

The key advantage: GPIO pins hold their last written state. Each WS2812 bit ends with a RESET
phase that drives the pin LOW. When the ISR fires between chunks, the pin stays LOW -- no
stray pulses. The extended low period during the ISR pause (under 10 microseconds) is well
within the WS2812 protocol tolerance (the reset threshold is approximately 9 microseconds for
older parts, 280 microseconds for newer ones, but the inter-chunk gap is only the ISR
execution time).

See [ws2812-gpio-dma-driver.md](ws2812-gpio-dma-driver.md) for the full driver design.

### Lessons Learned

- **DMA validation that does not drive real hardware can miss electrical timing issues.** The
  DMA controller worked flawlessly -- the problem was the timer peripheral's behavior during
  ISR gaps. Always validate on actual target hardware, not just with register-level telemetry.
- **Timers do not stop when you stop feeding them DMA data.** On Cortex-M microcontrollers,
  a timer in PWM mode continues cycling at whatever CCR value was last written. Any gap in DMA
  delivery produces stray pulses. This is true of the WB32 and most STM32 families as well.
- **GPIO DMA is inherently glitch-free at chunk boundaries.** Because GPIO pins are
  level-hold (not edge-driven like PWM), any pause in DMA delivery simply extends the current
  pin state rather than generating unwanted transitions.

---

## Case 3: GDB Internal Crash with Stepping Loops

**Date**: 2026-01-27

### Symptom

`gdb-multiarch` segfaults when executing a `while` loop containing `stepi` commands in a GDB
batch script during remote debugging of a WB32FQ95xx target via OpenOCD.

### Investigation

The script used a GDB `commands` block with a loop to single-step through a section of code:

```gdb
commands
  set $step_count = 0
  while $step_count < 100
    stepi
    set $step_count = $step_count + 1
  end
end
```

GDB crashed with a segfault inside its own internals. The stack trace showed:

```
__strncmp_evex → [crash in GDB string comparison internals]
```

The crash was reproducible across multiple GDB sessions. The target (WB32FQ95xx via
CMSIS-DAP/OpenOCD) was functioning normally -- the crash was entirely within GDB's own
process. The same stepping commands executed manually (without the `while` loop) worked
fine.

### Root Cause

GDB has an internal bug in its scripting engine when handling programmatic stepping loops
over remote debug connections. The `stepi` command inside a `while` loop triggers a code path
in GDB's command processing that performs a string comparison on a null or invalid pointer.
This appears to be a known class of GDB bugs related to the interaction between its scripting
interpreter and the remote serial protocol (RSP) state machine.

The bug is specific to the combination of:
1. A scripted loop (`while`)
2. A stepping command (`stepi`, `nexti`, etc.)
3. A remote target (not a local process)

### Workaround

Replace scripted stepping loops with explicit sequential `stepi` commands:

```gdb
# Instead of a while loop, use explicit commands:
stepi
stepi
stepi
# ... repeat as needed
```

For large step counts, generate the GDB script programmatically:

```bash
# Generate 100 stepi commands
python3 -c "print('\n'.join(['stepi'] * 100))" > step_script.gdb
gdb-multiarch -batch -x step_script.gdb ...
```

### Lessons Learned

- **GDB has internal bugs with scripted stepping over remote connections.** Do not assume
  GDB scripting features are fully reliable in remote debug configurations. If GDB segfaults,
  the bug may be in GDB itself, not in the target firmware or debug adapter.
- **Use explicit commands for reliability.** Programmatic generation of flat command sequences
  is more reliable than GDB's built-in loop constructs for remote stepping.
- **Isolate the tool from the target.** When debugging crashes, always consider whether the
  crash is in the debugger, the debug adapter, or the target. A segfault in GDB's own process
  (visible in the host stack trace) is definitively a GDB bug.

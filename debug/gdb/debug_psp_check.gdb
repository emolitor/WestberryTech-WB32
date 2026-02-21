# Debug: Check PSP and stack pointers at crash
target extended-remote localhost:3333

file /home/emolitor/Development/Personal/Westberry-Tech/emolitor-qmk_firmware/.build/shortcut_bridge75_ansi_default.elf
monitor reset halt
load

set pagination off
set confirm off

set $cb_count = 0

# Break on callback to count
break *0x8005a88
commands 1
    silent
    set $cb_count = $cb_count + 1
    printf "CB#%d\n", $cb_count
    continue
end

# HardFault catcher
break HardFault_Handler
commands 2
    printf "\n!!! HARD FAULT !!!!\n"
    printf "Callback count: %d\n", $cb_count
    printf "\nRegisters:\n"
    printf "  PC  = 0x%08x\n", $pc
    printf "  LR  = 0x%08x\n", $lr
    printf "  MSP = 0x%08x\n", $msp
    printf "  PSP = 0x%08x\n", $psp
    printf "\nFault registers:\n"
    printf "  CFSR  = 0x%08x\n", *(unsigned int*)0xE000ED28
    printf "  HFSR  = 0x%08x\n", *(unsigned int*)0xE000ED2C
    printf "  MMFAR = 0x%08x\n", *(unsigned int*)0xE000ED34
    printf "  BFAR  = 0x%08x\n", *(unsigned int*)0xE000ED38
    printf "\nStack frames:\n"
    printf "MSP frame (if in handler mode):\n"
    printf "  [MSP+0x00]: 0x%08x R0\n", *(unsigned int*)($msp+0)
    printf "  [MSP+0x04]: 0x%08x R1\n", *(unsigned int*)($msp+4)
    printf "  [MSP+0x08]: 0x%08x R2\n", *(unsigned int*)($msp+8)
    printf "  [MSP+0x0c]: 0x%08x R3\n", *(unsigned int*)($msp+12)
    printf "  [MSP+0x10]: 0x%08x R12\n", *(unsigned int*)($msp+16)
    printf "  [MSP+0x14]: 0x%08x LR\n", *(unsigned int*)($msp+20)
    printf "  [MSP+0x18]: 0x%08x PC <-- faulting\n", *(unsigned int*)($msp+24)
    printf "  [MSP+0x1c]: 0x%08x xPSR\n", *(unsigned int*)($msp+28)
    printf "\nPSP frame (thread context):\n"
    printf "  [PSP+0x00]: 0x%08x R0\n", *(unsigned int*)($psp+0)
    printf "  [PSP+0x04]: 0x%08x R1\n", *(unsigned int*)($psp+4)
    printf "  [PSP+0x08]: 0x%08x R2\n", *(unsigned int*)($psp+8)
    printf "  [PSP+0x0c]: 0x%08x R3\n", *(unsigned int*)($psp+12)
    printf "  [PSP+0x10]: 0x%08x R12\n", *(unsigned int*)($psp+16)
    printf "  [PSP+0x14]: 0x%08x LR\n", *(unsigned int*)($psp+20)
    printf "  [PSP+0x18]: 0x%08x PC\n", *(unsigned int*)($psp+24)
    printf "  [PSP+0x1c]: 0x%08x xPSR\n", *(unsigned int*)($psp+28)
    disconnect
    quit
end

echo === Running - will dump stack info on fault ===\n
continue

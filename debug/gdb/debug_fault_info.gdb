# Debug: Get HardFault details
target extended-remote localhost:3333

file /home/emolitor/Development/Personal/Westberry-Tech/emolitor-qmk_firmware/.build/shortcut_bridge75_ansi_default.elf
monitor reset halt
load

set pagination off
set confirm off

# HardFault catcher with full fault register dump
break HardFault_Handler
commands 1
    printf "\n!!! HARD FAULT !!!!\n"
    printf "Registers: PC=0x%08x LR=0x%08x SP=0x%08x\n", $pc, $lr, $sp
    printf "\nFault Status Registers:\n"
    printf "  CFSR  = 0x%08x\n", *(unsigned int*)0xE000ED28
    printf "  HFSR  = 0x%08x\n", *(unsigned int*)0xE000ED2C
    printf "  DFSR  = 0x%08x\n", *(unsigned int*)0xE000ED30
    printf "  MMFAR = 0x%08x\n", *(unsigned int*)0xE000ED34
    printf "  BFAR  = 0x%08x\n", *(unsigned int*)0xE000ED38
    printf "\nException stack frame (from SP=0x%08x):\n", $sp
    printf "  R0:   0x%08x\n", *(unsigned int*)($sp+0)
    printf "  R1:   0x%08x\n", *(unsigned int*)($sp+4)
    printf "  R2:   0x%08x\n", *(unsigned int*)($sp+8)
    printf "  R3:   0x%08x\n", *(unsigned int*)($sp+12)
    printf "  R12:  0x%08x\n", *(unsigned int*)($sp+16)
    printf "  LR:   0x%08x\n", *(unsigned int*)($sp+20)
    printf "  PC:   0x%08x  <-- Faulting instruction\n", *(unsigned int*)($sp+24)
    printf "  xPSR: 0x%08x\n", *(unsigned int*)($sp+28)
    printf "\nWS2812 state:\n"
    printf "  ws2812_transfer_active  = %d\n", *(unsigned char*)0x20004fe8
    printf "  ws2812_dma_chunk_index  = %d\n", *(unsigned short*)0x20004fea
    disconnect
    quit
end

echo === Running - will catch HardFault and dump info ===\n
continue

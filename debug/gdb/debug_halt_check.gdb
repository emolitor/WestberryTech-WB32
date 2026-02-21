# Debug: Run briefly, halt, check state
target extended-remote localhost:3333

# Set this to your firmware ELF path
file build/firmware.elf
monitor reset halt
load

set pagination off
set confirm off

# No breakpoints - just run and halt
echo === Starting firmware ===\n
continue &
shell sleep 3
echo \n=== Halting after 3 seconds ===\n
monitor halt

printf "Current state:\n"
printf "  PC   = 0x%08x\n", $pc
printf "  LR   = 0x%08x\n", $lr
printf "  SP   = 0x%08x\n", $sp
printf "  xPSR = 0x%08x\n", $xpsr

printf "\nFault registers (check if any faults occurred):\n"
printf "  CFSR  = 0x%08x\n", *(unsigned int*)0xE000ED28
printf "  HFSR  = 0x%08x\n", *(unsigned int*)0xE000ED2C

printf "\nWS2812 state:\n"
printf "  ws2812_transfer_active  = %d\n", *(unsigned char*)0x20004fe8
printf "  ws2812_dma_chunk_index  = %d\n", *(unsigned short*)0x20004fea

printf "\nIf PC=0, check what's in the exception frame:\n"
if $pc == 0
    printf "  Stack[0x00]: 0x%08x\n", *(unsigned int*)($sp+0)
    printf "  Stack[0x04]: 0x%08x\n", *(unsigned int*)($sp+4)
    printf "  Stack[0x08]: 0x%08x\n", *(unsigned int*)($sp+8)
    printf "  Stack[0x0c]: 0x%08x\n", *(unsigned int*)($sp+12)
    printf "  Stack[0x10]: 0x%08x\n", *(unsigned int*)($sp+16)
    printf "  Stack[0x14]: 0x%08x\n", *(unsigned int*)($sp+20)
    printf "  Stack[0x18]: 0x%08x\n", *(unsigned int*)($sp+24)
    printf "  Stack[0x1c]: 0x%08x\n", *(unsigned int*)($sp+28)
end

disconnect
quit

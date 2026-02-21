# Simple debug script

target remote localhost:3333
monitor reset halt
load

# Set breakpoint at start_transfer
break start_transfer
continue

printf "\n=== At start_transfer ===\n"
printf "TIM3_CR1:  0x%08x (bit 0 = CEN)\n", *(uint32_t*)0x40000400
printf "TIM3_DIER: 0x%08x (bit 8 = UDE)\n", *(uint32_t*)0x4000040C

# Step through and show result
finish

printf "\n=== After start_transfer ===\n"
printf "dma_transfer_active: %d\n", dma_transfer_active

# Check DMA
printf "DMAC1_ChEnReg: 0x%08x\n", *(uint32_t*)0x40007E80
printf "DMAC1_DmaCfgReg: 0x%08x\n", *(uint32_t*)0x40007E98
printf "DMAC1_Ch0_CTLH: 0x%08x\n", *(uint32_t*)0x40007C1C

# Set breakpoint at callback
break dma_callback

printf "\nContinuing to wait for callback...\n"
continue

printf "\n=== Callback hit! ===\n"
bt
quit

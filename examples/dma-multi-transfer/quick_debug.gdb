# Quick debug to find where tests hang

target remote localhost:3333
monitor reset halt
load

# Start execution
printf "Starting program...\n"

# Set breakpoint at test_single_transfer (the first DMA test)
break test_single_transfer
continue

# When we hit this breakpoint, show state then continue
printf "\n=== Hit test_single_transfer ===\n"
printf "About to start first DMA transfer test\n"

# Single step into start_transfer and check DMA config
break start_transfer
continue

printf "\n=== Hit start_transfer ===\n"
printf "Check DMA and timer state:\n"

# Show PWM/Timer state
printf "\nTIM3 Registers:\n"
printf "  CR1:  0x%08x (bit 0 = CEN)\n", *(uint32_t*)0x40000400
printf "  DIER: 0x%08x (bit 8 = UDE)\n", *(uint32_t*)0x4000040C
printf "  CNT:  0x%08x\n", *(uint32_t*)0x40000424
printf "  ARR:  0x%08x\n", *(uint32_t*)0x4000042C

# Show DMA state
printf "\nDMAC1 State:\n"
printf "  DmaCfgReg: 0x%08x (bit 0 = enabled)\n", *(uint32_t*)0x40007E98
printf "  ChEnReg:   0x%08x (channels enabled)\n", *(uint32_t*)0x40007E80
printf "  Ch0 CTLH:  0x%08x (BLOCK_TS)\n", *(uint32_t*)0x40007C1C

# Step through dmaStreamEnable and check result
finish
printf "\n=== After start_transfer returns ===\n"
printf "dma_transfer_active: %d\n", dma_transfer_active
printf "dma_chunk_index: %d\n", dma_chunk_index

# Now show DMA state after configuration
printf "\nDMAC1 after dmaStreamEnable:\n"
printf "  DmaCfgReg: 0x%08x\n", *(uint32_t*)0x40007E98
printf "  ChEnReg:   0x%08x\n", *(uint32_t*)0x40007E80
printf "  Ch0 SAR:   0x%08x\n", *(uint32_t*)0x40007C00
printf "  Ch0 DAR:   0x%08x\n", *(uint32_t*)0x40007C08
printf "  Ch0 CTLH:  0x%08x\n", *(uint32_t*)0x40007C1C
printf "  Ch0 CFGH:  0x%08x\n", *(uint32_t*)0x40007C44

# Set breakpoint at dma_callback to see if it's ever called
break dma_callback
printf "\n=== Waiting for DMA callback (will timeout if not called)... ===\n"
printf "Use Ctrl+C to interrupt if it hangs\n"

# Continue and wait for callback
set pagination off
continue

printf "\n=== DMA callback hit! ===\n"
printf "flags parameter at callback entry\n"
bt

quit

# Debug script to investigate DMA callback issue

target remote localhost:3333
monitor reset halt

# Load firmware
printf "Loading firmware...\n"
load

# Set breakpoints at key locations
printf "Setting breakpoints...\n"
break dma_callback
break start_transfer
break test_single_transfer

# Define command to show DMA hardware registers
define dma_hw
    printf "=== DMAC1 Hardware State ===\n"
    printf "DmaCfgReg (0x40007E98):    0x%08x (bit 0 = DMAC enabled)\n", *(uint32_t*)0x40007E98
    printf "ChEnReg (0x40007E80):      0x%08x (channels enabled)\n", *(uint32_t*)0x40007E80
    printf "\n=== Channel 0 Registers ===\n"
    printf "SAR (0x40007C00):          0x%08x\n", *(uint32_t*)0x40007C00
    printf "DAR (0x40007C08):          0x%08x\n", *(uint32_t*)0x40007C08
    printf "CTLL (0x40007C18):         0x%08x\n", *(uint32_t*)0x40007C18
    printf "CTLH (0x40007C1C):         0x%08x (BLOCK_TS)\n", *(uint32_t*)0x40007C1C
    printf "CFGL (0x40007C40):         0x%08x\n", *(uint32_t*)0x40007C40
    printf "CFGH (0x40007C44):         0x%08x (HWHIF)\n", *(uint32_t*)0x40007C44
    printf "\n=== DMA Status ===\n"
    printf "RawTfr (0x40007E00):       0x%08x\n", *(uint32_t*)0x40007E00
    printf "RawBlock (0x40007E08):     0x%08x\n", *(uint32_t*)0x40007E08
    printf "RawErr (0x40007E20):       0x%08x\n", *(uint32_t*)0x40007E20
    printf "StatusTfr (0x40007EC0):    0x%08x\n", *(uint32_t*)0x40007EC0
    printf "MaskTfr (0x40007E90):      0x%08x (Int enable mask)\n", *(uint32_t*)0x40007E90
end

# Define command to show TIM3 state
define tim3
    printf "=== TIM3 Registers ===\n"
    printf "CR1 (0x40000400):          0x%08x (bit 0 = CEN)\n", *(uint32_t*)0x40000400
    printf "CR2 (0x40000404):          0x%08x\n", *(uint32_t*)0x40000404
    printf "DIER (0x4000040C):         0x%08x (bit 8 = UDE)\n", *(uint32_t*)0x4000040C
    printf "SR (0x40000410):           0x%08x (bit 0 = UIF)\n", *(uint32_t*)0x40000410
    printf "CNT (0x40000424):          0x%08x\n", *(uint32_t*)0x40000424
    printf "PSC (0x40000428):          0x%08x\n", *(uint32_t*)0x40000428
    printf "ARR (0x4000042C):          0x%08x\n", *(uint32_t*)0x4000042C
    printf "CCR2 (0x40000438):         0x%08x\n", *(uint32_t*)0x40000438
end

# Define command to show test state
define test_state
    printf "=== Test State ===\n"
    printf "current_test:          %d\n", current_test
    printf "dma_transfer_active:   %d\n", dma_transfer_active
    printf "dma_transfer_complete: %d\n", dma_transfer_complete
    printf "dma_chunk_index:       %d\n", dma_chunk_index
    printf "dma_callback_count:    %d\n", dma_callback_count
    printf "dma_error_count:       %d\n", dma_error_count
end

printf "\n"
printf "========================================\n"
printf " DMA Debug Session\n"
printf "========================================\n"
printf "Commands:\n"
printf "  dma_hw     - Show DMA hardware registers\n"
printf "  tim3       - Show TIM3 registers\n"
printf "  test_state - Show test variables\n"
printf "  continue   - Run until breakpoint\n"
printf "\n"
printf "Starting at main()...\n"
printf "\n"

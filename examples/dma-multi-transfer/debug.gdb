# GDB script for validating multi-transfer DMA on WB32FQ95
#
# Usage:
#   1. Start OpenOCD in one terminal:
#      openocd -f interface/cmsis-dap.cfg -f target/wb32fq95x.cfg
#
#   2. Run GDB in another terminal:
#      cd WB32FQ95-DMA-Test
#      gdb-multiarch -x debug.gdb build/dma_test.elf
#
# This script will:
#   - Connect to OpenOCD
#   - Load the firmware
#   - Set up helpful breakpoints
#   - Define commands for inspecting DMA state

# Connect to OpenOCD
target remote localhost:3333

# Reset and halt
monitor reset halt

# Load the firmware
load

# Set up breakpoints at key locations
break main.c:350
break dma_callback
break test_single_transfer
break test_consecutive_transfers
break test_rapid_transfers

# Define helper commands

# Print all DMA state variables
define dma_status
    printf "=== DMA Transfer Status ===\n"
    printf "transfer_active:    %d\n", dma_transfer_active
    printf "transfer_complete:  %d\n", dma_transfer_complete
    printf "chunk_index:        %d\n", dma_chunk_index
    printf "callback_count:     %d\n", dma_callback_count
    printf "error_count:        %d\n", dma_error_count
    printf "\n"
end

# Print chunk completion status
define dma_chunks
    printf "=== Chunk Status ===\n"
    printf "Total chunks: 4 (for 82 LEDs = 2008 bits)\n"
    printf "Chunk sizes: 511, 511, 511, 475\n"
    printf "\n"
    printf "Chunk 0: size=%3d, complete=%d\n", dma_chunk_sizes[0], dma_chunk_complete[0]
    printf "Chunk 1: size=%3d, complete=%d\n", dma_chunk_sizes[1], dma_chunk_complete[1]
    printf "Chunk 2: size=%3d, complete=%d\n", dma_chunk_sizes[2], dma_chunk_complete[2]
    printf "Chunk 3: size=%3d, complete=%d\n", dma_chunk_sizes[3], dma_chunk_complete[3]
    printf "\n"
end

# Print test results
define test_results
    printf "=== Test Results ===\n"
    printf "Current test: %d\n", current_test
    printf "\n"
    printf "Test 1 (chunk calculations): %s\n", test_results[1]==1 ? "PASS" : (test_results[1]==2 ? "FAIL" : "not run")
    printf "Test 2 (single transfer):    %s\n", test_results[2]==1 ? "PASS" : (test_results[2]==2 ? "FAIL" : "not run")
    printf "Test 3 (consecutive):        %s\n", test_results[3]==1 ? "PASS" : (test_results[3]==2 ? "FAIL" : "not run")
    printf "Test 4 (stress test):        %s\n", test_results[4]==1 ? "PASS" : (test_results[4]==2 ? "FAIL" : "not run")
    printf "\n"
end

# Show all debug info
define dma_info
    dma_status
    dma_chunks
    test_results
end

# Print DMA register state (DMAC1 Channel 0)
define dma_regs
    printf "=== DMAC1 Channel 0 Registers ===\n"
    printf "SAR  (Source):       0x%08x\n", *0x40007C00
    printf "DAR  (Dest):         0x%08x\n", *0x40007C08
    printf "CTLL (Control Low):  0x%08x\n", *0x40007C18
    printf "CTLH (Control High): 0x%08x (BLOCK_TS)\n", *0x40007C1C
    printf "CFG  (Config):       0x%08x\n", *0x40007C40
    printf "\n"
    printf "DMAC1 Status:\n"
    printf "  StatusTfr:   0x%08x\n", *0x40007E00
    printf "  StatusBlock: 0x%08x\n", *0x40007E08
    printf "  StatusErr:   0x%08x\n", *0x40007E20
    printf "\n"
end

# Run until tests complete
define run_tests
    printf "Running DMA multi-transfer tests...\n"
    printf "Break at main.c:350 to inspect before tests\n"
    continue
end

# Help
define dma_help
    printf "=== DMA Test Debug Commands ===\n"
    printf "dma_status  - Show transfer state variables\n"
    printf "dma_chunks  - Show chunk completion status\n"
    printf "test_results - Show test pass/fail status\n"
    printf "dma_info    - Show all debug info\n"
    printf "dma_regs    - Show DMA hardware registers\n"
    printf "run_tests   - Continue execution to run tests\n"
    printf "\n"
    printf "Breakpoints set at:\n"
    printf "  - main.c:350 (before tests start)\n"
    printf "  - dma_callback (each DMA transfer complete)\n"
    printf "  - test_* functions\n"
    printf "\n"
end

# Print welcome message
printf "\n"
printf "========================================\n"
printf " WB32FQ95 Multi-Transfer DMA Test\n"
printf "========================================\n"
printf "\n"
printf "Firmware loaded. Type 'dma_help' for commands.\n"
printf "\n"
printf "Quick start:\n"
printf "  1. Type 'run_tests' to start tests\n"
printf "  2. When breakpoint hits, use 'dma_info'\n"
printf "  3. Use 'continue' to proceed to next test\n"
printf "\n"

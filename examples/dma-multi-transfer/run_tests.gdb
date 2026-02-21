# Batch GDB script to run DMA tests

# Connect to OpenOCD
target remote localhost:3333

# Reset and halt
monitor reset halt

# Load firmware
printf "Loading firmware...\n"
load

printf "Firmware loaded. Starting execution...\n"

# Set breakpoint after all tests complete (in the success blink loop)
break main.c:410

# Run all tests
printf "\n=== Running all tests... ===\n"
printf "This will take a few seconds...\n\n"
continue

# Show final results
printf "\n"
printf "========================================\n"
printf " DMA Multi-Transfer Test Results\n"
printf "========================================\n"
printf "\n"

printf "DMA Transfer Status:\n"
printf "  callback_count:     %d\n", dma_callback_count
printf "  error_count:        %d\n", dma_error_count
printf "  transfer_active:    %d\n", dma_transfer_active
printf "  transfer_complete:  %d\n", dma_transfer_complete
printf "  chunk_index:        %d\n", dma_chunk_index
printf "\n"

printf "Chunk Completion Status:\n"
printf "  Chunk 0 (size %3d): %s\n", dma_chunk_sizes[0], dma_chunk_complete[0] ? "DONE" : "INCOMPLETE"
printf "  Chunk 1 (size %3d): %s\n", dma_chunk_sizes[1], dma_chunk_complete[1] ? "DONE" : "INCOMPLETE"
printf "  Chunk 2 (size %3d): %s\n", dma_chunk_sizes[2], dma_chunk_complete[2] ? "DONE" : "INCOMPLETE"
printf "  Chunk 3 (size %3d): %s\n", dma_chunk_sizes[3], dma_chunk_complete[3] ? "DONE" : "INCOMPLETE"
printf "\n"

printf "Test Results:\n"
printf "  Test 1 (chunk calculations): %s\n", test_results[1]==1 ? "PASS" : (test_results[1]==2 ? "FAIL" : "NOT RUN")
printf "  Test 2 (single transfer):    %s\n", test_results[2]==1 ? "PASS" : (test_results[2]==2 ? "FAIL" : "NOT RUN")
printf "  Test 3 (consecutive):        %s\n", test_results[3]==1 ? "PASS" : (test_results[3]==2 ? "FAIL" : "NOT RUN")
printf "  Test 4 (stress test):        %s\n", test_results[4]==1 ? "PASS" : (test_results[4]==2 ? "FAIL" : "NOT RUN")
printf "\n"

printf "Expected callback count: 424 (1 + 5 + 100 = 106 transfers x 4 chunks)\n"
printf "Actual callback count:   %d\n", dma_callback_count
printf "\n"

# Check overall pass/fail
set $all_passed = (test_results[1]==1) && (test_results[2]==1) && (test_results[3]==1) && (test_results[4]==1)
printf "========================================\n"
if $all_passed
    printf " OVERALL: ALL TESTS PASSED\n"
else
    printf " OVERALL: SOME TESTS FAILED\n"
end
printf "========================================\n"
printf "\n"

quit

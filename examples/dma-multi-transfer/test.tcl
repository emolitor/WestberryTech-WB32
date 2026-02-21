# OpenOCD TCL script to run tests

# Halt and load
reset halt
flash write_image erase build/dma_test.bin 0x08000000
verify_image build/dma_test.bin 0x08000000

# Set up breakpoint at start of tests (after LED blinks)
# Line numbers may vary but we'll break at test_single_transfer
bp 0x08003D84 2 hw

# Run
resume
after 5000

# Check results
halt
echo "Test Results:"
set test_results_addr 0x20000010
mdw $test_results_addr 10

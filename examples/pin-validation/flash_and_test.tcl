# SPDX-License-Identifier: MIT
# Copyright (c) 2026 emolitor (github.com/emolitor)
#
# OpenOCD TCL script: flash, run, poll test_results from SRAM
#
# Usage:
#   openocd -f interface/cmsis-dap.cfg -f target/wb32f10x.cfg \
#           -f examples/pin-validation/flash_and_test.tcl
#
# After first build, update TEST_RESULTS_ADDR from the .map file:
#   grep test_results build/PinValidation.map

# --- Configuration ---
# Default address -- update after first build by grepping the .map file
# for the test_results symbol address
set TEST_RESULTS_ADDR 0x200001f0
set TIMEOUT_MS 30000
set POLL_INTERVAL_MS 500
set MAGIC_COMPLETE 0xDEADBEEF

# --- Flash and run ---
echo "=== Pin Validation: Flash and Test ==="

reset halt
flash write_image erase build/PinValidation.bin 0x08000000
verify_image build/PinValidation.bin 0x08000000

echo "Firmware flashed and verified. Starting execution..."
resume

# --- Poll for completion ---
set elapsed 0
set complete 0

while {$elapsed < $TIMEOUT_MS} {
    after $POLL_INTERVAL_MS
    set elapsed [expr {$elapsed + $POLL_INTERVAL_MS}]

    halt
    set magic [mrw $TEST_RESULTS_ADDR]
    resume

    if {$magic == $MAGIC_COMPLETE} {
        set complete 1
        break
    }
}

if {!$complete} {
    echo "ERROR: Test did not complete within ${TIMEOUT_MS}ms"
    halt
    echo "Current magic value:"
    mdw $TEST_RESULTS_ADDR 1
    shutdown error
}

# --- Read results ---
halt

echo ""
echo "=== Test Results ==="

# Read result fields: magic, total, passed, failed, skipped
set results_base $TEST_RESULTS_ADDR
set total  [mrw [expr {$results_base + 4}]]
set passed [mrw [expr {$results_base + 8}]]
set failed [mrw [expr {$results_base + 12}]]
set skipped [mrw [expr {$results_base + 16}]]

echo "Total:   $total"
echo "Passed:  $passed"
echo "Failed:  $failed"
echo "Skipped: $skipped"

if {$failed == 0} {
    echo ""
    echo "*** ALL TESTS PASSED ***"
} else {
    echo ""
    echo "*** $failed TEST(S) FAILED ***"
    echo ""
    echo "Dumping first 20 result entries (20 bytes each: 40 name + 4 status + 4 expected + 4 actual):"

    # Each test_result_t is 52 bytes: 40 (name) + 4 (status) + 4 (expected) + 4 (actual)
    set entry_size 52
    set results_start [expr {$results_base + 20}]  ;# After magic + 4 counters
    set dump_count [expr {$total < 20 ? $total : 20}]

    for {set i 0} {$i < $dump_count} {incr i} {
        set addr [expr {$results_start + $i * $entry_size}]
        set status_addr [expr {$addr + 40}]
        set status [mrw $status_addr]

        # Only show failures (status == 1)
        if {$status == 1} {
            set exp [mrw [expr {$status_addr + 4}]]
            set act [mrw [expr {$status_addr + 8}]]
            echo "  FAIL entry $i: expected=0x[format %08x $exp] actual=0x[format %08x $act]"
            # Dump the name field as hex words
            echo -n "    name: "
            mdw $addr 10
        }
    }
}

echo ""
echo "=== Done ==="
shutdown

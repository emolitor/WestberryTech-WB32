# WS2812 PWM DMA Debug Script
# Connect to OpenOCD
target extended-remote localhost:3333

# Reset and halt
monitor reset halt

# Load the firmware
# Set this to your firmware ELF path
file build/firmware.elf
load

# Define convenience variables to track state
set $callback_count = 0
set $flush_count = 0
set $timeout_count = 0
set $error_count = 0

# Set breakpoints with commands

# Breakpoint on DMA callback
break ws2812_dma_callback
commands
    silent
    set $callback_count = $callback_count + 1
    # Check the flags parameter
    printf "CB#%d: chunk_idx=%d, flags=0x%x\n", $callback_count, ws2812_dma_chunk_index, $r1
    continue
end

# Breakpoint on flush timeout (when timeout variable reaches 0)
# We'll check this by breaking at the timeout reset line
break ws2812_pwm.c:582
commands
    silent
    set $timeout_count = $timeout_count + 1
    printf "TIMEOUT#%d: DMA stuck! transfer_active=%d, chunk_idx=%d\n", $timeout_count, ws2812_transfer_active, ws2812_dma_chunk_index
    # Print DMA register state
    printf "DMA ChEnReg: 0x%x\n", *(uint32_t*)0x400073A0
    printf "DMA StatusTfr: 0x%x\n", *(uint32_t*)0x400072E8
    printf "DMA StatusBlock: 0x%x\n", *(uint32_t*)0x400072F0
    printf "DMA StatusErr: 0x%x\n", *(uint32_t*)0x40007310
    continue
end

# Breakpoint on error condition in callback
break ws2812_pwm.c:360
commands
    silent
    set $error_count = $error_count + 1
    printf "DMA_ERROR#%d: flags=0x%x\n", $error_count, $r1
    continue
end

# Start running
printf "Starting firmware with DMA debugging enabled...\n"
printf "Press Ctrl+C to stop and examine state\n"
monitor reset run

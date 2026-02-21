# Interactive DMA Debug
target extended-remote localhost:3333

# Set this to your firmware ELF path
file build/firmware.elf
monitor reset halt
load

# Set a hardware breakpoint on the DMA callback function address
# First find the address
info address ws2812_dma_callback

# Set breakpoint using the function name
break ws2812_dma_callback

# Also set a breakpoint on HardFault to catch crashes
break HardFault_Handler

# Now run
echo \n=== Starting - will break on DMA callback or crash ===\n
continue

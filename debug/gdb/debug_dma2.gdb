# WS2812 DMA Debug - Direct Memory Access
target extended-remote localhost:3333

# Load firmware
# Set this to your firmware ELF path
file build/firmware.elf
monitor reset halt
load

# Run for 5 seconds
echo Starting firmware...\n
monitor reset run
shell sleep 5
monitor halt

echo \n=== Halted - Examining State ===\n

# Get PC and call stack
echo Program Counter and Stack:\n
info registers pc sp lr
backtrace

# DMA1 Channel 0 Registers (DMAC1 base = 0x40007000)
echo \n=== DMAC1 Channel 0 Registers ===\n
echo SAR (Source Address):
x/1xw 0x40007000
echo DAR (Dest Address):
x/1xw 0x40007008
echo CTLL (Control Low):
x/1xw 0x40007018
echo CTLH (Block Size):
x/1xw 0x4000701C
echo ChEnReg (Enable):
x/1xw 0x400073A0

# DMA1 Channel 1 Registers (offset 0x58 per channel)
echo \n=== DMAC1 Channel 1 Registers ===\n
echo SAR (Source Address):
x/1xw 0x40007058
echo DAR (Dest Address):
x/1xw 0x40007060
echo CTLL (Control Low):
x/1xw 0x40007070
echo CTLH (Block Size):
x/1xw 0x40007074

# DMA Interrupt Status
echo \n=== DMA Interrupt Status ===\n
echo RawTfr:
x/1xw 0x400072C0
echo RawBlock:
x/1xw 0x400072C8
echo RawErr:
x/1xw 0x400072E0
echo StatusTfr:
x/1xw 0x400072E8
echo StatusBlock:
x/1xw 0x400072F0
echo StatusErr:
x/1xw 0x40007310
echo MaskTfr:
x/1xw 0x40007318
echo MaskBlock:
x/1xw 0x40007320
echo MaskErr:
x/1xw 0x40007338

# TIM2 Registers (base = 0x40000000)
echo \n=== TIM2 (PWM Timer) Registers ===\n
echo TIM2_CR1 (Control):
x/1xw 0x40000000
echo TIM2_DIER (DMA/Int):
x/1xw 0x4000000C
echo TIM2_SR (Status):
x/1xw 0x40000010
echo TIM2_CNT (Counter):
x/1xw 0x40000024
echo TIM2_ARR (Auto-reload):
x/1xw 0x4000002C
echo TIM2_CCR3 (Our PWM channel):
x/1xw 0x4000003C

# Find ws2812 variables in memory by searching for symbols
echo \n=== Looking for ws2812 symbols ===\n
info address ws2812_transfer_active
info address ws2812_dma_chunk_index

quit

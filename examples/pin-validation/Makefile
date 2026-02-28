# ------------------------------------------------
# Pin Validation Test Suite Makefile
# Based on examples/blink/Makefile
# ------------------------------------------------

######################################
# target
######################################
TARGET = PinValidation
CHIP ?= F104RC


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

# Vendor peripheral library (relative to this Makefile)
VENDOR_LIB = ../../vendor-lib/WB32F10x_StdPeriph_Lib_V0.1.10/Libraries
VENDOR_UTIL = ../../vendor-lib/WB32F10x_StdPeriph_Lib_V0.1.10/Utilities/Common

######################################
# source
######################################
# C sources
C_SOURCES = \
$(VENDOR_LIB)/CMSIS/Device/WB/WB32F10x/system_wb32f10x.c \
$(VENDOR_LIB)/WB32F10x_StdPeriph_Driver/src/misc.c \
$(VENDOR_LIB)/WB32F10x_StdPeriph_Driver/src/wb32f10x_adc.c \
$(VENDOR_LIB)/WB32F10x_StdPeriph_Driver/src/wb32f10x_anctl.c \
$(VENDOR_LIB)/WB32F10x_StdPeriph_Driver/src/wb32f10x_bkp.c \
$(VENDOR_LIB)/WB32F10x_StdPeriph_Driver/src/wb32f10x_crc.c \
$(VENDOR_LIB)/WB32F10x_StdPeriph_Driver/src/wb32f10x_dmac.c \
$(VENDOR_LIB)/WB32F10x_StdPeriph_Driver/src/wb32f10x_exti.c \
$(VENDOR_LIB)/WB32F10x_StdPeriph_Driver/src/wb32f10x_fmc.c \
$(VENDOR_LIB)/WB32F10x_StdPeriph_Driver/src/wb32f10x_gpio.c \
$(VENDOR_LIB)/WB32F10x_StdPeriph_Driver/src/wb32f10x_i2c.c \
$(VENDOR_LIB)/WB32F10x_StdPeriph_Driver/src/wb32f10x_i2s.c \
$(VENDOR_LIB)/WB32F10x_StdPeriph_Driver/src/wb32f10x_iwdg.c \
$(VENDOR_LIB)/WB32F10x_StdPeriph_Driver/src/wb32f10x_led.c \
$(VENDOR_LIB)/WB32F10x_StdPeriph_Driver/src/wb32f10x_pwr.c \
$(VENDOR_LIB)/WB32F10x_StdPeriph_Driver/src/wb32f10x_rcc.c \
$(VENDOR_LIB)/WB32F10x_StdPeriph_Driver/src/wb32f10x_rng.c \
$(VENDOR_LIB)/WB32F10x_StdPeriph_Driver/src/wb32f10x_rtc.c \
$(VENDOR_LIB)/WB32F10x_StdPeriph_Driver/src/wb32f10x_sfm.c \
$(VENDOR_LIB)/WB32F10x_StdPeriph_Driver/src/wb32f10x_spi.c \
$(VENDOR_LIB)/WB32F10x_StdPeriph_Driver/src/wb32f10x_tim.c \
$(VENDOR_LIB)/WB32F10x_StdPeriph_Driver/src/wb32f10x_uart.c \
$(VENDOR_LIB)/WB32F10x_StdPeriph_Driver/src/wb32f10x_wwdg.c \
$(VENDOR_UTIL)/bsp_uart1.c \
main.c \
wb32f10x_it.c \
test_gpio_af.c \
test_uart.c \
test_timer.c \
test_adc.c \
test_spi.c \
test_i2c.c \
test_led.c \
test_usb.c

# ASM sources
ASM_SOURCES = \
$(VENDOR_LIB)/CMSIS/Device/WB/WB32F10x/startup/gcc/startup_wb32f10x.S


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m3

# mcu
MCU = $(CPU) -mthumb

# macros for gcc
AS_DEFS =

# C defines
C_DEFS += -DUSE_STDPERIPH_DRIVER
C_DEFS += -DHSE_VALUE=12000000
C_DEFS += -DMAINCLK_FREQ_96MHz

# C includes
C_INCLUDES = \
-I$(VENDOR_LIB)/CMSIS/Include \
-I$(VENDOR_LIB)/CMSIS/Device/WB/WB32F10x \
-I$(VENDOR_LIB)/WB32F10x_StdPeriph_Driver/inc \
-I$(VENDOR_UTIL) \
-I.


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
ifeq ($(CHIP),FQ95)
  LDSCRIPT = WB32FQ95_FLASH.ld
  C_DEFS += -DTARGET_WB32FQ95
else
  LDSCRIPT = WB32F10x_FLASH.ld
  C_DEFS += -DTARGET_WB32F104RC
endif

# libraries
LIBS = -lc -lm -lnosys
LDFLAGS = $(MCU) -specs=nano.specs -u _printf_float -T$(LDSCRIPT) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections -Wl,--no-warn-rwx-segments

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.S=.o)))
vpath %.S $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.S Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

$(BUILD_DIR):
	mkdir $@

#######################################
# flash
#######################################
flash: $(BUILD_DIR)/$(TARGET).bin
	openocd -f interface/cmsis-dap.cfg -f target/wb32f10x.cfg \
		-c "program $(BUILD_DIR)/$(TARGET).bin 0x08000000 verify reset exit"

test: $(BUILD_DIR)/$(TARGET).bin
	openocd -f interface/cmsis-dap.cfg -f target/wb32f10x.cfg \
		-f flash_and_test.tcl

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***

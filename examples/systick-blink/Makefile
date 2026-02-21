# ------------------------------------------------
# Generic Makefile
# ------------------------------------------------

######################################
# GCC Path
######################################
#GCC_PATH ?= /usr/local/arm-gnu-toolchain-14.3.rel1-x86_64-arm-none-eabi/bin
#GCC_PATH ?= /usr/local/gcc-arm-none-eabi-10.3-2021.10/bin
#GCC_PATH ?= /usr/local/gcc-arm-none-eabi-9-2020-q2-update/bin


######################################
# target
######################################
TARGET = Template


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

######################################
# source
######################################
# C sources
C_SOURCES =  \
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
main.c \
wb32f10x_it.c  

# ASM sources
ASM_SOURCES =  \
$(VENDOR_LIB)/CMSIS/Device/WB/WB32F10x/startup/gcc/startup_wb32f10x.S


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
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

# fpu
# NONE for Cortex-M0/M0+/M3

# float-abi


# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS += -DUSE_STDPERIPH_DRIVER
#C_DEFS += -DHSE_VALUE=8000000
C_DEFS += -DHSE_VALUE=12000000
#C_DEFS += -DMAINCLK_FREQ_32MHz
#C_DEFS += -DMAINCLK_FREQ_48MHz
#C_DEFS += -DMAINCLK_FREQ_64MHz
#C_DEFS += -DMAINCLK_FREQ_72MHz
C_DEFS += -DMAINCLK_FREQ_96MHz
#C_DEFS += -DMAINCLK_FREQ_128MHz # Not a valid otpion

# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-I$(VENDOR_LIB)/CMSIS/Include \
-I$(VENDOR_LIB)/CMSIS/Device/WB/WB32F10x \
-I$(VENDOR_LIB)/WB32F10x_StdPeriph_Driver/inc \
-I.


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

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
LDSCRIPT = WB32F10x_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

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
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***

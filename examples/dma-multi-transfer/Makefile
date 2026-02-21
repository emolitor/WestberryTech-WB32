##############################################################################
# Build global options
#

# Compiler options
ifeq ($(USE_OPT),)
  USE_OPT = -O0 -ggdb -fomit-frame-pointer -falign-functions=16
endif

# C specific options
ifeq ($(USE_COPT),)
  USE_COPT =
endif

# C++ specific options
ifeq ($(USE_CPPOPT),)
  USE_CPPOPT = -fno-rtti
endif

# Enable linker garbage collection
ifeq ($(USE_LINK_GC),)
  USE_LINK_GC = yes
endif

# Linker extra options
ifeq ($(USE_LDOPT),)
  USE_LDOPT =
endif

# Link time optimization (disabled for debugging)
ifeq ($(USE_LTO),)
  USE_LTO = no
endif

# Verbose compile
ifeq ($(USE_VERBOSE_COMPILE),)
  USE_VERBOSE_COMPILE = yes
endif

# Smart build
ifeq ($(USE_SMART_BUILD),)
  USE_SMART_BUILD = yes
endif

##############################################################################
# Architecture options
#

# Stack sizes
ifeq ($(USE_PROCESS_STACKSIZE),)
  USE_PROCESS_STACKSIZE = 0x800
endif

ifeq ($(USE_EXCEPTIONS_STACKSIZE),)
  USE_EXCEPTIONS_STACKSIZE = 0x400
endif

# FPU (none for Cortex-M3)
ifeq ($(USE_FPU),)
  USE_FPU = no
endif

##############################################################################
# Project, target, sources and paths
#

# Project name
PROJECT = dma_test

# Target
MCU  = cortex-m3

# Paths to ChibiOS (via QMK firmware tree)
CHIBIOS  := ../emolitor-qmk_firmware/lib/chibios
CHIBIOS_CONTRIB := ../emolitor-qmk_firmware/lib/chibios-contrib

CONFDIR  := ./cfg
BUILDDIR := ./build
DEPDIR   := ./.dep
BOARDDIR := ./board

# Licensing
include $(CHIBIOS)/os/license/license.mk

# Startup files for WB32FQ95xx
include $(CHIBIOS_CONTRIB)/os/common/startup/ARMCMx/compilers/GCC/mk/startup_wb32fq95xx.mk

# HAL-OSAL files
include $(CHIBIOS)/os/hal/hal.mk
include $(CHIBIOS_CONTRIB)/os/hal/ports/WB32/WB32FQ95xx/platform.mk
include $(BOARDDIR)/board.mk
include $(CHIBIOS)/os/hal/osal/rt-nil/osal.mk

# RTOS files
include $(CHIBIOS)/os/rt/rt.mk
include $(CHIBIOS)/os/common/ports/ARMv7-M/compilers/GCC/mk/port.mk

# Auto-build
include $(CHIBIOS)/tools/mk/autobuild.mk

# Linker script
LDSCRIPT= $(STARTUPLD_CONTRIB)/WB32FQ95xC.ld

# C sources
# Select which test to build:
#   make                    -> single DMA test (default)
#   make MAIN=main.c        -> simple blink
#   make MAIN=main_dma_test.c -> multi-chunk DMA test
MAIN ?= main_single_dma_test.c

CSRC = $(ALLCSRC) \
       $(BOARDSRC) \
       $(MAIN)

# C++ sources
CPPSRC = $(ALLCPPSRC)

# ASM sources
ASMSRC = $(ALLASMSRC)
ASMXSRC = $(ALLXASMSRC)

# Include directories
INCDIR = $(CONFDIR) $(ALLINC) $(BOARDDIR)

# Warnings
CWARN = -Wall -Wextra -Wundef -Wstrict-prototypes
CPPWARN = -Wall -Wextra -Wundef

##############################################################################
# User section
#

UDEFS =
UADEFS =
UINCDIR =
ULIBDIR =
ULIBS =

##############################################################################
# Common rules
#

RULESPATH = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/mk
include $(RULESPATH)/arm-none-eabi.mk
include $(RULESPATH)/rules.mk

##############################################################################
# Custom rules
#

# OpenOCD path (build from https://github.com/emolitor/openocd)
OPENOCD_SRC := ../openocd-src

# Flash via OpenOCD with CMSIS-DAP
flash: $(BUILDDIR)/$(PROJECT).bin
	$(OPENOCD_SRC)/src/openocd -s /usr/share/openocd/scripts -s $(OPENOCD_SRC)/tcl \
		-f interface/cmsis-dap.cfg -f target/wb32fq95x.cfg \
		-c "init; halt; flash probe 0" \
		-c "flash write_image erase $(BUILDDIR)/$(PROJECT).bin 0x08000000" \
		-c "verify_image $(BUILDDIR)/$(PROJECT).bin 0x08000000" \
		-c "reset run; shutdown"

# Debug via OpenOCD + GDB
debug: $(BUILDDIR)/$(PROJECT).elf
	@echo "Starting OpenOCD in background..."
	$(OPENOCD_SRC)/src/openocd -s /usr/share/openocd/scripts -s $(OPENOCD_SRC)/tcl \
		-f interface/cmsis-dap.cfg -f target/wb32fq95x.cfg &
	sleep 2
	gdb-multiarch -x debug.gdb $(BUILDDIR)/$(PROJECT).elf

# Just start OpenOCD server
openocd:
	$(OPENOCD_SRC)/src/openocd -s /usr/share/openocd/scripts -s $(OPENOCD_SRC)/tcl \
		-f interface/cmsis-dap.cfg -f target/wb32fq95x.cfg

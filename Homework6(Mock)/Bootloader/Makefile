###############################################################################
# Simple Makefile for Mock S32K144 Bootloader (GNU Arm Embedded Toolchain)
# Usage (PowerShell):  make                # build
#                      make clean          # clean
# Optional: set environment variable GCC_PREFIX if toolchain not in PATH.
# Example: $env:GCC_PREFIX="C:/gcc-arm-none-eabi/bin/arm-none-eabi-"
###############################################################################

GCC_PREFIX ?= arm-none-eabi-
CC      := $(GCC_PREFIX)gcc
AS      := $(GCC_PREFIX)gcc
OBJCOPY := $(GCC_PREFIX)objcopy
OBJDUMP := $(GCC_PREFIX)objdump
SIZE    := $(GCC_PREFIX)size

PROJECT := bootloader

INCLUDES := -Iinclude -IProject_Settings/Startup_Code

C_SOURCES := \
  src/main.c \
  src/boot_manager.c \
  src/boot_comm.c \
  src/boot_crc.c \
  src/flash_if.c \
  src/boot_jump.c \
  Project_Settings/Startup_Code/startup.c \
  Project_Settings/Startup_Code/system_S32K144.c

ASM_SOURCES := Project_Settings/Startup_Code/startup_S32K144.S

LDSCRIPT := Project_Settings/Linker_Files/S32K144_64_flash.ld

OBJ_DIR := build
OBJS := $(C_SOURCES:%.c=$(OBJ_DIR)/%.o) $(ASM_SOURCES:%.S=$(OBJ_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

CPUFLAGS := -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
CFLAGS := $(CPUFLAGS) -Os -ffunction-sections -fdata-sections -fno-common -Wall -Wextra -std=c11 \
  -DMOCK_BOOTLOADER -fno-builtin -g3

LDFLAGS := $(CPUFLAGS) -Wl,--gc-sections -Wl,-Map=$(PROJECT).map -T$(LDSCRIPT)

all: $(PROJECT).elf $(PROJECT).bin

$(PROJECT).elf: $(OBJS) $(LDSCRIPT)
	$(CC) $(OBJS) $(LDFLAGS) -o $@
	$(SIZE) $@

$(PROJECT).bin: $(PROJECT).elf
	$(OBJCOPY) -O binary $< $@

$(OBJ_DIR)/%.o: %.c | prep
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

$(OBJ_DIR)/%.o: %.S | prep
	@mkdir -p $(dir $@)
	$(AS) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

prep:
	@mkdir -p $(OBJ_DIR)/src
	@mkdir -p $(OBJ_DIR)/Project_Settings/Startup_Code

clean:
	rm -rf $(OBJ_DIR) *.elf *.bin *.map

-include $(DEPS)

.PHONY: all clean prep

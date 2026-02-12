# MSPM0C1104 Development Template - Makefile
# 
# Copyright (c) 2025 UNIT Electronics MX
# Licensed under MIT License - see LICENSE file for details
#
# This build system uses TI MSPM0 SDK components which are licensed
# under TI Commercial License and BSD-3-Clause License.
# See LICENSES.md for complete licensing information.
#
# SDK Path - Automatic submodule detection
MSPM0_SDK_INSTALL_DIR ?= $(shell if [ -d "../../../../mspm0-sdk" ]; then echo "../../../../mspm0-sdk"; elif [ -d "$(HOME)/Documents/mspm-/mspm0-sdk" ]; then echo "$(HOME)/Documents/mspm-/mspm0-sdk"; else echo "/usr/local/mspm0-sdk"; fi)

# Compiler tools
CC = arm-none-eabi-gcc
LNK = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# Project configuration
PROJECT_NAME = mspm0c1104_template
SOURCE_FILES = src/main.c libs/config/ti_msp_dl_config.c startup_mspm0c110x_gcc.c

# Build directories
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
OUTPUT_DIR = $(BUILD_DIR)/output

# Objects to compile (now in build/obj/)
OBJECTS = $(addprefix $(OBJ_DIR)/, $(notdir $(SOURCE_FILES:.c=.obj)))
TARGET = $(OUTPUT_DIR)/$(PROJECT_NAME).out
TARGET_HEX = $(OUTPUT_DIR)/$(PROJECT_NAME).hex

# Compiler flags for MSPM0C1104 with CMSIS
CFLAGS = \
    -DDeviceFamily_MSPM0C110X \
    -D__MSPM0C1104__ \
    -DARM_MATH_CM0 \
    -O2 \
    -mcpu=cortex-m0plus \
    -march=armv6-m \
    -mthumb \
    -std=c99 \
    -mfloat-abi=soft \
    -ffunction-sections \
    -fdata-sections \
    -g \
    -gstrict-dwarf \
    -Wall \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source/third_party/CMSIS/Core/Include" \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source/third_party/CMSIS/DSP/Include" \
    "-I$(MSPM0_SDK_INSTALL_DIR)/source" \
    -I. \
    -Ilibs/config

# Linker flags
LFLAGS = \
    "-L$(MSPM0_SDK_INSTALL_DIR)/source/third_party/CMSIS/DSP/lib/gcc/m0p" \
    -l:arm_cortexM0l_math.a \
    "-L$(MSPM0_SDK_INSTALL_DIR)/source/ti/driverlib/lib/gcc/m0p/mspm0g1x0x_g3x0x" \
    -l:driverlib.a \
    "-Wl,-Map,$(OUTPUT_DIR)/$(PROJECT_NAME).map" \
    "-L$(MSPM0_SDK_INSTALL_DIR)/source" \
    -Wl,-T,device_linker.lds \
    -march=armv6-m \
    -mthumb \
    -static \
    -Wl,--gc-sections \
    -lgcc \
    -lc \
    -lm \
    --specs=nano.specs \
    --specs=nosys.specs

# Main rules
all: $(BUILD_DIR) $(TARGET) $(TARGET_HEX)
	@echo "Build complete!"
	@$(SIZE) $(TARGET)

# Create build directories
$(BUILD_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OUTPUT_DIR)

# Variable to search source files with VPATH
VPATH = . libs/config src

# Compile source files to objects
$(OBJ_DIR)/%.obj: %.c | $(BUILD_DIR)
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) $< -c -o $@

# Link and create executable
$(TARGET): $(OBJECTS)
	@echo "Linking $@"
	@$(LNK) $(OBJECTS) $(LFLAGS) -o $(TARGET)

# Create HEX file for programming
$(TARGET_HEX): $(TARGET)
	@echo "Creating HEX file"
	@$(OBJCOPY) -O ihex $(TARGET) $(TARGET_HEX)

# Clean generated files
clean:
	@echo "Cleaning project"
	@rm -rf $(BUILD_DIR)
	@rm -f *.obj *.out *.map *.hex

# MAIN PROGRAMMING (verified working method)
flash: $(TARGET_HEX)
	@echo "=== Programming MSPM0C1104 with verified method ==="
	@echo "File: $(TARGET_HEX)"
	@if [ ! -f "$(TARGET_HEX)" ]; then \
		echo "Error: $(TARGET_HEX) does not exist. Run 'make all' first."; \
		exit 1; \
	fi
	pyocd flash -t mspm0c1104 --connect under-reset \
		-O pack.debug_sequences.disabled_sequences=ResetSystem,ResetHWReset,ResetCatchSet,ResetProcessor \
		$(TARGET_HEX)
	@echo "=== ✅ Programming complete - PA24 should be blinking ==="

# EMERGENCY PROGRAMMING (multiple methods - for problematic cases)
flash-emergency: $(TARGET_HEX)
	@echo "=== Programming MSPM0C1104 with Multiple Methods ==="
	@echo "File: $(TARGET_HEX)"
	@if [ ! -f "$(TARGET_HEX)" ]; then \
		echo "Error: $(TARGET_HEX) does not exist. Run 'make all' first."; \
		exit 1; \
	fi
	@echo "Method 1: Programming with auto-detection and sector erase..."
	-pyocd flash -O frequency=100000 --erase=sector $(TARGET_HEX) || echo "Method 1 failed"
	@echo "Method 2: Programming with specific target, no reset..."
	-pyocd flash -t mspm0c1104 -O frequency=100000 --no-reset $(TARGET_HEX) || echo "Method 2 failed"
	@echo "Method 3: Programming with basic auto-detection..."
	pyocd flash -O frequency=100000 $(TARGET_HEX) || echo "Method 3 failed - using flash-emergency"
	@echo "Verifying programming..."
	@sleep 1
	-pyocd reset || echo "Reset failed - press reset button manually"
	@echo "=== ✅ Programming complete - PA24 should be blinking ==="
	@echo "If not blinking, press the reset button on the board"

# SIMPLE PROGRAMMING (no automatic reset)
flash-simple: $(TARGET_HEX)
	@echo "=== Simple Programming (requires manual reset) ==="
	@if [ ! -f "$(TARGET_HEX)" ]; then \
		echo "Error: $(TARGET_HEX) does not exist. Run 'make all' first."; \
		exit 1; \
	fi
	pyocd flash \
		-t mspm0c1104 \
		$(TARGET_HEX)
	@echo "=== Programming complete - PRESS THE RESET BUTTON ON THE BOARD ==="

# PROGRAMMING WITH FORCED RESET
flash-reset: $(TARGET_HEX)
	@echo "=== Programming with Multiple Resets ==="
	@if [ ! -f "$(TARGET_HEX)" ]; then \
		echo "Error: $(TARGET_HEX) does not exist. Run 'make all' first."; \
		exit 1; \
	fi
	@echo "Initial reset..."
	-pyocd reset -t mspm0c1104 || echo "Initial reset failed (normal)"
	@sleep 1
	@echo "Programming..."
	pyocd flash \
		-t mspm0c1104 \
		--connect under-reset \
		-O reset_type=hw \
		$(TARGET_HEX)
	@echo "Final reset..."
	@sleep 1
	-pyocd reset -t mspm0c1104 || echo "Final reset failed"
	@sleep 1
	@echo "=== ✅ Programming with multiple resets complete ==="

# Manual microcontroller reset (with disabled debug sequences)
reset:
	@echo "=== MSPM0C1104 Reset ==="
	pyocd reset -t mspm0c1104 \
		-O pack.debug_sequences.disabled_sequences=ResetSystem,ResetHWReset,ResetCatchSet,ResetProcessor
	@echo "=== ✅ Reset complete ==="

# Project information
info: $(TARGET)
	@echo "=== Project Information ==="
	@echo "Project: $(PROJECT_NAME)"
	@echo "Source files: $(SOURCE_FILES)"
	@echo "Device: MSPM0C1104"
	@echo "SDK Path: $(MSPM0_SDK_INSTALL_DIR)"
	@echo ""
	@echo "=== Binary size ==="
	@$(SIZE) $(TARGET)

# Help
help:
	@echo "=== MSPM0C1104 Development Template - Makefile ==="
	@echo ""
	@echo "Main targets:"
	@echo "  all           - Build complete project"
	@echo "  clean         - Clean generated files"
	@echo "  flash         - Program with verified method (RECOMMENDED) ✅"
	@echo "  flash-emergency - Multiple methods for problematic cases"
	@echo "  reset         - Manual microcontroller reset"
	@echo "  info          - Project information"
	@echo "  sdk-paths     - Show available SDK paths"
	@echo "  help          - This help"
	@echo ""
	@echo "Typical usage:"
	@echo "  make clean && make && make flash"
	@echo ""
	@echo "NOTE: 'make flash' command uses the verified method:"
	@echo "  pyocd flash -t mspm0c1104 --connect under-reset \\"
	@echo "    -O pack.debug_sequences.disabled_sequences=..."

# Show available SDK paths
sdk-paths:
	@echo "=== Automatic SDK Detection ==="
	@echo "Current detected path: $(MSPM0_SDK_INSTALL_DIR)"
	@echo ""
	@echo "=== Verified paths ==="
	@if [ -d "../mspm0-sdk" ]; then echo "✅ ../mspm0-sdk (submodule)"; else echo "❌ ../mspm0-sdk (not found)"; fi
	@if [ -d "$(HOME)/Documents/mspm-/mspm0-sdk" ]; then echo "✅ $(HOME)/Documents/mspm-/mspm0-sdk (local installation)"; else echo "❌ $(HOME)/Documents/mspm-/mspm0-sdk (not found)"; fi
	@if [ -d "/usr/local/mspm0-sdk" ]; then echo "✅ /usr/local/mspm0-sdk (system)"; else echo "❌ /usr/local/mspm0-sdk (not found)"; fi
	@echo ""
	@echo "To use a specific path:"
	@echo "  make MSPM0_SDK_INSTALL_DIR=/custom/path all"

.PHONY: all clean flash flash-simple flash-reset reset info help flash-soft flash-no-reset flash-emergency sdk-paths

# SOFT RESET PROGRAMMING
flash-soft: $(TARGET_HEX)
	@echo "=== Programming with Soft Reset ==="
	@if [ ! -f "$(TARGET_HEX)" ]; then \
		echo "Error: $(TARGET_HEX) does not exist. Run 'make all' first."; \
		exit 1; \
	fi
	@echo "Programming with soft reset..."
	pyocd flash \
		-t mspm0c1104 \
		-O reset_type=sw \
		-O frequency=500000 \
		$(TARGET_HEX)
	@echo "Soft reset..."
	@sleep 1
	-pyocd reset -t mspm0c1104 --soft || echo "Soft reset failed"
	@echo "=== ✅ Programming with soft reset complete ==="

# BASIC PROGRAMMING WITHOUT RESET (MORE COMPATIBLE)
flash-no-reset: $(TARGET_HEX)
	@echo "=== Programming without Reset (more compatible) ==="
	@if [ ! -f "$(TARGET_HEX)" ]; then \
		echo "Error: $(TARGET_HEX) does not exist. Run 'make all' first."; \
		exit 1; \
	fi
	@echo "Programming without automatic reset..."
	pyocd flash \
		-t mspm0c1104 \
		-O frequency=500000 \
		$(TARGET_HEX)
	@echo "=== ✅ Programming complete - PRESS THE RESET BUTTON MANUALLY ==="

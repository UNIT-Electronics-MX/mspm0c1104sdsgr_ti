# MSPM0C1104 Development Template - Makefile wrapper
#
# Legacy Make entrypoint preserved for users who still prefer `make`.
# The actual build is driven by CMake + Ninja.

PROJECT_NAME := mspm0c1104_template
BUILD_DIR ?= build
BUILD_DIR_RELEASE ?= build-release
BUILD_DIR_WINDOWS ?= build-windows
TOOLCHAIN_FILE ?= cmake/arm-none-eabi-gcc.cmake
MSPM0_SDK_INSTALL_DIR ?=
DEBUG_PRESET := ninja-debug
RELEASE_PRESET := ninja-release
WINDOWS_PRESET := ninja-windows

CONFIGURE_CMD = cmake -S . -B $(BUILD_DIR) -G Ninja -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN_FILE)
CONFIGURE_RELEASE_CMD = cmake -S . -B $(BUILD_DIR_RELEASE) -G Ninja -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN_FILE) -DCMAKE_BUILD_TYPE=Release
CONFIGURE_WINDOWS_CMD = cmake -S . -B $(BUILD_DIR_WINDOWS) -G Ninja -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN_FILE) -DCMAKE_BUILD_TYPE=Debug

ifneq ($(strip $(MSPM0_SDK_INSTALL_DIR)),)
CONFIGURE_CMD += -DMSPM0_SDK_INSTALL_DIR=$(MSPM0_SDK_INSTALL_DIR)
CONFIGURE_RELEASE_CMD += -DMSPM0_SDK_INSTALL_DIR=$(MSPM0_SDK_INSTALL_DIR)
CONFIGURE_WINDOWS_CMD += -DMSPM0_SDK_INSTALL_DIR=$(MSPM0_SDK_INSTALL_DIR)
endif

TARGET_HEX = $(BUILD_DIR)/output/$(PROJECT_NAME).hex
PYOCD = pyocd

.PHONY: all configure configure-release configure-windows clean flash flash-simple flash-reset flash-soft flash-no-reset flash-emergency reset info help sdk-paths

all: configure
	cmake --build $(BUILD_DIR)

configure:
	@if [ -z "$(strip $(MSPM0_SDK_INSTALL_DIR))" ]; then \
		cmake --preset $(DEBUG_PRESET); \
	else \
		$(CONFIGURE_CMD); \
	fi

configure-release:
	@if [ -z "$(strip $(MSPM0_SDK_INSTALL_DIR))" ]; then \
		cmake --preset $(RELEASE_PRESET); \
	else \
		$(CONFIGURE_RELEASE_CMD); \
	fi

configure-windows:
	@if [ -z "$(strip $(MSPM0_SDK_INSTALL_DIR))" ]; then \
		cmake --preset $(WINDOWS_PRESET); \
	else \
		$(CONFIGURE_WINDOWS_CMD); \
	fi

clean:
	cmake -E rm -rf $(BUILD_DIR)
	cmake -E rm -rf $(BUILD_DIR_RELEASE)
	cmake -E rm -rf $(BUILD_DIR_WINDOWS)
	cmake -E rm -f *.obj *.out *.map *.hex

flash: all
	cmake --build $(BUILD_DIR) --target flash

flash-emergency: all
	$(PYOCD) flash -O frequency=100000 --erase=sector $(TARGET_HEX) || echo "Method 1 failed"
	-$(PYOCD) flash -t mspm0c1104 -O frequency=100000 --no-reset $(TARGET_HEX) || echo "Method 2 failed"
	$(PYOCD) flash -O frequency=100000 $(TARGET_HEX) || echo "Method 3 failed"
	-$(PYOCD) reset || echo "Reset failed - press reset button manually"

flash-simple: all
	$(PYOCD) flash -t mspm0c1104 $(TARGET_HEX)

flash-reset: all
	-$(PYOCD) reset -t mspm0c1104 || echo "Initial reset failed"
	$(PYOCD) flash -t mspm0c1104 --connect under-reset -O reset_type=hw $(TARGET_HEX)
	-$(PYOCD) reset -t mspm0c1104 || echo "Final reset failed"

flash-soft: all
	$(PYOCD) flash -t mspm0c1104 -O reset_type=sw -O frequency=500000 $(TARGET_HEX)
	-$(PYOCD) reset -t mspm0c1104 --soft || echo "Soft reset failed"

flash-no-reset: all
	$(PYOCD) flash -t mspm0c1104 --no-reset $(TARGET_HEX)

reset:
	cmake --build $(BUILD_DIR) --target reset

info: all
	@echo "Project: $(PROJECT_NAME)"
	@echo "Build directory: $(BUILD_DIR)"
	@echo "Firmware: $(TARGET_HEX)"
	@echo "SDK Path override: $(if $(strip $(MSPM0_SDK_INSTALL_DIR)),$(MSPM0_SDK_INSTALL_DIR),auto-detect)"

help:
	@echo "MSPM0C1104 Development Template - Makefile wrapper"
	@echo ""
	@echo "Main targets:"
	@echo "  make all              - Configure and build with CMake + Ninja"
	@echo "  make flash            - Build and program with the recommended pyOCD flow"
	@echo "  make configure        - Configure the debug build directory"
	@echo "  make configure-release - Configure the release build directory"
	@echo "  make configure-windows - Configure a Windows-oriented build directory"
	@echo "  make clean            - Remove CMake build directories"
	@echo "  make sdk-paths        - Show SDK override usage"
	@echo ""
	@echo "Typical usage:"
	@echo "  make clean && make all && make flash"
	@echo ""
	@echo "Optional SDK override:"
	@echo "  make MSPM0_SDK_INSTALL_DIR=/custom/path all"

sdk-paths:
	@echo "CMake SDK detection order:"
	@echo "  1. MSPM0_SDK_INSTALL_DIR make variable / CMake cache variable"
	@echo "  2. MSPM0_SDK_INSTALL_DIR environment variable"
	@echo "  3. ../mspm0-sdk"
	@echo "  4. $$HOME/Documents/mspm-/mspm0-sdk"
	@echo "  5. /usr/local/mspm0-sdk"
	@echo "  6. Windows paths under USERPROFILE and C:/ti/mspm0-sdk"

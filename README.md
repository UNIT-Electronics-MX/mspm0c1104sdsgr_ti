# MSPM0C1104 Development Template

A professional development template for Texas Instruments MSPM0C1104 microcontroller using the official TI MSPM0 SDK as a Git submodule.

## Project Overview

- **Microcontroller**: MSPM0C1104 (32KB Flash, 4KB RAM, Cortex-M0+)
- **SDK**: TI MSPM0 SDK v2.06.00.05 (official submodule)
- **Toolchain**: GNU Arm Embedded Toolchain
- **Programming**: pyOCD with verified flash sequences
- **Build System**: CMake + Ninja with automatic SDK path detection

## Quick Start

### Prerequisites

```bash
# Install ARM toolchain
sudo apt install gcc-arm-none-eabi

# Install CMake and Ninja
sudo apt install cmake ninja-build

# Install pyOCD
pip install pyocd
```

On Windows, install the GNU Arm Embedded Toolchain, CMake, Ninja, and pyOCD, then ensure `arm-none-eabi-gcc`, `cmake`, `ninja`, and `pyocd` are available in `PATH`.

Typical Windows installation options:

```powershell
winget install Kitware.CMake
winget install Ninja-build.Ninja
py -m pip install pyocd
```

### Clone and Build

Linux/macOS:

```bash
# Clone with submodules
git clone --recurse-submodules https://github.com/UNIT-Electronics-MX/mspm0c1104sdsgr_ti.git
cd mspm0c1104sdsgr_ti/mspm0c1104_template

# Configure and build
cmake -S . -B build -G "Ninja" -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi-gcc.cmake
cmake --build build

# Flash
cmake --build build --target flash
```

Windows PowerShell:

```powershell
git clone --recurse-submodules https://github.com/UNIT-Electronics-MX/mspm0c1104sdsgr_ti.git
cd mspm0c1104sdsgr_ti\mspm0c1104_template

cmake --preset ninja-windows
cmake --build --preset build-windows
cmake --build build-windows --target flash
```

If the SDK is not located in `../mspm0-sdk`, pass it explicitly during configure:

```bash
cmake -S . -B build -G "Ninja" \
  -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi-gcc.cmake \
  -DMSPM0_SDK_INSTALL_DIR=/custom/path/to/mspm0-sdk
```

Windows PowerShell with explicit SDK path:

```powershell
cmake -S . -B build-windows -G Ninja `
  -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi-gcc.cmake `
  -DMSPM0_SDK_INSTALL_DIR="C:/ti/mspm0-sdk"
```

## Project Structure

```
mspm0c1104sdsgr_ti/
├── mspm0c1104_template/       # Main project template
│   ├── src/                   # Source code directory
│   │   ├── main.c             # Synchronized GPIO blink example
│   │   └── README.md          # Source code documentation
│   ├── CMakeLists.txt        # Cross-platform build definition
│   ├── CMakePresets.json     # Ready-to-use Ninja presets
│   ├── cmake/                # GNU Arm toolchain files
│   ├── Makefile              # Legacy GNU Make workflow
│   ├── libs/                 # Organized library modules
│   │   ├── config/           # Hardware configuration
│   │   ├── i2c/              # I2C communication
│   │   ├── display/          # OLED display drivers
│   │   └── fonts/            # Graphics and fonts
├── mspm0-sdk/                # → Official TI SDK (submodule)
├── PYOCD.md                  # Complete pyOCD programming guide
└── README.md                 # This file
```

## Build System Features

### Automatic SDK Detection

The CMake configuration automatically detects the SDK location:
1. **`../mspm0-sdk`** (submodule - highest priority)
2. **`MSPM0_SDK_INSTALL_DIR`** (cache variable or environment variable)
3. **`$HOME/Documents/mspm-/mspm0-sdk`** (local installation)
4. **`/usr/local/mspm0-sdk`** (system installation)
5. **`%USERPROFILE%/Documents/mspm-/mspm0-sdk`** (Windows local installation)
6. **`%USERPROFILE%/Documents/Texas Instruments/mspm0-sdk`** (Windows Documents)
7. **`C:/ti/mspm0-sdk`** (Windows system installation)

### Recommended Commands

```bash
cmake --preset ninja-debug              # Configure debug build in ./build
cmake --build --preset build-debug      # Build .out and .hex
cmake --build build --target flash      # Program with pyOCD
cmake --build build --target reset      # Reset target with pyOCD
```

If you prefer to keep the old CLI habit, the Makefile is still available as a wrapper over CMake:

```bash
make all
make flash
```

On Windows, the matching preset is:

```bash
cmake --preset ninja-windows
cmake --build --preset build-windows
cmake --build build-windows --target flash
```

If you prefer the classic out-of-source flow, the equivalent is:

```bash
mkdir -p build
cd build
cmake -G "Ninja" .. -DCMAKE_TOOLCHAIN_FILE=../cmake/arm-none-eabi-gcc.cmake
cmake --build .
```

### Custom SDK Path

```bash
cmake -S . -B build -G "Ninja" \
  -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi-gcc.cmake \
  -DMSPM0_SDK_INSTALL_DIR=/custom/path/to/mspm0-sdk
```

## Hardware Configuration

### LED Example (PA0, PA24, PA27)
- **Pins**: PA0, PA24, PA27 (GPIO)
- **Function**: Synchronized GPIO blink demonstration
- **Period**: 500ms on / 500ms off
- **Note**: PA0 is open-drain and requires an external pull-up

### Programming Interface
- **Debugger**: Any CMSIS-DAP compatible (LaunchPad XDS110)
- **Method**: pyOCD with optimized flash sequences
- **Target**: `mspm0c1104`

## Usage Examples

### Basic GPIO Blink (`src/main.c`)
```c
#include "ti_msp_dl_config.h"

int main(void) {
    SYSCFG_DL_init();
    
    while (1) {
    DL_GPIO_setPins(GPIO_PA0_PORT, GPIO_PA0_PIN);
        DL_GPIO_setPins(GPIO_PA24_PORT, GPIO_PA24_PIN);
    DL_GPIO_setPins(GPIO_PA27_PORT, GPIO_PA27_PIN);
    DL_Common_delayCycles(CPUCLK_FREQ / 2);

    DL_GPIO_clearPins(GPIO_PA0_PORT, GPIO_PA0_PIN);
        DL_GPIO_clearPins(GPIO_PA24_PORT, GPIO_PA24_PIN);
    DL_GPIO_clearPins(GPIO_PA27_PORT, GPIO_PA27_PIN);
    DL_Common_delayCycles(CPUCLK_FREQ / 2);
    }
}
```

### Programming Commands
```bash
# Recommended method (verified working)
cmake --build build --target flash

# Reset target
cmake --build build --target reset

# Legacy Make wrapper
make flash
```

## Development Workflow

1. **Code**: Edit `src/main.c` or add modules in `src/` and `libs/`
2. **Configure**: `cmake --preset ninja-debug`
3. **Build**: `cmake --build --preset build-debug`
4. **Flash**: `cmake --build build --target flash`
5. **Debug**: Reset button or `cmake --build build --target reset`

### Adding New Source Files
1. Add `.c` files to `src/` directory
2. Update `PROJECT_SOURCES` in `mspm0c1104_template/CMakeLists.txt`
3. Add corresponding `.h` files if needed
4. Rebuild project

## pyOCD Configuration

The project includes optimized pyOCD settings for reliable programming:

```bash
pyocd flash -t mspm0c1104 --connect under-reset \
  -O pack.debug_sequences.disabled_sequences=ResetSystem,ResetHWReset,ResetCatchSet,ResetProcessor \
  firmware.hex
```

## License Compliance

This template uses the official TI MSPM0 SDK as a Git submodule, ensuring:
- **No SDK modifications** (full compliance)
- **Official licensing** (TI Commercial + BSD-3-Clause)
- **Easy updates** (`git submodule update`)
- **Professional structure**

## SDK Updates

```bash
# Update SDK to latest version
git submodule update --remote mspm0-sdk

# Commit SDK update
git add mspm0-sdk
git commit -m "Update MSPM0 SDK to latest version"
```

## Support

For issues related to:
- **MSPM0 SDK**: [TI Support](https://e2e.ti.com/)
- **pyOCD**: [pyOCD GitHub](https://github.com/pyocd/pyOCD)
- **This Template**: Create an issue in this repository

## License

This project uses multiple licenses:

- **Template Code**: [MIT License](LICENSE) - UNIT Electronics MX
- **TI MSPM0 SDK**: TI Commercial + BSD-3-Clause - Texas Instruments  
- **ARM CMSIS**: Apache-2.0 - ARM Limited

See [LICENSES.md](LICENSES.md) for complete license information and compliance guidelines.

### Quick License Summary
**Commercial use allowed**  
**Modification and distribution permitted**  
**Attribution required**  
**TI SDK only for TI MSPM0 devices**

## Version History

- **v1.0**: Initial template with dynamic SDK detection
- **v1.1**: Added pyOCD optimization and emergency programming
- **v1.2**: Professional structure with official SDK submodule
- **v1.3**: Added comprehensive licensing documentation
- **v1.4**: Added CMake + Ninja workflow for Linux and Windows

---

**Developed by**: [UNIT Electronics](https://github.com/UNIT-Electronics-MX)  
**SDK Version**: TI MSPM0 SDK 2.06.00.05  
**Template License**: MIT

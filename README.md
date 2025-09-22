# MSPM0C1104 Development Template

A professional development template for Texas Instruments MSPM0C1104 microcontroller using the official TI MSPM0 SDK as a Git submodule.

## Project Overview

- **Microcontroller**: MSPM0C1104 (32KB Flash, 4KB RAM, Cortex-M0+)
- **SDK**: TI MSPM0 SDK v2.06.00.05 (official submodule)
- **Toolchain**: GNU ARM Embedded Toolchain
- **Programming**: pyOCD with verified flash sequences
- **Build System**: Make with automatic SDK path detection

## Quick Start

### Prerequisites

```bash
# Install ARM toolchain
sudo apt install gcc-arm-none-eabi

# Install pyOCD
pip install pyocd
```

### Clone and Build

```bash
# Clone with submodules
git clone --recurse-submodules https://github.com/UNIT-Electronics-MX/mspm0c1104sdsgr_ti.git
cd mspm0c1104sdsgr_ti/mspm0c1104_template

# Build and flash
make clean && make && make flash
```

## Project Structure

```
mspm0c1104sdsgr_ti/
├── mspm0c1104_template/       # Main project template
│   ├── src/                   # Source code directory
│   │   ├── main.c             # LED blink example (PA24)
│   │   └── README.md          # Source code documentation
│   ├── Makefile              # Dynamic build system (English)
│   ├── Makefile_ES           # Spanish version (backup)
│   ├── libs/                 # Organized library modules
│   │   ├── config/           # Hardware configuration
│   │   ├── i2c/              # I2C communication
│   │   ├── display/          # OLED display drivers
│   │   └── fonts/            # Graphics and fonts
│   └── backup_makefiles/     # Build system documentation
├── mspm0-sdk/                # → Official TI SDK (submodule)
├── PYOCD.md                  # Complete pyOCD programming guide
└── README.md                 # This file
```

## Build System Features

### Automatic SDK Detection

The Makefile automatically detects the SDK location:
1. **`../mspm0-sdk`** (submodule - highest priority)
2. **`$HOME/Documents/mspm-/mspm0-sdk`** (local installation)
3. **`/usr/local/mspm0-sdk`** (system installation)

### Available Targets

```bash
make help           # Show all available targets
make all            # Build complete project
make clean          # Clean generated files
make flash          # Program with verified method (RECOMMENDED)
make flash-emergency # Multiple methods for problematic cases
make info           # Show project information
make sdk-paths      # Show available SDK paths
```

### Custom SDK Path

```bash
make MSPM0_SDK_INSTALL_DIR=/custom/path all
```

## Hardware Configuration

### LED Example (PA24)
- **Pin**: PA24 (GPIO)
- **Function**: LED blink demonstration
- **Period**: ~1 second toggle

### Programming Interface
- **Debugger**: Any CMSIS-DAP compatible (LaunchPad XDS110)
- **Method**: pyOCD with optimized flash sequences
- **Target**: `mspm0c1104`

## Usage Examples

### Basic LED Blink (`src/main.c`)
```c
#include "ti_msp_dl_config.h"

int main(void) {
    SYSCFG_DL_init();
    
    while (1) {
        DL_GPIO_setPins(GPIO_PA24_PORT, GPIO_PA24_PIN);
        DL_Common_delayCycles(CPUCLK_FREQ / 8);  // 500ms on
        DL_GPIO_clearPins(GPIO_PA24_PORT, GPIO_PA24_PIN);
        DL_Common_delayCycles(CPUCLK_FREQ / 8);  // 500ms off
    }
}
```

### Programming Commands
```bash
# Recommended method (verified working)
make flash

# Emergency programming (multiple methods)
make flash-emergency

# Simple programming (requires manual reset)
make flash-simple
```

## Development Workflow

1. **Code**: Edit `src/main.c` or add modules in `src/` and `libs/`
2. **Build**: `make clean && make`
3. **Flash**: `make flash`
4. **Debug**: Reset button or `make reset`

### Adding New Source Files
1. Add `.c` files to `src/` directory
2. Update `SOURCE_FILES` variable in Makefile
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

---

**Developed by**: [UNIT Electronics](https://github.com/UNIT-Electronics-MX)  
**SDK Version**: TI MSPM0 SDK 2.06.00.05  
**Template License**: MIT

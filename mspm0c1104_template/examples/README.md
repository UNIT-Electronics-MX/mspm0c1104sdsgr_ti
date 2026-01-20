# MSPM0C1104 Examples

Example collection for the MSPM0C1104SDSGR microcontroller.

## Common Structure

All examples follow the same directory structure:

```
example/
├── Makefile               # Build system
├── README.md              # Specific documentation
├── build/                 # Generated files (compilation)
│   ├── obj/              # Object files (.obj)
│   └── output/           # Final binaries (.out, .hex, .map)
├── libs/                  # Libraries and configuration
│   └── config/
│       ├── ti_msp_dl_config.h    # Hardware definitions
│       └── ti_msp_dl_config.c    # Peripheral initialization
├── src/                   # Application source code
│   └── main.c            # Main application
├── device_linker.lds      # Linker script
├── mspm0c1104.cfg         # Debug configuration
└── startup_mspm0c110x_gcc.c  # Startup code
```

## Available Examples

### GPIO/blink
Synchronized blinking of three GPIO pins (PA0, PA24, PA27).

**Features:**
- Demonstrates basic GPIO configuration
- Uses CPU cycle delays
- Open Drain pin handling (PA0)

**Location:** `GPIO/blink/`

### UART/echo
UART serial communication demonstration with interactive control.

**Features:**
- UART at 115200 baud (8N1)
- Shows welcome message
- Displays unique device UID
- Continuous UID transmission
- Interactive control (press 's' to pause/resume)

**Pins:**
- PA24 (PINCM25): UART0_RX
- PA27 (PINCM28): UART0_TX

**Location:** `UART/echo/`

## Building

Each example builds independently:

```bash
cd examples/[CATEGORY]/[EXAMPLE]/
make
```

## Flashing

```bash
make flash
```

## Cleaning

```bash
make clean
```

## Requirements

- **Toolchain**: arm-none-eabi-gcc 13.2.1 or higher
- **Flasher**: pyOCD 0.37.0 or higher
- **SDK**: MSPM0 SDK 2.09.00.01 (included as submodule)

## Adding New Examples

1. Create directory following the structure: `[CATEGORY]/[NAME]/`
2. Copy base files from an existing example
3. Modify `src/main.c` with your application
4. Update `libs/config/` according to required peripherals
5. Document in `README.md`
6. Add entry to this file

## Notes

- All Makefiles automatically search for SDK at `../../../../mspm0-sdk/`
- Examples share the same linker script and startup code
- Hardware configuration is modularized in `libs/config/`

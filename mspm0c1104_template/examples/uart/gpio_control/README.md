# UART Demo Example

UART communication example for MSPM0C1104 with interactive control and device identification.

## Hardware Configuration

### UART Pins
- **PA24 (PINCM25)**: UART0_RX - Reception
- **PA27 (PINCM28)**: UART0_TX - Transmission

### Communication Parameters
- **Baud Rate**: 115200
- **Format**: 8N1 (8 data bits, no parity, 1 stop bit)
- **Flow Control**: None

## Connection

To test this example, you need a USB-to-Serial adapter (FTDI, CP2102, etc.):

```
USB-Serial       MSPM0C1104
---------        ----------
    TX    ---->  PA24 (RX)
    RX    <----  PA27 (TX)
   GND    ----   GND
```

**IMPORTANT**: Verify the serial adapter voltage levels. The MSPM0C1104 operates at 3.3V.

## Operation

The program:
1. Displays welcome message "WELCOME TO TEXAS INSTRUMENTS"
2. Shows the microcontroller's unique UID
3. Initializes UART0 at 115200 baud
4. Sends the UID continuously every 500ms
5. Press 's' to pause/resume the loop

## Building and Flashing

```bash
cd examples/uart/gpio_control

# Build
make clean && make

# Flash
make flash

# Clean
make clean
```

This example currently uses its local `Makefile`. The top-level `mspm0c1104_template` project uses CMake + Ninja.

## Testing

Using a serial terminal (minicom, screen, PuTTY, etc.):

```bash
# Linux/Mac with minicom
minicom -D /dev/ttyUSB0 -b 115200

# Linux/Mac with screen
screen /dev/ttyUSB0 115200

# Or use the provided test script
./test_uart.sh /dev/ttyUSB0
```

## Project Structure

```
echo/
gpio_control/
├── Makefile
├── README.md
├── build/
├── libs/
│   └── config/
│       ├── ti_msp_dl_config.h
│       └── ti_msp_dl_config.c
├── src/
│   └── main.c
├── device_linker.lds
├── mspm0c1104.cfg
├── startup_mspm0c110x_gcc.c
└── test_uart.sh
```

## Notes

- The example uses polling (no interrupts) for simplicity
- PA24 and PA27 are dedicated pins for UART0 on MSPM0C1104
- Baud rate calculated for 24MHz BUSCLK
- Device UID is read from Factory Region (0x41C40000)

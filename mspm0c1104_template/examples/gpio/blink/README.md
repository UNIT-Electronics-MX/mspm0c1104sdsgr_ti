# GPIO Blink Example

This example demonstrates synchronized blinking of three GPIO pins.

## Description

The program simultaneously blinks pins PA0, PA24, and PA27 with a 500ms on and 500ms off interval.

## Hardware

- **PA0**: GPIO output (Open Drain - requires external pull-up)
- **PA24**: GPIO output
- **PA27**: GPIO output

## Important Note

PA0 is an Open Drain pin and requires an external pull-up resistor (10kΩ recommended) connected to 3.3V to function correctly as an output.

## Building

```bash
cd examples/GPIO/blink
make clean && make
```

## Flashing

```bash
make flash
```

## Operation

1. All three pins turn on simultaneously
2. Wait 500ms
3. All three pins turn off simultaneously
4. Wait 500ms
5. Repeat cycle

## Project Structure

```
blink/
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
└── startup_mspm0c110x_gcc.c
```

## Notes

- The example uses CPU cycle delays (polling)
- PA0 requires external pull-up due to its Open Drain nature
- All examples share the same directory structure

# pyOCD Programming Guide for MSPM0C1104

This document provides comprehensive information about programming the MSPM0C1104 microcontroller using pyOCD.

## Overview

pyOCD is a Python-based debugger and programmer for ARM microcontrollers. This guide focuses on the specific configuration needed for reliable MSPM0C1104 programming.

## Installation

### Ubuntu/Debian
```bash
# Install pyOCD
pip install pyocd

# Verify installation
pyocd -V
```

### Alternative installation methods
```bash
# System-wide installation
sudo pip3 install pyocd

# User installation
pip3 install --user pyocd

# Development version
pip install git+https://github.com/pyocd/pyOCD.git
```

## Configuration

### Recommended pyOCD Command (Verified Working)
```bash
pyocd flash -t mspm0c1104 --connect under-reset \
  -O pack.debug_sequences.disabled_sequences=ResetSystem,ResetHWReset,ResetCatchSet,ResetProcessor \
  firmware.hex
```

### Key Parameters Explained

| Parameter | Description | Purpose |
|-----------|-------------|---------|
| `-t mspm0c1104` | Specify target device | Ensures correct device configuration |
| `--connect under-reset` | Connect while holding reset | Prevents code execution during programming |
| `-O pack.debug_sequences.disabled_sequences=...` | Disable problematic reset sequences | Avoids conflicts with MSPM0C1104 reset behavior |

### Disabled Sequences Details
- **ResetSystem**: System-level reset sequence
- **ResetHWReset**: Hardware reset sequence  
- **ResetCatchSet**: Reset catch configuration
- **ResetProcessor**: Processor reset sequence

## Common Issues and Solutions

### Issue 1: "Could not halt device"
**Symptoms**: Programming fails with halt errors
**Solution**: Use the `--connect under-reset` option
```bash
pyocd flash -t mspm0c1104 --connect under-reset firmware.hex
```

### Issue 2: "Flash programming failed"
**Symptoms**: Programming appears to succeed but doesn't work
**Solution**: Use the complete verified command with disabled sequences
```bash
pyocd flash -t mspm0c1104 --connect under-reset \
  -O pack.debug_sequences.disabled_sequences=ResetSystem,ResetHWReset,ResetCatchSet,ResetProcessor \
  firmware.hex
```

### Issue 3: "Target not responding"
**Symptoms**: pyOCD cannot connect to the device
**Solutions**:
1. Check physical connections
2. Try different frequency: `-O frequency=100000`
3. Manual reset before programming
4. Use emergency programming methods

### Issue 4: Code doesn't run after programming
**Symptoms**: Programming succeeds but LED doesn't blink
**Solutions**:
1. Manual reset: Press reset button on LaunchPad
2. Software reset: `pyocd reset -t mspm0c1104`
3. Power cycle the board

## Programming Methods

### Method 1: Recommended (CMake integration)
```bash
cmake --build build --target flash
```

### Method 2: Direct command
```bash
pyocd flash -t mspm0c1104 --connect under-reset \
  -O pack.debug_sequences.disabled_sequences=ResetSystem,ResetHWReset,ResetCatchSet,ResetProcessor \
  build/output/firmware.hex
```

### Method 3: Emergency (multiple attempts)
```bash
# Try basic programming
pyocd flash -O frequency=100000 --erase=sector firmware.hex

# If that fails, try specific target
pyocd flash -t mspm0c1104 -O frequency=100000 --no-reset firmware.hex

# Final attempt with auto-detection
pyocd flash -O frequency=100000 firmware.hex
```

### Method 4: Simple (requires manual reset)
```bash
pyocd flash -t mspm0c1104 firmware.hex
# Press reset button manually after programming
```

## Programming Options

### Frequency Settings
```bash
# Default frequency (usually works)
pyocd flash -t mspm0c1104 firmware.hex

# Slower frequency (more reliable)
pyocd flash -t mspm0c1104 -O frequency=100000 firmware.hex

# Faster frequency (if supported)
pyocd flash -t mspm0c1104 -O frequency=1000000 firmware.hex
```

### Erase Options
```bash
# Sector erase (recommended)
pyocd flash --erase=sector firmware.hex

# Chip erase (slower but thorough)
pyocd flash --erase=chip firmware.hex

# Auto erase (default)
pyocd flash firmware.hex
```

### Reset Options
```bash
# Hardware reset
pyocd flash -O reset_type=hw firmware.hex

# Software reset
pyocd flash -O reset_type=sw firmware.hex

# No automatic reset
pyocd flash --no-reset firmware.hex
```

## Debugging Commands

### List connected devices
```bash
pyocd list
```

### Get device information
```bash
pyocd info -t mspm0c1104
```

### Manual reset
```bash
# Soft reset
pyocd reset -t mspm0c1104 --soft

# Normal reset
pyocd reset -t mspm0c1104

# Hardware reset
pyocd reset -t mspm0c1104 --hard
```

### Interactive debugging
```bash
# Start GDB server
pyocd gdbserver -t mspm0c1104

# Commander mode (interactive)
pyocd commander -t mspm0c1104
```

## Configuration Files

### Project-specific configuration (.pyocd.yaml)
```yaml
target_override: mspm0c1104
frequency: 1000000
connect_mode: under_reset
pack:
  debug_sequences:
    disabled_sequences:
      - ResetSystem
      - ResetHWReset
      - ResetCatchSet
      - ResetProcessor
```

### Global configuration (~/.pyocd.yaml)
```yaml
# Global pyOCD configuration
defaults:
  frequency: 1000000
  connect_mode: attach
  
targets:
  mspm0c1104:
    frequency: 500000
    connect_mode: under_reset
```

## Version Compatibility

### Tested Versions
- **pyOCD 0.37.0**: Fully compatible
- **pyOCD 0.36.x**: Compatible  
- **pyOCD 0.35.x**: May need adjustments
- **pyOCD < 0.35**: Not recommended

### Check your version
```bash
pyocd -V
```

### Update pyOCD
```bash
pip install --upgrade pyocd
```

## Hardware Setup

### Required Hardware
- **LaunchPad**: LP-MSPM0C1104 or compatible
- **Target**: MSPM0C1104 device
- **Debugger**: XDS110 (integrated in LaunchPad)
- **USB Cable**: USB-A to Micro-USB

### Connection Verification
```bash
# List available debuggers
pyocd list

# Expected output:
#   0 => XDS110 [serial_number] (LP-MSPM0C1104)
```

## 📞 Troubleshooting

### If programming consistently fails:
1. **Check connections**: Ensure USB cable is good
2. **Try different USB port**: Some ports provide better power
3. **Update drivers**: Install latest XDS110 drivers
4. **Try different computer**: Rule out system-specific issues
5. **Check board power**: Ensure LaunchPad powers on correctly

### Emergency recovery:
1. **Hold reset while connecting**: Physical button press
2. **Try mass erase**: `pyocd erase -t mspm0c1104 --mass`
3. **Power cycle**: Disconnect and reconnect USB
4. **Factory reset**: Use TI tools if available

## References

- [pyOCD Documentation](https://pyocd.io/)
- [TI MSPM0C1104 Datasheet](https://www.ti.com/product/MSPM0C1104)
- [LaunchPad User Guide](https://www.ti.com/tool/LP-MSPM0C1104)
- [MSPM0 SDK Documentation](https://www.ti.com/tool/MSPM0-SDK)

---

**Last updated**: September 2025  
**Template version**: v1.2  
**pyOCD version tested**: 0.37.0

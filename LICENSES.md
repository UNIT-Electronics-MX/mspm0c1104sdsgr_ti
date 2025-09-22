# License Information

This project uses multiple components with different licenses. This document provides a comprehensive overview of all licensing requirements.

## 📋 License Summary

| Component | License | Copyright | Usage |
|-----------|---------|-----------|-------|
| **Template Code** | MIT | UNIT Electronics MX (2025) | Template files, build system, documentation |
| **MSPM0 SDK** | TI Commercial + BSD-3-Clause | Texas Instruments (2024) | Official SDK libraries and drivers |
| **CMSIS** | Apache-2.0 | ARM Limited | ARM CMSIS headers and libraries |
| **GNU Toolchain** | GPL-3.0 + Runtime Exception | Free Software Foundation | Compiler and linker (not affecting output) |

## 🏷️ Template License (MIT)

### Scope
The following files and directories are licensed under MIT:

```
├── LICENSE                    # MIT License text
├── README.md                  # Project documentation
├── PYOCD.md                  # pyOCD programming guide
├── LICENSES.md               # This file
├── mspm0c1104_template/      # Template code
│   ├── src/                  # Application source code
│   ├── Makefile             # Build system
│   ├── Makefile_ES          # Spanish build system
│   ├── libs/                # Custom libraries (if any)
│   │   ├── config/          # Configuration files (generated from TI tools)
│   │   ├── i2c/             # I2C wrapper libraries (if custom)
│   │   ├── display/         # Display libraries (if custom)
│   │   └── fonts/           # Font libraries (if custom)
│   └── *.cfg                # Configuration files
```

### MIT License Text
```
MIT License

Copyright (c) 2025 UNIT Electronics MX

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

## 🏢 TI MSPM0 SDK License

### Scope
The MSPM0 SDK is included as a Git submodule and maintains its original licensing:

```
mspm0-sdk/                    # Git submodule - original licensing applies
├── source/                   # TI Commercial License + BSD-3-Clause
├── examples/                 # TI Commercial License
├── docs/                     # TI Commercial License
└── tools/                    # Various (see individual tool licenses)
```

### License Summary
The TI MSPM0 SDK uses a **dual licensing** approach:

1. **TI Commercial License**: 
   - Applies to most SDK components
   - Allows commercial use with TI devices
   - See: `mspm0-sdk/license_mspm0_sdk_*.txt`

2. **BSD-3-Clause License**:
   - Applies to some components
   - Standard BSD terms for certain libraries

### Key Points
- ✅ **Commercial use allowed** with TI MSPM0 devices
- ✅ **No royalties** for TI device usage
- ✅ **Distribution permitted** when used with TI hardware
- ⚠️ **Device restriction**: License tied to TI MSPM0 family usage

### Compliance Requirements
1. **Include license files**: Keep original SDK license files
2. **Attribution**: Include TI copyright notices in distributions
3. **Device binding**: Only use with TI MSPM0 devices
4. **Unmodified SDK**: Use official SDK via Git submodule (recommended)

## 🔧 ARM CMSIS License (Apache-2.0)

### Scope
ARM CMSIS components within the SDK:

```
mspm0-sdk/source/third_party/CMSIS/
├── Core/                     # Apache-2.0
├── DSP/                      # Apache-2.0
└── Include/                  # Apache-2.0
```

### Key Points
- ✅ **Commercial use allowed**
- ✅ **Modification allowed**
- ✅ **Distribution allowed**
- ⚠️ **Attribution required**

## 🛠️ GNU Toolchain License

### Components
- **arm-none-eabi-gcc**: GPL-3.0 with Runtime Library Exception
- **arm-none-eabi-ld**: GPL-3.0 with Runtime Library Exception
- **Newlib C library**: Various licenses (mostly BSD-like)

### Key Points
- ✅ **Output binaries are NOT affected** by GPL
- ✅ **Commercial use of generated binaries allowed**
- ✅ **No source code disclosure required** for your application
- ✅ **Runtime Library Exception** protects your application

## 📄 Third-Party Components

### pyOCD
- **License**: Apache-2.0
- **Usage**: Programming tool (not linked into final binary)
- **Impact**: None on final product licensing

### Other Tools
- **Make**: GPL-3.0 (build tool only)
- **Git**: GPL-2.0 (version control only)

## 🚀 Distribution Guidelines

### For Template Users

#### If you're using this template:
1. **Keep the original LICENSE file** for template attribution
2. **Add your own copyright** to modified files
3. **Follow TI SDK licensing** for SDK components
4. **Include required attributions** in your documentation

#### Example file header:
```c
/*
 * Copyright (c) 2025 Your Company Name
 * 
 * Based on MSPM0C1104 Template by UNIT Electronics MX
 * Licensed under MIT License
 *
 * This file uses TI MSPM0 SDK components
 * Copyright (c) 2024 Texas Instruments Incorporated
 */
```

### For Commercial Products

#### ✅ You CAN:
- Use this template in commercial products
- Modify and extend the template code
- Distribute products based on this template
- Keep your modifications proprietary (for template parts)

#### ⚠️ You MUST:
- Include required copyright notices
- Follow TI SDK licensing terms
- Use only with TI MSPM0 devices (SDK requirement)
- Include license attributions in documentation

#### ❌ You CANNOT:
- Remove original copyright notices
- Use TI SDK with non-TI devices
- Claim ownership of TI or ARM IP

## 📞 License Compliance Checklist

### Before Distribution:
- [ ] Include `LICENSE` file (MIT for template)
- [ ] Include TI SDK license files
- [ ] Add copyright notices to modified files
- [ ] Update documentation with attributions
- [ ] Verify target device is TI MSPM0 family
- [ ] Include third-party attributions

### Documentation Requirements:
- [ ] List all third-party components
- [ ] Include license texts or references
- [ ] Provide source code availability (if required)
- [ ] Document any modifications to SDK components

## 🔗 License References

### Primary Licenses
- **MIT License**: [https://opensource.org/licenses/MIT](https://opensource.org/licenses/MIT)
- **TI License**: See `mspm0-sdk/license_mspm0_sdk_*.txt`
- **Apache-2.0**: [https://www.apache.org/licenses/LICENSE-2.0](https://www.apache.org/licenses/LICENSE-2.0)
- **BSD-3-Clause**: [https://opensource.org/licenses/BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause)

### Additional Resources
- **TI Licensing**: [https://www.ti.com/legal/termsofsale.html](https://www.ti.com/legal/termsofsale.html)
- **SPDX License List**: [https://spdx.org/licenses/](https://spdx.org/licenses/)
- **OSI Approved Licenses**: [https://opensource.org/licenses](https://opensource.org/licenses)

---

**Last updated**: September 2025  
**Template version**: v1.3  
**Reviewed by**: UNIT Electronics MX Legal

> **Disclaimer**: This document provides licensing information for reference. 
> Always consult with legal counsel for specific licensing questions and compliance requirements.

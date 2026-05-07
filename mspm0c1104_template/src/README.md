# Source Code Directory

This directory contains the main application source code for the MSPM0C1104 template.

## Files

- **`main.c`**: Main application file with synchronized GPIO blink on PA0, PA24, and PA27

## Adding New Source Files

To add new source files to the project:

1. **Add your `.c` files to this directory**
2. **Update `CMakeLists.txt`** - Add new files to `PROJECT_SOURCES`:
   ```cmake
   set(PROJECT_SOURCES
       src/main.c
       src/your_new_file.c
       libs/config/ti_msp_dl_config.c
       startup_mspm0c110x_gcc.c
   )
   ```
3. **Add corresponding `.h` files** if needed
4. **Rebuild**: `cmake --build build`

## Example Structure

```
src/
├── main.c              # Main application
├── sensors.c           # Sensor handling
├── communication.c     # Communication protocols
├── utils.c             # Utility functions
└── README.md          # This file
```

## Include Path

The `src/` directory is automatically included in the compiler include path via `target_include_directories()` in `CMakeLists.txt`.

## Best Practices

- Keep hardware-specific configuration in `libs/config/`
- Use descriptive filenames
- Add function prototypes to corresponding `.h` files
- Follow consistent coding style

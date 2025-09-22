# Organización de Bibliotecas - PA24 Example

## Estructura del Proyecto

```
pa24_example/
├── build/                        # Directorio de construcción ✅ ACTIVO
│   ├── obj/                      # Archivos objeto (.obj)
│   │   ├── main_minimal_test.obj
│   │   ├── ti_msp_dl_config.obj
│   │   ├── i2c_mspm0.obj
│   │   └── startup_mspm0c110x_gcc.obj
│   └── output/                   # Archivos finales
│       ├── pa24_example.out      # Ejecutable
│       ├── pa24_example.hex      # Para programar
│       └── pa24_example.map      # Mapa de memoria
├── src/                          # Código fuente principal (vacío por ahora)
├── libs/                         # Bibliotecas organizadas
│   ├── config/                   # Configuraciones del microcontrolador
│   │   ├── ti_msp_dl_config.c
│   │   ├── ti_msp_dl_config.h
│   │   ├── ti_msp_dl_config_simple.c
│   │   └── ti_msp_dl_config_simple.h
│   ├── i2c/                      # Driver de comunicación I2C
│   │   ├── i2c_mspm0.c
│   │   └── i2c_mspm0.h
│   ├── display/                  # Bibliotecas para pantalla OLED SSD1306
│   │   ├── ssd1306.c
│   │   ├── ssd1306.h
│   │   ├── ssd1306_simple.c
│   │   └── ssd1306_simple.h
│   └── fonts/                    # Recursos de fuentes/tipografías
│       ├── ascii_fonts.c
│       └── ascii_fonts.h
├── main*.c                       # Archivos de ejemplo principales
├── Makefile                      # Makefile configurado para bibliotecas
├── .gitignore                    # Ignora archivos de construcción
└── *.lds, *.c, *.h             # Otros archivos del proyecto
```

## Configuraciones Disponibles

El Makefile ha sido configurado para manejar diferentes configuraciones según tus necesidades:

### 1. Configuración Básica (solo LED)
- **Comando:** `make basic && make flash`
- **Archivos:** `main_test_led.c`
- **Bibliotecas:** Solo configuración básica
- **Tamaño:** ~5KB (cabe perfectamente en MSPM0C1104)
- **Uso:** Pruebas básicas de LED sin I2C

### 2. Configuración I2C
- **Comando:** `make i2c && make flash`
- **Archivos:** `main_minimal_test.c`
- **Bibliotecas:** Configuración básica + I2C
- **Tamaño:** ~5KB (cabe perfectamente)
- **Uso:** Proyecto con comunicación I2C (configuración actual)

### 3. Configuración OLED Completa
- **Comando:** `make oled && make flash`
- **Archivos:** `main.c`
- **Bibliotecas:** Todas las bibliotecas incluidas
- **Tamaño:** ~45KB (NO cabe en MSPM0C1104 de 32KB)
- **Uso:** Solo para microcontroladores con más memoria

## Uso del Sistema

### Comandos Principales
```bash
# Ver ayuda completa
make help

# Limpiar proyecto
make clean

# Compilar configuración específica
make basic    # Solo LED
make i2c      # Con I2C (actual)
make oled     # Con OLED (no cabe)

# Programar el microcontrolador
make flash

# Compilación normal (usando configuración actual)
make clean && make && make flash
```

### Cambiar Configuración Manualmente

Si quieres personalizar qué bibliotecas incluir, edita el `Makefile` en las líneas:

```makefile
# Para uso básico solo con LED:
#LIB_SOURCES = $(BASIC_LIBS)

# Para uso con I2C pero sin pantalla:
LIB_SOURCES = $(BASIC_LIBS) $(I2C_LIBS)

# Para uso completo con pantalla OLED (necesita más memoria):
#LIB_SOURCES = $(BASIC_LIBS) $(I2C_LIBS) $(DISPLAY_LIBS) $(FONT_LIBS)
```

## Rutas de Inclusión

Las rutas de inclusión están configuradas automáticamente en el Makefile:
- `-Ilibs/config`
- `-Ilibs/i2c`
- `-Ilibs/display`
- `-Ilibs/fonts`

Por lo tanto, puedes incluir archivos directamente:
```c
#include "ti_msp_dl_config.h"    // desde libs/config/
#include "i2c_mspm0.h"           // desde libs/i2c/
#include "ssd1306.h"             // desde libs/display/
#include "ascii_fonts.h"         // desde libs/fonts/
```

## Migración de Proyectos Existentes

Si tienes código existente que usa las bibliotecas, no necesitas cambiar las inclusiones. Solo:

1. Asegúrate de que tu archivo main esté en la raíz del proyecto
2. Compila con la configuración apropiada
3. Las rutas se resuelven automáticamente

## Limitaciones de Memoria

- **MSPM0C1104**: 32KB Flash, 4KB RAM
- **Configuración básica**: ~5KB ✅
- **Configuración I2C**: ~5KB ✅
- **Configuración OLED completa**: ~45KB ❌

Para proyectos con OLED, considera usar un microcontrolador con más memoria como MSPM0G series.

## Beneficios de esta Organización

1. **Modularidad**: Cada biblioteca en su propio directorio
2. **Escalabilidad**: Fácil agregar nuevas bibliotecas
3. **Flexibilidad**: Compilar solo lo que necesitas
4. **Mantenibilidad**: Código organizado y fácil de mantener
5. **Reutilización**: Bibliotecas reutilizables en otros proyectos
6. **Organización de salidas**: Archivos de construcción separados del código fuente

## Directorio de Construcción

El sistema usa un directorio `build/` para organizar todas las salidas de compilación:

```
build/
├── obj/                          # Archivos objeto (.obj)
│   ├── main_minimal_test.obj
│   ├── startup_mspm0c110x_gcc.obj
│   ├── ti_msp_dl_config.obj
│   └── i2c_mspm0.obj
└── output/                       # Archivos finales
    ├── pa24_example.out          # Ejecutable linkado
    ├── pa24_example.hex          # Archivo HEX para programar
    └── pa24_example.map          # Mapa de memoria
```

## Directorio Build ✅

El directorio `build/` mantiene el directorio principal limpio y organiza todas las salidas de compilación:

### Ventajas:
- **Directorio principal limpio**: Solo código fuente en la raíz
- **Organización clara**: Objetos separados de archivos finales  
- **Fácil limpieza**: `make clean` elimina todo build/
- **Control de versiones**: .gitignore excluye archivos generados
- **Construcción incremental**: Solo recompila archivos modificados

### Estructura de build/:
```
build/obj/         # Archivos objeto intermedios (.obj)
build/output/      # Archivos finales (.out, .hex, .map)
```

### Comandos útiles:
```bash
# Ver contenido del directorio build
find build/ -type f

# Ver tamaño de los archivos generados  
ls -lh build/output/

# Limpiar completamente
make clean
```

### Ventajas del directorio build:
- **Limpieza**: El directorio raíz permanece limpio
- **Organización**: Separación clara entre fuente y salidas
- **Fácil limpieza**: `make clean` elimina todo el directorio build
- **Control de versiones**: `.gitignore` excluye automáticamente los archivos generados

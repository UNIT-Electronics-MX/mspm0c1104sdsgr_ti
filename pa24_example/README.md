# Ejemplo PA24 para MSPM0C1104SDSGR

Este proyecto demuestra cómo controlar el **PA24** en el microcontrolador MSPM0C1104SDSGR usando Texas Instruments DriverLib.

## Hardware
- **Microcontrolador**: MSPM0C1104SDSGR DevLab
- **Pin**: PA24 (Pin físico en conector J1_5)
- **Función**: Salida digital con blink cada 200ms
- **Debugger**: CMSIS-DAP integrado

## Características
- ✅ Proyecto limpio específico para PA24
- ✅ Configuración optimizada para MSPM0C1104
- ✅ Programación con pyOCD (funciona perfectamente)
- ✅ Sistema de build simple con Makefile
- ✅ Debug support integrado

## Estructura del proyecto
```
pa24_example/
├── main.c                    # Código principal - control PA24
├── ti_msp_dl_config.h        # Configuración del sistema
├── ti_msp_dl_config.c        # Implementación de configuración
├── startup_mspm0c110x_gcc.c  # Startup code para MSPM0C1104
├── device_linker.lds         # Linker script
├── Makefile                  # Sistema de build
└── README.md                 # Esta documentación
```

## Uso rápido

### Compilar
```bash
make clean && make
```

### Programar
```bash
# Programar tu MSPM0C1104SDSGR
make flash
```

### Programación manual con pyOCD
```bash
pyocd flash -t mspm0c1104 --connect under-reset \
  -O pack.debug_sequences.disabled_sequences=ResetSystem,ResetHWReset,ResetCatchSet,ResetProcessor \
  pa24_example.hex
```

## Funcionamiento
El programa:
1. Inicializa el sistema MSPM0C1104
2. Configura PA24 como salida digital
3. En bucle infinito:
   - Pone PA24 en HIGH (100ms)
   - Pone PA24 en LOW (100ms)

## Conexiones
Para ver el funcionamiento:
- **LED**: Conecta un LED + resistencia (330Ω) entre PA24 y GND
- **Osciloscopio**: Conecta probe a PA24 para ver la señal
- **Multímetro**: Medir voltaje en PA24 (0V/3.3V alternante)

## Ubicación física
- **PA24**: Conector J1, Pin 5 en LP_MSPM0C1104
- **Voltaje**: 3.3V cuando está HIGH

## Comandos disponibles
```bash
make help    # Ver todos los comandos
make info    # Información del proyecto
make clean   # Limpiar archivos compilados
make flash   # Programar con pyOCD
```

## Especificaciones técnicas
- **Pin físico**: PA24 (J1_5)
- **Voltaje salida**: 0V - 3.3V
- **Corriente máxima**: 8mA por pin
- **Frecuencia blink**: 5 Hz (100ms ON, 100ms OFF)
- **Tamaño código**: ~4KB
- **RAM usada**: ~508 bytes

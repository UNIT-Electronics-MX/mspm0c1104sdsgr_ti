# Respaldos de Makefiles - PA24 Example

Este directorio contiene los respaldos de diferentes versiones de Makefiles que fueron usados durante el desarrollo.

## Archivos:

- **Makefile.backup** - Versión de respaldo con múltiples targets de programación
- **Makefile.backup.full** - Versión completa con todas las opciones
- **Makefile_clean** - Intento de versión limpia
- **Makefile.original** - Primera versión funcional
- **rules.mk** - Archivo de reglas separado (experimento de organización)

## Estado actual:

El **Makefile principal** (en el directorio padre) es la versión final, limpia y funcional que:
- Compila correctamente el proyecto
- Programa el MSPM0C1104 con el método verificado
- No tiene redundancias ni advertencias
- Incluye todos los targets necesarios

## Método de programación actual:

```bash
pyocd flash -t mspm0c1104 --connect under-reset \
  -O pack.debug_sequences.disabled_sequences=ResetSystem,ResetHWReset,ResetCatchSet,ResetProcessor \
  build/output/pa24_example.hex
```

## Uso típico:

```bash
make clean && make && make flash
```

Los archivos en este directorio se mantienen por si se necesita revertir algún cambio, pero no son necesarios para el funcionamiento del proyecto.

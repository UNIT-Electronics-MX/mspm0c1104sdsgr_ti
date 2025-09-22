# Solución para Error de pyOCD: "AP at address #4 is not a MEM-AP"

## Problema

Al intentar programar el MSPM0C1104 con pyOCD, aparecía el siguiente error:
```
Error while running debug sequence 'ResetSystem' (core Cortex-M0+): AP at address #4 is not a MEM-AP
```

## Causa

El error se debe a que pyOCD tiene problemas cuando se especifica el target específico `-t mspm0c1104` junto con opciones de reset por hardware. El MSPM0C1104 tiene una configuración particular del Access Port (AP) que no es compatible con todas las opciones de pyOCD.

## Solución ✅

**Usar autodetección en lugar de target específico:**

```bash
# ❌ Problmático
pyocd flash -t mspm0c1104 --connect under-reset -O reset_type=hw archivo.hex

# ✅ Funciona
pyocd flash -O frequency=500000 archivo.hex
```

## Configuración del Makefile

El Makefile ahora usa la configuración que funciona:

- **Target principal**: `make flash` - Usa autodetección ✅
- **Target de emergencia**: `make flash-emergency` - Prueba múltiples opciones
- **Reset manual**: `make reset` - Reset sin especificar target

## Explicación Técnica

1. **Autodetección**: pyOCD detecta automáticamente el tipo de chip
2. **Frecuencia reducida**: 500KHz es más estable que 1MHz
3. **Sin reset complejo**: Evita configuraciones avanzadas de reset que causan conflictos
4. **Warning normal**: El warning sobre "Generic 'cortex_m' target type" es informativo, no un error

## Comandos de Trabajo

```bash
# Compilar y programar (método recomendado)
make clean && make && make flash

# Solo programar
make flash

# Reset manual si es necesario
make reset

# Si hay problemas, probar múltiples opciones
make flash-emergency
```

## Verificación

Si el comando funciona correctamente, verás:
```
[==================================================] 100%
Erased 0 bytes (0 sectors), programmed 4700 bytes (0 pages), skipped 0 bytes (0 pages) at 11.84 kB/s
```

El PA24 debería empezar a parpadear inmediatamente después de la programación.

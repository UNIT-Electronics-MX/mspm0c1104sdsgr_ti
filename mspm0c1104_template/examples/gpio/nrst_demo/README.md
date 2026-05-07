# NRST Demo - MSPM0C1104

Demostración de cómo funciona el pin NRST (PA1) en el MSPM0C1104.

## ⚠️ INFORMACIÓN CRÍTICA SOBRE NRST

### Estado por Defecto
- **PA1 tiene función de NRST habilitada por defecto** (reset externo)
- Si usas PA1 como GPIO/I2C sin deshabilitar NRST, el chip se reiniciará

### Cómo Deshabilitar NRST
```c
DL_SYSCTL_disableNRSTPin();  // Ejecutar UNA VEZ en el init
```

### Cómo Re-habilitar NRST
**NO SE PUEDE desde código.** Solo hay una forma:

1. **Quitar completamente la alimentación** (desconectar VCC)
2. Esperar unos segundos  
3. Volver a conectar la alimentación
4. Esto genera un **POR (Power-On Reset)** que restaura NRST

## Resumen de Estados

| Estado | PA1 como GPIO | PA1 como I2C | PA1 como Reset |
|--------|---------------|--------------|----------------|
| **NRST Habilitado** (default) | ❌ Se reinicia | ❌ Se reinicia | ✅ Funciona |
| **NRST Deshabilitado** | ✅ Funciona | ✅ Funciona | ❌ No funciona |

## Procedimiento Completo

### Para Deshabilitar NRST (usar PA1 como GPIO/I2C):
```c
void SYSCFG_DL_SYSCTL_init(void)
{
    DL_SYSCTL_disableNRSTPin();  // ← Agregar esta línea
    
    DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
    // ...
}
```

### Para Re-habilitar NRST:
1. **Desconecta** completamente la alimentación
2. **Espera** 10 segundos
3. **Reconecta** la alimentación  
4. **Flashea inmediatamente** un código SIN `DL_SYSCTL_disableNRSTPin()`

## Build

Este directorio es principalmente documental. Si agregas firmware de prueba aquí, sigue la misma estructura de los demás ejemplos en `examples/`.

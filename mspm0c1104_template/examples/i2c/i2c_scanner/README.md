# I2C Scanner - MSPM0C1104

Escanea el bus I2C en busca de dispositivos conectados y reporta las direcciones encontradas por UART.

## Hardware

### Pines I2C
- **PA0** (PINCM1) - I2C SDA (requiere pull-up externo de 2.2kΩ - 10kΩ)
- **PA1** (PINCM2) - I2C SCL (5V tolerant, open-drain - requiere pull-up externo de 2.2kΩ - 10kΩ)

⚠️ **IMPORTANTE**: Configurar jumper **J5 en OFF** para desconectar el circuito de reset y liberar PA1 para I2C.

### Pines UART (para monitoreo)
- **PA24** (PINCM25) - UART RX
- **PA27** (PINCM28) - UART TX

### Pin LED (indicador de escaneo)
- **PA26** (PINCM27) - LED (Open Drain - requiere pull-up externo de 10kΩ)

## Configuración

- **I2C Speed**: 100 kHz (Standard Mode)
- **UART Baud**: 115200
- **Rango de escaneo**: 0x08 - 0x77 (direcciones válidas de 7 bits)

## Funcionamiento

1. Al iniciar, muestra información del dispositivo (UID)
2. Escanea todas las direcciones I2C válidas (0x08 a 0x77)
3. Reporta por UART las direcciones que responden (ACK)
4. El LED parpadea durante el escaneo
5. Espera 5 segundos y repite el escaneo

## Compilar y Flashear

```bash
cd examples/i2c/i2c_scanner
cmake --preset ninja-debug
cmake --build --preset build-debug
cmake --build build --target flash
```

Windows:

```bash
cmake --preset ninja-windows
cmake --build --preset build-windows
cmake --build build-windows --target flash
```

Compatibilidad con wrapper:

```bash
make clean && make && make flash
```

## Salida Esperada (UART)

```
====================================
  MSPM0C1104 - I2C Scanner
====================================

UID: 009FEC31-3BBA102F-807957B3

I2C Configuration:
  SDA: PA0 (PINCM1)
  SCL: PA1 (PINCM2) - J5 must be OFF
  Speed: 100 kHz

UART: 115200 baud
LED: PA26 (Open Drain)

>>> Starting I2C Scan <<<
Scanning addresses 0x08 to 0x77...

Device found at address: 0x3C
Device found at address: 0x76

--- Scan Complete ---
Devices found: 2

Next scan in 5 seconds...
```

## Dispositivos I2C Comunes

| Dirección | Dispositivo Típico |
|-----------|-------------------|
| 0x3C      | OLED SSD1306 (128x64) |
| 0x3D      | OLED SSD1306 (alternativo) |
| 0x48      | ADS1115 ADC, TMP102 |
| 0x68      | MPU6050, DS1307 RTC |
| 0x76      | BMP280, BME280 |
| 0x77      | BMP280, BME280 (alt) |

## Notas Importantes

⚠️ **Jumper J5**: DEBE estar en posición **OFF** para desconectar el botón S3 y el circuito de reset de PA1, liberándolo para uso como I2C SCL.

⚠️ **Resistencias Pull-up**: El bus I2C requiere resistencias pull-up en SDA y SCL (típicamente 4.7kΩ). Sin ellas, el bus no funcionará. PA1 ya tiene pull-up interno cuando J5 está ON, pero para I2C es mejor usar pull-ups externos.

⚠️ **Nivel de Voltaje**: PA1 es 5V tolerant, pero se recomienda operar a 3.3V para compatibilidad con otros dispositivos I2C.

⚠️ **Conexión del LED**: PA26 es Open Drain, requiere pull-up externo de 10kΩ a 3.3V.

## Troubleshooting

**No se encuentran dispositivos:**
- Verificar resistencias pull-up en SDA y SCL
- Verificar conexiones (SDA, SCL, GND, VCC)
- Verificar que los dispositivos estén alimentados a 3.3V
- Verificar dirección I2C del dispositivo (consultar datasheet)
- Intentar reducir la velocidad I2C (modificar `DL_I2C_setTimerPeriod`)

**El LED no parpadea:**
- Verificar resistencia pull-up de 10kΩ en PA26
- Verificar conexión del LED

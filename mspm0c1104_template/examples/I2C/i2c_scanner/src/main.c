/*
 * I2C Scanner - MSPM0C1104
 * Hardware I2C implementation (NRST disabled)
 * 
 * Hardware:
 *   PA0 (PINCM1) - I2C SDA (Hardware I2C0)
 *   PA1 (PINCM2) - I2C SCL (Hardware I2C0 - NRST disabled permanently)
 *   PA24 (PINCM25) - UART RX
 *   PA27 (PINCM28) - UART TX
 *   PA26 (PINCM27) - LED (Open Drain - needs 10kΩ pull-up)
 *
 * Function:
 *   - Scans I2C bus for connected devices (0x08 - 0x77)
 *   - Reports found devices via UART
 *   - LED blinks during scanning
 *   - UART: 115200 baud, Hardware I2C: 100kHz
 *
 * Requirements:
 *   - External 2.2kΩ-10kΩ pull-ups on SDA and SCL (R4, R5 on your board)
 *   - NRST disabled on first boot (done automatically)
 */

#include "ti_msp_dl_config.h"
#include <stdio.h>
#include <string.h>

void UART_SendString(const char *str);
bool I2C_ScanAddress(uint8_t address);

int main(void)
{
    char buffer[128];
    uint8_t devices_found = 0;
    
    /* Inicializar hardware */
    SYSCFG_DL_init();
    
    /* Leer Device ID */
    volatile uint32_t *factory_region = (volatile uint32_t *)0x41C40000;
    uint32_t traceid = factory_region[0];
    uint32_t deviceid = factory_region[1];
    uint32_t userid = factory_region[2];
    
    /* Mensaje de bienvenida */
    UART_SendString("\r\n\r\n");
    UART_SendString("====================================\r\n");
    UART_SendString("  MSPM0C1104 - I2C Scanner\r\n");
    UART_SendString("====================================\r\n\r\n");
    
    snprintf(buffer, sizeof(buffer), 
             "UID: %08lX-%08lX-%08lX\r\n\r\n", 
             traceid, deviceid, userid);
    UART_SendString(buffer);
    
    UART_SendString("I2C Configuration:\r\n");
    UART_SendString("  SDA: PA0 (PINCM1)\r\n");
    UART_SendString("  SCL: PA1 (PINCM2)\r\n");
    UART_SendString("  Hardware I2C @ 100 kHz\r\n");
    UART_SendString("  NRST disabled\r\n\r\n");
    
    UART_SendString("UART: 115200 baud\r\n");
    UART_SendString("LED: PA26 (Open Drain)\r\n\r\n");
    
    while (1) {
        devices_found = 0;
        
        UART_SendString("\r\n>>> Starting I2C Scan <<<\r\n");
        UART_SendString("Scanning addresses 0x08 to 0x77...\r\n\r\n");
        
        /* LED ON - inicio de escaneo */
        DL_GPIO_clearPins(GPIOA, GPIO_LED_PIN);
        
        /* Escanear direcciones de 7 bits (0x08 a 0x77) */
        for (uint8_t addr = 0x08; addr <= 0x77; addr++) {
            if (I2C_ScanAddress(addr)) {
                snprintf(buffer, sizeof(buffer), 
                         "Device found at address: 0x%02X\r\n", addr);
                UART_SendString(buffer);
                devices_found++;
            }
            
            /* Toggle LED cada 16 direcciones */
            if ((addr & 0x0F) == 0) {
                DL_GPIO_togglePins(GPIOA, GPIO_LED_PIN);
            }
            
            delay_cycles(48000);  // ~2ms entre intentos
        }
        
        /* LED OFF - fin de escaneo */
        DL_GPIO_setPins(GPIOA, GPIO_LED_PIN);
        
        UART_SendString("\r\n--- Scan Complete ---\r\n");
        snprintf(buffer, sizeof(buffer), 
                 "Devices found: %d\r\n\r\n", devices_found);
        UART_SendString(buffer);
        
        if (devices_found == 0) {
            UART_SendString("No I2C devices found!\r\n");
            UART_SendString("Check connections and pull-ups\r\n\r\n");
        }
        
        UART_SendString("Next scan in 5 seconds...\r\n");
        
        /* Esperar 5 segundos */
        delay_cycles(120000000);
    }
}

void UART_SendString(const char *str)
{
    while (*str) {
        while (DL_UART_Main_isBusy(UART_INST));
        DL_UART_Main_transmitData(UART_INST, *str++);
    }
}

bool I2C_ScanAddress(uint8_t address)
{
    /* Wait for I2C idle */
    while (!(DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_IDLE));
    
    /* Try to write 0 bytes (just START + ADDRESS + STOP) */
    DL_I2C_startControllerTransfer(I2C_INST, address,
        DL_I2C_CONTROLLER_DIRECTION_TX, 0);
    
    /* Small delay for I2C errata workaround */
    delay_cycles(100);
    
    /* Wait for transfer to complete */
    while (DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_BUSY);
    
    /* Check for error (NACK = no device) */
    if (DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_ERROR) {
        return false;  // No device at this address
    }
    
    return true;  // Device found (ACK received)
}

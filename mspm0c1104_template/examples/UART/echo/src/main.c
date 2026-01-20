/*
 * UART Demo Continuo - MSPM0C1104
 * 
 * Configuración:
 *   PA24 (PINCM25) - UART RX
 *   PA27 (PINCM28) - UART TX
 *   PA0  (PINCM1)  - LED (Open Drain - requiere pull-up externo 10kΩ)
 *   Baud Rate: 115200
 *   8N1 (8 bits, sin paridad, 1 bit de parada)
 *
 * Funcionamiento:
 *   - Envía mensajes continuamente por UART
 *   - LED parpadea cada 500ms para validar funcionamiento
 *   - Presiona 's' para detener el loop
 */

#include "ti_msp_dl_config.h"
#include <stdio.h>

void UART_SendString(const char *str)
{
    while (*str) {
        while (DL_UART_Main_isBusy(UART_INST));
        DL_UART_Main_transmitData(UART_INST, *str++);
    }
}

int main(void)
{
    char buffer[128];
    uint8_t received;
    bool running = true;
    bool led_state = false;
    
    /* Inicializar hardware */
    SYSCFG_DL_init();
    
    /* Leer Device ID desde Factory Region (0x41C40000) */
    volatile uint32_t *factory_region = (volatile uint32_t *)0x41C40000;
    uint32_t traceid = factory_region[0];   // TRACEID @ 0x41C40000
    uint32_t deviceid = factory_region[1];  // DEVICEID @ 0x41C40004
    uint32_t userid = factory_region[2];    // USERID @ 0x41C40008
    
    /* Mensaje de bienvenida */
    UART_SendString("\r\n\r\n");
    UART_SendString("====================================\r\n");
    UART_SendString("  WELCOME TO TEXAS INSTRUMENTS\r\n");
    UART_SendString("  MSPM0C1104 - UART Demo\r\n");
    UART_SendString("====================================\r\n\r\n");
    
    snprintf(buffer, sizeof(buffer), 
             "UID: %08lX-%08lX-%08lX\r\n\r\n", 
             traceid, deviceid, userid);
    UART_SendString(buffer);
    
    UART_SendString("Baud Rate: 115200\r\n");
    UART_SendString("PA24=RX, PA27=TX\r\n");
    UART_SendString("PA0=LED (Open Drain)\r\n\r\n");
    UART_SendString("Press 's' to stop loop\r\n");
    UART_SendString("====================================\r\n\r\n");
    
    while (1) {
        /* Verificar si hay datos recibidos */
        if (!DL_UART_Main_isRXFIFOEmpty(UART_INST)) {
            received = DL_UART_Main_receiveData(UART_INST);
            
            /* Detener si se presiona 's' o 'S' */
            if (received == 's' || received == 'S') {
                running = !running;
                if (running) {
                    UART_SendString("\r\n>>> Loop STARTED <<<\r\n\r\n");
                } else {
                    UART_SendString("\r\n>>> Loop STOPPED <<<\r\n");
                    UART_SendString("Press 's' to resume\r\n\r\n");
                }
            }
        }
        
        /* Enviar UID solo si está activo */
        if (running) {
            /* Toggle LED */
            led_state = !led_state;
            if (led_state) {
                DL_GPIO_setPins(GPIOA, GPIO_LED_PIN);
            } else {
                DL_GPIO_clearPins(GPIOA, GPIO_LED_PIN);
            }
            
            snprintf(buffer, sizeof(buffer), 
                     "UID: %08lX-%08lX-%08lX\r\n", 
                     traceid, deviceid, userid);
            UART_SendString(buffer);
        }
        
        /* Delay ~500ms */
        delay_cycles(12000000);
    }
}

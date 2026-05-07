/*
 * UART Echo Example - MSPM0C1104
 * 
 * Hardware:
 *   PA24 (PINCM25) - UART RX
 *   PA27 (PINCM28) - UART TX
 *   PA0  (PINCM1)  - LED (Open Drain - requires external 10kΩ pull-up)
 *   Baud Rate: 115200
 *   8N1 (8 bits, no parity, 1 stop bit)
 *
 * Function:
 *   - Echoes back any character received via UART
 *   - LED toggles with each received character
 *   - Shows device UID at startup
 */

#include "ti_msp_dl_config.h"
#include <stdio.h>

/* Factory Region addresses for UID */
#define FACTORY_REGION_BASE     0x41C40000
#define TRACEID_OFFSET          0x00
#define DEVICEID_OFFSET         0x04  
#define USERID_OFFSET           0x08

void UART_SendString(const char* str);
void ShowWelcomeMessage(void);

int main(void)
{
    uint8_t received_char;
    bool led_state = false;
    
    SYSCFG_DL_init();
    
    /* Show welcome message with UID */
    ShowWelcomeMessage();
    
    UART_SendString("\r\n====================================\r\n");
    UART_SendString("  UART Echo Demo\r\n");
    UART_SendString("====================================\r\n");
    UART_SendString("\r\nType any character to echo it back\r\n");
    UART_SendString("LED toggles with each character\r\n\r\n");
    UART_SendString("Ready!\r\n\r\n");
    
    while (1) {
        /* Check if data received */
        if (!DL_UART_Main_isRXFIFOEmpty(UART_INST)) {
            received_char = DL_UART_Main_receiveData(UART_INST);
            
            /* Echo back the character */
            DL_UART_Main_transmitDataBlocking(UART_INST, received_char);
            
            /* Toggle LED */
            led_state = !led_state;
            if (led_state) {
                DL_GPIO_setPins(GPIOA, GPIO_LED_PIN);
            } else {
                DL_GPIO_clearPins(GPIOA, GPIO_LED_PIN);
            }
        }
        
        /* Small delay to prevent CPU hogging */
        delay_cycles(2400);  // ~100us at 24MHz
    }
}

void UART_SendString(const char* str)
{
    while (*str) {
        DL_UART_Main_transmitDataBlocking(UART_INST, *str++);
    }
}

void ShowWelcomeMessage(void)
{
    char buffer[100];
    volatile uint32_t *factory_region = (volatile uint32_t*)FACTORY_REGION_BASE;
    uint32_t trace_id = factory_region[TRACEID_OFFSET / 4];
    uint32_t device_id = factory_region[DEVICEID_OFFSET / 4];
    uint32_t user_id = factory_region[USERID_OFFSET / 4];
    
    UART_SendString("\r\n\r\n");
    UART_SendString("====================================\r\n");
    UART_SendString("  TEXAS INSTRUMENTS\r\n");
    UART_SendString("  MSPM0C1104 - UART Echo\r\n");
    UART_SendString("====================================\r\n\r\n");
    
    snprintf(buffer, sizeof(buffer), "UID: %08lX-%08lX-%08lX\r\n\r\n", 
             trace_id, device_id, user_id);
    UART_SendString(buffer);
    
    UART_SendString("UART Configuration:\r\n");
    UART_SendString("  PA24 - RX, PA27 - TX\r\n");
    UART_SendString("  Baud: 115200\r\n");
}

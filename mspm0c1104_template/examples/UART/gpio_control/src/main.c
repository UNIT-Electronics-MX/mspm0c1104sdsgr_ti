/*
 * UART GPIO Control Example for MSPM0C1104
 * 
 * Hardware:
 *   PA24 (PINCM25) - UART0_RX
 *   PA27 (PINCM28) - UART0_TX
 *   PA1  (PINCM2)  - Button input (with internal pull-up)
 *   PA0  (PINCM1)  - LED output (requires external 10kΩ pull-up - Open Drain)
 *
 * Function:
 *   - Press button on PA1 to toggle LED on PA0
 *   - Reports button press and LED state via UART
 *   - 115200 baud, 8N1
 */

#include "ti_msp_dl_config.h"
#include <stdio.h>
#include <string.h>

#define DELAY_MS(ms) ((CPUCLK_FREQ / 1000U) * (ms))

/* Factory Region addresses for UID */
#define FACTORY_REGION_BASE     0x41C40000
#define TRACEID_OFFSET          0x00
#define DEVICEID_OFFSET         0x04  
#define USERID_OFFSET           0x08

/* Function prototypes */
void UART_SendString(const char* str);
void ShowWelcomeMessage(void);

/* Global variables */
volatile bool led_state = false;
uint8_t last_button_state = 1;  // Start high (pull-up)

int main(void)
{
    uint8_t current_button_state;
    
    SYSCFG_DL_init();
    
    UART_SendString("\r\n====================================\r\n");
    UART_SendString("        Iniciando Serial\r\n");
    UART_SendString("====================================\r\n");
    UART_SendString("Ready!\r\n\r\n");
    
    while (1) {
        /* Read button state (active LOW with pull-up) */
        current_button_state = DL_GPIO_readPins(GPIOA, GPIO_BUTTON_PIN);
        
        
        DL_GPIO_setPins(GPIOA, GPIO_LED_PIN);
        UART_SendString("LED OFF\r\n");
        DL_Common_delayCycles(DELAY_MS(1000));
        DL_GPIO_clearPins(GPIOA, GPIO_LED_PIN);
        UART_SendString("LED ON\r\n");
        DL_Common_delayCycles(DELAY_MS(1000));
        UART_SendString("\r{Result: OK}\r\n");
        DL_Common_delayCycles(DELAY_MS(1000));
        
        /* Small delay to prevent CPU hogging */
        delay_cycles(2400);  // ~100us
    }
}

void UART_SendString(const char* str)
{
    while (*str) {
        DL_UART_Main_transmitDataBlocking(UART_INST, *str++);
    }
}

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
    
    /* Show welcome message with UID */
    ShowWelcomeMessage();
    
    UART_SendString("\r\n====================================\r\n");
    UART_SendString("  GPIO Control Demo\r\n");
    UART_SendString("====================================\r\n");
    UART_SendString("\r\nPress button on PA1 to toggle LED\r\n");
    UART_SendString("LED is on PA0 (Open Drain - needs pull-up)\r\n\r\n");
    UART_SendString("Ready!\r\n\r\n");
    
    while (1) {
        /* Read button state (active LOW with pull-up) */
        current_button_state = DL_GPIO_readPins(GPIOA, GPIO_BUTTON_PIN);
        
        /* Detect button press (falling edge) */
        if (last_button_state && !current_button_state) {
            /* Debounce delay */
            delay_cycles(240000);  // ~10ms at 24MHz
            
            /* Verify button still pressed */
            if (!DL_GPIO_readPins(GPIOA, GPIO_BUTTON_PIN)) {
                /* Toggle LED */
                led_state = !led_state;
                
                if (led_state) {
                    DL_GPIO_setPins(GPIOA, GPIO_LED_PIN);
                    UART_SendString("Button pressed! LED ON\r\n");
                } else {
                    DL_GPIO_clearPins(GPIOA, GPIO_LED_PIN);
                    UART_SendString("Button pressed! LED OFF\r\n");
                }
                
                /* Wait for button release */
                while (!DL_GPIO_readPins(GPIOA, GPIO_BUTTON_PIN)) {
                    delay_cycles(24000);  // Poll every 1ms
                }
                
                UART_SendString("Button released\r\n\r\n");
            }
        }
        
        last_button_state = current_button_state;
        
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

void ShowWelcomeMessage(void)
{
    char buffer[100];
    volatile uint32_t *factory_region = (volatile uint32_t*)FACTORY_REGION_BASE;
    uint32_t trace_id = factory_region[TRACEID_OFFSET / 4];
    uint32_t device_id = factory_region[DEVICEID_OFFSET / 4];
    uint32_t user_id = factory_region[USERID_OFFSET / 4];
    
    UART_SendString("\r\n\r\n");
    UART_SendString("====================================\r\n");
    UART_SendString("  MSPM0C1104 - GPIO Control\r\n");
    UART_SendString("====================================\r\n\r\n");
    
    snprintf(buffer, sizeof(buffer), "UID: %08lX-%08lX-%08lX\r\n\r\n", 
             trace_id, device_id, user_id);
    UART_SendString(buffer);
    
    UART_SendString("GPIO Configuration:\r\n");
    UART_SendString("  PA1 (PINCM2)  - Button input (internal pull-up)\r\n");
    UART_SendString("  PA0 (PINCM1)  - LED output (Open Drain)\r\n\r\n");
    
    UART_SendString("UART Configuration:\r\n");
    UART_SendString("  PA24 - RX, PA27 - TX\r\n");
    UART_SendString("  Baud: 115200\r\n\r\n");
}

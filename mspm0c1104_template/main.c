#include "ti_msp_dl_config.h"

int main(void)
{
    // Inicializar el sistema
    SYSCFG_DL_init();
    
    while(1) 
    {
        // Encender PA24
        DL_GPIO_setPins(GPIO_PA24_PORT, GPIO_PA24_PIN);
        DL_Common_delayCycles(CPUCLK_FREQ / 8);  // 500ms encendido
        
        // Apagar PA24
        DL_GPIO_clearPins(GPIO_PA24_PORT, GPIO_PA24_PIN);
        DL_Common_delayCycles(CPUCLK_FREQ / 8);  // 500ms apagado
    }
}

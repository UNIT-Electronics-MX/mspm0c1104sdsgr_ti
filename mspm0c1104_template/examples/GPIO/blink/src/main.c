#include "ti_msp_dl_config.h"

#define DELAY_MS(ms) ((CPUCLK_FREQ / 1000U) * (ms))

int main(void)
{
    SYSCFG_DL_init();

    while (1)
    {
        DL_GPIO_setPins(GPIO_PA24_PORT, GPIO_PA24_PIN);
        DL_Common_delayCycles(DELAY_MS(200));

        DL_GPIO_clearPins(GPIO_PA24_PORT, GPIO_PA24_PIN);
        DL_Common_delayCycles(DELAY_MS(200));
    }
}
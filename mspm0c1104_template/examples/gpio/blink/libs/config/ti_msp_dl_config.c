/*
 * ti_msp_dl_config.c - Blink Configuration for PA0, PA24, PA27
 */

#include "ti_msp_dl_config.h"

void SYSCFG_DL_init(void)
{
    SYSCFG_DL_initPower();
    SYSCFG_DL_GPIO_init();
    SYSCFG_DL_SYSCTL_init();
}

void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_enablePower(GPIOA);
    delay_cycles(POWER_STARTUP_DELAY);
}

void SYSCFG_DL_GPIO_init(void)
{
    /* Configure PA0 como salida digital (Open Drain con pull-up externa) */
    DL_GPIO_initDigitalOutput(GPIO_PA0_IOMUX);
    DL_GPIO_clearPins(GPIO_PA0_PORT, GPIO_PA0_PIN);
    DL_GPIO_enableOutput(GPIO_PA0_PORT, GPIO_PA0_PIN);
    
    /* Configure PA24 como salida digital */
    DL_GPIO_initDigitalOutput(GPIO_PA24_IOMUX);
    DL_GPIO_clearPins(GPIO_PA24_PORT, GPIO_PA24_PIN);
    DL_GPIO_enableOutput(GPIO_PA24_PORT, GPIO_PA24_PIN);
    
    /* Configure PA27 como salida digital */
    DL_GPIO_initDigitalOutput(GPIO_PA27_IOMUX);
    DL_GPIO_clearPins(GPIO_PA27_PORT, GPIO_PA27_PIN);
    DL_GPIO_enableOutput(GPIO_PA27_PORT, GPIO_PA27_PIN);
}

void SYSCFG_DL_SYSCTL_init(void)
{
    DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
    DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE);

    //Low Power Mode is configured to be SLEEP0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);
}

bool dl_tamper_check_status(void)
{
    return false;
}

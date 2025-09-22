/*
 * ti_msp_dl_config.c - PA24 Configuration SIMPLE (sin I2C)
 * Volver a la configuración básica que funcionaba
 */

#include "ti_msp_dl_config.h"

void SYSCFG_DL_init(void)
{
    SYSCFG_DL_initPower();
    SYSCFG_DL_GPIO_init();
    SYSCFG_DL_SYSCTL_init();
    SYSCFG_DL_I2C_init();  // Agregar I2C al final
}

void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_I2C_reset(I2C_INST);       // Reset I2C
    
    DL_GPIO_enablePower(GPIOA);
    DL_I2C_enablePower(I2C_INST); // Enable I2C power
    delay_cycles(POWER_STARTUP_DELAY);
}

void SYSCFG_DL_GPIO_init(void)
{
    /* Configure PA24 PRIMERO - mantener funcionando */
    DL_GPIO_initDigitalOutput(GPIO_PA24_IOMUX);
    DL_GPIO_clearPins(GPIO_PA24_PORT, GPIO_PA24_PIN);
    DL_GPIO_enableOutput(GPIO_PA24_PORT, GPIO_PA24_PIN);
    
    /* Configure I2C pins DESPUÉS - PA0 (SDA) + PA1 (SCL) */
    DL_GPIO_initPeripheralInputFunctionFeatures(GPIO_I2C_IOMUX_SDA,
        GPIO_I2C_IOMUX_SDA_FUNC, DL_GPIO_INVERSION_DISABLE,
        DL_GPIO_RESISTOR_NONE, DL_GPIO_HYSTERESIS_DISABLE,
        DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initPeripheralInputFunctionFeatures(GPIO_I2C_IOMUX_SCL,
        GPIO_I2C_IOMUX_SCL_FUNC, DL_GPIO_INVERSION_DISABLE,
        DL_GPIO_RESISTOR_NONE, DL_GPIO_HYSTERESIS_DISABLE,
        DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_enableHiZ(GPIO_I2C_IOMUX_SDA);
    DL_GPIO_enableHiZ(GPIO_I2C_IOMUX_SCL);
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

/* Configuración de I2C para SSD1306 */
static const DL_I2C_ClockConfig gI2CClockConfig = {
    .clockSel = DL_I2C_CLOCK_BUSCLK,
    .divideRatio = DL_I2C_CLOCK_DIVIDE_1,
};

void SYSCFG_DL_I2C_init(void) {
    DL_I2C_setClockConfig(I2C_INST, (DL_I2C_ClockConfig *) &gI2CClockConfig);
    DL_I2C_disableAnalogGlitchFilter(I2C_INST);

    /* Configurar modo Controller para I2C */
    DL_I2C_resetControllerTransfer(I2C_INST);
    
    /* Configurar frecuencia a 400kHz */
    DL_I2C_setTimerPeriod(I2C_INST, 7);  // 24MHz / (2 * (7+1) * 4) ≈ 375kHz
    
    DL_I2C_setControllerTXFIFOThreshold(I2C_INST, DL_I2C_TX_FIFO_LEVEL_BYTES_1);
    DL_I2C_setControllerRXFIFOThreshold(I2C_INST, DL_I2C_RX_FIFO_LEVEL_BYTES_1);
    DL_I2C_enableControllerClockStretching(I2C_INST);

    /* Habilitar el módulo I2C */
    DL_I2C_enableController(I2C_INST);
}

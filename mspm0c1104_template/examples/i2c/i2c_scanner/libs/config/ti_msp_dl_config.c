/*
 * ti_msp_dl_config.c - I2C Scanner Configuration
 */

#include "ti_msp_dl_config.h"

void SYSCFG_DL_init(void)
{
    SYSCFG_DL_initPower();
    SYSCFG_DL_GPIO_init();
    SYSCFG_DL_SYSCTL_init();
    SYSCFG_DL_I2C_init();
    SYSCFG_DL_UART_init();
}

void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_I2C_reset(I2C_INST);
    DL_UART_Main_reset(UART_INST);
    
    DL_GPIO_enablePower(GPIOA);
    DL_I2C_enablePower(I2C_INST);
    DL_UART_Main_enablePower(UART_INST);
    delay_cycles(POWER_STARTUP_DELAY);
}

void SYSCFG_DL_GPIO_init(void)
{
    /* Configure LED pin - PA26 as Open Drain output */
    DL_GPIO_initDigitalOutputFeatures(GPIO_LED_IOMUX,
        DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
        DL_GPIO_DRIVE_STRENGTH_LOW, DL_GPIO_HIZ_DISABLE);
    
    DL_GPIO_setPins(GPIO_LED_PORT, GPIO_LED_PIN);
    DL_GPIO_enableOutput(GPIO_LED_PORT, GPIO_LED_PIN);
    
    /* Configure Hardware I2C pins - PA0 (SDA) and PA1 (SCL) */
    DL_GPIO_initPeripheralInputFunction(
        GPIO_I2C_IOMUX_SDA, GPIO_I2C_IOMUX_SDA_FUNC);
    DL_GPIO_initPeripheralInputFunction(
        GPIO_I2C_IOMUX_SCL, GPIO_I2C_IOMUX_SCL_FUNC);
    
    /* Configure UART pins */
    DL_GPIO_initPeripheralOutputFunction(
        GPIO_UART_IOMUX_TX, GPIO_UART_IOMUX_TX_FUNC);
    DL_GPIO_initPeripheralInputFunction(
        GPIO_UART_IOMUX_RX, GPIO_UART_IOMUX_RX_FUNC);
}

void SYSCFG_DL_SYSCTL_init(void)
{
    /* Disable NRST functionality on PA1 to use as GPIO for I2C */
    /* WARNING: Can only be re-enabled by Power-On Reset (POR) */
    DL_SYSCTL_disableNRSTPin();
    
    DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
    DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE);
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);
}

void SYSCFG_DL_UART_init(void)
{
    static const DL_UART_Main_ClockConfig gUARTClockConfig = {
        .clockSel = DL_UART_MAIN_CLOCK_BUSCLK,
        .divideRatio = DL_UART_MAIN_CLOCK_DIVIDE_RATIO_1
    };

    static const DL_UART_Main_Config gUARTConfig = {
        .mode        = DL_UART_MAIN_MODE_NORMAL,
        .direction   = DL_UART_MAIN_DIRECTION_TX_RX,
        .flowControl = DL_UART_MAIN_FLOW_CONTROL_NONE,
        .parity      = DL_UART_MAIN_PARITY_NONE,
        .wordLength  = DL_UART_MAIN_WORD_LENGTH_8_BITS,
        .stopBits    = DL_UART_MAIN_STOP_BITS_ONE
    };

    DL_UART_Main_setClockConfig(UART_INST, (DL_UART_Main_ClockConfig *) &gUARTClockConfig);
    DL_UART_Main_init(UART_INST, (DL_UART_Main_Config *) &gUARTConfig);
    
    /* Configure baud rate: 115200 at 24MHz BUSCLK (24MHz / (16 * 115200) = 13.02) */
    DL_UART_Main_setOversampling(UART_INST, DL_UART_OVERSAMPLING_RATE_16X);
    DL_UART_Main_setBaudRateDivisor(UART_INST, 13, 1);
    
    DL_UART_Main_enableFIFOs(UART_INST);
    DL_UART_Main_enable(UART_INST);
}

void SYSCFG_DL_I2C_init(void)
{
    /* I2C clock configuration - using BUSCLK (24MHz) */
    static const DL_I2C_ClockConfig gI2CClockConfig = {
        .clockSel = DL_I2C_CLOCK_BUSCLK,
        .divideRatio = DL_I2C_CLOCK_DIVIDE_1
    };
    
    DL_I2C_setClockConfig(I2C_INST, (DL_I2C_ClockConfig *) &gI2CClockConfig);
    
    /* Configure I2C as Controller at 100kHz */
    DL_I2C_resetControllerTransfer(I2C_INST);
    DL_I2C_setTimerPeriod(I2C_INST, 239);          // SCL period: (239+1) / 24MHz = 10us = 100kHz
    DL_I2C_setControllerTXFIFOThreshold(I2C_INST, DL_I2C_TX_FIFO_LEVEL_EMPTY);
    DL_I2C_setControllerRXFIFOThreshold(I2C_INST, DL_I2C_RX_FIFO_LEVEL_BYTES_1);
    DL_I2C_enableControllerClockStretching(I2C_INST);
    
    /* Enable I2C module */
    DL_I2C_enableController(I2C_INST);
}

bool dl_tamper_check_status(void)
{
    return false;
}

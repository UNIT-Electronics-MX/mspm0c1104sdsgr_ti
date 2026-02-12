/*
 * ti_msp_dl_config.c - UART Echo Configuration
 */

#include "ti_msp_dl_config.h"

void SYSCFG_DL_init(void)
{
    SYSCFG_DL_initPower();
    SYSCFG_DL_GPIO_init();
    SYSCFG_DL_SYSCTL_init();
    SYSCFG_DL_UART_init();
}

void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_UART_Main_reset(UART_INST);
    
    DL_GPIO_enablePower(GPIOA);
    DL_UART_Main_enablePower(UART_INST);
    delay_cycles(POWER_STARTUP_DELAY);
}

void SYSCFG_DL_GPIO_init(void)
{
    /* Configure LED pin - PA0 as Open Drain output */
    DL_GPIO_initDigitalOutputFeatures(GPIO_LED_IOMUX,
        DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
        DL_GPIO_DRIVE_STRENGTH_LOW, DL_GPIO_HIZ_DISABLE);
    
    DL_GPIO_setPins(GPIO_LED_PORT, GPIO_LED_PIN);
    DL_GPIO_enableOutput(GPIO_LED_PORT, GPIO_LED_PIN);
    
    /* Configure UART pins */
    DL_GPIO_initPeripheralOutputFunction(
        GPIO_UART_IOMUX_TX, GPIO_UART_IOMUX_TX_FUNC);
    DL_GPIO_initPeripheralInputFunction(
        GPIO_UART_IOMUX_RX, GPIO_UART_IOMUX_RX_FUNC);
}

void SYSCFG_DL_SYSCTL_init(void)
{
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
    
    /* Configure baud rate: 115200 at 24MHz BUSCLK */
    /* 24MHz / (16 * 13.02) = 115246 (~115200) */
    DL_UART_Main_setOversampling(UART_INST, DL_UART_OVERSAMPLING_RATE_16X);
    DL_UART_Main_setBaudRateDivisor(UART_INST, 13, 1);
    
    /* Configure TX/RX FIFO thresholds for better reliability */
    DL_UART_Main_setTXFIFOThreshold(UART_INST, DL_UART_TX_FIFO_LEVEL_ONE_ENTRY);
    DL_UART_Main_setRXFIFOThreshold(UART_INST, DL_UART_RX_FIFO_LEVEL_ONE_ENTRY);
    
    DL_UART_Main_enableFIFOs(UART_INST);
    DL_UART_Main_enable(UART_INST);
}

bool dl_tamper_check_status(void)
{
    return false;
}

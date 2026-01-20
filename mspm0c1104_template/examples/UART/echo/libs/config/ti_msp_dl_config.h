/*
 * ti_msp_dl_config.h - UART Echo Configuration
 * PA24 = UART RX, PA27 = UART TX
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_LP_MSPM0C1104

/* UART defines */
#define UART_INST                                                   UART0
#define UART_INST_IRQHandler                             UART0_IRQHandler
#define UART_INST_INT_IRQN                                 UART0_INT_IRQn
#define GPIO_UART_TX_PORT                                          GPIOA
#define GPIO_UART_TX_PIN                               DL_GPIO_PIN_27
#define GPIO_UART_IOMUX_TX                              (IOMUX_PINCM28)
#define GPIO_UART_IOMUX_TX_FUNC              IOMUX_PINCM28_PF_UART0_TX
#define GPIO_UART_RX_PORT                                          GPIOA
#define GPIO_UART_RX_PIN                               DL_GPIO_PIN_24
#define GPIO_UART_IOMUX_RX                              (IOMUX_PINCM25)
#define GPIO_UART_IOMUX_RX_FUNC              IOMUX_PINCM25_PF_UART0_RX

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/* clang-format off */
#define POWER_STARTUP_DELAY                     (16)
#define CPUCLK_FREQ                         24000000
/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_UART_init(void);

bool dl_tamper_check_status(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */

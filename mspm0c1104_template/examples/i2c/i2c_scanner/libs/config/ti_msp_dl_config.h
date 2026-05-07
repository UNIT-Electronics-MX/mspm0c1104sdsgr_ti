/*
 * ti_msp_dl_config.h - I2C Scanner Configuration
 * Hardware I2C + NRST disabled permanently
 * 
 * PA0 = I2C SDA (PINCM1) - Hardware I2C0_SDA
 * PA1 = I2C SCL (PINCM2) - Hardware I2C0_SCL (NRST disabled)
 * PA24 = UART RX
 * PA27 = UART TX
 * PA26 = LED (Open Drain)
 * 
 * NOTE: NRST is permanently disabled on first boot
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_LP_MSPM0C1104

/* Hardware I2C defines */
#define I2C_INST                                                    I2C0
#define I2C_INST_IRQHandler                              I2C0_IRQHandler
#define I2C_INST_INT_IRQN                                  I2C0_INT_IRQn
#define I2C_BUS_SPEED_HZ                                          100000
#define GPIO_I2C_SDA_PORT                                          GPIOA
#define GPIO_I2C_SDA_PIN                                 DL_GPIO_PIN_0
#define GPIO_I2C_IOMUX_SDA                                (IOMUX_PINCM1)
#define GPIO_I2C_IOMUX_SDA_FUNC               IOMUX_PINCM1_PF_I2C0_SDA
#define GPIO_I2C_SCL_PORT                                          GPIOA
#define GPIO_I2C_SCL_PIN                                 DL_GPIO_PIN_1
#define GPIO_I2C_IOMUX_SCL                                (IOMUX_PINCM2)
#define GPIO_I2C_IOMUX_SCL_FUNC               IOMUX_PINCM2_PF_I2C0_SCL

/* LED defines */
#define GPIO_LED_PORT                                              GPIOA
#define GPIO_LED_PIN                                   DL_GPIO_PIN_26
#define GPIO_LED_IOMUX                                 (IOMUX_PINCM27)

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
void SYSCFG_DL_I2C_init(void);

bool dl_tamper_check_status(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */

/*
 * ti_msp_dl_config.h - PA24 Configuration SIMPLE (sin I2C)
 * Para debug - volver a la configuración que funcionaba
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_LP_MSPM0C1104

/* GPIO defines for PA24 - CONFIGURACIÓN SIMPLE */
#define GPIO_PA24_PORT                                          (GPIOA)
#define GPIO_PA24_PIN                                   (DL_GPIO_PIN_24)
#define GPIO_PA24_IOMUX                                  (IOMUX_PINCM25)

/* I2C defines for SSD1306 OLED - PA0 (SDA) + PA1 (SCL) */
#define I2C_INST                                                    I2C0
#define I2C_INST_IRQHandler                              I2C0_IRQHandler
#define I2C_INST_INT_IRQN                                  I2C0_INT_IRQn
#define I2C_BUS_SPEED_HZ                                      400000
#define GPIO_I2C_SDA_PORT                                      GPIOA
#define GPIO_I2C_SDA_PIN                              DL_GPIO_PIN_0
#define GPIO_I2C_IOMUX_SDA                             (IOMUX_PINCM1)
#define GPIO_I2C_IOMUX_SDA_FUNC            IOMUX_PINCM1_PF_I2C0_SDA
#define GPIO_I2C_SCL_PORT                                      GPIOA
#define GPIO_I2C_SCL_PIN                              DL_GPIO_PIN_1
#define GPIO_I2C_IOMUX_SCL                             (IOMUX_PINCM2)
#define GPIO_I2C_IOMUX_SCL_FUNC            IOMUX_PINCM2_PF_I2C0_SCL

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
void SYSCFG_DL_I2C_init(void);

bool dl_tamper_check_status(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */

/*
 * ti_msp_dl_config.h - PA24 Configuration SIMPLE (sin I2C)
 * Para debug - volver a la configuración que funcionaba
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_LP_MSPM0C1104

/* GPIO defines for PA0 - LED Blink (Open Drain con pull-up externa) */
#define GPIO_PA0_PORT                                           (GPIOA)
#define GPIO_PA0_PIN                                    (DL_GPIO_PIN_0)
#define GPIO_PA0_IOMUX                                   (IOMUX_PINCM1)

/* GPIO defines for PA24 - LED Blink */
#define GPIO_PA24_PORT                                          (GPIOA)
#define GPIO_PA24_PIN                                   (DL_GPIO_PIN_24)
#define GPIO_PA24_IOMUX                                  (IOMUX_PINCM25)

/* GPIO defines for PA27 - LED Blink */
#define GPIO_PA27_PORT                                         (GPIOA)
#define GPIO_PA27_PIN                                  (DL_GPIO_PIN_27)
#define GPIO_PA27_IOMUX                                 (IOMUX_PINCM28)

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

bool dl_tamper_check_status(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */

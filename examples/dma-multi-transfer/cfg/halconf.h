/*
 * HAL configuration for WB32FQ95 Multi-Transfer DMA Test
 */

#ifndef HALCONF_H
#define HALCONF_H

#define _CHIBIOS_HAL_CONF_
#define _CHIBIOS_HAL_CONF_VER_8_4_

#include "mcuconf.h"

/* Enable PAL (GPIO) */
#define HAL_USE_PAL                         TRUE

/* Enable PWM for timer-based DMA transfers */
#define HAL_USE_PWM                         TRUE

/* Disable unused subsystems */
#define HAL_USE_ADC                         FALSE
#define HAL_USE_CAN                         FALSE
#define HAL_USE_CRY                         FALSE
#define HAL_USE_DAC                         FALSE
#define HAL_USE_EFL                         FALSE
#define HAL_USE_GPT                         FALSE
#define HAL_USE_I2C                         FALSE
#define HAL_USE_I2S                         FALSE
#define HAL_USE_ICU                         FALSE
#define HAL_USE_MAC                         FALSE
#define HAL_USE_MMC_SPI                     FALSE
#define HAL_USE_RTC                         FALSE
#define HAL_USE_SDC                         FALSE
#define HAL_USE_SERIAL                      FALSE
#define HAL_USE_SERIAL_USB                  FALSE
#define HAL_USE_SIO                         FALSE
#define HAL_USE_SPI                         FALSE
#define HAL_USE_TRNG                        FALSE
#define HAL_USE_UART                        FALSE
#define HAL_USE_USB                         FALSE
#define HAL_USE_WDG                         FALSE
#define HAL_USE_WSPI                        FALSE

/* PAL driver settings */
#define PAL_USE_CALLBACKS                   FALSE
#define PAL_USE_WAIT                        FALSE

#endif /* HALCONF_H */

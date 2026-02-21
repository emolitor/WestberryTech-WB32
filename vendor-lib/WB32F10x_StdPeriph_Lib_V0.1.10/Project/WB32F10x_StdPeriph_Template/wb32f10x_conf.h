/**
  * @file    Project/WB32F10x_StdPeriph_Template/wb32f10x_conf.h
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Library configuration file.
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WB32F10x_CONF_H
#define __WB32F10x_CONF_H

/* Includes ------------------------------------------------------------------*/
/* Uncomment/Comment the line below to enable/disable peripheral header file inclusion */
#include "wb32f10x_adc.h"
#include "wb32f10x_anctl.h"
#include "wb32f10x_bkp.h"
#include "wb32f10x_crc.h"
#include "wb32f10x_rcc.h"
#include "wb32f10x_dmac.h"
#include "wb32f10x_exti.h"
#include "wb32f10x_gpio.h"
#include "wb32f10x_i2c.h"
#include "wb32f10x_i2s.h"
#include "wb32f10x_iwdg.h"
#include "wb32f10x_led.h"
#include "wb32f10x_fmc.h"
#include "wb32f10x_pwr.h"
#include "wb32f10x_rng.h"
#include "wb32f10x_rtc.h"
#include "wb32f10x_sfm.h"
#include "wb32f10x_spi.h"
#include "wb32f10x_tim.h"
#include "wb32f10x_uart.h"
#include "wb32f10x_wwdg.h"
#include "misc.h" /* High level functions for NVIC and SysTick (add-on to CMSIS functions) */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line below to expanse the "assert_param" macro in the 
   Standard Peripheral Library drivers code */
/* #define USE_FULL_ASSERT    1 */

/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT
/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports 
  *         the name of the source file and the source line number of the call 
  *         that failed. If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#endif /* __WB32F10x_CONF_H */

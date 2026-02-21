/**
 * @file    wb32f10x_led.h
 * @author  Westberry Application Team
 * @version V0.1.10
 * @date    05-January-2023
 * @brief   This file contains all the functions prototypes for the LED firmware
 *          library.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WB32F10x_LED_H
#define __WB32F10x_LED_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/** @addtogroup WB32F10x_StdPeriph_Driver
  * @{
  */

/** @addtogroup LED
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void LED_DeInit(void);
void LED_Cmd(FunctionalState NewState);
void LED_SetSegmentCode(uint8_t idx_seg, uint8_t seg_code);
uint8_t LED_GetSegmentCode(uint8_t idx_seg);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __WB32F10x_LED_H */

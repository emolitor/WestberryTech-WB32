/**
 * @file    wb32f10x_rng.h
 * @author  Westberry Application Team
 * @version V0.1.10
 * @date    05-January-2023
 * @brief   This file contains all the functions prototypes for the RNG firmware
 *          library.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WB32F10x_RNG_H
#define __WB32F10x_RNG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/** @addtogroup WB32F10x_StdPeriph_Driver
  * @{
  */

/** @addtogroup RNG
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void RNG_DeInit(void);
void RNG_Cmd(FunctionalState NewState);
uint8_t RNG_RandByte(void);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __WB32F10x_RNG_H */

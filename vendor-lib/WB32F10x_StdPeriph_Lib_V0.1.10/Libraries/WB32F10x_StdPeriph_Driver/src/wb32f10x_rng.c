/**
 * @file    wb32f10x_rng.c
 * @author  Westberry Application Team
 * @version V0.1.10
 * @date    05-January-2023
 * @brief   This file provides all the RNG firmware functions.
 */

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x_rng.h"
#include "wb32f10x_rcc.h"

/** @addtogroup WB32F10x_StdPeriph_Driver
  * @{
  */

/** @defgroup RNG
  * @brief RNG driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup RNG_Private_Functions
  * @{
  */

/**
 * @brief  Deinitializes the RNG peripheral registers to their default reset values.
 * @return None
 */
void RNG_DeInit(void)
{
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_RNG, ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_RNG, DISABLE);
}

/**
 * @brief  Enables or disables the RNG peripheral.
 * @param  NewState: new state of the RNG peripheral.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None
 */
void RNG_Cmd(FunctionalState NewState)
{
  if (NewState != DISABLE) {
    RNG->STOP = 0x00;
  }
  else {
    RNG->STOP = 0x01;
  }
}

/**
 * @brief  Return a random byte.
 * @return Random byte.
 */
uint8_t RNG_RandByte(void)
{
  return RNG->RAND;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

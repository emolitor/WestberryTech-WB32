/**
 * @file    wb32f10x_led.c
 * @author  Westberry Application Team
 * @version V0.1.10
 * @date    05-January-2023
 * @brief   This file provides all the LED firmware functions.
 */

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x_led.h"
#include "wb32f10x_rcc.h"

/** @addtogroup WB32F10x_StdPeriph_Driver
  * @{
  */

/** @defgroup LED
  * @brief LED driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup LED_Private_Functions
  * @{
  */

/**
 * @brief  Deinitializes the LED peripheral registers to their default reset values.
 * @return None
 */
void LED_DeInit(void)
{
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_LED, ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_LED, DISABLE);
}

/**
 * @brief  Enables or disables the LED peripheral.
 * @param  NewState: new state of the LED peripheral.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None
 */
void LED_Cmd(FunctionalState NewState)
{
  if (NewState != DISABLE) {
    LED->CON |= 0x01;
  }
  else {
    LED->CON &= ~0x01;
  }
}

/**
 * @brief  Set the segment code.
 * @param  idx_seg: The segment to set. This parameter must be between 0 to 7.
 * @param  seg_code: The segment code. This parameter must be between 0x00 to 0x7F.
 * @return None
 */
void LED_SetSegmentCode(uint8_t idx_seg, uint8_t seg_code)
{
  uint32_t regaddr, tmp;
  tmp = (0x01 << idx_seg) - 1;
  seg_code = ((seg_code << 1) & (~tmp)) | (seg_code & tmp);
  seg_code &= ~(0x01 << idx_seg);
  regaddr = ((uint32_t)&LED->SEGL) - (idx_seg & 0x04);
  tmp = (idx_seg & 0x03) << 3;
  *((volatile uint32_t*)regaddr) = ((*((volatile uint32_t*)regaddr)) & (~(0xFFU << tmp))) | (seg_code << tmp);
}

/**
 * @brief  Get the segment code.
 * @param  idx_seg: The segment to set. This parameter must be between 0 to 7.
 * @return The segment code.
 */
uint8_t LED_GetSegmentCode(uint8_t idx_seg)
{
  uint32_t regaddr, tmp;
  uint8_t seg_code;
  regaddr = ((uint32_t)&LED->SEGL) - (idx_seg & 0x04);
  seg_code = (uint8_t)((*((volatile uint32_t*)regaddr)) >> ((idx_seg & 0x03) << 3));
  tmp = (0x01 << idx_seg) - 1;
  seg_code = ((seg_code >> 1) & (~tmp)) | (seg_code & tmp);
  return seg_code;
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

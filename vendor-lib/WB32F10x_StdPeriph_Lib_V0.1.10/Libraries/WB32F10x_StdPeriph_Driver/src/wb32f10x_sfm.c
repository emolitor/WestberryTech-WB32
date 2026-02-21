/**
 * @file    wb32f10x_sfm.c
 * @author  Westberry Application Team
 * @version V0.1.10
 * @date    05-January-2023
 * @brief   This file provides all the SFM firmware functions.
 */

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x_sfm.h"
#include "wb32f10x_rcc.h"

/** @addtogroup WB32F10x_StdPeriph_Driver
  * @{
  */

/** @defgroup SFM
  * @brief SFM driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup SFM_Private_Functions
  * @{
  */

/**
 * @brief  Deinitializes the CRC and SFM peripheral registers to their default reset values.
 * @return None
 */
void CRC_SFM_DeInit(void)
{
  RCC_AHBPeriphResetCmd(RCC_AHBPeriph_CRC_SFM, ENABLE);
  RCC_AHBPeriphResetCmd(RCC_AHBPeriph_CRC_SFM, DISABLE);
}

/**
 * @brief  Returns the number of bit 1 in a word(32-bit).
 * @param  Data: The input data.
 * @return The number of bit 1 in a word(32-bit).
 */
uint32_t SFM_ComputeBit1Number(uint32_t Data)
{
  SFM->CTRL = 0x00;
  SFM->DATA = Data;
  return SFM->DOUT[0];
}

/**
 * @brief  Expand a word(32-bit) with specified rate.
 * @param  Data: The input data.
 * @param  pBuffer: The buffer to save result.
 * @param  ExpandRate: specifies the expand rate (1 ~ 8).
 * @return None.
 */
void SFM_ExpandBits(uint32_t Data, uint32_t* pBuffer, uint32_t ExpandRate)
{
  uint32_t iter;

  SFM->CTRL = (ExpandRate - 1) | SFM_CTRL_EXP_EN;
  SFM->DATA = Data;

  for (iter = 0; iter < ExpandRate; iter++)
  {
    pBuffer[iter] = SFM->DOUT[iter];
  }
}

/**
 * @brief  Configure the USB Port state detect function.
 * @param  SFM_USBP_DETECT: This parameter can be any combination of the following values:
 *         @arg SFM_USBP_DETECT_SE0
 *         @arg SFM_USBP_DETECT_JSTAT
 *         @arg SFM_USBP_DETECT_KSTAT
 *         @arg SFM_USBP_DETECT_SE1
 * @param  NewState: new state of the USB Port state detect function.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None
 */
void SFM_USBPort_DetectConfig(uint32_t SFM_USBP_DETECT, FunctionalState NewState)
{
  if (NewState != DISABLE) {
    SFM->USBPSDCSR |= SFM_USBP_DETECT;
  }
  else {
    SFM->USBPSDCSR &= ~SFM_USBP_DETECT;
  }
}

/**
 * @brief  Checks whether the specified USB Port flag is set or not.
 * @param  SFM_USBP_FLAG: specifies the flag to check.
 *    This parameter can be one of the following values:
 *      @arg SFM_USBP_FLAG_SE0
 *      @arg SFM_USBP_FLAG_JSTAT
 *      @arg SFM_USBP_FLAG_KSTAT
 *      @arg SFM_USBP_FLAG_SE1
 * @return The new state of SFM_USBP_FLAG (SET or RESET).
 */
FlagStatus SFM_USBPort_GetFlagStatus(uint32_t SFM_USBP_FLAG)
{
  FlagStatus bitstatus = RESET;

  if ((SFM->USBPSDCSR & SFM_USBP_FLAG) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

/**
 * @brief  Clears the USB Port's pending flags.
 * @param  SFM_USBP_FLAG: specifies the flag to clear.
 *    This parameter can be any combination of the following values:
 *      @arg SFM_USBP_FLAG_SE0
 *      @arg SFM_USBP_FLAG_JSTAT
 *      @arg SFM_USBP_FLAG_KSTAT
 *      @arg SFM_USBP_FLAG_SE1
 * @return None
 */
void SFM_USBPort_ClearFlag(uint32_t SFM_USBP_FLAG)
{
  SFM->USBPSDCSR &= ~SFM_USBP_FLAG;
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

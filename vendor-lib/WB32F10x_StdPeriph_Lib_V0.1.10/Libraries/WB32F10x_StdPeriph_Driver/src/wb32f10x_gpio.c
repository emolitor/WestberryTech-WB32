/**
 * @file    wb32f10x_gpio.c
 * @author  Westberry Application Team
 * @version V0.1.10
 * @date    05-January-2023
 * @brief   This file provides all the GPIO firmware functions.
 */

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x_gpio.h"
#include "wb32f10x_rcc.h"

/** @addtogroup WB32F10x_StdPeriph_Driver
  * @{
  */

/** @defgroup GPIO
  * @brief GPIO driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup GPIO_Private_Functions
  * @{
  */

/**
 * @brief  De-initializes the GPIOx peripheral registers to their default reset values.
 * @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
 * @return None
 */
void GPIO_DeInit(GPIO_TypeDef* GPIOx)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

  if (GPIOx == GPIOA)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_GPIOA, DISABLE);
  }
  else if (GPIOx == GPIOB)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_GPIOB, DISABLE);
  }
  else if (GPIOx == GPIOC)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_GPIOC, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_GPIOC, DISABLE);
  }
  else if (GPIOx == GPIOD)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_GPIOD, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_GPIOD, DISABLE);
  }
}

/**
 * @brief  Initializes the GPIOx peripheral according to the PinConfig.
 * @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
 * @param  GPIO_Pin: specifies the port bit to be initialized.
 *         This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
 * @param  PinConfig: the port bit configuration.
 *         This parameter must be a combination of one GPIO_MODE_x flag, one GPIO_OTYPE_x flag,
 *         one GPIO_PUPD_x flag and one GPIO_SPEED_x flag.
 * @return None
 */
void GPIO_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t PinConfig)
{
  uint32_t tmp = PinConfig;
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));

  GPIOx->CFGMSK = ~GPIO_Pin;
  GPIOx->MODER = ((tmp >> 28) & 0x3) * 0x55555555U;
  GPIOx->OTYPER = ((tmp >> 24) & 0x1) * 0xFFFFFFFFU;
  GPIOx->OSPEEDR = ((tmp >> 20) & 0x3) * 0x55555555U;
  GPIOx->PUPDR = ((tmp >> 16) & 0x3) * 0x55555555U;

  tmp = (tmp & 0xF) * 0x11111111U;
  GPIOx->AFRL = tmp;
  GPIOx->AFRH = tmp;
}

/**
 * @brief  Configure gpio alternate function of a dedicated pin from 0 to 7 for a dedicated port.
 * @note   Possible values are from AF0 to AF15 depending on target.
 * @note   Warning: only one pin can be passed as parameter.
 * @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
 * @param  GPIO_Pin: specifies the port bit to configure.
 *         This parameter can be GPIO_Pin_x where x can be (0..7).
 * @param  Alternate: This parameter can be GPIO_AFx where x can be (0..15).
 * @return None
 */
void GPIO_SetAFPin_0_7(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t Alternate)
{
  GPIOx->CFGMSK = ~GPIO_Pin;
  GPIOx->AFRL = Alternate * 0x11111111U;
}

/**
 * @brief  Configure gpio alternate function of a dedicated pin from 8 to 15 for a dedicated port.
 * @note   Possible values are from AF0 to AF15 depending on target.
 * @note   Warning: only one pin can be passed as parameter.
 * @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
 * @param  GPIO_Pin: specifies the port bit to configure.
 *         This parameter can be GPIO_Pin_x where x can be (8..15).
 * @param  Alternate: This parameter can be GPIO_AFx where x can be (0..15).
 * @return None
 */
void GPIO_SetAFPin_8_15(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t Alternate)
{
  GPIOx->CFGMSK = ~GPIO_Pin;
  GPIOx->AFRH = Alternate * 0x11111111U;
}

/**
 * @brief  Configure gpio alternate function of any pins for a dedicated port.
 * @note   Possible values are from AF0 to AF15 depending on target.
 * @note   Warning: only one pin can be passed as parameter.
 * @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
 * @param  GPIO_Pin: specifies the port bits to configure.
 *         This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
 * @param  Alternate: This parameter can be GPIO_AFx where x can be (0..15).
 * @return None
 */
void GPIO_SetAFPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t Alternate)
{
  uint32_t tmp = Alternate * 0x11111111U;
  GPIOx->CFGMSK = ~GPIO_Pin;
  GPIOx->AFRL = tmp;
  GPIOx->AFRH = tmp;
}

/**
 * @brief  Reads the specified input port pin.
 * @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
 * @param  GPIO_Pin: specifies the port bit to read.
 *         This parameter can be GPIO_Pin_x where x can be (0..15).
 * @return The input port pin value.
 */
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  uint8_t bitstatus = 0x00;

  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GET_GPIO_PIN(GPIO_Pin));

  if ((GPIOx->IDR & GPIO_Pin) != (uint32_t)Bit_RESET)
  {
    bitstatus = (uint8_t)Bit_SET;
  }
  else
  {
    bitstatus = (uint8_t)Bit_RESET;
  }
  return bitstatus;
}

/**
 * @brief  Reads the specified GPIO input data port.
 * @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
 * @return GPIO input data port value.
 */
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

  return ((uint16_t)GPIOx->IDR);
}

/**
  * @brief  Reads the specified output data port bit.
  * @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
  * @param  GPIO_Pin: specifies the port bit to read.
  *         This parameter can be GPIO_Pin_x where x can be (0..15).
  * @return The output port pin value.
  */
uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  uint8_t bitstatus = 0x00;

  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GET_GPIO_PIN(GPIO_Pin));

  if (((GPIOx->ODR) & GPIO_Pin) != (uint32_t)Bit_RESET)
  {
    bitstatus = (uint8_t)Bit_SET;
  }
  else
  {
    bitstatus = (uint8_t)Bit_RESET;
  }
  return bitstatus;
}

/**
 * @brief  Reads the specified GPIO output data port.
 * @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
 * @return GPIO output data port value.
 */
uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

  return ((uint16_t)GPIOx->ODR);
}

/**
 * @brief  Sets the selected data port bits.
 * @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
 * @param  GPIO_Pin: specifies the port bits to be written.
 *         This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
 * @return None
 */
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));

  GPIOx->BSRR = GPIO_Pin;
}

/**
 * @brief  Clears the selected data port bits.
 * @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
 * @param  GPIO_Pin: specifies the port bits to be written.
 *         This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
 * @return None
 */
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));

  GPIOx->BSRR = (GPIO_Pin << 16);
}

/**
 * @brief  Sets or clears the selected data port bit.
 * @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
 * @param  GPIO_Pin: specifies the port bit to be written.
 *         This parameter can be one of GPIO_Pin_x where x can be (0..15).
 * @param  BitVal: specifies the value to be written to the selected bit.
 *         This parameter can be one of the BitAction enum values:
 *         @arg Bit_RESET: to clear the port pin
 *         @arg Bit_SET: to set the port pin
 * @return None
 */
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GET_GPIO_PIN(GPIO_Pin));
  assert_param(IS_GPIO_BIT_ACTION(BitVal));

  if (BitVal != Bit_RESET)
  {
    GPIOx->BSRR = GPIO_Pin;
  }
  else
  {
    GPIOx->BSRR = (GPIO_Pin << 16);
  }
}

/**
 * @brief  Writes data to the specified GPIO data port.
 * @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
 * @param  PortVal: specifies the value to be written to the port output data register.
 * @return None
 */
void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

  GPIOx->ODR = PortVal;
}

/**
 * @brief  Toggle data value for several pin of specified port.
 * @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
 * @param  GPIO_Pin: specifies the pins to be toggled.
 *         This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
 * @return None
 */
void GPIO_ToggleBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

  GPIOx->ODR ^= GPIO_Pin;
}

/**
 * @brief  Lock configuration of several pins for a specified port.
 * @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
 * @param  GPIO_Pin: specifies the pins to be locked.
 *         This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
 * @return None
 * @note   When the lock sequence has been applied on a port bit, the
 *         value of this port bit can no longer be modified until the
 *         next reset.
 */
void GPIO_LockPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  uint32_t tmp = GPIO_LCKR_LCKK;

  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));

  tmp |= GPIO_Pin;
  /* Set LCKK bit */
  GPIOx->LCKR = tmp;
  /* Reset LCKK bit */
  GPIOx->LCKR = GPIO_Pin;
  /* Set LCKK bit */
  GPIOx->LCKR = tmp;
  /* Read LCKK bit*/
  (void) GPIOx->LCKR;
}

/**
 * @brief  Return 1 if all pins passed as parameter, of a specified port, are locked. else Return 0.
 * @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
 * @param  GPIO_Pin: specifies the pins to check lock.
 *         This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
 * @return State of bit (1 or 0).
 */
uint32_t GPIO_IsPinLocked(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  return ((GPIOx->LCKR & GPIO_Pin) == GPIO_Pin);
}

/**
 * @brief  Enables or disables the schmitt trigger of several pins for a specified port.
 * @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
 * @param  GPIO_Pin: specifies the pins to enable/disable schmitt trigger.
 *         This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
 * @param  NewState: new state of the schmitt trigger.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None
 */
void GPIO_SchmittTriggerCmd(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, FunctionalState NewState)
{
  GPIOx->CFGMSK = ~GPIO_Pin;

  if (NewState != DISABLE) {
    GPIOx->SMIT = 0xFFFF;
  }
  else {
    GPIOx->SMIT = 0x0000;
  }
}

/**
 * @brief  Configure the drive current of several pins for a specified port.
 * @param  GPIOx: where x can be (A..D) to select the GPIO peripheral.
 * @param  GPIO_Pin: specifies the pins to be configured.
 *         This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
 * @param  Current: specifies the drive current.
 * @return None
 */
void GPIO_DriveCurrentConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t Current)
{
  GPIOx->CFGMSK = ~GPIO_Pin;
  GPIOx->CURRENT = (Current & 0x3) * 0x55555555U;
}

/**
 * @brief  Selects the GPIO pin used as EXTI Line.
 * @param  GPIO_PortSource: selects the GPIO port to be used as source for EXTI lines.
 *         This parameter can be GPIO_PortSourceGPIOx where x can be (A..D).
 * @param  GPIO_PinSource: specifies the EXTI line to be configured.
 *         This parameter can be GPIO_PinSourcex where x can be (0..15).
 * @return None
 */
void GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource)
{
  uint32_t tmp = 0x00;
  
  tmp = ((uint32_t)0x0F) << (0x04 * (GPIO_PinSource & (uint8_t)0x03));
  AFIO->EXTICR[GPIO_PinSource >> 0x02] &= ~tmp;
  AFIO->EXTICR[GPIO_PinSource >> 0x02] |= (((uint32_t)GPIO_PortSource) << (0x04 * (GPIO_PinSource & (uint8_t)0x03)));
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

/**
 * @file    wb32f10x_gpio.h
 * @author  Westberry Application Team
 * @version V0.1.10
 * @date    05-January-2023
 * @brief   This file contains all the functions prototypes for the GPIO firmware
 *          library.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WB32F10x_GPIO_H
#define __WB32F10x_GPIO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/** @addtogroup WB32F10x_StdPeriph_Driver
  * @{
  */

/** @addtogroup GPIO
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/** @defgroup GPIO_Exported_Constants
  * @{
  */

#define IS_GPIO_ALL_PERIPH(PERIPH) (((PERIPH) == GPIOA) || \
                                    ((PERIPH) == GPIOB) || \
                                    ((PERIPH) == GPIOC) || \
                                    ((PERIPH) == GPIOD))

/** 
  * @brief  GPIO Bit SET and Bit RESET enumeration 
  */
typedef enum
{
  Bit_RESET = 0,
  Bit_SET
} BitAction;
#define IS_GPIO_BIT_ACTION(ACTION) (((ACTION) == Bit_RESET) || ((ACTION) == Bit_SET))


/** @defgroup GPIO_Pins_define 
  * @{
  */
#define GPIO_Pin_0                  ((uint16_t)0x0001)    /*!< Pin 0    */
#define GPIO_Pin_1                  ((uint16_t)0x0002)    /*!< Pin 1    */
#define GPIO_Pin_2                  ((uint16_t)0x0004)    /*!< Pin 2    */
#define GPIO_Pin_3                  ((uint16_t)0x0008)    /*!< Pin 3    */
#define GPIO_Pin_4                  ((uint16_t)0x0010)    /*!< Pin 4    */
#define GPIO_Pin_5                  ((uint16_t)0x0020)    /*!< Pin 5    */
#define GPIO_Pin_6                  ((uint16_t)0x0040)    /*!< Pin 6    */
#define GPIO_Pin_7                  ((uint16_t)0x0080)    /*!< Pin 7    */
#define GPIO_Pin_8                  ((uint16_t)0x0100)    /*!< Pin 8    */
#define GPIO_Pin_9                  ((uint16_t)0x0200)    /*!< Pin 9    */
#define GPIO_Pin_10                 ((uint16_t)0x0400)    /*!< Pin 10   */
#define GPIO_Pin_11                 ((uint16_t)0x0800)    /*!< Pin 11   */
#define GPIO_Pin_12                 ((uint16_t)0x1000)    /*!< Pin 12   */
#define GPIO_Pin_13                 ((uint16_t)0x2000)    /*!< Pin 13   */
#define GPIO_Pin_14                 ((uint16_t)0x4000)    /*!< Pin 14   */
#define GPIO_Pin_15                 ((uint16_t)0x8000)    /*!< Pin 15   */
#define GPIO_Pin_All                ((uint16_t)0xFFFF)    /*!< All pins */

#define IS_GPIO_PIN(PIN)    (((PIN) & 0x0000FFFF) != (uint32_t)0x00)
#define IS_GET_GPIO_PIN(PIN) (((PIN) == GPIO_Pin_0) || \
                              ((PIN) == GPIO_Pin_1) || \
                              ((PIN) == GPIO_Pin_2) || \
                              ((PIN) == GPIO_Pin_3) || \
                              ((PIN) == GPIO_Pin_4) || \
                              ((PIN) == GPIO_Pin_5) || \
                              ((PIN) == GPIO_Pin_6) || \
                              ((PIN) == GPIO_Pin_7) || \
                              ((PIN) == GPIO_Pin_8) || \
                              ((PIN) == GPIO_Pin_9) || \
                              ((PIN) == GPIO_Pin_10) || \
                              ((PIN) == GPIO_Pin_11) || \
                              ((PIN) == GPIO_Pin_12) || \
                              ((PIN) == GPIO_Pin_13) || \
                              ((PIN) == GPIO_Pin_14) || \
                              ((PIN) == GPIO_Pin_15))
/**
  * @}
  */

/** @defgroup GPIO_Port_Sources 
  * @{
  */
#define GPIO_PortSourceGPIOA       ((uint8_t)0x00)
#define GPIO_PortSourceGPIOB       ((uint8_t)0x01)
#define GPIO_PortSourceGPIOC       ((uint8_t)0x02)
#define GPIO_PortSourceGPIOD       ((uint8_t)0x03)
/**
  * @}
  */

/** @defgroup GPIO_Pin_sources 
  * @{
  */
#define GPIO_PinSource0            ((uint8_t)0x00)
#define GPIO_PinSource1            ((uint8_t)0x01)
#define GPIO_PinSource2            ((uint8_t)0x02)
#define GPIO_PinSource3            ((uint8_t)0x03)
#define GPIO_PinSource4            ((uint8_t)0x04)
#define GPIO_PinSource5            ((uint8_t)0x05)
#define GPIO_PinSource6            ((uint8_t)0x06)
#define GPIO_PinSource7            ((uint8_t)0x07)
#define GPIO_PinSource8            ((uint8_t)0x08)
#define GPIO_PinSource9            ((uint8_t)0x09)
#define GPIO_PinSource10           ((uint8_t)0x0A)
#define GPIO_PinSource11           ((uint8_t)0x0B)
#define GPIO_PinSource12           ((uint8_t)0x0C)
#define GPIO_PinSource13           ((uint8_t)0x0D)
#define GPIO_PinSource14           ((uint8_t)0x0E)
#define GPIO_PinSource15           ((uint8_t)0x0F)

#define IS_GPIO_PIN_SOURCE(PINSOURCE) (((PINSOURCE) == GPIO_PinSource0) || \
                                       ((PINSOURCE) == GPIO_PinSource1) || \
                                       ((PINSOURCE) == GPIO_PinSource2) || \
                                       ((PINSOURCE) == GPIO_PinSource3) || \
                                       ((PINSOURCE) == GPIO_PinSource4) || \
                                       ((PINSOURCE) == GPIO_PinSource5) || \
                                       ((PINSOURCE) == GPIO_PinSource6) || \
                                       ((PINSOURCE) == GPIO_PinSource7) || \
                                       ((PINSOURCE) == GPIO_PinSource8) || \
                                       ((PINSOURCE) == GPIO_PinSource9) || \
                                       ((PINSOURCE) == GPIO_PinSource10) || \
                                       ((PINSOURCE) == GPIO_PinSource11) || \
                                       ((PINSOURCE) == GPIO_PinSource12) || \
                                       ((PINSOURCE) == GPIO_PinSource13) || \
                                       ((PINSOURCE) == GPIO_PinSource14) || \
                                       ((PINSOURCE) == GPIO_PinSource15))
/**
  * @}
  */

/** @defgroup GPIO_MODE_define
  * @{
  */
#define GPIO_MODE_IN          0x00000000U     /*!< Input mode */
#define GPIO_MODE_OUT         0x10000000U     /*!< Output mode */
#define GPIO_MODE_AF          0x20000000U     /*!< Alternate function mode */
#define GPIO_MODE_ANA         0x30000000U     /*!< Analog mode */
/**
  * @}
  */

/** @defgroup GPIO_OTYPE_define
  * @{
  */
#define GPIO_OTYPE_PP         0x00000000U     /*!< Output push-pull */
#define GPIO_OTYPE_OD         0x01000000U     /*!< Output open-drain */
/**
  * @}
  */

/** @defgroup GPIO_SPEED_define
  * @{
  */
#define GPIO_SPEED_LOW        0x00100000U     /*!< Low speed */
#define GPIO_SPEED_HIGH       0x00000000U     /*!< High speed */
/**
  * @}
  */

/** @defgroup GPIO_PUPD_define
  * @{
  */
#define GPIO_PUPD_NOPULL      0x00000000U     /*!< No pull resistor */
#define GPIO_PUPD_UP          0x00010000U     /*!< Pull up resistor enabled */
#define GPIO_PUPD_DOWN        0x00020000U     /*!< Pull down resistor enabled */
/**
  * @}
  */

/** @defgroup GPIO_AF_define
  * @{
  */
#define GPIO_AF0      0
#define GPIO_AF1      1
#define GPIO_AF2      2
#define GPIO_AF3      3
#define GPIO_AF4      4
#define GPIO_AF5      5
#define GPIO_AF6      6
#define GPIO_AF7      7
#define GPIO_AF8      8
#define GPIO_AF9      9
#define GPIO_AF10     10
#define GPIO_AF11     11
#define GPIO_AF12     12
#define GPIO_AF13     13
#define GPIO_AF14     14
#define GPIO_AF15     15
/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void GPIO_DeInit(GPIO_TypeDef* GPIOx);
void GPIO_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t PinConfig);
void GPIO_SetAFPin_0_7(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t Alternate);
void GPIO_SetAFPin_8_15(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t Alternate);
void GPIO_SetAFPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t Alternate);
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);
uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal);
void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal);
void GPIO_ToggleBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_LockPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint32_t GPIO_IsPinLocked(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_SchmittTriggerCmd(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, FunctionalState NewState);
void GPIO_DriveCurrentConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t Current);
void GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __WB32F10x_GPIO_H */

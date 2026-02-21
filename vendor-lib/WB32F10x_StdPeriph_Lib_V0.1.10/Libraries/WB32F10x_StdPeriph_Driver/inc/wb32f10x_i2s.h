/**
 * @file    wb32f10x_i2s.h
 * @author  Westberry Application Team
 * @version V0.1.10
 * @date    05-January-2023
 * @brief   This file contains all the functions prototypes for the I2S firmware
 *          library.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WB32F10x_I2S_H
#define __WB32F10x_I2S_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/** @addtogroup WB32F10x_StdPeriph_Driver
  * @{
  */

/** @addtogroup I2S
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/** @defgroup I2S_Exported_Constants
  * @{
  */

#define I2S_WordSelectSize_16   I2S_CCR_WSS_16
#define I2S_WordSelectSize_24   I2S_CCR_WSS_24
#define I2S_WordSelectSize_32   I2S_CCR_WSS_32

#define I2S_SclkGate_None       I2S_CCR_SCLKG_NONE
#define I2S_SclkGate_12         I2S_CCR_SCLKG_12
#define I2S_SclkGate_16         I2S_CCR_SCLKG_16
#define I2S_SclkGate_20         I2S_CCR_SCLKG_20
#define I2S_SclkGate_24         I2S_CCR_SCLKG_24




#define I2S_IT_RXDA   (0x01)
#define I2S_IT_RXFO   (0x02)
#define I2S_IT_TXFE   (0x10)
#define I2S_IT_TXFO   (0x20)


#define I2S_DataFormat_Ignore   ((uint32_t)0x00000000)
#define I2S_DataFormat_12b      ((uint32_t)0x00000001)
#define I2S_DataFormat_16b      ((uint32_t)0x00000002)
#define I2S_DataFormat_20b      ((uint32_t)0x00000003)
#define I2S_DataFormat_24b      ((uint32_t)0x00000004)
#define I2S_DataFormat_32b      ((uint32_t)0x00000005)

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void I2S_DeInit(void);
void I2S_Cmd(FunctionalState NewState);

void I2S_ReceiverCmd(FunctionalState NewState);
void I2S_TransmitterCmd(FunctionalState NewState);

void I2S_ClockConfig(uint32_t I2S_WordSelectSize, uint32_t I2S_SclkGate);
void I2S_ClockCmd(FunctionalState NewState);

void I2S_ReceiverFIFOFlush(void);
void I2S_TransmitterFIFOFlush(void);


uint32_t I2S_Channel_ReadLeftData(uint32_t I2S_Channel);
uint32_t I2S_Channel_ReadRightData(uint32_t I2S_Channel);

void I2S_Channel_WriteLeftData(uint32_t I2S_Channel, uint32_t LeftData);
void I2S_Channel_WriteRightData(uint32_t I2S_Channel, uint32_t RightData);

void I2S_Channel_ReceiveCmd(uint32_t I2S_Channel, FunctionalState NewState);
void I2S_Channel_TransmitCmd(uint32_t I2S_Channel, FunctionalState NewState);

void I2S_Channel_ReceiveConfig(uint32_t I2S_Channel, uint32_t I2S_DataFormat);
void I2S_Channel_TransmitConfig(uint32_t I2S_Channel, uint32_t I2S_DataFormat);

void I2S_Channel_ITConfig(uint32_t I2S_Channel, uint8_t I2S_IT, FunctionalState NewState);
ITStatus I2S_Channel_GetITStatus(uint32_t I2S_Channel, uint8_t I2S_IT);
void I2S_Channel_ClearITPendingBit(uint32_t I2S_Channel, uint8_t I2S_IT);

void I2S_Channel_ReceiveFIFOConfig(uint32_t I2S_Channel, uint8_t Threshold);
void I2S_Channel_TransmitFIFOConfig(uint32_t I2S_Channel, uint8_t Threshold);

void I2S_Channel_ReceiveFIFOFlush(uint32_t I2S_Channel);
void I2S_Channel_TransmitFIFOFlush(uint32_t I2S_Channel);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __WB32F10x_I2S_H */

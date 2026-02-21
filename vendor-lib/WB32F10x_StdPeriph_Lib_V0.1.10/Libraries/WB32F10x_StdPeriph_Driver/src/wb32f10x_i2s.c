/**
 * @file    wb32f10x_i2s.c
 * @author  Westberry Application Team
 * @version V0.1.10
 * @date    05-January-2023
 * @brief   This file provides all the I2S firmware functions.
 */

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x_i2s.h"
#include "wb32f10x_rcc.h"

/** @addtogroup WB32F10x_StdPeriph_Driver
  * @{
  */

/** @defgroup I2S
  * @brief I2S driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup I2S_Private_Functions
  * @{
  */

/**
 * @brief  Deinitializes the I2S peripheral registers to their default reset values.
 * @return None
 */
void I2S_DeInit(void)
{
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_I2S, ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_I2S, DISABLE);
}

/**
 * @brief  Enables or disables the specified I2S peripheral.
 * @param  NewState: new state of the I2S peripheral.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None
 */
void I2S_Cmd(FunctionalState NewState)
{
  if (NewState != DISABLE) {
    I2S->IER = 0x01;
  }
  else {
    I2S->IER = 0x00;
  }
}

/**
 * @brief  Enables or disables the I2S receiver block.
 * @param  NewState: new state of the receiver block.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None
 */
void I2S_ReceiverCmd(FunctionalState NewState)
{
  if (NewState != DISABLE) {
    I2S->IRER = 0x01;
  }
  else {
    I2S->IRER = 0x00;
  }
}

/**
 * @brief  Enables or disables the I2S transmitter block.
 * @param  NewState: new state of the transmitter block.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None
 */
void I2S_TransmitterCmd(FunctionalState NewState)
{
  if (NewState != DISABLE) {
    I2S->ITER = 0x01;
  }
  else {
    I2S->ITER = 0x00;
  }
}

/**
 * @brief  Configures I2S Clock Generation block.
 * @param  I2S_WordSelectSize: specifies the number of sclk cycles for which the 
 *         word select line stays in the left or right sample mode.
 *          This parameter can be one of the following values:
 *           @arg @ref I2S_WordSelectSize_16
 *           @arg @ref I2S_WordSelectSize_24
 *           @arg @ref I2S_WordSelectSize_32
 * @param  I2S_SclkGate: specifies the gating of sclk.
 *          This parameter can be one of the following values:
 *           @arg @ref I2S_SclkGate_None
 *           @arg @ref I2S_SclkGate_12
 *           @arg @ref I2S_SclkGate_16
 *           @arg @ref I2S_SclkGate_20
 *           @arg @ref I2S_SclkGate_24
 * @return None
 */
void I2S_ClockConfig(uint32_t I2S_WordSelectSize, uint32_t I2S_SclkGate)
{
  I2S->CCR = I2S_WordSelectSize | I2S_SclkGate;
}

/**
 * @brief  Enables or disables the I2S clock generation block.
 * @param  NewState: new state of the clock generation block.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None
 */
void I2S_ClockCmd(FunctionalState NewState)
{
  if (NewState != DISABLE) {
    I2S->CER = 0x01;
  }
  else {
    I2S->CER = 0x00;
  }
}

/**
 * @brief  Flushes all the RX FIFOs.
 * @return None
 */
void I2S_ReceiverFIFOFlush(void)
{
  I2S->RXFFR = 0x01;
}

/**
 * @brief  Flushes all the TX FIFOs.
 * @return None
 */
void I2S_TransmitterFIFOFlush(void)
{
  I2S->TXFFR = 0x01;
}

/**
 * @brief  Read the left stereo data from Rx FIFO.
 * @param  I2S_Channel: specifies the selected I2S channel.
 * @return The left stereo data.
 */
uint32_t I2S_Channel_ReadLeftData(uint32_t I2S_Channel)
{
  return I2S->Ch[I2S_Channel].LRBR;
}

/**
 * @brief  Read the right stereo data from Rx FIFO.
 * @param  I2S_Channel: specifies the selected I2S channel.
 * @return The right stereo data.
 */
uint32_t I2S_Channel_ReadRightData(uint32_t I2S_Channel)
{
  return I2S->Ch[I2S_Channel].RRBR;
}

/**
 * @brief  Write the left stereo data to Tx FIFO.
 * @param  I2S_Channel: specifies the selected I2S channel.
 * @param  LeftData: The left stereo data to write.
 * @return None
 */
void I2S_Channel_WriteLeftData(uint32_t I2S_Channel, uint32_t LeftData)
{
  I2S->Ch[I2S_Channel].LTHR = LeftData;
}

/**
 * @brief  Write the right stereo data to Tx FIFO.
 * @param  I2S_Channel: specifies the selected I2S channel.
 * @param  RightData: The right stereo data to write.
 * @return None
 */
void I2S_Channel_WriteRightData(uint32_t I2S_Channel, uint32_t RightData)
{
  I2S->Ch[I2S_Channel].RTHR = RightData;
}

/**
 * @brief  Enables or disables the receive channel.
 * @param  I2S_Channel: specifies the selected I2S channel.
 * @param  NewState: new state of the receive channel.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None
 */
void I2S_Channel_ReceiveCmd(uint32_t I2S_Channel, FunctionalState NewState)
{
  if (NewState != DISABLE) {
    I2S->Ch[I2S_Channel].RER = 0x01;
  }
  else {
    I2S->Ch[I2S_Channel].RER = 0x00;
  }
}

/**
 * @brief  Enables or disables the transmit channel.
 * @param  I2S_Channel: specifies the selected I2S channel.
 * @param  NewState: new state of the transmit channel.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None
 */
void I2S_Channel_TransmitCmd(uint32_t I2S_Channel, FunctionalState NewState)
{
  if (NewState != DISABLE) {
    I2S->Ch[I2S_Channel].TER = 0x01;
  }
  else {
    I2S->Ch[I2S_Channel].TER = 0x00;
  }
}

/**
 * @brief  Configures the desired data resolution of the receiver.
 * @param  I2S_Channel: specifies the selected I2S channel.
 * @param  I2S_DataFormat: specifies the desired data resolution.
 *          This parameter can be one of the following values:
 *           @arg @ref I2S_DataFormat_Ignore
 *           @arg @ref I2S_DataFormat_12b
 *           @arg @ref I2S_DataFormat_16b
 *           @arg @ref I2S_DataFormat_20b
 *           @arg @ref I2S_DataFormat_24b
 *           @arg @ref I2S_DataFormat_32b
 * @return None
 */
void I2S_Channel_ReceiveConfig(uint32_t I2S_Channel, uint32_t I2S_DataFormat)
{
  I2S->Ch[I2S_Channel].RCR = I2S_DataFormat;
}

/**
 * @brief  Configures the data resolution of the transmitter.
 * @param  I2S_Channel: specifies the selected I2S channel.
 * @param  I2S_DataFormat: specifies the data resolution.
 *          This parameter can be one of the following values:
 *           @arg @ref I2S_DataFormat_Ignore
 *           @arg @ref I2S_DataFormat_12b
 *           @arg @ref I2S_DataFormat_16b
 *           @arg @ref I2S_DataFormat_20b
 *           @arg @ref I2S_DataFormat_24b
 *           @arg @ref I2S_DataFormat_32b
 * @return None
 */
void I2S_Channel_TransmitConfig(uint32_t I2S_Channel, uint32_t I2S_DataFormat)
{
  I2S->Ch[I2S_Channel].TCR = I2S_DataFormat;
}

/**
 * @brief  Enables or disables the specified I2S interrupts.
 * @param  I2S_Channel: specifies the selected I2S channel.
 * @param  I2S_IT: specifies the I2S interrupt source to be enabled or disabled. 
 *         This parameter can be a combination of the following values:
 *         @arg @ref I2S_IT_RXDA: RX FIFO Data Available interrupt.
 *         @arg @ref I2S_IT_RXFO: RX FIFO Overrun interrupt.
 *         @arg @ref I2S_IT_TXFE: TX FIFO Empty interrupt.
 *         @arg @ref I2S_IT_TXFO: TX FIFO Overrun interrupt.
 * @param  NewState: new state of the specified I2S interrupt.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None.
 */
void I2S_Channel_ITConfig(uint32_t I2S_Channel, uint8_t I2S_IT, FunctionalState NewState)
{
  if (NewState != DISABLE) {
    I2S->Ch[I2S_Channel].IMR &= ~I2S_IT;
  }
  else {
    I2S->Ch[I2S_Channel].IMR |= I2S_IT;
  }
}

/**
 * @brief  Checks whether the specified I2S interrupt has occurred or not.
 * @param  I2S_Channel: specifies the selected I2S channel.
 * @param  I2S_IT: specifies the I2S interrupt source to check.
 *         This parameter can be one of the following values:
 *         @arg @ref I2S_IT_RXDA: RX FIFO Data Available interrupt.
 *         @arg @ref I2S_IT_RXFO: RX FIFO Overrun interrupt.
 *         @arg @ref I2S_IT_TXFE: TX FIFO Empty interrupt.
 *         @arg @ref I2S_IT_TXFO: TX FIFO Overrun interrupt.
 * @return The new state of I2S_IT (SET or RESET).
 */
ITStatus I2S_Channel_GetITStatus(uint32_t I2S_Channel, uint8_t I2S_IT)
{
  ITStatus bitstatus = RESET;

  if (I2S->Ch[I2S_Channel].ISR & I2S_IT) {
    bitstatus = SET;
  }
  else {
    bitstatus = RESET;
  }
  return bitstatus;
}

/**
 * @brief  Clears the I2S interrupt pending bits.
 * @param  I2S_Channel: specifies the selected I2S channel.
 * @param  I2S_IT: specifies the interrupt pending bit to clear.
 *         This parameter can be a combination of the following values:
 *         @arg @ref I2S_IT_RXFO: RX FIFO Overrun interrupt.
 *         @arg @ref I2S_IT_TXFO: TX FIFO Overrun interrupt.
 * @return None.
 */
void I2S_Channel_ClearITPendingBit(uint32_t I2S_Channel, uint8_t I2S_IT)
{
  if (I2S_IT & I2S_IT_RXFO) {
    (void)I2S->Ch[I2S_Channel].ROR;
  }

  if (I2S_IT & I2S_IT_TXFO) {
    (void)I2S->Ch[I2S_Channel].TOR;
  }
}

/**
 * @brief  Configures the Rx FIFO threshold.
 * @param  I2S_Channel: specifies the selected I2S channel.
 * @param  Threshold: The Rx FIFO threshold to set.
 * @note   The RX FIFO Data Available interrupt will trigger when the level > Threshold.
 * @return None.
 */
void I2S_Channel_ReceiveFIFOConfig(uint32_t I2S_Channel, uint8_t Threshold)
{
  I2S->Ch[I2S_Channel].RFCR = Threshold;
}

/**
 * @brief  Configures the Tx FIFO threshold.
 * @param  I2S_Channel: specifies the selected I2S channel.
 * @param  Threshold: The Tx FIFO threshold to set.
 * @note   The TX FIFO Empty interrupt will trigger when the level <= Threshold.
 * @return None.
 */
void I2S_Channel_TransmitFIFOConfig(uint32_t I2S_Channel, uint8_t Threshold)
{
  I2S->Ch[I2S_Channel].TFCR = Threshold;
}

/**
 * @brief  Flushes the corresponding RX FIFO.
 * @param  I2S_Channel: specifies the selected I2S channel.
 * @return None
 */
void I2S_Channel_ReceiveFIFOFlush(uint32_t I2S_Channel)
{
  I2S->Ch[I2S_Channel].RFF = 0x01;
}

/**
 * @brief  Flushes the corresponding TX FIFO.
 * @param  I2S_Channel: specifies the selected I2S channel.
 * @return None
 */
void I2S_Channel_TransmitFIFOFlush(uint32_t I2S_Channel)
{
  I2S->Ch[I2S_Channel].TFF = 0x01;
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

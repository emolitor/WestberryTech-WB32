/**
 * @file    wb32f10x_i2c.c
 * @author  Westberry Application Team
 * @version V0.1.10
 * @date    05-January-2023
 * @brief   This file provides all the I2C firmware functions.
 */

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x_i2c.h"
#include "wb32f10x_rcc.h"

/** @addtogroup WB32F10x_StdPeriph_Driver
  * @{
  */

/** @defgroup I2C
  * @brief I2C driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup I2C_Private_Functions
  * @{
  */

/**
 * @brief  Deinitializes the I2Cx peripheral registers to their default reset values.
 * @param  I2Cx: Pointer to selected I2C peripheral.
 *         This parameter can be one of the following values:
 *         I2C1, I2C2.
 * @return None
 */
void I2C_DeInit(I2C_TypeDef* I2Cx)
{
  if (I2Cx == I2C1)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_I2C1, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_I2C1, DISABLE);
  }
  else if (I2Cx == I2C2)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_I2C2, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_I2C2, DISABLE);
  }
}

/**
 * @brief  Initializes the I2Cx peripheral according to the specified 
 *         parameters in the I2C_InitStruct.
 * @param  I2Cx: Pointer to selected I2C peripheral.
 *         This parameter can be one of the following values:
 *         I2C1, I2C2.
 * @param  I2C_CON: specifies the configuration for the specified 
 *         I2C peripheral.
 * @return None
 */
void I2C_Init(I2C_TypeDef* I2Cx, uint32_t I2C_CON)
{
  I2Cx->CON = I2C_CON;
}

/**
 * @brief  Configures the specified I2C target address.
 * @param  I2Cx: Pointer to selected I2C peripheral.
 *         This parameter can be one of the following values:
 *         I2C1, I2C2.
 * @param  TargetAddress: specifies the I2C target address.
 * @return None
 */
void I2C_TargetAddressConfig(I2C_TypeDef* I2Cx, uint32_t TargetAddress)
{
  I2Cx->TAR = TargetAddress;
}

/**
 * @brief  Configures the specified I2C own address.
 * @param  I2Cx: Pointer to selected I2C peripheral.
 *         This parameter can be one of the following values:
 *         I2C1, I2C2.
 * @param  Address: specifies the I2C own address.
 * @return None
 */
void I2C_OwnAddressConfig(I2C_TypeDef* I2Cx, uint16_t Address)
{
  I2Cx->SAR = Address;
}

/**
 * @brief  Configures the I2C HS mode master code.
 * @param  I2Cx: Pointer to selected I2C peripheral.
 *         This parameter can be one of the following values:
 *         I2C2.
 * @param  Code: specifies the I2C HS mode master code.
 * @return None
 */
void I2C_HighSpeedModeMasterCodeConfig(I2C_TypeDef* I2Cx, uint8_t Code)
{
  I2Cx->HS_MADDR = Code;
}

/**
 * @brief  Returns the transmit abort source for the specified I2C.
 * @param  I2Cx: Pointer to selected I2C peripheral.
 *         This parameter can be one of the following values:
 *         I2C1, I2C2.
 * @return The transmit abort source.
 */
uint32_t I2C_GetTxAbortSource(I2C_TypeDef* I2Cx)
{
  return I2Cx->TX_ABRT_SOURCE;
}

/**
 * @brief  Configures the specified I2C SCL stuck at low timeout.
 * @param  I2Cx: Pointer to selected I2C peripheral.
 *         This parameter can be one of the following values:
 *         I2C1, I2C2.
 * @param  Timeout: specifies the SCL stuck at low timeout.
 * @return None
 */
void I2C_SCLStuckAtLowTimeoutConfig(I2C_TypeDef* I2Cx, uint32_t Timeout)
{
  I2Cx->SCL_STUCK_AT_LOW_TIMEOUT = Timeout;
}

/**
 * @brief  Configures the specified I2C SDA stuck at low timeout.
 * @param  I2Cx: Pointer to selected I2C peripheral.
 *         This parameter can be one of the following values:
 *         I2C1, I2C2.
 * @param  Timeout: specifies the SDA stuck at low timeout.
 * @return None
 */
void I2C_SDAStuckAtLowTimeoutConfig(I2C_TypeDef* I2Cx, uint32_t Timeout)
{
  I2Cx->SDA_STUCK_AT_LOW_TIMEOUT = Timeout;
}

/**
 * @brief  Enables or disables the specified I2C peripheral.
 * @param  I2Cx: Pointer to selected I2C peripheral.
 *         This parameter can be one of the following values:
 *         I2C1, I2C2.
 * @param  NewState: new state of the I2Cx peripheral.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None
 */
void I2C_Cmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  if (NewState != DISABLE) {
    I2Cx->ENABLE |= I2C_ENABLE_ENABLE;
  }
  else {
    I2Cx->ENABLE &= ~I2C_ENABLE_ENABLE;
  }
}

/**
 * @brief  Enables or disables generate NACK after data byte received
 *         when as slave-receiver.
 * @param  I2Cx: Pointer to selected I2C peripheral.
 *         This parameter can be one of the following values:
 *         I2C1, I2C2.
 * @param  NewState: new state of generate NACK after data byte received.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None
 */
void I2C_SlaveDataNAckOnlyConfig(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  if (NewState != DISABLE) {
    I2Cx->SLV_DATA_NACK_ONLY = 0x01;
  }
  else {
    I2Cx->SLV_DATA_NACK_ONLY = 0x00;
  }
}

/**
 * @brief  Enables or disables the General Call ACK Generate.
 * @param  I2Cx: Pointer to selected I2C peripheral.
 *         This parameter can be one of the following values:
 *         I2C1, I2C2.
 * @param  NewState: new state of the General Call ACK Generate.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None
 */
void I2C_AckGeneralCallCmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  if (NewState != DISABLE) {
    I2Cx->ACK_GENERAL_CALL = 0x01;
  }
  else {
    I2Cx->ACK_GENERAL_CALL = 0x00;
  }
}

/**
 * @brief  Read data from Rx FIFO.
 * @param  I2Cx: Pointer to selected I2C peripheral.
 *         This parameter can be one of the following values:
 *         I2C1, I2C2.
 * @return The received data.
 */
uint16_t I2C_ReadData(I2C_TypeDef* I2Cx)
{
  return I2Cx->DATA_CMD;
}

/**
 * @brief  Write data and command Tx FIFO.
 * @param  I2Cx: Pointer to selected I2C peripheral.
 *         This parameter can be one of the following values:
 *         I2C1, I2C2.
 * @param  I2C_DataCmd: the data and command to write.
 * @return None
 */
void I2C_WriteDataCmd(I2C_TypeDef* I2Cx, uint16_t I2C_DataCmd)
{
  I2Cx->DATA_CMD = I2C_DataCmd;
}

/**
 * @brief  Checks whether the specified I2C flag is set or not.
 * @param  I2Cx: Pointer to selected I2C peripheral.
 *         This parameter can be one of the following values:
 *         I2C1, I2C2.
 * @param  I2C_FLAG: specifies the flag to check. 
 *         This parameter can be one of the following values:
 *         @arg @ref I2C_FLAG_ACTIVITY
 *         @arg @ref I2C_FLAG_TFNF
 *         @arg @ref I2C_FLAG_TFE
 *         @arg @ref I2C_FLAG_RFNE
 *         @arg @ref I2C_FLAG_RFF
 *         @arg @ref I2C_FLAG_MST_ACTIVITY
 *         @arg @ref I2C_FLAG_SLV_ACTIVITY
 *         @arg @ref I2C_FLAG_MST_HOLD_TX_FIFO_EMPTY
 *         @arg @ref I2C_FLAG_MST_HOLD_RX_FIFO_FULL
 *         @arg @ref I2C_FLAG_SLV_HOLD_TX_FIFO_EMPTY
 *         @arg @ref I2C_FLAG_SLV_HOLD_RX_FIFO_FULL
 *         @arg @ref I2C_FLAG_SDA_STUCK_NOT_RECOVERED
 *         @arg @ref I2C_FLAG_SMBUS_QUICK_CMD_BIT
 *         @arg @ref I2C_FLAG_SMBUS_SLAVE_ADDR_VALID
 *         @arg @ref I2C_FLAG_SMBUS_SLAVE_ADDR_RESOLVED
 *         @arg @ref I2C_FLAG_SMBUS_SUSPEND_STATUS
 *         @arg @ref I2C_FLAG_SMBUS_ALERT_STATUS
 * @return The new state of I2C_FLAG (SET or RESET).
 */
FlagStatus I2C_GetFlagStatus(I2C_TypeDef* I2Cx, uint32_t I2C_FLAG)
{
  FlagStatus bitstatus = RESET;

  /* Check the status of the specified I2C flag */
  if ((I2Cx->STATUS & I2C_FLAG) != (uint32_t)RESET)
  {
    /* I2C_FLAG is set */
    bitstatus = SET;
  }
  else
  {
    /* I2C_FLAG is reset */
    bitstatus = RESET;
  }
  /* Return the I2C_FLAG status */
  return  bitstatus;
}

/**
 * @brief  Enables or disables the specified I2C interrupts.
 * @param  I2Cx: Pointer to selected I2C peripheral.
 *         This parameter can be one of the following values:
 *         I2C1, I2C2.
 * @param  I2C_IT: specifies the I2C interrupt source to be enabled or disabled. 
 *         This parameter can be a combination of the following values:
 *         @arg @ref I2C_IT_RX_UNDER
 *         @arg @ref I2C_IT_RX_OVER
 *         @arg @ref I2C_IT_RX_FULL
 *         @arg @ref I2C_IT_TX_OVER
 *         @arg @ref I2C_IT_TX_EMPTY
 *         @arg @ref I2C_IT_RD_REQ
 *         @arg @ref I2C_IT_TX_ABRT
 *         @arg @ref I2C_IT_RX_DONE
 *         @arg @ref I2C_IT_ACTIVITY
 *         @arg @ref I2C_IT_STOP_DET
 *         @arg @ref I2C_IT_START_DET
 *         @arg @ref I2C_IT_GEN_CALL
 *         @arg @ref I2C_IT_RESTART_DET
 *         @arg @ref I2C_IT_SCL_STUCK_AT_LOW
 * @param  NewState: new state of the specified I2C interrupt.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None.
 */
void I2C_ITConfig(I2C_TypeDef* I2Cx, uint16_t I2C_IT, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    /* Enable the selected I2C interrupt */
    I2Cx->INTR_MASK |= I2C_IT;
  }
  else
  {
    /* Disable the selected I2C interrupt */
    I2Cx->INTR_MASK &= ~I2C_IT;
  }
}

/**
 * @brief  Checks whether the specified I2Cx raw interrupt status.
 * @param  I2Cx: Pointer to selected I2C peripheral.
 *         This parameter can be one of the following values:
 *         I2C1, I2C2.
 * @param  I2C_IT: specifies the I2C interrupt source to check.
 *         This parameter can be one of the following values:
 *         @arg @ref I2C_IT_RX_UNDER
 *         @arg @ref I2C_IT_RX_OVER
 *         @arg @ref I2C_IT_RX_FULL
 *         @arg @ref I2C_IT_TX_OVER
 *         @arg @ref I2C_IT_TX_EMPTY
 *         @arg @ref I2C_IT_RD_REQ
 *         @arg @ref I2C_IT_TX_ABRT
 *         @arg @ref I2C_IT_RX_DONE
 *         @arg @ref I2C_IT_ACTIVITY
 *         @arg @ref I2C_IT_STOP_DET
 *         @arg @ref I2C_IT_START_DET
 *         @arg @ref I2C_IT_GEN_CALL
 *         @arg @ref I2C_IT_RESTART_DET
 *         @arg @ref I2C_IT_SCL_STUCK_AT_LOW
 * @return The new state of raw I2C_IT (SET or RESET).
 */
ITStatus I2C_GetRawITStatus(I2C_TypeDef* I2Cx, uint16_t I2C_IT)
{
  ITStatus bitstatus = RESET;

  /* Check the raw status of the specified I2C interrupt */
  if (I2Cx->RAW_INTR_STAT & I2C_IT)
  {
    /* I2C_IT is set */
    bitstatus = SET;
  }
  else
  {
    /* I2C_IT is reset */
    bitstatus = RESET;
  }
  /* Return the I2C_IT raw status */
  return bitstatus;
}

/**
 * @brief  Checks whether the specified I2Cx interrupt has occurred or not.
 * @param  I2Cx: Pointer to selected I2C peripheral.
 *         This parameter can be one of the following values:
 *         I2C1, I2C2.
 * @param  I2C_IT: specifies the I2C interrupt source to check.
 *         This parameter can be one of the following values:
 *         @arg @ref I2C_IT_RX_UNDER
 *         @arg @ref I2C_IT_RX_OVER
 *         @arg @ref I2C_IT_RX_FULL
 *         @arg @ref I2C_IT_TX_OVER
 *         @arg @ref I2C_IT_TX_EMPTY
 *         @arg @ref I2C_IT_RD_REQ
 *         @arg @ref I2C_IT_TX_ABRT
 *         @arg @ref I2C_IT_RX_DONE
 *         @arg @ref I2C_IT_ACTIVITY
 *         @arg @ref I2C_IT_STOP_DET
 *         @arg @ref I2C_IT_START_DET
 *         @arg @ref I2C_IT_GEN_CALL
 *         @arg @ref I2C_IT_RESTART_DET
 *         @arg @ref I2C_IT_SCL_STUCK_AT_LOW
 * @return The new state of I2C_IT (SET or RESET).
 */
ITStatus I2C_GetITStatus(I2C_TypeDef* I2Cx, uint16_t I2C_IT)
{
  ITStatus bitstatus = RESET;

  /* Check the status of the specified I2C interrupt */
  if (I2Cx->INTR_STAT & I2C_IT)
  {
    /* I2C_IT is set */
    bitstatus = SET;
  }
  else
  {
    /* I2C_IT is reset */
    bitstatus = RESET;
  }
  /* Return the I2C_IT status */
  return bitstatus;
}

/**
 * @brief  Clears the I2Cx's interrupt pending bits.
 * @param  I2Cx: Pointer to selected I2C peripheral.
 *         This parameter can be one of the following values:
 *         I2C1, I2C2.
 * @param  I2C_IT: specifies the interrupt pending bit to clear.
 *         This parameter can be a combination of the following values:
 *         @arg @ref I2C_IT_RX_UNDER
 *         @arg @ref I2C_IT_RX_OVER
 *         @arg @ref I2C_IT_TX_OVER
 *         @arg @ref I2C_IT_RD_REQ
 *         @arg @ref I2C_IT_TX_ABRT
 *         @arg @ref I2C_IT_RX_DONE
 *         @arg @ref I2C_IT_ACTIVITY
 *         @arg @ref I2C_IT_STOP_DET
 *         @arg @ref I2C_IT_START_DET
 *         @arg @ref I2C_IT_GEN_CALL
 *         @arg @ref I2C_IT_RESTART_DET
 *         @arg @ref I2C_IT_SCL_STUCK_AT_LOW
 * @return None.
 */
void I2C_ClearITPendingBit(I2C_TypeDef* I2Cx, uint16_t I2C_IT)
{
  if ((I2C_IT & (I2C_IT_RX_UNDER | I2C_IT_RX_OVER | I2C_IT_TX_OVER |
             I2C_IT_RD_REQ | I2C_IT_TX_ABRT | I2C_IT_RX_DONE |
             I2C_IT_ACTIVITY | I2C_IT_STOP_DET | I2C_IT_START_DET |
             I2C_IT_GEN_CALL | I2C_IT_RESTART_DET | I2C_IT_SCL_STUCK_AT_LOW)) ==
         (I2C_IT_RX_UNDER | I2C_IT_RX_OVER | I2C_IT_TX_OVER |
          I2C_IT_RD_REQ | I2C_IT_TX_ABRT | I2C_IT_RX_DONE |
          I2C_IT_ACTIVITY | I2C_IT_STOP_DET | I2C_IT_START_DET |
          I2C_IT_GEN_CALL | I2C_IT_RESTART_DET | I2C_IT_SCL_STUCK_AT_LOW))
  {
    (void)I2Cx->CLR_INTR;
  }
  else
  {
    if (I2C_IT & I2C_IT_RX_UNDER)          (void)I2Cx->CLR_RX_UNDER;
    if (I2C_IT & I2C_IT_RX_OVER)           (void)I2Cx->CLR_RX_OVER;
    if (I2C_IT & I2C_IT_TX_OVER)           (void)I2Cx->CLR_TX_OVER;
    if (I2C_IT & I2C_IT_RD_REQ)            (void)I2Cx->CLR_RD_REQ;
    if (I2C_IT & I2C_IT_TX_ABRT)           (void)I2Cx->CLR_TX_ABRT;
    if (I2C_IT & I2C_IT_RX_DONE)           (void)I2Cx->CLR_RX_DONE;
    if (I2C_IT & I2C_IT_ACTIVITY)          (void)I2Cx->CLR_ACTIVITY;
    if (I2C_IT & I2C_IT_STOP_DET)          (void)I2Cx->CLR_STOP_DET;
    if (I2C_IT & I2C_IT_START_DET)         (void)I2Cx->CLR_START_DET;
    if (I2C_IT & I2C_IT_GEN_CALL)          (void)I2Cx->CLR_GEN_CALL;
    if (I2C_IT & I2C_IT_RESTART_DET)       (void)I2Cx->CLR_RESTART_DET;
    if (I2C_IT & I2C_IT_SCL_STUCK_AT_LOW)  (void)I2Cx->CLR_SCL_STUCK_DET;
  }
}

/**
 * @brief  Configures the Receive FIFO threshold.
 * @param  I2Cx: Pointer to selected I2C peripheral.
 *         This parameter can be one of the following values:
 *         I2C1, I2C2.
 * @param  Threshold: The Receive FIFO threshold to set.
 * @param  The RX_FULL interrupt will trigger when the level > Threshold.
 * @return None.
 */
void I2C_RxFIFOThresholdConfig(I2C_TypeDef* I2Cx, uint8_t Threshold)
{
  I2Cx->RX_TL = Threshold;
}

/**
 * @brief  Configures the Transmit FIFO threshold.
 * @param  I2Cx: Pointer to selected I2C peripheral.
 *         This parameter can be one of the following values:
 *         I2C1, I2C2.
 * @param  Threshold: The Transmit FIFO threshold to set.
 * @note   The TX_EMPTY interrupt will trigger when the level <= Threshold.
 * @return None.
 */
void I2C_TxFIFOThresholdConfig(I2C_TypeDef* I2Cx, uint8_t Threshold)
{
  I2Cx->TX_TL = Threshold;
}

/**
 * @brief  Returns the number of valid data entries in the receive FIFO buffer.
 * @param  I2Cx: Pointer to selected I2C peripheral.
 *         This parameter can be one of the following values:
 *         I2C1, I2C2.
 * @return The number of valid data entries in the receive FIFO.
 */
uint8_t I2C_GetRxFIFOLevel(I2C_TypeDef* I2Cx)
{
  return I2Cx->RXFLR;
}

/**
 * @brief  Returns the number of valid data entries in the transmit FIFO buffer.
 * @param  I2Cx: Pointer to selected I2C peripheral.
 *         This parameter can be one of the following values:
 *         I2C1, I2C2.
 * @return The number of valid data entries in the transmit FIFO.
 */
uint8_t I2C_GetTxFIFOLevel(I2C_TypeDef* I2Cx)
{
  return I2Cx->TXFLR;
}

/**
 * @brief  Configures the DMA Rx request level.
 * @param  I2Cx: Pointer to selected I2C peripheral.
 *         This parameter can be one of the following values:
 *         I2C1, I2C2.
 * @param  Level: The level of the DMA Rx request to set.
 * @return None.
 */
void I2C_DMARxReqLevelConfig(I2C_TypeDef* I2Cx, uint8_t Level)
{
  I2Cx->DMA_RDLR = Level;
}

/**
 * @brief  Configures the DMA Tx request level.
 * @param  I2Cx: Pointer to selected I2C peripheral.
 *         This parameter can be one of the following values:
 *         I2C1, I2C2.
 * @param  Level: The level of the DMA Tx request to set.
 * @return None.
 */
void I2C_DMATxReqLevelConfig(I2C_TypeDef* I2Cx, uint8_t Level)
{
  I2Cx->DMA_TDLR = Level;
}

/**
 * @brief  Enables or disables the I2Cx's DMA interface.
 * @param  I2Cx: Pointer to selected I2C peripheral.
 *         This parameter can be one of the following values:
 *         I2C1, I2C2.
 * @param  I2C_DMAReq: specifies the DMA request.
 *         This parameter can be any combination of the following values:
 *         @arg I2C_DMAReq_Rx: I2C DMA receive request
 *         @arg I2C_DMAReq_Tx: I2C DMA transmit request
 * @param  NewState: new state of the DMA Request sources.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None.
 */
void I2C_DMACmd(I2C_TypeDef* I2Cx, uint32_t I2C_DMAReq, FunctionalState NewState)
{
  if (NewState != DISABLE) {
    I2Cx->DMA_CR |= I2C_DMAReq;
  }
  else {
    I2Cx->DMA_CR &= ~I2C_DMAReq;
  }
}

/**
 * @brief  Enables or disables the specified SMBus interrupts.
 * @param  I2Cx: Pointer to selected I2C peripheral.
 *         This parameter can be one of the following values:
 *         I2C1.
 * @param  I2C_SMBUS_IT: specifies the SMBus interrupt source to be enabled or disabled. 
 *         This parameter can be a combination of the following values:
 *         @arg @ref I2C_SMBUS_IT_SLV_CLOCK_EXTND_TIMEOUT
 *         @arg @ref I2C_SMBUS_IT_MST_CLOCK_EXTND_TIMEOUT
 *         @arg @ref I2C_SMBUS_IT_QUICK_CMD_DET
 *         @arg @ref I2C_SMBUS_IT_HOST_NTFY_MST_DET
 *         @arg @ref I2C_SMBUS_IT_ARP_PREPARE_CMD_DET
 *         @arg @ref I2C_SMBUS_IT_ARP_RST_CMD_DET
 *         @arg @ref I2C_SMBUS_IT_ARP_GET_UDID_CMD_DET
 *         @arg @ref I2C_SMBUS_IT_ARP_ASSGN_ADDR_CMD_DET
 *         @arg @ref I2C_SMBUS_IT_SLV_RX_PEC_NACK
 *         @arg @ref I2C_SMBUS_IT_SMBUS_SUSPEND_DET
 *         @arg @ref I2C_SMBUS_IT_SMBUS_ALERT_DET
 * @param  NewState: new state of the specified SMBus interrupt.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None.
 */
void I2C_SMBus_ITConfig(I2C_TypeDef* I2Cx, uint16_t I2C_SMBUS_IT, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    /* Enable the selected SMBus interrupt */
    I2Cx->SMBUS_INTR_MASK |= I2C_SMBUS_IT;
  }
  else
  {
    /* Disable the selected SMBus interrupt */
    I2Cx->SMBUS_INTR_MASK &= ~I2C_SMBUS_IT;
  }
}

/**
 * @brief  Checks whether the specified SMBus raw interrupt status.
 * @param  I2Cx: Pointer to selected I2C peripheral.
 *         This parameter can be one of the following values:
 *         I2C1.
 * @param  I2C_SMBUS_IT: specifies the SMBus interrupt source to check.
 *         This parameter can be one of the following values:
 *         @arg @ref I2C_SMBUS_IT_SLV_CLOCK_EXTND_TIMEOUT
 *         @arg @ref I2C_SMBUS_IT_MST_CLOCK_EXTND_TIMEOUT
 *         @arg @ref I2C_SMBUS_IT_QUICK_CMD_DET
 *         @arg @ref I2C_SMBUS_IT_HOST_NTFY_MST_DET
 *         @arg @ref I2C_SMBUS_IT_ARP_PREPARE_CMD_DET
 *         @arg @ref I2C_SMBUS_IT_ARP_RST_CMD_DET
 *         @arg @ref I2C_SMBUS_IT_ARP_GET_UDID_CMD_DET
 *         @arg @ref I2C_SMBUS_IT_ARP_ASSGN_ADDR_CMD_DET
 *         @arg @ref I2C_SMBUS_IT_SLV_RX_PEC_NACK
 *         @arg @ref I2C_SMBUS_IT_SMBUS_SUSPEND_DET
 *         @arg @ref I2C_SMBUS_IT_SMBUS_ALERT_DET
 * @return The new state of raw I2C_SMBUS_IT (SET or RESET).
 */
ITStatus I2C_SMBus_GetRawITStatus(I2C_TypeDef* I2Cx, uint16_t I2C_SMBUS_IT)
{
  ITStatus bitstatus = RESET;

  /* Check the raw status of the specified SMBus interrupt */
  if (I2Cx->SMBUS_RAW_INTR_STAT & I2C_SMBUS_IT)
  {
    /* I2C_SMBUS_IT is set */
    bitstatus = SET;
  }
  else
  {
    /* I2C_SMBUS_IT is reset */
    bitstatus = RESET;
  }
  /* Return the I2C_SMBUS_IT raw status */
  return bitstatus;
}

/**
 * @brief  Checks whether the specified SMBus interrupt has occurred or not.
 * @param  I2Cx: Pointer to selected I2C peripheral.
 *         This parameter can be one of the following values:
 *         I2C1.
 * @param  I2C_SMBUS_IT: specifies the SMBus interrupt source to check.
 *         This parameter can be one of the following values:
 *         @arg @ref I2C_SMBUS_IT_SLV_CLOCK_EXTND_TIMEOUT
 *         @arg @ref I2C_SMBUS_IT_MST_CLOCK_EXTND_TIMEOUT
 *         @arg @ref I2C_SMBUS_IT_QUICK_CMD_DET
 *         @arg @ref I2C_SMBUS_IT_HOST_NTFY_MST_DET
 *         @arg @ref I2C_SMBUS_IT_ARP_PREPARE_CMD_DET
 *         @arg @ref I2C_SMBUS_IT_ARP_RST_CMD_DET
 *         @arg @ref I2C_SMBUS_IT_ARP_GET_UDID_CMD_DET
 *         @arg @ref I2C_SMBUS_IT_ARP_ASSGN_ADDR_CMD_DET
 *         @arg @ref I2C_SMBUS_IT_SLV_RX_PEC_NACK
 *         @arg @ref I2C_SMBUS_IT_SMBUS_SUSPEND_DET
 *         @arg @ref I2C_SMBUS_IT_SMBUS_ALERT_DET
 * @return The new state of I2C_SMBUS_IT (SET or RESET).
 */
ITStatus I2C_SMBus_GetITStatus(I2C_TypeDef* I2Cx, uint16_t I2C_SMBUS_IT)
{
  ITStatus bitstatus = RESET;

  /* Check the status of the specified SMBus interrupt */
  if (I2Cx->SMBUS_INTR_STAT & I2C_SMBUS_IT)
  {
    /* I2C_SMBUS_IT is set */
    bitstatus = SET;
  }
  else
  {
    /* I2C_SMBUS_IT is reset */
    bitstatus = RESET;
  }
  /* Return the I2C_SMBUS_IT status */
  return bitstatus;
}

/**
 * @brief  Clears the I2Cx's interrupt pending bits.
 * @param  I2Cx: Pointer to selected I2C peripheral.
 *         This parameter can be one of the following values:
 *         I2C1.
 * @param  I2C_SMBUS_IT: specifies the interrupt pending bit to clear.
 *         This parameter can be a combination of the following values:
 *         @arg @ref I2C_SMBUS_IT_SLV_CLOCK_EXTND_TIMEOUT
 *         @arg @ref I2C_SMBUS_IT_MST_CLOCK_EXTND_TIMEOUT
 *         @arg @ref I2C_SMBUS_IT_QUICK_CMD_DET
 *         @arg @ref I2C_SMBUS_IT_HOST_NTFY_MST_DET
 *         @arg @ref I2C_SMBUS_IT_ARP_PREPARE_CMD_DET
 *         @arg @ref I2C_SMBUS_IT_ARP_RST_CMD_DET
 *         @arg @ref I2C_SMBUS_IT_ARP_GET_UDID_CMD_DET
 *         @arg @ref I2C_SMBUS_IT_ARP_ASSGN_ADDR_CMD_DET
 *         @arg @ref I2C_SMBUS_IT_SLV_RX_PEC_NACK
 *         @arg @ref I2C_SMBUS_IT_SMBUS_SUSPEND_DET
 *         @arg @ref I2C_SMBUS_IT_SMBUS_ALERT_DET
 * @return None.
 */
void I2C_SMBus_ClearITPendingBit(I2C_TypeDef* I2Cx, uint16_t I2C_SMBUS_IT)
{
  I2Cx->CLR_SMBUS_INTR = I2C_SMBUS_IT;
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

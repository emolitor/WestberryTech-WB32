/**
 * @file    wb32f10x_i2c.h
 * @author  Westberry Application Team
 * @version V0.1.10
 * @date    05-January-2023
 * @brief   This file contains all the functions prototypes for the I2C firmware
 *          library.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WB32F10x_I2C_H
#define __WB32F10x_I2C_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/** @addtogroup WB32F10x_StdPeriph_Driver
  * @{
  */

/** @addtogroup I2C
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/** @defgroup I2C_Exported_Constants
  * @{
  */

/** @defgroup I2C_TargetAddress_definition 
  * @{
  */
#define I2C_TARGETADDRESS_GENERAL_CALL        (I2C_TAR_SPECIAL)
#define I2C_TARGETADDRESS_START_BYTE          (I2C_TAR_SPECIAL | I2C_TAR_GC_OR_START)
#define I2C_TARGETADDRESS_DEVICE_ID           (I2C_TAR_SPECIAL | I2C_TAR_DEVICE_ID)
#define I2C_TARGETADDRESS_SMBUS_QUICK_CMD     (I2C_TAR_SPECIAL | I2C_TAR_SMBUS_QUICK_CMD)
/**
  * @}
  */


/** @defgroup I2C_flags_definition 
  * @{
  */
#define I2C_FLAG_ACTIVITY                   I2C_STATUS_ACTIVITY
#define I2C_FLAG_TFNF                       I2C_STATUS_TFNF
#define I2C_FLAG_TFE                        I2C_STATUS_TFE
#define I2C_FLAG_RFNE                       I2C_STATUS_RFNE
#define I2C_FLAG_RFF                        I2C_STATUS_RFF
#define I2C_FLAG_MST_ACTIVITY               I2C_STATUS_MST_ACTIVITY
#define I2C_FLAG_SLV_ACTIVITY               I2C_STATUS_SLV_ACTIVITY
#define I2C_FLAG_MST_HOLD_TX_FIFO_EMPTY     I2C_STATUS_MST_HOLD_TX_FIFO_EMPTY
#define I2C_FLAG_MST_HOLD_RX_FIFO_FULL      I2C_STATUS_MST_HOLD_RX_FIFO_FULL
#define I2C_FLAG_SLV_HOLD_TX_FIFO_EMPTY     I2C_STATUS_SLV_HOLD_TX_FIFO_EMPTY
#define I2C_FLAG_SLV_HOLD_RX_FIFO_FULL      I2C_STATUS_SLV_HOLD_RX_FIFO_FULL
#define I2C_FLAG_SDA_STUCK_NOT_RECOVERED    I2C_STATUS_SDA_STUCK_NOT_RECOVERED
#define I2C_FLAG_SMBUS_QUICK_CMD_BIT        I2C_STATUS_SMBUS_QUICK_CMD_BIT
#define I2C_FLAG_SMBUS_SLAVE_ADDR_VALID     I2C_STATUS_SMBUS_SLAVE_ADDR_VALID
#define I2C_FLAG_SMBUS_SLAVE_ADDR_RESOLVED  I2C_STATUS_SMBUS_SLAVE_ADDR_RESOLVED
#define I2C_FLAG_SMBUS_SUSPEND_STATUS       I2C_STATUS_SMBUS_SUSPEND_STATUS
#define I2C_FLAG_SMBUS_ALERT_STATUS         I2C_STATUS_SMBUS_ALERT_STATUS
/**
  * @}
  */


/** @defgroup I2C_interrupts_definition 
  * @{
  */
#define I2C_IT_RX_UNDER             I2C_INTR_RX_UNDER
#define I2C_IT_RX_OVER              I2C_INTR_RX_OVER
#define I2C_IT_RX_FULL              I2C_INTR_RX_FULL
#define I2C_IT_TX_OVER              I2C_INTR_TX_OVER
#define I2C_IT_TX_EMPTY             I2C_INTR_TX_EMPTY
#define I2C_IT_RD_REQ               I2C_INTR_RD_REQ
#define I2C_IT_TX_ABRT              I2C_INTR_TX_ABRT
#define I2C_IT_RX_DONE              I2C_INTR_RX_DONE
#define I2C_IT_ACTIVITY             I2C_INTR_ACTIVITY
#define I2C_IT_STOP_DET             I2C_INTR_STOP_DET
#define I2C_IT_START_DET            I2C_INTR_START_DET
#define I2C_IT_GEN_CALL             I2C_INTR_GEN_CALL
#define I2C_IT_RESTART_DET          I2C_INTR_RESTART_DET
#define I2C_IT_SCL_STUCK_AT_LOW     I2C_INTR_SCL_STUCK_AT_LOW
/**
  * @}
  */


/** @defgroup I2C_SMBus_interrupts_definition 
  * @{
  */
#define I2C_SMBUS_IT_SLV_CLOCK_EXTND_TIMEOUT      I2C_SMBUS_INTR_SLV_CLOCK_EXTND_TIMEOUT
#define I2C_SMBUS_IT_MST_CLOCK_EXTND_TIMEOUT      I2C_SMBUS_INTR_MST_CLOCK_EXTND_TIMEOUT
#define I2C_SMBUS_IT_QUICK_CMD_DET                I2C_SMBUS_INTR_QUICK_CMD_DET
#define I2C_SMBUS_IT_HOST_NTFY_MST_DET            I2C_SMBUS_INTR_HOST_NTFY_MST_DET
#define I2C_SMBUS_IT_ARP_PREPARE_CMD_DET          I2C_SMBUS_INTR_ARP_PREPARE_CMD_DET
#define I2C_SMBUS_IT_ARP_RST_CMD_DET              I2C_SMBUS_INTR_ARP_RST_CMD_DET
#define I2C_SMBUS_IT_ARP_GET_UDID_CMD_DET         I2C_SMBUS_INTR_ARP_GET_UDID_CMD_DET
#define I2C_SMBUS_IT_ARP_ASSGN_ADDR_CMD_DET       I2C_SMBUS_INTR_ARP_ASSGN_ADDR_CMD_DET
#define I2C_SMBUS_IT_SLV_RX_PEC_NACK              I2C_SMBUS_INTR_SLV_RX_PEC_NACK
#define I2C_SMBUS_IT_SMBUS_SUSPEND_DET            I2C_SMBUS_INTR_SMBUS_SUSPEND_DET
#define I2C_SMBUS_IT_SMBUS_ALERT_DET              I2C_SMBUS_INTR_SMBUS_ALERT_DET
/**
  * @}
  */


/** @defgroup I2C_DMA_Requests 
  * @{
  */
#define I2C_DMAReq_Rx             I2C_DMA_CR_RDMAE
#define I2C_DMAReq_Tx             I2C_DMA_CR_TDMAE
/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void I2C_DeInit(I2C_TypeDef* I2Cx);
void I2C_Init(I2C_TypeDef* I2Cx, uint32_t I2C_CON);
void I2C_TargetAddressConfig(I2C_TypeDef* I2Cx, uint32_t TargetAddress);
void I2C_OwnAddressConfig(I2C_TypeDef* I2Cx, uint16_t Address);
void I2C_HighSpeedModeMasterCodeConfig(I2C_TypeDef* I2Cx, uint8_t Code);
uint32_t I2C_GetTxAbortSource(I2C_TypeDef* I2Cx);
void I2C_SCLStuckAtLowTimeoutConfig(I2C_TypeDef* I2Cx, uint32_t Timeout);
void I2C_SDAStuckAtLowTimeoutConfig(I2C_TypeDef* I2Cx, uint32_t Timeout);
void I2C_Cmd(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_SlaveDataNAckOnlyConfig(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_AckGeneralCallCmd(I2C_TypeDef* I2Cx, FunctionalState NewState);
uint16_t I2C_ReadData(I2C_TypeDef* I2Cx);
void I2C_WriteDataCmd(I2C_TypeDef* I2Cx, uint16_t I2C_DataCmd);
FlagStatus I2C_GetFlagStatus(I2C_TypeDef* I2Cx, uint32_t I2C_FLAG);
void I2C_ITConfig(I2C_TypeDef* I2Cx, uint16_t I2C_IT, FunctionalState NewState);
ITStatus I2C_GetRawITStatus(I2C_TypeDef* I2Cx, uint16_t I2C_IT);
ITStatus I2C_GetITStatus(I2C_TypeDef* I2Cx, uint16_t I2C_IT);
void I2C_ClearITPendingBit(I2C_TypeDef* I2Cx, uint16_t I2C_IT);
void I2C_RxFIFOThresholdConfig(I2C_TypeDef* I2Cx, uint8_t Threshold);
void I2C_TxFIFOThresholdConfig(I2C_TypeDef* I2Cx, uint8_t Threshold);
uint8_t I2C_GetRxFIFOLevel(I2C_TypeDef* I2Cx);
uint8_t I2C_GetTxFIFOLevel(I2C_TypeDef* I2Cx);
void I2C_DMARxReqLevelConfig(I2C_TypeDef* I2Cx, uint8_t Level);
void I2C_DMATxReqLevelConfig(I2C_TypeDef* I2Cx, uint8_t Level);
void I2C_DMACmd(I2C_TypeDef* I2Cx, uint32_t I2C_DMAReq, FunctionalState NewState);
void I2C_SMBus_ITConfig(I2C_TypeDef* I2Cx, uint16_t I2C_SMBUS_IT, FunctionalState NewState);
ITStatus I2C_SMBus_GetRawITStatus(I2C_TypeDef* I2Cx, uint16_t I2C_SMBUS_IT);
ITStatus I2C_SMBus_GetITStatus(I2C_TypeDef* I2Cx, uint16_t I2C_SMBUS_IT);
void I2C_SMBus_ClearITPendingBit(I2C_TypeDef* I2Cx, uint16_t I2C_SMBUS_IT);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __WB32F10x_I2C_H */

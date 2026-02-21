/**
 * @file    wb32f10x_spi.h
 * @author  Westberry Application Team
 * @version V0.1.10
 * @date    05-January-2023
 * @brief   This file contains all the functions prototypes for the SPI firmware
 *          library.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WB32F10x_SPI_H
#define __WB32F10x_SPI_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/** @addtogroup WB32F10x_StdPeriph_Driver
  * @{
  */

/** @addtogroup SPI
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  SPI Init structure definition  
  */
typedef struct
{
  uint16_t SPI_TransferMode;
  uint16_t SPI_DataSize;
  uint16_t SPI_CPOL;
  uint16_t SPI_CPHA;
  uint16_t SPI_BaudRatePrescaler;
  uint16_t SPI_FrameFormat;
} SPI_InitTypeDef;


/** 
  * @brief  SPI Microwire Init structure definition  
  */
typedef struct
{
  uint16_t SPI_MicrowireControlFrameSize;
  uint16_t SPI_MicrowireTransferMode;
  uint16_t SPI_MicrowireDirection;
  uint16_t SPI_MicrowireHandshaking;
} SPI_MicrowireInitTypeDef;


/** 
  * @brief  SPI Enhanced SPI Mode Init structure definition  
  */
typedef struct
{
  uint16_t SPI_EnhancedSpiTransferType;
  uint16_t SPI_EnhancedSpiInstructionLength;
  uint16_t SPI_EnhancedSpiAddressLength;
  uint16_t SPI_EnhancedSpiWaitCycles;
} SPI_EnhancedSpiInitTypeDef;

/* Exported constants --------------------------------------------------------*/

/** @defgroup SPI_Exported_Constants
  * @{
  */

/** @defgroup SPI_transfer_mode 
  * @{
  */
#define SPI_TransferMode_TxAndRx        ((uint16_t)SPI_CR0_TMOD_TX_AND_RX)
#define SPI_TransferMode_TxOnly         ((uint16_t)SPI_CR0_TMOD_TX_ONLY)
#define SPI_TransferMode_RxOnly         ((uint16_t)SPI_CR0_TMOD_RX_ONLY)
#define SPI_TransferMode_EepromRead     ((uint16_t)SPI_CR0_TMOD_EEPROM_READ)
/**
  * @}
  */


/** @defgroup SPI_data_size 
  * @{
  */
#define SPI_DataSize_4b                 ((uint16_t)SPI_CR0_DFS_4BITS)
#define SPI_DataSize_5b                 ((uint16_t)SPI_CR0_DFS_5BITS)
#define SPI_DataSize_6b                 ((uint16_t)SPI_CR0_DFS_6BITS)
#define SPI_DataSize_7b                 ((uint16_t)SPI_CR0_DFS_7BITS)
#define SPI_DataSize_8b                 ((uint16_t)SPI_CR0_DFS_8BITS)
#define SPI_DataSize_9b                 ((uint16_t)SPI_CR0_DFS_9BITS)
#define SPI_DataSize_10b                ((uint16_t)SPI_CR0_DFS_10BITS)
#define SPI_DataSize_11b                ((uint16_t)SPI_CR0_DFS_11BITS)
#define SPI_DataSize_12b                ((uint16_t)SPI_CR0_DFS_12BITS)
#define SPI_DataSize_13b                ((uint16_t)SPI_CR0_DFS_13BITS)
#define SPI_DataSize_14b                ((uint16_t)SPI_CR0_DFS_14BITS)
#define SPI_DataSize_15b                ((uint16_t)SPI_CR0_DFS_15BITS)
#define SPI_DataSize_16b                ((uint16_t)SPI_CR0_DFS_16BITS)
/**
  * @}
  */


/** @defgroup SPI_Clock_Polarity 
  * @{
  */
#define SPI_CPOL_Low                    ((uint16_t)0x00)
#define SPI_CPOL_High                   ((uint16_t)0x80)
/**
  * @}
  */


/** @defgroup SPI_Clock_Phase 
  * @{
  */
#define SPI_CPHA_1Edge                  ((uint16_t)0x00)
#define SPI_CPHA_2Edge                  ((uint16_t)0x40)
/**
  * @}
  */


/** @defgroup SPI_Frame_Format 
  * @{
  */
#define SPI_FrameFormat_SPI             ((uint16_t)SPI_CR0_FRF_SPI)
#define SPI_FrameFormat_SSP             ((uint16_t)SPI_CR0_FRF_SSP)
#define SPI_FrameFormat_Microwire       ((uint16_t)SPI_CR0_FRF_NS)
/**
  * @}
  */


/** @defgroup SPI_Microwire_Control_Frame_Size 
  * @{
  */
#define SPI_MicrowireControlFrameSize_1b      ((uint16_t)SPI_CR0_CFS_01_BIT)
#define SPI_MicrowireControlFrameSize_2b      ((uint16_t)SPI_CR0_CFS_02_BIT)
#define SPI_MicrowireControlFrameSize_3b      ((uint16_t)SPI_CR0_CFS_03_BIT)
#define SPI_MicrowireControlFrameSize_4b      ((uint16_t)SPI_CR0_CFS_04_BIT)
#define SPI_MicrowireControlFrameSize_5b      ((uint16_t)SPI_CR0_CFS_05_BIT)
#define SPI_MicrowireControlFrameSize_6b      ((uint16_t)SPI_CR0_CFS_06_BIT)
#define SPI_MicrowireControlFrameSize_7b      ((uint16_t)SPI_CR0_CFS_07_BIT)
#define SPI_MicrowireControlFrameSize_8b      ((uint16_t)SPI_CR0_CFS_08_BIT)
#define SPI_MicrowireControlFrameSize_9b      ((uint16_t)SPI_CR0_CFS_09_BIT)
#define SPI_MicrowireControlFrameSize_10b     ((uint16_t)SPI_CR0_CFS_10_BIT)
#define SPI_MicrowireControlFrameSize_11b     ((uint16_t)SPI_CR0_CFS_11_BIT)
#define SPI_MicrowireControlFrameSize_12b     ((uint16_t)SPI_CR0_CFS_12_BIT)
#define SPI_MicrowireControlFrameSize_13b     ((uint16_t)SPI_CR0_CFS_13_BIT)
#define SPI_MicrowireControlFrameSize_14b     ((uint16_t)SPI_CR0_CFS_14_BIT)
#define SPI_MicrowireControlFrameSize_15b     ((uint16_t)SPI_CR0_CFS_15_BIT)
#define SPI_MicrowireControlFrameSize_16b     ((uint16_t)SPI_CR0_CFS_16_BIT)
/**
  * @}
  */


/** @defgroup SPI_Microwire_Transfer_Mode 
  * @{
  */
#define SPI_MicrowireTransferMode_NonSequential   ((uint16_t)0x00)
#define SPI_MicrowireTransferMode_Sequential      ((uint16_t)0x01)
/**
  * @}
  */


/** @defgroup SPI_Microwire_Direction 
  * @{
  */
#define SPI_MicrowireDirection_Receive        ((uint16_t)0x00)
#define SPI_MicrowireDirection_Transmit       ((uint16_t)0x02)
/**
  * @}
  */


/** @defgroup SPI_Microwire_Handshaking 
  * @{
  */
#define SPI_MicrowireHandshaking_Enable       ((uint16_t)0x04)
#define SPI_MicrowireHandshaking_Disable      ((uint16_t)0x00)
/**
  * @}
  */


/** @defgroup SPI_SpiMode 
  * @{
  */
#define SPI_SpiMode_Standard   (SPI_CR0_SPI_MODE_STD)
#define SPI_SpiMode_Dual       (SPI_CR0_SPI_MODE_DUAL)
#define SPI_SpiMode_Quad       (SPI_CR0_SPI_MODE_QUAD)
/**
  * @}
  */


/** @defgroup SPI_EnhancedSpi_TransferType 
  * @{
  */
#define SPI_EnhancedSpiTransferType_0         ((uint16_t)0x00)
#define SPI_EnhancedSpiTransferType_1         ((uint16_t)0x01)
#define SPI_EnhancedSpiTransferType_2         ((uint16_t)0x02)
/**
  * @}
  */


/** @defgroup SPI_EnhancedSpi_InstructionLength 
  * @{
  */
#define SPI_EnhancedSpiInstructionLength_0b   ((uint16_t)SPI_ESPICR_INSTL_0BIT)
#define SPI_EnhancedSpiInstructionLength_4b   ((uint16_t)SPI_ESPICR_INSTL_4BIT)
#define SPI_EnhancedSpiInstructionLength_8b   ((uint16_t)SPI_ESPICR_INSTL_8BIT)
#define SPI_EnhancedSpiInstructionLength_16b  ((uint16_t)SPI_ESPICR_INSTL_16BIT)
/**
  * @}
  */


/** @defgroup SPI_EnhancedSpi_AddressLength 
  * @{
  */
#define SPI_EnhancedSpiAddressLength_0b       ((uint16_t)SPI_ESPICR_ADDRL_0BIT)
#define SPI_EnhancedSpiAddressLength_4b       ((uint16_t)SPI_ESPICR_ADDRL_4BIT)
#define SPI_EnhancedSpiAddressLength_8b       ((uint16_t)SPI_ESPICR_ADDRL_8BIT)
#define SPI_EnhancedSpiAddressLength_12b      ((uint16_t)SPI_ESPICR_ADDRL_12BIT)
#define SPI_EnhancedSpiAddressLength_16b      ((uint16_t)SPI_ESPICR_ADDRL_16BIT)
#define SPI_EnhancedSpiAddressLength_20b      ((uint16_t)SPI_ESPICR_ADDRL_20BIT)
#define SPI_EnhancedSpiAddressLength_24b      ((uint16_t)SPI_ESPICR_ADDRL_24BIT)
#define SPI_EnhancedSpiAddressLength_28b      ((uint16_t)SPI_ESPICR_ADDRL_28BIT)
#define SPI_EnhancedSpiAddressLength_32b      ((uint16_t)SPI_ESPICR_ADDRL_32BIT)
#define SPI_EnhancedSpiAddressLength_36b      ((uint16_t)SPI_ESPICR_ADDRL_36BIT)
#define SPI_EnhancedSpiAddressLength_40b      ((uint16_t)SPI_ESPICR_ADDRL_40BIT)
#define SPI_EnhancedSpiAddressLength_44b      ((uint16_t)SPI_ESPICR_ADDRL_44BIT)
#define SPI_EnhancedSpiAddressLength_48b      ((uint16_t)SPI_ESPICR_ADDRL_48BIT)
#define SPI_EnhancedSpiAddressLength_52b      ((uint16_t)SPI_ESPICR_ADDRL_52BIT)
#define SPI_EnhancedSpiAddressLength_56b      ((uint16_t)SPI_ESPICR_ADDRL_56BIT)
#define SPI_EnhancedSpiAddressLength_60b      ((uint16_t)SPI_ESPICR_ADDRL_60BIT)
/**
  * @}
  */


/** @defgroup SPI_NSS_definition 
  * @{
  */
#define SPI_NSS_0       (0x01)
#define SPI_NSS_1       (0x02)
#define SPI_NSS_2       (0x04)
/**
  * @}
  */


/** @defgroup SPI_flags_definition 
  * @{
  */
#define SPI_FLAG_BUSY   ((uint8_t)SPI_SR_BUSY)
#define SPI_FLAG_TFNF   ((uint8_t)SPI_SR_TFNF)
#define SPI_FLAG_TFE    ((uint8_t)SPI_SR_TFE)
#define SPI_FLAG_RFNE   ((uint8_t)SPI_SR_RFNE)
#define SPI_FLAG_RFF    ((uint8_t)SPI_SR_RFF)
#define SPI_FLAG_TXERR  ((uint8_t)SPI_SR_TXERR)
#define SPI_FLAG_DCOL   ((uint8_t)SPI_SR_DCOL)
/**
  * @}
  */


/** @defgroup SPI_interrupts_definition
  * @{
  */
#define SPI_IT_TXE      (0x1 << 0)
#define SPI_IT_TXO      (0x1 << 1)
#define SPI_IT_RXU      (0x1 << 2)
#define SPI_IT_RXO      (0x1 << 3)
#define SPI_IT_RXF      (0x1 << 4)
#define SPI_IT_MST      (0x1 << 5)
/**
  * @}
  */


/** @defgroup SPI_DMA_Requests 
  * @{
  */
#define SPI_DMAReq_Rx             0x01
#define SPI_DMAReq_Tx             0x02
/**
  * @}
  */ 

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void SPI_DeInit(SPI_TypeDef* SPIx);
void SPI_Init(SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStruct);
void SPI_StructInit(SPI_InitTypeDef* SPI_InitStruct);
void SPI_Cmd(SPI_TypeDef* SPIx, FunctionalState NewState);
void SPI_TransferModeConfig(SPI_TypeDef* SPIx, uint16_t SPI_TransferMode);
void SPI_SlaveOutputCmd(SPI_TypeDef* SPIx, FunctionalState NewState);
void SPI_NSSConfig(SPI_TypeDef* SPIx, uint32_t SPI_NSS, FunctionalState NewState);
void SPI_NSSToggleModeCmd(SPI_TypeDef* SPIx, FunctionalState NewState);
void SPI_ReceiveDataLengthConfig(SPI_TypeDef* SPIx, uint32_t DataLength);
void SPI_MicrowireConfig(SPI_TypeDef* SPIx, SPI_MicrowireInitTypeDef* SPI_MicrowireInitStruct);
void SPI_MicrowireStructInit(SPI_MicrowireInitTypeDef* SPI_MicrowireInitStruct);
void SPI_SpiModeConfig(SPI_TypeDef* SPIx, uint32_t SPI_SpiMode);
void SPI_EnhancedSpiConfig(SPI_TypeDef* SPIx, SPI_EnhancedSpiInitTypeDef* SPI_EnhancedSpiInitStruct);
void SPI_EnhancedSpiStructInit(SPI_EnhancedSpiInitTypeDef* SPI_EnhancedSpiInitStruct);
uint16_t SPI_ReadData(SPI_TypeDef* SPIx);
void SPI_WriteData(SPI_TypeDef* SPIx, uint16_t data);
FlagStatus SPI_GetFlagStatus(SPI_TypeDef* SPIx, uint8_t SPI_FLAG);
void SPI_ITConfig(SPI_TypeDef* SPIx, uint8_t SPI_IT, FunctionalState NewState);
ITStatus SPI_GetRawITStatus(SPI_TypeDef* SPIx, uint8_t SPI_IT);
ITStatus SPI_GetITStatus(SPI_TypeDef* SPIx, uint8_t SPI_IT);
void SPI_ClearITPendingBit(SPI_TypeDef* SPIx, uint8_t SPI_IT);
void SPI_RxFIFOThresholdConfig(SPI_TypeDef* SPIx, uint8_t Threshold);
void SPI_TxFIFOThresholdConfig(SPI_TypeDef* SPIx, uint8_t Threshold);
uint8_t SPI_GetRxFIFOLevel(SPI_TypeDef* SPIx);
uint8_t SPI_GetTxFIFOLevel(SPI_TypeDef* SPIx);
void SPI_DMARxReqLevelConfig(SPI_TypeDef* SPIx, uint8_t DMARxReqLevel);
void SPI_DMATxReqLevelConfig(SPI_TypeDef* SPIx, uint8_t DMATxReqLevel);
void SPI_DMACmd(SPI_TypeDef* SPIx, uint32_t SPI_DMAReq, FunctionalState NewState);
void SPI_RxdSampleDelayConfig(SPI_TypeDef* SPIx, uint8_t DelayValue);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __WB32F10x_SPI_H */

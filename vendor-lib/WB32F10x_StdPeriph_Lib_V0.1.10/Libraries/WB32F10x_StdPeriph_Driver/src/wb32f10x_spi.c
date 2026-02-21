/**
 * @file    wb32f10x_spi.c
 * @author  Westberry Application Team
 * @version V0.1.10
 * @date    05-January-2023
 * @brief   This file provides all the SPI firmware functions.
 */

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x_spi.h"
#include "wb32f10x_rcc.h"

/** @addtogroup WB32F10x_StdPeriph_Driver
  * @{
  */

/** @defgroup SPI
  * @brief SPI driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup SPI_Private_Functions
  * @{
  */

/**
 * @brief  Deinitializes the SPIx peripheral registers to their default reset values.
 * @param  SPIx: Pointer to selected SPI peripheral.
 *         This parameter can be one of the following values:
 *         QSPI, SPIM2, SPIS1, SPIS2.
 * @return None
 */
void SPI_DeInit(SPI_TypeDef* SPIx)
{
  if (SPIx == QSPI)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_QSPI, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_QSPI, DISABLE);
  }
  else if (SPIx == SPIS1)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPIS1, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPIS1, DISABLE);
  }
  else if (SPIx == SPIM2)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPIM2, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPIM2, DISABLE);
  }
  else if (SPIx == SPIS2)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPIS2, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPIS2, DISABLE);
  }
}

/**
 * @brief  Initializes the SPIx peripheral according to the specified 
 *         parameters in the SPI_InitStruct.
 * @param  SPIx: Pointer to selected SPI peripheral.
 *         This parameter can be one of the following values:
 *         QSPI, SPIM2, SPIS1, SPIS2.
 * @param  SPI_InitStruct: pointer to a SPI_InitTypeDef structure
 *         that contains the configuration information for the specified 
 *         SPI peripheral.
 * @return None
 */
void SPI_Init(SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStruct)
{
  SPIx->CR0 = (SPI_InitStruct->SPI_DataSize | SPI_InitStruct->SPI_FrameFormat |
        SPI_InitStruct->SPI_CPHA | SPI_InitStruct->SPI_CPOL |
        SPI_InitStruct->SPI_TransferMode);

  SPIx->BAUDR = SPI_InitStruct->SPI_BaudRatePrescaler;
}

/**
 * @brief  Fills each SPI_InitStruct member with its default value.
 * @param  SPI_InitStruct: pointer to a SPI_InitTypeDef structure
 *         which will be initialized.
 * @return None
 */
void SPI_StructInit(SPI_InitTypeDef* SPI_InitStruct)
{
  SPI_InitStruct->SPI_TransferMode = SPI_TransferMode_TxAndRx;
  SPI_InitStruct->SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStruct->SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStruct->SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStruct->SPI_BaudRatePrescaler = 2;
  SPI_InitStruct->SPI_FrameFormat = SPI_FrameFormat_SPI;
}

/**
 * @brief  Enables or disables the specified SPI peripheral.
 * @param  SPIx: Pointer to selected SPI peripheral.
 *         This parameter can be one of the following values:
 *         QSPI, SPIM2, SPIS1, SPIS2.
 * @param  NewState: new state of the SPIx peripheral.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None
 */
void SPI_Cmd(SPI_TypeDef* SPIx, FunctionalState NewState)
{
  if (NewState != DISABLE) {
    SPIx->SPIENR = 0x01;
  }
  else {
    SPIx->SPIENR = 0x00;
  }
}

/**
 * @brief  Configures the transfer mode.
 * @param  SPIx: Pointer to selected SPI peripheral.
 *         This parameter can be one of the following values:
 *         QSPI, SPIM2, SPIS1, SPIS2.
 * @param  SPI_TransferMode: specifies the transfer mode.
 *         This parameter can be one of the following values:
 *         @arg @ref SPI_TransferMode_TxAndRx
 *         @arg @ref SPI_TransferMode_TxOnly
 *         @arg @ref SPI_TransferMode_RxOnly
 *         @arg @ref SPI_TransferMode_EepromRead
 * @return None
 */
void SPI_TransferModeConfig(SPI_TypeDef* SPIx, uint16_t SPI_TransferMode)
{
  SPIx->CR0 = (SPIx->CR0 & (~SPI_CR0_TMOD_Msk)) | SPI_TransferMode;
}

/**
 * @brief  Enables or disables the slave output (only used for SPISx).
 * @param  SPIx: Pointer to selected SPI peripheral.
 *         This parameter can be one of the following values:
 *         SPIS1, SPIS2.
 * @param  NewState: new state of the slave output.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None
 */
void SPI_SlaveOutputCmd(SPI_TypeDef* SPIx, FunctionalState NewState)
{
  if (NewState != DISABLE) {
    SPIx->CR0 &= ~SPI_CR0_SLV_OE;
  }
  else {
    SPIx->CR0 |= SPI_CR0_SLV_OE;
  }
}

/**
 * @brief  Enables or disables the specified slave select line (only used for SPIMx).
 * @param  SPIx: Pointer to selected SPI peripheral.
 *         This parameter can be one of the following values:
 *         QSPI, SPIM2.
 * @param  SPI_NSS: specifies the slave select line to be enabled or disabled.
 *         This parameter can be one of the following values:
 *         @arg @ref SPI_NSS_0
 *         @arg @ref SPI_NSS_1
 *         @arg @ref SPI_NSS_2
 * @param  NewState: new state of the slave select line.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None
 */
void SPI_NSSConfig(SPI_TypeDef* SPIx, uint32_t SPI_NSS, FunctionalState NewState)
{
  if (NewState != DISABLE) {
    SPIx->SER |= SPI_NSS;
  }
  else {
    SPIx->SER &= ~SPI_NSS;
  }
}

/**
 * @brief  Enables or disables the slave select toggle mode.
 * @param  SPIx: Pointer to selected SPI peripheral.
 *         This parameter can be one of the following values:
 *         QSPI, SPIM2, SPIS1, SPIS2.
 * @param  NewState: new state of the slave select toggle mode.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None
 */
void SPI_NSSToggleModeCmd(SPI_TypeDef* SPIx, FunctionalState NewState)
{
  if (NewState != DISABLE) {
    SPIx->CR0 |= SPI_CR0_SSTE;
  }
  else {
    SPIx->CR0 &= ~SPI_CR0_SSTE;
  }
}

/**
 * @brief  Configures the number of data frames to be continuously received (only used for SPIMx).
 * @param  SPIx: Pointer to selected SPI peripheral.
 *         This parameter can be one of the following values:
 *         QSPI, SPIM2.
 * @param  DataLength: specifies the number of data frames to be continuously received.
 * @return None
 */
void SPI_ReceiveDataLengthConfig(SPI_TypeDef* SPIx, uint32_t DataLength)
{
  SPIx->CR1 = DataLength - 1;
}

/**
 * @brief  Configures the Microwire frame format according to the
 *         specified parameters in the SPI_MicrowireInitStruct.
 * @param  SPIx: Pointer to selected SPI peripheral.
 *         This parameter can be one of the following values:
 *         QSPI, SPIM2, SPIS1, SPIS2.
 * @param  SPI_MicrowireInitStruct: pointer to a SPI_MicrowireInitTypeDef structure
 *         that contains the configuration information for the Microwire frame format. 
 * @return None
 */
void SPI_MicrowireConfig(SPI_TypeDef* SPIx, SPI_MicrowireInitTypeDef* SPI_MicrowireInitStruct)
{
  SPIx->CR0 = (SPIx->CR0 & (~SPI_CR0_CFS_Msk)) | SPI_MicrowireInitStruct->SPI_MicrowireControlFrameSize;
  SPIx->MWCR = (SPIx->MWCR & 0xFFFFFFF8) | SPI_MicrowireInitStruct->SPI_MicrowireTransferMode |
    SPI_MicrowireInitStruct->SPI_MicrowireDirection | SPI_MicrowireInitStruct->SPI_MicrowireHandshaking;
}

/**
 * @brief  Fills each SPI_MicrowireInitStruct member with its default value.
 * @param  SPI_MicrowireInitStruct: pointer to a SPI_MicrowireInitTypeDef structure
 *         which will be initialized.
 * @return None
 */
void SPI_MicrowireStructInit(SPI_MicrowireInitTypeDef* SPI_MicrowireInitStruct)
{
  SPI_MicrowireInitStruct->SPI_MicrowireControlFrameSize = SPI_MicrowireControlFrameSize_8b;
  SPI_MicrowireInitStruct->SPI_MicrowireTransferMode = SPI_MicrowireTransferMode_NonSequential;
  SPI_MicrowireInitStruct->SPI_MicrowireDirection = SPI_MicrowireDirection_Transmit;
  SPI_MicrowireInitStruct->SPI_MicrowireHandshaking = SPI_MicrowireHandshaking_Disable;
}

/**
 * @brief  Configures the Spi Mode.
 * @param  SPIx: Pointer to selected SPI peripheral.
 *         This parameter must be QSPI.
 * @param  SPI_SpiMode: specifies the Spi Mode.
 *         This parameter can be one of the following values:
 *         @arg @ref SPI_SpiMode_Standard
 *         @arg @ref SPI_SpiMode_Dual
 *         @arg @ref SPI_SpiMode_Quad
 * @return None
 */
void SPI_SpiModeConfig(SPI_TypeDef* SPIx, uint32_t SPI_SpiMode)
{
  SPIx->CR0 = (SPIx->CR0 & (~SPI_CR0_SPI_MODE_Msk)) | SPI_SpiMode;
}

/**
 * @brief  Configures the Enhanced SPI Mode according to the
 *         specified parameters in the SPI_EnhancedSpiInitStruct (only used for QSPI).
 * @param  SPIx: Pointer to selected SPI peripheral.
 *         This parameter must be QSPI.
 * @param  SPI_EnhancedSpiInitStruct: pointer to a SPI_EnhancedSpiInitTypeDef structure
 *         that contains the configuration information for the Enhanced SPI Mode. 
 * @return None
 */
void SPI_EnhancedSpiConfig(SPI_TypeDef* SPIx, SPI_EnhancedSpiInitTypeDef* SPI_EnhancedSpiInitStruct)
{
  SPIx->ESPICR = SPI_EnhancedSpiInitStruct->SPI_EnhancedSpiTransferType |
      SPI_EnhancedSpiInitStruct->SPI_EnhancedSpiAddressLength |
      SPI_EnhancedSpiInitStruct->SPI_EnhancedSpiInstructionLength |
      (SPI_EnhancedSpiInitStruct->SPI_EnhancedSpiWaitCycles << 11);
}

/**
 * @brief  Fills each SPI_EnhancedSpiInitStruct member with its default value.
 * @param  SPI_EnhancedSpiInitStruct: pointer to a SPI_EnhancedSpiInitTypeDef structure
 *         which will be initialized.
 * @return None
 */
void SPI_EnhancedSpiStructInit(SPI_EnhancedSpiInitTypeDef* SPI_EnhancedSpiInitStruct)
{
  SPI_EnhancedSpiInitStruct->SPI_EnhancedSpiTransferType = SPI_EnhancedSpiTransferType_0;
  SPI_EnhancedSpiInitStruct->SPI_EnhancedSpiAddressLength = SPI_EnhancedSpiAddressLength_24b;
  SPI_EnhancedSpiInitStruct->SPI_EnhancedSpiInstructionLength = SPI_EnhancedSpiInstructionLength_8b;
  SPI_EnhancedSpiInitStruct->SPI_EnhancedSpiWaitCycles = 0;
}

/**
 * @brief  Read one data from Rx FIFO.
 * @param  SPIx: Pointer to selected SPI peripheral.
 *         This parameter can be one of the following values:
 *         QSPI, SPIM2, SPIS1, SPIS2.
 * @return The read data.
 */
uint16_t SPI_ReadData(SPI_TypeDef* SPIx)
{
  return SPIx->DR;
}

/**
 * @brief  Write one data to Tx FIFO.
 * @param  SPIx: Pointer to selected SPI peripheral.
 *         This parameter can be one of the following values:
 *         QSPI, SPIM2, SPIS1, SPIS2.
 * @param  Data: The data to write.
 * @return None
 */
void SPI_WriteData(SPI_TypeDef* SPIx, uint16_t Data)
{
  SPIx->DR = Data;
}

/**
 * @brief  Checks whether the specified SPIx flag is set or not.
 * @param  SPIx: Pointer to selected SPI peripheral.
 *         This parameter can be one of the following values:
 *         QSPI, SPIM2, SPIS1, SPIS2.
 * @param  SPI_FLAG: specifies the SPI flag to check.
 *         This parameter can be one of the following values:
 *         @arg @ref SPI_FLAG_BUSY: SPI busy flag.
 *         @arg @ref SPI_FLAG_TFNF: Transmit FIFO not full.
 *         @arg @ref SPI_FLAG_TFE: Transmit FIFO empty.
 *         @arg @ref SPI_FLAG_RFNE: Receive FIFO not empty.
 *         @arg @ref SPI_FLAG_RFF: Receive FIFO full.
 *         @arg @ref SPI_FLAG_TXERR: Transmission error (only for SPIS1 and SPIS2).
 *         @arg @ref SPI_FLAG_DCOL: Data collision error (only for QSPI and SPIM2).
 * @return The new state of SPI_FLAG (SET or RESET).
 */
FlagStatus SPI_GetFlagStatus(SPI_TypeDef* SPIx, uint8_t SPI_FLAG)
{
  FlagStatus bitstatus = RESET;

  /* Check the status of the specified SPI flag */
  if (SPIx->SR & SPI_FLAG)
  {
    /* SPI_FLAG is set */
    bitstatus = SET;
  }
  else
  {
    /* SPI_FLAG is reset */
    bitstatus = RESET;
  }
  /* Return the SPI_FLAG status */
  return  bitstatus;
}

/**
 * @brief  Enables or disables the specified SPI interrupts.
 * @param  SPIx: Pointer to selected SPI peripheral.
 *         This parameter can be one of the following values:
 *         QSPI, SPIM2, SPIS1, SPIS2.
 * @param  SPI_IT: specifies the SPI interrupt source to be enabled or disabled. 
 *         This parameter can be a combination of the following values:
 *         @arg @ref SPI_IT_TXE: Transmit FIFO empty interrupt mask
 *         @arg @ref SPI_IT_TXO: Transmit FIFO overflow interrupt mask
 *         @arg @ref SPI_IT_RXU: Receive FIFO underflow interrupt mask
 *         @arg @ref SPI_IT_RXO: Receive FIFO overflow interrupt mask
 *         @arg @ref SPI_IT_RXF: Receive FIFO full interrupt mask
 *         @arg @ref SPI_IT_MST: Multi-Master contention interrupt mask (only for QSPI and SPIM2)
 * @param  NewState: new state of the specified SPI interrupt.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None.
 */
void SPI_ITConfig(SPI_TypeDef* SPIx, uint8_t SPI_IT, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    /* Enable the selected SPI interrupt */
    SPIx->IER |= SPI_IT;
  }
  else
  {
    /* Disable the selected SPI interrupt */
    SPIx->IER &= ~SPI_IT;
  }
}

/**
 * @brief  Checks whether the specified SPIx raw interrupt status.
 * @param  SPIx: Pointer to selected SPI peripheral.
 *         This parameter can be one of the following values:
 *         QSPI, SPIM2, SPIS1, SPIS2.
 * @param  SPI_IT: specifies the SPI interrupt source to check.
 *         This parameter can be one of the following values:
 *         @arg @ref SPI_IT_TXE: Transmit FIFO empty interrupt
 *         @arg @ref SPI_IT_TXO: Transmit FIFO overflow interrupt
 *         @arg @ref SPI_IT_RXU: Receive FIFO underflow interrupt
 *         @arg @ref SPI_IT_RXO: Receive FIFO overflow interrupt
 *         @arg @ref SPI_IT_RXF: Receive FIFO full interrupt
 *         @arg @ref SPI_IT_MST: Multi-Master contention interrupt (only for QSPI and SPIM2)
 * @return The new state of raw SPI_IT (SET or RESET).
 */
ITStatus SPI_GetRawITStatus(SPI_TypeDef* SPIx, uint8_t SPI_IT)
{
  ITStatus bitstatus = RESET;

  /* Check the raw status of the specified SPI interrupt */
  if (SPIx->RISR & SPI_IT)
  {
    /* SPI_IT is set */
    bitstatus = SET;
  }
  else
  {
    /* SPI_IT is reset */
    bitstatus = RESET;
  }
  /* Return the SPI_IT raw status */
  return bitstatus;
}

/**
 * @brief  Checks whether the specified SPIx interrupt has occurred or not.
 * @param  SPIx: Pointer to selected SPI peripheral.
 *         This parameter can be one of the following values:
 *         QSPI, SPIM2, SPIS1, SPIS2.
 * @param  SPI_IT: specifies the SPI interrupt source to check.
 *         This parameter can be one of the following values:
 *         @arg @ref SPI_IT_TXE: Transmit FIFO empty interrupt
 *         @arg @ref SPI_IT_TXO: Transmit FIFO overflow interrupt
 *         @arg @ref SPI_IT_RXU: Receive FIFO underflow interrupt
 *         @arg @ref SPI_IT_RXO: Receive FIFO overflow interrupt
 *         @arg @ref SPI_IT_RXF: Receive FIFO full interrupt
 *         @arg @ref SPI_IT_MST: Multi-Master contention interrupt (Only for QSPI and SPIM2)
 * @return The new state of SPI_IT (SET or RESET).
 */
ITStatus SPI_GetITStatus(SPI_TypeDef* SPIx, uint8_t SPI_IT)
{
  ITStatus bitstatus = RESET;

  /* Check the status of the specified SPI interrupt */
  if (SPIx->ISR & SPI_IT)
  {
    /* SPI_IT is set */
    bitstatus = SET;
  }
  else
  {
    /* SPI_IT is reset */
    bitstatus = RESET;
  }
  /* Return the SPI_IT status */
  return bitstatus;
}

/**
 * @brief  Clears the SPIx's interrupt pending bits.
 * @param  SPIx: Pointer to selected SPI peripheral.
 *         This parameter can be one of the following values:
 *         QSPI, SPIM2, SPIS1, SPIS2.
 * @param  SPI_IT: specifies the interrupt pending bit to clear.
 *         This parameter can be a combination of the following values:
 *         @arg @ref SPI_IT_TXO: Transmit FIFO overflow interrupt
 *         @arg @ref SPI_IT_RXU: Receive FIFO underflow interrupt
 *         @arg @ref SPI_IT_RXO: Receive FIFO overflow interrupt
 *         @arg @ref SPI_IT_MST: Multi-Master contention interrupt (Only for QSPI and SPIM2)
 * @return None.
 */
void SPI_ClearITPendingBit(SPI_TypeDef* SPIx, uint8_t SPI_IT)
{
  if ((SPI_IT & (SPI_IT_TXO | SPI_IT_RXU | SPI_IT_RXO | SPI_IT_MST)) ==
               (SPI_IT_TXO | SPI_IT_RXU | SPI_IT_RXO | SPI_IT_MST))
  {
    (void)SPIx->ICR;
  }
  else
  {
    if(SPI_IT & SPI_IT_TXO) (void)SPIx->TXOICR;
    if(SPI_IT & SPI_IT_RXU) (void)SPIx->RXUICR;
    if(SPI_IT & SPI_IT_RXO) (void)SPIx->RXOICR;
    if(SPI_IT & SPI_IT_MST) (void)SPIx->MSTICR;
  }
}

/**
 * @brief  Configures the Rx FIFO threshold.
 * @param  SPIx: Pointer to selected SPI peripheral.
 *         This parameter can be one of the following values:
 *         QSPI, SPIM2, SPIS1, SPIS2.
 * @param  Threshold: The Rx FIFO threshold to set.
 * @note   The Receive FIFO Full interrupt will trigger when the level > Threshold.
 * @return None.
 */
void SPI_RxFIFOThresholdConfig(SPI_TypeDef* SPIx, uint8_t Threshold)
{
  SPIx->RXFTLR = Threshold;
}

/**
 * @brief  Configures the Tx FIFO threshold.
 * @param  SPIx: Pointer to selected SPI peripheral.
 *         This parameter can be one of the following values:
 *         QSPI, SPIM2, SPIS1, SPIS2.
 * @param  Threshold: The Tx FIFO threshold to set.
 * @note   The Transmit FIFO Empty interrupt will trigger when the level <= Threshold.
 * @return None.
 */
void SPI_TxFIFOThresholdConfig(SPI_TypeDef* SPIx, uint8_t Threshold)
{
  SPIx->TXFTLR = Threshold;
}

/**
 * @brief  Returns the number of data can be read from Rx FIFO.
 * @param  SPIx: Pointer to selected SPI peripheral.
 *         This parameter can be one of the following values:
 *         QSPI, SPIM2, SPIS1, SPIS2.
 * @return The number of data can be read from Rx FIFO.
 */
uint8_t SPI_GetRxFIFOLevel(SPI_TypeDef* SPIx)
{
  return SPIx->RXFLR;
}

/**
 * @brief  Returns the number of data left in Tx FIFO.
 * @param  SPIx: Pointer to selected SPI peripheral.
 *         This parameter can be one of the following values:
 *         QSPI, SPIM2, SPIS1, SPIS2.
 * @return The number of data left in Tx FIFO.
 */
uint8_t SPI_GetTxFIFOLevel(SPI_TypeDef* SPIx)
{
  return SPIx->TXFLR;
}

/**
 * @brief  Configures the DMA Rx request level.
 * @param  SPIx: Pointer to selected SPI peripheral.
 *         This parameter can be one of the following values:
 *         QSPI, SPIM2, SPIS1, SPIS2.
 * @param  Level: The level of the DMA Rx request to set.
 * @note   The DMA Rx request will trigger when the rx fifo level > DMARxReqLevel.
 * @return None.
 */
void SPI_DMARxReqLevelConfig(SPI_TypeDef* SPIx, uint8_t DMARxReqLevel)
{
  SPIx->DMARDLR = DMARxReqLevel;
}

/**
 * @brief  Configures the DMA Tx request level.
 * @param  SPIx: Pointer to selected SPI peripheral.
 *         This parameter can be one of the following values:
 *         QSPI, SPIM2, SPIS1, SPIS2.
 * @param  Level: The level of the DMA Tx request to set.
 * @note   The DMA Tx request will trigger when the tx fifo level <= DMATxReqLevel.
 * @return None.
 */
void SPI_DMATxReqLevelConfig(SPI_TypeDef* SPIx, uint8_t DMATxReqLevel)
{
  SPIx->DMATDLR = DMATxReqLevel;
}

/**
 * @brief  Enables or disables the SPIx's DMA interface.
 * @param  SPIx: Pointer to selected SPI peripheral.
 *         This parameter can be one of the following values:
 *         QSPI, SPIM2, SPIS1, SPIS2.
 * @param  SPI_DMAReq: specifies the DMA request.
 *         This parameter can be any combination of the following values:
 *         @arg SPI_DMAReq_Rx: SPI DMA receive request
 *         @arg SPI_DMAReq_Tx: SPI DMA transmit request
 * @param  NewState: new state of the DMA Request sources.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None.
 */
void SPI_DMACmd(SPI_TypeDef* SPIx, uint32_t SPI_DMAReq, FunctionalState NewState)
{
  if (NewState != DISABLE) {
    SPIx->DMACR |= SPI_DMAReq;
  }
  else {
    SPIx->DMACR &= ~SPI_DMAReq;
  }
}

/**
 * @brief  Configures the rxd sample delay value.
 * @param  SPIx: Pointer to selected SPI peripheral.
 *         This parameter can be one of the following values:
 *         QSPI, SPIM2.
 * @param  DelayValue: specifies the rxd sample delay value.
 * @return None.
 */
void SPI_RxdSampleDelayConfig(SPI_TypeDef* SPIx, uint8_t DelayValue)
{
  SPIx->RX_SAMPLE_DLY = DelayValue;
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

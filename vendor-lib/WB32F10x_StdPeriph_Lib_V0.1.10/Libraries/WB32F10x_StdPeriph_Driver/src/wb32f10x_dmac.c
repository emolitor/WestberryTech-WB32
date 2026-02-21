/**
 * @file    wb32f10x_dmac.c
 * @author  Westberry Application Team
 * @version V0.1.10
 * @date    05-January-2023
 * @brief   This file provides all the DMAC firmware functions.
 */

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x_dmac.h"
#include "wb32f10x_rcc.h"

/** @addtogroup WB32F10x_StdPeriph_Driver
  * @{
  */

/** @defgroup DMAC
  * @brief DMAC driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup DMAC_Private_Functions
  * @{
  */

/**
 * @brief  Deinitializes the DMACx peripheral registers to their default reset values.
 * @param  DMACx: Pointer to selected DMAC peripheral.
 *         This parameter can be one of the following values:
 *         DMAC1, DMAC2.
 * @return None
 */
void DMAC_DeInit(DMAC_TypeDef* DMACx)
{
  if (DMACx == DMAC1)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_DMAC1, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_DMAC1, DISABLE);
  }
  else if (DMACx == DMAC2)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_DMAC2, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_DMAC2, DISABLE);
  }
}

/**
 * @brief  Initializes the DMACx Channely according to the specified parameters in 
 *         the DMAC_Channel_InitStruct structure.
 * @param  DMACx: Pointer to selected DMAC peripheral.
 *         This parameter can be one of the following values:
 *         DMAC1, DMAC2.
 * @param  DMAC_Channel: specifies the selected DMAC channel.
 *         This parameter can be one of the following values:
 *         @arg @ref DMAC_Channel_0
 *         @arg @ref DMAC_Channel_1
 *         @arg @ref DMAC_Channel_2
 * @param  DMAC_Channel_InitStruct: pointer to a DMAC_Channel_InitTypeDef structure that contains
 *         the configuration information for the specified DMACx Channely.
 * @return None
 */
void DMAC_Channel_Init(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, DMAC_Channel_InitTypeDef* DMAC_Channel_InitStruct)
{
  DMACx->Ch[DMAC_Channel].SAR = DMAC_Channel_InitStruct->DMAC_SourceBaseAddr;
  DMACx->Ch[DMAC_Channel].DAR = DMAC_Channel_InitStruct->DMAC_DestinationBaseAddr;

  DMACx->Ch[DMAC_Channel].CTLL = DMAC_Channel_InitStruct->DMAC_Interrupt |
                        DMAC_Channel_InitStruct->DMAC_DestinationTransferWidth |
                        DMAC_Channel_InitStruct->DMAC_SourceTransferWidth      |
                        DMAC_Channel_InitStruct->DMAC_DestinationAddrInc       |
                        DMAC_Channel_InitStruct->DMAC_SourceAddrInc            |
                        DMAC_Channel_InitStruct->DMAC_DestinationTransactionLength |
                        DMAC_Channel_InitStruct->DMAC_SourceTransactionLength |
                        DMAC_Channel_InitStruct->DMAC_TransferTypeAndFlowControl |
                        DMAC_Channel_InitStruct->DMAC_DestinationMasterInterface |
                        DMAC_Channel_InitStruct->DMAC_SourceMasterInterface;

  DMACx->Ch[DMAC_Channel].CTLH = DMAC_Channel_InitStruct->DMAC_BlockTransferSize;
  
  DMACx->Ch[DMAC_Channel].CFGL = (DMAC_Channel_InitStruct->DMAC_ChannelPriority << 5) |
                        DMAC_Channel_InitStruct->DMAC_DestinationHandshakingInterfaceSelect |
                        DMAC_Channel_InitStruct->DMAC_SourceHandshakingInterfaceSelect      |
                        DMAC_Channel_InitStruct->DMAC_DestinationHandshakingInterfacePolarity |
                        DMAC_Channel_InitStruct->DMAC_SourceHandshakingInterfacePolarity |
                        (DMAC_Channel_InitStruct->DMAC_MaximumAMBABurstLength << 20) |
                        DMAC_Channel_InitStruct->DMAC_AutomaticSourceReload |
                        DMAC_Channel_InitStruct->DMAC_AutomaticDestinationReload;
  
  DMACx->Ch[DMAC_Channel].CFGH = DMAC_Channel_InitStruct->DMAC_FlowControlMode |
                        DMAC_Channel_InitStruct->DMAC_FIFOMode |
                        (DMAC_Channel_InitStruct->DMAC_ProtectionControl << 2) |
                        (DMAC_Channel_InitStruct->DMAC_SourceHardwareHandshakingInterfaceAssign << 7) |
                        (DMAC_Channel_InitStruct->DMAC_DestinationHardwareHandshakingInterfaceAssign << 11);
}

/**
 * @brief  Fills each DMAC_Channel_InitStruct member with its default value.
 * @param  DMAC_Channel_InitStruct: pointer to a DMAC_Channel_InitTypeDef structure which will 
 *         be initialized.
 * @return None
 */
void DMAC_Channel_StructInit(DMAC_Channel_InitTypeDef* DMAC_Channel_InitStruct)
{
  DMAC_Channel_InitStruct->DMAC_SourceBaseAddr = 0x00000000;
  DMAC_Channel_InitStruct->DMAC_DestinationBaseAddr = 0x00000000;

  DMAC_Channel_InitStruct->DMAC_Interrupt = DMAC_Interrupt_Disable;
  DMAC_Channel_InitStruct->DMAC_SourceTransferWidth = DMAC_SourceTransferWidth_8b;
  DMAC_Channel_InitStruct->DMAC_DestinationTransferWidth = DMAC_DestinationTransferWidth_8b;

  DMAC_Channel_InitStruct->DMAC_SourceAddrInc = DMAC_SourceAddrInc_Increment;
  DMAC_Channel_InitStruct->DMAC_DestinationAddrInc = DMAC_DestinationAddrInc_Increment;

  DMAC_Channel_InitStruct->DMAC_SourceTransactionLength = DMAC_SourceTransactionLength_4;
  DMAC_Channel_InitStruct->DMAC_DestinationTransactionLength = DMAC_DestinationTransactionLength_4;

  DMAC_Channel_InitStruct->DMAC_TransferTypeAndFlowControl = DMAC_TransferTypeAndFlowControl_MemoryToMemory_DMAC;

  DMAC_Channel_InitStruct->DMAC_SourceMasterInterface = DMAC_SourceMasterInterface_AHB;
  DMAC_Channel_InitStruct->DMAC_DestinationMasterInterface = DMAC_DestinationMasterInterface_AHB;

  DMAC_Channel_InitStruct->DMAC_BlockTransferSize = 2;

  DMAC_Channel_InitStruct->DMAC_SourceHandshakingInterfaceSelect = DMAC_SourceHandshakingInterfaceSelect_Hardware;
  DMAC_Channel_InitStruct->DMAC_DestinationHandshakingInterfaceSelect = DMAC_DestinationHandshakingInterfaceSelect_Hardware;

  DMAC_Channel_InitStruct->DMAC_SourceHandshakingInterfacePolarity = DMAC_SourceHandshakingInterfacePolarity_High;
  DMAC_Channel_InitStruct->DMAC_DestinationHandshakingInterfacePolarity = DMAC_DestinationHandshakingInterfacePolarity_High;

  DMAC_Channel_InitStruct->DMAC_AutomaticSourceReload = DMAC_AutomaticSourceReload_Disable;
  DMAC_Channel_InitStruct->DMAC_AutomaticDestinationReload = DMAC_AutomaticDestinationReload_Disable;

  DMAC_Channel_InitStruct->DMAC_FlowControlMode = DMAC_FlowControlMode_0;
  DMAC_Channel_InitStruct->DMAC_FIFOMode = DMAC_FIFOMode_0;
  DMAC_Channel_InitStruct->DMAC_ChannelPriority = 0;
  DMAC_Channel_InitStruct->DMAC_ProtectionControl = 0x1;

  DMAC_Channel_InitStruct->DMAC_SourceHardwareHandshakingInterfaceAssign = 0;
  DMAC_Channel_InitStruct->DMAC_DestinationHardwareHandshakingInterfaceAssign = 0;

  DMAC_Channel_InitStruct->DMAC_MaximumAMBABurstLength = 0;
}

/**
 * @brief  Initializes the source gather of DMACx Channely.
 * @param  DMACx: Pointer to selected DMAC peripheral.
 *         This parameter can be one of the following values:
 *         DMAC1, DMAC2.
 * @param  DMAC_Channel: specifies the selected DMAC channel.
 *         This parameter can be one of the following values:
 *         @arg @ref DMAC_Channel_0
 *         @arg @ref DMAC_Channel_1
 *         @arg @ref DMAC_Channel_2
 * @param  DMAC_SourceGatherInitStruct: pointer to a DMAC_SourceGatherInitTypeDef structure that contains
 *         the configuration information for the source gather of DMACx Channely.
 * @return None
 */
void DMAC_Channel_SourceGatherConfig(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, DMAC_SourceGatherInitTypeDef* DMAC_SourceGatherInitStruct)
{
  DMACx->Ch[DMAC_Channel].SGR = (DMAC_SourceGatherInitStruct->DMAC_SourceGatherCount << 20) |
              DMAC_SourceGatherInitStruct->DMAC_SourceGatherInterval;

  if (DMAC_SourceGatherInitStruct->DMAC_SourceGather != DISABLE)
  {
    DMACx->Ch[DMAC_Channel].CTLL |= DMAC_CTLL_SRC_GATHER_EN;
  }
  else
  {
    DMACx->Ch[DMAC_Channel].CTLL &= ~DMAC_CTLL_SRC_GATHER_EN;
  }
}

/**
 * @brief  Initializes the destination scatter of DMACx Channely.
 * @param  DMACx: Pointer to selected DMAC peripheral.
 *         This parameter can be one of the following values:
 *         DMAC1, DMAC2.
 * @param  DMAC_Channel: specifies the selected DMAC channel.
 *         This parameter can be one of the following values:
 *         @arg @ref DMAC_Channel_0
 *         @arg @ref DMAC_Channel_1
 *         @arg @ref DMAC_Channel_2
 * @param  DMAC_DestinationScatterInitStruct: pointer to a DMAC_DestinationScatterInitTypeDef structure that contains
 *         the configuration information for the destination scatter of DMACx Channely.
 * @return None
 */
void DMAC_Channel_DestinationScatterConfig(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, DMAC_DestinationScatterInitTypeDef* DMAC_DestinationScatterInitStruct)
{
  DMACx->Ch[DMAC_Channel].DSR = (DMAC_DestinationScatterInitStruct->DMAC_DestinationScatterCount << 20) |
              DMAC_DestinationScatterInitStruct->DMAC_DestinationScatterInterval;

  if (DMAC_DestinationScatterInitStruct->DMAC_DestinationScatter != DISABLE)
  {
    DMACx->Ch[DMAC_Channel].CTLL |= DMAC_CTLL_DST_SCATTER_EN;
  }
  else
  {
    DMACx->Ch[DMAC_Channel].CTLL &= ~DMAC_CTLL_DST_SCATTER_EN;
  }
}

/**
 * @brief  Sets the source address of the specified DMACx Channely.
 * @param  DMACx: Pointer to selected DMAC peripheral.
 *         This parameter can be one of the following values:
 *         DMAC1, DMAC2.
 * @param  DMAC_Channel: specifies the selected DMAC channel.
 *         This parameter can be one of the following values:
 *         @arg @ref DMAC_Channel_0
 *         @arg @ref DMAC_Channel_1
 *         @arg @ref DMAC_Channel_2
 * @param  SourceAddress: specifies the source address of the specified DMACx Channely.
 * @return None
 */
void DMAC_Channel_SetSourceAddress(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, uint32_t SourceAddress)
{
  DMACx->Ch[DMAC_Channel].SAR = SourceAddress;
}

/**
 * @brief  Sets the destination address of the specified DMACx Channely.
 * @param  DMACx: Pointer to selected DMAC peripheral.
 *         This parameter can be one of the following values:
 *         DMAC1, DMAC2.
 * @param  DMAC_Channel: specifies the selected DMAC channel.
 *         This parameter can be one of the following values:
 *         @arg @ref DMAC_Channel_0
 *         @arg @ref DMAC_Channel_1
 *         @arg @ref DMAC_Channel_2
 * @param  DestinationAddress: specifies the destination address of the specified DMACx Channely.
 * @return None
 */
void DMAC_Channel_SetDestinationAddress(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, uint32_t DestinationAddress)
{
  DMACx->Ch[DMAC_Channel].DAR = DestinationAddress;
}

/**
 * @brief  Sets the number of data units to be transferred on the block transfer.
 * @param  DMACx: Pointer to selected DMAC peripheral.
 *         This parameter can be one of the following values:
 *         DMAC1, DMAC2.
 * @param  DMAC_Channel: specifies the selected DMAC channel.
 *         This parameter can be one of the following values:
 *         @arg @ref DMAC_Channel_0
 *         @arg @ref DMAC_Channel_1
 *         @arg @ref DMAC_Channel_2
 * @param  DMAC_BlockTransferSize: specifies the number of data units to be transferred on the block transfer.
 *         This parameter value must be lower than 512.
 * @return None
 */
void DMAC_Channel_SetBlockTransferSize(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, uint16_t DMAC_BlockTransferSize)
{
  DMACx->Ch[DMAC_Channel].CTLH = (DMACx->Ch[DMAC_Channel].CTLH & 0xFFFFF000U) | DMAC_BlockTransferSize;
}

/**
 * @brief  Returns the number of remaining data units in the current DMACx Channely block transfer.
 * @param  DMACx: Pointer to selected DMAC peripheral.
 *         This parameter can be one of the following values:
 *         DMAC1, DMAC2.
 * @param  DMAC_Channel: specifies the selected DMAC channel.
 *         This parameter can be one of the following values:
 *         @arg @ref DMAC_Channel_0
 *         @arg @ref DMAC_Channel_1
 *         @arg @ref DMAC_Channel_2
 * @return The number of remaining data units in the current DMACx Channely block transfer.
 */
uint16_t DMAC_Channel_GetBlockTransferSize(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel)
{
  return (uint16_t)(DMACx->Ch[DMAC_Channel].CTLH & 0x00000FFFU);
}

/**
 * @brief  Forces or releases the current DMACx Channely transfer suspend.
 * @param  DMACx: Pointer to selected DMAC peripheral.
 *         This parameter can be one of the following values:
 *         DMAC1, DMAC2.
 * @param  DMAC_Channel: specifies the selected DMAC channel.
 *         This parameter can be one of the following values:
 *         @arg @ref DMAC_Channel_0
 *         @arg @ref DMAC_Channel_1
 *         @arg @ref DMAC_Channel_2
 * @param  NewState: new state of the current DMACx Channely transfer suspend.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None
 */
void DMAC_Channel_SuspendCmd(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, FunctionalState NewState)
{
  if (NewState != DISABLE) {
    DMACx->Ch[DMAC_Channel].CFGL |= (0x1U << 8);
  }
  else {
    DMACx->Ch[DMAC_Channel].CFGL &= (~(0x1U << 8));
  }
}

/**
 * @brief  Checks whether the DMACx Channely FIFO is empty or not.
 * @param  DMACx: Pointer to selected DMAC peripheral.
 *         This parameter can be one of the following values:
 *         DMAC1, DMAC2.
 * @param  DMAC_Channel: specifies the selected DMAC channel.
 *         This parameter can be one of the following values:
 *         @arg @ref DMAC_Channel_0
 *         @arg @ref DMAC_Channel_1
 *         @arg @ref DMAC_Channel_2
 * @return The new state of the DMACx Channely FIFO (SET or RESET).
 */
FlagStatus DMAC_Channel_IsFIFOEmpty(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel)
{
  FlagStatus bitstatus = RESET;

  /* Check the status of FIFO_EMPTY bit */
  if (DMACx->Ch[DMAC_Channel].CFGL & DMAC_CFGL_FIFO_EMPTY)
  {
    /* FIFO_EMPTY bit is set */
    bitstatus = SET;
  }
  else
  {
    /* FIFO_EMPTY bit is reset */
    bitstatus = RESET;
  }
  /* Return the FIFO_EMPTY bit status */
  return bitstatus;
}

/**
 * @brief  Enables or disables the specified DMACx peripheral.
 * @param  DMACx: Pointer to selected DMAC peripheral.
 *         This parameter can be one of the following values:
 *         DMAC1, DMAC2.
 * @param  NewState: new state of the DMACx peripheral.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None
 */
void DMAC_Cmd(DMAC_TypeDef* DMACx, FunctionalState NewState)
{
  if (NewState != DISABLE) {
    DMACx->DmaCfgReg = 0x01;
  }
  else {
    DMACx->DmaCfgReg = 0x00;
  }
}

/**
 * @brief  Returns the status of EN bit for the specified DMACx.
 * @param  DMACx: Pointer to selected DMAC peripheral.
 *         This parameter can be one of the following values:
 *         DMAC1, DMAC2.
 * @return Current state of the DMACx (ENABLE or DISABLE).
 */
FunctionalState DMAC_GetCmdStatus(DMAC_TypeDef* DMACx)
{
  FunctionalState state = DISABLE;

  if (DMACx->DmaCfgReg != 0)
  {
    state = ENABLE;
  }
  else
  {
    state = DISABLE;
  }
  return state;
}

/**
 * @brief  Enables or disables the specified DMACx Channely.
 * @param  DMACx: Pointer to selected DMAC peripheral.
 *         This parameter can be one of the following values:
 *         DMAC1, DMAC2.
 * @param  DMAC_Channel: specifies the selected DMAC channel.
 *         This parameter can be one of the following values:
 *         @arg @ref DMAC_Channel_0
 *         @arg @ref DMAC_Channel_1
 *         @arg @ref DMAC_Channel_2
 * @param  NewState: new state of the DMACx Channely.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None
 */
void DMAC_ChannelCmd(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, FunctionalState NewState)
{
  uint32_t mask = (uint32_t)(0x01U << DMAC_Channel);

  if (NewState != DISABLE) {
    DMACx->ChEnReg = (mask << 8) | mask;
  }
  else {
    DMACx->ChEnReg = (mask << 8);
  }
}

/**
 * @brief  Returns the status of EN bit for the specified DMACx Channely.
 * @param  DMACx: Pointer to selected DMAC peripheral.
 *         This parameter can be one of the following values:
 *         DMAC1, DMAC2.
 * @param  DMAC_Channel: specifies the selected DMAC channel.
 *         This parameter can be one of the following values:
 *         @arg @ref DMAC_Channel_0
 *         @arg @ref DMAC_Channel_1
 *         @arg @ref DMAC_Channel_2
 * @return Current state of the DMACx Channely (ENABLE or DISABLE).
 */
FunctionalState DMAC_GetChannelCmdStatus(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel)
{
  uint32_t mask = (uint32_t)(0x01U << DMAC_Channel);
  FunctionalState state = DISABLE;

  if ((DMACx->ChEnReg & mask) != 0)
  {
    state = ENABLE;
  }
  else
  {
    state = DISABLE;
  }
  return state;
}

/**
 * @brief  Enables or disables the specified DMACx Channely interrupts.
 * @param  DMACx: Pointer to selected DMAC peripheral.
 *         This parameter can be one of the following values:
 *         DMAC1, DMAC2.
 * @param  DMAC_Channel: specifies the selected DMAC channel.
 *         This parameter can be one of the following values:
 *         @arg @ref DMAC_Channel_0
 *         @arg @ref DMAC_Channel_1
 *         @arg @ref DMAC_Channel_2
 * @param  DMAC_IT: specifies the DMAC interrupt source to be enabled or disabled. 
 *         This parameter can be one of the following values:
 *         @arg @ref DMAC_IT_TFR: Transfer complete interrupt.
 *         @arg @ref DMAC_IT_BLOCK: Block complete interrupt.
 *         @arg @ref DMAC_IT_SRCTRAN: Source transaction complete interrupt.
 *         @arg @ref DMAC_IT_DSTTRAN: Destination transaction complete interrupt.
 *         @arg @ref DMAC_IT_ERR: Error interrupt.
 * @param  NewState: new state of the specified DMAC interrupt.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None.
 */
void DMAC_ITConfig(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, uint16_t DMAC_IT, FunctionalState NewState)
{
  uint32_t mask = (uint32_t)(0x01U << DMAC_Channel);
  uint32_t regaddr = ((uint32_t)(&DMACx->MaskTfr) + DMAC_IT);

  if (NewState != DISABLE)
  {
    /* Enable the selected DMAC interrupt */
    *((__IO uint32_t*)(regaddr)) = (mask << 8) | mask;
  }
  else
  {
    /* Disable the selected DMAC interrupt */
    *((__IO uint32_t*)(regaddr)) = (mask << 8);
  }
}

/**
 * @brief  Checks whether the specified DMACx Channely raw interrupt status.
 * @param  DMACx: Pointer to selected DMAC peripheral.
 *         This parameter can be one of the following values:
 *         DMAC1, DMAC2.
 * @param  DMAC_Channel: specifies the selected DMAC channel.
 *         This parameter can be one of the following values:
 *         @arg @ref DMAC_Channel_0
 *         @arg @ref DMAC_Channel_1
 *         @arg @ref DMAC_Channel_2
 * @param  DMAC_IT: specifies the DMAC interrupt source to check. 
 *         This parameter can be one of the following values:
 *         @arg @ref DMAC_IT_TFR: Transfer complete interrupt.
 *         @arg @ref DMAC_IT_BLOCK: Block complete interrupt.
 *         @arg @ref DMAC_IT_SRCTRAN: Source transaction complete interrupt.
 *         @arg @ref DMAC_IT_DSTTRAN: Destination transaction complete interrupt.
 *         @arg @ref DMAC_IT_ERR: Error interrupt.
 * @return The new state of raw DMAC_IT (SET or RESET).
 */
ITStatus DMAC_GetRawITStatus(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, uint16_t DMAC_IT)
{
  uint32_t mask = (uint32_t)(0x01U << DMAC_Channel);
  uint32_t regaddr = ((uint32_t)(&DMACx->RawTfr) + DMAC_IT);
  ITStatus bitstatus = RESET;

  if ((*((__I uint32_t*)(regaddr))) & mask)
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
 * @brief  Checks whether the specified DMACx Channely interrupt has occurred or not.
 * @param  DMACx: Pointer to selected DMAC peripheral.
 *         This parameter can be one of the following values:
 *         DMAC1, DMAC2.
 * @param  DMAC_Channel: specifies the selected DMAC channel.
 *         This parameter can be one of the following values:
 *         @arg @ref DMAC_Channel_0
 *         @arg @ref DMAC_Channel_1
 *         @arg @ref DMAC_Channel_2
 * @param  DMAC_IT: specifies the DMAC interrupt source to check. 
 *         This parameter can be one of the following values:
 *         @arg @ref DMAC_IT_TFR: Transfer complete interrupt.
 *         @arg @ref DMAC_IT_BLOCK: Block complete interrupt.
 *         @arg @ref DMAC_IT_SRCTRAN: Source transaction complete interrupt.
 *         @arg @ref DMAC_IT_DSTTRAN: Destination transaction complete interrupt.
 *         @arg @ref DMAC_IT_ERR: Error interrupt.
 * @return The new state of DMAC_IT (SET or RESET).
 */
ITStatus DMAC_GetITStatus(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, uint16_t DMAC_IT)
{
  uint32_t mask = (uint32_t)(0x01U << DMAC_Channel);
  uint32_t regaddr = ((uint32_t)(&DMACx->StatusTfr) + DMAC_IT);
  ITStatus bitstatus = RESET;

  if ((*((__I uint32_t*)(regaddr))) & mask)
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
 * @brief  Checks whether the specified DMACx interrupt has occurred or not.
 * @param  DMACx: Pointer to selected DMAC peripheral.
 *         This parameter can be one of the following values:
 *         DMAC1, DMAC2.
 * @param  DMAC_IT: specifies the DMAC interrupt source to check. 
 *         This parameter can be one of the following values:
 *         @arg @ref DMAC_IT_TFR: Transfer complete interrupt.
 *         @arg @ref DMAC_IT_BLOCK: Block complete interrupt.
 *         @arg @ref DMAC_IT_SRCTRAN: Source transaction complete interrupt.
 *         @arg @ref DMAC_IT_DSTTRAN: Destination transaction complete interrupt.
 *         @arg @ref DMAC_IT_ERR: Error interrupt.
 * @return The new state of DMAC_IT (SET or RESET).
 */
ITStatus DMAC_GetCombinedITStatus(DMAC_TypeDef* DMACx, uint16_t DMAC_IT)
{
  uint32_t bitmask = 0x01U << ((uint32_t)DMAC_IT / 8);
  ITStatus bitstatus = RESET;

  if (DMACx->StatusInt & bitmask)
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
 * @brief  Clears the DMACx's interrupt pending bits.
 * @param  DMACx: Pointer to selected DMAC peripheral.
 *         This parameter can be one of the following values:
 *         DMAC1, DMAC2.
 * @param  DMAC_Channel: specifies the selected DMAC channel.
 *         This parameter can be one of the following values:
 *         @arg @ref DMAC_Channel_0
 *         @arg @ref DMAC_Channel_1
 *         @arg @ref DMAC_Channel_2
 * @param  DMAC_IT: specifies the interrupt pending bit to clear.
 *         This parameter can be one of the following values:
 *         @arg @ref DMAC_IT_TFR: Transfer complete interrupt.
 *         @arg @ref DMAC_IT_BLOCK: Block complete interrupt.
 *         @arg @ref DMAC_IT_SRCTRAN: Source transaction complete interrupt.
 *         @arg @ref DMAC_IT_DSTTRAN: Destination transaction complete interrupt.
 *         @arg @ref DMAC_IT_ERR: Error interrupt.
 * @return None.
 */
void DMAC_ClearITPendingBit(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, uint16_t DMAC_IT)
{
  uint32_t mask = (uint32_t)(0x01U << DMAC_Channel);
  uint32_t regaddr = ((uint32_t)(&DMACx->ClearTfr) + DMAC_IT);

  *((__O uint32_t*)(regaddr)) = mask;
}



/**
 * @brief  Controls the state of ReqSrc signal for the corresponding channels.
 * @param  DMACx: Pointer to selected DMAC peripheral.
 *         This parameter can be one of the following values:
 *         DMAC1, DMAC2.
 * @param  DMAC_Channel: specifies the selected DMAC channel.
 *         This parameter can be one of the following values:
 *         @arg @ref DMAC_Channel_0
 *         @arg @ref DMAC_Channel_1
 *         @arg @ref DMAC_Channel_2
 * @param  NewState: new state of ReqSrc signal for the corresponding channels.
 *         This parameter can be: ACTIVE or INACTIVE.
 * @return None.
 */
void DMAC_SWHS_SetReqSrcSignalState(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, SignalState NewState)
{
  uint32_t mask = (uint32_t)(0x01U << DMAC_Channel);

  if (NewState != INACTIVE) {
    DMACx->ReqSrcReg = (mask << 8) | mask;
  }
  else {
    DMACx->ReqSrcReg = (mask << 8);
  }
}

/**
 * @brief  Controls the state of ReqDst signal for the corresponding channels.
 * @param  DMACx: Pointer to selected DMAC peripheral.
 *         This parameter can be one of the following values:
 *         DMAC1, DMAC2.
 * @param  DMAC_Channel: specifies the selected DMAC channel.
 *         This parameter can be one of the following values:
 *         @arg @ref DMAC_Channel_0
 *         @arg @ref DMAC_Channel_1
 *         @arg @ref DMAC_Channel_2
 * @param  NewState: new state of ReqDst signal for the corresponding channels.
 *         This parameter can be: ACTIVE or INACTIVE.
 * @return None.
 */
void DMAC_SWHS_SetReqDstSignalState(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, SignalState NewState)
{
  uint32_t mask = (uint32_t)(0x01U << DMAC_Channel);

  if (NewState != INACTIVE) {
    DMACx->ReqDstReg = (mask << 8) | mask;
  }
  else {
    DMACx->ReqDstReg = (mask << 8);
  }
}

/**
 * @brief  Controls the state of SglReqSrc signal for the corresponding channels.
 * @param  DMACx: Pointer to selected DMAC peripheral.
 *         This parameter can be one of the following values:
 *         DMAC1, DMAC2.
 * @param  DMAC_Channel: specifies the selected DMAC channel.
 *         This parameter can be one of the following values:
 *         @arg @ref DMAC_Channel_0
 *         @arg @ref DMAC_Channel_1
 *         @arg @ref DMAC_Channel_2
 * @param  NewState: new state of SglReqSrc signal for the corresponding channels.
 *         This parameter can be: ACTIVE or INACTIVE.
 * @return None.
 */
void DMAC_SWHS_SetSglReqSrcSignalState(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, SignalState NewState)
{
  uint32_t mask = (uint32_t)(0x01U << DMAC_Channel);

  if (NewState != INACTIVE) {
    DMACx->SglReqSrcReg = (mask << 8) | mask;
  }
  else {
    DMACx->SglReqSrcReg = (mask << 8);
  }
}

/**
 * @brief  Controls the state of SglReqDst signal for the corresponding channels.
 * @param  DMACx: Pointer to selected DMAC peripheral.
 *         This parameter can be one of the following values:
 *         DMAC1, DMAC2.
 * @param  DMAC_Channel: specifies the selected DMAC channel.
 *         This parameter can be one of the following values:
 *         @arg @ref DMAC_Channel_0
 *         @arg @ref DMAC_Channel_1
 *         @arg @ref DMAC_Channel_2
 * @param  NewState: new state of SglReqDst signal for the corresponding channels.
 *         This parameter can be: ACTIVE or INACTIVE.
 * @return None.
 */
void DMAC_SWHS_SetSglReqDstSignalState(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, SignalState NewState)
{
  uint32_t mask = (uint32_t)(0x01U << DMAC_Channel);

  if (NewState != INACTIVE) {
    DMACx->SglReqDstReg = (mask << 8) | mask;
  }
  else {
    DMACx->SglReqDstReg = (mask << 8);
  }
}

/**
 * @brief  Controls the state of LstSrc signal for the corresponding channels.
 * @param  DMACx: Pointer to selected DMAC peripheral.
 *         This parameter can be one of the following values:
 *         DMAC1, DMAC2.
 * @param  DMAC_Channel: specifies the selected DMAC channel.
 *         This parameter can be one of the following values:
 *         @arg @ref DMAC_Channel_0
 *         @arg @ref DMAC_Channel_1
 *         @arg @ref DMAC_Channel_2
 * @param  NewState: new state of LstSrc signal for the corresponding channels.
 *         This parameter can be: ACTIVE or INACTIVE.
 * @return None.
 */
void DMAC_SWHS_SetLstSrcSignalState(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, SignalState NewState)
{
  uint32_t mask = (uint32_t)(0x01U << DMAC_Channel);

  if (NewState != INACTIVE) {
    DMACx->LstSrcReg = (mask << 8) | mask;
  }
  else {
    DMACx->LstSrcReg = (mask << 8);
  }
}

/**
 * @brief  Controls the state of LstDst signal for the corresponding channels.
 * @param  DMACx: Pointer to selected DMAC peripheral.
 *         This parameter can be one of the following values:
 *         DMAC1, DMAC2.
 * @param  DMAC_Channel: specifies the selected DMAC channel.
 *         This parameter can be one of the following values:
 *         @arg @ref DMAC_Channel_0
 *         @arg @ref DMAC_Channel_1
 *         @arg @ref DMAC_Channel_2
 * @param  NewState: new state of LstDst signal for the corresponding channels.
 *         This parameter can be: ACTIVE or INACTIVE.
 * @return None.
 */
void DMAC_SWHS_SetLstDstSignalState(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, SignalState NewState)
{
  uint32_t mask = (uint32_t)(0x01U << DMAC_Channel);

  if (NewState != INACTIVE) {
    DMACx->LstDstReg = (mask << 8) | mask;
  }
  else {
    DMACx->LstDstReg = (mask << 8);
  }
}



/**
 * @brief  Returns the state of ReqSrc signal for the corresponding channel.
 * @param  DMACx: Pointer to selected DMAC peripheral.
 *         This parameter can be one of the following values:
 *         DMAC1, DMAC2.
 * @param  DMAC_Channel: specifies the selected DMAC channel.
 *         This parameter can be one of the following values:
 *         @arg @ref DMAC_Channel_0
 *         @arg @ref DMAC_Channel_1
 *         @arg @ref DMAC_Channel_2
 * @return Current state of ReqSrc signal for the corresponding channel (ACTIVE or INACTIVE).
 */
SignalState DMAC_SWHS_GetReqSrcSignalState(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel)
{
  uint32_t mask = (uint32_t)(0x01U << DMAC_Channel);
  SignalState state = INACTIVE;

  if ((DMACx->ReqSrcReg & mask) != 0)
  {
    state = ACTIVE;
  }
  else
  {
    state = INACTIVE;
  }
  return state;
}

/**
 * @brief  Returns the state of ReqDst signal for the corresponding channel.
 * @param  DMACx: Pointer to selected DMAC peripheral.
 *         This parameter can be one of the following values:
 *         DMAC1, DMAC2.
 * @param  DMAC_Channel: specifies the selected DMAC channel.
 *         This parameter can be one of the following values:
 *         @arg @ref DMAC_Channel_0
 *         @arg @ref DMAC_Channel_1
 *         @arg @ref DMAC_Channel_2
 * @return Current state of ReqDst signal for the corresponding channel (ACTIVE or INACTIVE).
 */
SignalState DMAC_SWHS_GetReqDstSignalState(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel)
{
  uint32_t mask = (uint32_t)(0x01U << DMAC_Channel);
  SignalState state = INACTIVE;

  if ((DMACx->ReqDstReg & mask) != 0)
  {
    state = ACTIVE;
  }
  else
  {
    state = INACTIVE;
  }
  return state;
}

/**
 * @brief  Returns the state of SglReqSrc signal for the corresponding channel.
 * @param  DMACx: Pointer to selected DMAC peripheral.
 *         This parameter can be one of the following values:
 *         DMAC1, DMAC2.
 * @param  DMAC_Channel: specifies the selected DMAC channel.
 *         This parameter can be one of the following values:
 *         @arg @ref DMAC_Channel_0
 *         @arg @ref DMAC_Channel_1
 *         @arg @ref DMAC_Channel_2
 * @return Current state of SglReqSrc signal for the corresponding channel (ACTIVE or INACTIVE).
 */
SignalState DMAC_SWHS_GetSglReqSrcSignalState(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel)
{
  uint32_t mask = (uint32_t)(0x01U << DMAC_Channel);
  SignalState state = INACTIVE;

  if ((DMACx->SglReqSrcReg & mask) != 0)
  {
    state = ACTIVE;
  }
  else
  {
    state = INACTIVE;
  }
  return state;
}

/**
 * @brief  Returns the state of SglReqDst signal for the corresponding channel.
 * @param  DMACx: Pointer to selected DMAC peripheral.
 *         This parameter can be one of the following values:
 *         DMAC1, DMAC2.
 * @param  DMAC_Channel: specifies the selected DMAC channel.
 *         This parameter can be one of the following values:
 *         @arg @ref DMAC_Channel_0
 *         @arg @ref DMAC_Channel_1
 *         @arg @ref DMAC_Channel_2
 * @return Current state of SglReqDst signal for the corresponding channel (ACTIVE or INACTIVE).
 */
SignalState DMAC_SWHS_GetSglReqDstSignalState(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel)
{
  uint32_t mask = (uint32_t)(0x01U << DMAC_Channel);
  SignalState state = INACTIVE;

  if ((DMACx->SglReqDstReg & mask) != 0)
  {
    state = ACTIVE;
  }
  else
  {
    state = INACTIVE;
  }
  return state;
}

/**
 * @brief  Returns the state of LstSrc signal for the corresponding channel.
 * @param  DMACx: Pointer to selected DMAC peripheral.
 *         This parameter can be one of the following values:
 *         DMAC1, DMAC2.
 * @param  DMAC_Channel: specifies the selected DMAC channel.
 *         This parameter can be one of the following values:
 *         @arg @ref DMAC_Channel_0
 *         @arg @ref DMAC_Channel_1
 *         @arg @ref DMAC_Channel_2
 * @return Current state of LstSrc signal for the corresponding channel (ACTIVE or INACTIVE).
 */
SignalState DMAC_SWHS_GetLstSrcSignalState(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel)
{
  uint32_t mask = (uint32_t)(0x01U << DMAC_Channel);
  SignalState state = INACTIVE;

  if ((DMACx->LstSrcReg & mask) != 0)
  {
    state = ACTIVE;
  }
  else
  {
    state = INACTIVE;
  }
  return state;
}

/**
 * @brief  Returns the state of LstDst signal for the corresponding channel.
 * @param  DMACx: Pointer to selected DMAC peripheral.
 *         This parameter can be one of the following values:
 *         DMAC1, DMAC2.
 * @param  DMAC_Channel: specifies the selected DMAC channel.
 *         This parameter can be one of the following values:
 *         @arg @ref DMAC_Channel_0
 *         @arg @ref DMAC_Channel_1
 *         @arg @ref DMAC_Channel_2
 * @return Current state of LstDst signal for the corresponding channel (ACTIVE or INACTIVE).
 */
SignalState DMAC_SWHS_GetLstDstSignalState(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel)
{
  uint32_t mask = (uint32_t)(0x01U << DMAC_Channel);
  SignalState state = INACTIVE;

  if ((DMACx->LstDstReg & mask) != 0)
  {
    state = ACTIVE;
  }
  else
  {
    state = INACTIVE;
  }
  return state;
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

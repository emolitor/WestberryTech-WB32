/**
 * @file    wb32f10x_uart.c
 * @author  Westberry Application Team
 * @version V0.1.10
 * @date    05-January-2023
 * @brief   This file provides all the UART firmware functions.
 */

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x_uart.h"
#include "wb32f10x_rcc.h"

/** @addtogroup WB32F10x_StdPeriph_Driver
  * @{
  */

/** @defgroup UART
  * @brief UART driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup UART_Private_Functions
  * @{
  */

/**
 * @brief  Deinitializes the UARTx peripheral registers to their default reset values.
 * @param  UARTx: Pointer to selected UART peripheral.
 *         This parameter can be one of the following values:
 *         UART1, UART2, UART3.
 * @return None
 */
void UART_DeInit(UART_TypeDef* UARTx)
{
  if (UARTx == UART1)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART1, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART1, DISABLE);
  }
  else if (UARTx == UART2)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_UART2, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_UART2, DISABLE);
  }
  else if (UARTx == UART3)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_UART3, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_UART3, DISABLE);
  }
}

/**
 * @brief  Initializes the UARTx peripheral according to the specified
 *         parameters in the UART_InitStruct.
 * @param  UARTx: Pointer to selected UART peripheral.
 *         This parameter can be one of the following values:
 *         UART1, UART2, UART3.
 * @param  UART_InitStruct: pointer to a UART_InitTypeDef structure
 *         that contains the configuration information for the specified 
 *         UART peripheral.
 * @return None
 */
void UART_Init(UART_TypeDef* UARTx, UART_InitTypeDef* UART_InitStruct)
{
  uint32_t divider, apbclock;
  RCC_ClocksTypeDef RCC_ClocksStatus;

  UARTx->MCR = (UARTx->MCR & 0x50) | UART_InitStruct->UART_AutoFlowControl;

  RCC_GetClocksFreq(&RCC_ClocksStatus);
  if (UARTx == UART1)
  {
    apbclock = RCC_ClocksStatus.APB1CLK_Frequency;
  }
  else
  {
    apbclock = RCC_ClocksStatus.APB2CLK_Frequency;
  }

  if (UART_InitStruct->UART_BaudRate != 0)
  {
    // round off
    divider = (apbclock + (UART_InitStruct->UART_BaudRate >> 1)) / UART_InitStruct->UART_BaudRate;
  }
  else
  {
    divider = 0;
  }

  UARTx->DLF = divider & 0x0F;
  UARTx->LCR = UART_LCR_DLAB;
  UARTx->DLL = (uint8_t)(divider >> 4);
  UARTx->DLH = (uint8_t)(divider >> 12);
  UARTx->LCR = 0x00;
  UARTx->LCR = UART_InitStruct->UART_WordLength | UART_InitStruct->UART_StopBits | UART_InitStruct->UART_Parity;
}

/**
 * @brief  Fills each UART_InitStruct member with its default value.
 * @param  UART_InitStruct: pointer to a UART_InitTypeDef structure
 *         which will be initialized.
 * @return None
 */
void UART_StructInit(UART_InitTypeDef* UART_InitStruct)
{
  /* UART_InitStruct members default value */
  UART_InitStruct->UART_BaudRate = 9600;
  UART_InitStruct->UART_WordLength = UART_WordLength_8b;
  UART_InitStruct->UART_StopBits = UART_StopBits_One;
  UART_InitStruct->UART_Parity = UART_Parity_None;
  UART_InitStruct->UART_AutoFlowControl = UART_AutoFlowControl_None;
}

/**
 * @brief  Write data to THR register or Tx FIFO.
 * @param  UARTx: Pointer to selected UART peripheral.
 *         This parameter can be one of the following values:
 *         UART1, UART2, UART3.
 * @param  Data: the data to write.
 * @return None
 */
void UART_WriteData(UART_TypeDef* UARTx, uint16_t Data)
{
  UARTx->THR = Data;
}

/**
 * @brief  Read data from RBR register or Rx FIFO.
 * @param  UARTx: Pointer to selected UART peripheral.
 *         This parameter can be one of the following values:
 *         UART1, UART2, UART3.
 * @return The received data
 */
uint16_t UART_ReadData(UART_TypeDef* UARTx)
{
  return (uint16_t)UARTx->RBR;
}

/**
 * @brief  Configure UARTx's Rx FIFO trigger threshold.
 * @param  UARTx: Pointer to selected UART peripheral.
 *         This parameter can be one of the following values:
 *         UART1, UART2, UART3.
 * @param  Threshold: This parameter can be one of the following values:
 *         @arg @ref UART_RxFIFOThreshold_1
 *         @arg @ref UART_RxFIFOThreshold_4
 *         @arg @ref UART_RxFIFOThreshold_8
 *         @arg @ref UART_RxFIFOThreshold_14
 * @return None
 */
void UART_RxFIFOThresholdConfig(UART_TypeDef* UARTx, uint8_t Threshold)
{
  UARTx->SRT = (uint32_t)Threshold;
}

/**
 * @brief  Configure UARTx's Tx FIFO trigger threshold.
 * @param  UARTx: Pointer to selected UART peripheral.
 *         This parameter can be one of the following values:
 *         UART1, UART2, UART3.
 * @param  Threshold: This parameter can be one of the following values:
 *         @arg @ref UART_TxFIFOThreshold_0
 *         @arg @ref UART_TxFIFOThreshold_2
 *         @arg @ref UART_TxFIFOThreshold_4
 *         @arg @ref UART_TxFIFOThreshold_8
 * @return None
 */
void UART_TxFIFOThresholdConfig(UART_TypeDef* UARTx, uint8_t Threshold)
{
  UARTx->STET = (uint32_t)Threshold;
}

/**
 * @brief  Enables or disables the UARTx's FIFO.
 * @param  UARTx: Pointer to selected UART peripheral.
 *         This parameter can be one of the following values:
 *         UART1, UART2, UART3.
 * @param  NewState: new state of the FIFO.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None
 */
void UART_FIFOCmd(UART_TypeDef* UARTx, FunctionalState NewState)
{
  if (NewState != DISABLE) {
    UARTx->SFE = 0x01;
  }
  else {
    UARTx->SFE = 0x00;
  }
}

/**
 * @brief  Reset the UART's Rx FIFO.
 * @param  UARTx: Pointer to selected UART peripheral.
 *         This parameter can be one of the following values:
 *         UART1, UART2, UART3.
 * @return None
 */
void UART_ResetRxFIFO(UART_TypeDef* UARTx)
{
  UARTx->SRR = UART_SRR_RFR;
}

/**
 * @brief  Reset the UART's Tx FIFO.
 * @param  UARTx: Pointer to selected UART peripheral.
 *         This parameter can be one of the following values:
 *         UART1, UART2, UART3.
 * @return None
 */
void UART_ResetTxFIFO(UART_TypeDef* UARTx)
{
  UARTx->SRR = UART_SRR_XFR;
}

/**
 * @brief  Force the serial out to spacing state or not.
 * @param  UARTx: Pointer to selected UART peripheral.
 *         This parameter can be one of the following values:
 *         UART1, UART2, UART3.
 * @param  NewState: new state of the Break control bit.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None
 */
void UART_ForceBreakCmd(UART_TypeDef* UARTx, FunctionalState NewState)
{
  if (NewState != DISABLE) {
    UARTx->SBCR = 0x01;
  }
  else {
    UARTx->SBCR = 0x00;
  }
}

/**
 * @brief  Enables or disables the UARTx's IrDA SIR Mode.
 * @param  UARTx: Pointer to selected UART peripheral.
 *         This parameter can be one of the following values:
 *         UART1, UART2, UART3.
 * @param  NewState: new state of the IrDA SIR Mode.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None
 */
void UART_IrDACmd(UART_TypeDef* UARTx, FunctionalState NewState)
{
  if (NewState != DISABLE) {
    UARTx->MCR |= UART_MCR_SIRE;
  }
  else {
    UARTx->MCR &= ~UART_MCR_SIRE;
  }
}

/**
 * @brief  Enables or disables the UARTx's Programmable THRE Interrupt Mode.
 * @param  UARTx: Pointer to selected UART peripheral.
 *         This parameter can be one of the following values:
 *         UART1, UART2, UART3.
 * @param  NewState: new state of the Programmable THRE Interrupt Mode.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None
 */
void UART_ProgrammableTHREModeCmd(UART_TypeDef* UARTx, FunctionalState NewState)
{
  if (NewState != DISABLE) {
    UARTx->IER |= UART_IER_PTIME;
  }
  else {
    UARTx->IER &= ~UART_IER_PTIME;
  }
}

/**
 * @brief  Select the method for clearing the status in the LSR register.
 * @note   This is applicable only for Overrun Error, Parity Error, Framing Error, 
 *         and Break Interrupt status bits.
 * @param  UARTx: Pointer to selected UART peripheral.
 *         This parameter can be one of the following values:
 *         UART1, UART2, UART3.
 * @param  Mode: the method for clearing the status in the LSR register.
 *         This parameter can be: 0 or 1.
 *         @arg 0: LSR status bits are cleared either on reading Rx FIFO (RBR Read) or on reading LSR register.
 *         @arg 1: LSR status bits are cleared only on reading LSR register.
 * @return None
 */
void UART_LineStatusClearModeConfig(UART_TypeDef* UARTx, uint8_t Mode)
{
  if (Mode != 0) {
    UARTx->IER |= UART_IER_LSRCLRMD;
  }
  else {
    UARTx->IER &= ~UART_IER_LSRCLRMD;
  }
}

/**
 * @brief  Enables or disables the UARTx's interrupts.
 * @param  UARTx: Pointer to selected UART peripheral.
 *         This parameter can be one of the following values:
 *         UART1, UART2, UART3.
 * @param  UART_IT: specifies the UART interrupt sources to be enabled or disabled.
 *         This parameter can be a combination of the following values:
 *         @arg @ref UART_IT_RDA
 *         @arg @ref UART_IT_THRE
 *         @arg @ref UART_IT_RLS
 *         @arg @ref UART_IT_MSI
 * @param  NewState: new state of the interrupts.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None
 */
void UART_ITConfig(UART_TypeDef* UARTx, uint8_t UART_IT, FunctionalState NewState)
{
  if (NewState != DISABLE) {
    UARTx->IER |= UART_IT;
  }
  else {
    UARTx->IER &= ~UART_IT;
  }
}

/**
 * @brief  Get UARTx's Interrupt ID.
 * @param  UARTx: Pointer to selected UART peripheral.
 *         This parameter can be one of the following values:
 *         UART1, UART2, UART3.
 * @return Interrupt ID.
 */
uint8_t UART_GetIntID(UART_TypeDef* UARTx)
{
  return UARTx->IIR & UART_IIR_INTID_Msk;
}

/**
 * @brief  Checks whether the specified UART flag is set or not.
 * @param  UARTx: Pointer to selected UART peripheral.
 *         This parameter can be one of the following values:
 *         UART1, UART2, UART3.
 * @param  UART_FLAG: specifies the flag to check.
 *         This parameter can be one of the following values:
 *         @arg @ref UART_FLAG_BUSY: Busy flag.
 *         @arg @ref UART_FLAG_TFNF: Transmit FIFO Not Full flag.
 *         @arg @ref UART_FLAG_TFE: Transmit FIFO Empty flag.
 *         @arg @ref UART_FLAG_RFNE: Receive FIFO Not Empty flag.
 *         @arg @ref UART_FLAG_RFF: Receive FIFO Full flag.
 * @return The new state of UART_FLAG (SET or RESET).
 */
FlagStatus UART_GetFlagStatus(UART_TypeDef* UARTx, uint32_t UART_FLAG)
{
  FlagStatus bitstatus = RESET;

  if ((UARTx->USR & UART_FLAG) != (uint32_t)RESET)
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
 * @brief  Get UARTx's Line status.
 * @param  UARTx: Pointer to selected UART peripheral.
 *         This parameter can be one of the following values:
 *         UART1, UART2, UART3.
 * @return UARTx's Line status.
 */
uint32_t UART_GetLineStatus(UART_TypeDef* UARTx)
{
  return UARTx->LSR;
}

/**
 * @brief  Get UARTx's Modem Status.
 * @param  UARTx: Pointer to selected UART peripheral.
 *         This parameter can be one of the following values:
 *         UART1, UART2, UART3.
 * @return UARTx's Modem Status.
 */
uint32_t UART_GetModemStatus(UART_TypeDef* UARTx)
{
  return UARTx->MSR;
}

/**
 * @brief  Enables or disables the UARTx's LoopBack Mode.
 * @param  UARTx: Pointer to selected UART peripheral.
 *         This parameter can be one of the following values:
 *         UART1, UART2, UART3.
 * @param  NewState: new state of the LoopBack Mode.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None
 */
void UART_LoopBackModeCmd(UART_TypeDef* UARTx, FunctionalState NewState)
{
  if (NewState != DISABLE) {
    UARTx->MCR |= UART_MCR_LB;
  }
  else {
    UARTx->MCR &= ~UART_MCR_LB;
  }
}

/**
 * @brief  Control RTS Pin State.
 * @param  UARTx: Pointer to selected UART peripheral.
 *         This parameter can be one of the following values:
 *         UART1, UART2, UART3.
 * @param  NewState: new state of the RTS Pin State.
 *         This parameter can be: ACTIVE or INACTIVE.
 * @return None
 */
void UART_ModemForceRtsPinState(UART_TypeDef* UARTx, SignalState NewState)
{
  if (NewState != INACTIVE) {
    UARTx->MCR |= UART_MCR_RTS;
  }
  else {
    UARTx->MCR &= ~UART_MCR_RTS;
  }
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

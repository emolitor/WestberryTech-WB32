/**
 * @file    wb32f10x_uart.h
 * @author  Westberry Application Team
 * @version V0.1.10
 * @date    05-January-2023
 * @brief   This file contains all the functions prototypes for the UART firmware
 *          library.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WB32F10x_UART_H
#define __WB32F10x_UART_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/** @addtogroup WB32F10x_StdPeriph_Driver
  * @{
  */

/** @addtogroup UART
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  UART Init Structure definition  
  */
typedef struct
{
  uint32_t UART_BaudRate;         /*!< This member configures the UART communication baud rate. */

  uint8_t  UART_WordLength;       /*!< Specifies the number of data bits transmitted or received in a frame.
                                       This parameter can be a value of @ref UART_Word_Length */

  uint8_t  UART_StopBits;         /*!< Specifies the number of stop bits transmitted.
                                      This parameter can be a value of @ref UART_Stop_Bits */

  uint8_t  UART_Parity;           /*!< Specifies the parity mode.
                                       This parameter can be a value of @ref UART_Parity */

  uint8_t  UART_AutoFlowControl;  /*!< Specifies the auto flow control mode is enabled
                                       or disabled.
                                       This parameter can be a value of @ref UART_AutoFlowControl */
} UART_InitTypeDef;

/* Exported constants --------------------------------------------------------*/

/** @defgroup UART_Exported_Constants
  * @{
  */

/** @defgroup UART_Word_Length 
  * @{
  */
#define UART_WordLength_5b          UART_LCR_WLS_5BIT
#define UART_WordLength_6b          UART_LCR_WLS_6BIT
#define UART_WordLength_7b          UART_LCR_WLS_7BIT
#define UART_WordLength_8b          UART_LCR_WLS_8BIT
/**
  * @}
  */


/** @defgroup UART_Stop_Bits 
  * @{
  */
#define UART_StopBits_One           UART_LCR_SBS_1BIT
#define UART_StopBits_Two           UART_LCR_SBS_2BIT
#define UART_StopBits_OnePointFive  UART_LCR_SBS_2BIT
/**
  * @}
  */


/** @defgroup UART_Parity 
  * @{
  */
#define UART_Parity_None            (0x00U)
#define UART_Parity_Odd             (UART_LCR_PARITY_ODD)
#define UART_Parity_Even            (UART_LCR_PARITY_EVEN)
#define UART_Parity_Mark            (UART_LCR_PARITY_MARK)
#define UART_Parity_Space           (UART_LCR_PARITY_SPACE)
/**
  * @}
  */


/** @defgroup UART_AutoFlowControl 
  * @{
  */
#define UART_AutoFlowControl_None       (0x00)
#define UART_AutoFlowControl_CTS        (UART_MCR_AFCE)
#define UART_AutoFlowControl_RTS_CTS    (UART_MCR_AFCE | UART_MCR_RTS)
/**
  * @}
  */


/** @defgroup UART_RxFIFOThreshold 
  * @{
  */
#define UART_RxFIFOThreshold_1    0x00
#define UART_RxFIFOThreshold_4    0x01
#define UART_RxFIFOThreshold_8    0x02
#define UART_RxFIFOThreshold_14   0x03
/**
  * @}
  */


/** @defgroup UART_TxFIFOThreshold 
  * @{
  */
#define UART_TxFIFOThreshold_0    0x00
#define UART_TxFIFOThreshold_2    0x01
#define UART_TxFIFOThreshold_4    0x02
#define UART_TxFIFOThreshold_8    0x03
/**
  * @}
  */


/** @defgroup UART_IT 
  * @{
  */
#define UART_IT_RDA                 UART_IER_RDAIE
#define UART_IT_THRE                UART_IER_THREIE
#define UART_IT_RLS                 UART_IER_RLSIE
#define UART_IT_MSI                 UART_IER_MSIE
/**
  * @}
  */



/** @defgroup UART_INTID 
  * @{
  */
#define UART_INTID_MSI              UART_IIR_INTID_MSI    /*!< Modem status interrupt */
#define UART_INTID_NONE             UART_IIR_INTID_NONE   /*!< No interrupt pending */
#define UART_INTID_THRE             UART_IIR_INTID_THRE   /*!< Transmitter holding register empty */
#define UART_INTID_RDA              UART_IIR_INTID_RDA    /*!< Received data available interrupt */
#define UART_INTID_RLS              UART_IIR_INTID_RLS    /*!< Receiver line status interrupt */
#define UART_INTID_BUSY             UART_IIR_INTID_BUSY   /*!< Busy detect */
#define UART_INTID_CTI              UART_IIR_INTID_CTI    /*!< character timeout indicator */
/**
  * @}
  */


/** @defgroup UART_Flags 
  * @{
  */
#define UART_FLAG_BUSY              UART_USR_BUSY
#define UART_FLAG_TFNF              UART_USR_TFNF
#define UART_FLAG_TFE               UART_USR_TFE
#define UART_FLAG_RFNE              UART_USR_RFNE
#define UART_FLAG_RFF               UART_USR_RFF
/**
  * @}
  */


/** @defgroup UART_LINE_STATUS 
  * @{
  */
#define UART_LINE_STATUS_DR         UART_LSR_DR
#define UART_LINE_STATUS_OE         UART_LSR_OE
#define UART_LINE_STATUS_PE         UART_LSR_PE
#define UART_LINE_STATUS_FE         UART_LSR_FE
#define UART_LINE_STATUS_BI         UART_LSR_BI
#define UART_LINE_STATUS_THRE       UART_LSR_THRE
#define UART_LINE_STATUS_TEMT       UART_LSR_TEMT
#define UART_LINE_STATUS_RFE        UART_LSR_RFE
#define UART_LINE_STATUS_ADDR_RCVD  UART_LSR_ADDR_RCVD
/**
  * @}
  */


/** @defgroup UART_MODEM_STATUS 
  * @{
  */
#define UART_MODEM_STATUS_DCTS      UART_MSR_DCTS
#define UART_MODEM_STATUS_CTS       UART_MSR_CTS
/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void UART_DeInit(UART_TypeDef* UARTx);
void UART_Init(UART_TypeDef* UARTx, UART_InitTypeDef* UART_InitStruct);
void UART_StructInit(UART_InitTypeDef* UART_InitStruct);
void UART_WriteData(UART_TypeDef* UARTx, uint16_t Data);
uint16_t UART_ReadData(UART_TypeDef* UARTx);
void UART_RxFIFOThresholdConfig(UART_TypeDef* UARTx, uint8_t Threshold);
void UART_TxFIFOThresholdConfig(UART_TypeDef* UARTx, uint8_t Threshold);
void UART_FIFOCmd(UART_TypeDef* UARTx, FunctionalState NewState);
void UART_ResetRxFIFO(UART_TypeDef* UARTx);
void UART_ResetTxFIFO(UART_TypeDef* UARTx);
void UART_ForceBreakCmd(UART_TypeDef* UARTx, FunctionalState NewState);
void UART_IrDACmd(UART_TypeDef* UARTx, FunctionalState NewState);
void UART_ProgrammableTHREModeCmd(UART_TypeDef* UARTx, FunctionalState NewState);
void UART_LineStatusClearModeConfig(UART_TypeDef* UARTx, uint8_t Mode);
void UART_ITConfig(UART_TypeDef* UARTx, uint8_t UART_IT, FunctionalState NewState);
uint8_t UART_GetIntID(UART_TypeDef* UARTx);
FlagStatus UART_GetFlagStatus(UART_TypeDef* UARTx, uint32_t UART_FLAG);
uint32_t UART_GetLineStatus(UART_TypeDef* UARTx);
uint32_t UART_GetModemStatus(UART_TypeDef* UARTx);
void UART_LoopBackModeCmd(UART_TypeDef* UARTx, FunctionalState NewState);
void UART_ModemForceRtsPinState(UART_TypeDef* UARTx, SignalState NewState);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __WB32F10x_UART_H */

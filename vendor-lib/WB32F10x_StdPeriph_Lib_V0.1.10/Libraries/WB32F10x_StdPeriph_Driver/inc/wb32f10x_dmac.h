/**
 * @file    wb32f10x_dmac.h
 * @author  Westberry Application Team
 * @version V0.1.10
 * @date    05-January-2023
 * @brief   This file contains all the functions prototypes for the DMAC firmware
 *          library.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WB32F10x_DMAC_H
#define __WB32F10x_DMAC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/** @addtogroup WB32F10x_StdPeriph_Driver
  * @{
  */

/** @addtogroup DMAC
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  DMAC Source Gather Structure definition  
  */
typedef struct
{
  FunctionalState DMAC_SourceGather;
  uint32_t DMAC_SourceGatherCount;
  uint32_t DMAC_SourceGatherInterval;
} DMAC_SourceGatherInitTypeDef;

/** 
  * @brief  DMAC Destination Scatter Structure definition  
  */
typedef struct
{
  FunctionalState DMAC_DestinationScatter;
  uint32_t DMAC_DestinationScatterCount;
  uint32_t DMAC_DestinationScatterInterval;
} DMAC_DestinationScatterInitTypeDef;

/** 
  * @brief  DMAC Channel Init Structure definition  
  */
typedef struct
{
  uint32_t DMAC_SourceBaseAddr;                                 /*!< Specifies the source base address. */

  uint32_t DMAC_DestinationBaseAddr;                            /*!< Specifies the destination base address. */

  uint32_t DMAC_Interrupt;                                      /*!< Specifies the global interrupt enable/disable for the channel.  
                                                                     This parameter can be a value of @ref DMAC_Interrupt. */

  uint32_t DMAC_SourceTransferWidth;                            /*!< Specifies the source transfer width.  
                                                                     This parameter can be a value of @ref DMAC_SourceTransferWidth. */

  uint32_t DMAC_DestinationTransferWidth;                       /*!< Specifies the destination transfer width.  
                                                                     This parameter can be a value of @ref DMAC_DestinationTransferWidth. */

  uint32_t DMAC_SourceAddrInc;                                  /*!< Specifies whether to increment or decrement the source address on every source transfer.  
                                                                     This parameter can be a value of @ref DMAC_SourceAddrInc. */

  uint32_t DMAC_DestinationAddrInc;                             /*!< Specifies whether to increment or decrement the destination address on every destination transfer.  
                                                                     This parameter can be a value of @ref DMAC_DestinationAddrInc. */

  uint32_t DMAC_SourceTransactionLength;                        /*!< Specifies the source burst transaction length.  
                                                                     This parameter can be a value of @ref DMAC_SourceTransactionLength.
                                                                     @note This parameter has no effect if the source is memory. */

  uint32_t DMAC_DestinationTransactionLength;                   /*!< Specifies the destination burst transaction length.  
                                                                     This parameter can be a value of @ref DMAC_DestinationTransactionLength.
                                                                     @note This parameter has no effect if the destination is memory. */

  uint32_t DMAC_TransferTypeAndFlowControl;                     /*!< Specifies the transfer type and flow control.  
                                                                     This parameter can be a value of @ref DMAC_TransferTypeAndFlowControl. */

  uint32_t DMAC_SourceMasterInterface;                          /*!< Specifies the Master Interface layer from which the source device (peripheral or memory) is accessed.  
                                                                     This parameter can be a value of @ref DMAC_SourceMasterInterface. */

  uint32_t DMAC_DestinationMasterInterface;                     /*!< Specifies the Master Interface layer where the destination device (peripheral or memory) resides.  
                                                                     This parameter can be a value of @ref DMAC_DestinationMasterInterface. */

  uint32_t DMAC_BlockTransferSize;                              /*!< Specifies the block transfer size when the DMAC is the flow controller, in data unit. \n
                                                                     The data unit is equal to the configuration set in DMAC_SourceTransferWidth.  
                                                                     This parameter can be a number between 0 and 511. */

  uint32_t DMAC_SourceHandshakingInterfaceSelect;               /*!< Specifies which of the handshaking interfaces - hardware or software - is active for
                                                                     source requests on this channel.   
                                                                     This parameter can be a value of @ref DMAC_SourceHandshakingInterfaceSelect. */

  uint32_t DMAC_DestinationHandshakingInterfaceSelect;          /*!< Specifies which of the handshaking interfaces - hardware or software - is active for
                                                                     destination requests on this channel.  
                                                                     This parameter can be a value of @ref DMAC_DestinationHandshakingInterfaceSelect. */

  uint32_t DMAC_SourceHandshakingInterfacePolarity;             /*!< Specifies the source handshaking interface polarity.  
                                                                     This parameter can be a value of @ref DMAC_SourceHandshakingInterfacePolarity. */

  uint32_t DMAC_DestinationHandshakingInterfacePolarity;        /*!< Specifies the destination handshaking interface polarity.  
                                                                     This parameter can be a value of @ref DMAC_DestinationHandshakingInterfacePolarity. */

  uint32_t DMAC_AutomaticSourceReload;                          /*!< Specifies the automatic source reload feature enable or not.  
                                                                     This parameter can be a value of @ref DMAC_AutomaticSourceReload.
                                                                     @note This parameter is valid only for DMACx channel0. */

  uint32_t DMAC_AutomaticDestinationReload;                     /*!< Specifies the automatic destination reload feature enable or not.  
                                                                     This parameter can be a value of @ref DMAC_AutomaticDestinationReload.
                                                                     @note This parameter is valid only for DMACx channel0. */

  uint32_t DMAC_FlowControlMode;                                /*!< Specifies the flow control mode.  
                                                                     This parameter can be a value of @ref DMAC_FlowControlMode. */

  uint32_t DMAC_FIFOMode;                                       /*!< Specifies the FIFO mode.  
                                                                     This parameter can be a value of @ref DMAC_FIFOMode. */

  uint8_t  DMAC_ChannelPriority;                                /*!< Specifies the channel priority from 0 to the maximum channel number. \n
                                                                     A low priority number means low priority. 0 is the lowest priority.  
                                                                     This parameter can be a number between 0 and 2. */

  uint8_t  DMAC_ProtectionControl;                              /*!< Specifies the protection control, used to drive the AHB HPROT[3:1] bus.  
                                                                     The AMBA Specification recommends that the default value of HPROT
                                                                     indicates a non-cached, non-buffered, privileged data access. \n
                                                                     Therefore, this parameter is recommended to be 0x01. */

  uint8_t  DMAC_SourceHardwareHandshakingInterfaceAssign;       /*!< Specifies the hardware handshaking interface to the source of this channel if
                                                                     the configuration DMAC_SourceHandshakingInterfaceSelect is DMAC_SourceHandshakingInterfaceSelect_Hardware.  
                                                                     This parameter can be a value of @ref DMAC_HardwareHandshakingInterface. */

  uint8_t  DMAC_DestinationHardwareHandshakingInterfaceAssign;  /*!< Specifies the hardware handshaking interface to the source of this channel if
                                                                     the configuration DMAC_DestinationHandshakingInterfaceSelect is DMAC_DestinationHandshakingInterfaceSelect_Hardware.  
                                                                     This parameter can be a value of @ref DMAC_HardwareHandshakingInterface. */

  uint16_t DMAC_MaximumAMBABurstLength;                         /*!< Specifies the Maximum AMBA burst length that is used for DMA transfers on this channel. \n
                                                                     A value of 0 indicates that software is not limiting the maximum AMBA burst length
                                                                     for DMA transfers on this channel.  
                                                                     This parameter is recommended to be 0. */
} DMAC_Channel_InitTypeDef;

/* Exported constants --------------------------------------------------------*/

/** @defgroup DMAC_Exported_Constants
  * @{
  */

/** @defgroup DMAC_Interrupt 
  * @{
  */
#define DMAC_Interrupt_Enable     ((uint32_t)0x00000001)
#define DMAC_Interrupt_Disable    ((uint32_t)0x00000000)
/**
  * @}
  */


/** @defgroup DMAC_SourceTransferWidth 
  * @{
  */
#define DMAC_SourceTransferWidth_8b         DMAC_CTLL_SRC_TR_WIDTH_8
#define DMAC_SourceTransferWidth_16b        DMAC_CTLL_SRC_TR_WIDTH_16
#define DMAC_SourceTransferWidth_32b        DMAC_CTLL_SRC_TR_WIDTH_32
/**
  * @}
  */


/** @defgroup DMAC_DestinationTransferWidth 
  * @{
  */
#define DMAC_DestinationTransferWidth_8b    DMAC_CTLL_DST_TR_WIDTH_8
#define DMAC_DestinationTransferWidth_16b   DMAC_CTLL_DST_TR_WIDTH_16
#define DMAC_DestinationTransferWidth_32b   DMAC_CTLL_DST_TR_WIDTH_32
/**
  * @}
  */


/** @defgroup DMAC_SourceAddrInc 
  * @{
  */
#define DMAC_SourceAddrInc_Increment        DMAC_CTLL_SINC_INC
#define DMAC_SourceAddrInc_Decrement        DMAC_CTLL_SINC_DEC
#define DMAC_SourceAddrInc_NoChange         DMAC_CTLL_SINC_NO
/**
  * @}
  */


/** @defgroup DMAC_DestinationAddrInc 
  * @{
  */
#define DMAC_DestinationAddrInc_Increment   DMAC_CTLL_DINC_INC
#define DMAC_DestinationAddrInc_Decrement   DMAC_CTLL_DINC_DEC
#define DMAC_DestinationAddrInc_NoChange    DMAC_CTLL_DINC_NO
/**
  * @}
  */


/** @defgroup DMAC_SourceTransactionLength 
  * @{
  */
#define DMAC_SourceTransactionLength_1      DMAC_CTLL_SRC_MSIZE_1
#define DMAC_SourceTransactionLength_4      DMAC_CTLL_SRC_MSIZE_4
#define DMAC_SourceTransactionLength_8      DMAC_CTLL_SRC_MSIZE_8
/**
  * @}
  */


/** @defgroup DMAC_DestinationTransactionLength 
  * @{
  */
#define DMAC_DestinationTransactionLength_1   DMAC_CTLL_DEST_MSIZE_1
#define DMAC_DestinationTransactionLength_4   DMAC_CTLL_DEST_MSIZE_4
#define DMAC_DestinationTransactionLength_8   DMAC_CTLL_DEST_MSIZE_8
/**
  * @}
  */


/** @defgroup DMAC_TransferTypeAndFlowControl 
  * @{
  */
#define DMAC_TransferTypeAndFlowControl_MemoryToMemory_DMAC                           DMAC_CTLL_TT_FC_M2M_DMAC
#define DMAC_TransferTypeAndFlowControl_MemoryToPeripheral_DMAC                       DMAC_CTLL_TT_FC_M2P_DMAC
#define DMAC_TransferTypeAndFlowControl_PeripheralToMemory_DMAC                       DMAC_CTLL_TT_FC_P2M_DMAC
#define DMAC_TransferTypeAndFlowControl_PeripheralToPeripheral_DMAC                   DMAC_CTLL_TT_FC_P2P_DMAC
/* The following definitions is only used for DMACx channel0. */
#define DMAC_TransferTypeAndFlowControl_PeripheralToMemory_Peripheral                 DMAC_CTLL_TT_FC_P2M_PERIPH
#define DMAC_TransferTypeAndFlowControl_PeripheralToPeripheral_SourcePeripheral       DMAC_CTLL_TT_FC_P2P_SRC_PERIPH
#define DMAC_TransferTypeAndFlowControl_MemoryToPeripheral_Peripheral                 DMAC_CTLL_TT_FC_M2P_PERIPH
#define DMAC_TransferTypeAndFlowControl_PeripheralToPeripheral_DestinationPeripheral  DMAC_CTLL_TT_FC_P2P_DST_PERIPH
/**
  * @}
  */


/** @defgroup DMAC_SourceMasterInterface 
  * @{
  */
#define DMAC_SourceMasterInterface_APB          (0x0U << 25)
#define DMAC_SourceMasterInterface_AHB          (0x1U << 25)
/**
  * @}
  */


/** @defgroup DMAC_DestinationMasterInterface 
  * @{
  */
#define DMAC_DestinationMasterInterface_APB     (0x0U << 23)
#define DMAC_DestinationMasterInterface_AHB     (0x1U << 23)
/**
  * @}
  */


/** @defgroup DMAC_SourceHandshakingInterfaceSelect 
  * @{
  */
#define DMAC_SourceHandshakingInterfaceSelect_Hardware    (0x0U << 11)
#define DMAC_SourceHandshakingInterfaceSelect_Software    (0x1U << 11)
/**
  * @}
  */


/** @defgroup DMAC_DestinationHandshakingInterfaceSelect 
  * @{
  */
#define DMAC_DestinationHandshakingInterfaceSelect_Hardware   (0x0U << 10)
#define DMAC_DestinationHandshakingInterfaceSelect_Software   (0x1U << 10)
/**
  * @}
  */


/** @defgroup DMAC_SourceHandshakingInterfacePolarity 
  * @{
  */
#define DMAC_SourceHandshakingInterfacePolarity_High    (0x0U << 19)
#define DMAC_SourceHandshakingInterfacePolarity_Low     (0x1U << 19)
/**
  * @}
  */


/** @defgroup DMAC_DestinationHandshakingInterfacePolarity 
  * @{
  */
#define DMAC_DestinationHandshakingInterfacePolarity_High   (0x0U << 18)
#define DMAC_DestinationHandshakingInterfacePolarity_Low    (0x1U << 18)
/**
  * @}
  */


/** @defgroup DMAC_AutomaticSourceReload 
  * @{
  */
#define DMAC_AutomaticSourceReload_Enable         (DMAC_CFGL_RELOAD_SRC)
#define DMAC_AutomaticSourceReload_Disable        ((uint32_t)0x00000000)
/**
  * @}
  */


/** @defgroup DMAC_AutomaticDestinationReload 
  * @{
  */
#define DMAC_AutomaticDestinationReload_Enable    (DMAC_CFGL_RELOAD_DST)
#define DMAC_AutomaticDestinationReload_Disable   ((uint32_t)0x00000000)
/**
  * @}
  */


/** @defgroup DMAC_FlowControlMode 
  * @{
  */
#define DMAC_FlowControlMode_0    (0x0 << 0)
#define DMAC_FlowControlMode_1    (0x1 << 0)
/**
  * @}
  */


/** @defgroup DMAC_FIFOMode 
  * @{
  */
#define DMAC_FIFOMode_0           (0x0 << 1)
#define DMAC_FIFOMode_1           (0x1 << 1)
/**
  * @}
  */


/** @defgroup DMAC_HardwareHandshakingInterface 
  * @{
  */
/* The following definitions is only used for DMAC1. */
#define DMAC_HardwareHandshakingInterface_TIM1_CH1__TIM2_UP__TIM3_CH3               0
#define DMAC_HardwareHandshakingInterface_TIM1_CH4__TIM1_TRIG__TIM1_COM__TIM4_CH2   1
#define DMAC_HardwareHandshakingInterface_TIM1_UP__TIM2_CH1__TIM4_CH3               2
#define DMAC_HardwareHandshakingInterface_TIM1_CH3__TIM3_CH1__TIM3_TRIG             3
#define DMAC_HardwareHandshakingInterface_TIM2_CH3__TIM4_CH1                        4
#define DMAC_HardwareHandshakingInterface_TIM2_CH2__TIM2_CH4__TIM4_UP               5
#define DMAC_HardwareHandshakingInterface_TIM3_CH4__TIM3_UP__TIM1_CH2               6
#define DMAC_HardwareHandshakingInterface_QSPI_RX                                   7
#define DMAC_HardwareHandshakingInterface_QSPI_TX                                   8
#define DMAC_HardwareHandshakingInterface_SPIS1_RX                                  9
#define DMAC_HardwareHandshakingInterface_SPIS1_TX                                  10
#define DMAC_HardwareHandshakingInterface_UART1_RX                                  11
#define DMAC_HardwareHandshakingInterface_UART1_TX                                  12
#define DMAC_HardwareHandshakingInterface_ADC_Regular                               13
#define DMAC_HardwareHandshakingInterface_ADC_Injected                              14

/* The following definitions is only used for DMAC2. */
#define DMAC_HardwareHandshakingInterface_SPIM2_RX                                  0
#define DMAC_HardwareHandshakingInterface_SPIM2_TX                                  1
#define DMAC_HardwareHandshakingInterface_SPIS2_RX                                  2
#define DMAC_HardwareHandshakingInterface_SPIS2_TX                                  3
#define DMAC_HardwareHandshakingInterface_UART2_RX                                  4
#define DMAC_HardwareHandshakingInterface_UART2_TX                                  5
#define DMAC_HardwareHandshakingInterface_UART3_RX                                  6
#define DMAC_HardwareHandshakingInterface_UART3_TX                                  7
#define DMAC_HardwareHandshakingInterface_I2C1_RX                                   8
#define DMAC_HardwareHandshakingInterface_I2C1_TX                                   9
#define DMAC_HardwareHandshakingInterface_I2C2_RX                                   10
#define DMAC_HardwareHandshakingInterface_I2C2_TX                                   11
/**
  * @}
  */


/** @defgroup DMAC_channel 
  * @{
  */
#define DMAC_Channel_0    ((uint8_t)0x00)
#define DMAC_Channel_1    ((uint8_t)0x01)
#define DMAC_Channel_2    ((uint8_t)0x02)
/**
  * @}
  */


/** @defgroup DMAC_interrupts_definitions 
  * @{
  */
#define DMAC_IT_TFR       ((uint16_t)0x0000)    /* Transfer complete interrupt */
#define DMAC_IT_BLOCK     ((uint16_t)0x0008)    /* Block complete interrupt */
#define DMAC_IT_SRCTRAN   ((uint16_t)0x0010)    /* Source transaction complete interrupt */
#define DMAC_IT_DSTTRAN   ((uint16_t)0x0018)    /* Destination transaction complete interrupt */
#define DMAC_IT_ERR       ((uint16_t)0x0020)    /* Error interrupt */
/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void DMAC_DeInit(DMAC_TypeDef* DMACx);
void DMAC_Channel_Init(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, DMAC_Channel_InitTypeDef* DMAC_Channel_InitStruct);
void DMAC_Channel_StructInit(DMAC_Channel_InitTypeDef* DMAC_Channel_InitStruct);
void DMAC_Channel_SourceGatherConfig(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, DMAC_SourceGatherInitTypeDef* DMAC_SourceGatherInitStruct);
void DMAC_Channel_DestinationScatterConfig(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, DMAC_DestinationScatterInitTypeDef* DMAC_DestinationScatterInitStruct);
void DMAC_Channel_SetSourceAddress(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, uint32_t SourceAddress);
void DMAC_Channel_SetDestinationAddress(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, uint32_t DestinationAddress);
void DMAC_Channel_SetBlockTransferSize(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, uint16_t DMAC_BlockTransferSize);
uint16_t DMAC_Channel_GetBlockTransferSize(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel);
void DMAC_Channel_SuspendCmd(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, FunctionalState NewState);
FlagStatus DMAC_Channel_IsFIFOEmpty(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel);

/* Configuration and Channel Enable functions *********************************/
void DMAC_Cmd(DMAC_TypeDef* DMACx, FunctionalState NewState);
FunctionalState DMAC_GetCmdStatus(DMAC_TypeDef* DMACx);
void DMAC_ChannelCmd(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, FunctionalState NewState);
FunctionalState DMAC_GetChannelCmdStatus(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel);

/* Interrupts management functions ********************************************/
void DMAC_ITConfig(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, uint16_t DMAC_IT, FunctionalState NewState);
ITStatus DMAC_GetRawITStatus(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, uint16_t DMAC_IT);
ITStatus DMAC_GetITStatus(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, uint16_t DMAC_IT);
ITStatus DMAC_GetCombinedITStatus(DMAC_TypeDef* DMACx, uint16_t DMAC_IT);
void DMAC_ClearITPendingBit(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, uint16_t DMAC_IT);

/* Software Handshaking interface management functions ************************/
void DMAC_SWHS_SetReqSrcSignalState(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, SignalState NewState);
void DMAC_SWHS_SetReqDstSignalState(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, SignalState NewState);
void DMAC_SWHS_SetSglReqSrcSignalState(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, SignalState NewState);
void DMAC_SWHS_SetSglReqDstSignalState(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, SignalState NewState);
void DMAC_SWHS_SetLstSrcSignalState(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, SignalState NewState);
void DMAC_SWHS_SetLstDstSignalState(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel, SignalState NewState);
SignalState DMAC_SWHS_GetReqSrcSignalState(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel);
SignalState DMAC_SWHS_GetReqDstSignalState(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel);
SignalState DMAC_SWHS_GetSglReqSrcSignalState(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel);
SignalState DMAC_SWHS_GetSglReqDstSignalState(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel);
SignalState DMAC_SWHS_GetLstSrcSignalState(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel);
SignalState DMAC_SWHS_GetLstDstSignalState(DMAC_TypeDef* DMACx, uint8_t DMAC_Channel);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __WB32F10x_DMAC_H */

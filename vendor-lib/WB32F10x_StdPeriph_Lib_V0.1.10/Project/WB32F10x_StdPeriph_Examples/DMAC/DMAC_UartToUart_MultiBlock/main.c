/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"
#include <stdio.h>
#include "bsp_uart1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
UART_InitTypeDef UART_InitStructure;
DMAC_Channel_InitTypeDef DMAC_Channel_InitStruct;
NVIC_InitTypeDef NVIC_InitStructure;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

int main(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* Init UART1 to 115200-8n1 for print message */
  uart1_init(96000000, 115200);

  printf("Enter any key to continue...\r\n");
  getchar();
  printf("GoGo!!!\r\n");

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_BMX2 |RCC_APB2Periph_UART2, ENABLE);

  /*
    PA2 (UART2_TX)
    PA3 (UART2_RX)
  */
  GPIO_Init(GPIOA, GPIO_Pin_2 |GPIO_Pin_3, GPIO_MODE_AF |GPIO_OTYPE_PP |GPIO_PUPD_UP |GPIO_SPEED_HIGH |GPIO_AF7);

  UART_DeInit(UART2);
  UART_InitStructure.UART_BaudRate = 115200;
  UART_InitStructure.UART_WordLength = UART_WordLength_8b;
  UART_InitStructure.UART_StopBits = UART_StopBits_One;
  UART_InitStructure.UART_Parity = UART_Parity_None;
  UART_InitStructure.UART_AutoFlowControl = UART_AutoFlowControl_None;
  UART_Init(UART2, &UART_InitStructure);
  UART_TxFIFOThresholdConfig(UART2, UART_TxFIFOThreshold_8);  /* Must adhere to this equation: DMAC.CTLLx.DEST_MSIZE <= UART.FIFO_DEPTH - decoded level of UART.FCR[5:4] */
  UART_RxFIFOThresholdConfig(UART2, UART_RxFIFOThreshold_8);  /* Must adhere to this equation: DMAC.CTLLx.SRC_MSIZE = decoded level of FCR[7:6] */
  UART_FIFOCmd(UART2, ENABLE);
  UART_ProgrammableTHREModeCmd(UART2, ENABLE);


  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMAC2Bridge, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_BMX2 | RCC_APB2Periph_DMAC2, ENABLE);

  DMAC_DeInit(DMAC2);

  DMAC_Channel_InitStruct.DMAC_SourceBaseAddr = (uint32_t)&UART2->RBR;
  DMAC_Channel_InitStruct.DMAC_DestinationBaseAddr = (uint32_t)&UART2->THR;
  DMAC_Channel_InitStruct.DMAC_Interrupt = DMAC_Interrupt_Enable;
  DMAC_Channel_InitStruct.DMAC_SourceTransferWidth = DMAC_SourceTransferWidth_8b;
  DMAC_Channel_InitStruct.DMAC_DestinationTransferWidth = DMAC_DestinationTransferWidth_8b;
  DMAC_Channel_InitStruct.DMAC_SourceAddrInc = DMAC_SourceAddrInc_NoChange;
  DMAC_Channel_InitStruct.DMAC_DestinationAddrInc = DMAC_DestinationAddrInc_NoChange;
  DMAC_Channel_InitStruct.DMAC_SourceTransactionLength = DMAC_SourceTransactionLength_8;
  DMAC_Channel_InitStruct.DMAC_DestinationTransactionLength = DMAC_DestinationTransactionLength_8;
  DMAC_Channel_InitStruct.DMAC_TransferTypeAndFlowControl = DMAC_TransferTypeAndFlowControl_PeripheralToPeripheral_DMAC;
  DMAC_Channel_InitStruct.DMAC_SourceMasterInterface = DMAC_SourceMasterInterface_APB;
  DMAC_Channel_InitStruct.DMAC_DestinationMasterInterface = DMAC_DestinationMasterInterface_APB;
  DMAC_Channel_InitStruct.DMAC_BlockTransferSize = 16;
  DMAC_Channel_InitStruct.DMAC_SourceHandshakingInterfaceSelect = DMAC_SourceHandshakingInterfaceSelect_Hardware;
  DMAC_Channel_InitStruct.DMAC_DestinationHandshakingInterfaceSelect = DMAC_DestinationHandshakingInterfaceSelect_Hardware;
  DMAC_Channel_InitStruct.DMAC_SourceHandshakingInterfacePolarity = DMAC_SourceHandshakingInterfacePolarity_High;
  DMAC_Channel_InitStruct.DMAC_DestinationHandshakingInterfacePolarity = DMAC_DestinationHandshakingInterfacePolarity_High;
  DMAC_Channel_InitStruct.DMAC_AutomaticSourceReload = DMAC_AutomaticSourceReload_Enable;
  DMAC_Channel_InitStruct.DMAC_AutomaticDestinationReload = DMAC_AutomaticDestinationReload_Enable;
  DMAC_Channel_InitStruct.DMAC_FlowControlMode = DMAC_FlowControlMode_0;
  DMAC_Channel_InitStruct.DMAC_FIFOMode = DMAC_FIFOMode_0;
  DMAC_Channel_InitStruct.DMAC_ChannelPriority = 0;
  DMAC_Channel_InitStruct.DMAC_ProtectionControl = 0x1;
  DMAC_Channel_InitStruct.DMAC_SourceHardwareHandshakingInterfaceAssign = DMAC_HardwareHandshakingInterface_UART2_RX;
  DMAC_Channel_InitStruct.DMAC_DestinationHardwareHandshakingInterfaceAssign = DMAC_HardwareHandshakingInterface_UART2_TX;
  DMAC_Channel_InitStruct.DMAC_MaximumAMBABurstLength = 0;
  DMAC_Channel_Init(DMAC2, DMAC_Channel_0, &DMAC_Channel_InitStruct);

  DMAC_ITConfig(DMAC2, DMAC_Channel_0, DMAC_IT_BLOCK, ENABLE);
  DMAC_ITConfig(DMAC2, DMAC_Channel_0, DMAC_IT_TFR, ENABLE);
  DMAC_ITConfig(DMAC2, DMAC_Channel_0, DMAC_IT_ERR, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = DMAC2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  printf("DMA transfer enable.\r\n");
  DMAC_Cmd(DMAC2, ENABLE);
  DMAC_ChannelCmd(DMAC2, DMAC_Channel_0, ENABLE);

  /* Infinite loop */
  while (1)
  {
  }
}

void DMAC2_IRQHandler(void)
{
  if(DMAC_GetITStatus(DMAC2, DMAC_Channel_0, DMAC_IT_BLOCK) != RESET)
  {
    DMAC_ClearITPendingBit(DMAC2, DMAC_Channel_0, DMAC_IT_BLOCK);
    printf("DMA block transfer complete.\r\n");
  }

  if(DMAC_GetITStatus(DMAC2, DMAC_Channel_0, DMAC_IT_TFR) != RESET)
  {
    DMAC_ClearITPendingBit(DMAC2, DMAC_Channel_0, DMAC_IT_TFR);
    printf("DMA transfer complete.\r\n");
  }

  if(DMAC_GetITStatus(DMAC2, DMAC_Channel_0, DMAC_IT_ERR) != RESET)
  {
    DMAC_ClearITPendingBit(DMAC2, DMAC_Channel_0, DMAC_IT_ERR);
    printf("DMA transfer error!!!\r\n");
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"
#include <stdio.h>
#include "bsp_uart1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const uint8_t master_tx_data[20] = {
  0x97,0x8D,0x38,0xDA,0xDB,0x54,0xBF,0x6D,0x1C,0xFF,
  0x54,0xEF,0x82,0x5C,0x40,0xAC,0x68,0xA0,0x91,0xEC
};
uint8_t master_rx_buf[20] = {0};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

int main(void)
{
  uint32_t iter;
  SPI_InitTypeDef SPI_InitStructure;
  DMAC_Channel_InitTypeDef DMAC_Channel_InitStruct;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* Init UART1 to 115200-8n1 for print message */
  uart1_init(96000000, 115200);
  printf("[SPIM2_Master_DMA]\r\n");

  /* Enable the GPIOC and SPIM2 clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_BMX2 |RCC_APB2Periph_SPIM2, ENABLE);

  /*
    PC0 (SPIM2_NSS0)
    PC1 (SPIM2_SCK)
    PC2 (SPIM2_MI)
    PC3 (SPIM2_MO)
  */
  GPIO_Init(GPIOC, GPIO_Pin_0 |GPIO_Pin_1 |GPIO_Pin_2 |GPIO_Pin_3, GPIO_MODE_AF |GPIO_OTYPE_PP |GPIO_PUPD_NOPULL |GPIO_SPEED_HIGH |GPIO_AF5);

  /* SPI configuration */
  SPI_DeInit(SPIM2);
  SPI_InitStructure.SPI_TransferMode = SPI_TransferMode_TxAndRx;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_BaudRatePrescaler = 8;
  SPI_InitStructure.SPI_FrameFormat = SPI_FrameFormat_SPI;
  SPI_Init(SPIM2, &SPI_InitStructure);
  SPI_ITConfig(SPIM2, 0xFF, DISABLE);
  SPI_DMATxReqLevelConfig(SPIM2, 2);  /* Must adhere to this equation: DMAC.CTLLx.DEST_MSIZE <= SPI.FIFO_DEPTH - SPI.DMATDLR */
  SPI_DMARxReqLevelConfig(SPIM2, 0);  /* Must adhere to this equation: DMAC.CTLLx.SRC_MSIZE = SPI.DMARDLR + 1 */
  SPI_NSSConfig(SPIM2, SPI_NSS_0, ENABLE);

  /* Enable the DMAC2 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMAC2Bridge, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_BMX2 |RCC_APB2Periph_DMAC2, ENABLE);

  /* DMAC2 configuration */
  DMAC_DeInit(DMAC2);
  DMAC_Channel_InitStruct.DMAC_SourceBaseAddr = (uint32_t)master_tx_data;
  DMAC_Channel_InitStruct.DMAC_DestinationBaseAddr = (uint32_t)&SPIM2->DR;
  DMAC_Channel_InitStruct.DMAC_Interrupt = DMAC_Interrupt_Disable;
  DMAC_Channel_InitStruct.DMAC_SourceTransferWidth = DMAC_SourceTransferWidth_8b;
  DMAC_Channel_InitStruct.DMAC_DestinationTransferWidth = DMAC_DestinationTransferWidth_8b;
  DMAC_Channel_InitStruct.DMAC_SourceAddrInc = DMAC_SourceAddrInc_Increment;
  DMAC_Channel_InitStruct.DMAC_DestinationAddrInc = DMAC_DestinationAddrInc_NoChange;
  DMAC_Channel_InitStruct.DMAC_SourceTransactionLength = DMAC_SourceTransactionLength_1;
  DMAC_Channel_InitStruct.DMAC_DestinationTransactionLength = DMAC_DestinationTransactionLength_1;
  DMAC_Channel_InitStruct.DMAC_TransferTypeAndFlowControl = DMAC_TransferTypeAndFlowControl_MemoryToPeripheral_DMAC;
  DMAC_Channel_InitStruct.DMAC_SourceMasterInterface = DMAC_SourceMasterInterface_AHB;
  DMAC_Channel_InitStruct.DMAC_DestinationMasterInterface = DMAC_DestinationMasterInterface_APB;
  DMAC_Channel_InitStruct.DMAC_BlockTransferSize = 20;
  DMAC_Channel_InitStruct.DMAC_SourceHandshakingInterfaceSelect = DMAC_SourceHandshakingInterfaceSelect_Hardware;
  DMAC_Channel_InitStruct.DMAC_DestinationHandshakingInterfaceSelect = DMAC_DestinationHandshakingInterfaceSelect_Hardware;
  DMAC_Channel_InitStruct.DMAC_SourceHandshakingInterfacePolarity = DMAC_SourceHandshakingInterfacePolarity_High;
  DMAC_Channel_InitStruct.DMAC_DestinationHandshakingInterfacePolarity = DMAC_DestinationHandshakingInterfacePolarity_High;
  DMAC_Channel_InitStruct.DMAC_AutomaticSourceReload = DMAC_AutomaticSourceReload_Disable;
  DMAC_Channel_InitStruct.DMAC_AutomaticDestinationReload = DMAC_AutomaticDestinationReload_Disable;
  DMAC_Channel_InitStruct.DMAC_FlowControlMode = DMAC_FlowControlMode_0;
  DMAC_Channel_InitStruct.DMAC_FIFOMode = DMAC_FIFOMode_0;
  DMAC_Channel_InitStruct.DMAC_ChannelPriority = 0;
  DMAC_Channel_InitStruct.DMAC_ProtectionControl = 0x1;
  DMAC_Channel_InitStruct.DMAC_SourceHardwareHandshakingInterfaceAssign = 0;
  DMAC_Channel_InitStruct.DMAC_DestinationHardwareHandshakingInterfaceAssign = DMAC_HardwareHandshakingInterface_SPIM2_TX;
  DMAC_Channel_InitStruct.DMAC_MaximumAMBABurstLength = 0;
  DMAC_Channel_Init(DMAC2, DMAC_Channel_0, &DMAC_Channel_InitStruct);

  DMAC_Channel_InitStruct.DMAC_SourceBaseAddr = (uint32_t)&SPIM2->DR;
  DMAC_Channel_InitStruct.DMAC_DestinationBaseAddr = (uint32_t)master_rx_buf;
  DMAC_Channel_InitStruct.DMAC_Interrupt = DMAC_Interrupt_Disable;
  DMAC_Channel_InitStruct.DMAC_SourceTransferWidth = DMAC_SourceTransferWidth_8b;
  DMAC_Channel_InitStruct.DMAC_DestinationTransferWidth = DMAC_DestinationTransferWidth_8b;
  DMAC_Channel_InitStruct.DMAC_SourceAddrInc = DMAC_SourceAddrInc_NoChange;
  DMAC_Channel_InitStruct.DMAC_DestinationAddrInc = DMAC_DestinationAddrInc_Increment;
  DMAC_Channel_InitStruct.DMAC_SourceTransactionLength = DMAC_SourceTransactionLength_1;
  DMAC_Channel_InitStruct.DMAC_DestinationTransactionLength = DMAC_DestinationTransactionLength_1;
  DMAC_Channel_InitStruct.DMAC_TransferTypeAndFlowControl = DMAC_TransferTypeAndFlowControl_PeripheralToMemory_DMAC;
  DMAC_Channel_InitStruct.DMAC_SourceMasterInterface = DMAC_SourceMasterInterface_APB;
  DMAC_Channel_InitStruct.DMAC_DestinationMasterInterface = DMAC_DestinationMasterInterface_AHB;
  DMAC_Channel_InitStruct.DMAC_BlockTransferSize = 20;
  DMAC_Channel_InitStruct.DMAC_SourceHandshakingInterfaceSelect = DMAC_SourceHandshakingInterfaceSelect_Hardware;
  DMAC_Channel_InitStruct.DMAC_DestinationHandshakingInterfaceSelect = DMAC_DestinationHandshakingInterfaceSelect_Hardware;
  DMAC_Channel_InitStruct.DMAC_SourceHandshakingInterfacePolarity = DMAC_SourceHandshakingInterfacePolarity_High;
  DMAC_Channel_InitStruct.DMAC_DestinationHandshakingInterfacePolarity = DMAC_DestinationHandshakingInterfacePolarity_High;
  DMAC_Channel_InitStruct.DMAC_AutomaticSourceReload = DMAC_AutomaticSourceReload_Disable;
  DMAC_Channel_InitStruct.DMAC_AutomaticDestinationReload = DMAC_AutomaticDestinationReload_Disable;
  DMAC_Channel_InitStruct.DMAC_FlowControlMode = DMAC_FlowControlMode_0;
  DMAC_Channel_InitStruct.DMAC_FIFOMode = DMAC_FIFOMode_0;
  DMAC_Channel_InitStruct.DMAC_ChannelPriority = 0;
  DMAC_Channel_InitStruct.DMAC_ProtectionControl = 0x1;
  DMAC_Channel_InitStruct.DMAC_SourceHardwareHandshakingInterfaceAssign = DMAC_HardwareHandshakingInterface_SPIM2_RX;
  DMAC_Channel_InitStruct.DMAC_DestinationHardwareHandshakingInterfaceAssign = 0;
  DMAC_Channel_InitStruct.DMAC_MaximumAMBABurstLength = 0;
  DMAC_Channel_Init(DMAC2, DMAC_Channel_1, &DMAC_Channel_InitStruct);

  DMAC_Cmd(DMAC2, ENABLE);
  DMAC_ChannelCmd(DMAC2, DMAC_Channel_0, ENABLE);
  DMAC_ChannelCmd(DMAC2, DMAC_Channel_1, ENABLE);
  SPI_Cmd(SPIM2, ENABLE);

  printf("The master is ready.\r\n");
  printf("Before starting the data transfer, make sure the slave is ready.\r\n");
  printf("Press any key to start the transfer.\r\n");
  getchar();
  printf("\r\n");

  SPI_DMACmd(SPIM2, SPI_DMAReq_Tx | SPI_DMAReq_Rx, ENABLE);

  /* Wait for transfer done */
  while(DMAC_GetChannelCmdStatus(DMAC2, DMAC_Channel_1) != DISABLE);
  while(SPI_GetFlagStatus(SPIM2, SPI_FLAG_BUSY) != RESET);

  printf("The data transfer has been completed with the following results:\r\n");
  printf("INDEX | TX DATA | RX DATA\r\n");
  printf("------|---------|--------\r\n");
  for(iter = 0; iter < 20; iter++)
  {
    printf("  %2d  |  0x%02X   |  0x%02X\r\n", iter, master_tx_data[iter], master_rx_buf[iter]);
  }
  printf("------|---------|--------\r\n");

  /* Infinite loop */
  while (1)
  {
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

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"
#include <stdio.h>
#include <string.h>
#include "bsp_uart1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DMAC_Channel_InitTypeDef DMAC_Channel_InitStruct;
NVIC_InitTypeDef NVIC_InitStructure;
char memSrc[] = "DMAC Memory to Memory Example\r\n";
char memDst[sizeof(memSrc)] = {0};
uint32_t flag;

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

  /* Enable DMAC1 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMAC1Bridge, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 | RCC_APB1Periph_DMAC1, ENABLE);

  /* Reset DMAC1 module */
  DMAC_DeInit(DMAC1);

  /* Configures the DMAC1 Channel 0 */
  DMAC_Channel_InitStruct.DMAC_SourceBaseAddr = (uint32_t)memSrc;
  DMAC_Channel_InitStruct.DMAC_DestinationBaseAddr = (uint32_t)memDst;
  DMAC_Channel_InitStruct.DMAC_Interrupt = DMAC_Interrupt_Enable;
  DMAC_Channel_InitStruct.DMAC_SourceTransferWidth = DMAC_SourceTransferWidth_8b;
  DMAC_Channel_InitStruct.DMAC_DestinationTransferWidth = DMAC_DestinationTransferWidth_8b;
  DMAC_Channel_InitStruct.DMAC_SourceAddrInc = DMAC_SourceAddrInc_Increment;
  DMAC_Channel_InitStruct.DMAC_DestinationAddrInc = DMAC_DestinationAddrInc_Increment;
  DMAC_Channel_InitStruct.DMAC_SourceTransactionLength = DMAC_SourceTransactionLength_1;    /* Invalid for memory */
  DMAC_Channel_InitStruct.DMAC_DestinationTransactionLength = DMAC_DestinationTransactionLength_1;    /* Invalid for memory */
  DMAC_Channel_InitStruct.DMAC_TransferTypeAndFlowControl = DMAC_TransferTypeAndFlowControl_MemoryToMemory_DMAC;
  DMAC_Channel_InitStruct.DMAC_SourceMasterInterface = DMAC_SourceMasterInterface_AHB;
  DMAC_Channel_InitStruct.DMAC_DestinationMasterInterface = DMAC_DestinationMasterInterface_AHB;
  DMAC_Channel_InitStruct.DMAC_BlockTransferSize = sizeof(memSrc);
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
  DMAC_Channel_InitStruct.DMAC_DestinationHardwareHandshakingInterfaceAssign = 0;
  DMAC_Channel_InitStruct.DMAC_MaximumAMBABurstLength = 0;
  DMAC_Channel_Init(DMAC1, DMAC_Channel_0, &DMAC_Channel_InitStruct);

  DMAC_ITConfig(DMAC1, DMAC_Channel_0, DMAC_IT_BLOCK, ENABLE);
  DMAC_ITConfig(DMAC1, DMAC_Channel_0, DMAC_IT_TFR, ENABLE);
  DMAC_ITConfig(DMAC1, DMAC_Channel_0, DMAC_IT_ERR, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = DMAC1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  printf("DMA transfer enable.\r\n");
  DMAC_Cmd(DMAC1, ENABLE);
  DMAC_ChannelCmd(DMAC1, DMAC_Channel_0, ENABLE);

  /* Infinite loop */
  while (1)
  {
  }
}

void DMAC1_IRQHandler(void)
{
  if(DMAC_GetITStatus(DMAC1, DMAC_Channel_0, DMAC_IT_BLOCK) != RESET)
  {
    DMAC_ClearITPendingBit(DMAC1, DMAC_Channel_0, DMAC_IT_BLOCK);
    printf("DMA block transfer complete.\r\n");
  }

  if(DMAC_GetITStatus(DMAC1, DMAC_Channel_0, DMAC_IT_TFR) != RESET)
  {
    DMAC_ClearITPendingBit(DMAC1, DMAC_Channel_0, DMAC_IT_TFR);
    printf("DMA transfer complete.\r\n");
    if(memcmp(memDst, memSrc, sizeof(memSrc)) == 0) {
      printf("DMA transfer success!!!\r\n");
    }
    else {
      printf("DMA transfer failed!!!\r\n");
    }
  }

  if(DMAC_GetITStatus(DMAC1, DMAC_Channel_0, DMAC_IT_ERR) != RESET)
  {
    DMAC_ClearITPendingBit(DMAC1, DMAC_Channel_0, DMAC_IT_ERR);
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

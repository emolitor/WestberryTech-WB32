/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DMAC_Channel_InitTypeDef DMAC_Channel_InitStruct;
UART_InitTypeDef UART_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
__IO uint32_t LowPowerMode = 0;
uint8_t DST_Buffer[10]= {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const uint8_t SRC_Buffer[10] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void DMAC_Configuration(void);
void EXTI9_5_Config(void);
void Delay(__IO uint32_t nCount);

/* Private functions ---------------------------------------------------------*/

int main(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* Configure the system clocks */
  RCC_Configuration();

  /* Initialize Leds and Key Button mounted on the board ---------------------*/
  /* Configure PB13 and PB14 in output push-pull mode */
  GPIO_Init(GPIOB, GPIO_Pin_13 |GPIO_Pin_14, GPIO_MODE_OUT |GPIO_OTYPE_PP |GPIO_PUPD_NOPULL |GPIO_SPEED_HIGH);

  /* Turn off LED1 and LED2 */
  GPIO_SetBits(GPIOB, GPIO_Pin_13 |GPIO_Pin_14);

  /* Configure PB6 in interrupt mode */
  EXTI9_5_Config();

  /* Configures the DMA Channel */
  DMAC_Configuration();

  /* UART1 configuration -----------------------------------------------------*/
  /* Configure PA9(UART1_TX) and PA10(UART1_RX) as alternate function mode */
  GPIO_Init(GPIOA, GPIO_Pin_9 |GPIO_Pin_10, GPIO_MODE_AF |GPIO_OTYPE_PP |GPIO_PUPD_UP |GPIO_SPEED_HIGH |GPIO_AF7);

  UART_DeInit(UART1);
  UART_InitStructure.UART_BaudRate = 115200;
  UART_InitStructure.UART_WordLength = UART_WordLength_8b;
  UART_InitStructure.UART_StopBits = UART_StopBits_One;
  UART_InitStructure.UART_Parity = UART_Parity_None;
  UART_InitStructure.UART_AutoFlowControl = UART_AutoFlowControl_None;
  UART_Init(UART1, &UART_InitStructure);
  UART_RxFIFOThresholdConfig(UART1, UART_RxFIFOThreshold_8);  /* Must adhere to this equation: DMAC.CTLLx.SRC_MSIZE = decoded level of FCR[7:6] */
  UART_FIFOCmd(UART1, ENABLE);

  /* Enable the DMAC1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = DMAC1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Infinite loop */
  while (1)
  {
    if(LowPowerMode == 1)
    {
      /* Turn off LED2 */
      GPIO_SetBits(GPIOB, GPIO_Pin_13);

      /* Request to enter WFI mode */
      __WFI();
      LowPowerMode = 0;
    }
    Delay(0xFFFFF);
    GPIO_ToggleBits(GPIOB, GPIO_Pin_14);
  }
}

/**
  * @brief  Configures the different system clocks
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{ 
  /* Enable DMAC1 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMAC1Bridge, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1  | RCC_APB1Periph_DMAC1 |
                         RCC_APB1Periph_GPIOA | RCC_APB1Periph_GPIOB |
                         RCC_APB1Periph_EXTI  | RCC_APB1Periph_AFIO  |
                         RCC_APB1Periph_UART1, ENABLE);
}

/**
  * @brief  Configures the used DMAC Channel.
  * @param  None
  * @retval None
  */
void DMAC_Configuration(void)
{
  DMAC_Channel_InitTypeDef DMAC_Channel_InitStruct;
  DMAC_DeInit(DMAC1);

  DMAC_Channel_InitStruct.DMAC_SourceBaseAddr = (uint32_t)&UART1->RBR;
  DMAC_Channel_InitStruct.DMAC_DestinationBaseAddr = (uint32_t)DST_Buffer;
  DMAC_Channel_InitStruct.DMAC_Interrupt = DMAC_Interrupt_Enable;
  DMAC_Channel_InitStruct.DMAC_SourceTransferWidth = DMAC_SourceTransferWidth_8b;
  DMAC_Channel_InitStruct.DMAC_DestinationTransferWidth = DMAC_DestinationTransferWidth_8b;
  DMAC_Channel_InitStruct.DMAC_SourceAddrInc = DMAC_SourceAddrInc_NoChange;
  DMAC_Channel_InitStruct.DMAC_DestinationAddrInc = DMAC_DestinationAddrInc_Increment;
  DMAC_Channel_InitStruct.DMAC_SourceTransactionLength = DMAC_SourceTransactionLength_8;
  DMAC_Channel_InitStruct.DMAC_DestinationTransactionLength = DMAC_DestinationTransactionLength_1;    /* Invalid for memory */
  DMAC_Channel_InitStruct.DMAC_TransferTypeAndFlowControl = DMAC_TransferTypeAndFlowControl_PeripheralToMemory_DMAC;
  DMAC_Channel_InitStruct.DMAC_SourceMasterInterface = DMAC_SourceMasterInterface_APB;
  DMAC_Channel_InitStruct.DMAC_DestinationMasterInterface = DMAC_DestinationMasterInterface_AHB;
  DMAC_Channel_InitStruct.DMAC_BlockTransferSize = 10;
  DMAC_Channel_InitStruct.DMAC_SourceHandshakingInterfaceSelect = DMAC_SourceHandshakingInterfaceSelect_Hardware;
  DMAC_Channel_InitStruct.DMAC_DestinationHandshakingInterfaceSelect = DMAC_DestinationHandshakingInterfaceSelect_Hardware;
  DMAC_Channel_InitStruct.DMAC_SourceHandshakingInterfacePolarity = DMAC_SourceHandshakingInterfacePolarity_High;
  DMAC_Channel_InitStruct.DMAC_DestinationHandshakingInterfacePolarity = DMAC_DestinationHandshakingInterfacePolarity_High;
  DMAC_Channel_InitStruct.DMAC_AutomaticSourceReload = DMAC_AutomaticSourceReload_Disable;
  DMAC_Channel_InitStruct.DMAC_AutomaticDestinationReload = DMAC_AutomaticDestinationReload_Enable;
  DMAC_Channel_InitStruct.DMAC_FlowControlMode = DMAC_FlowControlMode_0;
  DMAC_Channel_InitStruct.DMAC_FIFOMode = DMAC_FIFOMode_0;
  DMAC_Channel_InitStruct.DMAC_ChannelPriority = 0;
  DMAC_Channel_InitStruct.DMAC_ProtectionControl = 0x1;
  DMAC_Channel_InitStruct.DMAC_SourceHardwareHandshakingInterfaceAssign = DMAC_HardwareHandshakingInterface_UART1_RX;
  DMAC_Channel_InitStruct.DMAC_DestinationHardwareHandshakingInterfaceAssign = 0;
  DMAC_Channel_InitStruct.DMAC_MaximumAMBABurstLength = 0;
  DMAC_Channel_Init(DMAC1, DMAC_Channel_0, &DMAC_Channel_InitStruct);

  DMAC_ITConfig(DMAC1, DMAC_Channel_0, DMAC_IT_BLOCK, ENABLE);

  DMAC_Cmd(DMAC1, ENABLE);
  DMAC_ChannelCmd(DMAC1, DMAC_Channel_0, ENABLE);
}

/**
  * @brief  Configure PB6 in interrupt mode
  * @param  None
  * @return None
  */
void EXTI9_5_Config(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;

  /* Configure PB6 pin as input pull-up */
  GPIO_Init(GPIOB, GPIO_Pin_6, GPIO_MODE_IN |GPIO_PUPD_UP);

  /* Connect EXTI6 Line to PB6 pin */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);

  /* Configure EXTI6 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line6;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Configure and enable EXTI9_5 interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}


/**
  * @brief  This function handles DMAC1 interrupt request.
  * @param  None
  * @retval None
  */
void DMAC1_IRQHandler(void)
{
  if(DMAC_GetITStatus(DMAC1, DMAC_Channel_0, DMAC_IT_BLOCK) != RESET)
  {
    DMAC_ClearITPendingBit(DMAC1, DMAC_Channel_0, DMAC_IT_BLOCK);

    if(memcmp(SRC_Buffer, DST_Buffer, 10) == 0)
    {
      /* Turn on LED2 */
      GPIO_ResetBits(GPIOB, GPIO_Pin_13);
    }
    else
    {
      /* Turn off LED2 */
      GPIO_SetBits(GPIOB, GPIO_Pin_13);
    }
  }
}

/**
  * @brief  This function handles External lines 9 to 5 interrupt request.
  * @param  None
  * @return None
  */
void EXTI9_5_IRQHandler(void)
{
  if(EXTI_GetFlagStatus(EXTI_Line6) != RESET)
  {
    /* Clear the EXTI line 6 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line6);

    LowPowerMode = 1;
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

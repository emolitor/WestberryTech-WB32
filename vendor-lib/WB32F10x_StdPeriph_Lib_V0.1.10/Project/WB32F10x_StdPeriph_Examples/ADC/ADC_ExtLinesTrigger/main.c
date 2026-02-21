/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DMAC_Channel_InitTypeDef DMAC_Channel_InitStruct;
ADC_InitTypeDef ADC_InitStructure;
__IO uint16_t ADC_RegularConvertedValueTab[64], ADC_InjectedConvertedValueTab[32];
uint32_t Index = 0;

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void EXTI_Configuration(void);

/* Private functions ---------------------------------------------------------*/

int main(void)
{
  /* System clocks configuration ---------------------------------------------*/
  RCC_Configuration();

  /* NVIC configuration ------------------------------------------------------*/
  NVIC_Configuration();

  /* GPIO configuration ------------------------------------------------------*/
  GPIO_Configuration();

  /* EXTI configuration ------------------------------------------------------*/
  EXTI_Configuration();

  /* DMAC channel0 configuration ---------------------------------------------*/
  DMAC_DeInit(DMAC1);
  DMAC_Channel_InitStruct.DMAC_SourceBaseAddr = (uint32_t)&ADC->DR;
  DMAC_Channel_InitStruct.DMAC_DestinationBaseAddr = (uint32_t)ADC_RegularConvertedValueTab;
  DMAC_Channel_InitStruct.DMAC_Interrupt = DMAC_Interrupt_Disable;
  DMAC_Channel_InitStruct.DMAC_SourceTransferWidth = DMAC_SourceTransferWidth_16b;
  DMAC_Channel_InitStruct.DMAC_DestinationTransferWidth = DMAC_DestinationTransferWidth_16b;
  DMAC_Channel_InitStruct.DMAC_SourceAddrInc = DMAC_SourceAddrInc_NoChange;
  DMAC_Channel_InitStruct.DMAC_DestinationAddrInc = DMAC_DestinationAddrInc_Increment;
  DMAC_Channel_InitStruct.DMAC_SourceTransactionLength = DMAC_SourceTransactionLength_1;
  DMAC_Channel_InitStruct.DMAC_DestinationTransactionLength = DMAC_DestinationTransactionLength_1;    /* Invalid for memory */
  DMAC_Channel_InitStruct.DMAC_TransferTypeAndFlowControl = DMAC_TransferTypeAndFlowControl_PeripheralToMemory_DMAC;
  DMAC_Channel_InitStruct.DMAC_SourceMasterInterface = DMAC_SourceMasterInterface_APB;
  DMAC_Channel_InitStruct.DMAC_DestinationMasterInterface = DMAC_DestinationMasterInterface_AHB;
  DMAC_Channel_InitStruct.DMAC_BlockTransferSize = 64;
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
  DMAC_Channel_InitStruct.DMAC_SourceHardwareHandshakingInterfaceAssign = DMAC_HardwareHandshakingInterface_ADC_Regular;
  DMAC_Channel_InitStruct.DMAC_DestinationHardwareHandshakingInterfaceAssign = 0;
  DMAC_Channel_InitStruct.DMAC_MaximumAMBABurstLength = 0;
  DMAC_Channel_Init(DMAC1, DMAC_Channel_0, &DMAC_Channel_InitStruct);
  
  DMAC_Cmd(DMAC1, ENABLE);
  DMAC_ChannelCmd(DMAC1, DMAC_Channel_0, ENABLE);
  
  
  /* ADC configuration -------------------------------------------------------*/
  PWR_UnlockANA();
  ANCTL_SARADCCmd(ENABLE);
  PWR_LockANA();
  
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_Ext_IT11;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 2;
  ADC_Init(&ADC_InitStructure);

  /* ADC regular channels configuration */
  ADC_RegularChannelConfig(ADC_Channel_3, 1, ADC_SampleTime_7Cycles5);
  ADC_RegularChannelConfig(ADC_Channel_14, 2, ADC_SampleTime_7Cycles5);

  /* Regular discontinuous mode channel number configuration */
  ADC_DiscModeChannelCountConfig(1);
  /* Enable regular discontinuous mode */
  ADC_DiscModeCmd(ENABLE);

  /* Enable ADC external trigger conversion */
  ADC_ExternalTrigConvCmd(ENABLE);

  /* Set injected sequencer length */
  ADC_InjectedSequencerLengthConfig(2);
  /* ADC injected channel configuration */
  ADC_InjectedChannelConfig(ADC_Channel_11, 1, ADC_SampleTime_7Cycles5);
  ADC_InjectedChannelConfig(ADC_Channel_12, 2, ADC_SampleTime_7Cycles5);
  /* ADC injected external trigger configuration */
  ADC_ExternalTrigInjectedConvConfig(ADC_ExternalTrigInjecConv_Ext_IT15);
  /* Enable ADC injected external trigger conversion */
  ADC_ExternalTrigInjectedConvCmd(ENABLE);

  /* Enable JEOC interrupt */
  ADC_ITConfig(ADC_IT_JEOC, ENABLE);

  /* Enable ADC DMA */
  ADC_DMACmd(ENABLE);

  /* Enable ADC */
  ADC_Cmd(ENABLE);

  /* Start ADC calibration */
  ADC_StartCalibration();
  /* Check the end of ADC calibration */
  while(ADC_GetCalibrationStatus());

  /* Enable ADC reset calibration register */
  ADC_ResetCalibration();
  /* Check the end of ADC reset calibration register */
  while(ADC_GetResetCalibrationStatus());

  /* Infinite loop */
  while (1)
  {
  }
}

void ADC_IRQHandler(void)
{
  /* Clear ADC JEOC pending interrupt bit */
  ADC_ClearITPendingBit(ADC_IT_JEOC);

  /* Get injected channel11 and channel12 converted value */
  ADC_InjectedConvertedValueTab[Index++] = ADC_GetADValue(ADC->JDR3);
  ADC_InjectedConvertedValueTab[Index++] = ADC_GetADValue(ADC->JDR4);

  if (Index == 32)
  {
    Index = 0;
  }
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMAC1Bridge, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1  | RCC_APB1Periph_DMAC1 |
                         RCC_APB1Periph_ADC   | RCC_APB1Periph_GPIOA |
                         RCC_APB1Periph_GPIOC | RCC_APB1Periph_EXTI  |
                         RCC_APB1Periph_AFIO, ENABLE);
}

/**
  * @brief  Configures the different EXTI lines.
  * @param  None
  * @retval None
  */
void EXTI_Configuration(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;

  /* Connect EXTI11 Line to PA11 pin */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11);

  /* EXTI line11 configuration -----------------------------------------------*/
  EXTI_InitStructure.EXTI_Line = EXTI_Line11;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Connect EXTI15 Line to PA15 pin */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15);
  /* EXTI line15 configuration -----------------------------------------------*/
  EXTI_InitStructure.EXTI_Line = EXTI_Line15;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  /* Configure PC1, PC2 and PC4 (ADC Channel11, Channel12 and Channel14)
     as analog mode ----------------------------------------------------------*/
  GPIO_Init(GPIOC, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_4, GPIO_MODE_ANA);

  /* Configure PA3 (ADC Channel3) as analog mode -----------------------------*/
  GPIO_Init(GPIOA, GPIO_Pin_3, GPIO_MODE_ANA);

  /* Configure EXTI line11 ---------------------------------------------------*/
  GPIO_Init(GPIOA, GPIO_Pin_11, GPIO_MODE_IN | GPIO_PUPD_UP);

  /* Configure EXTI line15 ---------------------------------------------------*/
  GPIO_Init(GPIOA, GPIO_Pin_15, GPIO_MODE_IN | GPIO_PUPD_UP);
}

/**
  * @brief  Configures NVIC and Vector Table base location.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* Configure and enable ADC interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
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

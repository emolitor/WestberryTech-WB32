/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DMAC_Channel_InitTypeDef  DMAC_Channel_InitStruct;
ADC_InitTypeDef           ADC_InitStructure;
TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
TIM_OCInitTypeDef         TIM_OCInitStructure;
__IO uint16_t ADC_RegularConvertedValueTab[32], ADC_InjectedConvertedValueTab[32];
uint32_t Index = 0;

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);

/* Private functions ---------------------------------------------------------*/

int main(void)
{
  /* System clocks configuration ---------------------------------------------*/
  RCC_Configuration();

  /* NVIC configuration ------------------------------------------------------*/
  NVIC_Configuration();

  /* GPIO configuration ------------------------------------------------------*/
  GPIO_Configuration();

  /* TIM1 configuration ------------------------------------------------------*/
  TIM_DeInit(TIM1);
  /* Time Base configuration */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  TIM_TimeBaseStructure.TIM_Period = 9999;
  TIM_TimeBaseStructure.TIM_Prescaler = 95;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
  /* TIM1 channel1 configuration in PWM mode */
  TIM_OCStructInit(&TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 5000;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);

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
  DMAC_Channel_InitStruct.DMAC_BlockTransferSize = 32;
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
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 2;
  ADC_Init(&ADC_InitStructure);

  /* ADC regular channels configuration */
  ADC_RegularChannelConfig(ADC_Channel_3, 1, ADC_SampleTime_7Cycles5);
  ADC_RegularChannelConfig(ADC_Channel_14, 2, ADC_SampleTime_7Cycles5);

  /* Set injected sequencer length */
  ADC_InjectedSequencerLengthConfig(1);
  /* ADC injected channel Configuration */
  ADC_InjectedChannelConfig(ADC_Channel_11, 1, ADC_SampleTime_7Cycles5);
  /* ADC injected external trigger configuration */
  ADC_ExternalTrigInjectedConvConfig(ADC_ExternalTrigInjecConv_None);

  /* Enable automatic injected conversion start after regular one */
  ADC_AutoInjectedConvCmd(ENABLE);

  /* Enable ADC DMA */
  ADC_DMACmd(ENABLE);

  /* Enable ADC external trigger conversion */
  ADC_ExternalTrigConvCmd(ENABLE);

  /* Enable JEOC interrupt */
  ADC_ITConfig(ADC_IT_JEOC, ENABLE);

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

  /* TIM1 counter enable */
  TIM_Cmd(TIM1, ENABLE);
  /* TIM1 main Output Enable */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);  

  /* Wait till DMAC1 channel 0 transfer complete */
  while(DMAC_GetRawITStatus(DMAC1, DMAC_Channel_0, DMAC_IT_TFR) == RESET);

  /* Clear DMAC1 channel 0 transfer complete flag */
  DMAC_ClearITPendingBit(DMAC1, DMAC_Channel_0, DMAC_IT_TFR);

  /* TIM1 counter disable */
  TIM_Cmd(TIM1, DISABLE);

  /* Infinite loop */
  while (1)
  {
  }
}

void ADC_IRQHandler(void)
{
  /* Get injected channel11 converted value */
  ADC_InjectedConvertedValueTab[Index++] = ADC_GetADValue(ADC->JDR4);

  /* Clear ADC JEOC pending interrupt bit */
  ADC_ClearITPendingBit(ADC_IT_JEOC);
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
                         RCC_APB1Periph_GPIOC | RCC_APB1Periph_TIM1, ENABLE);
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  /* Configure TIM1_CH1 (PA8) as alternate function push-pull */
  GPIO_Init(GPIOA, GPIO_Pin_8, GPIO_MODE_AF |GPIO_OTYPE_PP |GPIO_PUPD_NOPULL |GPIO_SPEED_HIGH |GPIO_AF1);

  /* Configure PC1 and PC4 (ADC Channel11 and Channel14) as analog mode ------*/
  GPIO_Init(GPIOC, GPIO_Pin_1 | GPIO_Pin_4, GPIO_MODE_ANA);

  /* Configure PA3 (ADC Channel3) as analog mode -----------------------------*/
  GPIO_Init(GPIOA, GPIO_Pin_3, GPIO_MODE_ANA);
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

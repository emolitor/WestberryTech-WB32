/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"
#include <stdio.h>
#include "bsp_uart1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DMAC_Channel_InitTypeDef DMAC_Channel_InitStruct;
ADC_InitTypeDef ADC_InitStructure;
__IO uint16_t ADCConvertedValue, ADC_InjectedConvertedValueTab[4];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

int main(void)
{
  uint32_t iter;
  /* Init UART1 to 115200-8n1 for print message */
  uart1_init(96000000, 115200);

  /* Enable DMAC1, ADC, GPIOA and GPIOC clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMAC1Bridge, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 | RCC_APB1Periph_DMAC1 |
                         RCC_APB1Periph_ADC  | RCC_APB1Periph_GPIOA |
                         RCC_APB1Periph_GPIOC, ENABLE);

  /* Configure PA3 (ADC Channel3) as analog mode */
  GPIO_Init(GPIOA, GPIO_Pin_3, GPIO_MODE_ANA);

 /* Configure PC1 and PC4 (ADC Channel11 and Channel14) as analog mode -------*/
  GPIO_Init(GPIOC, GPIO_Pin_1 | GPIO_Pin_4, GPIO_MODE_ANA);

  /* DMAC channel0 configuration ---------------------------------------------*/
  DMAC_DeInit(DMAC1);
  DMAC_Channel_InitStruct.DMAC_SourceBaseAddr = (uint32_t)&ADC->JDMAR;
  DMAC_Channel_InitStruct.DMAC_DestinationBaseAddr = (uint32_t)ADC_InjectedConvertedValueTab;
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
  DMAC_Channel_InitStruct.DMAC_BlockTransferSize = 4;
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
  DMAC_Channel_InitStruct.DMAC_SourceHardwareHandshakingInterfaceAssign = DMAC_HardwareHandshakingInterface_ADC_Injected;
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
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(&ADC_InitStructure);

  /* ADC regular channel3 configuration */
  ADC_RegularChannelConfig(ADC_Channel_3, 1, ADC_SampleTime_7Cycles5);

  /* Enable ADC external trigger conversion */
  ADC_ExternalTrigConvCmd(ENABLE);

  /* Set injected sequencer length */
  ADC_InjectedSequencerLengthConfig(2);
  /* ADC injected channel configuration */
  ADC_InjectedChannelConfig(ADC_Channel_11, 1, ADC_SampleTime_7Cycles5);
  ADC_InjectedChannelConfig(ADC_Channel_14, 2, ADC_SampleTime_7Cycles5);
  /* ADC injected external trigger configuration */
  ADC_ExternalTrigInjectedConvConfig(ADC_ExternalTrigInjecConv_None);
  /* Enable ADC injected external trigger conversion */
  ADC_ExternalTrigInjectedConvCmd(ENABLE);

  /* Enable ADC injected group DMA */
  ADC_InjectedDMACmd(ENABLE);

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


  /* Start ADC Software Conversion */
  ADC_SoftwareStartConvCmd(ENABLE);

  while(ADC_GetFlagStatus(ADC_FLAG_EOC) == RESET);
  ADCConvertedValue = ADC_GetADValue(ADC->DR);
  printf("regular channel3: %4d\r\n", ADCConvertedValue);


  /* Start ADC Injected Group Software Conversion */
  ADC_SoftwareStartInjectedConvCmd(ENABLE);

  /* Start ADC Injected Group Software Conversion */
  ADC_SoftwareStartInjectedConvCmd(ENABLE);

  while(DMAC_GetRawITStatus(DMAC1, DMAC_Channel_0, DMAC_IT_TFR) == RESET);
  DMAC_ClearITPendingBit(DMAC1, DMAC_Channel_0, DMAC_IT_TFR);
  for(iter = 0; iter < 4; iter++)
  {
    ADC_InjectedConvertedValueTab[iter] = ADC_GetADValue(ADC_InjectedConvertedValueTab[iter]);
  }
  printf("ADC_InjectedConvertedValueTab: %4d, %4d, %4d, %4d\r\n",
                                ADC_InjectedConvertedValueTab[0],
                                ADC_InjectedConvertedValueTab[1],
                                ADC_InjectedConvertedValueTab[2],
                                ADC_InjectedConvertedValueTab[3]);

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

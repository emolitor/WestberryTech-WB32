/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
NVIC_InitTypeDef NVIC_InitStructure;
ADC_InitTypeDef ADC_InitStructure;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

int main(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* System clocks configuration ---------------------------------------------*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 | RCC_APB1Periph_GPIOA | RCC_APB1Periph_ADC, ENABLE);

  /* Configure and enable ADC interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Configure PA3 (Channel3) as analog mode ---------------------------------*/
  GPIO_Init(GPIOA, GPIO_Pin_3, GPIO_MODE_ANA);

  /* ADC configuration -------------------------------------------------------*/
  PWR_UnlockANA();
  ANCTL_SARADCCmd(ENABLE);
  PWR_LockANA();

  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(&ADC_InitStructure);

  /* ADC regular channel3 configuration */
  ADC_RegularChannelConfig(ADC_Channel_3, 1, ADC_SampleTime_7Cycles5);

  /* Configure high and low analog watchdog thresholds */
  ADC_AnalogWatchdogThresholdsConfig(0x0B00, 0x0300);
  /* Configure channel3 as the single analog watchdog guarded channel */
  ADC_AnalogWatchdogSingleChannelConfig(ADC_Channel_3);
  /* Enable analog watchdog on one regular channel */
  ADC_AnalogWatchdogCmd(ADC_AnalogWatchdog_SingleRegEnable);

  /* Enable AWD interrupt */
  ADC_ITConfig(ADC_IT_AWD, ENABLE);

  /* Enable ADC external trigger conversion */
  ADC_ExternalTrigConvCmd(ENABLE);

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

  /* Infinite loop */
  while (1)
  {

  }
}

void ADC_IRQHandler(void)
{
  if (ADC_GetITStatus(ADC_IT_AWD) != RESET)
  {
    /* Clear ADC AWD pending interrupt bit */
    ADC_ClearITPendingBit(ADC_IT_AWD);
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

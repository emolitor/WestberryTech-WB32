/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"
#include <stdio.h>
#include "bsp_uart1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ADC_InitTypeDef ADC_InitStructure;

/* Private function prototypes -----------------------------------------------*/
void NVIC_Configuration(void);

/* Private functions ---------------------------------------------------------*/

int main(void)
{
  /* Init UART1 to 115200-8n1 for print message */
  uart1_init(96000000, 115200);

  /* Enable ADC, GPIOA clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_ADC |RCC_APB1Periph_GPIOA, ENABLE);

  /* Configure PA3 (ADC Channel3) as analog mode */
  GPIO_Init(GPIOA, GPIO_Pin_3, GPIO_MODE_ANA);

  /* NVIC configuration ------------------------------------------------------*/
  NVIC_Configuration();

  /* ADC configuration -------------------------------------------------------*/
  PWR_UnlockANA();
  ANCTL_SARADCCmd(ENABLE);
  PWR_LockANA();

  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(&ADC_InitStructure);

  /* ADC regular channel3 configuration */
  ADC_RegularChannelConfig(ADC_Channel_3, 1, ADC_SampleTime_7Cycles5);

  /* Enable EOC interrupt */
  ADC_ITConfig(ADC_IT_EOC, ENABLE);

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
  if(ADC_GetITStatus(ADC_IT_EOC) != RESET)
  {
    ADC_ClearITPendingBit(ADC_IT_EOC);
    printf("\rADC Channel 3: %-5d", ADC_GetADValue(ADC->DR));
  }
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

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"
#include <stdio.h>
#include "bsp_uart1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define CurrentVDD  3300
#define K           4432
#define TEMP1       30
#define TEMP1_VAL   787214

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ADC_InitTypeDef ADC_InitStructure;

/* Private function prototypes -----------------------------------------------*/
void NVIC_Configuration(void);
float GetChipTemperature(uint32_t Value, uint32_t VDD);

/* Private functions ---------------------------------------------------------*/

int main(void)
{
  /* Init UART1 to 115200-8n1 for print message */
  uart1_init(96000000, 115200);

  /* Enable ADC, GPIOA clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_ADC |RCC_APB1Periph_GPIOA, ENABLE);

  /* NVIC configuration ------------------------------------------------------*/
  NVIC_Configuration();

  /* ADC configuration -------------------------------------------------------*/
  PWR_UnlockANA();
  ANCTL_SARADCCmd(ENABLE);
  ANCTL->BGCR2 |= 0x02;
  PWR_LockANA();

  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(&ADC_InitStructure);

  /* ADC regular channel16 configuration */
  ADC_RegularChannelConfig(ADC_Channel_16, 1, ADC_SampleTime_239Cycles5);

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

  for(uint32_t idx = 0; idx < 0xFF; idx++)
  {
    /* Start ADC Software Conversion */
    ADC_SoftwareStartConvCmd(ENABLE);
    /* Wating for EOC*/
    while(ADC_GetFlagStatus(ADC_FLAG_EOC) != SET){}
    ADC_GetADValue(ADC->DR);
  }

  /* Enable EOC interrupt */
  ADC_ITConfig(ADC_IT_EOC, ENABLE);

  /* Start ADC Software Conversion */
  ADC_SoftwareStartConvCmd(ENABLE);

  /* Infinite loop */
  while (1)
  {
  }
}

void ADC_IRQHandler(void)
{
  uint32_t tmp;
  if(ADC_GetITStatus(ADC_IT_EOC) != RESET)
  {
    ADC_ClearITPendingBit(ADC_IT_EOC);
    tmp = ADC_GetADValue(ADC->DR);
    printf("\rADC Channel 16: %-5d", tmp);
    printf("\rChip Temperature: %.2f", GetChipTemperature(tmp, CurrentVDD));
  }
}

/**
 * @brief  Get chip temperature .
 * @param  Value: Channel 16 ADC acquisition value .
 * @param  CurrentVDD: Chip supply voltage.
 * @return Centigrade value 
 */
float GetChipTemperature(uint32_t Value, uint32_t VDD)
{
  return (float)((TEMP1_VAL - ((float)Value * VDD * 1000 / 4096)) / K + TEMP1);
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

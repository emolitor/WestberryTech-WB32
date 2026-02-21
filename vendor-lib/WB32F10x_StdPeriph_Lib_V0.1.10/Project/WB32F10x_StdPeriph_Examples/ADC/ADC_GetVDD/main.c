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
float GetSupplyVoltage(void);

/* Private functions ---------------------------------------------------------*/

int main(void)
{
  /* Init UART1 to 115200-8n1 for print message */
  uart1_init(96000000, 115200);

  /* Enable ADC, GPIOA clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOA, ENABLE);

  printf("\rVCC: %.2f",GetSupplyVoltage());

  /* Infinite loop */
  while (1)
  {
  }
}

/**
 * @brief  Obtain external supply voltage.
 * @param  None.
 * @return Supply voltage 
 */
float GetSupplyVoltage(void)
{
  uint16_t tmp;
  
  /* Enable ADC clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_ADC, ENABLE);

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

  /* ADC regular channel17 configuration */
  ADC_RegularChannelConfig(ADC_Channel_17, 1, ADC_SampleTime_7Cycles5);

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

  /* Waiting for stability */
  for(uint16_t index = 0; index < 0xFFFF; index++)
  {
    ADC_SoftwareStartConvCmd(ENABLE);
    while(ADC_GetFlagStatus(ADC_FLAG_EOC) != SET){}
    ADC_GetADValue(ADC->DR);
  }

  /* Start ADC Software Conversion */
  ADC_SoftwareStartConvCmd(ENABLE);

  /* Waiting for EOC */
  while(ADC_GetFlagStatus(ADC_FLAG_EOC) != SET){}

  tmp = ADC_GetADValue(ADC->DR);
  if(tmp == 0) return 0;

  return ((float)(*((uint16_t *)(0x1FFFF378))) / tmp * 3.3);
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

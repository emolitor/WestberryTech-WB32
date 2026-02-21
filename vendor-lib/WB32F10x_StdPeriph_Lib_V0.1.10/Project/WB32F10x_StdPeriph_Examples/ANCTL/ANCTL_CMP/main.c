/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t CMP_Result = 0;

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOB, ENABLE);
  GPIO_Init(GPIOB, GPIO_Pin_4 | GPIO_Pin_6, GPIO_MODE_ANA);
  GPIO_Init(GPIOB, GPIO_Pin_13 | GPIO_Pin_14, GPIO_MODE_OUT);

  PWR_UnlockANA();
  ANCTL_CMPAConfig(CMPA_PSEL_PB4, CMPA_NSEL_PB6);
  ANCTL_CMPACmd(ENABLE);
  PWR_LockANA();

  /* Infinite loop */
  while (1)
  {
    CMP_Result = ANCTL_CMPAGetOutputLevel();
    if(CMP_Result == 1)
    {
      /* Turn on LED1 */
      GPIO_ResetBits(GPIOB, GPIO_Pin_14);
      /* Turn off LED2 */
      GPIO_SetBits(GPIOB, GPIO_Pin_13);
    }
    else
    {
      /* Turn off LED1 */
      GPIO_SetBits(GPIOB, GPIO_Pin_14);
      /* Turn on LED2 */
      GPIO_ResetBits(GPIOB, GPIO_Pin_13);
    }
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

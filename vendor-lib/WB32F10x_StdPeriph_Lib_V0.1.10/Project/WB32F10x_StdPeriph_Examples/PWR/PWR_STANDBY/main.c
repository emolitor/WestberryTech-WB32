/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void delay(uint32_t n)
{
  while(n--);
}

int main(void)
{
  /* Enable BMX1 and GPIOB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOB, ENABLE);
  
  GPIO_Init(GPIOB, GPIO_Pin_14, GPIO_MODE_OUT |GPIO_OTYPE_PP |GPIO_PUPD_NOPULL |GPIO_SPEED_HIGH);
  GPIO_ResetBits(GPIOB, GPIO_Pin_14);   // Turn on LED1

  delay(80000000);

  /* Enable WKUP pin */
  PWR_WakeUpPinCmd(ENABLE);

  /* Enter STANDBY Mode */
  PWR_EnterSTANDBYMode();

  /* Infinite loop */
  while (1)
  {
    /* LED1 blink */
    GPIO_ToggleBits(GPIOB, GPIO_Pin_14);
    delay(8000000);
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

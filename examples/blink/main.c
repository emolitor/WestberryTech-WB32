/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void delay(volatile uint32_t n)
{
  while(n--);
}

int main(void)
{
  /* Enable GPIOB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOB, ENABLE);

  /* Configure PB13 and PB14 in output push-pull mode */
  GPIO_Init(GPIOB, GPIO_Pin_13 |GPIO_Pin_14, GPIO_MODE_OUT |GPIO_OTYPE_PP |GPIO_PUPD_NOPULL |GPIO_SPEED_HIGH);
  GPIO_SetBits(GPIOB, GPIO_Pin_14);     // LED1 turn off
  GPIO_ResetBits(GPIOB, GPIO_Pin_13);   // LED2 turn on

  /* Infinite loop */
  while (1)
  {
    delay(9600000);
    GPIO_ToggleBits(GPIOB, GPIO_Pin_13 |GPIO_Pin_14);   // Toggle LED1 and LED2
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

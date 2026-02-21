/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

int main(void)
{
  /* Enable GPIOA and GPIOB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOA |RCC_APB1Periph_GPIOB, ENABLE);

  /* Configure PA0 as input pull-down mode */
  GPIO_Init(GPIOA, GPIO_Pin_0, GPIO_MODE_IN |GPIO_PUPD_DOWN);

  /* Configure PA1 as input pull-up mode */
  GPIO_Init(GPIOA, GPIO_Pin_1, GPIO_MODE_IN |GPIO_PUPD_UP);

  /* Configure PB13 and PB14 in output push-pull mode */
  GPIO_Init(GPIOB, GPIO_Pin_13 |GPIO_Pin_14, GPIO_MODE_OUT |GPIO_OTYPE_PP |GPIO_PUPD_NOPULL |GPIO_SPEED_HIGH);
  GPIO_SetBits(GPIOB, GPIO_Pin_13 |GPIO_Pin_14);    // LED1 and LED2 turn off

  /* Infinite loop */
  while (1)
  {
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) != Bit_RESET)
    {
      GPIO_ResetBits(GPIOB, GPIO_Pin_14);   // LED1 turn on
    }
    else
    {
      GPIO_SetBits(GPIOB, GPIO_Pin_14);   // LED1 turn off
    }

    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == Bit_RESET)
    {
      GPIO_ResetBits(GPIOB, GPIO_Pin_13);   // LED2 turn on
    }
    else
    {
      GPIO_SetBits(GPIOB, GPIO_Pin_13);   // LED2 turn off
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

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void Delay_ms(int nMS)
{
  SysTick->LOAD = 96000;
  SysTick->VAL  = 0x00;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
  while(nMS--)
    while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
  SysTick->CTRL = 0x00;
}

int main(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOB, ENABLE);

  GPIO_Init(GPIOB, GPIO_Pin_14, GPIO_MODE_OUT |GPIO_OTYPE_PP |GPIO_PUPD_NOPULL |GPIO_SPEED_HIGH);
  
  /* Turn off LED1 */
  GPIO_SetBits(GPIOB, GPIO_Pin_14);
  Delay_ms(3000);
  /* Turn on LED1 */
  GPIO_ResetBits(GPIOB, GPIO_Pin_14);
  Delay_ms(200);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_BMX2 |RCC_APB2Periph_WWDG, ENABLE);
  
  WWDG_DeInit();
  
  /* WWDG clock counter = (PCLK(96MHz)/4096)/8 = 2929.6875 Hz (~0.341 ms)  */
  WWDG_SetPrescaler(WWDG_Prescaler_8);
  
  /*
    Enable WWDG and set counter value to 127, WWDG timeout = ~0.341 ms * 64 = 21.8 ms
    In this case the refresh window is: ~0.341 ms * (127-80) = 16.027 ms < refresh window < ~0.341 ms * 64 = 21.8ms
  */
  WWDG_SetWindowValue(80);
  WWDG_Enable(127);

  /* Infinite loop */
  while (1)
  {
    Delay_ms(20);
    WWDG_SetCounter(127);
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

#include "wb32f10x.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nCount);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOB, ENABLE);
  GPIO_Init(GPIOB, GPIO_Pin_13 |GPIO_Pin_14, GPIO_MODE_OUT |GPIO_OTYPE_PP |GPIO_PUPD_NOPULL |GPIO_SPEED_HIGH);

  PWR_UnlockANA();

  ANCTL_LSICmd(ENABLE);
  while(ANCTL_GetFlagStatus(ANCTL_FLAG_LSIRDY) == RESET);

  RCC_DCSSCLKCmd(ENABLE);

  ANCTL_ClockSecuritySystemCmd(ENABLE);

  /* Infinite loop */
  while (1)
  {
    /* LED1 blink */
    GPIO_ToggleBits(GPIOB, GPIO_Pin_14);
    Delay(1000000);
  }
}

/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}


void NMI_Handler(void)
{
  if (ANCTL_GetITStatus(ANCTL_IT_DCSS) != RESET)
  {
    ANCTL_ClearITPendingBit(ANCTL_IT_DCSS);

    while (1)
    {
      /* LED2 blink */
      GPIO_ToggleBits(GPIOB, GPIO_Pin_13);
      Delay(100000);
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

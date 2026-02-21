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

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 | RCC_APB1Periph_GPIOB, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_BKP, ENABLE);

  GPIO_Init(GPIOB, GPIO_Pin_13 |GPIO_Pin_14, GPIO_MODE_OUT);
  GPIO_SetBits(GPIOB, GPIO_Pin_13 |GPIO_Pin_14);

  PWR_BackupAccessCmd(ENABLE);

  if (BKP->DR1 != 0x55AA)
  {
    BKP->DR1 = 0x55AA;

    while(1) {
      delay(800000);
      GPIO_ToggleBits(GPIOB, GPIO_Pin_14);   // LED1 Blink
    }
  }
  else
  {
    while(1) {
      delay(800000);
      GPIO_ToggleBits(GPIOB, GPIO_Pin_13);   // LED2 Blink
    }
  }

  /* Infinite loop */
  while (1)
  {
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

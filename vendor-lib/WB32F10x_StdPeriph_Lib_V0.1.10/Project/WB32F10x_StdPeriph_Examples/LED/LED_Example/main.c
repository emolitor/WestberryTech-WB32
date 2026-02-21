/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const uint8_t table[16] = {
  0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,    /*0-9*/
  0x77,0x7c,0x39,0x5e,0x79,0x71   /*A-F*/
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void delay(volatile uint32_t n)
{
  while(n--);
}

int main(void)
{
  uint32_t iter;

  /* Enable GPIOC and GPIOD clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 | RCC_APB1Periph_GPIOA | RCC_APB1Periph_GPIOC | RCC_APB1Periph_GPIOD, ENABLE);

  /*
    PA8  (LEDO0)
    PC7  (LEDO1)
    PC8  (LEDO2)
    PC9  (LEDO3)
    PC10 (LEDO4)
    PC11 (LEDO5)
    PC12 (LEDO6)
    PD2  (LEDO7)
  */
  GPIO_Init(GPIOA, GPIO_Pin_8, GPIO_MODE_AF | GPIO_AF4);
  GPIO_Init(GPIOC, GPIO_Pin_7 |GPIO_Pin_8 |GPIO_Pin_9 |GPIO_Pin_10 |GPIO_Pin_11 |GPIO_Pin_12, GPIO_MODE_AF | GPIO_AF4);
  GPIO_Init(GPIOD, GPIO_Pin_2, GPIO_MODE_AF | GPIO_AF4);

  /* Configure the drive current of LED pins */
  GPIO_DriveCurrentConfig(GPIOA, GPIO_Pin_8, 0x03);
  GPIO_DriveCurrentConfig(GPIOC, GPIO_Pin_7 |GPIO_Pin_8 |GPIO_Pin_9 |GPIO_Pin_10 |GPIO_Pin_11 |GPIO_Pin_12, 0x03);
  GPIO_DriveCurrentConfig(GPIOD, GPIO_Pin_2, 0x03);

  /* Enable LED clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_BMX2 | RCC_APB2Periph_LED, ENABLE);

  /* Reset LED module */
  LED_DeInit();

  /* LED configuration */
  LED->CYC = 200;
  LED->ECO = 180;
  LED->CON = (0x00 << 4);
  LED->CON |= 0x01;

  /* Infinite loop */
  while (1)
  {
    for(iter = 0; iter < 16; iter++)
    {
      LED_SetSegmentCode(0, table[iter]);
      LED_SetSegmentCode(1, table[iter]);
      LED_SetSegmentCode(2, table[iter]);
      LED_SetSegmentCode(3, table[iter]);
//      LED_SetSegmentCode(4, table[iter]);
//      LED_SetSegmentCode(5, table[iter]);
//      LED_SetSegmentCode(6, table[iter]);
//      LED_SetSegmentCode(7, table[iter]);
      delay(20000000);
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

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"
#include <stdio.h>
#include "bsp_uart1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t result_buf[2];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

int main(void)
{
  /* Init UART1 to 115200-8n1 for print message */
  uart1_init(96000000, 115200);

  /* Enable the CRC and SFM clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC_SFM, ENABLE);

  /* Reset CRC and SFM module */
  CRC_SFM_DeInit();

  printf("0xAAAAAAAA Expand 2 rate, The result is:\r\n");
  SFM_ExpandBits(0xAAAAAAAA, result_buf, 2);
  printf("0x%08X 0x%08X\r\n", result_buf[0], result_buf[1]);

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

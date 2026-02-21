/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"
#include <stdio.h>
#include "bsp_uart1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
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

  printf("The number of bit 1 in 0xAAAAAAAA is %d\r\n", SFM_ComputeBit1Number(0xAAAAAAAA));
  printf("The number of bit 1 in 0x55555555 is %d\r\n", SFM_ComputeBit1Number(0x55555555));
  printf("The number of bit 1 in 0xFFFFFFFF is %d\r\n", SFM_ComputeBit1Number(0xFFFFFFFF));
  printf("The number of bit 1 in 0x7FFFFFFF is %d\r\n", SFM_ComputeBit1Number(0x7FFFFFFF));
  printf("The number of bit 1 in 0x00000000 is %d\r\n", SFM_ComputeBit1Number(0x00000000));
  printf("The number of bit 1 in 0x1BC4D029 is %d\r\n", SFM_ComputeBit1Number(0x1BC4D029));
  printf("The number of bit 1 in 0xFFFF0000 is %d\r\n", SFM_ComputeBit1Number(0xFFFF0000));
  printf("The number of bit 1 in 0x0000F0FF is %d\r\n", SFM_ComputeBit1Number(0x0000F0FF));
  printf("The number of bit 1 in 0x5503AAFF is %d\r\n", SFM_ComputeBit1Number(0x5503AAFF));

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

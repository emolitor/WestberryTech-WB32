/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"
#include "compute.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t result1 = 0;
uint32_t result2 = 0;
uint32_t result3 = 0;
uint32_t result4 = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

int main(void)
{
  /* Enable the CRC and SFM clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC_SFM, ENABLE);

  /* Reset CRC and SFM module */
  CRC_SFM_DeInit();

  computeBytes();
  computeHalfWords();
  computeWords();

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

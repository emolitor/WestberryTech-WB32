/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TEST_PAGE_ADDR   0x08004000

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

int main(void)
{
  uint32_t iter;

  /* Before flash operation, FHSI must be enabled */
  PWR_UnlockANA();
  ANCTL_FHSICmd(ENABLE);
  PWR_LockANA();

  /* Erase the specified FLASH page */
  FMC_ErasePage(TEST_PAGE_ADDR);

  /* Clear page latch */
  FMC_ClearPageLatch();

  /* Write data to page latch */
  for(iter = 0; iter < 64; iter++) {
    FMC->BUF[iter] = 0x12345678 + iter;
  }

  /* Program data in page latch to the specified FLASH page */
  FMC_ProgramPage(TEST_PAGE_ADDR);

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

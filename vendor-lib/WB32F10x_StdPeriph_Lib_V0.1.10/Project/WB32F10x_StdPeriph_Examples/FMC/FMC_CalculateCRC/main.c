/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TEST_START_ADDR   0x08004000

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t hw_crc, sw_crc;

/* Private function prototypes -----------------------------------------------*/
uint16_t crc16_flash_update(uint16_t crc/* = 0x0000*/, const uint8_t* pdata, uint32_t len);

/* Private functions ---------------------------------------------------------*/

int main(void)
{
  uint32_t page, iter, data;

  /* Before flash operation, FHSI must be enabled */
  PWR_UnlockANA();
  ANCTL_FHSICmd(ENABLE);
  PWR_LockANA();

  /* Write data to 3 pages */
  data = 0x12345678;
  for(page = 0; page < 3; page++)
  {
    /* Erase the specified FLASH page */
    FMC_ErasePage(TEST_START_ADDR + page * 0x100);

    /* Clear page latch */
    FMC_ClearPageLatch();

    /* Write data to page latch */
    for(iter = 0; iter < 64; iter++) {
      FMC->BUF[iter] = data;
      data++;
    }

    /* Program data in page latch to the specified FLASH page */
    FMC_ProgramPage(TEST_START_ADDR + page * 0x100);
  }

  /* FMC Calculate 3 pages CRC sum */
  FMC->ADDR = TEST_START_ADDR;
  FMC->CRCON = ((3-1) << FMC_CRCON_CRCLEN_Pos) | (2 << FMC_CRCON_PERIOD_Pos);
  FMC->CRCON |= FMC_CRCON_CRCEN;
  while((FMC->CRCON & FMC_CRCON_CRCF) == 0x00);
  hw_crc = FMC->DATA1;

  /* Software calculation */
  sw_crc = crc16_flash_update(0x0000, (uint8_t*)TEST_START_ADDR, 3 * 0x100);

  /* Infinite loop */
  while (1)
  {
  }
}


uint16_t crc16_flash_update(uint16_t crc/* = 0x0000*/, const uint8_t* pdata, uint32_t len)
{
  uint32_t idx, i;
  
  for(idx = 0; idx < len; idx++)
  {
    crc ^= pdata[idx] << 8;
    for(i = 0; i < 8; i++)
    {
      if((crc & 0x8000) != 0)
        crc = (uint16_t)((crc << 1) ^ 0x8005);
      else
        crc <<= 1;
    }
  }
  
  return crc;
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

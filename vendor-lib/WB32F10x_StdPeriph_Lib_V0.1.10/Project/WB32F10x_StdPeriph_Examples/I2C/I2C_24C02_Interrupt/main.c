/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"
#include <stdio.h>
#include "bsp_uart1.h"
#include "drv_eeprom_24c02.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t errCode = 0;
uint32_t result = 0;
uint8_t rdata1 = 0;
uint8_t rdata2 = 0;
uint8_t rd_buffer[256];
uint8_t wr_buffer[256];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

int main(void)
{
  uint32_t iter;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* Init UART1 to 115200-8n1 for print message */
  uart1_init(96000000, 115200);

  /* Initialize the 24C02 driver */
  eeprom_24c02_init();

  result = eeprom_24c02_random_read(0x06, &rdata1);
  printf("Read from [0x06] is 0x%02X\r\n", rdata1);
  printf("eeprom_24c02_random_read() - TX_ABRT_SOURCE = %08X\r\n", result);
  if(result != 0) {
    errCode = 0x11;
    goto finish;
  }

  result = eeprom_24c02_byte_write(0x06, rdata1 + 1);
  printf("Write 0x%02X to [0x06]\r\n", rdata1 + 1);
  printf("eeprom_24c02_byte_write() - TX_ABRT_SOURCE = %08X\r\n", result);
  if(result != 0) {
    errCode = 0x12;
    goto finish;
  }
  while(eeprom_24c02_probe() != 0x00);   // Wait write finish

  result = eeprom_24c02_random_read(0x06, &rdata2);
  printf("Read from [0x06] is 0x%02X\r\n", rdata2);
  printf("eeprom_24c02_random_read() - TX_ABRT_SOURCE = %08X\r\n", result);
  if(result != 0) {
    errCode = 0x13;
    goto finish;
  }
  if(rdata2 != ((uint8_t)(rdata1 + 1))) {
    errCode = 0x21;
    goto finish;
  }

  for(iter = 0; iter < 256; iter++) {
    wr_buffer[iter] = iter + 0x15;
  }

  printf("Write data to 24C02...\r\n");
  for(iter = 0; iter < 256; iter += 8)
  {
    result = eeprom_24c02_page_write(iter, wr_buffer + iter, 8);
    if(result != 0) {
      errCode = 0x15;
      goto finish;
    }
    while(eeprom_24c02_probe() != 0x00);   // Wait write finish
  }

  printf("Read all data from 24C02...\r\n");
  result = eeprom_24c02_sequential_read(0x00, rd_buffer, 256);
  printf("eeprom_24c02_sequential_read() - TX_ABRT_SOURCE = %08X\r\n", result);
  if(result != 0) {
    errCode = 0x16;
    goto finish;
  }

  printf("Compare the read data with the written data...\r\n");
  for(iter = 0; iter < 256; iter++)
  {
    if(rd_buffer[iter] != wr_buffer[iter]) {
      errCode = 0x22;
      goto finish;
    }
  }

finish:
  if(errCode != 0) {
    printf("Some errors have occurred! ERROR CODE:0x%08X\r\n", errCode);
  }
  else {
    printf("EEPROM Operation completed successfully!\r\n");
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

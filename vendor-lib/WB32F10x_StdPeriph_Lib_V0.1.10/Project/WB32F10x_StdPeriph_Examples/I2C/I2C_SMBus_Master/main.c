/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"
#include "bsp_uart1.h"
#include <stdio.h>
#include "drv_max1617a.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t result = 0;
uint8_t rdata = 0;
extern uint32_t flag_smbus_alert;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void delay(uint32_t n)
{
  while(n--);
}

int main(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* Init UART1 to 115200-8n1 for print message */
  uart1_init(96000000, 115200);

  delay(8000000);
  max1617a_init();

  result = max1617a_smbus_read_byte(0xFE, &rdata);
  printf("MFGID = 0x%02X\r\n", rdata);
  printf("max1617a_smbus_read_byte(0xFE) - TX_ABRT_SOURCE = %08X\r\n", result);

  result = max1617a_smbus_read_byte(0xFF, &rdata);
  printf("DEVID = 0x%02X\r\n", rdata);
  printf("max1617a_smbus_read_byte(0xFF) - TX_ABRT_SOURCE = %08X\r\n", result);

  result = max1617a_smbus_write_byte(0x0B, 30);
  printf("max1617a_smbus_write_byte(0x0B) - TX_ABRT_SOURCE = %08X\r\n", result);

  result = max1617a_smbus_read_byte(0x00, &rdata);
  printf("local = %d\r\n", (int8_t)rdata);
  result = max1617a_smbus_read_byte(0x01, &rdata);
  printf("remote = %d\r\n", (int8_t)rdata);


  /* Infinite loop */
  while (1)
  {
    if(flag_smbus_alert)
    {
      flag_smbus_alert = 0;
      printf("SMBUS_ALERT_DET interrupt occured.\r\n");
      while(I2C_GetFlagStatus(I2C1, I2C_FLAG_SMBUS_ALERT_STATUS) != RESET)
      {
        result = max1617a_smbus_alert_response_address(&rdata);
        printf("max1617a_smbus_alert_response_address() - TX_ABRT_SOURCE = %08X\r\n", result);
        printf("alert_response = 0x%02X\r\n", rdata);
      }

      result = max1617a_smbus_read_byte(0x02, &rdata);
      printf("status = 0x%02X\r\n", rdata);
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

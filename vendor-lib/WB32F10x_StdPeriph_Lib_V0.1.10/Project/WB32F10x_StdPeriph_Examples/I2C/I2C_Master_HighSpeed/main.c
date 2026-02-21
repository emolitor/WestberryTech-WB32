/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"
#include "bsp_uart1.h"
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void dac5571_init(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_BMX2 |RCC_APB2Periph_I2C2, ENABLE);

  /*
    PB10 (I2C2_SCL)
    PB11 (I2C2_SDA)
  */
  GPIO_Init(GPIOB, GPIO_Pin_10 |GPIO_Pin_11, GPIO_MODE_AF |GPIO_OTYPE_OD |GPIO_PUPD_UP |GPIO_SPEED_HIGH |GPIO_AF4);

  I2C_DeInit(I2C2);
  I2C_Init(I2C2, I2C_CON_SLAVE_DISABLE | I2C_CON_RESTART_EN | I2C_CON_SPEED_HIGH | I2C_CON_MASTER_MODE);
  I2C_TargetAddressConfig(I2C2, 0x98 >> 1);
  I2C_HighSpeedModeMasterCodeConfig(I2C2, 0x00);

  I2C2->FS_SCL_HCNT = 47;   /* tHIGH = (47 + FS_SPKLEN + 7) / 96MHz = 0.604us */
  I2C2->FS_SCL_LCNT = 124;  /* tLOW = (124 + 1) / 96MHz = 1.302us */
  I2C2->FS_SPKLEN = 4;      /* tSP = 4 / 96MHz = 41.67ns */

  I2C2->HS_SCL_HCNT = 6;    /* tHIGH = (6 + HS_SPKLEN + 7) / 96MHz = 145.83ns */
  I2C2->HS_SCL_LCNT = 15;   /* tLOW = (15 + 1) / 96MHz = 166.67ns */
  I2C2->HS_SPKLEN = 1;      /* tSP = 1 / 96MHz = 10.42ns */
  I2C2->SDA_SETUP = 2;      /* tSU;DAT = 2 / 96MHz = 20.83ns */
  I2C2->SDA_HOLD = 6;       /* tHD;DAT = 6 / 96MHz = 62.5ns */
}

uint32_t dac5571_update(uint8_t ctrl_byte, uint8_t data)
{
  uint32_t abrt_source = 0;

  I2C_Cmd(I2C2, ENABLE);
  I2C_ClearITPendingBit(I2C2, I2C_IT_TX_ABRT);

  I2C_WriteDataCmd(I2C2, ((ctrl_byte & 0x03) << 4) | (data >> 4));
  I2C_WriteDataCmd(I2C2, ((data & 0x0F) << 4) | I2C_DATA_CMD_STOP);

  while((I2C_GetFlagStatus(I2C2, I2C_FLAG_TFE) == RESET) && (I2C_GetRawITStatus(I2C2, I2C_IT_TX_ABRT) == RESET));
  while(I2C_GetFlagStatus(I2C2, I2C_FLAG_MST_ACTIVITY) != RESET);
  if(I2C_GetRawITStatus(I2C2, I2C_IT_TX_ABRT) != RESET) {
    abrt_source = I2C_GetTxAbortSource(I2C2);
    I2C_ClearITPendingBit(I2C2, I2C_IT_TX_ABRT);
  }
  I2C_Cmd(I2C2, DISABLE);

  return abrt_source;
}

void delay(uint32_t n)
{
  while(n--);
}

int main(void)
{
  uint32_t result;
  uint32_t errCode = 0;

  /* Init UART1 to 115200-8n1 for print message */
  uart1_init(96000000, 115200);

  dac5571_init();
  printf("I2C2 high speed example\r\n");

  result = dac5571_update(0x00, 192);
  printf("dac5571_update(0x00, 192) - TX_ABRT_SOURCE = %08X\r\n", result);
  if(result != 0) {
    errCode = 0x11;
    goto finish;
  }

  delay(60000000);
  result = dac5571_update(0x00, 128);
  printf("dac5571_update(0x00, 128) - TX_ABRT_SOURCE = %08X\r\n", result);
  if(result != 0) {
    errCode = 0x12;
    goto finish;
  }

  delay(60000000);
  result = dac5571_update(0x00, 64);
  printf("dac5571_update(0x00, 64) - TX_ABRT_SOURCE = %08X\r\n", result);
  if(result != 0) {
    errCode = 0x13;
    goto finish;
  }

finish:
  if(errCode != 0) {
    printf("Some errors have occurred! ERROR CODE:0x%08X\r\n", errCode);
  }
  else {
    printf("Operation completed successfully!\r\n");
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

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"
#include <stdio.h>
#include "bsp_uart1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
NVIC_InitTypeDef NVIC_InitStructure;
uint8_t ee_addr = 0;
uint8_t ee_mem[256] = {0};
uint32_t abrt_source = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

int main(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_BMX2 |RCC_APB2Periph_I2C1, ENABLE);

  /*
    PB6 (I2C1_SCL)
    PB7 (I2C1_SDA)
  */
  GPIO_Init(GPIOB, GPIO_Pin_6 |GPIO_Pin_7, GPIO_MODE_AF |GPIO_OTYPE_OD |GPIO_PUPD_UP |GPIO_SPEED_HIGH |GPIO_AF4);

  I2C_DeInit(I2C1);
  I2C_Init(I2C1, I2C_CON_STOP_DET_IFADDRESSED | I2C_CON_SPEED_STANDARD);
  I2C_OwnAddressConfig(I2C1, 0xC4 >> 1);
  I2C_RxFIFOThresholdConfig(I2C1, 0);
  I2C_TxFIFOThresholdConfig(I2C1, 0);

  I2C1->FS_SPKLEN = 4;    /* tSP = 4 / 96MHz = 41.67ns */
  I2C1->SDA_SETUP = 24;   /* tSU;DAT = 24 / 96MHz = 250ns */
  I2C1->SDA_HOLD = 29;    /* tHD;DAT = 29 / 96MHz = 302.08ns */

  I2C_ClearITPendingBit(I2C1, 0xFFFF);
  I2C1->INTR_MASK = I2C_INTR_RX_FULL | I2C_INTR_RD_REQ | I2C_INTR_TX_ABRT;
  I2C_Cmd(I2C1, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = I2C1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Infinite loop */
  while (1)
  {
  }
}

// I2C1 Interrupt Routine
void I2C1_IRQHandler(void)
{
  uint32_t data;

  if(I2C_GetITStatus(I2C1, I2C_IT_TX_ABRT) != RESET)
  {
    abrt_source = I2C_GetTxAbortSource(I2C1);
    I2C_ClearITPendingBit(I2C1, I2C_IT_TX_ABRT);
  }

  if(I2C_GetITStatus(I2C1, I2C_IT_RX_FULL) != RESET)
  {
    data = I2C_ReadData(I2C1);
    if(data & I2C_DATA_CMD_FIRST_DATA_BYTE) {
      ee_addr = data;
    }
    else {
      ee_mem[ee_addr] = data;
      ee_addr = (ee_addr & 0xF8) | ((ee_addr + 1) & 0x07);
    }
  }

  if(I2C_GetITStatus(I2C1, I2C_IT_RD_REQ) != RESET)
  {
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TFNF) != RESET)
    {
      I2C_WriteDataCmd(I2C1, ee_mem[ee_addr]);
      ee_addr++;
    }
    I2C_ClearITPendingBit(I2C1, I2C_IT_RD_REQ);
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

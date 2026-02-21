/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"
#include <stdio.h>
#include "bsp_uart1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const uint8_t slave_tx_data[20] = {
  0x64,0x85,0xD6,0xA0,0xFB,0xE4,0x68,0x0D,0xDB,0x90,
  0xC6,0x4B,0x5B,0xB6,0x63,0xE3,0x4B,0x8A,0xAA,0x99
};
uint8_t slave_rx_buf[20] = {0};
uint32_t tx_index = 0;
uint32_t rx_index = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

int main(void)
{
  uint32_t iter;
  SPI_InitTypeDef SPI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* Init UART1 to 115200-8n1 for print message */
  uart1_init(96000000, 115200);
  printf("[SPIS2_Slave_Interrupt]\r\n");

  /* Enable the GPIOC and SPIS2 clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_BMX2 |RCC_APB2Periph_SPIS2, ENABLE);
  RCC_SPIS2CLKCmd(ENABLE);

  /*
    PC0 (SPIS2_NSS)
    PC1 (SPIS2_SCK)
    PC2 (SPIS2_SO)
    PC3 (SPIS2_SI)
  */
  GPIO_Init(GPIOC, GPIO_Pin_0 |GPIO_Pin_1 |GPIO_Pin_2 |GPIO_Pin_3, GPIO_MODE_AF |GPIO_OTYPE_PP |GPIO_PUPD_NOPULL |GPIO_SPEED_HIGH |GPIO_AF6);

  /* SPI configuration */
  SPI_DeInit(SPIS2);
  SPI_InitStructure.SPI_TransferMode = SPI_TransferMode_TxAndRx;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_BaudRatePrescaler = 0;   /* Invalid for SPIS */
  SPI_InitStructure.SPI_FrameFormat = SPI_FrameFormat_SPI;
  SPI_Init(SPIS2, &SPI_InitStructure);
  SPI_ITConfig(SPIS2, 0xFF, DISABLE);
  SPI_TxFIFOThresholdConfig(SPIS2, 2);
  SPI_RxFIFOThresholdConfig(SPIS2, 1);

  NVIC_InitStructure.NVIC_IRQChannel = SPIS2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  printf("Make sure the master is ready, then press any key to make the slave ready.\r\n");
  getchar();

  SPI_ITConfig(SPIS2, SPI_IT_TXE |SPI_IT_RXF, ENABLE);
  SPI_Cmd(SPIS2, ENABLE);

  printf("The slave is ready.\r\n");
  printf("Waiting for the master initiate the transfer...\r\n");
  printf("\r\n");

  /* Wait for transfer done */
  while(rx_index < 20);

  printf("The data transfer has been completed with the following results:\r\n");
  printf("INDEX | TX DATA | RX DATA\r\n");
  printf("------|---------|--------\r\n");
  for(iter = 0; iter < 20; iter++)
  {
    printf("  %2d  |  0x%02X   |  0x%02X\r\n", iter, slave_tx_data[iter], slave_rx_buf[iter]);
  }
  printf("------|---------|--------\r\n");

  /* Infinite loop */
  while (1)
  {
  }
}

/**
 * @brief  This function handles SPIS2 interrupt request.
 * @param  None
 * @return None
 */
void SPIS2_IRQHandler(void)
{
  if(SPI_GetITStatus(SPIS2, SPI_IT_RXF) != RESET)
  {
    while(SPI_GetFlagStatus(SPIS2, SPI_FLAG_RFNE) != RESET)
    {
      slave_rx_buf[rx_index] = SPI_ReadData(SPIS2);
      rx_index++;
      if(rx_index >= 20) {
        SPI_ITConfig(SPIS2, SPI_IT_RXF, DISABLE);
        break;
      }
    }
  }

  if(SPI_GetITStatus(SPIS2, SPI_IT_TXE) != RESET)
  {
    while(SPI_GetFlagStatus(SPIS2, SPI_FLAG_TFNF) != RESET)
    {
      SPI_WriteData(SPIS2, slave_tx_data[tx_index]);
      tx_index++;
      if(tx_index >= 20) {
        SPI_ITConfig(SPIS2, SPI_IT_TXE, DISABLE);
        break;
      }
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

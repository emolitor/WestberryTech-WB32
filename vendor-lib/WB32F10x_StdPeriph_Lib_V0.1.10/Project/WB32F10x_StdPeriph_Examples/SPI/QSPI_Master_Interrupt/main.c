/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"
#include <stdio.h>
#include "bsp_uart1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const uint8_t master_tx_data[20] = {
  0x97,0x8D,0x38,0xDA,0xDB,0x54,0xBF,0x6D,0x1C,0xFF,
  0x54,0xEF,0x82,0x5C,0x40,0xAC,0x68,0xA0,0x91,0xEC
};
uint8_t master_rx_buf[20] = {0};
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
  printf("[QSPI_Master_Interrupt]\r\n");

  /* Enable the GPIOA and QSPI clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOA |RCC_APB1Periph_QSPI, ENABLE);
  
  /*
    PA4 (QSPI_NSS0)
    PA5 (QSPI_SCK)
    PA6 (QSPI_MI_IO1)
    PA7 (QSPI_MO_IO0)
  */
  GPIO_Init(GPIOA, GPIO_Pin_4 |GPIO_Pin_5 |GPIO_Pin_6 |GPIO_Pin_7, GPIO_MODE_AF |GPIO_OTYPE_PP |GPIO_PUPD_NOPULL |GPIO_SPEED_HIGH |GPIO_AF5);

  /* SPI configuration */
  SPI_DeInit(QSPI);
  SPI_InitStructure.SPI_TransferMode = SPI_TransferMode_TxAndRx;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_BaudRatePrescaler = 96;   /* In order to avoid TX FIFO underflow or RX FIFO overflow, SPI_BaudRatePrescaler needs to bigger */
  SPI_InitStructure.SPI_FrameFormat = SPI_FrameFormat_SPI;
  SPI_Init(QSPI, &SPI_InitStructure);
  SPI_ITConfig(QSPI, 0xFF, DISABLE);
  SPI_TxFIFOThresholdConfig(QSPI, 2);
  SPI_RxFIFOThresholdConfig(QSPI, 1);
  SPI_NSSConfig(QSPI, SPI_NSS_0, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = QSPI_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  printf("The master is ready.\r\n");
  printf("Before starting the data transfer, make sure the slave is ready.\r\n");
  printf("Press any key to start the transfer.\r\n");
  getchar();
  printf("\r\n");

  SPI_ITConfig(QSPI, SPI_IT_TXE |SPI_IT_RXF, ENABLE);
  SPI_Cmd(QSPI, ENABLE);

  /* Wait for transfer done */
  while(rx_index < 20);

  printf("The data transfer has been completed with the following results:\r\n");
  printf("INDEX | TX DATA | RX DATA\r\n");
  printf("------|---------|--------\r\n");
  for(iter = 0; iter < 20; iter++)
  {
    printf("  %2d  |  0x%02X   |  0x%02X\r\n", iter, master_tx_data[iter], master_rx_buf[iter]);
  }
  printf("------|---------|--------\r\n");

  /* Infinite loop */
  while (1)
  {
  }
}

/**
 * @brief  This function handles QSPI interrupt request.
 * @param  None
 * @return None
 */
void QSPI_IRQHandler(void)
{
  if(SPI_GetITStatus(QSPI, SPI_IT_RXF) != RESET)
  {
    while(SPI_GetFlagStatus(QSPI, SPI_FLAG_RFNE) != RESET)
    {
      master_rx_buf[rx_index] = SPI_ReadData(QSPI);
      rx_index++;
      if(rx_index >= 20) {
        SPI_ITConfig(QSPI, SPI_IT_RXF, DISABLE);
        break;
      }
    }
  }

  if(SPI_GetITStatus(QSPI, SPI_IT_TXE) != RESET)
  {
    while(SPI_GetFlagStatus(QSPI, SPI_FLAG_TFNF) != RESET)
    {
      SPI_WriteData(QSPI, master_tx_data[tx_index]);
      tx_index++;
      if(tx_index >= 20) {
        SPI_ITConfig(QSPI, SPI_IT_TXE, DISABLE);
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

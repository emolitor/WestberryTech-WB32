/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const uint8_t text[] = "Please enter some character.\r\n";
UART_InitTypeDef UART_InitStructure;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void Uart1Send(const uint8_t* txBuf, uint32_t txLen)
{
  while(txLen--)
  {
    /* Loop until the THR or TX FIFO is empty */
    while(!(UART_GetLineStatus(UART1) & UART_LINE_STATUS_THRE));

    UART_WriteData(UART1, *txBuf++);
  }
  /* Loop until the end of transmission */
  while(!(UART_GetLineStatus(UART1) & UART_LINE_STATUS_TEMT));
}

int main(void)
{
  uint32_t value;

  /* Enable GPIOA and UART1 clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOA |RCC_APB1Periph_UART1, ENABLE);

  /*
    PA9  (UART1_TX)
    PA10 (UART1_RX)
  */
  GPIO_Init(GPIOA, GPIO_Pin_9 |GPIO_Pin_10, GPIO_MODE_AF |GPIO_OTYPE_PP |GPIO_PUPD_UP |GPIO_SPEED_HIGH |GPIO_AF7);

  /* UART1 configuration */
  UART_DeInit(UART1);
  UART_InitStructure.UART_BaudRate = 115200;
  UART_InitStructure.UART_WordLength = UART_WordLength_8b;
  UART_InitStructure.UART_StopBits = UART_StopBits_One;
  UART_InitStructure.UART_Parity = UART_Parity_None;
  UART_InitStructure.UART_AutoFlowControl = UART_AutoFlowControl_None;
  UART_Init(UART1, &UART_InitStructure);

  /* Send a message to the host */
  Uart1Send(text, sizeof(text));

  /* Infinite loop */
  while (1)
  {
    while(!(UART_GetLineStatus(UART1) & UART_LINE_STATUS_DR));
    value = UART_ReadData(UART1);
    UART_WriteData(UART1, value);
    while(!(UART_GetLineStatus(UART1) & UART_LINE_STATUS_THRE));
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

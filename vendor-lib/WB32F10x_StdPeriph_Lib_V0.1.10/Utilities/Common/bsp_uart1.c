#include "bsp_uart1.h"
#include "wb32f10x.h"

/**
  * @name    uart1_init
  * @brief   初始化UART1
  * @param   apbclk: UART1所在的APB总线时钟频率
  * @param   baud: 要配置的波特率
  * @retval  无
  */
void uart1_init(uint32_t apbclk, uint32_t baud)
{
  uint32_t divider;
  
  /* Enable APB1, BMX1, GPIOA, UART1 clock */
  RCC->APB1PRE |= RCC_APB1PRE_SRCEN;
  RCC->APB1ENR |= (RCC_APB1ENR_BMX1EN | RCC_APB1ENR_GPIOAEN | RCC_APB1ENR_UART1EN);
  
  /* Reset UART1 */
  RCC->APB1RSTR |= RCC_APB1RSTR_UART1RST;
  RCC->APB1RSTR &= ~RCC_APB1RSTR_UART1RST;
  
  /* Configure PA9 PA10 */
  GPIO_Init(GPIOA, GPIO_Pin_9 |GPIO_Pin_10, GPIO_MODE_AF |GPIO_OTYPE_PP |GPIO_PUPD_UP |GPIO_SPEED_HIGH |GPIO_AF7);
  
  /* Configure UART1 */
  UART1->MCR = 0x0;
  divider = (apbclk + (baud >> 1)) / baud;    // 整型除法四舍五入
  UART1->DLF = divider & 0x0F;
  UART1->LCR = UART_LCR_DLAB;
  UART1->DLL = (uint8_t)(divider >> 4);
  UART1->DLH = (uint8_t)(divider >> 12);
  UART1->LCR = UART_LCR_WLS_8BIT | UART_LCR_SBS_1BIT | UART_LCR_PARITY_NONE;
  UART1->FCR = UART_FCR_FIFOE;
}

/**
  * @name    uart1_send
  * @brief   通过UART1发送数据
  * @param   buffer: 要发送的数据缓冲区
  * @param   length: 要发送的数据长度
  * @retval  无
  */
void uart1_send(const uint8_t* buffer, uint32_t length)
{
  while(length)
  {
    while(!(UART1->LSR & UART_LSR_THRE));   // 等待THR或TX FIFO为空
    UART1->THR = *buffer;
    buffer++;  length--;
  }
  while(!(UART1->LSR & UART_LSR_TEMT));   // 等待发送器为空
}


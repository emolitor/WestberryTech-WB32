/*
    Copyright 2026 emolitor (github.com/emolitor)
    SPDX-License-Identifier: GPL-2.0-or-later

    Phase 2: UART functional tests using internal hardware loopback.

    UART_LoopBackModeCmd() connects TX to RX internally, so no external
    wiring is needed. We test UART2, UART3 (PB10/PB11), and UART3 alt
    pins (PC10/PC11, LQFP64 only).
*/

#include "wb32f10x.h"
#include "test_framework.h"

extern void delay_ms(uint32_t ms);

/* Configure a UART for loopback testing at 115200 baud */
static void uart_loopback_init(UART_TypeDef *uart, uint32_t apbclk)
{
  uint32_t divider;

  /* Reset and configure */
  uart->MCR = 0x0;
  divider = (apbclk + (115200 >> 1)) / 115200;
  uart->DLF = divider & 0x0F;
  uart->LCR = UART_LCR_DLAB;
  uart->DLL = (uint8_t)(divider >> 4);
  uart->DLH = (uint8_t)(divider >> 12);
  uart->LCR = UART_LCR_WLS_8BIT | UART_LCR_SBS_1BIT | UART_LCR_PARITY_NONE;
  uart->FCR = UART_FCR_FIFOE;

  /* Enable loopback mode */
  UART_LoopBackModeCmd(uart, ENABLE);
}

/* Send a byte and try to receive it back via loopback */
static int uart_loopback_test_byte(UART_TypeDef *uart, uint8_t tx_byte)
{
  uint32_t timeout;

  /* Wait for TX ready */
  timeout = 10000;
  while (!(uart->LSR & UART_LSR_THRE) && --timeout);
  if (timeout == 0) return -1;

  /* Send byte */
  uart->THR = tx_byte;

  /* Wait for RX data */
  timeout = 100000;
  while (!(uart->LSR & 0x01) && --timeout); /* LSR bit 0 = Data Ready */
  if (timeout == 0) return -2;

  /* Read received byte */
  return (int)(uart->RBR & 0xFF);
}

/* Test one UART with loopback */
static void test_one_uart(const char *name, UART_TypeDef *uart,
                           GPIO_TypeDef *tx_port, uint16_t tx_pin,
                           GPIO_TypeDef *rx_port, uint16_t rx_pin,
                           uint32_t af, uint32_t apbclk)
{
  /* Configure TX and RX pins to AF mode */
  GPIO_Init(tx_port, tx_pin, GPIO_MODE_AF | GPIO_OTYPE_PP | GPIO_PUPD_UP | GPIO_SPEED_HIGH | af);
  GPIO_Init(rx_port, rx_pin, GPIO_MODE_AF | GPIO_OTYPE_PP | GPIO_PUPD_UP | GPIO_SPEED_HIGH | af);

  /* Initialize UART in loopback mode */
  uart_loopback_init(uart, apbclk);

  /* Test with multiple byte values */
  uint8_t test_bytes[] = { 0x55, 0xAA, 0x00, 0xFF };
  int all_ok = 1;
  uint32_t expected = 0, actual = 0;
  uint32_t i;

  for (i = 0; i < sizeof(test_bytes); i++) {
    int rx = uart_loopback_test_byte(uart, test_bytes[i]);
    if (rx != (int)test_bytes[i]) {
      all_ok = 0;
      expected = test_bytes[i];
      actual = (rx < 0) ? (uint32_t)rx : (uint32_t)rx;
      break;
    }
  }

  /* Disable loopback */
  UART_LoopBackModeCmd(uart, DISABLE);

  /* Restore pins to input */
  GPIO_Init(tx_port, tx_pin, GPIO_MODE_IN | GPIO_PUPD_NOPULL);
  GPIO_Init(rx_port, rx_pin, GPIO_MODE_IN | GPIO_PUPD_NOPULL);

  if (all_ok) {
    test_pass(name, 0x55AA, 0x55AA);
  } else {
    test_fail(name, expected, actual);
  }
}

void test_uart_loopback(void)
{
  /* Enable UART2 and UART3 clocks (APB2 domain) */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_BMX2 |
                         RCC_APB2Periph_UART2 |
                         RCC_APB2Periph_UART3, ENABLE);

  /* Reset UART2 */
  RCC->APB2RSTR |= (1 << 2);  /* UART2RST */
  RCC->APB2RSTR &= ~(1 << 2);

  /* Reset UART3 */
  RCC->APB2RSTR |= (1 << 3);  /* UART3RST */
  RCC->APB2RSTR &= ~(1 << 3);

  /* UART2: PA2 (TX, AF7), PA3 (RX, AF7) */
  test_one_uart("UART2_PA2_PA3_loopback",
                UART2, GPIOA, GPIO_Pin_2, GPIOA, GPIO_Pin_3,
                GPIO_AF7, SystemCoreClock);

  /* UART3: PB10 (TX, AF7), PB11 (RX, AF7) */
  test_one_uart("UART3_PB10_PB11_loopback",
                UART3, GPIOB, GPIO_Pin_10, GPIOB, GPIO_Pin_11,
                GPIO_AF7, SystemCoreClock);

  /* Reset UART3 before switching to alternate pin group */
  RCC->APB2RSTR |= (1 << 3);
  RCC->APB2RSTR &= ~(1 << 3);

  /* UART3 alt: PC10 (TX, AF7), PC11 (RX, AF7) -- LQFP64 only */
  test_one_uart("UART3_PC10_PC11_loopback",
                UART3, GPIOC, GPIO_Pin_10, GPIOC, GPIO_Pin_11,
                GPIO_AF7, SystemCoreClock);
}

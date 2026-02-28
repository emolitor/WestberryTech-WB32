/*
    Copyright 2026 emolitor (github.com/emolitor)
    SPDX-License-Identifier: GPL-2.0-or-later

    Phase 5: SPI register validation.

    Initialize QSPI and SPIM2 peripherals, verify status registers show
    expected idle state (TX FIFO empty, not busy).
*/

#include "wb32f10x.h"
#include "test_framework.h"

static void test_one_spi(const char *name, SPI_TypeDef *spi,
                          GPIO_TypeDef *sck_port, uint16_t sck_pin,
                          GPIO_TypeDef *miso_port, uint16_t miso_pin,
                          GPIO_TypeDef *mosi_port, uint16_t mosi_pin,
                          uint8_t af)
{
  SPI_InitTypeDef spi_init;

  /* Configure pins */
  GPIO_Init(sck_port, sck_pin,
            GPIO_MODE_AF | GPIO_OTYPE_PP | GPIO_PUPD_NOPULL | GPIO_SPEED_HIGH | af);
  GPIO_Init(miso_port, miso_pin,
            GPIO_MODE_AF | GPIO_PUPD_NOPULL | GPIO_SPEED_HIGH | af);
  GPIO_Init(mosi_port, mosi_pin,
            GPIO_MODE_AF | GPIO_OTYPE_PP | GPIO_PUPD_NOPULL | GPIO_SPEED_HIGH | af);

  /* Initialize SPI in master mode */
  spi_init.SPI_TransferMode = SPI_TransferMode_TxAndRx;
  spi_init.SPI_DataSize = SPI_DataSize_8b;
  spi_init.SPI_CPOL = SPI_CPOL_Low;
  spi_init.SPI_CPHA = SPI_CPHA_1Edge;
  spi_init.SPI_BaudRatePrescaler = 48;
  spi_init.SPI_FrameFormat = SPI_FrameFormat_SPI;
  SPI_Init(spi, &spi_init);
  SPI_Cmd(spi, ENABLE);

  /* Check status: TX FIFO empty and not busy */
  FlagStatus tfe = SPI_GetFlagStatus(spi, SPI_FLAG_TFE);
  FlagStatus busy = SPI_GetFlagStatus(spi, SPI_FLAG_BUSY);

  SPI_Cmd(spi, DISABLE);

  /* Restore pins */
  GPIO_Init(sck_port, sck_pin, GPIO_MODE_IN | GPIO_PUPD_NOPULL);
  GPIO_Init(miso_port, miso_pin, GPIO_MODE_IN | GPIO_PUPD_NOPULL);
  GPIO_Init(mosi_port, mosi_pin, GPIO_MODE_IN | GPIO_PUPD_NOPULL);

  if (tfe == SET && busy == RESET) {
    test_pass(name, 1, 1);
  } else {
    /* Pack TFE and BUSY into result for debugging */
    test_fail(name, 0x10, ((uint32_t)tfe << 4) | (uint32_t)busy);
  }
}

void test_spi_init(void)
{
  /* Enable SPI clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_QSPI, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPIM2, ENABLE);

  /* QSPI: PA5 (SCK, AF5), PA6 (MISO, AF5), PA7 (MOSI, AF5) */
  test_one_spi("QSPI_PA5_PA6_PA7",
               QSPI,
               GPIOA, GPIO_Pin_5,   /* SCK */
               GPIOA, GPIO_Pin_6,   /* MISO */
               GPIOA, GPIO_Pin_7,   /* MOSI */
               GPIO_AF5);

  /* SPIM2: PB13 (SCK, AF5), PB14 (MISO, AF5), PB15 (MOSI, AF5) */
  test_one_spi("SPIM2_PB13_PB14_PB15",
               SPIM2,
               GPIOB, GPIO_Pin_13,  /* SCK */
               GPIOB, GPIO_Pin_14,  /* MISO */
               GPIOB, GPIO_Pin_15,  /* MOSI */
               GPIO_AF5);
}

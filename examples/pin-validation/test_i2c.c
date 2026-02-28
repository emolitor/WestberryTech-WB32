/*
    Copyright 2026 emolitor (github.com/emolitor)
    SPDX-License-Identifier: GPL-2.0-or-later

    Phase 6: I2C peripheral init tests.

    Initialize I2C1 and I2C2 in master mode, verify TX FIFO empty flag.
    Pull-ups recommended on SDA/SCL but not strictly required for this test.
*/

#include "wb32f10x.h"
#include "test_framework.h"

static void test_one_i2c(const char *name, I2C_TypeDef *i2c,
                          GPIO_TypeDef *scl_port, uint16_t scl_pin, uint8_t scl_af,
                          GPIO_TypeDef *sda_port, uint16_t sda_pin, uint8_t sda_af)
{
  /* Configure pins: open-drain AF with pull-up */
  GPIO_Init(scl_port, scl_pin,
            GPIO_MODE_AF | GPIO_OTYPE_OD | GPIO_PUPD_UP | GPIO_SPEED_HIGH | scl_af);
  GPIO_Init(sda_port, sda_pin,
            GPIO_MODE_AF | GPIO_OTYPE_OD | GPIO_PUPD_UP | GPIO_SPEED_HIGH | sda_af);

  /* Initialize I2C in master mode */
  I2C_Init(i2c, 0x0063); /* Standard mode master, typical CON value */

  /* Check TX FIFO empty flag */
  FlagStatus tfe = I2C_GetFlagStatus(i2c, I2C_FLAG_TFE);

  /* Disable I2C */
  i2c->ENABLE = 0;

  /* Restore pins */
  GPIO_Init(scl_port, scl_pin, GPIO_MODE_IN | GPIO_PUPD_NOPULL);
  GPIO_Init(sda_port, sda_pin, GPIO_MODE_IN | GPIO_PUPD_NOPULL);

  if (tfe == SET) {
    test_pass(name, 1, 1);
  } else {
    test_fail(name, 1, 0);
  }
}

void test_i2c_init(void)
{
  /* Enable I2C clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_BMX2 |
                         RCC_APB2Periph_I2C1 |
                         RCC_APB2Periph_I2C2, ENABLE);

  /* I2C1: PB6 (SCL, AF5), PB7 (SDA, AF3) */
  test_one_i2c("I2C1_PB6_PB7",
               I2C1,
               GPIOB, GPIO_Pin_6, GPIO_AF5,
               GPIOB, GPIO_Pin_7, GPIO_AF3);

  /* I2C2: PB10 (SCL, AF3), PB11 (SDA, AF3) */
  test_one_i2c("I2C2_PB10_PB11",
               I2C2,
               GPIOB, GPIO_Pin_10, GPIO_AF3,
               GPIOB, GPIO_Pin_11, GPIO_AF3);
}

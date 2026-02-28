/*
    Copyright 2026 emolitor (github.com/emolitor)
    SPDX-License-Identifier: GPL-2.0-or-later

    Phase 1: Register-level AF validation for all pins.

    For each pin/AF in the datasheet table:
    1. Save current AF register state
    2. Configure pin to the specified AF
    3. Read back AFRL or AFRH and extract the 4-bit field
    4. Compare against expected AF number
    5. Restore pin to safe input state
*/

#include "wb32f10x.h"
#include "test_framework.h"
#include "pin_defs.h"

static uint32_t read_af_field(GPIO_TypeDef *port, uint8_t pin)
{
  uint32_t reg;
  uint8_t shift;

  if (pin < 8) {
    reg = port->AFRL;
    shift = pin * 4;
  } else {
    reg = port->AFRH;
    shift = (pin - 8) * 4;
  }

  return (reg >> shift) & 0x0F;
}

void test_gpio_af_all(void)
{
  uint32_t i;

  for (i = 0; i < AF_TABLE_SIZE; i++) {
    const pin_af_entry_t *entry = &af_table[i];
    GPIO_TypeDef *port = entry->port;
    uint8_t pin = entry->pin;
    uint8_t af = entry->af;
    uint16_t pin_mask = (uint16_t)(1U << pin);

    /* Skip excluded pins */
    if (PIN_IS_EXCLUDED(port, pin)) {
      test_skip(entry->name, "reserved for console/SWD/HSE");
      continue;
    }

    /* Save current AF register state */
    uint32_t saved_afrl = port->AFRL;
    uint32_t saved_afrh = port->AFRH;
    uint32_t saved_moder = port->MODER;

    /* Configure pin to AF mode with the specified AF number */
    GPIO_Init(port, pin_mask,
              GPIO_MODE_AF | GPIO_OTYPE_PP | GPIO_PUPD_NOPULL |
              GPIO_SPEED_HIGH | af);

    /* Read back the AF field */
    uint32_t actual = read_af_field(port, pin);

    /* Compare */
    if (actual == af) {
      test_pass(entry->name, af, actual);
    } else {
      test_fail(entry->name, af, actual);
    }

    /* Restore: set pin back to input mode (safe default) */
    port->AFRL = saved_afrl;
    port->AFRH = saved_afrh;
    port->MODER = saved_moder;

    /* Also explicitly set to input mode */
    GPIO_Init(port, pin_mask,
              GPIO_MODE_IN | GPIO_PUPD_NOPULL);
  }
}

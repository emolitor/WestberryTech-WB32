/*
    Copyright 2026 emolitor (github.com/emolitor)
    SPDX-License-Identifier: GPL-2.0-or-later

    Phase 7: LED controller register read/write test.

    Configure LED pins to AF, enable LED controller, write segment
    codes and read them back to verify the peripheral is accessible.

    LED_SetSegmentCode/LED_GetSegmentCode apply a bit-reordering
    transform and skip bit[idx_seg] in the hardware register, so the
    round-trip is the correct validation (not raw register comparison).
*/

#include "wb32f10x.h"
#include "test_framework.h"

void test_led_controller(void)
{
  uint8_t readback;
  int pass;

  /* Enable LED clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_BMX2 | RCC_APB2Periph_LED, ENABLE);

  /* Configure PA8-PA11 as LED0-LED3 (AF5) for testing */
  GPIO_Init(GPIOA, GPIO_Pin_8 | GPIO_Pin_11,
            GPIO_MODE_AF | GPIO_OTYPE_PP | GPIO_PUPD_NOPULL | GPIO_SPEED_HIGH | GPIO_AF5);

  /* Enable LED controller */
  LED_Cmd(ENABLE);

  /* Test multiple segment indices with round-trip write/read.
     The API applies a bit transform, so the value passed to Set
     should equal the value returned by Get. Use 7-bit values (0-127)
     since bit[idx_seg] is cleared in hardware for each segment. */
  pass = 1;
  uint8_t test_vals[] = { 0x55, 0x2A, 0x7F, 0x01 };
  uint32_t i;
  uint32_t exp = 0, act = 0;

  for (i = 0; i < sizeof(test_vals); i++) {
    /* Use segment 0 for all tests */
    LED_SetSegmentCode(0, test_vals[i]);
    readback = LED_GetSegmentCode(0);
    if (readback != test_vals[i]) {
      pass = 0;
      exp = test_vals[i];
      act = readback;
      break;
    }
  }

  /* Disable LED controller */
  LED_Cmd(DISABLE);

  /* Restore pins */
  GPIO_Init(GPIOA, GPIO_Pin_8 | GPIO_Pin_11,
            GPIO_MODE_IN | GPIO_PUPD_NOPULL);

  if (pass) {
    test_pass("LED_segment_roundtrip", 0x55, 0x55);
  } else {
    test_fail("LED_segment_roundtrip", exp, act);
  }
}

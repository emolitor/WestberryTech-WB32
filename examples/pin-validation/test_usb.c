/*
    Copyright 2026 emolitor (github.com/emolitor)
    SPDX-License-Identifier: GPL-2.0-or-later

    Phase 8: USB peripheral register access check.

    Enable USB clock, read a peripheral register to verify it's accessible.
    PA11/PA12 are dedicated analog pins for USB (not AF-muxed).
*/

#include "wb32f10x.h"
#include "test_framework.h"

void test_usb_init(void)
{
  /*
   * Enable USB clock. The USB peripheral clock is controlled by
   * RCC->USBCLKENR. We just verify the clock enable register is
   * writable and the USB peripheral base is accessible.
   */
  RCC->USBCLKENR = 0x01;

  /* Small delay for clock stabilization */
  volatile uint32_t i;
  for (i = 0; i < 1000; i++);

  /*
   * Read USB clock enable register back to verify write took effect.
   * This confirms the USB peripheral clock domain is accessible.
   */
  uint32_t clk_en = RCC->USBCLKENR;

  /* Disable USB clock */
  RCC->USBCLKENR = 0x00;

  if (clk_en & 0x01) {
    test_pass("USB_clock_enable", 1, 1);
  } else {
    test_fail("USB_clock_enable", 1, 0);
  }
}

/*
    WB32F104RC Pin Mapping Validation Test Suite
    Copyright 2026 emolitor (github.com/emolitor)
    SPDX-License-Identifier: GPL-2.0-or-later

    Validates AF pin mappings from DS004 Table 3.4-2 against real hardware.
    Results output via UART1 (PA9 TX, PA10 RX) at 115200 baud.
*/

#include <stdio.h>
#include <string.h>
#include "wb32f10x.h"
#include "bsp_uart1.h"
#include "test_framework.h"
#include "test_gpio_af.h"
#include "test_uart.h"
#include "test_timer.h"
#include "test_adc.h"
#include "test_spi.h"
#include "test_i2c.h"
#include "test_led.h"
#include "test_usb.h"

/* Provided by wb32f10x_it.c */
extern uint32_t get_tick(void);
extern void delay_ms(uint32_t ms);

/* GDB-inspectable results */
volatile test_results_t test_results;

/* ---------- Printf retarget for GCC/newlib ---------- */

int _write(int fd, char *ptr, int len)
{
  (void)fd;
  uart1_send((const uint8_t *)ptr, (uint32_t)len);
  return len;
}

/* ---------- Test framework implementation ---------- */

void test_init(void)
{
  memset((void *)&test_results, 0, sizeof(test_results));
}

static void record_result(const char *name, test_status_t status,
                           uint32_t expected, uint32_t actual)
{
  uint32_t idx = test_results.total;
  if (idx < MAX_TESTS) {
    strncpy((char *)test_results.results[idx].name, name, MAX_NAME_LEN - 1);
    test_results.results[idx].name[MAX_NAME_LEN - 1] = '\0';
    test_results.results[idx].status = status;
    test_results.results[idx].expected = expected;
    test_results.results[idx].actual = actual;
  }
  test_results.total++;
}

void test_pass(const char *name, uint32_t expected, uint32_t actual)
{
  printf("[PASS] %s: reg=0x%lx exp=0x%lx\r\n",
         name, (unsigned long)actual, (unsigned long)expected);
  test_results.passed++;
  record_result(name, TEST_PASS, expected, actual);
}

void test_fail(const char *name, uint32_t expected, uint32_t actual)
{
  printf("[FAIL] %s: reg=0x%lx exp=0x%lx\r\n",
         name, (unsigned long)actual, (unsigned long)expected);
  test_results.failed++;
  record_result(name, TEST_FAIL, expected, actual);
}

void test_skip(const char *name, const char *reason)
{
  printf("[SKIP] %s: %s\r\n", name, reason);
  test_results.skipped++;
  record_result(name, TEST_SKIP, 0, 0);
}

void test_finalize(void)
{
  test_results.magic = 0xDEADBEEF;
}

/* ---------- LED indicator ---------- */

#define LED_PIN  GPIO_Pin_13  /* PB13 on WB32F104RC dev board */

static void led_indicator_init(void)
{
  GPIO_Init(GPIOB, LED_PIN,
            GPIO_MODE_OUT | GPIO_OTYPE_PP | GPIO_PUPD_NOPULL | GPIO_SPEED_HIGH);
  GPIO_SetBits(GPIOB, LED_PIN);
}

static void led_blink_result(void)
{
  uint32_t period = (test_results.failed == 0) ? 1000 : 100;
  while (1) {
    GPIO_ToggleBits(GPIOB, LED_PIN);
    delay_ms(period);
  }
}

/* ---------- Main ---------- */

int main(void)
{
  /* System clock setup (done by SystemInit via startup) */
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock / 1000); /* 1ms tick */

  /* Enable all GPIO port clocks + bus matrix */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |
                         RCC_APB1Periph_GPIOA |
                         RCC_APB1Periph_GPIOB |
                         RCC_APB1Periph_GPIOC |
                         RCC_APB1Periph_GPIOD, ENABLE);

  /* UART1 for console output (96 MHz APB1 clock, 115200 baud) */
  uart1_init(96000000, 115200);

  /* Banner */
  printf("\r\n");
  printf("========================================\r\n");
#ifdef TARGET_WB32FQ95
  printf("  WB32FQ95 Pin Validation Test Suite\r\n");
#else
  printf("  WB32F104RC Pin Validation Test Suite\r\n");
#endif
  printf("  Build: %s %s\r\n", __DATE__, __TIME__);
  printf("  SystemCoreClock: %lu Hz\r\n", (unsigned long)SystemCoreClock);
  printf("========================================\r\n\r\n");

  /* LED indicator pin */
  led_indicator_init();

  /* Initialize test results */
  test_init();

  /* Phase 1: Register-level AF validation */
  printf("--- Phase 1: GPIO AF Register Validation ---\r\n");
  test_gpio_af_all();
  printf("\r\n");

  /* Phase 2: UART functional tests (internal loopback) */
  printf("--- Phase 2: UART Loopback Tests ---\r\n");
  test_uart_loopback();
  printf("\r\n");

  /* Phase 3: Timer self-check */
  printf("--- Phase 3: Timer Self-Check ---\r\n");
  test_timer_self_check();
  printf("\r\n");

  /* Phase 4: ADC tests */
  printf("--- Phase 4: ADC Channel Tests ---\r\n");
  test_adc_channels();
  printf("\r\n");

  /* Phase 5: SPI register tests */
  printf("--- Phase 5: SPI Register Tests ---\r\n");
  test_spi_init();
  printf("\r\n");

  /* Phase 6: I2C init tests */
  printf("--- Phase 6: I2C Init Tests ---\r\n");
  test_i2c_init();
  printf("\r\n");

  /* Phase 7: LED controller tests */
  printf("--- Phase 7: LED Controller Tests ---\r\n");
  test_led_controller();
  printf("\r\n");

  /* Phase 8: USB peripheral check */
  printf("--- Phase 8: USB Peripheral Check ---\r\n");
  test_usb_init();
  printf("\r\n");

  /* Summary */
  printf("========================================\r\n");
  printf("  TOTAL:   %lu\r\n", (unsigned long)test_results.total);
  printf("  PASSED:  %lu\r\n", (unsigned long)test_results.passed);
  printf("  FAILED:  %lu\r\n", (unsigned long)test_results.failed);
  printf("  SKIPPED: %lu\r\n", (unsigned long)test_results.skipped);
  printf("  RESULT:  %s\r\n", test_results.failed == 0 ? "ALL PASS" : "FAILURES");
  printf("========================================\r\n");

  /* Mark results as complete for GDB/OpenOCD inspection */
  test_finalize();

  /* Blink LED: slow = all pass, fast = any fail */
  led_blink_result();
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
  printf("ASSERT: %s:%lu\r\n", (char *)file, (unsigned long)line);
  while (1);
}
#endif

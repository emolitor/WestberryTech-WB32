/*
    Copyright 2026 emolitor (github.com/emolitor)
    SPDX-License-Identifier: GPL-2.0-or-later

    Phase 3: Timer self-check.

    Configure each timer for PWM output and verify the counter is running
    by reading it twice with a small delay and checking it has incremented.
*/

#include "wb32f10x.h"
#include "test_framework.h"

extern void delay_ms(uint32_t ms);

static void test_one_timer(const char *name, TIM_TypeDef *tim,
                            GPIO_TypeDef *port, uint16_t pin, uint8_t af,
                            uint8_t channel)
{
  TIM_TimeBaseInitTypeDef tb;
  TIM_OCInitTypeDef oc;

  /* Configure pin to AF mode */
  GPIO_Init(port, pin,
            GPIO_MODE_AF | GPIO_OTYPE_PP | GPIO_PUPD_NOPULL |
            GPIO_SPEED_HIGH | af);

  /* Time base: 1 MHz counter, period = 1000 (1 kHz PWM) */
  tb.TIM_Prescaler = (SystemCoreClock / 1000000) - 1;
  tb.TIM_CounterMode = TIM_CounterMode_Up;
  tb.TIM_Period = 999;
  tb.TIM_ClockDivision = TIM_CKD_DIV1;
  tb.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(tim, &tb);

  /* Output compare: PWM mode 1, 50% duty */
  oc.TIM_OCMode = TIM_OCMode_PWM1;
  oc.TIM_OutputState = TIM_OutputState_Enable;
  oc.TIM_OutputNState = TIM_OutputNState_Disable;
  oc.TIM_Pulse = 500;
  oc.TIM_OCPolarity = TIM_OCPolarity_High;
  oc.TIM_OCNPolarity = TIM_OCNPolarity_High;
  oc.TIM_OCIdleState = TIM_OCIdleState_Reset;
  oc.TIM_OCNIdleState = TIM_OCNIdleState_Reset;

  switch (channel) {
    case 1: TIM_OC1Init(tim, &oc); break;
    case 2: TIM_OC2Init(tim, &oc); break;
    case 3: TIM_OC3Init(tim, &oc); break;
    case 4: TIM_OC4Init(tim, &oc); break;
  }

  /* For TIM1 (advanced timer), enable main output */
  if (tim == TIM1) {
    TIM_CtrlPWMOutputs(tim, ENABLE);
  }

  /* Start timer */
  TIM_Cmd(tim, ENABLE);

  /* Read counter twice with delay */
  uint32_t cnt1 = tim->CNT;
  delay_ms(2);
  uint32_t cnt2 = tim->CNT;

  /* Stop timer */
  TIM_Cmd(tim, DISABLE);

  /* Restore pin to input */
  GPIO_Init(port, pin, GPIO_MODE_IN | GPIO_PUPD_NOPULL);

  /* Verify counter changed (timer was running) */
  if (cnt1 != cnt2) {
    test_pass(name, 1, 1);
  } else {
    test_fail(name, 1, 0);  /* Counter didn't increment */
  }
}

void test_timer_self_check(void)
{
  /* Enable timer clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM1 |
                         RCC_APB1Periph_TIM2 |
                         RCC_APB1Periph_TIM3 |
                         RCC_APB1Periph_TIM4, ENABLE);

  /* TIM1 CH1 on PA8 (AF1) */
  test_one_timer("TIM1_CH1_PA8", TIM1, GPIOA, GPIO_Pin_8, GPIO_AF1, 1);

  /* TIM2 CH1 on PA0 (AF1) */
  test_one_timer("TIM2_CH1_PA0", TIM2, GPIOA, GPIO_Pin_0, GPIO_AF1, 1);

  /* TIM3 CH1 on PA6 (AF2) */
  test_one_timer("TIM3_CH1_PA6", TIM3, GPIOA, GPIO_Pin_6, GPIO_AF2, 1);

  /* TIM4 CH1 on PB6 (AF2) */
  test_one_timer("TIM4_CH1_PB6", TIM4, GPIOB, GPIO_Pin_6, GPIO_AF2, 1);
}

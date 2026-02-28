/*
    Copyright 2026 emolitor (github.com/emolitor)
    SPDX-License-Identifier: GPL-2.0-or-later

    Phase 4: ADC tests.

    - Internal channels: temp sensor (CH16) and Vref (CH17)
    - External channel sweep: PA0-PA7, PB0-PB1, PC0-PC5
    All verified for valid 12-bit conversion results (no timeout).

    Requires ANCTL SAR ADC enable (via PWR_UnlockANA / ANCTL_SARADCCmd)
    before the ADC peripheral will actually convert.
*/

#include "wb32f10x.h"
#include "test_framework.h"

extern void delay_ms(uint32_t ms);

static int adc_read_channel(uint8_t channel)
{
  uint32_t timeout;

  ADC_RegularChannelConfig(channel, 1, ADC_SampleTime_55Cycles5);
  ADC_SoftwareStartConvCmd(ENABLE);

  /* Wait for end of conversion */
  timeout = 100000;
  while (ADC_GetFlagStatus(ADC_FLAG_EOC) == RESET && --timeout);
  if (timeout == 0) return -1;

  ADC_ClearFlag(ADC_FLAG_EOC);
  return (int)ADC_GetConversionValue();
}

/* Test one ADC channel */
static void test_adc_one(const char *name, uint8_t channel,
                          uint16_t min_val, uint16_t max_val)
{
  int val = adc_read_channel(channel);

  if (val < 0) {
    test_fail(name, (uint32_t)channel, 0xFFFFFFFF);
  } else {
    /* Conversion completed -- pass regardless of value since we're
       testing that the ADC channel is accessible, not calibration */
    test_pass(name, (uint32_t)channel, (uint32_t)val);
  }
}

/* External pin channel map */
typedef struct {
  GPIO_TypeDef *port;
  uint16_t pin;
  uint8_t channel;
  const char *name;
} adc_pin_t;

static const adc_pin_t adc_ext_pins[] = {
  { GPIOA, GPIO_Pin_0,  ADC_Channel_0,  "ADC_CH0_PA0" },
  { GPIOA, GPIO_Pin_1,  ADC_Channel_1,  "ADC_CH1_PA1" },
  { GPIOA, GPIO_Pin_2,  ADC_Channel_2,  "ADC_CH2_PA2" },
  { GPIOA, GPIO_Pin_3,  ADC_Channel_3,  "ADC_CH3_PA3" },
  { GPIOA, GPIO_Pin_4,  ADC_Channel_4,  "ADC_CH4_PA4" },
  { GPIOA, GPIO_Pin_5,  ADC_Channel_5,  "ADC_CH5_PA5" },
  { GPIOA, GPIO_Pin_6,  ADC_Channel_6,  "ADC_CH6_PA6" },
  { GPIOA, GPIO_Pin_7,  ADC_Channel_7,  "ADC_CH7_PA7" },
  { GPIOB, GPIO_Pin_0,  ADC_Channel_8,  "ADC_CH8_PB0" },
  { GPIOB, GPIO_Pin_1,  ADC_Channel_9,  "ADC_CH9_PB1" },
  { GPIOC, GPIO_Pin_0,  ADC_Channel_10, "ADC_CH10_PC0" },
  { GPIOC, GPIO_Pin_1,  ADC_Channel_11, "ADC_CH11_PC1" },
  { GPIOC, GPIO_Pin_2,  ADC_Channel_12, "ADC_CH12_PC2" },
  { GPIOC, GPIO_Pin_3,  ADC_Channel_13, "ADC_CH13_PC3" },
  { GPIOC, GPIO_Pin_4,  ADC_Channel_14, "ADC_CH14_PC4" },
  { GPIOC, GPIO_Pin_5,  ADC_Channel_15, "ADC_CH15_PC5" },
};

#define ADC_EXT_COUNT (sizeof(adc_ext_pins) / sizeof(adc_ext_pins[0]))

void test_adc_channels(void)
{
  ADC_InitTypeDef adc_init;
  uint32_t i;

  /* Enable ADC clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_ADC, ENABLE);

  /* Enable SAR ADC analog circuitry (critical -- without this, no conversions) */
  PWR_UnlockANA();
  ANCTL_SARADCCmd(ENABLE);
  PWR_LockANA();

  /* Configure ADC */
  adc_init.ADC_ScanConvMode = DISABLE;
  adc_init.ADC_ContinuousConvMode = DISABLE;
  adc_init.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  adc_init.ADC_DataAlign = ADC_DataAlign_Right;
  adc_init.ADC_NbrOfChannel = 1;
  ADC_Init(&adc_init);

  /* Enable external trigger (required even for software trigger) */
  ADC_ExternalTrigConvCmd(ENABLE);

  /* Enable ADC */
  ADC_Cmd(ENABLE);

  /* Calibrate ADC */
  ADC_StartCalibration();
  while (ADC_GetCalibrationStatus());
  ADC_ResetCalibration();
  while (ADC_GetResetCalibrationStatus());

  /* Small delay for ADC stabilization */
  delay_ms(2);

  /* Internal channels: temp sensor (CH16) and Vrefint (CH17) */
  test_adc_one("ADC_CH16_TempSensor", ADC_Channel_16, 0, 4095);
  test_adc_one("ADC_CH17_Vrefint", ADC_Channel_17, 0, 4095);

  /* External channel sweep */
  for (i = 0; i < ADC_EXT_COUNT; i++) {
    const adc_pin_t *p = &adc_ext_pins[i];

    /* Configure pin as analog input */
    GPIO_Init(p->port, p->pin, GPIO_MODE_ANA);

    test_adc_one(p->name, p->channel, 0, 4095);

    /* Restore pin to input */
    GPIO_Init(p->port, p->pin, GPIO_MODE_IN | GPIO_PUPD_NOPULL);
  }

  /* Disable ADC */
  ADC_Cmd(DISABLE);

  /* Disable SAR ADC analog */
  PWR_UnlockANA();
  ANCTL_SARADCCmd(DISABLE);
  PWR_LockANA();
}

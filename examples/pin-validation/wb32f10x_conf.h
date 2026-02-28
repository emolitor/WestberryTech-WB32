#ifndef __WB32F10x_CONF_H
#define __WB32F10x_CONF_H

#include "wb32f10x_adc.h"
#include "wb32f10x_anctl.h"
#include "wb32f10x_bkp.h"
#include "wb32f10x_crc.h"
#include "wb32f10x_rcc.h"
#include "wb32f10x_dmac.h"
#include "wb32f10x_exti.h"
#include "wb32f10x_gpio.h"
#include "wb32f10x_i2c.h"
#include "wb32f10x_i2s.h"
#include "wb32f10x_iwdg.h"
#include "wb32f10x_led.h"
#include "wb32f10x_fmc.h"
#include "wb32f10x_pwr.h"
#include "wb32f10x_rng.h"
#include "wb32f10x_rtc.h"
#include "wb32f10x_sfm.h"
#include "wb32f10x_spi.h"
#include "wb32f10x_tim.h"
#include "wb32f10x_uart.h"
#include "wb32f10x_wwdg.h"
#include "misc.h"

#ifdef  USE_FULL_ASSERT
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif

#endif /* __WB32F10x_CONF_H */

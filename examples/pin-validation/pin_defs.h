/*
    Complete AF mapping table from DS004 Table 3.4-2 (WB32F104xx)
    Each entry: { port, pin_number, af_number, "description" }

    This drives the register-level AF validation test.
*/

#ifndef PIN_DEFS_H
#define PIN_DEFS_H

#include "wb32f10x.h"

typedef struct {
  GPIO_TypeDef *port;
  uint8_t pin;       /* 0-15 */
  uint8_t af;        /* AF0-AF7 */
  const char *name;  /* Human-readable test name */
} pin_af_entry_t;

/*
 * Excluded pins (cannot test without breaking harness):
 *   PA9, PA10  -- UART1 console
 *   PA13, PA14 -- SWD
 *   PD0, PD1   -- HSE oscillator
 *
 * These are still listed in the table but flagged for skip in the test.
 */
#define PIN_IS_EXCLUDED(port, pin) ( \
  ((port) == GPIOA && (pin) == 9)  || \
  ((port) == GPIOA && (pin) == 10) || \
  ((port) == GPIOA && (pin) == 13) || \
  ((port) == GPIOA && (pin) == 14) || \
  ((port) == GPIOD && (pin) == 0)  || \
  ((port) == GPIOD && (pin) == 1))

static const pin_af_entry_t af_table[] = {
  /* ===== PORT A ===== */
  /* PA0 */
  { GPIOA,  0, 1, "PA0_AF1_TIM2_CH1_ETR" },
  { GPIOA,  0, 7, "PA0_AF7_UART2_CTS" },
  /* PA1 */
  { GPIOA,  1, 1, "PA1_AF1_TIM2_CH2" },
  { GPIOA,  1, 7, "PA1_AF7_UART2_RTS" },
  /* PA2 */
  { GPIOA,  2, 1, "PA2_AF1_TIM2_CH3" },
  { GPIOA,  2, 7, "PA2_AF7_UART2_TX" },
  /* PA3 */
  { GPIOA,  3, 1, "PA3_AF1_TIM2_CH4" },
  { GPIOA,  3, 7, "PA3_AF7_UART2_RX" },
  /* PA4 */
  { GPIOA,  4, 4, "PA4_AF4_QSPI_NSS0" },
  { GPIOA,  4, 5, "PA4_AF5_SPIS1_NSS" },
  { GPIOA,  4, 7, "PA4_AF7_UART2_CK" },
  /* PA5 */
  { GPIOA,  5, 4, "PA5_AF4_QSPI_SCK" },
  { GPIOA,  5, 5, "PA5_AF5_SPIS1_SCK" },
  /* PA6 */
  { GPIOA,  6, 1, "PA6_AF1_TIM1_BKIN" },
  { GPIOA,  6, 2, "PA6_AF2_TIM3_CH1" },
  { GPIOA,  6, 4, "PA6_AF4_QSPI_MI_IO1" },
  { GPIOA,  6, 5, "PA6_AF5_SPIS1_SO" },
  /* PA7 */
  { GPIOA,  7, 1, "PA7_AF1_TIM1_CH1N" },
  { GPIOA,  7, 2, "PA7_AF2_TIM3_CH2" },
  { GPIOA,  7, 4, "PA7_AF4_QSPI_MO_IO0" },
  { GPIOA,  7, 5, "PA7_AF5_SPIS1_SI" },
  /* PA8 */
  { GPIOA,  8, 1, "PA8_AF1_TIM1_CH1" },
  { GPIOA,  8, 5, "PA8_AF5_LED0" },
  { GPIOA,  8, 7, "PA8_AF7_UART1_CK" },
  /* PA9 -- UART1 TX (excluded) */
  { GPIOA,  9, 1, "PA9_AF1_TIM1_CH2" },
  { GPIOA,  9, 5, "PA9_AF5_LED1" },
  { GPIOA,  9, 7, "PA9_AF7_UART1_TX" },
  /* PA10 -- UART1 RX (excluded) */
  { GPIOA, 10, 1, "PA10_AF1_TIM1_CH3" },
  { GPIOA, 10, 5, "PA10_AF5_LED2" },
  { GPIOA, 10, 7, "PA10_AF7_UART1_RX" },
  /* PA11 */
  { GPIOA, 11, 1, "PA11_AF1_TIM1_CH4" },
  { GPIOA, 11, 5, "PA11_AF5_LED3" },
  { GPIOA, 11, 7, "PA11_AF7_UART1_CTS" },
  /* PA12 */
  { GPIOA, 12, 1, "PA12_AF1_TIM1_ETR" },
  { GPIOA, 12, 7, "PA12_AF7_UART1_RTS" },
  /* PA13 -- SWDIO (excluded) */
  { GPIOA, 13, 4, "PA13_AF4_QSPI_NSS1" },
  /* PA14 -- SWCLK (excluded) */
  { GPIOA, 14, 4, "PA14_AF4_QSPI_NSS2" },
  /* PA15 */
  { GPIOA, 15, 1, "PA15_AF1_TIM2_CH1_ETR" },
  { GPIOA, 15, 2, "PA15_AF2_I2S_WS" },
  { GPIOA, 15, 3, "PA15_AF3_I2C_SMBAI" },
  { GPIOA, 15, 4, "PA15_AF4_QSPI_NSS0" },
  { GPIOA, 15, 5, "PA15_AF5_SPIS1_NSS" },

  /* ===== PORT B ===== */
  /* PB0 */
  { GPIOB,  0, 1, "PB0_AF1_TIM1_CH2N" },
  { GPIOB,  0, 2, "PB0_AF2_TIM3_CH3" },
  { GPIOB,  0, 3, "PB0_AF3_I2S_MCLK" },
  { GPIOB,  0, 4, "PB0_AF4_QSPI_IO2" },
  /* PB1 */
  { GPIOB,  1, 1, "PB1_AF1_TIM1_CH3N" },
  { GPIOB,  1, 2, "PB1_AF2_TIM3_CH4" },
  { GPIOB,  1, 4, "PB1_AF4_QSPI_IO3" },
  /* PB2 -- BOOT1 (AF0 only, skip) */
  /* PB3 */
  { GPIOB,  3, 1, "PB3_AF1_TIM2_CH2" },
  { GPIOB,  3, 2, "PB3_AF2_I2S_SCLK" },
  { GPIOB,  3, 4, "PB3_AF4_QSPI_SCK" },
  { GPIOB,  3, 5, "PB3_AF5_SPIS1_SCK" },
  /* PB4 */
  { GPIOB,  4, 2, "PB4_AF2_TIM3_CH1" },
  { GPIOB,  4, 4, "PB4_AF4_QSPI_MI_IO1" },
  { GPIOB,  4, 5, "PB4_AF5_SPIS1_SO" },
  /* PB5 */
  { GPIOB,  5, 2, "PB5_AF2_TIM3_CH2" },
  { GPIOB,  5, 3, "PB5_AF3_I2S_SD1" },
  { GPIOB,  5, 4, "PB5_AF4_QSPI_MO_IO0" },
  { GPIOB,  5, 5, "PB5_AF5_SPIS1_SI" },
  { GPIOB,  5, 6, "PB5_AF6_I2C1_SMBAI" },
  /* PB6 */
  { GPIOB,  6, 2, "PB6_AF2_TIM4_CH1" },
  { GPIOB,  6, 4, "PB6_AF4_QSPI_NSS1" },
  { GPIOB,  6, 5, "PB6_AF5_I2C1_SCL" },
  { GPIOB,  6, 7, "PB6_AF7_UART1_TX" },
  /* PB7 */
  { GPIOB,  7, 2, "PB7_AF2_TIM4_CH2" },
  { GPIOB,  7, 3, "PB7_AF3_I2C1_SDA" },
  { GPIOB,  7, 4, "PB7_AF4_SPIM2_NSS1" },
  { GPIOB,  7, 7, "PB7_AF7_UART1_RX" },
  /* PB8 */
  { GPIOB,  8, 2, "PB8_AF2_TIM4_CH3" },
  { GPIOB,  8, 3, "PB8_AF3_I2C1_SCL" },
  { GPIOB,  8, 4, "PB8_AF4_SPIM2_NSS2" },
  { GPIOB,  8, 7, "PB8_AF7_UART1_CTS" },
  /* PB9 */
  { GPIOB,  9, 2, "PB9_AF2_TIM4_CH4" },
  { GPIOB,  9, 3, "PB9_AF3_I2C1_SDA" },
  { GPIOB,  9, 7, "PB9_AF7_UART1_RTS" },
  /* PB10 */
  { GPIOB, 10, 1, "PB10_AF1_TIM2_CH3" },
  { GPIOB, 10, 2, "PB10_AF2_TIM4_CH1" },
  { GPIOB, 10, 3, "PB10_AF3_I2C2_SCL" },
  { GPIOB, 10, 4, "PB10_AF4_QSPI_NSS2" },
  { GPIOB, 10, 7, "PB10_AF7_UART3_TX" },
  /* PB11 */
  { GPIOB, 11, 1, "PB11_AF1_TIM2_CH4" },
  { GPIOB, 11, 3, "PB11_AF3_I2C2_SDA" },
  { GPIOB, 11, 4, "PB11_AF4_SPIM2_NSS1" },
  { GPIOB, 11, 7, "PB11_AF7_UART3_RX" },
  /* PB12 (LQFP64 only) */
  { GPIOB, 12, 1, "PB12_AF1_TIM1_BKIN" },
  { GPIOB, 12, 2, "PB12_AF2_I2S_WS" },
  { GPIOB, 12, 4, "PB12_AF4_LED4" },
  { GPIOB, 12, 5, "PB12_AF5_SPIM2_NSS0" },
  { GPIOB, 12, 6, "PB12_AF6_SPIS2_NSS" },
  { GPIOB, 12, 7, "PB12_AF7_UART3_CK" },
  /* PB13 (LQFP64 only) */
  { GPIOB, 13, 1, "PB13_AF1_TIM1_CH1N" },
  { GPIOB, 13, 2, "PB13_AF2_I2S_SCLK" },
  { GPIOB, 13, 4, "PB13_AF4_LED5" },
  { GPIOB, 13, 5, "PB13_AF5_SPIM2_SCK" },
  { GPIOB, 13, 6, "PB13_AF6_SPIS2_SCK" },
  { GPIOB, 13, 7, "PB13_AF7_UART3_CTS" },
  /* PB14 (LQFP64 only) */
  { GPIOB, 14, 1, "PB14_AF1_TIM1_CH2N" },
  { GPIOB, 14, 4, "PB14_AF4_LED6" },
  { GPIOB, 14, 5, "PB14_AF5_SPIM2_MI" },
  { GPIOB, 14, 6, "PB14_AF6_SPIS2_SO" },
  { GPIOB, 14, 7, "PB14_AF7_UART3_RTS" },
  /* PB15 (LQFP64 only) */
  { GPIOB, 15, 1, "PB15_AF1_TIM1_CH3N" },
  { GPIOB, 15, 2, "PB15_AF2_I2S_SD0" },
  { GPIOB, 15, 4, "PB15_AF4_LED7" },
  { GPIOB, 15, 5, "PB15_AF5_SPIM2_MO" },
  { GPIOB, 15, 6, "PB15_AF6_SPIS2_SI" },

  /* ===== PORT C (LQFP64 only) ===== */
  /* PC0 */
  { GPIOC,  0, 2, "PC0_AF2_I2S_WS" },
  { GPIOC,  0, 4, "PC0_AF4_SPIM2_NSS0" },
  { GPIOC,  0, 5, "PC0_AF5_SPIS2_NSS" },
  /* PC1 */
  { GPIOC,  1, 2, "PC1_AF2_I2S_SCLK" },
  { GPIOC,  1, 4, "PC1_AF4_SPIM2_SCK" },
  { GPIOC,  1, 5, "PC1_AF5_SPIS2_SCK" },
  /* PC2 */
  { GPIOC,  2, 2, "PC2_AF2_I2S_SD0" },
  { GPIOC,  2, 4, "PC2_AF4_SPIM2_MI" },
  { GPIOC,  2, 5, "PC2_AF5_SPIS2_SO" },
  /* PC3 */
  { GPIOC,  3, 2, "PC3_AF2_I2S_SD1" },
  { GPIOC,  3, 4, "PC3_AF4_SPIM2_MO" },
  { GPIOC,  3, 5, "PC3_AF5_SPIS2_SI" },
  /* PC4 -- TRACECK (AF0 only, skip) */
  /* PC5 */
  { GPIOC,  5, 4, "PC5_AF4_SPIM2_NSS2" },
  /* PC6 */
  { GPIOC,  6, 2, "PC6_AF2_TIM3_CH1" },
  { GPIOC,  6, 3, "PC6_AF3_I2S_MCLK" },
  { GPIOC,  6, 5, "PC6_AF5_LED0" },
  /* PC7 */
  { GPIOC,  7, 2, "PC7_AF2_TIM3_CH2" },
  { GPIOC,  7, 3, "PC7_AF3_I2S_MCLK" },
  { GPIOC,  7, 5, "PC7_AF5_LED1" },
  /* PC8 */
  { GPIOC,  8, 2, "PC8_AF2_TIM3_CH3" },
  { GPIOC,  8, 5, "PC8_AF5_LED2" },
  /* PC9 */
  { GPIOC,  9, 2, "PC9_AF2_TIM3_CH4" },
  { GPIOC,  9, 5, "PC9_AF5_LED3" },
  /* PC10 */
  { GPIOC, 10, 5, "PC10_AF5_LED4" },
  { GPIOC, 10, 7, "PC10_AF7_UART3_TX" },
  /* PC11 */
  { GPIOC, 11, 5, "PC11_AF5_LED5" },
  { GPIOC, 11, 7, "PC11_AF7_UART3_RX" },
  /* PC12 */
  { GPIOC, 12, 2, "PC12_AF2_TIM4_ETR" },
  { GPIOC, 12, 5, "PC12_AF5_LED6" },
  { GPIOC, 12, 7, "PC12_AF7_UART3_CK" },
  /* PC13 -- TAMPER_RTC (AF0 only, skip) */
  /* PC14 -- OSC32_IN (AF0 only, skip) */
  /* PC15 -- OSC32_OUT (AF0 only, skip) */

  /* ===== PORT D ===== */
  /* PD0 -- OSC_IN (excluded) */
  /* PD1 -- OSC_OUT (excluded) */
  /* PD2 */
  { GPIOD,  2, 2, "PD2_AF2_TIM3_ETR" },
  { GPIOD,  2, 5, "PD2_AF5_LED7" },
};

#define AF_TABLE_SIZE (sizeof(af_table) / sizeof(af_table[0]))

#endif /* PIN_DEFS_H */

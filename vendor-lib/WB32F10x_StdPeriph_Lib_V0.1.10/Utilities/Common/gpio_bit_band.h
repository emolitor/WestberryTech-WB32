#ifndef __GPIO_BIT_BAND_H
#define __GPIO_BIT_BAND_H
#include "wb32f10x.h"


#define PAout(n)   (*(volatile uint32_t*)BIT_BAND_ADDR(&GPIOA->ODR, n))  // Output
#define PAin(n)    (*(volatile uint32_t*)BIT_BAND_ADDR(&GPIOA->IDR, n))  // Input

#define PBout(n)   (*(volatile uint32_t*)BIT_BAND_ADDR(&GPIOB->ODR, n))  // Output
#define PBin(n)    (*(volatile uint32_t*)BIT_BAND_ADDR(&GPIOB->IDR, n))  // Input

#define PCout(n)   (*(volatile uint32_t*)BIT_BAND_ADDR(&GPIOC->ODR, n))  // Output
#define PCin(n)    (*(volatile uint32_t*)BIT_BAND_ADDR(&GPIOC->IDR, n))  // Input

#define PDout(n)   (*(volatile uint32_t*)BIT_BAND_ADDR(&GPIOD->ODR, n))  // Output
#define PDin(n)    (*(volatile uint32_t*)BIT_BAND_ADDR(&GPIOD->IDR, n))  // Input


#endif /* __GPIO_BIT_BAND_H */

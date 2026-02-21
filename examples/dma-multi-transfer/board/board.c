/*
 * Board initialization for WB32FQ95 Multi-Transfer DMA Test
 */

#include "hal.h"

/*
 * GPIO clock initialization - must be called before any GPIO operations
 */
static void wb32_gpio_init(void) {

#if WB32_HAS_GPIOA
  rccEnableAPB1(RCC_APB1ENR_GPIOAEN);
#endif

#if WB32_HAS_GPIOB
  rccEnableAPB1(RCC_APB1ENR_GPIOBEN);
#endif

#if WB32_HAS_GPIOC
  rccEnableAPB1(RCC_APB1ENR_GPIOCEN);
#endif

#if WB32_HAS_GPIOD
  rccEnableAPB1(RCC_APB1ENR_GPIODEN);
#endif
}

/*
 * Early initialization code.
 * This initialization must be performed just after stack setup and before
 * any other initialization.
 */
void __early_init(void) {
  wb32_clock_init();
  wb32_gpio_init();
}

/*
 * Board-specific initialization code.
 */
void boardInit(void) {
    /* Board-specific initialization (none needed) */
}

/*
 * Simple LED Blink Demo for WB32FQ95xx
 *
 * Just blinks the LED on PB13 to verify basic ChibiOS operation.
 */

#include "ch.h"
#include "hal.h"

/* LED is on PB13 on the WB32FQ95 dev board */
#define LED_LINE            PAL_LINE(GPIOB, 13U)

int main(void) {
    /* System initializations */
    halInit();
    chSysInit();

    /* Configure LED pin as output */
    palSetLineMode(LED_LINE, PAL_MODE_OUTPUT_PUSHPULL);

    /* Blink forever */
    while (true) {
        palToggleLine(LED_LINE);
        chThdSleepMilliseconds(500);
    }

    return 0;
}

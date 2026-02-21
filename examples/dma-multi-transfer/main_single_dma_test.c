/*
 * WB32FQ95xx Single-Transfer DMA Validation
 *
 * STEP 1: Validate that DMA+PWM works at all with a small buffer
 * STEP 2: Validate max single transfer (511 elements)
 * STEP 3: Only after both pass, proceed to multi-chunk tests
 *
 * Hardware: WB32FQ95 dev board connected via CMSIS-DAP SWD
 * Output: TIM3 CH2 PWM on PA7 (directly observable with oscilloscope)
 * LED: PB13 (blinks to indicate progress)
 */

#include "ch.h"
#include "hal.h"

/*===========================================================================*/
/* Configuration                                                             */
/*===========================================================================*/

/* Test sizes - start small, work up to max */
#define SMALL_BUFFER_SIZE   100     /* Well under 511 limit */
#define MAX_BUFFER_SIZE     511     /* Maximum single DMA transfer */

/* PWM settings for WS2812 timing at 96MHz system clock */
#define PWM_PERIOD          60      /* 96MHz / 60 = 1.6MHz timer, ~800kHz PWM */
#define DUTY_0              18      /* ~0.3us high for '0' bit */
#define DUTY_1              36      /* ~0.6us high for '1' bit */

/* GPIO definitions */
#define LED_LINE            PAL_LINE(GPIOB, 13U)  /* LED on dev board */
#define PWM_OUTPUT_LINE     PAL_LINE(GPIOA, 7U)   /* TIM3 CH2 */

/* DMA stream (DMAC1 Channel 0) */
#define DMA_STREAM          WB32_DMA_STREAM_ID(1, 0)

/*===========================================================================*/
/* Debug Variables - Inspect these via GDB                                   */
/*===========================================================================*/

volatile uint32_t dma_callback_count = 0;
volatile uint32_t dma_error_count = 0;
volatile bool     dma_transfer_complete = false;
volatile uint32_t dma_flags_received = 0;  /* Last flags from callback */

/* Test results: 0=not run, 1=pass, 2=fail */
volatile uint32_t test_results[10];
volatile uint32_t current_test = 0;

/* DMA stream pointer */
static const wb32_dma_stream_t *dma_stream = NULL;

/*===========================================================================*/
/* Buffers                                                                   */
/*===========================================================================*/

static uint32_t small_buffer[SMALL_BUFFER_SIZE];
static uint32_t max_buffer[MAX_BUFFER_SIZE];

/*===========================================================================*/
/* DMA Callback                                                              */
/*===========================================================================*/

/**
 * @brief   DMA transfer complete callback
 * @note    Called from ISR context
 */
static void dma_callback(void *p, uint32_t flags) {
    (void)p;

    dma_callback_count++;
    dma_flags_received = flags;

    if (flags & WB32_DMAC_IT_STATE_TFR) {
        /* Transfer complete */
        dma_transfer_complete = true;
    }

    if (flags & WB32_DMAC_IT_STATE_ERR) {
        /* Transfer error */
        dma_error_count++;
        dma_transfer_complete = true;  /* Stop waiting */
    }
}

/*===========================================================================*/
/* PWM Configuration                                                         */
/*===========================================================================*/

static const PWMConfig pwm_config = {
    .frequency = 96000000 / 2,       /* 48MHz timer clock */
    .period    = PWM_PERIOD,
    .callback  = NULL,
    .channels  = {
        [0] = {.mode = PWM_OUTPUT_DISABLED, .callback = NULL},
        [1] = {.mode = PWM_OUTPUT_ACTIVE_HIGH, .callback = NULL},  /* CH2 on PA7 */
        [2] = {.mode = PWM_OUTPUT_DISABLED, .callback = NULL},
        [3] = {.mode = PWM_OUTPUT_DISABLED, .callback = NULL},
    },
    .cr2  = 0,
    .dier = TIM_DIER_UDE,  /* DMA on update event */
};

/*===========================================================================*/
/* Helper Functions                                                          */
/*===========================================================================*/

static void init_buffer(uint32_t *buf, uint32_t size) {
    /* Fill with alternating duty cycles */
    for (uint32_t i = 0; i < size; i++) {
        buf[i] = ((i / 8) & 1) ? DUTY_1 : DUTY_0;
    }
}

static void blink_led(int count, int delay_ms) {
    for (int i = 0; i < count * 2; i++) {
        palToggleLine(LED_LINE);
        chThdSleepMilliseconds(delay_ms);
    }
}

/**
 * @brief   Start a single DMA transfer
 */
static void start_single_transfer(uint32_t *buffer, uint16_t size) {
    osalSysLock();

    /* Reset state */
    dma_transfer_complete = false;

    /* Configure and start */
    dmaStreamSetSource(dma_stream, buffer);
    dmaStreamSetTransactionSize(dma_stream, size);
    dmaStreamEnable(dma_stream);

    osalSysUnlock();
}

/**
 * @brief   Wait for transfer to complete with timeout
 */
static bool wait_for_transfer(uint32_t timeout_ms) {
    systime_t start = chVTGetSystemTime();
    systime_t timeout = TIME_MS2I(timeout_ms);

    while (!dma_transfer_complete) {
        if (chVTTimeElapsedSinceX(start) > timeout) {
            return false;  /* Timeout */
        }
        chThdYield();
    }
    return true;
}

/*===========================================================================*/
/* Test Functions                                                            */
/*===========================================================================*/

/**
 * @brief   Test 0: Verify DMA stream allocation
 */
static bool test_dma_allocation(void) {
    current_test = 0;

    if (dma_stream == NULL) {
        test_results[0] = 2;  /* Fail */
        return false;
    }

    test_results[0] = 1;  /* Pass */
    return true;
}

/**
 * @brief   Test 1: Small buffer transfer (100 elements)
 */
static bool test_small_transfer(void) {
    current_test = 1;

    /* Reset counters */
    uint32_t initial_callbacks = dma_callback_count;
    dma_error_count = 0;

    /* Start transfer */
    start_single_transfer(small_buffer, SMALL_BUFFER_SIZE);

    /* Wait for completion */
    if (!wait_for_transfer(100)) {
        test_results[1] = 2;  /* Fail - timeout */
        return false;
    }

    /* Verify callback received */
    if (dma_callback_count != initial_callbacks + 1) {
        test_results[1] = 2;  /* Fail - wrong callback count */
        return false;
    }

    /* Verify transfer complete flag */
    if (!(dma_flags_received & WB32_DMAC_IT_STATE_TFR)) {
        test_results[1] = 2;  /* Fail - no TFR flag */
        return false;
    }

    /* Verify no errors */
    if (dma_error_count != 0) {
        test_results[1] = 2;  /* Fail - DMA errors */
        return false;
    }

    test_results[1] = 1;  /* Pass */
    return true;
}

/**
 * @brief   Test 2: Maximum single transfer (511 elements)
 */
static bool test_max_transfer(void) {
    current_test = 2;

    /* Reset counters */
    uint32_t initial_callbacks = dma_callback_count;
    dma_error_count = 0;

    /* Start transfer */
    start_single_transfer(max_buffer, MAX_BUFFER_SIZE);

    /* Wait for completion */
    if (!wait_for_transfer(100)) {
        test_results[2] = 2;  /* Fail - timeout */
        return false;
    }

    /* Verify callback received */
    if (dma_callback_count != initial_callbacks + 1) {
        test_results[2] = 2;  /* Fail - wrong callback count */
        return false;
    }

    /* Verify no errors */
    if (dma_error_count != 0) {
        test_results[2] = 2;  /* Fail - DMA errors */
        return false;
    }

    test_results[2] = 1;  /* Pass */
    return true;
}

/**
 * @brief   Test 3: Repeat small transfer 10 times (consistency check)
 */
static bool test_repeated_transfers(void) {
    current_test = 3;

    uint32_t initial_callbacks = dma_callback_count;
    dma_error_count = 0;

    for (int i = 0; i < 10; i++) {
        start_single_transfer(small_buffer, SMALL_BUFFER_SIZE);
        if (!wait_for_transfer(100)) {
            test_results[3] = 2;  /* Fail - timeout */
            return false;
        }
        if (dma_error_count != 0) {
            test_results[3] = 2;  /* Fail - DMA errors */
            return false;
        }
    }

    /* Verify 10 callbacks received */
    if (dma_callback_count != initial_callbacks + 10) {
        test_results[3] = 2;  /* Fail - wrong callback count */
        return false;
    }

    test_results[3] = 1;  /* Pass */
    return true;
}

/*===========================================================================*/
/* Main                                                                      */
/*===========================================================================*/

int main(void) {
    /*
     * System initializations
     */
    halInit();
    chSysInit();

    /*
     * Initialize LED
     */
    palSetLineMode(LED_LINE, PAL_MODE_OUTPUT_PUSHPULL);
    palClearLine(LED_LINE);

    /*
     * Initialize PWM output pin (PA7 = TIM3 CH2, AF2)
     */
    palSetLineMode(PWM_OUTPUT_LINE, PAL_MODE_ALTERNATE(2) |
                                    PAL_WB32_OTYPE_PUSHPULL |
                                    PAL_WB32_OSPEED_HIGH);

    /*
     * Initialize buffers
     */
    init_buffer(small_buffer, SMALL_BUFFER_SIZE);
    init_buffer(max_buffer, MAX_BUFFER_SIZE);

    /*
     * Allocate DMA stream with callback
     */
    dma_stream = dmaStreamAlloc(DMA_STREAM, 10, dma_callback, NULL);
    osalDbgAssert(dma_stream != NULL, "DMA stream allocation failed");

    /*
     * Configure DMA for memory-to-peripheral, single transfer mode
     */
    dmaStreamSetDestination(dma_stream, &(PWMD3.tim->CCR[1]));  /* TIM3 CH2 CCR */
    dmaStreamSetMode(dma_stream,
        WB32_DMA_CHCFG_HWHIF(WB32_DMAC_HWHIF_TIM3_UP) |  /* TIM3 update event */
        WB32_DMA_CHCFG_DIR_M2P |                          /* Memory to Peripheral */
        WB32_DMA_CHCFG_PSIZE_WORD |                       /* 32-bit peripheral */
        WB32_DMA_CHCFG_MSIZE_WORD |                       /* 32-bit memory */
        WB32_DMA_CHCFG_MINC |                             /* Memory increment */
        WB32_DMA_CHCFG_TCIE |                             /* Transfer complete interrupt */
        WB32_DMA_CHCFG_TEIE |                             /* Transfer error interrupt */
        WB32_DMA_CHCFG_PL(3));                            /* Highest priority */

    /*
     * Start PWM driver
     */
    pwmStart(&PWMD3, &pwm_config);
    pwmEnableChannel(&PWMD3, 1, 0);  /* Channel 2 (0-indexed), initial duty = 0 */

    /*
     * Blink LED twice to indicate ready
     */
    blink_led(2, 200);

    /*
     * ========== RUN SINGLE-TRANSFER TESTS ==========
     *
     * GDB commands:
     *   (gdb) print dma_callback_count
     *   (gdb) print dma_error_count
     *   (gdb) print test_results
     *   (gdb) print dma_flags_received
     */

    bool all_passed = true;

    /* Test 0: DMA allocation */
    palSetLine(LED_LINE);
    if (!test_dma_allocation()) all_passed = false;
    chThdSleepMilliseconds(100);
    palClearLine(LED_LINE);
    chThdSleepMilliseconds(100);

    /* Test 1: Small buffer (100 elements) */
    palSetLine(LED_LINE);
    if (!test_small_transfer()) all_passed = false;
    chThdSleepMilliseconds(100);
    palClearLine(LED_LINE);
    chThdSleepMilliseconds(100);

    /* Test 2: Max buffer (511 elements) */
    palSetLine(LED_LINE);
    if (!test_max_transfer()) all_passed = false;
    chThdSleepMilliseconds(100);
    palClearLine(LED_LINE);
    chThdSleepMilliseconds(100);

    /* Test 3: Repeated transfers */
    palSetLine(LED_LINE);
    if (!test_repeated_transfers()) all_passed = false;
    chThdSleepMilliseconds(100);
    palClearLine(LED_LINE);
    chThdSleepMilliseconds(100);

    /*
     * Final status indication
     */
    if (all_passed) {
        /* Success: Fast blink */
        while (true) {
            palToggleLine(LED_LINE);
            chThdSleepMilliseconds(100);
        }
    } else {
        /* Failure: Slow blink */
        while (true) {
            palToggleLine(LED_LINE);
            chThdSleepMilliseconds(500);
        }
    }
}

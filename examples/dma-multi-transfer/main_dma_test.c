/*
 * WB32FQ95xx Multi-Transfer DMA Validation Demo
 * Copyright 2025 emolitor (github.com/emolitor)
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * This demo validates the multi-chunk DMA transfer mechanism needed to work
 * around the WB32FQ95xx silicon errata where DMA BLOCK_TS is limited to 9 bits
 * (max 511) instead of the documented 12 bits (max 4095).
 *
 * Hardware: WB32FQ95 dev board connected via CMSIS-DAP SWD
 * Output: TIM3 CH2 PWM on PA7 (directly observable with oscilloscope)
 * LED: PB14 (blinks to indicate progress)
 *
 * Debug Variables (inspect via GDB):
 *   - dma_callback_count: Total number of DMA callbacks received
 *   - dma_chunk_complete[]: Array tracking which chunks completed
 *   - dma_transfer_complete: Set true when all chunks are done
 *   - dma_error_count: Number of DMA errors encountered
 *   - current_test: Current test number being run
 *   - test_results[]: Pass/fail status for each test
 */

#include "ch.h"
#include "hal.h"

/*===========================================================================*/
/* Configuration                                                             */
/*===========================================================================*/

/* Simulate 82 LEDs worth of WS2812 data (like Bridge75 keyboard) */
#define LED_COUNT           82
#define BITS_PER_LED        24
#define RESET_BITS          40       /* 50us reset at 1.25us/bit */
#define TOTAL_BITS          (LED_COUNT * BITS_PER_LED + RESET_BITS)  /* 2008 bits */

/* Multi-transfer DMA constants (matching ws2812_pwm.c) */
#define DMA_CHUNK_SIZE      511
#define DMA_CHUNK_COUNT     ((TOTAL_BITS + DMA_CHUNK_SIZE - 1) / DMA_CHUNK_SIZE)

/* PWM settings for WS2812 timing at 96MHz system clock */
#define PWM_FREQUENCY       800000   /* 800kHz = 1.25us period */
#define PWM_PERIOD          60       /* 96MHz / 60 = 1.6MHz, period = 75 ticks for 800kHz */
#define DUTY_0              18       /* ~0.3us high for '0' bit */
#define DUTY_1              36       /* ~0.6us high for '1' bit */

/* GPIO definitions */
#define LED_LINE            PAL_LINE(GPIOB, 13U)  /* Actual LED on dev board */
#define PWM_OUTPUT_LINE     PAL_LINE(GPIOA, 7U)   /* TIM3 CH2 */

/* DMA stream (DMAC1 Channel 0) - note: DMA controller is 1-indexed */
#define DMA_STREAM          WB32_DMA_STREAM_ID(1, 0)

/*===========================================================================*/
/* Debug Variables - Inspect these via GDB                                   */
/*===========================================================================*/

/* Counters and flags */
volatile uint32_t dma_callback_count = 0;
volatile uint32_t dma_error_count = 0;
volatile uint16_t dma_chunk_index = 0;
volatile bool     dma_transfer_active = false;
volatile bool     dma_transfer_complete = false;

/* Per-chunk tracking (for detailed debugging) */
volatile uint32_t dma_chunk_complete[DMA_CHUNK_COUNT];
volatile uint32_t dma_chunk_sizes[DMA_CHUNK_COUNT];

/* Test results */
volatile uint32_t current_test = 0;
volatile uint32_t test_results[10];  /* 0=not run, 1=pass, 2=fail */

/* DMA stream pointer */
static const wb32_dma_stream_t *dma_stream;

/*===========================================================================*/
/* Frame Buffer                                                              */
/*===========================================================================*/

/* Buffer matches ws2812_pwm.c: uint32_t for WB32 */
static uint32_t frame_buffer[TOTAL_BITS + 1];

/*===========================================================================*/
/* Helper Functions                                                          */
/*===========================================================================*/

static inline uint16_t get_chunk_size(uint16_t chunk_idx) {
    uint32_t start = (uint32_t)chunk_idx * DMA_CHUNK_SIZE;
    uint32_t remaining = TOTAL_BITS - start;
    return (remaining > DMA_CHUNK_SIZE) ? DMA_CHUNK_SIZE : (uint16_t)remaining;
}

static void init_frame_buffer(void) {
    /* Fill with alternating duty cycles to create visible pattern */
    for (uint32_t i = 0; i < LED_COUNT * BITS_PER_LED; i++) {
        /* Create a pattern: every 8 bits alternate */
        frame_buffer[i] = ((i / 8) & 1) ? DUTY_1 : DUTY_0;
    }
    /* Reset period: all zeros */
    for (uint32_t i = LED_COUNT * BITS_PER_LED; i < TOTAL_BITS; i++) {
        frame_buffer[i] = 0;
    }
    frame_buffer[TOTAL_BITS] = 0;  /* Extra safety byte */

    /* Record expected chunk sizes for verification */
    for (uint16_t i = 0; i < DMA_CHUNK_COUNT; i++) {
        dma_chunk_sizes[i] = get_chunk_size(i);
        dma_chunk_complete[i] = 0;
    }
}

/*===========================================================================*/
/* DMA Callback and Transfer Functions                                       */
/*===========================================================================*/

/**
 * @brief   DMA transfer complete callback - chains next chunk
 * @note    Called from ISR context
 */
static void dma_callback(void *p, uint32_t flags) {
    (void)p;

    dma_callback_count++;

    if (flags & WB32_DMAC_IT_STATE_TFR) {
        /* Mark current chunk as complete */
        if (dma_chunk_index < DMA_CHUNK_COUNT) {
            dma_chunk_complete[dma_chunk_index] = 1;
        }

        dma_chunk_index++;

        if (dma_chunk_index < DMA_CHUNK_COUNT) {
            /* Start next chunk immediately */
            uint32_t offset = (uint32_t)dma_chunk_index * DMA_CHUNK_SIZE;
            uint16_t chunk_size = get_chunk_size(dma_chunk_index);

            dmaStreamSetSource(dma_stream, &frame_buffer[offset]);
            dmaStreamSetTransactionSize(dma_stream, chunk_size);
            dmaStreamEnable(dma_stream);
        } else {
            /* All chunks done */
            dma_transfer_active = false;
            dma_transfer_complete = true;
        }
    }

    if (flags & WB32_DMAC_IT_STATE_ERR) {
        dma_error_count++;
        dma_transfer_active = false;
    }
}

/**
 * @brief   Start a new multi-chunk DMA transfer
 */
static void start_transfer(void) {
    osalSysLock();

    if (dma_transfer_active) {
        osalSysUnlock();
        return;
    }

    /* Reset state */
    dma_chunk_index = 0;
    dma_transfer_active = true;
    dma_transfer_complete = false;

    /* Clear chunk completion tracking */
    for (uint16_t i = 0; i < DMA_CHUNK_COUNT; i++) {
        dma_chunk_complete[i] = 0;
    }

    /* Start first chunk */
    dmaStreamSetSource(dma_stream, frame_buffer);
    dmaStreamSetTransactionSize(dma_stream, get_chunk_size(0));
    dmaStreamEnable(dma_stream);

    osalSysUnlock();
}

/**
 * @brief   Wait for current transfer to complete
 */
static bool wait_for_transfer(uint32_t timeout_ms) {
    systime_t start = chVTGetSystemTime();
    systime_t timeout = TIME_MS2I(timeout_ms);

    while (dma_transfer_active) {
        if (chVTTimeElapsedSinceX(start) > timeout) {
            return false;  /* Timeout */
        }
        chThdYield();
    }
    return true;
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
/* Test Functions                                                            */
/*===========================================================================*/

/**
 * @brief   Test 1: Verify DMA chunk calculations
 */
static bool test_chunk_calculations(void) {
    current_test = 1;

    /* Expected: 2008 bits / 511 = 3.93, so 4 chunks */
    /* Chunk 0: 511, Chunk 1: 511, Chunk 2: 511, Chunk 3: 475 */
    if (DMA_CHUNK_COUNT != 4) {
        test_results[1] = 2;  /* Fail */
        return false;
    }

    if (get_chunk_size(0) != 511) { test_results[1] = 2; return false; }
    if (get_chunk_size(1) != 511) { test_results[1] = 2; return false; }
    if (get_chunk_size(2) != 511) { test_results[1] = 2; return false; }
    if (get_chunk_size(3) != 475) { test_results[1] = 2; return false; }

    test_results[1] = 1;  /* Pass */
    return true;
}

/**
 * @brief   Test 2: Single multi-chunk transfer
 */
static bool test_single_transfer(void) {
    current_test = 2;

    /* Reset counters */
    dma_callback_count = 0;
    dma_error_count = 0;

    /* Start transfer */
    start_transfer();

    /* Wait for completion (should be ~2.5ms for 2008 bits at 800kHz) */
    if (!wait_for_transfer(100)) {
        test_results[2] = 2;  /* Fail - timeout */
        return false;
    }

    /* Verify all chunks completed */
    for (uint16_t i = 0; i < DMA_CHUNK_COUNT; i++) {
        if (dma_chunk_complete[i] != 1) {
            test_results[2] = 2;  /* Fail - chunk not completed */
            return false;
        }
    }

    /* Verify callback count matches chunk count */
    if (dma_callback_count != DMA_CHUNK_COUNT) {
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
 * @brief   Test 3: Multiple consecutive transfers
 */
static bool test_consecutive_transfers(void) {
    current_test = 3;

    uint32_t initial_callbacks = dma_callback_count;

    for (int i = 0; i < 5; i++) {
        start_transfer();
        if (!wait_for_transfer(100)) {
            test_results[3] = 2;  /* Fail - timeout */
            return false;
        }
        if (dma_error_count != 0) {
            test_results[3] = 2;  /* Fail - DMA errors */
            return false;
        }
    }

    /* Should have 5 * 4 = 20 more callbacks */
    if (dma_callback_count != initial_callbacks + 20) {
        test_results[3] = 2;  /* Fail - wrong callback count */
        return false;
    }

    test_results[3] = 1;  /* Pass */
    return true;
}

/**
 * @brief   Test 4: Rapid-fire transfers (stress test)
 */
static bool test_rapid_transfers(void) {
    current_test = 4;

    uint32_t initial_callbacks = dma_callback_count;
    uint32_t initial_errors = dma_error_count;

    for (int i = 0; i < 100; i++) {
        start_transfer();
        if (!wait_for_transfer(100)) {
            test_results[4] = 2;
            return false;
        }
    }

    /* Should have 100 * 4 = 400 more callbacks */
    if (dma_callback_count != initial_callbacks + 400) {
        test_results[4] = 2;
        return false;
    }

    /* Should have no new errors */
    if (dma_error_count != initial_errors) {
        test_results[4] = 2;
        return false;
    }

    test_results[4] = 1;  /* Pass */
    return true;
}

/*===========================================================================*/
/* Main                                                                      */
/*===========================================================================*/

int main(void) {
    /*
     * System initializations.
     */
    halInit();
    chSysInit();

    /*
     * Initialize LED for status indication
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
     * Initialize frame buffer
     */
    init_frame_buffer();

    /*
     * Allocate DMA stream with callback
     */
    dma_stream = dmaStreamAlloc(DMA_STREAM, 10, dma_callback, NULL);
    osalDbgAssert(dma_stream != NULL, "DMA stream allocation failed");

    /*
     * Configure DMA (matching ws2812_pwm.c but without circular mode)
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
    for (int i = 0; i < 4; i++) {
        palToggleLine(LED_LINE);
        chThdSleepMilliseconds(200);
    }

    /*
     * ========== RUN TESTS ==========
     *
     * Set a breakpoint here in GDB and step through tests:
     *   break main.c:350
     *   continue
     *
     * After tests, inspect:
     *   print dma_callback_count
     *   print dma_chunk_complete
     *   print test_results
     */

    bool all_passed = true;

    /* Test 1: Chunk calculations */
    palSetLine(LED_LINE);
    if (!test_chunk_calculations()) all_passed = false;
    chThdSleepMilliseconds(100);
    palClearLine(LED_LINE);
    chThdSleepMilliseconds(100);

    /* Test 2: Single transfer */
    palSetLine(LED_LINE);
    if (!test_single_transfer()) all_passed = false;
    chThdSleepMilliseconds(100);
    palClearLine(LED_LINE);
    chThdSleepMilliseconds(100);

    /* Test 3: Consecutive transfers */
    palSetLine(LED_LINE);
    if (!test_consecutive_transfers()) all_passed = false;
    chThdSleepMilliseconds(100);
    palClearLine(LED_LINE);
    chThdSleepMilliseconds(100);

    /* Test 4: Stress test */
    palSetLine(LED_LINE);
    if (!test_rapid_transfers()) all_passed = false;
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

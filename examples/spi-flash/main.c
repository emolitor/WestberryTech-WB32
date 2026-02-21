/*
    WB32FQ95 External SPI Flash Validation Demo

    Tests an external SPI NOR flash chip connected via the WB32FQ95 QSPI
    peripheral (used in standard SPI mode through ChibiOS's SPIDQ driver).

    Pin mapping:
      PB3  - QSPI_SCK  (AF5)
      PB4  - QSPI_MISO (AF5)
      PB5  - QSPI_MOSI (AF5)
      PC12 - CS         (GPIO output, active low)

    Results are stored in a volatile struct for GDB inspection.
    LED on PB14 blinks to indicate status: slow = pass, fast = fail.
*/

#include <string.h>

#include "ch.h"
#include "hal.h"

/*===========================================================================*/
/* SPI flash commands (JEDEC standard).                                      */
/*===========================================================================*/

#define CMD_READ_JEDEC_ID     0x9F
#define CMD_READ_STATUS       0x05
#define CMD_WRITE_ENABLE      0x06
#define CMD_SECTOR_ERASE      0x20
#define CMD_PAGE_PROGRAM      0x02
#define CMD_READ_DATA         0x03

#define STATUS_WEL            (1U << 1)  /* Write Enable Latch */
#define STATUS_BUSY           (1U << 0)  /* Write In Progress  */

/*===========================================================================*/
/* Test parameters.                                                          */
/*===========================================================================*/

/* Test address: sector 255 (last 4K sector in a 1MB flash). */
#define TEST_ADDR             0x0FF000U
#define TEST_SIZE             256U
#define SECTOR_SIZE           4096U

/*===========================================================================*/
/* LED indicator.                                                            */
/*===========================================================================*/

#define LED_LINE              PAL_LINE(GPIOB, 14U)
#define BLINK_PASS_MS         1000
#define BLINK_FAIL_MS         100

/*===========================================================================*/
/* Test results (volatile for GDB inspection).                               */
/*===========================================================================*/

volatile struct {
  uint8_t  jedec_id[3];       /* Manufacturer, Type, Capacity */
  uint8_t  status_reg;        /* Status register after init   */
  bool     id_valid;          /* JEDEC ID != 0x00/0xFF        */
  bool     erase_ok;          /* Erased sector reads all 0xFF */
  bool     write_ok;          /* Written data reads back OK   */
  bool     all_pass;          /* Overall result               */
  uint32_t error_offset;      /* First mismatch offset        */
} test_results;

/*===========================================================================*/
/* Buffers.                                                                  */
/*===========================================================================*/

static uint8_t tx_buf[4 + TEST_SIZE];
static uint8_t read_buf[TEST_SIZE];
static uint8_t original_data[TEST_SIZE];

/*===========================================================================*/
/* SPI configuration: QSPI peripheral, CS on PC12, Mode 0, ~1 MHz.          */
/*===========================================================================*/

static const SPIConfig spi_cfg = {
  .end_cb              = NULL,
  .ssport              = GPIOC,
  .sspad               = 12U,
  .SPI_CPOL            = SPI_CPOL_Low,
  .SPI_CPHA            = SPI_CPHA_1Edge,
  .SPI_BaudRatePrescaler = 48,
};

/*===========================================================================*/
/* SPI flash helper functions.                                               */
/*===========================================================================*/

/*
 * Send a command and receive response bytes.
 * cmd_len includes the command byte(s), resp_len is the number of
 * response bytes to clock out after the command.
 */
static void flash_cmd(const uint8_t *cmd, size_t cmd_len,
                       uint8_t *resp, size_t resp_len) {
  spiSelect(&SPIDQ);
  if (cmd_len > 0)
    spiSend(&SPIDQ, cmd_len, cmd);
  if (resp_len > 0)
    spiReceive(&SPIDQ, resp_len, resp);
  spiUnselect(&SPIDQ);
}

/*
 * Send a simple one-byte command with no response (e.g. Write Enable).
 */
static void flash_cmd_simple(uint8_t cmd) {
  spiSelect(&SPIDQ);
  spiSend(&SPIDQ, 1, &cmd);
  spiUnselect(&SPIDQ);
}

/*
 * Read the status register.
 */
static uint8_t flash_read_status(void) {
  uint8_t cmd = CMD_READ_STATUS;
  uint8_t status;
  flash_cmd(&cmd, 1, &status, 1);
  return status;
}

/*
 * Poll until the BUSY bit clears (write/erase complete).
 */
static void flash_wait_ready(void) {
  while (flash_read_status() & STATUS_BUSY)
    chThdSleepMilliseconds(1);
}

/*
 * Read JEDEC ID (3 bytes: manufacturer, device type, capacity).
 */
static void flash_read_jedec_id(uint8_t *id) {
  uint8_t cmd = CMD_READ_JEDEC_ID;
  flash_cmd(&cmd, 1, id, 3);
}

/*
 * Issue Write Enable command and verify WEL bit is set.
 * Returns true if WEL is set after the command.
 */
static bool flash_write_enable(void) {
  flash_cmd_simple(CMD_WRITE_ENABLE);
  return (flash_read_status() & STATUS_WEL) != 0;
}

/*
 * Erase a 4K sector containing the given address.
 */
static void flash_sector_erase(uint32_t addr) {
  uint8_t cmd[4];
  cmd[0] = CMD_SECTOR_ERASE;
  cmd[1] = (uint8_t)(addr >> 16);
  cmd[2] = (uint8_t)(addr >> 8);
  cmd[3] = (uint8_t)(addr);

  flash_write_enable();
  spiSelect(&SPIDQ);
  spiSend(&SPIDQ, 4, cmd);
  spiUnselect(&SPIDQ);
  flash_wait_ready();
}

/*
 * Program up to 256 bytes (one page).
 */
static void flash_page_program(uint32_t addr, const uint8_t *data, size_t len) {
  tx_buf[0] = CMD_PAGE_PROGRAM;
  tx_buf[1] = (uint8_t)(addr >> 16);
  tx_buf[2] = (uint8_t)(addr >> 8);
  tx_buf[3] = (uint8_t)(addr);
  memcpy(&tx_buf[4], data, len);

  flash_write_enable();
  spiSelect(&SPIDQ);
  spiSend(&SPIDQ, 4 + len, tx_buf);
  spiUnselect(&SPIDQ);
  flash_wait_ready();
}

/*
 * Read data from flash.
 */
static void flash_read(uint32_t addr, uint8_t *data, size_t len) {
  uint8_t cmd[4];
  cmd[0] = CMD_READ_DATA;
  cmd[1] = (uint8_t)(addr >> 16);
  cmd[2] = (uint8_t)(addr >> 8);
  cmd[3] = (uint8_t)(addr);

  spiSelect(&SPIDQ);
  spiSend(&SPIDQ, 4, cmd);
  spiReceive(&SPIDQ, len, data);
  spiUnselect(&SPIDQ);
}

/*===========================================================================*/
/* Test sequence.                                                            */
/*===========================================================================*/

static void run_tests(void) {
  uint32_t i;

  /* Clear results. */
  memset((void *)&test_results, 0, sizeof(test_results));

  /*
   * Test 1: Read JEDEC ID.
   */
  {
    uint8_t id[3];
    flash_read_jedec_id(id);
    test_results.jedec_id[0] = id[0];
    test_results.jedec_id[1] = id[1];
    test_results.jedec_id[2] = id[2];
  }
  test_results.id_valid =
    !(test_results.jedec_id[0] == 0x00 &&
      test_results.jedec_id[1] == 0x00 &&
      test_results.jedec_id[2] == 0x00) &&
    !(test_results.jedec_id[0] == 0xFF &&
      test_results.jedec_id[1] == 0xFF &&
      test_results.jedec_id[2] == 0xFF);

  if (!test_results.id_valid) {
    test_results.all_pass = false;
    return;
  }

  /*
   * Test 2: Read Status Register.
   */
  test_results.status_reg = flash_read_status();

  /*
   * Test 3: Read original data at test address (for reference).
   */
  flash_read(TEST_ADDR, original_data, TEST_SIZE);

  /*
   * Test 4: Sector Erase.
   */
  flash_sector_erase(TEST_ADDR);

  /*
   * Test 5: Verify erase — all bytes should be 0xFF.
   */
  flash_read(TEST_ADDR, read_buf, TEST_SIZE);
  test_results.erase_ok = true;
  for (i = 0; i < TEST_SIZE; i++) {
    if (read_buf[i] != 0xFF) {
      test_results.erase_ok = false;
      test_results.error_offset = i;
      break;
    }
  }

  if (!test_results.erase_ok) {
    test_results.all_pass = false;
    return;
  }

  /*
   * Test 6: Page Program — write a test pattern.
   *         Pattern: byte[i] = i (0x00..0xFF).
   */
  {
    uint8_t pattern[TEST_SIZE];
    for (i = 0; i < TEST_SIZE; i++)
      pattern[i] = (uint8_t)i;
    flash_page_program(TEST_ADDR, pattern, TEST_SIZE);
  }

  /*
   * Test 7: Read back and verify.
   */
  flash_read(TEST_ADDR, read_buf, TEST_SIZE);
  test_results.write_ok = true;
  for (i = 0; i < TEST_SIZE; i++) {
    if (read_buf[i] != (uint8_t)i) {
      test_results.write_ok = false;
      test_results.error_offset = i;
      break;
    }
  }

  test_results.all_pass = test_results.id_valid &&
                           test_results.erase_ok &&
                           test_results.write_ok;
}

/*===========================================================================*/
/* Application entry point.                                                  */
/*===========================================================================*/

int main(void) {

  halInit();
  chSysInit();

  /*
   * Configure LED pin (PB14) as push-pull output.
   */
  palSetPadMode(GPIOB, 14U,
                PAL_WB32_MODE_OUTPUT | PAL_WB32_OTYPE_PUSHPULL);

  /*
   * Configure SPI pins:
   *   PB3 = QSPI_SCK  (AF5, push-pull)
   *   PB4 = QSPI_MISO (AF5, input/floating)
   *   PB5 = QSPI_MOSI (AF5, push-pull)
   */
  palSetPadMode(GPIOB, 3U,
                PAL_WB32_MODE_ALTERNATE | PAL_WB32_OTYPE_PUSHPULL |
                PAL_WB32_ALTERNATE(5));
  palSetPadMode(GPIOB, 4U,
                PAL_WB32_MODE_ALTERNATE | PAL_WB32_PUPDR_FLOATING |
                PAL_WB32_ALTERNATE(5));
  palSetPadMode(GPIOB, 5U,
                PAL_WB32_MODE_ALTERNATE | PAL_WB32_OTYPE_PUSHPULL |
                PAL_WB32_ALTERNATE(5));

  /*
   * Configure CS pin (PC12) as push-pull output, initially high (deselected).
   */
  palSetPadMode(GPIOC, 12U,
                PAL_WB32_MODE_OUTPUT | PAL_WB32_OTYPE_PUSHPULL);
  palSetPad(GPIOC, 12U);

  /*
   * Start the SPI driver.
   */
  spiStart(&SPIDQ, &spi_cfg);

  /*
   * Run the test sequence.
   */
  run_tests();

  /*
   * Blink LED to indicate result.
   * Slow blink (1 Hz) = all tests passed.
   * Fast blink (10 Hz) = at least one test failed.
   * Place a GDB breakpoint here to inspect test_results.
   */
  unsigned blink_ms = test_results.all_pass ? BLINK_PASS_MS : BLINK_FAIL_MS;

  while (true) {
    palToggleLine(LED_LINE);
    chThdSleepMilliseconds(blink_ms);
  }
}

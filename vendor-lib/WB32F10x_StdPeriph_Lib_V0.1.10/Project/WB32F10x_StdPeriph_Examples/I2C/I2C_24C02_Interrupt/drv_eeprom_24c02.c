/* Includes ------------------------------------------------------------------*/
#include "drv_eeprom_24c02.h"
#include <stddef.h>

/* Private typedef -----------------------------------------------------------*/
typedef struct _i2c_xfer_info_t {
  const uint8_t* tx_buf;
  uint32_t tx_len;
  uint8_t* rx_buf;
  uint32_t rx_len;
  uint32_t rx_cmd_len;  
  uint32_t flag_complete;
  uint32_t tx_abrt_source;
} i2c_xfer_info_t;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
i2c_xfer_info_t g_i2c_xfer_info;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void eeprom_24c02_init(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_BMX2 |RCC_APB2Periph_I2C2, ENABLE);

  /*
    PB10 (I2C2_SCL)
    PB11 (I2C2_SDA)
  */
  GPIO_Init(GPIOB, GPIO_Pin_10 |GPIO_Pin_11, GPIO_MODE_AF |GPIO_OTYPE_OD |GPIO_PUPD_UP |GPIO_SPEED_HIGH |GPIO_AF4);

  I2C_DeInit(I2C2);
  I2C_TxFIFOThresholdConfig(I2C2, 0);
  I2C_RxFIFOThresholdConfig(I2C2, 0);
  I2C_Init(I2C2, I2C_CON_SLAVE_DISABLE | I2C_CON_RESTART_EN | I2C_CON_SPEED_STANDARD | I2C_CON_MASTER_MODE);
  I2C_TargetAddressConfig(I2C2, 0xA0 >> 1);
  I2C_ITConfig(I2C2, 0xFFFF, DISABLE);

  I2C2->SS_SCL_HCNT = 373;  /* tHIGH = (373 + FS_SPKLEN + 7) / 96MHz = 4us */
  I2C2->SS_SCL_LCNT = 451;  /* tLOW = (451 + 1) / 96MHz = 4.708us */
  I2C2->FS_SPKLEN = 4;      /* tSP = 4 / 96MHz = 41.67ns */
  I2C2->SDA_SETUP = 24;     /* tSU;DAT = 24 / 96MHz = 250ns */
  I2C2->SDA_HOLD = 29;      /* tHD;DAT = 29 / 96MHz = 302.08ns */

  NVIC_InitStructure.NVIC_IRQChannel = I2C2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

// I2C2 Interrupt Routine
void I2C2_IRQHandler(void)
{
  uint32_t cmd;
  uint32_t tx_limit, rx_limit;

  if(I2C_GetITStatus(I2C2, I2C_IT_TX_ABRT) != RESET)
  {
    g_i2c_xfer_info.tx_abrt_source = I2C_GetTxAbortSource(I2C2);
    I2C2->INTR_MASK = I2C_INTR_STOP_DET;   // Disable all interrupt except STOP_DET interrupt
    goto tx_aborted;
  }

  if(I2C_GetITStatus(I2C2, I2C_IT_RX_FULL) != RESET)
  {
    while((I2C_GetFlagStatus(I2C2, I2C_FLAG_RFNE) != RESET) && (g_i2c_xfer_info.rx_len))
    {
      *g_i2c_xfer_info.rx_buf = I2C_ReadData(I2C2);
      g_i2c_xfer_info.rx_buf++;
      g_i2c_xfer_info.rx_len--;
    }
  }

  if(I2C_GetITStatus(I2C2, I2C_IT_TX_EMPTY) != RESET)
  {
    tx_limit = 8 - I2C_GetTxFIFOLevel(I2C2);
    rx_limit = 8 - I2C_GetRxFIFOLevel(I2C2);
    while((tx_limit > 0) && (rx_limit > 0))
    {
      if((g_i2c_xfer_info.tx_len + g_i2c_xfer_info.rx_cmd_len) == 0) {
        I2C_ITConfig(I2C2, I2C_IT_TX_EMPTY, DISABLE);   // Disable TX Empty Interrupt
        break;
      }

      cmd = 0;
      if((g_i2c_xfer_info.tx_len + g_i2c_xfer_info.rx_cmd_len) == 1) {
        cmd |= I2C_DATA_CMD_STOP;
      }

      if(g_i2c_xfer_info.tx_len != 0)
      {
        I2C_WriteDataCmd(I2C2, cmd | *g_i2c_xfer_info.tx_buf);
        g_i2c_xfer_info.tx_buf++;
        g_i2c_xfer_info.tx_len--;
      }
      else if(g_i2c_xfer_info.rx_cmd_len != 0)
      {
        I2C_WriteDataCmd(I2C2, cmd | I2C_DATA_CMD_READ);
        g_i2c_xfer_info.rx_cmd_len--;
        rx_limit--;
      }
      tx_limit--;
    }
  }

tx_aborted:
  if(I2C_GetITStatus(I2C2, I2C_IT_STOP_DET) != RESET) {
    I2C_ClearITPendingBit(I2C2, 0xFFFF);    // Clear all interrupt flag
    I2C_ITConfig(I2C2, 0xFFFF, DISABLE);    // Disable all interrupt
    g_i2c_xfer_info.flag_complete = 1;
  }
}


uint32_t eeprom_24c02_random_read(uint8_t addr, uint8_t* pdata)
{
  g_i2c_xfer_info.tx_buf = &addr;
  g_i2c_xfer_info.tx_len = 1;

  g_i2c_xfer_info.rx_buf = pdata;
  g_i2c_xfer_info.rx_len = 1;
  g_i2c_xfer_info.rx_cmd_len = 1;

  g_i2c_xfer_info.flag_complete = 0;
  g_i2c_xfer_info.tx_abrt_source = 0x00;

  I2C_Cmd(I2C2, ENABLE);
  I2C_ClearITPendingBit(I2C2, 0xFFFF);
  I2C_ITConfig(I2C2, 0xFFFF, DISABLE);
  I2C_ITConfig(I2C2, I2C_IT_TX_EMPTY | I2C_IT_TX_ABRT | I2C_IT_STOP_DET | I2C_IT_RX_FULL, ENABLE);

  while(g_i2c_xfer_info.flag_complete == 0);

  I2C_Cmd(I2C2, DISABLE);

  return g_i2c_xfer_info.tx_abrt_source;
}

uint32_t eeprom_24c02_sequential_read(uint8_t addr, uint8_t* pbuf, uint32_t len)
{
  g_i2c_xfer_info.tx_buf = &addr;
  g_i2c_xfer_info.tx_len = 1;

  g_i2c_xfer_info.rx_buf = pbuf;
  g_i2c_xfer_info.rx_len = len;
  g_i2c_xfer_info.rx_cmd_len = len;

  g_i2c_xfer_info.flag_complete = 0;
  g_i2c_xfer_info.tx_abrt_source = 0x00;

  I2C_Cmd(I2C2, ENABLE);
  I2C_ClearITPendingBit(I2C2, 0xFFFF);
  I2C_ITConfig(I2C2, 0xFFFF, DISABLE);
  I2C_ITConfig(I2C2, I2C_IT_TX_EMPTY | I2C_IT_TX_ABRT | I2C_IT_STOP_DET | I2C_IT_RX_FULL, ENABLE);

  while(g_i2c_xfer_info.flag_complete == 0);

  I2C_Cmd(I2C2, DISABLE);

  return g_i2c_xfer_info.tx_abrt_source;
}

uint32_t eeprom_24c02_byte_write(uint8_t addr, uint8_t data)
{
  uint8_t tx_buf[2] = {addr, data};

  g_i2c_xfer_info.tx_buf = tx_buf;
  g_i2c_xfer_info.tx_len = 2;

  g_i2c_xfer_info.rx_buf = NULL;
  g_i2c_xfer_info.rx_len = 0;
  g_i2c_xfer_info.rx_cmd_len = 0;

  g_i2c_xfer_info.flag_complete = 0;
  g_i2c_xfer_info.tx_abrt_source = 0x00;

  I2C_Cmd(I2C2, ENABLE);
  I2C_ClearITPendingBit(I2C2, 0xFFFF);
  I2C_ITConfig(I2C2, 0xFFFF, DISABLE);
  I2C_ITConfig(I2C2, I2C_IT_TX_EMPTY | I2C_IT_TX_ABRT | I2C_IT_STOP_DET | I2C_IT_RX_FULL, ENABLE);

  while(g_i2c_xfer_info.flag_complete == 0);

  I2C_Cmd(I2C2, DISABLE);

  return g_i2c_xfer_info.tx_abrt_source;
}

static uint8_t pw_txbuf[9];
uint32_t eeprom_24c02_page_write(uint8_t addr, const uint8_t* pdata, uint8_t len)
{
  pw_txbuf[0] = addr;
  pw_txbuf[1] = pdata[0];
  pw_txbuf[2] = pdata[1];
  pw_txbuf[3] = pdata[2];
  pw_txbuf[4] = pdata[3];
  pw_txbuf[5] = pdata[4];
  pw_txbuf[6] = pdata[5];
  pw_txbuf[7] = pdata[6];
  pw_txbuf[8] = pdata[7];

  g_i2c_xfer_info.tx_buf = pw_txbuf;
  g_i2c_xfer_info.tx_len = len + 1;

  g_i2c_xfer_info.rx_buf = NULL;
  g_i2c_xfer_info.rx_len = 0;
  g_i2c_xfer_info.rx_cmd_len = 0;

  g_i2c_xfer_info.flag_complete = 0;
  g_i2c_xfer_info.tx_abrt_source = 0x00;

  I2C_Cmd(I2C2, ENABLE);
  I2C_ClearITPendingBit(I2C2, 0xFFFF);
  I2C_ITConfig(I2C2, 0xFFFF, DISABLE);
  I2C_ITConfig(I2C2, I2C_IT_TX_EMPTY | I2C_IT_TX_ABRT | I2C_IT_STOP_DET | I2C_IT_RX_FULL, ENABLE);

  while(g_i2c_xfer_info.flag_complete == 0);

  I2C_Cmd(I2C2, DISABLE);

  return g_i2c_xfer_info.tx_abrt_source;
}

uint32_t eeprom_24c02_probe(void)
{
  uint8_t tmpdata = 0;

  g_i2c_xfer_info.tx_buf = &tmpdata;
  g_i2c_xfer_info.tx_len = 1;

  g_i2c_xfer_info.rx_buf = NULL;
  g_i2c_xfer_info.rx_len = 0;
  g_i2c_xfer_info.rx_cmd_len = 0;

  g_i2c_xfer_info.flag_complete = 0;
  g_i2c_xfer_info.tx_abrt_source = 0x00;

  I2C_Cmd(I2C2, ENABLE);
  I2C_ClearITPendingBit(I2C2, 0xFFFF);
  I2C_ITConfig(I2C2, 0xFFFF, DISABLE);
  I2C_ITConfig(I2C2, I2C_IT_TX_EMPTY | I2C_IT_TX_ABRT | I2C_IT_STOP_DET | I2C_IT_RX_FULL, ENABLE);

  while(g_i2c_xfer_info.flag_complete == 0);

  I2C_Cmd(I2C2, DISABLE);

  return g_i2c_xfer_info.tx_abrt_source;
}


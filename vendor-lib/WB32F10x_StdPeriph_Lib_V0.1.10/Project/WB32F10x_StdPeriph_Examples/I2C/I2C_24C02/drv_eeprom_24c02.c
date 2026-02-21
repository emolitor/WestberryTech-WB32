/* Includes ------------------------------------------------------------------*/
#include "drv_eeprom_24c02.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void eeprom_24c02_init(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_BMX2 |RCC_APB2Periph_I2C2, ENABLE);

  /*
    PB10 (I2C2_SCL)
    PB11 (I2C2_SDA)
  */
  GPIO_Init(GPIOB, GPIO_Pin_10 |GPIO_Pin_11, GPIO_MODE_AF |GPIO_OTYPE_OD |GPIO_PUPD_UP |GPIO_SPEED_HIGH |GPIO_AF4);

  I2C_DeInit(I2C2);
  I2C_Init(I2C2, I2C_CON_SLAVE_DISABLE | I2C_CON_RESTART_EN | I2C_CON_SPEED_STANDARD | I2C_CON_MASTER_MODE | I2C_CON_BUS_CLEAR_FEATURE_CTRL);
  I2C_TargetAddressConfig(I2C2, 0xA0 >> 1);

  I2C2->SS_SCL_HCNT = 373;  /* tHIGH = (373 + FS_SPKLEN + 7) / 96MHz = 4us */
  I2C2->SS_SCL_LCNT = 451;  /* tLOW = (451 + 1) / 96MHz = 4.708us */
  I2C2->FS_SPKLEN = 4;      /* tSP = 4 / 96MHz = 41.67ns */
  I2C2->SDA_SETUP = 24;     /* tSU;DAT = 24 / 96MHz = 250ns */
  I2C2->SDA_HOLD = 29;      /* tHD;DAT = 29 / 96MHz = 302.08ns */

  I2C2->SCL_STUCK_AT_LOW_TIMEOUT = 960000;  /* I2C SCL Stuck at Low Timeout = 960000 / 96MHz = 10ms */
}

uint32_t eeprom_24c02_random_read(uint8_t addr, uint8_t* pdata)
{
  uint32_t abrt_source = 0;

  I2C_Cmd(I2C2, ENABLE);
  I2C_ClearITPendingBit(I2C2, 0xFFFF);

  I2C_WriteDataCmd(I2C2, addr);
  I2C_WriteDataCmd(I2C2, I2C_DATA_CMD_READ | I2C_DATA_CMD_STOP);

  while((I2C_GetFlagStatus(I2C2, I2C_FLAG_RFNE) == RESET) && (I2C_GetRawITStatus(I2C2, I2C_IT_TX_ABRT) == RESET) && (I2C_GetRawITStatus(I2C2, I2C_IT_SCL_STUCK_AT_LOW) == RESET));
  while(I2C_GetFlagStatus(I2C2, I2C_FLAG_MST_ACTIVITY) != RESET);
  if(I2C_GetRawITStatus(I2C2, I2C_IT_TX_ABRT) != RESET) {
    abrt_source = I2C_GetTxAbortSource(I2C2);
    I2C_ClearITPendingBit(I2C2, I2C_IT_TX_ABRT);
  }
  else if(I2C_GetRawITStatus(I2C2, I2C_IT_SCL_STUCK_AT_LOW) != RESET) {
    abrt_source = 0xFFFFFFFF;
    I2C_ClearITPendingBit(I2C2, I2C_IT_SCL_STUCK_AT_LOW);
  }
  else {
    *pdata = I2C_ReadData(I2C2);
  }

  I2C_Cmd(I2C2, DISABLE);

  return abrt_source;
}

uint32_t eeprom_24c02_sequential_read(uint8_t addr, uint8_t* pbuf, uint32_t len)
{
  uint32_t abrt_source = 0;

  I2C_Cmd(I2C2, ENABLE);
  I2C_ClearITPendingBit(I2C2, 0xFFFF);

  I2C_WriteDataCmd(I2C2, addr);
  while(len)
  {
    if(len == 1) {
      I2C_WriteDataCmd(I2C2, I2C_DATA_CMD_READ | I2C_DATA_CMD_STOP);
    }
    else {
      I2C_WriteDataCmd(I2C2, I2C_DATA_CMD_READ);
    }
    while((I2C_GetFlagStatus(I2C2, I2C_FLAG_RFNE) == RESET) && (I2C_GetRawITStatus(I2C2, I2C_IT_TX_ABRT) == RESET) && (I2C_GetRawITStatus(I2C2, I2C_IT_SCL_STUCK_AT_LOW) == RESET));
    if((I2C_GetRawITStatus(I2C2, I2C_IT_TX_ABRT) != RESET) || (I2C_GetRawITStatus(I2C2, I2C_IT_SCL_STUCK_AT_LOW) != RESET)) {
      break;
    }
    *pbuf = I2C_ReadData(I2C2);
    pbuf++;
    len--;
  }
  while((I2C_GetFlagStatus(I2C2, I2C_FLAG_TFE) == RESET) && (I2C_GetRawITStatus(I2C2, I2C_IT_TX_ABRT) == RESET) && (I2C_GetRawITStatus(I2C2, I2C_IT_SCL_STUCK_AT_LOW) == RESET));
  while(I2C_GetFlagStatus(I2C2, I2C_FLAG_MST_ACTIVITY) != RESET);
  if(I2C_GetRawITStatus(I2C2, I2C_IT_TX_ABRT) != RESET) {
    abrt_source = I2C_GetTxAbortSource(I2C2);
    I2C_ClearITPendingBit(I2C2, I2C_IT_TX_ABRT);
  }
  else if(I2C_GetRawITStatus(I2C2, I2C_IT_SCL_STUCK_AT_LOW) != RESET) {
    abrt_source = 0xFFFFFFFF;
    I2C_ClearITPendingBit(I2C2, I2C_IT_SCL_STUCK_AT_LOW);
  }

  I2C_Cmd(I2C2, DISABLE);

  return abrt_source;
}

uint32_t eeprom_24c02_byte_write(uint8_t addr, uint8_t data)
{
  uint32_t abrt_source = 0;

  I2C_Cmd(I2C2, ENABLE);
  I2C_ClearITPendingBit(I2C2, 0xFFFF);

  I2C_WriteDataCmd(I2C2, addr);
  I2C_WriteDataCmd(I2C2, data | I2C_DATA_CMD_STOP);

  while((I2C_GetFlagStatus(I2C2, I2C_FLAG_TFE) == RESET) && (I2C_GetRawITStatus(I2C2, I2C_IT_TX_ABRT) == RESET) && (I2C_GetRawITStatus(I2C2, I2C_IT_SCL_STUCK_AT_LOW) == RESET));
  while(I2C_GetFlagStatus(I2C2, I2C_FLAG_MST_ACTIVITY) != RESET);
  if(I2C_GetRawITStatus(I2C2, I2C_IT_TX_ABRT) != RESET) {
    abrt_source = I2C_GetTxAbortSource(I2C2);
    I2C_ClearITPendingBit(I2C2, I2C_IT_TX_ABRT);
  }
  else if(I2C_GetRawITStatus(I2C2, I2C_IT_SCL_STUCK_AT_LOW) != RESET) {
    abrt_source = 0xFFFFFFFF;
    I2C_ClearITPendingBit(I2C2, I2C_IT_SCL_STUCK_AT_LOW);
  }
  I2C_Cmd(I2C2, DISABLE);

  return abrt_source;
}

uint32_t eeprom_24c02_page_write(uint8_t addr, const uint8_t* pdata, uint8_t len)
{
  uint32_t abrt_source = 0;

  I2C_Cmd(I2C2, ENABLE);
  I2C_ClearITPendingBit(I2C2, 0xFFFF);

  I2C_WriteDataCmd(I2C2, addr);
  while(len)
  {
    while(I2C_GetFlagStatus(I2C2, I2C_FLAG_TFNF) == RESET);
    if((I2C_GetRawITStatus(I2C2, I2C_IT_TX_ABRT) != RESET) || (I2C_GetRawITStatus(I2C2, I2C_IT_SCL_STUCK_AT_LOW) != RESET)) {
      break;
    }
    if(len == 1) {
      I2C_WriteDataCmd(I2C2, *pdata | I2C_DATA_CMD_STOP);
    }
    else {
      I2C_WriteDataCmd(I2C2, *pdata);
    }
    pdata++;
    len--;
  }

  while((I2C_GetFlagStatus(I2C2, I2C_FLAG_TFE) == RESET) && (I2C_GetRawITStatus(I2C2, I2C_IT_TX_ABRT) == RESET) && (I2C_GetRawITStatus(I2C2, I2C_IT_SCL_STUCK_AT_LOW) == RESET));
  while(I2C_GetFlagStatus(I2C2, I2C_FLAG_MST_ACTIVITY) != RESET);
  if(I2C_GetRawITStatus(I2C2, I2C_IT_TX_ABRT) != RESET) {
    abrt_source = I2C_GetTxAbortSource(I2C2);
    I2C_ClearITPendingBit(I2C2, I2C_IT_TX_ABRT);
  }
  else if(I2C_GetRawITStatus(I2C2, I2C_IT_SCL_STUCK_AT_LOW) != RESET) {
    abrt_source = 0xFFFFFFFF;
    I2C_ClearITPendingBit(I2C2, I2C_IT_SCL_STUCK_AT_LOW);
  }

  I2C_Cmd(I2C2, DISABLE);

  return abrt_source;
}

uint32_t eeprom_24c02_probe(void)
{
  uint32_t abrt_source = 0;

  I2C_Cmd(I2C2, ENABLE);
  I2C_ClearITPendingBit(I2C2, 0xFFFF);

  I2C_WriteDataCmd(I2C2, I2C_DATA_CMD_STOP);

  while((I2C_GetFlagStatus(I2C2, I2C_FLAG_TFE) == RESET) && (I2C_GetRawITStatus(I2C2, I2C_IT_TX_ABRT) == RESET) && (I2C_GetRawITStatus(I2C2, I2C_IT_SCL_STUCK_AT_LOW) == RESET));
  while(I2C_GetFlagStatus(I2C2, I2C_FLAG_MST_ACTIVITY) != RESET);
  if(I2C_GetRawITStatus(I2C2, I2C_IT_TX_ABRT) != RESET) {
    abrt_source = I2C_GetTxAbortSource(I2C2);
    I2C_ClearITPendingBit(I2C2, I2C_IT_TX_ABRT);
  }
  else if(I2C_GetRawITStatus(I2C2, I2C_IT_SCL_STUCK_AT_LOW) != RESET) {
    abrt_source = 0xFFFFFFFF;
    I2C_ClearITPendingBit(I2C2, I2C_IT_SCL_STUCK_AT_LOW);
  }
  I2C_Cmd(I2C2, DISABLE);

  return abrt_source;
}


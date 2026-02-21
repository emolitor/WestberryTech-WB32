/* Includes ------------------------------------------------------------------*/
#include "drv_max1617a.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t flag_smbus_alert = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void max1617a_init(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_BMX2 |RCC_APB2Periph_I2C1, ENABLE);

  /*
    PB5 (I2C1_SMBAL)
    PB6 (I2C1_SCL)
    PB7 (I2C1_SDA)
  */
  GPIO_Init(GPIOB, GPIO_Pin_5 |GPIO_Pin_6 |GPIO_Pin_7, GPIO_MODE_AF |GPIO_OTYPE_OD |GPIO_PUPD_UP |GPIO_SPEED_HIGH |GPIO_AF4);

  I2C_DeInit(I2C1);
  I2C_Init(I2C1, I2C_CON_SLAVE_DISABLE | I2C_CON_RESTART_EN | I2C_CON_SPEED_STANDARD | I2C_CON_MASTER_MODE);
  I2C_TargetAddressConfig(I2C1, 0x30 >> 1);

  I2C1->SS_SCL_HCNT = 373;  /* tHIGH = (373 + FS_SPKLEN + 7) / 96MHz = 4us */
  I2C1->SS_SCL_LCNT = 451;  /* tLOW = (451 + 1) / 96MHz = 4.708us */
  I2C1->FS_SPKLEN = 4;      /* tSP = 4 / 96MHz = 41.67ns */
  I2C1->SDA_SETUP = 24;     /* tSU;DAT = 24 / 96MHz = 250ns */
  I2C1->SDA_HOLD = 29;      /* tHD;DAT = 29 / 96MHz = 302.08ns */

  I2C_ITConfig(I2C1, 0xFFFF, DISABLE);
  I2C_SMBus_ITConfig(I2C1, 0xFFFF, DISABLE);
  I2C_SMBus_ITConfig(I2C1, I2C_SMBUS_IT_SMBUS_ALERT_DET, ENABLE);
  I2C_Cmd(I2C1, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = I2C1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

// I2C1 Interrupt Routine
void I2C1_IRQHandler(void)
{
  if(I2C_SMBus_GetITStatus(I2C1, I2C_SMBUS_IT_SMBUS_ALERT_DET) != RESET)
  {
    flag_smbus_alert = 1;
    I2C_SMBus_ClearITPendingBit(I2C1, I2C_SMBUS_IT_SMBUS_ALERT_DET);
  }
}

uint32_t max1617a_smbus_read_byte(uint8_t command, uint8_t* pdata)
{
  uint32_t abrt_source = 0;

  I2C_ClearITPendingBit(I2C1, I2C_IT_TX_ABRT);

  I2C_WriteDataCmd(I2C1, command);
  I2C_WriteDataCmd(I2C1, I2C_DATA_CMD_READ | I2C_DATA_CMD_STOP);

  while((I2C_GetFlagStatus(I2C1, I2C_FLAG_RFNE) == RESET) && (I2C_GetRawITStatus(I2C1, I2C_IT_TX_ABRT) == RESET));
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_MST_ACTIVITY) != RESET);
  if(I2C_GetRawITStatus(I2C1, I2C_IT_TX_ABRT) != RESET) {
    abrt_source = I2C_GetTxAbortSource(I2C1);
    I2C_ClearITPendingBit(I2C1, I2C_IT_TX_ABRT);
  }
  else {
    *pdata = I2C_ReadData(I2C1);
  }

  return abrt_source;
}

uint32_t max1617a_smbus_write_byte(uint8_t command, uint8_t data)
{
  uint32_t abrt_source = 0;

  I2C_ClearITPendingBit(I2C1, I2C_IT_TX_ABRT);

  I2C_WriteDataCmd(I2C1, command);
  I2C_WriteDataCmd(I2C1, data | I2C_DATA_CMD_STOP);

  while((I2C_GetFlagStatus(I2C1, I2C_FLAG_TFE) == RESET) && (I2C_GetRawITStatus(I2C1, I2C_IT_TX_ABRT) == RESET));
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_MST_ACTIVITY) != RESET);
  if(I2C_GetRawITStatus(I2C1, I2C_IT_TX_ABRT) != RESET) {
    abrt_source = I2C_GetTxAbortSource(I2C1);
    I2C_ClearITPendingBit(I2C1, I2C_IT_TX_ABRT);
  }

  return abrt_source;
}

uint32_t max1617a_smbus_alert_response_address(uint8_t* pdata)
{
  uint32_t abrt_source = 0;
  uint32_t tmp_tar = 0;

  I2C_Cmd(I2C1, DISABLE);
  tmp_tar = I2C1->TAR;
  I2C1->TAR = 0x0C;   // Alert Response Address
  I2C_Cmd(I2C1, ENABLE);
  I2C_ClearITPendingBit(I2C1, I2C_IT_TX_ABRT);

  I2C_WriteDataCmd(I2C1, I2C_DATA_CMD_READ | I2C_DATA_CMD_STOP);

  while((I2C_GetFlagStatus(I2C1, I2C_FLAG_RFNE) == RESET) && (I2C_GetRawITStatus(I2C1, I2C_IT_TX_ABRT) == RESET));
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_MST_ACTIVITY) != RESET);
  if(I2C_GetRawITStatus(I2C1, I2C_IT_TX_ABRT) != RESET) {
    abrt_source = I2C_GetTxAbortSource(I2C1);
    I2C_ClearITPendingBit(I2C1, I2C_IT_TX_ABRT);
  }
  else {
    *pdata = I2C_ReadData(I2C1);
  }

  I2C_Cmd(I2C1, DISABLE);
  I2C1->TAR = tmp_tar;
  I2C_Cmd(I2C1, ENABLE);

  return abrt_source;
}

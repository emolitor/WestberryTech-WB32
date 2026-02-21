#include "drv_es8316.h"
#include <string.h>



/**
 * @brief  初始化es8316 I2C接口
 * @return 0 - 成功
 */
uint32_t es8316_init(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_BMX2 |RCC_APB2Periph_I2C1, ENABLE);

  /*
    PB8 (I2C1_SCL)
    PB9 (I2C1_SDA)
  */
  GPIO_Init(GPIOB, GPIO_Pin_8 |GPIO_Pin_9, GPIO_MODE_AF |GPIO_OTYPE_OD |GPIO_PUPD_UP |GPIO_SPEED_HIGH |GPIO_AF4);
  GPIO_DriveCurrentConfig(GPIOB, GPIO_Pin_8 |GPIO_Pin_9, 3);

  I2C_DeInit(I2C1);
  I2C_Init(I2C1, I2C_CON_SLAVE_DISABLE | I2C_CON_RESTART_EN | I2C_CON_SPEED_STANDARD | I2C_CON_MASTER_MODE);
  I2C_TargetAddressConfig(I2C1, ES8316_SLAVE_ADDRESS >> 1);

  I2C1->SS_SCL_HCNT = 373;  /* tHIGH = (373 + FS_SPKLEN + 7) / 96MHz = 4us */
  I2C1->SS_SCL_LCNT = 451;  /* tLOW = (451 + 1) / 96MHz = 4.708us */
  I2C1->FS_SPKLEN = 4;      /* tSP = 4 / 96MHz = 41.67ns */
  I2C1->SDA_SETUP = 24;     /* tSU;DAT = 24 / 96MHz = 250ns */
  I2C1->SDA_HOLD = 29;      /* tHD;DAT = 29 / 96MHz = 302.08ns */

  return es8316_probe();
}

/**
 * @brief  探测es8316是否存在
 * @return 0 - 成功
 */
uint32_t es8316_probe(void)
{
  uint32_t abrt_source = 0;

  I2C_Cmd(I2C1, ENABLE);
  I2C_ClearITPendingBit(I2C1, 0xFFFF);

  I2C_WriteDataCmd(I2C1, I2C_DATA_CMD_STOP);

  while((I2C_GetFlagStatus(I2C1, I2C_FLAG_TFE) == RESET) && (I2C_GetRawITStatus(I2C1, I2C_IT_TX_ABRT) == RESET));
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_MST_ACTIVITY) != RESET);
  if(I2C_GetRawITStatus(I2C1, I2C_IT_TX_ABRT) != RESET) {
    abrt_source = I2C_GetTxAbortSource(I2C1);
    I2C_ClearITPendingBit(I2C1, I2C_IT_TX_ABRT);
  }
  I2C_Cmd(I2C1, DISABLE);

  return abrt_source;
}

/**
 * @brief  写入es8316指定寄存器
 * @param  reg: 寄存器地址
 * @param  data: 要写入的值
 * @return 0 - 成功
 */
uint32_t es8316_write_reg(uint8_t reg, uint8_t data)
{
  uint32_t abrt_source = 0;

  I2C_Cmd(I2C1, ENABLE);
  I2C_ClearITPendingBit(I2C1, 0xFFFF);

  I2C_WriteDataCmd(I2C1, reg);
  I2C_WriteDataCmd(I2C1, data | I2C_DATA_CMD_STOP);

  while((I2C_GetFlagStatus(I2C1, I2C_FLAG_TFE) == RESET) && (I2C_GetRawITStatus(I2C1, I2C_IT_TX_ABRT) == RESET));
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_MST_ACTIVITY) != RESET);
  if(I2C_GetRawITStatus(I2C1, I2C_IT_TX_ABRT) != RESET) {
    abrt_source = I2C_GetTxAbortSource(I2C1);
    I2C_ClearITPendingBit(I2C1, I2C_IT_TX_ABRT);
  }
  I2C_Cmd(I2C1, DISABLE);

  return abrt_source;
}

/**
 * @brief  读取es8316指定寄存器
 * @param  reg: 寄存器地址
 * @param  pdata: 指向一个变量用于存放读出的值
 * @return 0 - 成功
 */
uint32_t es8316_read_reg(uint8_t reg, uint8_t* pdata)
{
  uint32_t abrt_source = 0;

  I2C_Cmd(I2C1, ENABLE);
  I2C_ClearITPendingBit(I2C1, 0xFFFF);

  I2C_WriteDataCmd(I2C1, reg);
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

  return abrt_source;
}


static void delay(volatile uint32_t n)
{
  while (n--);
}

void es8316_config_play_path(void)
{
  es8316_write_reg(0x00,0x3F); //IC Rst On
  delay(800000);  //Delay
  es8316_write_reg(0x00,0x00); //IC Normal
  es8316_write_reg(0x0C,0xFF); // Cal Time Set
  es8316_write_reg(0x02,0x09); //Clock Set
  es8316_write_reg(0x03,0x20); //adc Clock Set
  es8316_write_reg(0x06,0x11); //dac Clock Set
  es8316_write_reg(0x07,0x00); //dac Clock Set
  es8316_write_reg(0x08,0x00); //Bclk Set
  es8316_write_reg(0x09,0x04); //Cpclk Set
  es8316_write_reg(0x01,0x7F); //:System Clock Set
  es8316_write_reg(0x31,0x00); //dac Single Speed
  es8316_write_reg(0x33,0x00); //dacVolumeL set
  es8316_write_reg(0x34,0x00); //dacVolumeR set
  es8316_write_reg(0x0B,0x0C); //:dac IIS Mode  16-bit
  es8316_write_reg(0x10,0x11); //:dac set
  es8316_write_reg(0x0E,0x04); //:Lower Power Mode
  es8316_write_reg(0x0F,0x00); //:Lower Power Mode
  es8316_write_reg(0x2F,0x00); //:dac set
  es8316_write_reg(0x13,0x00); //:Hpmix Set
  es8316_write_reg(0x14,0x88); //:Hpmix Set
  es8316_write_reg(0x15,0x88); //:Hpmix Set
  es8316_write_reg(0x16,0xBB); //:Hpmix Set
  es8316_write_reg(0x1A,0x10); //:Cphp Set
  es8316_write_reg(0x1B,0x30); //:Cphp Set
  es8316_write_reg(0x19,0x02); //:Cphp Set
  es8316_write_reg(0x18,0x00); //:Cphp Set
  es8316_write_reg(0x1E,0x90); //CAL set
  es8316_write_reg(0x1F,0x90); //:CAL set
  es8316_write_reg(0x1C,0x0F); //:CAL set
  delay(8000);  //Delay
  es8316_write_reg(0x00,0xC0); //:Clk on
  delay(8000);  //Delay
  es8316_write_reg(0x17,0x66); //:dac output

}

void es8316_config_record_path(void)
{
  es8316_write_reg(0x00,0x3F);
  delay(800000);  //Delay
  es8316_write_reg(0x00,0x01);
  es8316_write_reg(0x22,0x30);
  es8316_write_reg(0x23,0x60);
  es8316_write_reg(0x24,0x01);
  es8316_write_reg(0x27,0x00);
  es8316_write_reg(0x0A,0x0C);    //16bit
  es8316_write_reg(0x0B,0x0C);
  es8316_write_reg(0x09,0x04);
  es8316_write_reg(0x02,0x09);
  es8316_write_reg(0x03,0x20);
  es8316_write_reg(0x04,0x11);
  es8316_write_reg(0x05,0x00);
  es8316_write_reg(0x06,0x11);
  es8316_write_reg(0x07,0x00);
  es8316_write_reg(0x08,0x00);
  es8316_write_reg(0x01,0x7F);
  es8316_write_reg(0x0C,0xFF);
  es8316_write_reg(0x0D,0x01);
  es8316_write_reg(0x0E,0x3F);
  es8316_write_reg(0x0F,0x10);
  es8316_write_reg(0x10,0x01);
  delay(80000);  //Delay
  es8316_write_reg(0x00,0xC1);
}


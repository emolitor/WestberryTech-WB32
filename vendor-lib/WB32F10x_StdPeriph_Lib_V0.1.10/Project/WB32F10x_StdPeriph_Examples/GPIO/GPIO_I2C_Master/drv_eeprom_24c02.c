/* Includes ------------------------------------------------------------------*/
#include "drv_iic_soft.h"
#include "drv_eeprom_24c02.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define EE_DEV_ADDR   0xA0

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void eeprom_24c02_init(void)
{
  IIC_Init();
}

uint32_t eeprom_24c02_random_read(uint8_t addr, uint8_t* pdata)
{
  IIC_Start();
  IIC_SendByte(EE_DEV_ADDR | IIC_WR);
  if (IIC_WaitAck())
    return 1;

  IIC_SendByte(addr);
  if (IIC_WaitAck())
    return 1;

  IIC_Start();
  IIC_SendByte(EE_DEV_ADDR | IIC_RD);
  if (IIC_WaitAck())
    return 1;

  *pdata = IIC_ReadByte();
  IIC_NAck();
  IIC_Stop();
  return 0;
}

uint32_t eeprom_24c02_sequential_read(uint8_t addr, uint8_t* pbuf, uint32_t len)
{
  IIC_Start();
  IIC_SendByte(EE_DEV_ADDR | IIC_WR);
  if (IIC_WaitAck())
    return 1;

  IIC_SendByte(addr);
  if (IIC_WaitAck())
    return 1;

  IIC_Start();
  IIC_SendByte(EE_DEV_ADDR | IIC_RD);
  if (IIC_WaitAck())
    return 1;

  while (len)
  {
    *pbuf = IIC_ReadByte();
    pbuf++;
    len--;
    if (len) IIC_Ack();
    else IIC_NAck();
  }
  IIC_Stop();
  return 0;
}

uint32_t eeprom_24c02_byte_write(uint8_t addr, uint8_t data)
{
  IIC_Start();
  IIC_SendByte(EE_DEV_ADDR | IIC_WR);
  if (IIC_WaitAck())
    return 1;

  IIC_SendByte(addr);
  if (IIC_WaitAck())
    return 1;

  IIC_SendByte(data);
  if (IIC_WaitAck())
    return 1;

  IIC_Stop();
  return 0;
}

uint32_t eeprom_24c02_page_write(uint8_t addr, const uint8_t* pdata, uint8_t len)
{
  uint8_t idx;

  IIC_Start();
  IIC_SendByte(EE_DEV_ADDR | IIC_WR);
  if (IIC_WaitAck())
    return 1;

  IIC_SendByte(addr);
  if (IIC_WaitAck())
    return 1;

  for (idx = 0; idx < len; idx++)
  {
    IIC_SendByte(pdata[idx]);
    if (IIC_WaitAck())
      return 1;
  }

  IIC_Stop();
  return 0;
}

uint32_t eeprom_24c02_probe(void)
{
  IIC_Start();
  IIC_SendByte(EE_DEV_ADDR | IIC_WR);
  if (IIC_WaitAck())
    return 1;

  IIC_Stop();
  return 0;
}


#include "drv_iic_soft.h"


#define IIC_SCL_H()   GPIO_SetBits(GPIOB, GPIO_Pin_10)      /* SCL = 1 */
#define IIC_SCL_L()   GPIO_ResetBits(GPIOB, GPIO_Pin_10)    /* SCL = 0 */

#define IIC_SDA_H()     GPIO_SetBits(GPIOB, GPIO_Pin_11)    /* SDA = 1 */
#define IIC_SDA_L()     GPIO_ResetBits(GPIOB, GPIO_Pin_11)  /* SDA = 0 */
#define IIC_SDA_READ()  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)     /* 读取SDA线状态 */


/**
  * @name    IIC_Init
  * @brief   配置模拟IIC的GPIO
  * @param   无
  * @retval  无
  */
void IIC_Init(void)
{
  /* 打开GPIO时钟 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOB, ENABLE);

  /* 配置为开漏输出，必须要接上拉电阻。在开漏模式时，对输入数据寄存器的读访问可得到I/O状态 */
  GPIO_Init(GPIOB, GPIO_Pin_10 |GPIO_Pin_11, GPIO_MODE_OUT |GPIO_OTYPE_OD);

  /* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
  IIC_Stop();
}

static void iic_delay(void)
{ // #CONFIG#
  volatile uint32_t n = 100;
  while(n--);
}

/**
  * @name    IIC_Start
  * @brief   产生IIC起始信号
  * @param   无
  * @retval  无
  */
void IIC_Start(void)
{
  IIC_SDA_H();    /* SDA拉高 */
  IIC_SCL_H();    /* SCL拉高 */
  iic_delay();
  IIC_SDA_L();    /* SDA拉低 */
  iic_delay();
}

/**
  * @name    IIC_Stop
  * @brief   产生IIC停止信号
  * @param   无
  * @retval  无
  */
void IIC_Stop(void)
{
  IIC_SDA_L();    /* SDA拉低 */
  IIC_SCL_H();    /* SCL拉高 */
  iic_delay();
  IIC_SDA_H();    /* SDA拉高 */
  iic_delay();
}

/**
  * @name    IIC_Ack
  * @brief   产生ACK应答
  * @param   无
  * @retval  无
  */
void IIC_Ack(void)
{
  IIC_SDA_L();    /* SDA拉低 */
  IIC_SCL_L();    /* SCL拉低 */
  iic_delay();
  IIC_SCL_H();    /* SCL拉高 */
  iic_delay();
  IIC_SCL_L();    /* SCL拉低 */
  iic_delay();
  IIC_SDA_H();    /* 释放SDA总线 */
}

/**
  * @name    IIC_NAck
  * @brief   不产生ACK应答
  * @param   无
  * @retval  无
  */
void IIC_NAck(void)
{
  IIC_SDA_H();    /* SDA拉高 */
  IIC_SCL_L();    /* SCL拉低 */
  iic_delay();
  IIC_SCL_H();    /* SCL拉高 */
  iic_delay();
  IIC_SCL_L();    /* SCL拉低 */
  iic_delay();
}

/**
  * @name    IIC_WaitAck
  * @brief   等待应答信号到来
  * @param   无
  * @retval  1: 接收应答失败
  *          0: 接收应答成功
  */
uint8_t IIC_WaitAck(void)
{
  IIC_SDA_H();    /* 释放SDA总线 */
  iic_delay();
  IIC_SCL_H();    /* SCL拉高 */
  iic_delay();
  
  if(IIC_SDA_READ())
  {
    IIC_SCL_L();    /* SCL拉低 */
    iic_delay();
    IIC_Stop();
    return 1;   /* 接收应答失败 */
  }
  else
  {
    IIC_SCL_L();    /* SCL拉低 */
    iic_delay();
    return 0;   /*接收应答成功*/
  }
}

/**
  * @name    IIC_SendByte
  * @brief   发送一个字节数据
  * @param   data: 要发送的字节数据
  * @retval  无
  */
void IIC_SendByte(uint8_t data)
{
  uint8_t bit_of_byte = 0x80;
  
  while (bit_of_byte)
  {
    IIC_SCL_L();    /* SCL拉低 */
    if(data & bit_of_byte) {
      IIC_SDA_H();
    }
    else {
      IIC_SDA_L();
    }
    bit_of_byte >>= 1;
    iic_delay();
    IIC_SCL_H();    /* SCL拉高 */
    iic_delay();
  }
  IIC_SCL_L();    /* SCL拉低 */
}


/**
  * @name    IIC_ReadByte
  * @brief   接收一字节数据
  * @param   无
  * @retval  读到的数据
  */
uint8_t IIC_ReadByte(void)
{
  uint8_t bit_of_byte = 0x80;
  uint8_t receive_byte = 0;
  
  while (bit_of_byte)
  {
    IIC_SCL_L();    /* SCL拉低 */
    iic_delay();
    IIC_SCL_H();    /* SCL拉高 */
    iic_delay();
    if(IIC_SDA_READ()) {
      receive_byte |= bit_of_byte;
    }
    bit_of_byte >>= 1;
  }
  
  IIC_SCL_L();    /* SCL拉低 */
  
  return receive_byte;
}

/**
  * @name    IIC_CheckDevice
  * @brief   检测I2C总线设备，CPU发送设备地址，通过是否有ACK判断设备是否存在
  * @param   无
  * @retval  0: 表示检测到相应设备
  *          1: 表示未检测到
  */
uint8_t IIC_CheckDevice(uint8_t address)
{
  uint8_t ack;
  
  IIC_Start();
  IIC_SendByte(address | IIC_WR);
  ack = IIC_WaitAck();
  IIC_Stop();
  
  return ack;
}




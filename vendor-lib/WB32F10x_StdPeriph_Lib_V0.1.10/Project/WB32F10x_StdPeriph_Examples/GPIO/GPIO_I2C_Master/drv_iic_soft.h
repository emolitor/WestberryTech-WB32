#ifndef __DRV_IIC_SOFT_H__
#define __DRV_IIC_SOFT_H__

/* includes. */
#include "wb32f10x.h"



void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Ack(void);
void IIC_NAck(void);
uint8_t IIC_WaitAck(void);
void IIC_SendByte(uint8_t data);
uint8_t IIC_ReadByte(void);
uint8_t IIC_CheckDevice(uint8_t address);

#define IIC_WR  0   /* 写控制bit */
#define IIC_RD  1   /* 读控制bit */


#endif /* __DRV_IIC_SOFT_H__ */

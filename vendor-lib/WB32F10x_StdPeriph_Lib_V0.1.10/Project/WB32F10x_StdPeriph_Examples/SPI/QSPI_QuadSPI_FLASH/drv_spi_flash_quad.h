/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_SPI_FLASH_H__
#define __DRV_SPI_FLASH_H__

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void SpiFlash_Init(void);
uint32_t SpiFlash_ReadIdentification(void);
uint16_t SpiFlash_ReadMidDid(void);
uint8_t SpiFlash_ReadStatusReg1(void);
uint8_t SpiFlash_ReadStatusReg2(void);
uint8_t SpiFlash_ReadStatusReg3(void);
void SpiFlash_WaitReady(void);
void SpiFlash_WriteEnable(void);
void SpiFlash_WriteStatusReg(uint16_t value);
void SpiFlash_WriteStatusReg1(uint8_t value);
void SpiFlash_WriteStatusReg2(uint8_t value);
void SpiFlash_ReadData(uint32_t address, uint8_t* buffer, uint32_t length);
void SpiFlash_ChipErase(void);
void SpiFlash_SectorErase(uint32_t address);
void SpiFlash_PageProgram(uint32_t address, const uint8_t* buffer, uint16_t length);

void SpiFlash_WriteBuffer(uint32_t address, const uint8_t* buffer, uint32_t length);

uint16_t SpiFlash_DualReadMidDid(void);
void SpiFlash_DualFastRead(uint32_t address, uint8_t* buffer, uint32_t length);

uint16_t SpiFlash_QuadReadMidDid(void);
void SpiFlash_QuadFastRead(uint32_t address, uint8_t* buffer, uint32_t length);
void SpiFlash_QuadPageProgram(uint32_t address, const uint8_t* buffer, uint16_t length);

void SpiFlash_QuadWriteBuffer(uint32_t address, const uint8_t* buffer, uint32_t length);

#endif /* __DRV_SPI_FLASH_H__ */

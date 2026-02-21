/**
 * @file    wb32f10x_crc.h
 * @author  Westberry Application Team
 * @version V0.1.10
 * @date    05-January-2023
 * @brief   This file contains all the functions prototypes for the CRC firmware
 *          library.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WB32F10x_CRC_H
#define __WB32F10x_CRC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/** @addtogroup WB32F10x_StdPeriph_Driver
  * @{
  */

/** @addtogroup CRC
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

uint8_t CRC8_ComputeBytes(const uint8_t *ptr_data, uint32_t data_len);
uint8_t CRC8_ComputeBytes_Update(uint8_t seed, const uint8_t *ptr_data, uint32_t data_len);

uint16_t CRC16_CCITT_ComputeBytes(const uint8_t *ptr_data, uint32_t data_len);
uint16_t CRC16_CCITT_ComputeBytes_Update(uint16_t seed, const uint8_t *ptr_data, uint32_t data_len);

uint16_t CRC16_Modbus_ComputeBytes(const uint8_t *ptr_data, uint32_t data_len);
uint16_t CRC16_Modbus_ComputeBytes_Update(uint16_t seed, const uint8_t *ptr_data, uint32_t data_len);

uint16_t CRC16_IBM_ComputeBytes(const uint8_t *ptr_data, uint32_t data_len);
#define  CRC16_IBM_ComputeBytes_Update CRC16_Modbus_ComputeBytes_Update

uint32_t CRC32_ComputeBytes(const uint8_t *ptr_data, uint32_t data_len);
uint32_t CRC32_ComputeBytes_Update(uint32_t seed, const uint8_t *ptr_data, uint32_t data_len);

uint32_t CRC32_MPEG2_ComputeBytes(const uint8_t *ptr_data, uint32_t data_len);
uint32_t CRC32_MPEG2_ComputeBytes_Update(uint32_t seed, const uint8_t *ptr_data, uint32_t data_len);

uint16_t CRC16_CCITT_ComputeHalfWords(const uint16_t *ptr_data, uint32_t data_len);
uint16_t CRC16_CCITT_ComputeHalfWords_Update(uint16_t seed, const uint16_t *ptr_data, uint32_t data_len);

uint16_t CRC16_Modbus_ComputeHalfWords(const uint16_t *ptr_data, uint32_t data_len);
uint16_t CRC16_Modbus_ComputeHalfWords_Update(uint16_t seed, const uint16_t *ptr_data, uint32_t data_len);

uint16_t CRC16_IBM_ComputeHalfWords(const uint16_t *ptr_data, uint32_t data_len);
#define  CRC16_IBM_ComputeHalfWords_Update CRC16_Modbus_ComputeHalfWords_Update

uint32_t CRC32_ComputeWords(const uint32_t *ptr_data, uint32_t data_len);
uint32_t CRC32_ComputeWords_Update(uint32_t seed, const uint32_t *ptr_data, uint32_t data_len);

uint32_t CRC32_MPEG2_ComputeWords(const uint32_t *ptr_data, uint32_t data_len);
uint32_t CRC32_MPEG2_ComputeWords_Update(uint32_t seed, const uint32_t *ptr_data, uint32_t data_len);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __WB32F10x_CRC_H */


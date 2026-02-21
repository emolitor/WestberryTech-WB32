/**
 * @file    wb32f10x_crc.c
 * @author  Westberry Application Team
 * @version V0.1.10
 * @date    05-January-2023
 * @brief   This file provides all the CRC firmware functions.
 */

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x_crc.h"


/** @addtogroup WB32F10x_StdPeriph_Driver
  * @{
  */

/** @defgroup CRC
  * @brief CRC driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup CRC_Private_Functions
  * @{
  */

/**
 * @brief  Computes the 8-bit CRC of a given buffer of byte(8-bit) with default seed.
 * @param  ptr_data: Pointer to the buffer containing the data to be computed
 * @param  data_len: Length of the buffer to be computed
 * @return 8-bit CRC
 */
uint8_t CRC8_ComputeBytes(const uint8_t *ptr_data, uint32_t data_len)
{
  CRC->MODE = CRC_MODE_CRC_POLY_CRC8 | CRC_MODE_SEED_SET;

  while(data_len--)
  {
    CRC->WR_DATA_BYTE = *ptr_data++;
  }
  __NOP();
  __NOP();
  return CRC->SUM;
}


/**
 * @brief  Computes the 8-bit CRC of a given buffer of byte(8-bit) with user-defined seed.
 * @param  seed:     User-defined seed
 * @param  ptr_data: Pointer to the buffer containing the data to be computed
 * @param  data_len: Length of the buffer to be computed
 * @return 8-bit CRC
 */
uint8_t CRC8_ComputeBytes_Update(uint8_t seed, const uint8_t *ptr_data, uint32_t data_len)
{
  CRC->SEED = seed;
  CRC->MODE = CRC_MODE_CRC_POLY_CRC8 | CRC_MODE_SEED_OP | CRC_MODE_SEED_SET;

  while(data_len--)
  {
    CRC->WR_DATA_BYTE = *ptr_data++;
  }
  __NOP();
  __NOP();
  return CRC->SUM;
}


/**
 * @brief  Computes the 16-bit CRC-CCITT of a given buffer of byte(8-bit) with default seed.
 * @param  ptr_data: Pointer to the buffer containing the data to be computed
 * @param  data_len: Length of the buffer to be computed
 * @return 16-bit CRC-CCITT
 */
uint16_t CRC16_CCITT_ComputeBytes(const uint8_t *ptr_data, uint32_t data_len)
{
  CRC->MODE = CRC_MODE_CRC_POLY_CCITT | CRC_MODE_SEED_SET;

  while(data_len--)
  {
    CRC->WR_DATA_BYTE = *ptr_data++;
  }
  __NOP();
  __NOP();
  return CRC->SUM;
}


/**
 * @brief  Computes the 16-bit CRC-CCITT of a given buffer of byte(8-bit) with user-defined seed.
 * @param  seed:     User-defined seed
 * @param  ptr_data: Pointer to the buffer containing the data to be computed
 * @param  data_len: Length of the buffer to be computed
 * @return 16-bit CRC-CCITT
 */
uint16_t CRC16_CCITT_ComputeBytes_Update(uint16_t seed, const uint8_t *ptr_data, uint32_t data_len)
{
  CRC->SEED = seed;
  CRC->MODE = CRC_MODE_CRC_POLY_CCITT | CRC_MODE_SEED_OP | CRC_MODE_SEED_SET;

  while(data_len--)
  {
    CRC->WR_DATA_BYTE = *ptr_data++;
  }
  __NOP();
  __NOP();
  return CRC->SUM;
}


/**
 * @brief  Computes the 16-bit CRC(Modbus) of a given buffer of byte(8-bit) with default seed.
 * @param  ptr_data: Pointer to the buffer containing the data to be computed
 * @param  data_len: Length of the buffer to be computed
 * @return 16-bit CRC(Modbus)
 */
uint16_t CRC16_Modbus_ComputeBytes(const uint8_t *ptr_data, uint32_t data_len)
{
  CRC->SEED = 0xFFFF;
  CRC->MODE = CRC_MODE_CRC_POLY_CRC16 | CRC_MODE_BIT_RVS_WR | CRC_MODE_BIT_RVS_SUM | CRC_MODE_SEED_OP | CRC_MODE_SEED_SET;

  while(data_len--)
  {
    CRC->WR_DATA_BYTE = *ptr_data++;
  }
  __NOP();
  __NOP();
  return CRC->SUM;
}


/**
 * @brief  Computes the 16-bit CRC(Modbus) of a given buffer of byte(8-bit) with user-defined seed.
 * @param  seed:     User-defined seed
 * @param  ptr_data: Pointer to the buffer containing the data to be computed
 * @param  data_len: Length of the buffer to be computed
 * @return 16-bit CRC(Modbus)
 */
uint16_t CRC16_Modbus_ComputeBytes_Update(uint16_t seed, const uint8_t *ptr_data, uint32_t data_len)
{
  CRC->SEED = __RBIT(seed) >> 16;
  CRC->MODE = CRC_MODE_CRC_POLY_CRC16 | CRC_MODE_BIT_RVS_WR | CRC_MODE_BIT_RVS_SUM | CRC_MODE_SEED_OP | CRC_MODE_SEED_SET;

  while(data_len--)
  {
    CRC->WR_DATA_BYTE = *ptr_data++;
  }
  __NOP();
  __NOP();
  return CRC->SUM;
}


/**
 * @brief  Computes the 16-bit CRC(IBM) of a given buffer of byte(8-bit) with default seed.
 * @param  ptr_data: Pointer to the buffer containing the data to be computed
 * @param  data_len: Length of the buffer to be computed
 * @return 16-bit CRC(IBM)
 */
uint16_t CRC16_IBM_ComputeBytes(const uint8_t *ptr_data, uint32_t data_len)
{
  CRC->MODE = CRC_MODE_CRC_POLY_CRC16 | CRC_MODE_BIT_RVS_WR | CRC_MODE_BIT_RVS_SUM | CRC_MODE_SEED_SET;

  while(data_len--)
  {
    CRC->WR_DATA_BYTE = *ptr_data++;
  }
  __NOP();
  __NOP();
  return CRC->SUM;
}


/**
 * @brief  Computes the 32-bit CRC of a given buffer of byte(8-bit) with default seed.
 * @param  ptr_data: Pointer to the buffer containing the data to be computed
 * @param  data_len: Length of the buffer to be computed
 * @return 32-bit CRC
 */
uint32_t CRC32_ComputeBytes(const uint8_t *ptr_data, uint32_t data_len)
{
  CRC->MODE = CRC_MODE_CRC_POLY_CRC32 | CRC_MODE_BIT_RVS_WR
        | CRC_MODE_BIT_RVS_SUM | CRC_MODE_CMPL_SUM | CRC_MODE_SEED_SET;

  while(data_len--)
  {
    CRC->WR_DATA_BYTE = *ptr_data++;
  }
  __NOP();
  __NOP();
  return CRC->SUM;
}


/**
 * @brief  Computes the 32-bit CRC of a given buffer of byte(8-bit) with user-defined seed.
 * @param  seed:     User-defined seed
 * @param  ptr_data: Pointer to the buffer containing the data to be computed
 * @param  data_len: Length of the buffer to be computed
 * @return 32-bit CRC
 */
uint32_t CRC32_ComputeBytes_Update(uint32_t seed, const uint8_t *ptr_data, uint32_t data_len)
{
  CRC->SEED = __RBIT(~seed);
  CRC->MODE = CRC_MODE_CRC_POLY_CRC32 | CRC_MODE_BIT_RVS_WR
        | CRC_MODE_BIT_RVS_SUM | CRC_MODE_CMPL_SUM | CRC_MODE_SEED_OP | CRC_MODE_SEED_SET;

  while(data_len--)
  {
    CRC->WR_DATA_BYTE = *ptr_data++;
  }
  __NOP();
  __NOP();
  return CRC->SUM;
}


/**
 * @brief  Computes the 32-bit CRC(MPEG2) of a given buffer of byte(8-bit) with default seed.
 * @param  ptr_data: Pointer to the buffer containing the data to be computed
 * @param  data_len: Length of the buffer to be computed
 * @return 32-bit CRC(MPEG2)
 */
uint32_t CRC32_MPEG2_ComputeBytes(const uint8_t *ptr_data, uint32_t data_len)
{
  CRC->MODE = CRC_MODE_CRC_POLY_CRC32 | CRC_MODE_SEED_SET;

  while(data_len--)
  {
    CRC->WR_DATA_BYTE = *ptr_data++;
  }
  __NOP();
  __NOP();
  return CRC->SUM;
}


/**
 * @brief  Computes the 32-bit CRC(MPEG2) of a given buffer of byte(8-bit) with user-defined seed.
 * @param  seed:     User-defined seed
 * @param  ptr_data: Pointer to the buffer containing the data to be computed
 * @param  data_len: Length of the buffer to be computed
 * @return 32-bit CRC(MPEG2)
 */
uint32_t CRC32_MPEG2_ComputeBytes_Update(uint32_t seed, const uint8_t *ptr_data, uint32_t data_len)
{
  CRC->SEED = seed;
  CRC->MODE = CRC_MODE_CRC_POLY_CRC32 | CRC_MODE_SEED_OP | CRC_MODE_SEED_SET;

  while(data_len--)
  {
    CRC->WR_DATA_BYTE = *ptr_data++;
  }
  __NOP();
  __NOP();
  return CRC->SUM;
}


/**
 * @brief  Computes the 16-bit CRC-CCITT of a given buffer of halfword(16-bit) with default seed.
 * @param  ptr_data: Pointer to the buffer containing the data to be computed
 * @param  data_len: Length of the buffer to be computed
 * @return 16-bit CRC-CCITT
 */
uint16_t CRC16_CCITT_ComputeHalfWords(const uint16_t *ptr_data, uint32_t data_len)
{
  CRC->MODE = CRC_MODE_CRC_POLY_CCITT | CRC_MODE_SEED_SET;

  while(data_len--)
  {
    CRC->WR_DATA_HALF_WORD = *ptr_data++;
  }
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  return CRC->SUM;
}


/**
 * @brief  Computes the 16-bit CRC-CCITT of a given buffer of halfword(16-bit) with user-defined seed.
 * @param  seed:     User-defined seed
 * @param  ptr_data: Pointer to the buffer containing the data to be computed
 * @param  data_len: Length of the buffer to be computed
 * @return 16-bit CRC-CCITT
 */
uint16_t CRC16_CCITT_ComputeHalfWords_Update(uint16_t seed, const uint16_t *ptr_data, uint32_t data_len)
{
  CRC->SEED = seed;
  CRC->MODE = CRC_MODE_CRC_POLY_CCITT | CRC_MODE_SEED_OP | CRC_MODE_SEED_SET;

  while(data_len--)
  {
    CRC->WR_DATA_HALF_WORD = *ptr_data++;
  }
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  return CRC->SUM;
}


/**
 * @brief  Computes the 16-bit CRC(Modbus) of a given buffer of halfword(16-bit) with default seed.
 * @param  ptr_data: Pointer to the buffer containing the data to be computed
 * @param  data_len: Length of the buffer to be computed
 * @return 16-bit CRC(Modbus)
 */
uint16_t CRC16_Modbus_ComputeHalfWords(const uint16_t *ptr_data, uint32_t data_len)
{
  CRC->SEED = 0xFFFF;
  CRC->MODE = CRC_MODE_CRC_POLY_CRC16 | CRC_MODE_BIT_RVS_WR | CRC_MODE_BIT_RVS_SUM | CRC_MODE_SEED_OP | CRC_MODE_SEED_SET;

  while(data_len--)
  {
    CRC->WR_DATA_HALF_WORD = *ptr_data++;
  }
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  return CRC->SUM;
}


/**
 * @brief  Computes the 16-bit CRC(Modbus) of a given buffer of halfword(16-bit) with user-defined seed.
 * @param  seed:     User-defined seed
 * @param  ptr_data: Pointer to the buffer containing the data to be computed
 * @param  data_len: Length of the buffer to be computed
 * @return 16-bit CRC(Modbus)
 */
uint16_t CRC16_Modbus_ComputeHalfWords_Update(uint16_t seed, const uint16_t *ptr_data, uint32_t data_len)
{
  CRC->SEED = __RBIT(seed) >> 16;
  CRC->MODE = CRC_MODE_CRC_POLY_CRC16 | CRC_MODE_BIT_RVS_WR | CRC_MODE_BIT_RVS_SUM | CRC_MODE_SEED_OP | CRC_MODE_SEED_SET;

  while(data_len--)
  {
    CRC->WR_DATA_HALF_WORD = *ptr_data++;
  }
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  return CRC->SUM;
}


/**
 * @brief  Computes the 16-bit CRC(IBM) of a given buffer of halfword(16-bit) with default seed.
 * @param  ptr_data: Pointer to the buffer containing the data to be computed
 * @param  data_len: Length of the buffer to be computed
 * @return 16-bit CRC(IBM)
 */
uint16_t CRC16_IBM_ComputeHalfWords(const uint16_t *ptr_data, uint32_t data_len)
{
  CRC->MODE = CRC_MODE_CRC_POLY_CRC16 | CRC_MODE_BIT_RVS_WR | CRC_MODE_BIT_RVS_SUM | CRC_MODE_SEED_SET;

  while(data_len--)
  {
    CRC->WR_DATA_HALF_WORD = *ptr_data++;
  }
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  return CRC->SUM;
}


/**
 * @brief  Computes the 32-bit CRC of a given buffer of word(32-bit) with default seed.
 * @param  ptr_data: Pointer to the buffer containing the data to be computed
 * @param  data_len: Length of the buffer to be computed
 * @return 32-bit CRC
 */
uint32_t CRC32_ComputeWords(const uint32_t *ptr_data, uint32_t data_len)
{
  CRC->MODE = CRC_MODE_CRC_POLY_CRC32 | CRC_MODE_BIT_RVS_WR
        | CRC_MODE_BIT_RVS_SUM | CRC_MODE_CMPL_SUM | CRC_MODE_SEED_SET;

  while(data_len--)
  {
    CRC->WR_DATA_WORD = *ptr_data++;
  }
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  return CRC->SUM;
}


/**
 * @brief  Computes the 32-bit CRC of a given buffer of word(32-bit) with user-defined seed.
 * @param  seed:     User-defined seed
 * @param  ptr_data: Pointer to the buffer containing the data to be computed
 * @param  data_len: Length of the buffer to be computed
 * @return 32-bit CRC
 */
uint32_t CRC32_ComputeWords_Update(uint32_t seed, const uint32_t *ptr_data, uint32_t data_len)
{
  CRC->SEED = __RBIT(~seed);
  CRC->MODE = CRC_MODE_CRC_POLY_CRC32 | CRC_MODE_BIT_RVS_WR
        | CRC_MODE_BIT_RVS_SUM | CRC_MODE_CMPL_SUM | CRC_MODE_SEED_OP | CRC_MODE_SEED_SET;

  while(data_len--)
  {
    CRC->WR_DATA_WORD = *ptr_data++;
  }
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  return CRC->SUM;
}


/**
 * @brief  Computes the 32-bit CRC(MPEG2) of a given buffer of word(32-bit) with default seed.
 * @param  ptr_data: Pointer to the buffer containing the data to be computed
 * @param  data_len: Length of the buffer to be computed
 * @return 32-bit CRC(MPEG2)
 */
uint32_t CRC32_MPEG2_ComputeWords(const uint32_t *ptr_data, uint32_t data_len)
{
  CRC->MODE = CRC_MODE_CRC_POLY_CRC32 | CRC_MODE_SEED_SET;

  while(data_len--)
  {
    CRC->WR_DATA_WORD = *ptr_data++;
  }
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  return CRC->SUM;
}


/**
 * @brief  Computes the 32-bit CRC(MPEG2) of a given buffer of word(32-bit) with user-defined seed.
 * @param  seed:     User-defined seed
 * @param  ptr_data: Pointer to the buffer containing the data to be computed
 * @param  data_len: Length of the buffer to be computed
 * @return 32-bit CRC(MPEG2)
 */
uint32_t CRC32_MPEG2_ComputeWords_Update(uint32_t seed, const uint32_t *ptr_data, uint32_t data_len)
{
  CRC->SEED = seed;
  CRC->MODE = CRC_MODE_CRC_POLY_CRC32 | CRC_MODE_SEED_OP | CRC_MODE_SEED_SET;

  while(data_len--)
  {
    CRC->WR_DATA_WORD = *ptr_data++;
  }
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  return CRC->SUM;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

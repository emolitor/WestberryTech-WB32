/* Includes ------------------------------------------------------------------*/
#include "compute.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const uint8_t bytes[36] = {0x76,0x15,0xE2,0xAE,0x1E,0x03,0xA5,0xFD,0xAE,0x1F,0x31,0xFC,0x06,0x56,0x0D,0x2A,0x7B,0x3C,0xCB,0xDD,0xD8,0x00,0x2F,0xDA,0xA2,0xA0,0x94,0x23,0xA1,0x59,0x8C,0x10,0x9B,0x62,0xC7,0xA1};
const uint32_t bytes_div_point = 15;
const uint8_t  bytes_crc8_desired_result = 0x0C;
const uint16_t bytes_crc16_ccitt_desired_result = 0x5DC4;
const uint16_t bytes_crc16_modbus_desired_result = 0x89E9;
const uint16_t bytes_crc16_ibm_desired_result = 0x99A9;
const uint32_t bytes_crc32_desired_result = 0x9B3730F9;
const uint32_t bytes_crc32_mpeg2_desired_result = 0x1D0DF5A4;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Verify CRC computes the byte array
 * @param  None
 * @return 0 - Pass
 *         Others - Failed
 */
uint8_t computeBytes(void)
{
  /************* Verify CRC8 ***************/
  result1 = CRC8_ComputeBytes(bytes, _countof(bytes));
  result2 = CRC8_ComputeBytes_Update(0x00, bytes, _countof(bytes));
  if(result1 != result2)
    return 0x11;    // Verify failed

  if(result1 != bytes_crc8_desired_result)
    return 0x12;    // Verify failed

  result3 = CRC8_ComputeBytes_Update(0x00, bytes, bytes_div_point);
  result4 = CRC8_ComputeBytes_Update(result3, bytes + bytes_div_point, _countof(bytes) - bytes_div_point);
  if(result4 != bytes_crc8_desired_result)
    return 0x13;    // Verify failed



  /************* Verify CRC16-CCITT ***************/
  result1 = CRC16_CCITT_ComputeBytes(bytes, _countof(bytes));
  result2 = CRC16_CCITT_ComputeBytes_Update(0xFFFF, bytes, _countof(bytes));
  if(result1 != result2)
    return 0x14;    // Verify failed

  if(result1 != bytes_crc16_ccitt_desired_result)
    return 0x15;    // Verify failed

  result3 = CRC16_CCITT_ComputeBytes_Update(0xFFFF, bytes, bytes_div_point);
  result4 = CRC16_CCITT_ComputeBytes_Update(result3, bytes + bytes_div_point, _countof(bytes) - bytes_div_point);
  if(result4 != bytes_crc16_ccitt_desired_result)
    return 0x16;    // Verify failed



  /************* Verify CRC16-Modbus ***************/
  result1 = CRC16_Modbus_ComputeBytes(bytes, _countof(bytes));
  result2 = CRC16_Modbus_ComputeBytes_Update(0xFFFF, bytes, _countof(bytes));
  if(result1 != result2)
    return 0x17;    // Verify failed

  if(result1 != bytes_crc16_modbus_desired_result)
    return 0x18;    // Verify failed

  result3 = CRC16_Modbus_ComputeBytes_Update(0xFFFF, bytes, bytes_div_point);
  result4 = CRC16_Modbus_ComputeBytes_Update(result3, bytes + bytes_div_point, _countof(bytes) - bytes_div_point);
  if(result4 != bytes_crc16_modbus_desired_result)
    return 0x19;    // Verify failed



  /************* Verify CRC16-IBM ***************/
  result1 = CRC16_IBM_ComputeBytes(bytes, _countof(bytes));
  result2 = CRC16_IBM_ComputeBytes_Update(0x0000, bytes, _countof(bytes));
  if(result1 != result2)
    return 0x1A;    // Verify failed

  if(result1 != bytes_crc16_ibm_desired_result)
    return 0x1B;    // Verify failed

  result3 = CRC16_IBM_ComputeBytes_Update(0x0000, bytes, bytes_div_point);
  result4 = CRC16_IBM_ComputeBytes_Update(result3, bytes + bytes_div_point, _countof(bytes) - bytes_div_point);
  if(result4 != bytes_crc16_ibm_desired_result)
    return 0x1C;    // Verify failed



  /************* Verify CRC32 ***************/
  result1 = CRC32_ComputeBytes(bytes, _countof(bytes));
  result2 = CRC32_ComputeBytes_Update(0x00000000, bytes, _countof(bytes));
  if(result1 != result2)
    return 0x1D;    // Verify failed

  if(result1 != bytes_crc32_desired_result)
    return 0x1E;    // Verify failed

  result3 = CRC32_ComputeBytes_Update(0x00000000, bytes, bytes_div_point);
  result4 = CRC32_ComputeBytes_Update(result3, bytes + bytes_div_point, _countof(bytes) - bytes_div_point);
  if(result4 != bytes_crc32_desired_result)
    return 0x1F;    // Verify failed



  /************* Verify CRC32-MPEG2 ***************/
  result1 = CRC32_MPEG2_ComputeBytes(bytes, _countof(bytes));
  result2 = CRC32_MPEG2_ComputeBytes_Update(0xFFFFFFFF, bytes, _countof(bytes));
  if(result1 != result2)
    return 0x20;    // Verify failed

  if(result1 != bytes_crc32_mpeg2_desired_result)
    return 0x21;    // Verify failed

  result3 = CRC32_MPEG2_ComputeBytes_Update(0xFFFFFFFF, bytes, bytes_div_point);
  result4 = CRC32_MPEG2_ComputeBytes_Update(result3, bytes + bytes_div_point, _countof(bytes) - bytes_div_point);
  if(result4 != bytes_crc32_mpeg2_desired_result)
    return 0x22;    // Verify failed

  return 0x00;
}



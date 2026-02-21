/* Includes ------------------------------------------------------------------*/
#include "compute.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const uint16_t halfwords[36] = {0x4176,0x6A15,0x78E2,0x0EAE,0x321E,0x5703,0x41A5,0x20FD,0x63AE,0x7E1F,0x3931,0x12FC,0x2606,0x0756,0x610D,0x342A,0x4E7B,0x673C,0x3DCB,0x0FDD,0x2FD8,0x0700,0x2F2F,0x74DA,0x53A2,0x28A0,0x1394,0x1B23,0x58A1,0x5B59,0x3A8C,0x5C10,0x1A9B,0x1462,0x6AC7,0x30A1};
const uint32_t halfwords_div_point = 15;
const uint16_t halfwords_crc16_ccitt_desired_result = 0xA177;
const uint16_t halfwords_crc16_modbus_desired_result = 0x99C2;
const uint16_t halfwords_crc16_ibm_desired_result = 0x6B18;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Verify CRC computes the halfword array
 * @param  None
 * @return 0 - Pass
 *         Others - Failed
 */
uint8_t computeHalfWords(void)
{
  /************* Verify CRC16-CCITT ***************/
  result1 = CRC16_CCITT_ComputeHalfWords(halfwords, _countof(halfwords));
  result2 = CRC16_CCITT_ComputeHalfWords_Update(0xFFFF, halfwords, _countof(halfwords));
  if(result1 != result2)
    return 0x51;    // Verify failed

  if(result1 != halfwords_crc16_ccitt_desired_result)
    return 0x52;    // Verify failed

  result3 = CRC16_CCITT_ComputeHalfWords_Update(0xFFFF, halfwords, halfwords_div_point);
  result4 = CRC16_CCITT_ComputeHalfWords_Update(result3, halfwords + halfwords_div_point, _countof(halfwords) - halfwords_div_point);
  if(result4 != halfwords_crc16_ccitt_desired_result)
    return 0x53;    // Verify failed



  /************* Verify CRC16-Modbus ***************/
  result1 = CRC16_Modbus_ComputeHalfWords(halfwords, _countof(halfwords));
  result2 = CRC16_Modbus_ComputeHalfWords_Update(0xFFFF, halfwords, _countof(halfwords));
  if(result1 != result2)
    return 0x54;    // Verify failed

  if(result1 != halfwords_crc16_modbus_desired_result)
    return 0x55;    // Verify failed

  result3 = CRC16_Modbus_ComputeHalfWords_Update(0xFFFF, halfwords, halfwords_div_point);
  result4 = CRC16_Modbus_ComputeHalfWords_Update(result3, halfwords + halfwords_div_point, _countof(halfwords) - halfwords_div_point);
  if(result4 != halfwords_crc16_modbus_desired_result)
    return 0x56;    // Verify failed



  /************* Verify CRC16-IBM ***************/
  result1 = CRC16_IBM_ComputeHalfWords(halfwords, _countof(halfwords));
  result2 = CRC16_IBM_ComputeHalfWords_Update(0x0000, halfwords, _countof(halfwords));
  if(result1 != result2)
    return 0x57;    // Verify failed

  if(result1 != halfwords_crc16_ibm_desired_result)
    return 0x58;    // Verify failed

  result3 = CRC16_IBM_ComputeHalfWords_Update(0x0000, halfwords, halfwords_div_point);
  result4 = CRC16_IBM_ComputeHalfWords_Update(result3, halfwords + halfwords_div_point, _countof(halfwords) - halfwords_div_point);
  if(result4 != halfwords_crc16_ibm_desired_result)
    return 0x59;    // Verify failed

  return 0x00;
}



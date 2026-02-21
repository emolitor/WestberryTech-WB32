/* Includes ------------------------------------------------------------------*/
#include "compute.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const uint32_t words[36] = {0x41766A15,0x78E20EAE,0x321E5703,0x41A520FD,0x63AE7E1F,0x393112FC,0x26060756,0x610D342A,0x4E7B673C,0x3DCB0FDD,0x2FD80700,0x2F2F74DA,0x53A228A0,0x13941B23,0x58A15B59,0x3A8C5C10,0x1A9B1462,0x6AC730A1,0x07633FD9,0x7F366A13,0x45135DFE,0x5F6F58B1,0x0D95009F,0x575454ED,0x63683CD3,0x11EF5EAF,0x03A064EB,0x3BA146ED,0x392F1EA6,0x0F7C177C,0x756429A4,0x53ED2D26,0x51B16A1D,0x0A9E37FC,0x5EB10BD3,0x579A15ED};
const uint32_t words_div_point = 15;
const uint32_t words_crc32_desired_result = 0x26F2490B;
const uint32_t words_crc32_mpeg2_desired_result = 0xC18FA807;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Verify CRC computes the word array
 * @param  None
 * @return 0 - Pass
 *         Others - Failed
 */
uint8_t computeWords(void)
{
  /************* Verify CRC32 ***************/
  result1 = CRC32_ComputeWords(words, _countof(words));
  result2 = CRC32_ComputeWords_Update(0x00000000, words, _countof(words));
  if(result1 != result2)
    return 0x81;    // Verify failed

  if(result1 != words_crc32_desired_result)
    return 0x82;    // Verify failed

  result3 = CRC32_ComputeWords_Update(0x00000000, words, words_div_point);
  result4 = CRC32_ComputeWords_Update(result3, words + words_div_point, _countof(words) - words_div_point);
  if(result4 != words_crc32_desired_result)
    return 0x83;    // Verify failed



  /************* Verify CRC32-MPEG2 ***************/
  result1 = CRC32_MPEG2_ComputeWords(words, _countof(words));
  result2 = CRC32_MPEG2_ComputeWords_Update(0xFFFFFFFF, words, _countof(words));
  if(result1 != result2)
    return 0x84;    // Verify failed

  if(result1 != words_crc32_mpeg2_desired_result)
    return 0x85;    // Verify failed

  result3 = CRC32_MPEG2_ComputeWords_Update(0xFFFFFFFF, words, words_div_point);
  result4 = CRC32_MPEG2_ComputeWords_Update(result3, words + words_div_point, _countof(words) - words_div_point);
  if(result4 != words_crc32_mpeg2_desired_result)
    return 0x86;    // Verify failed

  return 0x00;
}




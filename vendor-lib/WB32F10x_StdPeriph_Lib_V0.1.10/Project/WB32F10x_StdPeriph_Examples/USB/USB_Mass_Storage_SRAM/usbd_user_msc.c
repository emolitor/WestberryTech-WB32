#include "wb32f10x.h"
#include "usbd_user_msc.h"
#include <string.h>


static const uint8_t USBD_USER_MSC_InquiryData[36] = 
{
  0x00,           /* (0) */
  0x80,           /* (1) */
  0x02,           /* (2) */
  0x02,           /* (3) */
  (35-4),         /* (4) */
  0x00,           /* (5) */
  0x00,           /* (6) */
  0x00,           /* (7) */
  'X', 'X', 'X', ' ', ' ', ' ', ' ', ' ',   /* Vendor identification (8-15) */
  'S', 't', 'o', 'r', 'a', 'g', 'e', ' ',
  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   /* Product identification (16-31) */
  '0', '0', '0', '1',                       /* Product revision level (32-35) */
};

const uint8_t* USBD_User_MSC_GetInquiryData(uint8_t lun, uint32_t* inquiry_data_len)
{
  *inquiry_data_len = sizeof(USBD_USER_MSC_InquiryData);
  return USBD_USER_MSC_InquiryData;
}

int8_t USBD_User_MSC_GetCapacity(uint8_t lun, uint32_t *block_count, uint32_t *block_size)
{
  *block_count = 64;
  *block_size = 512;
  return 0;
}

uint8_t storage_memory[32*1024] = {0};

uint32_t USBD_User_MSC_Read(uint8_t lun, uint32_t addr, uint8_t *buf, uint16_t size)
{
  memcpy(buf, storage_memory + addr, size);
  return size;
}


static uint32_t write_addr;
static uint32_t write_len;

int8_t USBD_User_MSC_PrepareWrite(uint8_t lun, uint32_t lba, uint32_t cnt)
{
  write_addr = lba * 512;
  write_len = cnt * 512;
  // Erase blocks
  memset(storage_memory + write_addr, 0 , write_len);
  return 0;
}

uint32_t USBD_User_MSC_Write(uint8_t lun, const uint8_t *pdata, uint16_t len)
{
  if (write_len < len)
    len = write_len;
  memcpy(storage_memory + write_addr, pdata, len);
  write_addr += len;
  write_len -= len;
  return len;
}

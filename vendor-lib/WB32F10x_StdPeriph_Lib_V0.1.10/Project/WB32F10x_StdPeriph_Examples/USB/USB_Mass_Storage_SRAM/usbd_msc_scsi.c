#include <string.h>
#include "usbd_msc.h"
#include "usbd_user_msc.h"

void USBD_MSC_SendData(const uint8_t* pdata, uint32_t len);
void USBD_MSC_SendCSW(void);
void USBD_MSC_CommandAbort(void);

static struct {
  uint8_t SenseKey;
  uint8_t ASC;
} sense_data[USBD_USER_MSC_MAXLUN + 1];

void USBD_MSC_SCSI_SenseDataUpdate(uint8_t lun, uint8_t SenseKey, uint8_t ASC)
{
  sense_data[lun].SenseKey = SenseKey;
  sense_data[lun].ASC = ASC;
}

void USBD_MSC_SCSI_TestUnitReady(usbd_msc_t* msc)
{
  msc->csw.bStatus = CSW_CMD_PASSED;
  USBD_MSC_SendCSW();
}

void USBD_MSC_SCSI_RequestSense(usbd_msc_t* msc)
{
  memset(msc->bulk_buf, 0, 18);
  msc->bulk_buf[0] = 0x70;
  msc->bulk_buf[2] = sense_data[msc->cbw.bLUN].SenseKey;
  msc->bulk_buf[7] = 17-7;
  msc->bulk_buf[12] = sense_data[msc->cbw.bLUN].ASC;
  msc->bulk_buf[13] = 0;
  USBD_MSC_SendData(msc->bulk_buf, 18);
}

void USBD_MSC_SCSI_Inquiry(usbd_msc_t* msc)
{
  const uint8_t* inquiry_data;
  uint32_t inquiry_data_len = 0;

  if (msc->cbw.CB[1] & 0x01)
  {
    USBD_MSC_SCSI_SenseDataUpdate(msc->cbw.bLUN, SCSI_SENSE_KEY_ILLEGAL_REQUEST, SCSI_ASC_INVALID_FIELD_IN_CDB);
    USBD_MSC_CommandAbort();
  }
  else
  {
    inquiry_data = USBD_User_MSC_GetInquiryData(msc->cbw.bLUN, &inquiry_data_len);
    // TODO: Check return
    USBD_MSC_SendData(inquiry_data, inquiry_data_len);
  }
}

void USBD_MSC_SCSI_ReadFormatCapacities(usbd_msc_t* msc)
{
  uint32_t blk_cnt = 0, blk_size = 0;

  USBD_User_MSC_GetCapacity(msc->cbw.bLUN, &blk_cnt, &blk_size);
  // TODO: Check return
  msc->bulk_buf[0] = 0;
  msc->bulk_buf[1] = 0;
  msc->bulk_buf[2] = 0;
  msc->bulk_buf[3] = 0x08;    /* Capacity List Length */
  msc->bulk_buf[4] = (uint8_t)(blk_cnt >> 24);
  msc->bulk_buf[5] = (uint8_t)(blk_cnt >> 16);
  msc->bulk_buf[6] = (uint8_t)(blk_cnt >> 8);
  msc->bulk_buf[7] = (uint8_t)(blk_cnt);
  msc->bulk_buf[8] = 0x02;    /* Descriptor Code */
  msc->bulk_buf[9] = (uint8_t)(blk_size >> 16);
  msc->bulk_buf[10] = (uint8_t)(blk_size >> 8);
  msc->bulk_buf[11] = (uint8_t)(blk_size);
  USBD_MSC_SendData(msc->bulk_buf, 12);
}

void USBD_MSC_SCSI_ReadCapacity10(usbd_msc_t* msc)
{
  uint32_t blk_cnt = 0, blk_size = 0;

  USBD_User_MSC_GetCapacity(msc->cbw.bLUN, &blk_cnt, &blk_size);
  
  // TODO: Check return

  blk_cnt--;
  msc->bulk_buf[0] = (uint8_t)(blk_cnt >> 24);
  msc->bulk_buf[1] = (uint8_t)(blk_cnt >> 16);
  msc->bulk_buf[2] = (uint8_t)(blk_cnt >> 8);
  msc->bulk_buf[3] = (uint8_t)(blk_cnt);
  msc->bulk_buf[4] = (uint8_t)(blk_size >> 24);
  msc->bulk_buf[5] = (uint8_t)(blk_size >> 16);
  msc->bulk_buf[6] = (uint8_t)(blk_size >> 8);
  msc->bulk_buf[7] = (uint8_t)(blk_size);
  USBD_MSC_SendData(msc->bulk_buf, 8);
}


void USBD_MSC_SCSI_Read10(usbd_msc_t* msc)
{
  static uint32_t read_addr, read_len;

  if (msc->bot_state == MSC_BS_IDLE)
  {
    uint32_t blk_cnt = 0, blk_size = 0;
    USBD_User_MSC_GetCapacity(msc->cbw.bLUN, &blk_cnt, &blk_size);
    read_addr = ((msc->cbw.CB[2] << 24) | (msc->cbw.CB[3] << 16) | (msc->cbw.CB[4] << 8) | msc->cbw.CB[5]) * blk_size;
    read_len = ((msc->cbw.CB[7] << 8) | msc->cbw.CB[8]) * blk_size;
    msc->bot_state = MSC_BS_DATA_IN;
  }

  if (msc->bot_state == MSC_BS_DATA_IN)
  {
    if (read_len > 64)
    {
      USBD_User_MSC_Read(msc->cbw.bLUN, read_addr, msc->bulk_buf, 64);
      USBD_HW_Transmit(USBD_USER_MSC_BULK_IN_EP, msc->bulk_buf, 64);
      read_addr += 64;
      read_len -= 64;
      msc->csw.dDataResidue -= 64;
    }
    else
    {
      USBD_User_MSC_Read(msc->cbw.bLUN, read_addr, msc->bulk_buf, read_len);
      USBD_HW_Transmit(USBD_USER_MSC_BULK_IN_EP, msc->bulk_buf, read_len);
      msc->csw.dDataResidue -= read_len;
      msc->csw.bStatus = CSW_CMD_PASSED;
      msc->bot_state = MSC_BS_DATA_IN_LAST;
    }
  }
}


void USBD_MSC_SCSI_Write10(usbd_msc_t* msc)
{
  uint32_t lba, cnt;
  lba = (msc->cbw.CB[2] << 24) | (msc->cbw.CB[3] << 16) | (msc->cbw.CB[4] << 8) | msc->cbw.CB[5];
  cnt = (msc->cbw.CB[7] << 8) | msc->cbw.CB[8];

  USBD_User_MSC_PrepareWrite(msc->cbw.bLUN, lba, cnt);

  msc->bot_state = MSC_BS_DATA_OUT;
  USBD_HW_ReadyToReceive(USBD_USER_MSC_BULK_OUT_EP);
}

void USBD_MSC_SCSI_ProcessWrite(usbd_msc_t* msc, const uint8_t* buf, uint32_t len)
{
  USBD_User_MSC_Write(msc->cbw.bLUN, buf, len);

  msc->csw.dDataResidue -= len;
  if ((msc->csw.dDataResidue == 0) || (len < 64))
  {
    msc->csw.bStatus = CSW_CMD_PASSED;
    USBD_MSC_SendCSW();
  }
  else
  {
    /* Ready to get next BULK out */
    USBD_HW_ReadyToReceive(USBD_USER_MSC_BULK_OUT_EP);
  }
}

// void USBD_MSC_SCSI_ModeSense6(void)
// {
//   msc_bot_buf[0] = 0x00;
//   msc_bot_buf[1] = 0x00;
//   msc_bot_buf[2] = 0x00;
//   msc_bot_buf[3] = 0x00;
//   msc_bot_buf[4] = 0x00;
//   msc_bot_buf[5] = 0x00;
//   msc_bot_buf[6] = 0x00;
//   msc_bot_buf[7] = 0x00;
//   USBD_MSC_SendData(msc_bot_buf, 8);
// }

// void USBD_MSC_SCSI_ModeSense10(void)
// {
//   msc_bot_buf[0] = 0x00;
//   msc_bot_buf[1] = 0x06;
//   msc_bot_buf[2] = 0x00;
//   msc_bot_buf[3] = 0x00;
//   msc_bot_buf[4] = 0x00;
//   msc_bot_buf[5] = 0x00;
//   msc_bot_buf[6] = 0x00;
//   msc_bot_buf[7] = 0x00;
//   USBD_MSC_SendData(msc_bot_buf, 8);
// }




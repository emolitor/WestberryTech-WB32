#include <string.h>
#include "usbd_core.h"
#include "usbd_msc.h"
#include "usbd_msc_scsi.h"
#include "usbd_user_msc.h"

static uint8_t usbd_msc_max_lun = 0;

static usbd_msc_t usbd_msc;
void USBD_MSC_SendCSW(void);
void USBD_MSC_DecodeCBW(void);

void USBD_MSC_Init(void)
{
  memset(&usbd_msc, 0, sizeof(usbd_msc));
  usbd_msc.bot_state = MSC_BS_IDLE;
}

void USBD_MSC_DeInit(void)
{
  memset(&usbd_msc, 0, sizeof(usbd_msc));
  usbd_msc.bot_state = MSC_BS_IDLE;
}

/**
 * @brief  Handle the setup device requests of MSC class.
 * @return The next control stage.
 */
UsbdControlStage USBD_EndPoint0_Setup_MSC_Req(void)
{
  switch (UsbdCoreInfo.SetupPacket.bRequest)
  {
    case MSC_REQUEST_MASS_STORAGE_RESET:
      if ((UsbdCoreInfo.SetupPacket.wValue == 0) &&
          (UsbdCoreInfo.SetupPacket.wLength == 0))
      {
        return USBD_CONTROL_STAGE_STATUS;
      }
      break;

    case MSC_REQUEST_GET_MAX_LUN:
      if ((UsbdCoreInfo.SetupPacket.wValue == 0) &&
          (UsbdCoreInfo.SetupPacket.wLength == 1))
      {
        usbd_msc_max_lun = USBD_USER_MSC_MAXLUN;
        UsbdCoreInfo.DataPtr = (uint8_t*)&usbd_msc_max_lun;
        return USBD_CONTROL_STAGE_IN_DATA;
      }
      break;

    default:
      break;
  }

  return USBD_CONTROL_STAGE_STALL;
}

void USBD_ReqClrFeature_MSC(uint8_t ep_addr)
{
  if ((ep_addr == (USBD_USER_MSC_BULK_IN_EP | 0x80)) && (usbd_msc.bot_state == MSC_BS_ERROR))
  {
    USBD_MSC_SendCSW();
  }
}

void USBD_MSC_Bulk_Out(void)
{
  uint32_t length;
  length = USBD_HW_GetRxDataCount(USBD_USER_MSC_BULK_OUT_EP);
  USBD_HW_ReadEP(USBD_USER_MSC_BULK_OUT_EP, usbd_msc.bulk_buf, length);

  switch (usbd_msc.bot_state)
  {
  case MSC_BS_IDLE:
    memcpy(&usbd_msc.cbw, usbd_msc.bulk_buf, length);
    if ((length == sizeof(USB_MSC_CBW)) && 
        (usbd_msc.cbw.dSignature == MSC_CBW_SIGNATURE) && 
        (usbd_msc.cbw.bCBLength >= 1) && (usbd_msc.cbw.bCBLength <= 16))
    {
      USBD_MSC_DecodeCBW();
    }
    else
    {   /* CBW Not Valid */
      USBD_HW_SetStallEP(USBD_USER_MSC_BULK_IN_EP);
      USBD_HW_SetStallEP(USBD_USER_MSC_BULK_OUT_EP);
      usbd_msc.csw.bStatus = CSW_PHASE_ERROR;
      usbd_msc.bot_state = MSC_BS_ERROR;
    }
    break;

  case MSC_BS_DATA_OUT:
    USBD_MSC_SCSI_ProcessWrite(&usbd_msc, usbd_msc.bulk_buf, length);

  default:
    break;
  }
}

void USBD_MSC_Bulk_In(void)
{
  switch (usbd_msc.bot_state)
  {
  case MSC_BS_DATA_IN:
    USBD_MSC_SCSI_Read10(&usbd_msc);
    break;

  case MSC_BS_DATA_IN_LAST:
    USBD_MSC_SendCSW();
    break;

  case MSC_BS_STATUS:
    usbd_msc.bot_state = MSC_BS_IDLE;
    break;

  default:
    break;
  }
}

void USBD_MSC_SendData(const uint8_t* pdata, uint32_t len)
{
  if (len > usbd_msc.cbw.dDataLength)
    len = usbd_msc.cbw.dDataLength;
  
  USBD_HW_Transmit(USBD_USER_MSC_BULK_IN_EP, pdata, len);
  usbd_msc.csw.dDataResidue -= len;
  usbd_msc.csw.bStatus = CSW_CMD_PASSED;
  usbd_msc.bot_state = MSC_BS_DATA_IN_LAST;
}

void USBD_MSC_SendCSW(void)
{
  usbd_msc.csw.dSignature = MSC_CSW_SIGNATURE;
  USBD_HW_Transmit(USBD_USER_MSC_BULK_IN_EP, (const uint8_t*)&usbd_msc.csw, sizeof(usbd_msc.csw));
  usbd_msc.bot_state = MSC_BS_STATUS;

  /* Ready to get next BULK out */
  USBD_HW_ReadyToReceive(USBD_USER_MSC_BULK_OUT_EP);
}

void USBD_MSC_CommandAbort(void)
{
  usbd_msc.csw.bStatus = CSW_CMD_FAILED;
  usbd_msc.bot_state = MSC_BS_ERROR;
  USBD_HW_SetStallEP(USBD_USER_MSC_BULK_IN_EP);
}

void USBD_MSC_DecodeCBW(void)
{
  usbd_msc.csw.dSignature = 0;
  usbd_msc.csw.dTag = usbd_msc.cbw.dTag;
  usbd_msc.csw.dDataResidue = usbd_msc.cbw.dDataLength;

  switch (usbd_msc.cbw.CB[0])
  {
    case SCSI_TEST_UNIT_READY:
      USBD_MSC_SCSI_TestUnitReady(&usbd_msc);
      break;

    case SCSI_REQUEST_SENSE:
      USBD_MSC_SCSI_RequestSense(&usbd_msc);
      break;

    case SCSI_INQUIRY:
      USBD_MSC_SCSI_Inquiry(&usbd_msc);
      break;

    case SCSI_READ_FORMAT_CAPACITIES:
      USBD_MSC_SCSI_ReadFormatCapacities(&usbd_msc);
      break;

    case SCSI_READ_CAPACITY10:
      USBD_MSC_SCSI_ReadCapacity10(&usbd_msc);
      break;

    case SCSI_READ10:
      USBD_MSC_SCSI_Read10(&usbd_msc);
      break;

    case SCSI_WRITE10:
      USBD_MSC_SCSI_Write10(&usbd_msc);
      break;

    default:
      USBD_MSC_SCSI_SenseDataUpdate(usbd_msc.cbw.bLUN, SCSI_SENSE_KEY_ILLEGAL_REQUEST, SCSI_ASC_NO_ADDITIONAL_SENSE_INFO);
      USBD_MSC_CommandAbort();
      break;
  }
}

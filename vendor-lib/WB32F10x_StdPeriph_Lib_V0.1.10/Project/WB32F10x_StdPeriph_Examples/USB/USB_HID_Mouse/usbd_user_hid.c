#include "wb32f10x.h"
#include "usbd_core.h"
#include "usbd_user_hid.h"

uint8_t hid_report_buf[4] = {0,0,0,0};
int hid_report_xfer_flag = 0;

bool USBD_User_HID_GetReport(void)
{
  if ((UsbdCoreInfo.SetupPacket.wValueH == HID_REPORT_INPUT) &&
      (UsbdCoreInfo.SetupPacket.wLength == 4)) {
    UsbdCoreInfo.DataPtr = hid_report_buf;
    hid_report_xfer_flag = 0;
    return true;
  }

  return false;
}

bool USBD_User_HID_SetReport(bool data_received)
{
  return false;
}


static uint8_t hid_duration = 0;

bool USBD_User_HID_GetIdle(void)
{
  UsbdCoreInfo.DataPtr = &hid_duration;
  return true;
}

bool USBD_User_HID_SetIdle(void)
{
  hid_duration = UsbdCoreInfo.SetupPacket.wValueH;
  return true;
}


// 0 - Boot Protocol
// 1 - Report Protocol
static uint8_t hid_data_protocol = 1;

bool USBD_User_HID_GetProtocol(void)
{
  UsbdCoreInfo.DataPtr = &hid_data_protocol;
  return true;
}

bool USBD_User_HID_SetProtocol(void)
{
  hid_data_protocol = UsbdCoreInfo.SetupPacket.wValueL;
  return true;
}

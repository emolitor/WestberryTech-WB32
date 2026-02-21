#include <stdlib.h>
#include "usbd_core.h"
#include "usbd_hid.h"
#include "usbd_user.h"
#include "usbd_user_hid.h"

/**
 * @brief  Handle the setup device requests of HID class.
 * @return The next control stage.
 */
UsbdControlStage USBD_EndPoint0_Setup_HID_Req(void)
{
  if ((UsbdCoreInfo.SetupPacket.bmRequestType & USB_REQUEST_TYPE_Msk) == USB_REQUEST_TYPE_CLASS)
  {
    switch (UsbdCoreInfo.SetupPacket.bRequest)
    {
      case HID_REQUEST_GET_REPORT:
        if (USBD_User_HID_GetReport()) {
          return USBD_CONTROL_STAGE_IN_DATA;
        }
        break;
      case HID_REQUEST_GET_IDLE:
        if (UsbdCoreInfo.SetupPacket.wLength == 1) {
          if (USBD_User_HID_GetIdle()) {
            return USBD_CONTROL_STAGE_IN_DATA;
          }
        }
        break;
      case HID_REQUEST_GET_PROTOCOL:
        if (UsbdCoreInfo.SetupPacket.wLength == 1) {
          if (USBD_User_HID_GetProtocol()) {
            return USBD_CONTROL_STAGE_IN_DATA;
          }
        }
        break;
      case HID_REQUEST_SET_REPORT:
        if (USBD_User_HID_SetReport(false)) {
          return USBD_CONTROL_STAGE_OUT_DATA;
        }
        break;
      case HID_REQUEST_SET_IDLE:
        if (USBD_User_HID_SetIdle()) {
          return USBD_CONTROL_STAGE_STATUS;
        }
        break;
      case HID_REQUEST_SET_PROTOCOL:
        if (USBD_User_HID_SetProtocol()) {
          return USBD_CONTROL_STAGE_STATUS;
        }
        break;
    }
  }
  else if ((UsbdCoreInfo.SetupPacket.bmRequestType & USB_REQUEST_TYPE_Msk) == USB_REQUEST_TYPE_STANDARD)
  {
    if (UsbdCoreInfo.SetupPacket.bRequest == USB_REQUEST_GET_DESCRIPTOR)
    {
      uint16_t length;

      UsbdCoreInfo.DataPtr = USBD_User_GetHIDDescriptor(UsbdCoreInfo.SetupPacket.wValueH, &length);

      if (UsbdCoreInfo.DataPtr != NULL)
      {
        if (UsbdCoreInfo.DataSize > length) {
          UsbdCoreInfo.DataSize = length;
        }
        return USBD_CONTROL_STAGE_IN_DATA;
      }
    }
  }

  return USBD_CONTROL_STAGE_STALL;
}

/**
 * @brief  Handle the out device requests of HID class.
 * @return The next control stage.
 */
UsbdControlStage USBD_EndPoint0_Out_HID_Req(void)
{
  switch (UsbdCoreInfo.SetupPacket.bRequest)
  {
    case HID_REQUEST_SET_REPORT:
      if (USBD_User_HID_SetReport(true)) {
        return USBD_CONTROL_STAGE_STATUS;
      }
      break;
  }

  return USBD_CONTROL_STAGE_STALL;
}

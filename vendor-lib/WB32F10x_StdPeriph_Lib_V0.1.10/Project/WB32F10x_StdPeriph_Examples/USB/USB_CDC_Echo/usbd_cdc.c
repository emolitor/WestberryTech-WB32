#include "usbd_core.h"
#include "usbd_cdc.h"
#include "usbd_user_cdc.h"

static USB_CDC_LINE_CODING _usbd_cdc_acm_line_coding;

/**
 * @brief  Handle the setup device requests of CDC class.
 * @return The next control stage.
 */
UsbdControlStage USBD_EndPoint0_Setup_CDC_Req(void)
{
  switch (UsbdCoreInfo.SetupPacket.bRequest)
  {
    case USB_CDC_REQUEST_SET_COMM_FEATURE:
      /* Add your code here */
      break;

    case USB_CDC_REQUEST_GET_COMM_FEATURE:
      /* Add your code here */
      break;

    case USB_CDC_REQUEST_CLEAR_COMM_FEATURE:
      /* Add your code here */
      break;

    case USB_CDC_REQUEST_SET_LINE_CODING:
      if (UsbdCoreInfo.SetupPacket.wLength == 7) {
        UsbdCoreInfo.DataPtr = (uint8_t*)&_usbd_cdc_acm_line_coding;
        return USBD_CONTROL_STAGE_OUT_DATA;
      }
      break;

    case USB_CDC_REQUEST_GET_LINE_CODING:
      if (UsbdCoreInfo.SetupPacket.wLength == 7) {
        if (USBD_User_CDC_GetLineCoding(&_usbd_cdc_acm_line_coding)) {
          UsbdCoreInfo.DataPtr = (uint8_t*)&_usbd_cdc_acm_line_coding;
          return USBD_CONTROL_STAGE_IN_DATA;
        }
      }
      break;

    case USB_CDC_REQUEST_SET_CONTROL_LINE_STATE:
      if (USBD_User_CDC_SetControlLineState(UsbdCoreInfo.SetupPacket.wValue)) {
        return USBD_CONTROL_STAGE_STATUS;
      }
      break;

    case USB_CDC_REQUEST_SEND_BREAK:
      if (USBD_User_CDC_SendBreak(UsbdCoreInfo.SetupPacket.wValue)) {
        return USBD_CONTROL_STAGE_STATUS;
      }
      break;

    default:
      break;
  }

  return USBD_CONTROL_STAGE_STALL;
}

/**
 * @brief  Handle the out device requests of CDC class.
 * @return The next control stage.
 */
UsbdControlStage USBD_EndPoint0_Out_CDC_Req(void)
{
  switch (UsbdCoreInfo.SetupPacket.bRequest)
  {
    case USB_CDC_REQUEST_SET_COMM_FEATURE:
      /* Add your code here */
      break;

    case USB_CDC_REQUEST_SET_LINE_CODING:
      if (USBD_User_CDC_SetLineCoding(&_usbd_cdc_acm_line_coding)) {
        return USBD_CONTROL_STAGE_STATUS;
      }
      break;
  }

  return USBD_CONTROL_STAGE_STALL;
}

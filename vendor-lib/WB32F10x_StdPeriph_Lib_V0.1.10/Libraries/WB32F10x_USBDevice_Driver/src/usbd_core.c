/**
 * @file    usbd_core.c
 * @author  Westberry Application Team
 * @attention
 * 
 * Copyright (c) 2020 - 2023 Westberry Technology (ChangZhou) Corp., Ltd
 * All rights reserved.
 * 
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, WESTBERRY TECHNOLOGY SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 */

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"
#include <string.h>
#include "usbd_core.h"
#include "usbd_user.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile UsbdCoreInfo_t UsbdCoreInfo;
static volatile uint16_t EP0Buf;
const uint8_t UsbdMaxPacketSize0 = 64;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Initializes the USB device library.
 * @return None
 */
void USBD_Init(void)
{
  memset((void *)&UsbdCoreInfo, 0, sizeof(UsbdCoreInfo));

  USBD_User_Init();
}

/**
 * @brief  Deinitializes the USB device library.
 * @return None
 */
void USBD_DeInit(void)
{
  USBD_User_DeInit();
}

/**
 * @brief  Connects the device to the USB host.
 * @return None
 */
void USBD_Connect(void)
{
  USBD_User_Connect();
}

/**
 * @brief  Disconnects the device from the USB host.
 * @return None
 */
void USBD_Disconnect(void)
{
  USBD_User_Disconnect();
}


/**
 * @brief  Handle reset event.
 * @return None
 */
void USBD_Core_Reset(void)
{
  USB->INTRINE = 0x01;
  USB->INTROUTE = 0x00;
  memset((void *)&UsbdCoreInfo, 0, sizeof(UsbdCoreInfo));

  USBD_User_Reset();
}

/**
 * @brief  Handle suspend event.
 * @return None
 */
void USBD_Core_Suspend(void)
{
  if (UsbdCoreInfo.DeviceState != USBD_STATE_SUSPENDED)
  {
    UsbdCoreInfo.OldDeviceState = UsbdCoreInfo.DeviceState;
    UsbdCoreInfo.DeviceState = USBD_STATE_SUSPENDED;
    USBD_User_Suspend();
  }
}

/**
 * @brief  Handle resume event.
 * @return None
 */
void USBD_Core_Resume(void)
{
  if (UsbdCoreInfo.DeviceState == USBD_STATE_SUSPENDED)
  {
    USBD_User_Resume();
    UsbdCoreInfo.DeviceState = UsbdCoreInfo.OldDeviceState;
  }
}

/**
 * @brief  Handle Get Status request.
 * @return true - Success, false - Error
 */
static bool USBD_StdReqGetStatus(void)
{
  uint8_t ep_addr;
  uint8_t *cfg_desc;
  uint16_t len;

  EP0Buf = 0;

  switch (UsbdCoreInfo.SetupPacket.bmRequestType & USB_REQUEST_RECIPIENT_Msk)
  {
    case USB_REQUEST_RECIPIENT_DEVICE:
      cfg_desc = USBD_User_GetConfigDescriptor(UsbdCoreInfo.Configuration, &len);
      if (cfg_desc[7] & USB_CONFIG_POWERED_MASK) {
        EP0Buf = 0x01;
      }

      if (UsbdCoreInfo.RemoteWakeup) {
        EP0Buf |= 0x02;
      }
      break;

    case USB_REQUEST_RECIPIENT_INTERFACE:
      break;

    case USB_REQUEST_RECIPIENT_ENDPOINT:
      ep_addr = UsbdCoreInfo.SetupPacket.wIndexL;
      EP0Buf = USBD_HW_IsStalled(ep_addr);
      break;

    default:
      return false;
  }

  UsbdCoreInfo.DataPtr = (uint8_t*)&EP0Buf;

  return true;
}

__WEAK void USBD_ReqClrFeature_MSC(uint8_t ep_addr)
{}

/**
 * @brief  Handle Set/Clear Feature request.
 * @param  set: true - Set, false - Clear
 * @return true - Success, false - Error
 */
static bool USBD_StdReqSetClrFeature(bool set)
{
  uint32_t ep_addr;

  switch (UsbdCoreInfo.SetupPacket.bmRequestType & USB_REQUEST_RECIPIENT_Msk) {
    case USB_REQUEST_RECIPIENT_DEVICE:
      if (UsbdCoreInfo.SetupPacket.wValue == USB_FEATURE_REMOTE_WAKEUP) {
        if (set) {
          UsbdCoreInfo.RemoteWakeup = 1;
        }
        else {
          UsbdCoreInfo.RemoteWakeup = 0;
        }
        return true;
      }
      break;

    case USB_REQUEST_RECIPIENT_ENDPOINT:
      if (UsbdCoreInfo.SetupPacket.wValue == USB_FEATURE_ENDPOINT_STALL) {
        ep_addr = UsbdCoreInfo.SetupPacket.wIndexL;
        if ((ep_addr & 0x0F) != 0) {
          if (set) {
            USBD_HW_SetStallEP(ep_addr);
          }
          else {
            USBD_HW_ClrStallEP(ep_addr);
            USBD_ReqClrFeature_MSC(ep_addr);
          }
          return true;
        }
      }
      break;
  }

  return false;
}

/**
 * @brief  Handle Set Address request.
 * @return true - Success, false - Error
 */
static bool USBD_StdReqSetAddress(void)
{
  uint8_t dev_addr;

  dev_addr = UsbdCoreInfo.SetupPacket.wValueL & 0x7F;

  if (UsbdCoreInfo.DeviceState != USBD_STATE_CONFIGURED) {
    USB->FADDR = dev_addr;

    if (dev_addr != 0) {
      UsbdCoreInfo.DeviceState = USBD_STATE_ADDRESSED;
    }
    else {
      UsbdCoreInfo.DeviceState = USBD_STATE_DEFAULT;
    }
    return true;
  }

  return false;
}

/**
 * @brief  Handle Get Descriptor request.
 * @return true - Success, false - Error
 */
static bool USBD_StdReqGetDescriptor(void)
{
  uint16_t length;

  switch (UsbdCoreInfo.SetupPacket.wValueH) {
    case USB_DESC_TYPE_DEVICE:
      UsbdCoreInfo.DataPtr = USBD_User_GetDeviceDescriptor(&length);
      break;

    case USB_DESC_TYPE_CONFIGURATION:
      UsbdCoreInfo.DataPtr = USBD_User_GetConfigDescriptor(UsbdCoreInfo.SetupPacket.wValueL, &length);
      break;

    case USB_DESC_TYPE_STRING:
      UsbdCoreInfo.DataPtr = USBD_User_GetStringDescriptor(UsbdCoreInfo.SetupPacket.wValueL, &length);
      break;

    default:
      return false;
  }

  if (UsbdCoreInfo.DataPtr == NULL) {
    return false;
  }

  if (UsbdCoreInfo.DataSize > length) {
    UsbdCoreInfo.DataSize = length;
  }

  return true;
}

/**
 * @brief  Handle Set Configuration request.
 * @return true - Success, false - Error
 */
static bool USBD_StdReqSetConfiguration(void)
{
  uint8_t cfgidx;

  cfgidx = UsbdCoreInfo.SetupPacket.wValueL;

  switch (UsbdCoreInfo.DeviceState)
  {
  case USBD_STATE_ADDRESSED:
    if (cfgidx) {
      if (USBD_User_SetConfig(cfgidx)) {
        UsbdCoreInfo.Configuration = cfgidx;
        UsbdCoreInfo.DeviceState = USBD_STATE_CONFIGURED;
        return true;
      }
    }
    else {
      return true;
    }
    break;

  case USBD_STATE_CONFIGURED:
    if (cfgidx == 0) {
      UsbdCoreInfo.Configuration = 0;
      UsbdCoreInfo.DeviceState = USBD_STATE_ADDRESSED;
      USBD_User_ClearConfig(0);
      return true;
    }
    else if (cfgidx != UsbdCoreInfo.Configuration) {
      /* clear old configuration */
      USBD_User_ClearConfig(UsbdCoreInfo.Configuration);

      /* set new configuration */
      if (USBD_User_SetConfig(cfgidx)) {
        UsbdCoreInfo.Configuration = cfgidx;
        return true;
      }
    }
    else {
      return true;
    }
    break;

  default:
    break;
  }

  return false;
}

/**
 * @brief  Handle Standard Device Requests.
 * @return The next control stage.
 */
UsbdControlStage USBD_EndPoint0_Setup_StdReq(void)
{
  switch (UsbdCoreInfo.SetupPacket.bRequest) {
    case USB_REQUEST_GET_STATUS:
      if (UsbdCoreInfo.SetupPacket.wLength == 2)
      {
        if (USBD_StdReqGetStatus()) {
          return USBD_CONTROL_STAGE_IN_DATA;
        }
      }
      break;

    case USB_REQUEST_CLEAR_FEATURE:
      if (USBD_StdReqSetClrFeature(false)) {
        return USBD_CONTROL_STAGE_STATUS;
      }
      break;

    case USB_REQUEST_SET_FEATURE:
      if (USBD_StdReqSetClrFeature(true)) {
        return USBD_CONTROL_STAGE_STATUS;
      }
      break;

    case USB_REQUEST_SET_ADDRESS:
      if (UsbdCoreInfo.SetupPacket.bmRequestType == 0x00)
      {
        if (USBD_StdReqSetAddress()) {
          return USBD_CONTROL_STAGE_STATUS;
        }
      }
      break;

    case USB_REQUEST_GET_DESCRIPTOR:
      if (UsbdCoreInfo.SetupPacket.bmRequestType == 0x80)
      {
        if (USBD_StdReqGetDescriptor()) {
          return USBD_CONTROL_STAGE_IN_DATA;
        }
      }
      break;

    // case USB_REQUEST_SET_DESCRIPTOR:    // not support
    //   break;

    case USB_REQUEST_GET_CONFIGURATION:
      if ((UsbdCoreInfo.SetupPacket.bmRequestType == 0x80) &&
          (UsbdCoreInfo.SetupPacket.wLength == 1))
      {
        switch (UsbdCoreInfo.DeviceState)
        {
        case USBD_STATE_DEFAULT:
        case USBD_STATE_ADDRESSED:
          EP0Buf = 0;
          UsbdCoreInfo.DataPtr = (uint8_t*)&EP0Buf;
          return USBD_CONTROL_STAGE_IN_DATA;
          // break;

        case USBD_STATE_CONFIGURED:
          UsbdCoreInfo.DataPtr = (uint8_t*)&UsbdCoreInfo.Configuration;
          return USBD_CONTROL_STAGE_IN_DATA;
          // break;
        }
      }
      break;

    case USB_REQUEST_SET_CONFIGURATION:
      if (UsbdCoreInfo.SetupPacket.bmRequestType == 0x00)
      {
        if (USBD_StdReqSetConfiguration()) {
          return USBD_CONTROL_STAGE_STATUS;
        }
      }
      break;

    default:
      break;
  }

  return USBD_CONTROL_STAGE_STALL;
}

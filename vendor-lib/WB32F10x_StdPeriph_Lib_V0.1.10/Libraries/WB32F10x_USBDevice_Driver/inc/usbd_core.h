/**
 * @file    usbd_core.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CORE_H
#define __USBD_CORE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "usb_def.h"
#include "usbd_hw.h"

/* Exported types ------------------------------------------------------------*/
typedef struct {
  USB_SETUP_PACKET SetupPacket;
  uint8_t         *DataPtr;
  uint16_t         DataSize;
  uint16_t         DataSent;
  uint8_t          Configuration;
  uint8_t          RemoteWakeup;
  uint8_t          DeviceState;
  uint8_t          OldDeviceState;
} UsbdCoreInfo_t;
extern volatile UsbdCoreInfo_t UsbdCoreInfo;
extern const uint8_t UsbdMaxPacketSize0;

typedef enum
{
  USBD_CONTROL_STAGE_STALL,
  USBD_CONTROL_STAGE_IN_DATA,
  USBD_CONTROL_STAGE_OUT_DATA,
  USBD_CONTROL_STAGE_STATUS
} UsbdControlStage;

/* Exported constants --------------------------------------------------------*/

#define USB_DEVICE_STATE_UNCONNECTED  0
#define USB_DEVICE_STATE_ATTACHED     1
#define USB_DEVICE_STATE_POWERED      2
#define USB_DEVICE_STATE_SUSPENDED    3
#define USB_DEVICE_STATE_ADDRESSED    4
#define USB_DEVICE_STATE_CONFIGURED   5

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void USBD_Init(void);
void USBD_DeInit(void);
void USBD_Connect(void);
void USBD_Disconnect(void);

void USBD_Core_Reset(void);
void USBD_Core_Suspend(void);
void USBD_Core_Resume(void);

UsbdControlStage USBD_EndPoint0_Setup_StdReq(void);

void Handle_USBD_INT(uint8_t IntrUSB, uint8_t IntrIn, uint8_t IntrOut);

#ifdef __cplusplus
}
#endif

#endif /* __USBD_CORE_H */

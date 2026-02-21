/**
 * @file    usb_def.h
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
#ifndef __USB_DEF_H
#define __USB_DEF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/* Exported types ------------------------------------------------------------*/

/* ========================================  Start of section using anonymous unions  ======================================== */
#if   defined (__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined (__ICCARM__)
  #pragma language=extended
#elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wc11-extensions"
  #pragma clang diagnostic ignored "-Wreserved-id-macro"
#elif defined (__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined (__TMS470__)
  /* anonymous unions are enabled by default */
#elif defined (__TASKING__)
  #pragma warning 586
#elif defined (__CSMC__)
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif

/// USB Default Control Pipe Setup Packet
typedef __PACKED_STRUCT _USB_SETUP_PACKET {
  uint8_t           bmRequestType;      /* bmRequestType */
  uint8_t           bRequest;           /* bRequest */
  __PACKED_UNION {
    uint16_t          wValue;           /* wValue */
    __PACKED_STRUCT {
      uint8_t           wValueL;
      uint8_t           wValueH;
    };
  };
  __PACKED_UNION {
    uint16_t          wIndex;           /* wIndex */
    __PACKED_STRUCT {
      uint8_t           wIndexL;
      uint8_t           wIndexH;
    };
  };
  uint16_t          wLength;            /* wLength */
} USB_SETUP_PACKET;


/* =========================================  End of section using anonymous unions  ========================================= */
#if   defined (__CC_ARM)
  #pragma pop
#elif defined (__ICCARM__)
  /* leave anonymous unions enabled */
#elif (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic pop
#elif defined (__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined (__TMS470__)
  /* anonymous unions are enabled by default */
#elif defined (__TASKING__)
  #pragma warning restore
#elif defined (__CSMC__)
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif

/* Exported constants --------------------------------------------------------*/
/* bmRequestType.Direction */
#define USB_REQUEST_DIRECTION_HOST_TO_DEVICE      0x00U
#define USB_REQUEST_DIRECTION_DEVICE_TO_HOST      0x80U
#define USB_REQUEST_DIRECTION_Msk                 0x80U

/* bmRequestType.Type */
#define USB_REQUEST_TYPE_STANDARD            0x00U
#define USB_REQUEST_TYPE_CLASS               0x20U
#define USB_REQUEST_TYPE_VENDOR              0x40U
#define USB_REQUEST_TYPE_Msk                 0x60U

/* bmRequestType.Recipient */
#define USB_REQUEST_RECIPIENT_DEVICE         0x00U
#define USB_REQUEST_RECIPIENT_INTERFACE      0x01U
#define USB_REQUEST_RECIPIENT_ENDPOINT       0x02U
#define USB_REQUEST_RECIPIENT_Msk            0x1FU

/* USB Standard Request Codes */
#define USB_REQUEST_GET_STATUS                0x00U
#define USB_REQUEST_CLEAR_FEATURE             0x01U
#define USB_REQUEST_SET_FEATURE               0x03U
#define USB_REQUEST_SET_ADDRESS               0x05U
#define USB_REQUEST_GET_DESCRIPTOR            0x06U
#define USB_REQUEST_SET_DESCRIPTOR            0x07U
#define USB_REQUEST_GET_CONFIGURATION         0x08U
#define USB_REQUEST_SET_CONFIGURATION         0x09U
#define USB_REQUEST_GET_INTERFACE             0x0AU
#define USB_REQUEST_SET_INTERFACE             0x0BU
#define USB_REQUEST_SYNCH_FRAME               0x0CU

/* USB Standard Feature selectors */
#define USB_FEATURE_ENDPOINT_STALL            0U
#define USB_FEATURE_REMOTE_WAKEUP             1U

/* USB Descriptor Types */
#define USB_DESC_TYPE_DEVICE                           1U
#define USB_DESC_TYPE_CONFIGURATION                    2U
#define USB_DESC_TYPE_STRING                           3U
#define USB_DESC_TYPE_INTERFACE                        4U
#define USB_DESC_TYPE_ENDPOINT                         5U
#define USB_DESC_TYPE_DEVICE_QUALIFIER                 6U
#define USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION        7U
#define USB_DESC_TYPE_INTERFACE_POWER                  8U


/* bmAttributes in Configuration Descriptor */
#define USB_CONFIG_POWERED_MASK                0x40U
#define USB_CONFIG_BUS_POWERED                 0x80U
#define USB_CONFIG_SELF_POWERED                0xC0U
#define USB_CONFIG_REMOTE_WAKEUP               0x20U

/* bMaxPower in Configuration Descriptor */
#define USB_CONFIG_POWER_MA(mA)                   ((mA)/2)

/* bEndpointAddress in Endpoint Descriptor */
#define USB_ENDPOINT_DIRECTION_MASK               0x80U
#define USB_ENDPOINT_OUT(addr)                    (addr)
#define USB_ENDPOINT_IN(addr)                     ((addr) | 0x80U)

/* bmAttributes in Endpoint Descriptor */
#define USB_ENDPOINT_TYPE_MASK                    0x03U
#define USB_ENDPOINT_TYPE_CONTROL                 0x00U
#define USB_ENDPOINT_TYPE_ISOCHRONOUS             0x01U
#define USB_ENDPOINT_TYPE_BULK                    0x02U
#define USB_ENDPOINT_TYPE_INTERRUPT               0x03U
#define USB_ENDPOINT_SYNC_MASK                    0x0CU
#define USB_ENDPOINT_SYNC_NO_SYNCHRONIZATION      0x00U
#define USB_ENDPOINT_SYNC_ASYNCHRONOUS            0x04U
#define USB_ENDPOINT_SYNC_ADAPTIVE                0x08U
#define USB_ENDPOINT_SYNC_SYNCHRONOUS             0x0CU
#define USB_ENDPOINT_USAGE_MASK                   0x30U
#define USB_ENDPOINT_USAGE_DATA                   0x00U
#define USB_ENDPOINT_USAGE_FEEDBACK               0x10U
#define USB_ENDPOINT_USAGE_IMPLICIT_FEEDBACK      0x20U
#define USB_ENDPOINT_USAGE_RESERVED               0x30U

/* Device States */
#define USBD_STATE_DEFAULT                        0
#define USBD_STATE_ADDRESSED                      1
#define USBD_STATE_CONFIGURED                     2
#define USBD_STATE_SUSPENDED                      3

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif /* __USB_DEF_H */

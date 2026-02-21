/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_USER_HID_H
#define __USBD_USER_HID_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "usb_hid_def.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define USBD_HID_IF_NUM       0

extern uint8_t hid_report_buf[4];
extern int hid_report_xfer_flag;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

bool USBD_User_HID_GetReport(void);
bool USBD_User_HID_SetReport(bool data_received);
bool USBD_User_HID_GetIdle(void);
bool USBD_User_HID_SetIdle(void);
bool USBD_User_HID_GetProtocol(void);
bool USBD_User_HID_SetProtocol(void);

#ifdef __cplusplus
}
#endif

#endif /* __USBD_USER_HID_H */

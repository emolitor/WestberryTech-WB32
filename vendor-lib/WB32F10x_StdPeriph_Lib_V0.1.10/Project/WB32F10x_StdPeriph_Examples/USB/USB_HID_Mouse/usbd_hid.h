/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_HID_H
#define __USBD_HID_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "usb_hid_def.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

UsbdControlStage USBD_EndPoint0_Setup_HID_Req(void);
UsbdControlStage USBD_EndPoint0_Out_HID_Req(void);

#ifdef __cplusplus
}
#endif

#endif /* __USBD_HID_H */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CDC_H
#define __USBD_CDC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "usb_cdc_def.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

UsbdControlStage USBD_EndPoint0_Setup_CDC_Req(void);
UsbdControlStage USBD_EndPoint0_Out_CDC_Req(void);

#ifdef __cplusplus
}
#endif

#endif /* __USBD_CDC_H */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_USER_CDC_H
#define __USBD_USER_CDC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "usb_cdc_def.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define USBD_CDC_ACM_CIF_NUM      0
#define USBD_CDC_ACM_DIF_NUM      1

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

bool USBD_User_CDC_SetLineCoding(const USB_CDC_LINE_CODING *line_coding);
bool USBD_User_CDC_GetLineCoding(USB_CDC_LINE_CODING *line_coding);
bool USBD_User_CDC_SetControlLineState(uint16_t control_signal_bitmap);
bool USBD_User_CDC_SendBreak(uint16_t duration_of_break);

#ifdef __cplusplus
}
#endif

#endif /* __USBD_USER_CDC_H */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_MSC_H
#define __USBD_MSC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "usb_msc_def.h"
#include "usbd_core.h"
#include "usbd_user_msc.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  USB_MSC_CBW  cbw;
  USB_MSC_CSW  csw;
  uint8_t      bot_state;
  uint8_t      bulk_buf[64];
} usbd_msc_t;


/* Exported constants --------------------------------------------------------*/
#define MSC_BS_IDLE           0
#define MSC_BS_DATA_OUT       1
#define MSC_BS_DATA_IN        2
#define MSC_BS_DATA_IN_LAST   3
#define MSC_BS_STATUS         4
#define MSC_BS_ERROR          5

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

UsbdControlStage USBD_EndPoint0_Setup_MSC_Req(void);

void USBD_MSC_Init(void);
void USBD_MSC_DeInit(void);
void USBD_MSC_Bulk_Out(void);
void USBD_MSC_Bulk_In(void);

#ifdef __cplusplus
}
#endif

#endif /* __USBD_MSC_H */

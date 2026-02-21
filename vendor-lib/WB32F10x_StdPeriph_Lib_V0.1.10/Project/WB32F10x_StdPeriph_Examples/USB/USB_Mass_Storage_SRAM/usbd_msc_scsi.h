/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_MSC_SCSI_H
#define __USBD_MSC_SCSI_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "usbd_msc.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void USBD_MSC_SCSI_SenseDataUpdate(uint8_t lun, uint8_t SenseKey, uint8_t ASC);

void USBD_MSC_SCSI_TestUnitReady(usbd_msc_t* msc);
void USBD_MSC_SCSI_RequestSense(usbd_msc_t* msc);
void USBD_MSC_SCSI_Inquiry(usbd_msc_t* msc);
void USBD_MSC_SCSI_ReadFormatCapacities(usbd_msc_t* msc);
void USBD_MSC_SCSI_ReadCapacity10(usbd_msc_t* msc);
void USBD_MSC_SCSI_Read10(usbd_msc_t* msc);
void USBD_MSC_SCSI_Write10(usbd_msc_t* msc);
void USBD_MSC_SCSI_ProcessWrite(usbd_msc_t* msc, const uint8_t* buf, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /* __USBD_MSC_SCSI_H */

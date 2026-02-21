/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_USER_MSC_H
#define __USBD_USER_MSC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define USBD_USER_MSC_IF_NUM    0

#define USBD_USER_MSC_MAXLUN    0

#define USBD_USER_MSC_BULK_IN_EP    0x81
#define USBD_USER_MSC_BULK_OUT_EP   0x01

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/


const uint8_t* USBD_User_MSC_GetInquiryData(uint8_t lun, uint32_t* inquiry_data_len);
int8_t USBD_User_MSC_GetCapacity(uint8_t lun, uint32_t *block_count, uint32_t *block_size);

uint32_t USBD_User_MSC_Read(uint8_t lun, uint32_t addr, uint8_t *buf, uint16_t size);
int8_t USBD_User_MSC_PrepareWrite(uint8_t lun, uint32_t lba, uint32_t cnt);
uint32_t USBD_User_MSC_Write(uint8_t lun, const uint8_t *pdata, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif /* __USBD_USER_MSC_H */

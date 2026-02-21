/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_MAX1617A_H
#define __DRV_MAX1617A_H

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void max1617a_init(void);
uint32_t max1617a_smbus_read_byte(uint8_t command, uint8_t* pdata);
uint32_t max1617a_smbus_write_byte(uint8_t command, uint8_t data);
uint32_t max1617a_smbus_alert_response_address(uint8_t* pdata);

#endif /* __DRV_MAX1617A_H */

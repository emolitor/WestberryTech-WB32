/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_EEPROM_24C02_H
#define __DRV_EEPROM_24C02_H

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void eeprom_24c02_init(void);
uint32_t eeprom_24c02_random_read(uint8_t addr, uint8_t* pdata);
uint32_t eeprom_24c02_sequential_read(uint8_t addr, uint8_t* pbuf, uint32_t len);
uint32_t eeprom_24c02_byte_write(uint8_t addr, uint8_t data);
uint32_t eeprom_24c02_page_write(uint8_t addr, const uint8_t* pdata, uint8_t len);
uint32_t eeprom_24c02_probe(void);

#endif /* __DRV_EEPROM_24C02_H */

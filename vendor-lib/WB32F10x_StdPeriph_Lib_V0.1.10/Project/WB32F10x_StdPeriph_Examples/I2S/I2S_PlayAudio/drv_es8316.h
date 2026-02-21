#ifndef __DRV_ES8316_H__
#define __DRV_ES8316_H__

/* includes. */
#include "wb32f10x.h"


#define ES8316_SLAVE_ADDRESS  0x20



uint32_t es8316_init(void);
uint32_t es8316_probe(void);

uint32_t es8316_write_reg(uint8_t reg, uint8_t data);
uint32_t es8316_read_reg(uint8_t reg, uint8_t* pdata);

void es8316_config_play_path(void);
void es8316_config_record_path(void);


#endif /* __DRV_ES8316_H__ */

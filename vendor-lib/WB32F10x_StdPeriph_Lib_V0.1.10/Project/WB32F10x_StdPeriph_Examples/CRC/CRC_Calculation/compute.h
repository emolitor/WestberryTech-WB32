/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMPUTE_H__
#define __COMPUTE_H__

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
extern uint32_t result1;
extern uint32_t result2;
extern uint32_t result3;
extern uint32_t result4;

/* Exported macro ------------------------------------------------------------*/
#ifndef _countof
#define _countof(_Array)  (sizeof(_Array) / sizeof(_Array[0]))
#endif

/* Exported functions --------------------------------------------------------*/

uint8_t computeBytes(void);
uint8_t computeHalfWords(void);
uint8_t computeWords(void);


#endif /* __COMPUTE_H__ */

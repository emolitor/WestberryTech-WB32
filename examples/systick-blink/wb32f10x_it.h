/**
  * @file    Project/WB32F10x_StdPeriph_Template/wb32f10x_it.h
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   This file contains the headers of the interrupt handlers.
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WB32F10x_IT_H
#define __WB32F10x_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
//void SysTick_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __WB32F10x_IT_H */

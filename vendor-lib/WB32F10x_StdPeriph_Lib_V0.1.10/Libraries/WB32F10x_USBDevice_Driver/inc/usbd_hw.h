/**
 * @file    usbd_hw.h
 * @author  Westberry Application Team
 * @attention
 * 
 * Copyright (c) 2020 - 2023 Westberry Technology (ChangZhou) Corp., Ltd
 * All rights reserved.
 * 
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, WESTBERRY TECHNOLOGY SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_HW_H
#define __USBD_HW_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void USBD_HW_ReadEP(uint8_t ep_addr, uint8_t* pbuf, uint16_t size);
void USBD_HW_WriteEP(uint8_t ep_addr, const uint8_t* pdata, uint16_t len);
void USBD_HW_Transmit(uint8_t ep_addr, const uint8_t* pdata, uint16_t len);
uint16_t USBD_HW_GetRxDataCount(uint8_t ep_addr);
void USBD_HW_ReadyToReceive(uint8_t ep_addr);
void USBD_HW_SetStallEP(uint8_t ep_addr);
void USBD_HW_ClrStallEP(uint8_t ep_addr);
uint8_t USBD_HW_IsStalled(uint8_t ep_addr);


#ifdef __cplusplus
}
#endif

#endif /* __USBD_HW_H */

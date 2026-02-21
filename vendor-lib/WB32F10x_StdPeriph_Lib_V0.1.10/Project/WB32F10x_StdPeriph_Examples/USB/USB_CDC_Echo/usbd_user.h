/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_USER_H
#define __USBD_USER_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "usbd_core.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void USBD_User_Init(void);
void USBD_User_DeInit(void);
void USBD_User_Connect(void);
void USBD_User_Disconnect(void);

void USBD_User_Reset(void);
void USBD_User_Resume(void);
void USBD_User_Suspend(void);
void USBD_User_SOF(void);

uint8_t* USBD_User_GetDeviceDescriptor(uint16_t* length);
uint8_t* USBD_User_GetConfigDescriptor(uint8_t index, uint16_t* length);
uint8_t* USBD_User_GetStringDescriptor(uint8_t index, uint16_t* length);
bool USBD_User_SetConfig(uint8_t cfgidx);
void USBD_User_ClearConfig(uint8_t cfgidx);

UsbdControlStage USBD_User_EndPoint0_Setup(void);
UsbdControlStage USBD_User_EndPoint0_Out(void);

void USBD_User_EP1_IN(void);
void USBD_User_EP2_IN(void);
void USBD_User_EP3_IN(void);
void USBD_User_EP1_OUT(void);
void USBD_User_EP2_OUT(void);
void USBD_User_EP3_OUT(void);

#ifdef __cplusplus
}
#endif

#endif /* __USBD_USER_H */

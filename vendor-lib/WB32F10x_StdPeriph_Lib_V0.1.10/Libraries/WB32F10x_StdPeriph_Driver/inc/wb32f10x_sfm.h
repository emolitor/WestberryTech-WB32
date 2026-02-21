/**
 * @file    wb32f10x_sfm.h
 * @author  Westberry Application Team
 * @version V0.1.10
 * @date    05-January-2023
 * @brief   This file contains all the functions prototypes for the SFM firmware
 *          library.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WB32F10x_SFM_H
#define __WB32F10x_SFM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/** @addtogroup WB32F10x_StdPeriph_Driver
  * @{
  */

/** @addtogroup SFM
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/** @defgroup SFM_Exported_Constants 
  * @{
  */

/** @defgroup SFM_usb_port_state_detect 
  * @{
  */
#define SFM_USBP_DETECT_SE0       0x0100
#define SFM_USBP_DETECT_JSTAT     0x0200
#define SFM_USBP_DETECT_KSTAT     0x0400
#define SFM_USBP_DETECT_SE1       0x0800
/**
  * @}
  */

/** @defgroup SFM_usb_port_state_detect_flag 
  * @{
  */
#define SFM_USBP_FLAG_SE0         0x01
#define SFM_USBP_FLAG_JSTAT       0x02
#define SFM_USBP_FLAG_KSTAT       0x04
#define SFM_USBP_FLAG_SE1         0x08
/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void CRC_SFM_DeInit(void);
uint32_t SFM_ComputeBit1Number(uint32_t Data);
void SFM_ExpandBits(uint32_t Data, uint32_t* pBuffer, uint32_t ExpandRate);
void SFM_USBPort_DetectConfig(uint32_t SFM_USBP_DETECT, FunctionalState NewState);
FlagStatus SFM_USBPort_GetFlagStatus(uint32_t SFM_USBP_FLAG);
void SFM_USBPort_ClearFlag(uint32_t SFM_USBP_FLAG);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __WB32F10x_SFM_H */

/**
 * @file    wb32f10x_fmc.h
 * @author  Westberry Application Team
 * @version V0.1.10
 * @date    05-January-2023
 * @brief   This file contains all the functions prototypes for the FMC firmware
 *          library.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WB32F10x_FMC_H
#define __WB32F10x_FMC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/** @addtogroup WB32F10x_StdPeriph_Driver
  * @{
  */

/** @addtogroup FMC
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

uint32_t FMC_ClearPageLatch(void);
uint32_t FMC_ErasePage(uint32_t Page_Address);
uint32_t FMC_EraseSector(uint32_t Sector_Address);
uint32_t FMC_EraseBulk(void);
uint32_t FMC_ProgramPage(uint32_t Page_Address);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __WB32F10x_FMC_H */

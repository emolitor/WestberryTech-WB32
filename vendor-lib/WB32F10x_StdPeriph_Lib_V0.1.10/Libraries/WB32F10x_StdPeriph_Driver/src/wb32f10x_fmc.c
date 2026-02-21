/**
 * @file    wb32f10x_fmc.c
 * @author  Westberry Application Team
 * @version V0.1.10
 * @date    05-January-2023
 * @brief   This file provides all the FMC firmware functions.
 */

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x_fmc.h"

/** @addtogroup WB32F10x_StdPeriph_Driver
  * @{
  */

/** @defgroup FMC
  * @brief FMC driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/** @defgroup FMC_Private_Defines
  * @{
  */

// __ASM void FLASH_OP_EXEC_RAM(uint32_t code, uint32_t fmc_base)
// {
//   STR   R0, [R1, #0x00]   // FMC->CON = 0x00800080; # WR=1
//   NOP
// LB_CK
//   LDR   R0, [R1, #0x00]
//   LSLS  R0, R0, #24
//   BMI   LB_CK             // while(FMC->CON & FMC_CON_WR);
//   BX    LR
// }
/* Do not change if it is not necessary */
#define FLASH_OP_RAM_CODE {0xBF006008, 0x06006808, 0x4770D4FC}

/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const uint32_t pre_op_code[] = 
{
  0x4FF0E92D, 0xF8D14935, 0xF3C00200, 0x28023083, 
  0x4833D861, 0x62822203, 0x62C2220C, 0xC0C4F8DF, 
  0xF8CC2200, 0x46102000, 0x767AF44F, 0x42B01C40, 
  0x2301DBFC, 0x3000F8CC, 0x1C402000, 0xDBFC42B0, 
  0x68044829, 0x0480F044, 0x48276004, 0x6804302C, 
  0x6420F444, 0x469A6004, 0x4B234618, 0x6018333C, 
  0x46104691, 0x60104A21, 0x0340F8D1, 0xF3C02500, 
  0xF04F5107, 0x1F173BFF, 0x465C2200, 0x683B4610, 
  0x78C0EA5F, 0x42A3D106, 0x2401D101, 0x2400E000, 
  0x461C4422, 0x28141C40, 0x2A02DBF1, 0x2910D91B, 
  0x3910D901, 0x2100E000, 0x1F00480E, 0xF0226802, 
  0xEA42627F, 0x60025201, 0x9000F8CC, 0x1C402000, 
  0xDBFC42B0, 0xA000F8CC, 0x1C402000, 0xDBFC42B0, 
  0x2D051C6D, 0xE8BDDBD0, 0x00008FF0, 0x1FFFF000, 
  0x40010000, 0x40010438, 0x40010C20, 0x4000B804
};
#define PRE_OP()  ((void(*)(void))((unsigned int)(pre_op_code) | 0x01))()

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup FMC_Private_Functions
  * @{
  */

/**
 * @brief  Execute FLASH operation.
 * @return 0 - OK,  1 - Failed
 * @note   Before calling this function, FHSI must be enabled.
 */
static uint32_t FLASH_OP_EXEC(uint32_t op)
{
  volatile uint32_t flash_op_ram_code[] = FLASH_OP_RAM_CODE;

  /* Start FLASH operation and wait for a FLASH operation to complete  */
  RCC->PCLKENR = 0x01;
  FMC->CON = 0x7F5F0D40 | (op & 0x1F);    /* [14:8]=0x0D, WREN=1, [4:0]=op */
  FMC->KEY = 0x5188DA08;
  FMC->KEY = 0x12586590;
  ((void(*)(uint32_t, uint32_t))((unsigned int)(flash_op_ram_code) | 0x01))(0x00800080, FMC_BASE);

  RCC->PCLKENR = 0x00;
  /* Clear WREN and OP[4:0] bits */
  FMC->CON = 0x005F0000;

  if (FMC->STAT & FMC_STAT_ERR)
    return 1;   /* Any error occur */
  else
    return 0;   /* FLASH operation complete */
}

/**
 * @brief  Clear page latches.
 * @return 0 - OK,  1 - Failed
 * @note   Before calling this function, FHSI must be enabled.
 */
uint32_t FMC_ClearPageLatch(void)
{
  uint32_t ret;
  int state;
  state = __get_PRIMASK();
  __disable_irq();
  ret = FLASH_OP_EXEC(0x04);
  if (!state) {
    __enable_irq();
  }
  return ret;
}

/**
 * @brief  Erase a specified FLASH page.
 * @param  Page_Address: The page address to be erased.
 * @return 0 - OK,  1 - Failed
 * @note   Before calling this function, FHSI must be enabled.
 */
uint32_t FMC_ErasePage(uint32_t Page_Address)
{
  uint32_t ret;
  int state;
  state = __get_PRIMASK();
  __disable_irq();
  PRE_OP();
  FMC->ADDR = Page_Address;
  ret = FLASH_OP_EXEC(0x08);
  if (!state) {
    __enable_irq();
  }
  return ret;
}

/**
 * @brief  Erase a specified FLASH sector.
 * @param  Sector_Address: The sector address to be erased.
 * @return 0 - OK,  1 - Failed
 * @note   Before calling this function, FHSI must be enabled.
 */
uint32_t FMC_EraseSector(uint32_t Sector_Address)
{
  uint32_t ret;
  int state;
  state = __get_PRIMASK();
  __disable_irq();
  PRE_OP();
  FMC->ADDR = Sector_Address;
  ret = FLASH_OP_EXEC(0x09);
  if (!state) {
    __enable_irq();
  }
  return ret;
}

/**
 * @brief  Erase main flash memory.
 * @return 0 - OK,  1 - Failed
 * @note   Before calling this function, FHSI must be enabled.
 */
uint32_t FMC_EraseBulk(void)
{
  uint32_t ret;
  int state;
  state = __get_PRIMASK();
  __disable_irq();
  PRE_OP();
  FMC->ADDR = 0x08000000;
  ret = FLASH_OP_EXEC(0x1B);
  if (!state) {
    __enable_irq();
  }
  return ret;
}

/**
 * @brief  Programs the data to the specified page address.
 * @param  Page_Address: The page address to be programmed.
 * @return 0 - OK,  1 - Failed
 * @note   Before calling this function, FHSI must be enabled.
 */
uint32_t FMC_ProgramPage(uint32_t Page_Address)
{
  uint32_t ret;
  int state;
  state = __get_PRIMASK();
  __disable_irq();
  PRE_OP();
  FMC->ADDR = Page_Address;
  ret = FLASH_OP_EXEC(0x0C);
  if (!state) {
    __enable_irq();
  }
  return ret;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

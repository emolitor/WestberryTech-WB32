/**
 * @file     system_wb32f10x.c
 * @brief    CMSIS Device System Source File for
 *           WB32F10x Device Series
 * @version  V0.1.10
 * @date     05-January-2023
 */


#include "wb32f10x.h"

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/

/*!< Uncomment the line corresponding to the desired Main clock (MAINCLK)
   frequency (after reset, the MHSI is used as MAINCLK source)

   Tip: To avoid modifying this file each time you need to use different Main clock,
       you can define the MAINCLK_FREQ_x in your toolchain compiler preprocessor.

   IMPORTANT NOTE:
   ============== 
   1. After reset the MHSI is used as Main clock source.

   2. If none of the following definitions is enabled, the MHSI is used as 
      main clock source.

   3. Please make sure that the selected Main clock doesn't exceed your device's
      maximum frequency.

   4. If the external crystal is used, the HSE_VALUE macro (defined in wb32f10x.h)
      must be defined as the frequency value of the external crystal.

   5. In the Main clock configuration functions provided within this file which 
      enabled by MAINCLK_FREQ_xMHz macro:
        - The PLL clock is used as clock source of the Main Clock.
        - When the MAINCLK_PLLSRC_MHSI macro is defined, MHSI is used as PLL clock source,
          otherwise HSE is used as PLL clock source.
        - If HSE is used as PLL clock source, the desired clock frequency is configured
          depending on the external crystal frequency defined by HSE_VALUE.
 */

/* #define MAINCLK_FREQ_HSE    */
/* #define MAINCLK_FREQ_FHSI   */
/* #define MAINCLK_FREQ_32MHz  */   /* The HSE clock frequency must be 6MHz/8MHz/12MHz/16MHz */
/* #define MAINCLK_FREQ_48MHz  */   /* The HSE clock frequency must be 6MHz/8MHz/12MHz/16MHz */
/* #define MAINCLK_FREQ_64MHz  */   /* The HSE clock frequency must be 8MHz/12MHz/16MHz */
/* #define MAINCLK_FREQ_72MHz  */   /* The HSE clock frequency must be 6MHz/12MHz */
/* #define MAINCLK_FREQ_96MHz  */   /* The HSE clock frequency must be 6MHz/8MHz/12MHz/16MHz */
/* #define MAINCLK_FREQ_128MHz */   /* The HSE clock frequency must be 8MHz/16MHz */

#if ((defined(MAINCLK_FREQ_HSE) + defined(MAINCLK_FREQ_FHSI) + defined(MAINCLK_FREQ_32MHz) + defined(MAINCLK_FREQ_48MHz) + defined(MAINCLK_FREQ_64MHz) + defined(MAINCLK_FREQ_72MHz) + defined(MAINCLK_FREQ_96MHz) + defined(MAINCLK_FREQ_128MHz)) > 1)
  #error "Only one MAINCLK_FREQ_xx macro can be defined!"
#endif

/* #define MAINCLK_PLLSRC_MHSI */


/*!< Uncomment the following line if you need to relocate your vector Table in
     Internal SRAM. */ 
// #define VECT_TAB_SRAM
#ifndef VECT_TAB_OFFSET
#define VECT_TAB_OFFSET  0x0    /*!< Vector Table base offset field. 
                                     This value must be a multiple of 0x100. */
#endif


#if defined(MAINCLK_FREQ_32MHz) && (HSE_VALUE != 6000000) && (HSE_VALUE != 8000000) && (HSE_VALUE != 12000000) && (HSE_VALUE != 16000000)
  #error "The MAINCLK_FREQ_32MHz macro can only be defined if the external crystal is 6MHz/8MHz/12MHz/16MHz!"
#endif

#if defined(MAINCLK_FREQ_48MHz) && (HSE_VALUE != 6000000) && (HSE_VALUE != 8000000) && (HSE_VALUE != 12000000) && (HSE_VALUE != 16000000)
  #error "The MAINCLK_FREQ_48MHz macro can only be defined if the external crystal is 6MHz/8MHz/12MHz/16MHz!"
#endif

#if defined(MAINCLK_FREQ_64MHz) && (HSE_VALUE != 8000000) && (HSE_VALUE != 12000000) && (HSE_VALUE != 16000000)
  #error "The MAINCLK_FREQ_64MHz macro can only be defined if the external crystal is 8MHz/12MHz/16MHz!"
#endif

#if defined(MAINCLK_FREQ_72MHz) && (HSE_VALUE != 6000000) && (HSE_VALUE != 12000000)
  #error "The MAINCLK_FREQ_72MHz macro can only be defined if the external crystal is 6MHz/12MHz!"
#endif

#if defined(MAINCLK_FREQ_72MHz) && defined(MAINCLK_PLLSRC_MHSI)
  #error "MHSI cannot make PLL frequency to 72MHz! The MAINCLK_FREQ_72MHz macro can only be defined if the external crystal is 6MHz/12MHz!"
#endif

#if defined(MAINCLK_FREQ_96MHz) && (HSE_VALUE != 6000000) && (HSE_VALUE != 8000000) && (HSE_VALUE != 12000000) && (HSE_VALUE != 16000000)
  #error "The MAINCLK_FREQ_96MHz macro can only be defined if the external crystal is 6MHz/8MHz/12MHz/16MHz!"
#endif

#if defined(MAINCLK_FREQ_128MHz) && (HSE_VALUE != 8000000) && (HSE_VALUE != 16000000)
  #error "The MAINCLK_FREQ_128MHz macro can only be defined if the external crystal is 8MHz/16MHz!"
#endif

/*----------------------------------------------------------------------------
  System Core Clock Variable
 *----------------------------------------------------------------------------*/
#ifdef MAINCLK_FREQ_HSE
  uint32_t SystemCoreClock = HSE_VALUE;       /*!< System Clock Frequency (Core Clock) */
#elif defined MAINCLK_FREQ_FHSI
  uint32_t SystemCoreClock = FHSI_VALUE;      /*!< System Clock Frequency (Core Clock) */
#elif defined MAINCLK_FREQ_32MHz
  uint32_t SystemCoreClock = 32000000;        /*!< System Clock Frequency (Core Clock) */
#elif defined MAINCLK_FREQ_48MHz
  uint32_t SystemCoreClock = 48000000;        /*!< System Clock Frequency (Core Clock) */
#elif defined MAINCLK_FREQ_64MHz
  uint32_t SystemCoreClock = 64000000;        /*!< System Clock Frequency (Core Clock) */
#elif defined MAINCLK_FREQ_72MHz
  uint32_t SystemCoreClock = 72000000;        /*!< System Clock Frequency (Core Clock) */
#elif defined MAINCLK_FREQ_96MHz
  uint32_t SystemCoreClock = 96000000;        /*!< System Clock Frequency (Core Clock) */
#elif defined MAINCLK_FREQ_128MHz
  uint32_t SystemCoreClock = 128000000;       /*!< System Clock Frequency (Core Clock) */
#else
  uint32_t SystemCoreClock = MHSI_VALUE;      /*!< System Clock Frequency (Core Clock) */
#endif


static void SetSysClock(void);

#ifdef MAINCLK_FREQ_HSE
  static void SetSysClockToHSE(void);
#elif defined MAINCLK_FREQ_FHSI
  static void SetSysClockToFHSI(void);
#elif defined MAINCLK_FREQ_32MHz
  static void SetSysClockTo32(void);
#elif defined MAINCLK_FREQ_48MHz
  static void SetSysClockTo48(void);
#elif defined MAINCLK_FREQ_64MHz
  static void SetSysClockTo64(void);
#elif defined MAINCLK_FREQ_72MHz
  static void SetSysClockTo72(void);
#elif defined MAINCLK_FREQ_96MHz
  static void SetSysClockTo96(void);
#elif defined MAINCLK_FREQ_128MHz
  static void SetSysClockTo128(void);
#endif


/**
 * @brief  Setup the microcontroller system
 * @note   This function should be used only after reset.
 * @param  None
 * @return None
 */
void SystemInit (void)
{
  /* Unlocks write to ANCTL registers */
  PWR->ANAKEY1 = 0x03;
  PWR->ANAKEY2 = 0x0C;

  /* Turn off POR */
  ANCTL->PORCR = 0x7BE;

  /* Locks write to ANCTL registers */
  PWR->ANAKEY1 = 0x00;
  PWR->ANAKEY2 = 0x00;

  SetSysClock();

#ifdef VECT_TAB_SRAM
  SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM. */
#else
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH. */
#endif
}

/**
 * @brief  Update SystemCoreClock variable according to Clock Register Values.
 *         The SystemCoreClock variable contains the core clock (HCLK), it can
 *         be used by the user application to setup the SysTick timer or configure
 *         other parameters.
 * @note   Each time the core clock (HCLK) changes, this function must be called
 *         to update SystemCoreClock variable value. Otherwise, any configuration
 *         based on this variable will be incorrect.   
 * @param  None
 * @return None
 */
void SystemCoreClockUpdate (void)
{
  uint32_t ahbprediv, pllprediv, pllmul, mainclk;

  switch (RCC->MAINCLKSRC)
  {
    case 0x00:  /* MHSI used as main clock */
      mainclk = MHSI_VALUE;
      break;
    case 0x01:  /* FHSI used as main clock */
      mainclk = FHSI_VALUE;
      break;
    case 0x03:  /* HSE used as main clock */
      mainclk = HSE_VALUE;
      break;
    case 0x02:  /* PLL used as main clock */
      pllprediv = (((RCC->PLLPRE & (RCC_PLLPRE_RATIO_Msk | RCC_PLLPRE_DIVEN)) + 1) >> 1) + 1;
      pllmul = (0x03 - ((ANCTL->PLLCR >> 6) & 0x03)) * 4 + 12;
      if (RCC->PLLSRC == RCC_PLLSRC_HSE)
      {
        mainclk = HSE_VALUE * pllmul / pllprediv;
      }
      else
      {
        mainclk = MHSI_VALUE * pllmul / pllprediv;
      }
      break;
    default:
      mainclk = MHSI_VALUE;
      break;
  }

  ahbprediv = (((RCC->AHBPRE & (RCC_AHBPRE_RATIO_Msk | RCC_AHBPRE_DIVEN)) + 1) >> 1) + 1;
  SystemCoreClock = mainclk / ahbprediv;
}

/**
 * @brief  Configures the main clock frequency, AHBCLK, APB1CLK and APB2CLK prescalers.
 * @note   This function should be used only after reset.
 * @param  None
 * @return None
 */
static void SetSysClock(void)
{
#ifdef MAINCLK_FREQ_HSE
  SetSysClockToHSE();
#elif defined MAINCLK_FREQ_FHSI
  SetSysClockToFHSI();
#elif defined MAINCLK_FREQ_32MHz
  SetSysClockTo32();
#elif defined MAINCLK_FREQ_48MHz
  SetSysClockTo48();
#elif defined MAINCLK_FREQ_64MHz
  SetSysClockTo64();
#elif defined MAINCLK_FREQ_72MHz
  SetSysClockTo72();
#elif defined MAINCLK_FREQ_96MHz
  SetSysClockTo96();
#elif defined MAINCLK_FREQ_128MHz
  SetSysClockTo128();
#else
  /* If none of the above definitions is enabled, the MHSI is used as 
     main clock source (this is default after reset) */

  /* APB1CLK = MAINCLK */
  RCC->APB1PRE = RCC_APB1PRE_SRCEN;
  RCC->APB1PRE |= 0x00;

  /* APB2CLK = MAINCLK */
  RCC->APB2PRE = RCC_APB2PRE_SRCEN;
  RCC->APB2PRE |= 0x00;
#endif
}


#ifdef MAINCLK_FREQ_HSE
/**
 * @brief  Selects HSE as main clock source and configure AHBCLK, APB1CLK
 *         and APB2CLK prescalers.
 * @note   This function should be used only after reset.
 * @param  None
 * @return None
 */
static void SetSysClockToHSE(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

  /* Unlocks write to ANCTL registers */
  PWR->ANAKEY1 = 0x03;
  PWR->ANAKEY2 = 0x0C;

  /* APB1CLK = MAINCLK */
  RCC->APB1PRE = RCC_APB1PRE_SRCEN;
  RCC->APB1PRE |= 0x00;

  /* Configure PD0 and PD1 to analog mode */
  RCC->APB1ENR = RCC_APB1ENR_BMX1EN | RCC_APB1ENR_GPIODEN;
  GPIOD->CFGMSK = 0xFFFC;
  GPIOD->MODER = 0x0F;

  /* Enable HSE */
  ANCTL->HSECR1 = ANCTL_HSECR1_PADOEN;
  ANCTL->HSECR0 = ANCTL_HSECR0_HSEON;

  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = ANCTL->HSESR & ANCTL_HSESR_HSERDY;
    StartUpCounter++;
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

  if (HSEStatus != 0)
  {
    /* Configure wait state */
    CACHE->CR = CACHE_CR_LATENCY_0WS;

    /* AHBCLK = MAINCLK */
    RCC->AHBPRE = 0x00;

    /* APB2CLK = MAINCLK */
    RCC->APB2PRE = RCC_APB2PRE_SRCEN;
    RCC->APB2PRE |= 0x00;

    /* Select HSE as system clock source */
    RCC->MAINCLKSRC = RCC_MAINCLKSRC_HSE;
    RCC->MAINCLKUEN = RCC_MAINCLKUEN_ENA;
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
    while (1);
  }

  /* Locks write to ANCTL registers */
  PWR->ANAKEY1 = 0x00;
  PWR->ANAKEY2 = 0x00;
}

#elif defined MAINCLK_FREQ_FHSI
/**
 * @brief  Selects FHSI as main clock source and configure AHBCLK, APB1CLK
 *         and APB2CLK prescalers.
 * @note   This function should be used only after reset.
 * @param  None
 * @return None
 */
static void SetSysClockToFHSI(void)
{
  /* Unlocks write to ANCTL registers */
  PWR->ANAKEY1 = 0x03;
  PWR->ANAKEY2 = 0x0C;

  /* Configure Flash prefetch, Cache and wait state */
  CACHE->CR = CACHE_CR_CHEEN | CACHE_CR_PREFEN_ON | CACHE_CR_LATENCY_1WS;

  /* Enable FHSI and wait it for ready */
  ANCTL->FHSIENR = ANCTL_FHSIENR_FHSION;
  while (!(ANCTL->FHSISR & ANCTL_FHSISR_FHSIRDY));

  /* AHBCLK = MAINCLK */
  RCC->AHBPRE = 0x00;

  /* APB1CLK = MAINCLK */
  RCC->APB1PRE = RCC_APB1PRE_SRCEN;
  RCC->APB1PRE |= 0x00;

  /* APB2CLK = MAINCLK */
  RCC->APB2PRE = RCC_APB2PRE_SRCEN;
  RCC->APB2PRE |= 0x00;

  /* Select FHSI as system clock source */
  RCC->MAINCLKSRC = RCC_MAINCLKSRC_FHSI;
  RCC->MAINCLKUEN = RCC_MAINCLKUEN_ENA;

  /* Locks write to ANCTL registers */
  PWR->ANAKEY1 = 0x00;
  PWR->ANAKEY2 = 0x00;
}

#elif defined MAINCLK_FREQ_32MHz
#ifdef MAINCLK_PLLSRC_MHSI
/**
 * @brief  Sets main clock frequency to 32MHz and configure AHBCLK, APB1CLK 
 *         and APB2CLK prescalers.
 * @note   This function should be used only after reset.
 * @param  None
 * @return None
 */
static void SetSysClockTo32(void)
{
  /* Unlocks write to ANCTL registers */
  PWR->ANAKEY1 = 0x03;
  PWR->ANAKEY2 = 0x0C;

  /* APB1CLK = MAINCLK */
  RCC->APB1PRE = RCC_APB1PRE_SRCEN;
  RCC->APB1PRE |= 0x00;

  /* Configure Flash prefetch, Cache and wait state */
  CACHE->CR = CACHE_CR_LATENCY_0WS;

  /* AHBCLK = MAINCLK */
  RCC->AHBPRE = 0x00;

  /* APB2CLK = MAINCLK */
  RCC->APB2PRE = RCC_APB2PRE_SRCEN;
  RCC->APB2PRE |= 0x00;

  /* PLL configuration: PLLCLK = MHSI(8MHz) / 4 * 16 = 32 MHz */
  RCC->PLLSRC = RCC_PLLSRC_MHSI;
  RCC->PLLPRE = RCC_PLLPRE_SRCEN;
  RCC->PLLPRE |= RCC_PLLPRE_RATIO_4;
  RCC->PLLPRE |= RCC_PLLPRE_DIVEN;
  ANCTL->PLLCR = ANCTL_PLLCR_PLLMUL_16;

  /* Enable PLL */
  ANCTL->PLLENR = ANCTL_PLLENR_PLLON;

  /* Wait till PLL is ready */
  while(ANCTL->PLLSR != 0x03)
  {
  }

  /* Select PLL as system clock source */
  RCC->MAINCLKSRC = RCC_MAINCLKSRC_PLLCLK;
  RCC->MAINCLKUEN = RCC_MAINCLKUEN_ENA;

  /* Locks write to ANCTL registers */
  PWR->ANAKEY1 = 0x00;
  PWR->ANAKEY2 = 0x00;
}
#else /* MAINCLK_PLLSRC_MHSI */
/**
 * @brief  Sets main clock frequency to 32MHz and configure AHBCLK, APB1CLK 
 *         and APB2CLK prescalers.
 * @note   This function should be used only after reset.
 * @param  None
 * @return None
 */
static void SetSysClockTo32(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

  /* Unlocks write to ANCTL registers */
  PWR->ANAKEY1 = 0x03;
  PWR->ANAKEY2 = 0x0C;

  /* APB1CLK = MAINCLK */
  RCC->APB1PRE = RCC_APB1PRE_SRCEN;
  RCC->APB1PRE |= 0x00;

  /* Configure PD0 and PD1 to analog mode */
  RCC->APB1ENR = RCC_APB1ENR_BMX1EN | RCC_APB1ENR_GPIODEN;
  GPIOD->CFGMSK = 0xFFFC;
  GPIOD->MODER = 0x0F;

  /* Enable HSE */
  ANCTL->HSECR1 = ANCTL_HSECR1_PADOEN;
  ANCTL->HSECR0 = ANCTL_HSECR0_HSEON;

  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = ANCTL->HSESR & ANCTL_HSESR_HSERDY;
    StartUpCounter++;
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

  if (HSEStatus != 0)
  {
    /* Configure Flash prefetch, Cache and wait state */
    CACHE->CR = CACHE_CR_LATENCY_0WS;

    /* AHBCLK = MAINCLK */
    RCC->AHBPRE = 0x00;

    /* APB2CLK = MAINCLK */
    RCC->APB2PRE = RCC_APB2PRE_SRCEN;
    RCC->APB2PRE |= 0x00;

#if (HSE_VALUE == 6000000)
    /* PLL configuration: PLLCLK = 6MHz / 3 * 16 = 32 MHz */
    RCC->PLLSRC = RCC_PLLSRC_HSE;
    RCC->PLLPRE = RCC_PLLPRE_SRCEN;
    RCC->PLLPRE |= RCC_PLLPRE_RATIO_3;
    RCC->PLLPRE |= RCC_PLLPRE_DIVEN;
    ANCTL->PLLCR = ANCTL_PLLCR_PLLMUL_16;
#elif (HSE_VALUE == 8000000)
    /* PLL configuration: PLLCLK = 8MHz / 4 * 16 = 32 MHz */
    RCC->PLLSRC = RCC_PLLSRC_HSE;
    RCC->PLLPRE = RCC_PLLPRE_SRCEN;
    RCC->PLLPRE |= RCC_PLLPRE_RATIO_4;
    RCC->PLLPRE |= RCC_PLLPRE_DIVEN;
    ANCTL->PLLCR = ANCTL_PLLCR_PLLMUL_16;
#elif (HSE_VALUE == 12000000)
    /* PLL configuration: PLLCLK = 12MHz / 6 * 16 = 32 MHz */
    RCC->PLLSRC = RCC_PLLSRC_HSE;
    RCC->PLLPRE = RCC_PLLPRE_SRCEN;
    RCC->PLLPRE |= RCC_PLLPRE_RATIO_6;
    RCC->PLLPRE |= RCC_PLLPRE_DIVEN;
    ANCTL->PLLCR = ANCTL_PLLCR_PLLMUL_16;
#elif (HSE_VALUE == 16000000)
    /* PLL configuration: PLLCLK = 16MHz / 6 * 12 = 32 MHz */
    RCC->PLLSRC = RCC_PLLSRC_HSE;
    RCC->PLLPRE = RCC_PLLPRE_SRCEN;
    RCC->PLLPRE |= RCC_PLLPRE_RATIO_6;
    RCC->PLLPRE |= RCC_PLLPRE_DIVEN;
    ANCTL->PLLCR = ANCTL_PLLCR_PLLMUL_12;
#endif

    /* Enable PLL */
    ANCTL->PLLENR = ANCTL_PLLENR_PLLON;

    /* Wait till PLL is ready */
    while(ANCTL->PLLSR != 0x03)
    {
    }

    /* Select PLL as system clock source */
    RCC->MAINCLKSRC = RCC_MAINCLKSRC_PLLCLK;
    RCC->MAINCLKUEN = RCC_MAINCLKUEN_ENA;
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
    while (1);
  }

  /* Locks write to ANCTL registers */
  PWR->ANAKEY1 = 0x00;
  PWR->ANAKEY2 = 0x00;
}
#endif /* MAINCLK_PLLSRC_MHSI */

#elif defined MAINCLK_FREQ_48MHz
#ifdef MAINCLK_PLLSRC_MHSI
/**
 * @brief  Sets main clock frequency to 48MHz and configure AHBCLK, APB1CLK 
 *         and APB2CLK prescalers.
 * @note   This function should be used only after reset.
 * @param  None
 * @return None
 */
static void SetSysClockTo48(void)
{
  /* Unlocks write to ANCTL registers */
  PWR->ANAKEY1 = 0x03;
  PWR->ANAKEY2 = 0x0C;

  /* APB1CLK = MAINCLK */
  RCC->APB1PRE = RCC_APB1PRE_SRCEN;
  RCC->APB1PRE |= 0x00;

  /* Configure Flash prefetch, Cache and wait state */
  CACHE->CR = CACHE_CR_CHEEN | CACHE_CR_PREFEN_ON | CACHE_CR_LATENCY_1WS;

  /* AHBCLK = MAINCLK */
  RCC->AHBPRE = 0x00;

  /* APB2CLK = MAINCLK */
  RCC->APB2PRE = RCC_APB2PRE_SRCEN;
  RCC->APB2PRE |= 0x00;

  /* PLL configuration: PLLCLK = MHSI(8MHz) / 2 * 12 = 48 MHz */
  RCC->PLLSRC = RCC_PLLSRC_MHSI;
  RCC->PLLPRE = RCC_PLLPRE_SRCEN;
  RCC->PLLPRE |= RCC_PLLPRE_RATIO_2;
  RCC->PLLPRE |= RCC_PLLPRE_DIVEN;
  ANCTL->PLLCR = ANCTL_PLLCR_PLLMUL_12;

  /* Enable PLL */
  ANCTL->PLLENR = ANCTL_PLLENR_PLLON;

  /* Wait till PLL is ready */
  while(ANCTL->PLLSR != 0x03)
  {
  }

  /* Select PLL as system clock source */
  RCC->MAINCLKSRC = RCC_MAINCLKSRC_PLLCLK;
  RCC->MAINCLKUEN = RCC_MAINCLKUEN_ENA;

  /* Locks write to ANCTL registers */
  PWR->ANAKEY1 = 0x00;
  PWR->ANAKEY2 = 0x00;
}
#else /* MAINCLK_PLLSRC_MHSI */
/**
 * @brief  Sets main clock frequency to 48MHz and configure AHBCLK, APB1CLK 
 *         and APB2CLK prescalers.
 * @note   This function should be used only after reset.
 * @param  None
 * @return None
 */
static void SetSysClockTo48(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

  /* Unlocks write to ANCTL registers */
  PWR->ANAKEY1 = 0x03;
  PWR->ANAKEY2 = 0x0C;

  /* APB1CLK = MAINCLK */
  RCC->APB1PRE = RCC_APB1PRE_SRCEN;
  RCC->APB1PRE |= 0x00;

  /* Configure PD0 and PD1 to analog mode */
  RCC->APB1ENR = RCC_APB1ENR_BMX1EN | RCC_APB1ENR_GPIODEN;
  GPIOD->CFGMSK = 0xFFFC;
  GPIOD->MODER = 0x0F;

  /* Enable HSE */
  ANCTL->HSECR1 = ANCTL_HSECR1_PADOEN;
  ANCTL->HSECR0 = ANCTL_HSECR0_HSEON;

  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = ANCTL->HSESR & ANCTL_HSESR_HSERDY;
    StartUpCounter++;
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

  if (HSEStatus != 0)
  {
    /* Configure Flash prefetch, Cache and wait state */
    CACHE->CR = CACHE_CR_CHEEN | CACHE_CR_PREFEN_ON | CACHE_CR_LATENCY_1WS;

    /* AHBCLK = MAINCLK */
    RCC->AHBPRE = 0x00;

    /* APB2CLK = MAINCLK */
    RCC->APB2PRE = RCC_APB2PRE_SRCEN;
    RCC->APB2PRE |= 0x00;

#if (HSE_VALUE == 6000000)
    /* PLL configuration: PLLCLK = 6MHz / 2 * 16 = 48 MHz */
    RCC->PLLSRC = RCC_PLLSRC_HSE;
    RCC->PLLPRE = RCC_PLLPRE_SRCEN;
    RCC->PLLPRE |= RCC_PLLPRE_RATIO_2;
    RCC->PLLPRE |= RCC_PLLPRE_DIVEN;
    ANCTL->PLLCR = ANCTL_PLLCR_PLLMUL_16;
#elif (HSE_VALUE == 8000000)
    /* PLL configuration: PLLCLK = 8MHz / 2 * 12 = 48 MHz */
    RCC->PLLSRC = RCC_PLLSRC_HSE;
    RCC->PLLPRE = RCC_PLLPRE_SRCEN;
    RCC->PLLPRE |= RCC_PLLPRE_RATIO_2;
    RCC->PLLPRE |= RCC_PLLPRE_DIVEN;
    ANCTL->PLLCR = ANCTL_PLLCR_PLLMUL_12;
#elif (HSE_VALUE == 12000000)
    /* PLL configuration: PLLCLK = 12MHz / 4 * 16 = 48 MHz */
    RCC->PLLSRC = RCC_PLLSRC_HSE;
    RCC->PLLPRE = RCC_PLLPRE_SRCEN;
    RCC->PLLPRE |= RCC_PLLPRE_RATIO_4;
    RCC->PLLPRE |= RCC_PLLPRE_DIVEN;
    ANCTL->PLLCR = ANCTL_PLLCR_PLLMUL_16;
#elif (HSE_VALUE == 16000000)
    /* PLL configuration: PLLCLK = 16MHz / 4 * 12 = 48 MHz */
    RCC->PLLSRC = RCC_PLLSRC_HSE;
    RCC->PLLPRE = RCC_PLLPRE_SRCEN;
    RCC->PLLPRE |= RCC_PLLPRE_RATIO_4;
    RCC->PLLPRE |= RCC_PLLPRE_DIVEN;
    ANCTL->PLLCR = ANCTL_PLLCR_PLLMUL_12;
#endif

    /* Enable PLL */
    ANCTL->PLLENR = ANCTL_PLLENR_PLLON;

    /* Wait till PLL is ready */
    while(ANCTL->PLLSR != 0x03)
    {
    }

    /* Select PLL as system clock source */
    RCC->MAINCLKSRC = RCC_MAINCLKSRC_PLLCLK;
    RCC->MAINCLKUEN = RCC_MAINCLKUEN_ENA;
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
    while (1);
  }

  /* Locks write to ANCTL registers */
  PWR->ANAKEY1 = 0x00;
  PWR->ANAKEY2 = 0x00;
}
#endif /* MAINCLK_PLLSRC_MHSI */

#elif defined MAINCLK_FREQ_64MHz
#ifdef MAINCLK_PLLSRC_MHSI
/**
 * @brief  Sets main clock frequency to 64MHz and configure AHBCLK, APB1CLK 
 *         and APB2CLK prescalers.
 * @note   This function should be used only after reset.
 * @param  None
 * @return None
 */
static void SetSysClockTo64(void)
{
  /* Unlocks write to ANCTL registers */
  PWR->ANAKEY1 = 0x03;
  PWR->ANAKEY2 = 0x0C;

  /* APB1CLK = MAINCLK */
  RCC->APB1PRE = RCC_APB1PRE_SRCEN;
  RCC->APB1PRE |= 0x00;

  /* Configure Flash prefetch, Cache and wait state */
  CACHE->CR = CACHE_CR_CHEEN | CACHE_CR_PREFEN_ON | CACHE_CR_LATENCY_2WS;

  /* AHBCLK = MAINCLK */
  RCC->AHBPRE = 0x00;

  /* APB2CLK = MAINCLK */
  RCC->APB2PRE = RCC_APB2PRE_SRCEN;
  RCC->APB2PRE |= 0x00;

  /* PLL configuration: PLLCLK = MHSI(8MHz) / 2 * 16 = 64 MHz */
  RCC->PLLSRC = RCC_PLLSRC_MHSI;
  RCC->PLLPRE = RCC_PLLPRE_SRCEN;
  RCC->PLLPRE |= RCC_PLLPRE_RATIO_2;
  RCC->PLLPRE |= RCC_PLLPRE_DIVEN;
  ANCTL->PLLCR = ANCTL_PLLCR_PLLMUL_16;

  /* Enable PLL */
  ANCTL->PLLENR = ANCTL_PLLENR_PLLON;

  /* Wait till PLL is ready */
  while(ANCTL->PLLSR != 0x03)
  {
  }

  /* Select PLL as system clock source */
  RCC->MAINCLKSRC = RCC_MAINCLKSRC_PLLCLK;
  RCC->MAINCLKUEN = RCC_MAINCLKUEN_ENA;

  /* Locks write to ANCTL registers */
  PWR->ANAKEY1 = 0x00;
  PWR->ANAKEY2 = 0x00;
}
#else /* MAINCLK_PLLSRC_MHSI */
/**
 * @brief  Sets main clock frequency to 64MHz and configure AHBCLK, APB1CLK 
 *         and APB2CLK prescalers.
 * @note   This function should be used only after reset.
 * @param  None
 * @return None
 */
static void SetSysClockTo64(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

  /* Unlocks write to ANCTL registers */
  PWR->ANAKEY1 = 0x03;
  PWR->ANAKEY2 = 0x0C;

  /* APB1CLK = MAINCLK */
  RCC->APB1PRE = RCC_APB1PRE_SRCEN;
  RCC->APB1PRE |= 0x00;

  /* Configure PD0 and PD1 to analog mode */
  RCC->APB1ENR = RCC_APB1ENR_BMX1EN | RCC_APB1ENR_GPIODEN;
  GPIOD->CFGMSK = 0xFFFC;
  GPIOD->MODER = 0x0F;

  /* Enable HSE */
  ANCTL->HSECR1 = ANCTL_HSECR1_PADOEN;
  ANCTL->HSECR0 = ANCTL_HSECR0_HSEON;

  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = ANCTL->HSESR & ANCTL_HSESR_HSERDY;
    StartUpCounter++;
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

  if (HSEStatus != 0)
  {
    /* Configure Flash prefetch, Cache and wait state */
    CACHE->CR = CACHE_CR_CHEEN | CACHE_CR_PREFEN_ON | CACHE_CR_LATENCY_2WS;

    /* AHBCLK = MAINCLK */
    RCC->AHBPRE = 0x00;

    /* APB2CLK = MAINCLK */
    RCC->APB2PRE = RCC_APB2PRE_SRCEN;
    RCC->APB2PRE |= 0x00;

#if (HSE_VALUE == 8000000)
    /* PLL configuration: PLLCLK = 8MHz / 2 * 16 = 64 MHz */
    RCC->PLLSRC = RCC_PLLSRC_HSE;
    RCC->PLLPRE = RCC_PLLPRE_SRCEN;
    RCC->PLLPRE |= RCC_PLLPRE_RATIO_2;
    RCC->PLLPRE |= RCC_PLLPRE_DIVEN;
    ANCTL->PLLCR = ANCTL_PLLCR_PLLMUL_16;
#elif (HSE_VALUE == 12000000)
    /* PLL configuration: PLLCLK = 12MHz / 3 * 16 = 64 MHz */
    RCC->PLLSRC = RCC_PLLSRC_HSE;
    RCC->PLLPRE = RCC_PLLPRE_SRCEN;
    RCC->PLLPRE |= RCC_PLLPRE_RATIO_3;
    RCC->PLLPRE |= RCC_PLLPRE_DIVEN;
    ANCTL->PLLCR = ANCTL_PLLCR_PLLMUL_16;
#elif (HSE_VALUE == 16000000)
    /* PLL configuration: PLLCLK = 16MHz / 4 * 16 = 64 MHz */
    RCC->PLLSRC = RCC_PLLSRC_HSE;
    RCC->PLLPRE = RCC_PLLPRE_SRCEN;
    RCC->PLLPRE |= RCC_PLLPRE_RATIO_4;
    RCC->PLLPRE |= RCC_PLLPRE_DIVEN;
    ANCTL->PLLCR = ANCTL_PLLCR_PLLMUL_16;
#endif

    /* Enable PLL */
    ANCTL->PLLENR = ANCTL_PLLENR_PLLON;

    /* Wait till PLL is ready */
    while(ANCTL->PLLSR != 0x03)
    {
    }

    /* Select PLL as system clock source */
    RCC->MAINCLKSRC = RCC_MAINCLKSRC_PLLCLK;
    RCC->MAINCLKUEN = RCC_MAINCLKUEN_ENA;
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
    while (1);
  }

  /* Locks write to ANCTL registers */
  PWR->ANAKEY1 = 0x00;
  PWR->ANAKEY2 = 0x00;
}
#endif /* MAINCLK_PLLSRC_MHSI */

#elif defined MAINCLK_FREQ_72MHz
/**
 * @brief  Sets main clock frequency to 72MHz and configure AHBCLK, APB1CLK 
 *         and APB2CLK prescalers.
 * @note   This function should be used only after reset.
 * @param  None
 * @return None
 */
static void SetSysClockTo72(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

  /* Unlocks write to ANCTL registers */
  PWR->ANAKEY1 = 0x03;
  PWR->ANAKEY2 = 0x0C;

  /* APB1CLK = MAINCLK */
  RCC->APB1PRE = RCC_APB1PRE_SRCEN;
  RCC->APB1PRE |= 0x00;

  /* Configure PD0 and PD1 to analog mode */
  RCC->APB1ENR = RCC_APB1ENR_BMX1EN | RCC_APB1ENR_GPIODEN;
  GPIOD->CFGMSK = 0xFFFC;
  GPIOD->MODER = 0x0F;

  /* Enable HSE */
  ANCTL->HSECR1 = ANCTL_HSECR1_PADOEN;
  ANCTL->HSECR0 = ANCTL_HSECR0_HSEON;

  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = ANCTL->HSESR & ANCTL_HSESR_HSERDY;
    StartUpCounter++;
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

  if (HSEStatus != 0)
  {
    /* Configure Flash prefetch, Cache and wait state */
    CACHE->CR = CACHE_CR_CHEEN | CACHE_CR_PREFEN_ON | CACHE_CR_LATENCY_2WS;

    /* AHBCLK = MAINCLK */
    RCC->AHBPRE = 0x00;

    /* APB2CLK = MAINCLK */
    RCC->APB2PRE = RCC_APB2PRE_SRCEN;
    RCC->APB2PRE |= 0x00;

#if (HSE_VALUE == 6000000)
    /* PLL configuration: PLLCLK = 6MHz * 12 = 72 MHz */
    RCC->PLLSRC = RCC_PLLSRC_HSE;
    RCC->PLLPRE = RCC_PLLPRE_SRCEN;
    RCC->PLLPRE |= 0x00;
    ANCTL->PLLCR = ANCTL_PLLCR_PLLMUL_12;
#elif (HSE_VALUE == 12000000)
    /* PLL configuration: PLLCLK = 12MHz / 2 * 12 = 72 MHz */
    RCC->PLLSRC = RCC_PLLSRC_HSE;
    RCC->PLLPRE = RCC_PLLPRE_SRCEN;
    RCC->PLLPRE |= RCC_PLLPRE_RATIO_2;
    RCC->PLLPRE |= RCC_PLLPRE_DIVEN;
    ANCTL->PLLCR = ANCTL_PLLCR_PLLMUL_12;
#endif

    /* Enable PLL */
    ANCTL->PLLENR = ANCTL_PLLENR_PLLON;

    /* Wait till PLL is ready */
    while(ANCTL->PLLSR != 0x03)
    {
    }

    /* Select PLL as system clock source */
    RCC->MAINCLKSRC = RCC_MAINCLKSRC_PLLCLK;
    RCC->MAINCLKUEN = RCC_MAINCLKUEN_ENA;
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
    while (1);
  }

  /* Locks write to ANCTL registers */
  PWR->ANAKEY1 = 0x00;
  PWR->ANAKEY2 = 0x00;
}

#elif defined MAINCLK_FREQ_96MHz
#ifdef MAINCLK_PLLSRC_MHSI
/**
 * @brief  Sets main clock frequency to 96MHz and configure AHBCLK, APB1CLK 
 *         and APB2CLK prescalers.
 * @note   This function should be used only after reset.
 * @param  None
 * @return None
 */
static void SetSysClockTo96(void)
{
  /* Unlocks write to ANCTL registers */
  PWR->ANAKEY1 = 0x03;
  PWR->ANAKEY2 = 0x0C;

  /* APB1CLK = MAINCLK */
  RCC->APB1PRE = RCC_APB1PRE_SRCEN;
  RCC->APB1PRE |= 0x00;

  /* Configure Flash prefetch, Cache and wait state */
  CACHE->CR = CACHE_CR_CHEEN | CACHE_CR_PREFEN_ON | CACHE_CR_LATENCY_3WS;

  /* AHBCLK = MAINCLK */
  RCC->AHBPRE = 0x00;

  /* APB2CLK = MAINCLK */
  RCC->APB2PRE = RCC_APB2PRE_SRCEN;
  RCC->APB2PRE |= 0x00;

  /* PLL configuration: PLLCLK = MHSI(8MHz) * 12 = 96 MHz */
  RCC->PLLSRC = RCC_PLLSRC_MHSI;
  RCC->PLLPRE = RCC_PLLPRE_SRCEN;
  RCC->PLLPRE |= 0x00;
  ANCTL->PLLCR = ANCTL_PLLCR_PLLMUL_12;

  /* Enable PLL */
  ANCTL->PLLENR = ANCTL_PLLENR_PLLON;

  /* Wait till PLL is ready */
  while(ANCTL->PLLSR != 0x03)
  {
  }

  /* Select PLL as system clock source */
  RCC->MAINCLKSRC = RCC_MAINCLKSRC_PLLCLK;
  RCC->MAINCLKUEN = RCC_MAINCLKUEN_ENA;

  /* Locks write to ANCTL registers */
  PWR->ANAKEY1 = 0x00;
  PWR->ANAKEY2 = 0x00;
}
#else /* MAINCLK_PLLSRC_MHSI */
/**
 * @brief  Sets main clock frequency to 96MHz and configure AHBCLK, APB1CLK 
 *         and APB2CLK prescalers.
 * @note   This function should be used only after reset.
 * @param  None
 * @return None
 */
static void SetSysClockTo96(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

  /* Unlocks write to ANCTL registers */
  PWR->ANAKEY1 = 0x03;
  PWR->ANAKEY2 = 0x0C;

  /* APB1CLK = MAINCLK */
  RCC->APB1PRE = RCC_APB1PRE_SRCEN;
  RCC->APB1PRE |= 0x00;

  /* Configure PD0 and PD1 to analog mode */
  RCC->APB1ENR = RCC_APB1ENR_BMX1EN | RCC_APB1ENR_GPIODEN;
  GPIOD->CFGMSK = 0xFFFC;
  GPIOD->MODER = 0x0F;

  /* Enable HSE */
  ANCTL->HSECR1 = ANCTL_HSECR1_PADOEN;
  ANCTL->HSECR0 = ANCTL_HSECR0_HSEON;

  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = ANCTL->HSESR & ANCTL_HSESR_HSERDY;
    StartUpCounter++;
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

  if (HSEStatus != 0)
  {
    /* Configure Flash prefetch, Cache and wait state */
    CACHE->CR = CACHE_CR_CHEEN | CACHE_CR_PREFEN_ON | CACHE_CR_LATENCY_3WS;

    /* AHBCLK = MAINCLK */
    RCC->AHBPRE = 0x00;

    /* APB2CLK = MAINCLK */
    RCC->APB2PRE = RCC_APB2PRE_SRCEN;
    RCC->APB2PRE |= 0x00;

#if (HSE_VALUE == 6000000)
    /* PLL configuration: PLLCLK = 6MHz * 16 = 96 MHz */
    RCC->PLLSRC = RCC_PLLSRC_HSE;
    RCC->PLLPRE = RCC_PLLPRE_SRCEN;
    RCC->PLLPRE |= 0x00;
    ANCTL->PLLCR = ANCTL_PLLCR_PLLMUL_16;
#elif (HSE_VALUE == 8000000)
    /* PLL configuration: PLLCLK = 8MHz * 12 = 96 MHz */
    RCC->PLLSRC = RCC_PLLSRC_HSE;
    RCC->PLLPRE = RCC_PLLPRE_SRCEN;
    RCC->PLLPRE |= 0x00;
    ANCTL->PLLCR = ANCTL_PLLCR_PLLMUL_12;
#elif (HSE_VALUE == 12000000)
    /* PLL configuration: PLLCLK = 12MHz / 2 * 16 = 96 MHz */
    RCC->PLLSRC = RCC_PLLSRC_HSE;
    RCC->PLLPRE = RCC_PLLPRE_SRCEN;
    RCC->PLLPRE |= RCC_PLLPRE_RATIO_2;
    RCC->PLLPRE |= RCC_PLLPRE_DIVEN;
    ANCTL->PLLCR = ANCTL_PLLCR_PLLMUL_16;
#elif (HSE_VALUE == 16000000)
    /* PLL configuration: PLLCLK = 16MHz / 2 * 12 = 96 MHz */
    RCC->PLLSRC = RCC_PLLSRC_HSE;
    RCC->PLLPRE = RCC_PLLPRE_SRCEN;
    RCC->PLLPRE |= RCC_PLLPRE_RATIO_2;
    RCC->PLLPRE |= RCC_PLLPRE_DIVEN;
    ANCTL->PLLCR = ANCTL_PLLCR_PLLMUL_12;
#endif

    /* Enable PLL */
    ANCTL->PLLENR = ANCTL_PLLENR_PLLON;

    /* Wait till PLL is ready */
    while(ANCTL->PLLSR != 0x03)
    {
    }

    /* Select PLL as system clock source */
    RCC->MAINCLKSRC = RCC_MAINCLKSRC_PLLCLK;
    RCC->MAINCLKUEN = RCC_MAINCLKUEN_ENA;
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
    while (1);
  }

  /* Locks write to ANCTL registers */
  PWR->ANAKEY1 = 0x00;
  PWR->ANAKEY2 = 0x00;
}
#endif /* MAINCLK_PLLSRC_MHSI */

#elif defined MAINCLK_FREQ_128MHz
#ifdef MAINCLK_PLLSRC_MHSI
/**
 * @brief  Sets main clock frequency to 128MHz and configure AHBCLK, APB1CLK 
 *         and APB2CLK prescalers.
 * @note   This function should be used only after reset.
 * @param  None
 * @return None
 */
static void SetSysClockTo128(void)
{
  /* Unlocks write to ANCTL registers */
  PWR->ANAKEY1 = 0x03;
  PWR->ANAKEY2 = 0x0C;

  /* APB1CLK = MAINCLK */
  RCC->APB1PRE = RCC_APB1PRE_SRCEN;
  RCC->APB1PRE |= 0x00;

  /* Configure Flash prefetch, Cache and wait state */
  CACHE->CR = CACHE_CR_CHEEN | CACHE_CR_PREFEN_ON | CACHE_CR_HIFREQ | CACHE_CR_LATENCY_4WS;

  /* AHBCLK = MAINCLK */
  RCC->AHBPRE = 0x00;

  /* APB2CLK = MAINCLK */
  RCC->APB2PRE = RCC_APB2PRE_SRCEN;
  RCC->APB2PRE |= 0x00;

  /* PLL configuration: PLLCLK = MHSI(8MHz) * 16 = 128 MHz */
  RCC->PLLSRC = RCC_PLLSRC_MHSI;
  RCC->PLLPRE = RCC_PLLPRE_SRCEN;
  RCC->PLLPRE |= 0x00;
  ANCTL->PLLCR = ANCTL_PLLCR_PLLMUL_16 | (0x4 << 3);

  /* Enable PLL */
  ANCTL->PLLENR = ANCTL_PLLENR_PLLON;

  /* Wait till PLL is ready */
  while(ANCTL->PLLSR != 0x03)
  {
  }

  /* Select PLL as system clock source */
  RCC->MAINCLKSRC = RCC_MAINCLKSRC_PLLCLK;
  RCC->MAINCLKUEN = RCC_MAINCLKUEN_ENA;

  /* Locks write to ANCTL registers */
  PWR->ANAKEY1 = 0x00;
  PWR->ANAKEY2 = 0x00;
}
#else /* MAINCLK_PLLSRC_MHSI */
/**
 * @brief  Sets main clock frequency to 128MHz and configure AHBCLK, APB1CLK 
 *         and APB2CLK prescalers.
 * @note   This function should be used only after reset.
 * @param  None
 * @return None
 */
static void SetSysClockTo128(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

  /* Unlocks write to ANCTL registers */
  PWR->ANAKEY1 = 0x03;
  PWR->ANAKEY2 = 0x0C;

  /* APB1CLK = MAINCLK */
  RCC->APB1PRE = RCC_APB1PRE_SRCEN;
  RCC->APB1PRE |= 0x00;

  /* Configure PD0 and PD1 to analog mode */
  RCC->APB1ENR = RCC_APB1ENR_BMX1EN | RCC_APB1ENR_GPIODEN;
  GPIOD->CFGMSK = 0xFFFC;
  GPIOD->MODER = 0x0F;

  /* Enable HSE */
  ANCTL->HSECR1 = ANCTL_HSECR1_PADOEN;
  ANCTL->HSECR0 = ANCTL_HSECR0_HSEON;

  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = ANCTL->HSESR & ANCTL_HSESR_HSERDY;
    StartUpCounter++;
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

  if (HSEStatus != 0)
  {
    /* Configure Flash prefetch, Cache and wait state */
    CACHE->CR = CACHE_CR_CHEEN | CACHE_CR_PREFEN_ON | CACHE_CR_HIFREQ | CACHE_CR_LATENCY_4WS;

    /* AHBCLK = MAINCLK */
    RCC->AHBPRE = 0x00;

    /* APB2CLK = MAINCLK */
    RCC->APB2PRE = RCC_APB2PRE_SRCEN;
    RCC->APB2PRE |= 0x00;

#if (HSE_VALUE == 8000000)
    /* PLL configuration: PLLCLK = 8MHz * 16 = 128 MHz */
    RCC->PLLSRC = RCC_PLLSRC_HSE;
    RCC->PLLPRE = RCC_PLLPRE_SRCEN;
    RCC->PLLPRE |= 0x00;
    ANCTL->PLLCR = ANCTL_PLLCR_PLLMUL_16 | (0x4 << 3);
#elif (HSE_VALUE == 16000000)
    /* PLL configuration: PLLCLK = 16MHz / 2 * 16 = 128 MHz */
    RCC->PLLSRC = RCC_PLLSRC_HSE;
    RCC->PLLPRE = RCC_PLLPRE_SRCEN;
    RCC->PLLPRE |= RCC_PLLPRE_RATIO_2;
    RCC->PLLPRE |= RCC_PLLPRE_DIVEN;
    ANCTL->PLLCR = ANCTL_PLLCR_PLLMUL_16 | (0x4 << 3);
#endif

    /* Enable PLL */
    ANCTL->PLLENR = ANCTL_PLLENR_PLLON;

    /* Wait till PLL is ready */
    while(ANCTL->PLLSR != 0x03)
    {
    }

    /* Select PLL as system clock source */
    RCC->MAINCLKSRC = RCC_MAINCLKSRC_PLLCLK;
    RCC->MAINCLKUEN = RCC_MAINCLKUEN_ENA;
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
    while (1);
  }

  /* Locks write to ANCTL registers */
  PWR->ANAKEY1 = 0x00;
  PWR->ANAKEY2 = 0x00;
}
#endif /* MAINCLK_PLLSRC_MHSI */

#endif

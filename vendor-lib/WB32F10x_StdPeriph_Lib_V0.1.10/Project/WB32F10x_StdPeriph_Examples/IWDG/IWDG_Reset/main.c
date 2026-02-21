/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LSI_FREQ 32000

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t Index, Counter;

/* Private function prototypes -----------------------------------------------*/
void iwdg_erratum_workaround(void);

/* Private functions ---------------------------------------------------------*/

void Delay_ms(int nMS)
{
  SysTick->LOAD = 96000;
  SysTick->VAL  = 0x00;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
  while(nMS--)
    while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
  SysTick->CTRL = 0x00;
}

int main(void)
{
  /**
   * ERRATUM:    Once IWDG is enabled, it cannot be disabled even if a reset occurs.
   * WORKAROUND: Configure the IWDG timeout to a maximum before the program runs, 
   *             and don't forget to reload IWDG counter later.
   */
  iwdg_erratum_workaround();

  /* Enable GPIOB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOB, ENABLE);

  /* Configure PB13 and PB14 in output push-pull mode */
  GPIO_Init(GPIOB, GPIO_Pin_13 |GPIO_Pin_14, GPIO_MODE_OUT |GPIO_OTYPE_PP |GPIO_PUPD_NOPULL |GPIO_SPEED_HIGH);

  /* Turn off LED1 and LED2 */
  GPIO_SetBits(GPIOB, GPIO_Pin_13 |GPIO_Pin_14);

  Delay_ms(2000);

  /* Turn on LED1 and LED2 */
  GPIO_ResetBits(GPIOB, GPIO_Pin_13 |GPIO_Pin_14);

  /* Disable ANCTL register write-protection */
  PWR_UnlockANA();

  /* Enable the LSI OSC */
  ANCTL_LSICmd(ENABLE);

  /* Wait till LSI is ready */
  while(ANCTL_GetFlagStatus(ANCTL_FLAG_LSIRDY) == RESET);

  /* Enable ANCTL register write-protection */
  PWR_LockANA();

  /* Enable IWDG clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_IWDG, ENABLE);

  /* Enable write access to IWDG_PR and IWDG_RLR registers */
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

  /* IWDG counter clock: LSI/32 */
  IWDG_SetPrescaler(IWDG_Prescaler_32);
  while(IWDG_GetFlagStatus(IWDG_FLAG_PVU) != RESET);

  /* IWDG timeout is about 250ms */
  IWDG_SetReload(LSI_FREQ / 32 * 0.250);
  while(IWDG_GetFlagStatus(IWDG_FLAG_RVU) != RESET);

  /* ATTENTION: It is best to reload IWDG counter when the RVU bit is 0. */
  while(IWDG_GetFlagStatus(IWDG_FLAG_RVU) != RESET);
  IWDG_ReloadCounter();

  /* Enable IWDG */
  IWDG_Enable();

  /* Toggle LED2 6 times */
  Counter = 0;
  Index = 0;
  while (Index < 6)
  {
    Counter++;
    if (Counter > 500000)
    {
      Counter = 0;
      /* Toggle LED2 */
      GPIO_ToggleBits(GPIOB, GPIO_Pin_13);
      Index++;
    }

    /* ATTENTION: It is best to reload IWDG counter when the RVU bit is 0. */
    if (IWDG_GetFlagStatus(IWDG_FLAG_RVU) == RESET) {
      IWDG_ReloadCounter();
    }
  }

  /* Infinite loop */
  while (1)
  {
    Delay_ms(200);

    /* ATTENTION: It is best to reload IWDG counter when the RVU bit is 0. */
    while(IWDG_GetFlagStatus(IWDG_FLAG_RVU) != RESET);
    IWDG_ReloadCounter();
  }
}

void iwdg_erratum_workaround(void)
{
  /* Disable ANCTL register write-protection */
  PWR_UnlockANA();

  /* Enable the LSI OSC */
  ANCTL_LSICmd(ENABLE);

  /* Wait till LSI is ready */
  while(ANCTL_GetFlagStatus(ANCTL_FLAG_LSIRDY) == RESET);

  /* Enable ANCTL register write-protection */
  PWR_LockANA();

  /**
   * ERRATUM:    If IWDG is enabled, Once a reset occurs, the IWDG domain needs three LSI clock cycles to be ready.
   * WORKAROUND: After the LSI is ready, wait about 1ms before configuring the IWDG.
   */
  Delay_ms(1);

  /* Enable IWDG clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_IWDG, ENABLE);

  /* Enable write access to IWDG_PR and IWDG_RLR registers */
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

  /* IWDG counter clock: LSI/256 */
  IWDG_SetPrescaler(IWDG_Prescaler_256);
  while(IWDG_GetFlagStatus(IWDG_FLAG_PVU) != RESET);

  /* IWDG timeout is about 32768ms */
  IWDG_SetReload(0x0FFF);
  while(IWDG_GetFlagStatus(IWDG_FLAG_RVU) != RESET);

  /* ATTENTION: It is best to reload IWDG counter when the RVU bit is 0. */
  while(IWDG_GetFlagStatus(IWDG_FLAG_RVU) != RESET);
  IWDG_ReloadCounter();
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

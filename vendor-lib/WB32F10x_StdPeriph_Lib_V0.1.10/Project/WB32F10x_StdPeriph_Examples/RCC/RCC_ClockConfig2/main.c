/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Configuration(void);

/* Private functions ---------------------------------------------------------*/

void delay(uint32_t n)
{
  while(n--);
}

int main(void)
{
  /* Configures the Main clock frequency, HCLK, PCLK2 and PCLK1 prescalers. */
  SystemClock_Configuration();

  /* Enable GPIOB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOB, ENABLE);

  /* Configure PB14 in output push-pull mode */
  GPIO_Init(GPIOB, GPIO_Pin_14, GPIO_MODE_OUT |GPIO_OTYPE_PP |GPIO_PUPD_NOPULL |GPIO_SPEED_HIGH);

  /* Infinite loop */
  while (1)
  {
    GPIO_ToggleBits(GPIOB, GPIO_Pin_14);
    delay(1000000);
  }
}

/**
 * @brief  Configures the Main clock frequency, HCLK, PCLK2 and PCLK1
 *         prescalers.
 * @return None
 */
void SystemClock_Configuration(void)
{
  /* Disable ANCTL register write-protection */
  PWR_UnlockANA();

  /* Configure Flash prefetch, Cache and wait state */
  CACHE->CR = CACHE_CR_CHEEN | CACHE_CR_PREFEN_ON | CACHE_CR_LATENCY_3WS;

  /* AHBCLK = MAINCLK */
  RCC_AHBCLKConfig(RCC_MAINCLK_Div1);

  /* Enable APB1CLK. APB1CLK = MAINCLK */
  RCC_APB1CLKConfig(RCC_MAINCLK_Div1, ENABLE);

  /* Enable APB2CLK. APB2CLK = MAINCLK */
  RCC_APB2CLKConfig(RCC_MAINCLK_Div1, ENABLE);

  /* PLL configuration: PLLCLK = MHSI(8MHz) * 12 = 96 MHz */
  RCC_PLLSourceConfig(RCC_PLLSource_MHSI_Div1, ENABLE);
  ANCTL_PLLConfig(ANCTL_PLLMul_12);

  /* Enable PLL */
  ANCTL_PLLCmd(ENABLE);

  /* Wait till PLL is ready */
  while(ANCTL_GetFlagStatus(ANCTL_FLAG_PLLRDY) == RESET);

  /* Enable ANCTL register write-protection */
  PWR_LockANA();

  /* Select PLL as system clock source */
  RCC_MAINCLKConfig(RCC_MAINCLKSource_PLLCLK);
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

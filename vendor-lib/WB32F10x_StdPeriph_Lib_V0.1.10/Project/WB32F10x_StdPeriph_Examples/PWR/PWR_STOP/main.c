/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

/* Private function prototypes -----------------------------------------------*/
void Clock_Config(void);
void EXTI0_Config(void);

/* Private functions ---------------------------------------------------------*/

void delay(uint32_t n)
{
  while(n--);
}

int main(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  /* Configures system clock */
  Clock_Config();

  GPIO_Init(GPIOB, GPIO_Pin_13 |GPIO_Pin_14, GPIO_MODE_OUT |GPIO_OTYPE_PP |GPIO_PUPD_NOPULL |GPIO_SPEED_HIGH);
  GPIO_ResetBits(GPIOB, GPIO_Pin_14);   // Turn on LED1
  GPIO_SetBits(GPIOB, GPIO_Pin_13);     // Turn off LED2

  delay(50000000);

  EXTI0_Config();

  /* Enter STOP Mode */
  PWR_EnterSTOPMode(PWR_STOPMode_LP4_S32KON, PWR_EntryMode_WFI);

  /* After exit STOP mode, the clock configuration is reset. Reconfigures system clock. */
  Clock_Config();

  /* Infinite loop */
  while (1)
  {
    /* LED1 blink */
    GPIO_ToggleBits(GPIOB, GPIO_Pin_14);
    delay(8000000);
  }
}

void Clock_Config(void)
{
  /* Enable APB1CLK. APB1CLK = MAINCLK */
  RCC_APB1CLKConfig(RCC_MAINCLK_Div1, ENABLE);

  /* Enable BMX1 and GPIOD clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOD, ENABLE);

  /* Configure PD0 and PD1 to analog mode */
  GPIO_Init(GPIOD, GPIO_Pin_0 |GPIO_Pin_1, GPIO_MODE_ANA);

  /* Disable ANCTL register write-protection */
  PWR_UnlockANA();

  /* Enable HSE */
  ANCTL_HSEConfig(ANCTL_HSE_ON);

  /* Wait till HSE is ready */
  if (ANCTL_WaitForHSEStartUp() == ERROR)
  {
    /* HSE failed */
    while (1);
  }

  /* Configure Flash prefetch, Cache and wait state */
  CACHE->CR = CACHE_CR_CHEEN | CACHE_CR_PREFEN_ON | CACHE_CR_LATENCY_3WS;

  /* AHBCLK = MAINCLK */
  RCC_AHBCLKConfig(RCC_MAINCLK_Div1);

  /* Enable APB2CLK. APB2CLK = MAINCLK */
  RCC_APB2CLKConfig(RCC_MAINCLK_Div1, ENABLE);

  /* PLL configuration: PLLCLK = HSE(8MHz) * 12 = 96 MHz */
  RCC_PLLSourceConfig(RCC_PLLSource_HSE_Div1, ENABLE);
  ANCTL_PLLConfig(ANCTL_PLLMul_12);

  /* Enable PLL */
  ANCTL_PLLCmd(ENABLE);

  /* Wait till PLL is ready */
  while(ANCTL_GetFlagStatus(ANCTL_FLAG_PLLRDY) == RESET);

  /* Enable ANCTL register write-protection */
  PWR_LockANA();

  /* Select PLL as system clock source */
  RCC_MAINCLKConfig(RCC_MAINCLKSource_PLLCLK);


  /* Enable BMX1, GPIOA, GPIOB, EXTI and AFIO clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOA | RCC_APB1Periph_GPIOB | 
                         RCC_APB1Periph_EXTI |RCC_APB1Periph_AFIO, ENABLE);
}

void EXTI0_Config(void)
{
  /* Configure PA0 pin as pull-down input */
  GPIO_Init(GPIOA, GPIO_Pin_0, GPIO_MODE_IN |GPIO_PUPD_DOWN);

  /* Connect EXTI0 Line to PA0 pin */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

  /* Configure EXTI0 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI0 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void EXTI0_IRQHandler(void)
{
  RCC_APB1CLKConfig(RCC_MAINCLK_Div1, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOB |RCC_APB1Periph_EXTI, ENABLE);

  if(EXTI_GetFlagStatus(EXTI_Line0) != RESET)
  {
    /* Clear the  EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line0);

    /* Change LED2 status */
    GPIO_ToggleBits(GPIOB, GPIO_Pin_13);
  }
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

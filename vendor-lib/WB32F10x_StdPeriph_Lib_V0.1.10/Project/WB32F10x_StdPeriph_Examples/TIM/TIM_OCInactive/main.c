#include "wb32f10x.h"

/*
  本例程演示了TIM的TIM_OCMode_Inactive模式下4路中断请求
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;

uint16_t CCR1_Val = 800;
uint16_t CCR2_Val = 400;
uint16_t CCR3_Val = 200;
uint16_t CCR4_Val = 100;
uint16_t PrescalerValue = 0;

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* System Clocks Configuration */
  RCC_Configuration();

  /* NVIC Configuration */
  NVIC_Configuration();

  /* GPIO Configuration */
  GPIO_Configuration();

  /* ---------------------------------------------------------------
    TIM1 Configuration: 
    The objective is to get TIM1 counter clock at 1MHz:
     - Prescaler = (TIM1CLK / TIM1 counter clock) - 1
    And generate 4 signals with 4 different delays:
    TIM1_CH1 delay = CCR1_Val/TIM1 counter clock = 800 us
    TIM1_CH2 delay = CCR2_Val/TIM1 counter clock = 400 us
    TIM1_CH3 delay = CCR3_Val/TIM1 counter clock = 200 us
    TIM1_CH4 delay = CCR4_Val/TIM1 counter clock = 100 us
  --------------------------------------------------------------- */

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 999;
  TIM_TimeBaseStructure.TIM_Prescaler = 95;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  /* Output Compare Active Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Inactive;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);

  /* Output Compare Active Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Disable);

  /* Output Compare Active Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Disable);

  /* Output Compare Active Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;
  TIM_OC4Init(TIM1, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Disable);
  
  TIM_ARRPreloadConfig(TIM1, ENABLE);

  /* TIM IT enable */
  TIM_ITConfig(TIM1, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4, ENABLE);
  
  /* Set PA.08, PA.09, PA.10 and PA.11 pins */
  GPIO_SetBits(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11);

  /* TIM1 enable counter */
  TIM_Cmd(TIM1, ENABLE);

  while (1)
  {}
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
  /* TIM1 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 | RCC_APB1Periph_TIM1, ENABLE);

  /* GPIOB clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 | RCC_APB1Periph_GPIOA, ENABLE);
}

/**
  * @brief  Configure the GPIO Pins.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  /* GPIOA Configuration */
  GPIO_Init(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11, GPIO_MODE_OUT);
}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the TIM1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);

}

/**
  * @brief  This function handles TIM1 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM1_CC_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
  {
    /* Clear TIM1 Capture Compare1 interrupt pending bit*/
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);

    /* PA.08 turnoff after 800us */
    GPIO_ResetBits(GPIOA, GPIO_Pin_8);
  }
  else if (TIM_GetITStatus(TIM1, TIM_IT_CC2) != RESET)
  {
    /* Clear TIM1 Capture Compare2 interrupt pending bit*/
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC2);

    /* PA.09 turnoff after 400us */
    GPIO_ResetBits(GPIOA, GPIO_Pin_9);
  }
  else if (TIM_GetITStatus(TIM1, TIM_IT_CC3) != RESET)
  {
    /* Clear TIM1 Capture Compare3 interrupt pending bit*/
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC3);

    /* PA.10 turnoff after 200us */
    GPIO_ResetBits(GPIOA, GPIO_Pin_10);
  }
  else if (TIM_GetITStatus(TIM1, TIM_IT_CC4) != RESET)
  {
    /* Clear TIM1 Capture Compare4 interrupt pending bit*/
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC4);

    /* PA.11 turnoff after 100us */
    GPIO_ResetBits(GPIOA, GPIO_Pin_11);
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

  while (1)
  {}
}

#endif

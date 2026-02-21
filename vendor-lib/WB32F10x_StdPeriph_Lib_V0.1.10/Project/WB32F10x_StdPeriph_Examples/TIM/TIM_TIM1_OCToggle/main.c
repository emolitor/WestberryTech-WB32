#include "wb32f10x.h"

/*
  本例程演示了TIM4路通道生成4路频率不同的PWM
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
__IO uint16_t CCR1Val = 32768;
__IO uint16_t CCR2Val = 16384;
__IO uint16_t CCR3Val = 8192;
__IO uint16_t CCR4Val = 4096;
uint16_t PrescalerValue = 0;

uint32_t capture = 0;

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

  /* GPIO Configuration */
  GPIO_Configuration();

  /* NVIC Configuration */
  NVIC_Configuration();

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 0xFFFFF;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  /* Output Compare Toggle Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);

  /* Output Compare Toggle Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR2Val;
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Disable);

  /* Output Compare Toggle Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR3Val;
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Disable);
  
  /* Output Compare Toggle Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR4Val;
  TIM_OC4Init(TIM1, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Disable);
  
  /* TIM enable counter */
  TIM_Cmd(TIM1, ENABLE);
  
  /* TIM IT enable */
  TIM_ITConfig(TIM1, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4, ENABLE);
  
  TIM_CtrlPWMOutputs(TIM1, ENABLE);

  while (1)
  {
  }
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
  /* TIM1 GPIOA GPIOB and AFIO clocks enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |
                         RCC_APB1Periph_TIM1 | 
                         RCC_APB1Periph_GPIOA | 
                         RCC_APB1Periph_AFIO, 
                         ENABLE);
}

/**
  * @brief  Configure the GPIO Pins.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  GPIO_Init(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11, GPIO_MODE_AF | GPIO_AF1);
}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the TIM1 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel =  TIM1_CC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  This function handles TIM1 CC interrupts request.
  * @param  None
  * @retval None
  */
void TIM1_CC_IRQHandler(void)
{
  /* TIM1_CH1 toggling with frequency = 1.46 kHz */
  if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
  {
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC1 );
    capture = TIM_GetCapture1(TIM1);
    TIM_SetCompare1(TIM1, capture + CCR1Val );
  }
  /* TIM1_CH2 toggling with frequency = 2.92 kHz */
  if (TIM_GetITStatus(TIM1, TIM_IT_CC2) != RESET)
  {
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC2);
    capture = TIM_GetCapture2(TIM1);
    TIM_SetCompare2(TIM1, capture + CCR2Val);
  }
  /* TIM1_CH3 toggling with frequency = 5.85 kHz */
  if (TIM_GetITStatus(TIM1, TIM_IT_CC3) != RESET)
  {
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC3);
    capture = TIM_GetCapture3(TIM1);
    TIM_SetCompare3(TIM1, capture + CCR3Val);
  }
  /* TIM1_CH4 toggling with frequency = 11.71 kHz */
  if (TIM_GetITStatus(TIM1, TIM_IT_CC4) != RESET)
  {
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC4);
    capture = TIM_GetCapture4(TIM1);
    TIM_SetCompare4(TIM1, capture + CCR4Val);
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

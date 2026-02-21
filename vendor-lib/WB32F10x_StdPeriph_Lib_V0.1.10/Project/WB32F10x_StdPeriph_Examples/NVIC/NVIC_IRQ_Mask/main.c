/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
NVIC_InitTypeDef  NVIC_InitStructure;
EXTI_InitTypeDef EXTI0_InitStruct;

uint32_t CCR1_Val = 0x7FFFF;
uint32_t CCR2_Val = 0x3FFFF;
uint32_t CCR3_Val = 0x1FFFF;
uint32_t CCR4_Val = 0x0FFFF;
uint16_t PrescalerValue = 0;
__IO uint32_t index = 0;

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);

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

  /* TIM2 configuration */
  TIM_TimeBaseStructure.TIM_Period = 0xFFFFF;          
  TIM_TimeBaseStructure.TIM_Prescaler = 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_OCStructInit(&TIM_OCInitStructure);
  
  /* Output Compare Timing Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_Pulse = 0x0;  
  TIM_OC1Init(TIM2, &TIM_OCInitStructure);
  
  /* TIM3 configuration */
  TIM_TimeBaseStructure.TIM_Period = 0xFFFFF;    
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  
  /* Output Compare Timing Mode configuration: Channel1 */
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);
  
  /* TIM4 configuration */
  TIM_TimeBaseStructure.TIM_Period = 0xFFFFF;  
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  
  /* Output Compare Timing Mode configuration: Channel1 */
  TIM_OC1Init(TIM4, &TIM_OCInitStructure);


  /* Clear TIM2, TIM3 and TIM4 update pending flags */
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);
  TIM_ClearFlag(TIM3, TIM_FLAG_Update);
  TIM_ClearFlag(TIM4, TIM_FLAG_Update);

  /* Configure two bits for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* Enable the TIM2 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable the TIM3 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the TIM4 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable TIM2, TIM3 and TIM4 Update interrupts */
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

  /* TIM2, TIM3 and TIM4 enable counters */
  TIM_Cmd(TIM2, ENABLE);
  TIM_Cmd(TIM3, ENABLE);
  TIM_Cmd(TIM4, ENABLE);
  while (1)
  {
    /* Wait until KEY button is pressed. */
    while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == RESET)
    {
    }
    while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) != RESET)
    {
    }

    /* This instruction raises the execution priority to 0. This prevents all 
       exceptions with configurable priority from activating, other than through 
       the HardFault fault escalation mechanism. */
    __disable_irq();

    /* Turn LED2 ON */
    GPIO_SetBits(GPIOB, GPIO_Pin_11);

    /* Wait until KEY button is pressed. */
    while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == RESET)
    {
    }
    while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) != RESET)
    {
    }

    /* This instruction will allow all exceptions with configurable priority to 
       be activated. */
    __enable_irq();

    /* Turn LED2 OFF */
    GPIO_ResetBits(GPIOB, GPIO_Pin_11);
  }
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
  /* Clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | 
                         RCC_APB1Periph_TIM3 |
                         RCC_APB1Periph_TIM4 | 
                         RCC_APB1Periph_GPIOA | 
                         RCC_APB1Periph_GPIOB | 
                         RCC_APB1Periph_EXTI |
                         RCC_APB1Periph_AFIO , 
                         ENABLE);
}

/**
  * @brief  Configure the GPIO.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  /*GPIOA Configuration: TIM1 channel1, 2, 3 and 4 */
  GPIO_Init(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11, GPIO_MODE_OUT);

  GPIO_Init(GPIOA, GPIO_Pin_0 , GPIO_MODE_IN | GPIO_PUPD_DOWN);
  GPIO_Init(GPIOA, GPIO_Pin_1 , GPIO_MODE_IN | GPIO_PUPD_UP);
  GPIO_Init(GPIOB, GPIO_Pin_10 | GPIO_Pin_11 , GPIO_MODE_OUT);

  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
  EXTI0_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI0_InitStruct.EXTI_Line = EXTI_Line0;
  EXTI0_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI0_InitStruct.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI0_InitStruct);
  NVIC_EnableIRQ(EXTI0_IRQn);

}

/**
  * @brief  This function handles EXTI Lines 1 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
  if(EXTI_GetFlagStatus(EXTI_Line0) != RESET)
  {
    if(index == 0)
    {
      /* Configure the BASEPRI register to 0x40 (Preemption priority = 1). 
         Only IRQ with higher preemption priority than 1 are permitted. 
         This will mask TIM3 and TIM4 IRQ from generation. */
      __set_BASEPRI(0x40);
      index++;
    }
    else
    {
      /* Configure the BASEPRI register to 0x00 (Preemption priority = 0). 
         When this BASEPRI register is set to 0, it has no effect on the current 
         priority.
         TIM2, TIM3 and TIM4 generation is controlled by NVIC priority registers. */
      __set_BASEPRI(0x00);
      index = 0;
    }
    /* Clears the SEL Button EXTI line pending bits. */
    EXTI_ClearITPendingBit(EXTI_Line0);  
  }
} 

/**
  * @brief  This function handles TIM2 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
  /* Clear TIM2 update interrupt */
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  
  /* Toggle PA8 */
  GPIO_ToggleBits(GPIOA,GPIO_Pin_8);
}

/**
  * @brief  This function handles TIM3 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
  /* Clear TIM3 update interrupt */
  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
  
  /* Toggle PA9 */
  GPIO_ToggleBits(GPIOA,GPIO_Pin_9);
}

/**
  * @brief  This function handles TIM4 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM4_IRQHandler(void)
{
  /* Clear TIM4 update interrupt */
  TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
  
  /* Toggle PA10 */
  GPIO_ToggleBits(GPIOA,GPIO_Pin_10);
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

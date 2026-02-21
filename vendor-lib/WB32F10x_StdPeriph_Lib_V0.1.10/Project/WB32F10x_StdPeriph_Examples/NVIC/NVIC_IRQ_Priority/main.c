/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
NVIC_InitTypeDef NVIC_InitStructure;
EXTI_InitTypeDef EXTI0_InitStruct;
EXTI_InitTypeDef EXTI1_InitStruct;

__IO uint8_t PreemptionOccured = 0; 
__IO uint8_t PreemptionPriorityValue = 0; 

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void Delay(__IO uint32_t nCount);
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
  
  /* Configure the SysTick Handler Priority: Preemption priority and subpriority */
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), !PreemptionPriorityValue, 0));    
  
  while (1)
  {
    if(PreemptionOccured != 0)
    {
      /* Toggel The LED1 */
      GPIO_ToggleBits(GPIOB, GPIO_Pin_14);
      
      /* Insert delay Time */
      Delay(0x5FFFF);
      
    }    
  }
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
  /* TIM1 clock enable */
  /* GPIOA clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |
                         RCC_APB1Periph_GPIOA | 
                         RCC_APB1Periph_GPIOB |
                         RCC_APB1Periph_EXTI |
                         RCC_APB1Periph_AFIO , 
                         ENABLE);
}

/**
  * @brief  Configure the TIM1 Ouput Channels.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  /*GPIOA Configuration: TIM1 channel1, 2, 3 and 4 */
  GPIO_Init(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11, GPIO_MODE_OUT);

  GPIO_Init(GPIOA, GPIO_Pin_0 , GPIO_MODE_IN | GPIO_PUPD_DOWN);
  GPIO_Init(GPIOA, GPIO_Pin_1 , GPIO_MODE_IN | GPIO_PUPD_UP);
  GPIO_Init(GPIOB, GPIO_Pin_14 , GPIO_MODE_OUT);

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* Modify the EXIT0 Interrupt Preemption Priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriorityValue;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable the EXIT1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
  EXTI0_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI0_InitStruct.EXTI_Line = EXTI_Line0;
  EXTI0_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI0_InitStruct.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI0_InitStruct);
  
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
  EXTI1_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI1_InitStruct.EXTI_Line = EXTI_Line1;
  EXTI1_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI1_InitStruct.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI1_InitStruct);
}

/**
  * @brief  This function handles External interrupt Line 0 request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
  /* Generate SysTick exception */
  SCB->ICSR |= 0x04000000;
  
  /* Clear WAKEUP_BUTTON_EXTI_LINE pending bit */
  EXTI_ClearITPendingBit(EXTI_Line0);
}

/**
  * @brief  This function handles External lines 1 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI1_IRQHandler(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  if(EXTI_GetFlagStatus(EXTI_Line1) != RESET)
  {
    PreemptionPriorityValue = !PreemptionPriorityValue;
    PreemptionOccured = 0;

    /* Modify the EXIT0 Interrupt Preemption Priority */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriorityValue;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* Configure the SysTick Handler Priority: Preemption priority and subpriority */
    NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), !PreemptionPriorityValue, 0));    

    /* Clear EXIT1 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line1);
  }
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  /* If the EXTI0 IRQ Handler was preempted by SysTick Handler */
  if(NVIC_GetActive(EXTI0_IRQn) != 0)
  {
    PreemptionOccured = 1;
  }
}

/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
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

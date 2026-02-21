/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
EXTI_InitTypeDef   EXTI_InitStructure;
NVIC_InitTypeDef   NVIC_InitStructure;

/* Private function prototypes -----------------------------------------------*/
void EXTI0_Config(void);
void EXTI9_5_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @return None
  */
int main(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* Enable GPIOB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOB, ENABLE);

  /* Configure PB13 and PB14 in output push-pull mode */
  GPIO_Init(GPIOB, GPIO_Pin_13 |GPIO_Pin_14, GPIO_MODE_OUT |GPIO_OTYPE_PP |GPIO_PUPD_NOPULL |GPIO_SPEED_HIGH);

  /* Turn off LED1 and LED2 */
  GPIO_SetBits(GPIOB, GPIO_Pin_13 |GPIO_Pin_14);

  /* Configure PA0 in interrupt mode */
  EXTI0_Config();

  /* Configure PB6 in interrupt mode */
  EXTI9_5_Config();

  /* Generate software interrupt: simulate a falling edge applied on EXTI0 line */
  EXTI_GenerateSWInterrupt(EXTI_Line0);

  while(1)
  {
  }
}

/**
  * @brief  Configure PA0 in interrupt mode
  * @param  None
  * @return None
  */
void EXTI0_Config(void)
{
  /* Enable GPIOA clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOA, ENABLE);

  /* Configure PA0 pin as input pull-down */
  GPIO_Init(GPIOA, GPIO_Pin_0, GPIO_MODE_IN |GPIO_PUPD_DOWN);

  /* Enable AFIO clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_EXTI |RCC_APB1Periph_AFIO, ENABLE);

  /* Connect EXTI0 Line to PA0 pin */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

  /* Configure EXTI0 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Configure and enable EXTI0 interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Configure PB6 in interrupt mode
  * @param  None
  * @return None
  */
void EXTI9_5_Config(void)
{
  /* Enable GPIOB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOB, ENABLE);

  /* Configure PB6 pin as input pull-up */
  GPIO_Init(GPIOB, GPIO_Pin_6, GPIO_MODE_IN |GPIO_PUPD_UP);

  /* Enable AFIO clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_EXTI |RCC_APB1Periph_AFIO, ENABLE);

  /* Connect EXTI6 Line to PB6 pin */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);

  /* Configure EXTI6 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line6;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Configure and enable EXTI9_5 interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  This function handles External line 0 interrupt request.
  * @param  None
  * @return None
  */
void EXTI0_IRQHandler(void)
{
  if(EXTI_GetFlagStatus(EXTI_Line0) != RESET)
  {
    /* Toggle LED1 */
    GPIO_ToggleBits(GPIOB, GPIO_Pin_14);

    /* Clear the EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
}

/**
  * @brief  This function handles External lines 9 to 5 interrupt request.
  * @param  None
  * @return None
  */
void EXTI9_5_IRQHandler(void)
{
  if(EXTI_GetFlagStatus(EXTI_Line6) != RESET)
  {
    /* Toggle LED2 */
    GPIO_ToggleBits(GPIOB, GPIO_Pin_13);

    /* Clear the EXTI line 6 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line6);
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

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void PVD_Config(void);

/* Private functions ---------------------------------------------------------*/

void delay(uint32_t n)
{
  while(n--);
}

int main(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* Enable GPIOB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOB, ENABLE);

  GPIO_Init(GPIOB, GPIO_Pin_13 |GPIO_Pin_14, GPIO_MODE_OUT |GPIO_OTYPE_PP |GPIO_PUPD_NOPULL |GPIO_SPEED_HIGH);
  GPIO_SetBits(GPIOB, GPIO_Pin_13);     // Turn off LED2

  PVD_Config();

  /* Infinite loop */
  while (1)
  {
    /* LED1 blink */
    GPIO_ToggleBits(GPIOB, GPIO_Pin_14);
    delay(1000000);
  }
}

void PVD_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_EXTI, ENABLE);

  /* Disable ANCTL register write-protection */
  PWR_UnlockANA();

  /* Configure the PVD Level to 5 (refer to the electrical characteristics of
     you device datasheet for more details) */
  ANCTL_PVDLevelConfig(ANCTL_PVDLevel_5);

  /* Enable the PVD Output */
  ANCTL_PVDCmd(ENABLE);

  /* Enable ANCTL register write-protection */
  PWR_LockANA();

  delay(3000);

  /* Enable the PVD Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = PVD_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Configure EXTI Line16(PVD Output) to generate an interrupt on rising and
     falling edges */
  EXTI_InitStructure.EXTI_Line = EXTI_Line16;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_ClearITPendingBit(EXTI_Line16);
  EXTI_Init(&EXTI_InitStructure);
}

void PVD_IRQHandler(void)
{
  if(EXTI_GetFlagStatus(EXTI_Line16) != RESET)
  {
    /* Clear the EXTI line 16 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line16);

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

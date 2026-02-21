/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"
#include <stdio.h>
#include "bsp_uart1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t TimeDisplay = 0;
__IO uint32_t LEDToggleCounter = 0;
/* Private function prototypes -----------------------------------------------*/
void RTC_Configuration(void);
void NVIC_Configuration(void);
void Time_Display(uint32_t TimeVar);

/* Private functions ---------------------------------------------------------*/

void delay(uint32_t n)
{
  while(n--);
}

int main(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  uart1_init(8000000, 115200);
  
  /* Enable GPIOB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOB, ENABLE);
  /* Configure PB14 in output push-pull mode */
  GPIO_Init(GPIOB, GPIO_Pin_14, GPIO_MODE_OUT |GPIO_OTYPE_PP |GPIO_PUPD_NOPULL |GPIO_SPEED_HIGH);

  /* Enable BKP clocks */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_BKP, ENABLE);

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);

  /* NVIC configuration */
  NVIC_Configuration();

  if (BKP_ReadBackupRegister(BKP_DR1) != 0x55AA)
  {
    /* Backup data register value is not correct or not yet programmed (when
       the first time the program is executed) */

    printf("RTC not yet configured...\r\n");

    /* RTC Configuration */
    RTC_Configuration();

    printf("RTC configured...\r\n");

    BKP_WriteBackupRegister(BKP_DR1, 0x55AA);
  }
  else
  {
    /* Check if the Power On Reset flag is set */
    if (RCC_GetResetFlagStatus(RCC_RSTFLAG_PORRST) != RESET)
    {
      printf("Power On Reset occurred...\r\n");
    }
    /* Check if the Pin Reset flag is set */
    else if (RCC_GetResetFlagStatus(RCC_RSTFLAG_PINRST) != RESET)
    {
      printf("External Reset occurred...\r\n");
    }

    printf("No need to configure RTC...\r\n");
    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();

    /* Enable the RTC Second */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
    RTC_WaitForLastTask();
  }

  /* Clears the RCC reset flags. */
  RCC_ClearResetFlags();

  /* Infinite loop */
  while (1)
  {
    if (TimeDisplay)
    {
      TimeDisplay = 0;
      Time_Display(RTC_GetCounter());
    }

    if (LEDToggleCounter == 0)
    {
      GPIO_ToggleBits(GPIOB, GPIO_Pin_14);
      LEDToggleCounter = 30000;
    }
    LEDToggleCounter--;
  }
}

/**
  * @brief  Configures the RTC.
  * @param  None
  * @retval None
  */
void RTC_Configuration(void)
{
  /* Reset Backup Domain */
  BKP_DeInit();

  /* Enable LSE */
  BKP_LSEConfig(BKP_LSE_ON);
  /* Wait till LSE is ready */
  while (BKP_GetLSEReadyFlagStatus() == RESET)
  {}

  /* Select LSE as RTC Clock Source */
  BKP_RTCCLKConfig(BKP_RTCCLKSource_LSE);

  /* Enable RTC Clock */
  BKP_RTCCLKCmd(ENABLE);

  /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro();

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Enable the RTC Second */
  RTC_ITConfig(RTC_IT_SEC, ENABLE);

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Set RTC prescaler: set RTC period to 1sec */
  RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Sets the RTC counter */
  RTC_SetCounter(40271);
}

void RTC_IRQHandler(void)
{
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
  {
    /* Clear the RTC Second interrupt */
    RTC_ClearITPendingBit(RTC_IT_SEC);

    /* Enable time update */
    TimeDisplay = 1;
  }
}

/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the RTC Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Displays the current time.
  * @param  TimeVar: RTC counter value.
  * @retval None
  */
void Time_Display(uint32_t TimeVar)
{
  uint32_t THH = 0, TMM = 0, TSS = 0;
  
  /* Reset RTC Counter when Time is 23:59:59 */
  if (RTC_GetCounter() >= 0x0001517F)
  {
     RTC_SetCounter(0x0);
     /* Wait until last write operation on RTC registers has finished */
     RTC_WaitForLastTask();
  }
  
  /* Compute  hours */
  THH = TimeVar / 3600;
  /* Compute minutes */
  TMM = (TimeVar % 3600) / 60;
  /* Compute seconds */
  TSS = (TimeVar % 3600) % 60;

  printf("Time: %0.2d:%0.2d:%0.2d\r", THH, TMM, TSS);
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

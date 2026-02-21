/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
NVIC_InitTypeDef NVIC_InitStructure;
/* Private function prototypes -----------------------------------------------*/
void WriteToBackupReg(uint16_t FirstBackupData);
uint32_t CheckBackupReg(uint16_t FirstBackupData);
uint32_t IsBackupRegReset(void);
/* Private functions ---------------------------------------------------------*/

void delay(uint32_t n)
{
  while(n--);
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Initializes and turn off LED1 and LED2 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 | RCC_APB1Periph_GPIOB, ENABLE);
  GPIO_Init(GPIOB, GPIO_Pin_13 |GPIO_Pin_14, GPIO_MODE_OUT);
  GPIO_SetBits(GPIOB, GPIO_Pin_13 |GPIO_Pin_14);

  /* Enable TAMPER IRQChannel */
  NVIC_InitStructure.NVIC_IRQChannel = TAMPER_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable BKP clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_BKP, ENABLE);

  /* Enable write access to Backup domain */
  PWR_BackupAccessCmd(ENABLE);  

  /* Disable Tamper pin */
  BKP_TamperPinCmd(DISABLE);

  /* Disable Tamper interrupt */
  BKP_ITConfig(DISABLE);

  /* Tamper pin active on low level */
  BKP_TamperPinLevelConfig(BKP_TamperPinLevel_Low);

  /* Clear Tamper pin Event(TE) pending flag */
  BKP_ClearFlag();

  /* Enable Tamper interrupt */
  BKP_ITConfig(ENABLE);

  /* Enable Tamper pin */
  BKP_TamperPinCmd(ENABLE);

  /* Write data to Backup DRx registers */
  WriteToBackupReg(0xA53C);

  /* Check if the written data are correct */
  if(CheckBackupReg(0xA53C) != 0x00)
  {
    /* Blink LED1 */
    while (1)
    {
      GPIO_ToggleBits(GPIOB, GPIO_Pin_14);
      delay(800000);
    }
  }

  while (1)
  {
  }
}

/**
  * @brief  This function handles Tamper interrupt request. 
  * @param  None
  * @retval None
  */
void TAMPER_IRQHandler(void)
{
  if(BKP_GetITStatus() != RESET)
  { /* Tamper detection event occurred */

    /* Check if Backup registers are cleared */
    if(IsBackupRegReset() == 0)
    {/* OK, Backup registers are reset as expected */

      /* Turn on LED2 */
      GPIO_ResetBits(GPIOB, GPIO_Pin_13);
    }
    else
    {/* Backup registers are not reset */
      /* Turn on LED1 */
      GPIO_ResetBits(GPIOB, GPIO_Pin_14);
    }

    /* Clear Tamper pin interrupt pending bit */
    BKP_ClearITPendingBit();

    /* Clear Tamper pin Event(TE) pending flag */
    BKP_ClearFlag();

    /* Disable Tamper pin */
    BKP_TamperPinCmd(DISABLE);
    
    /* Enable Tamper pin */
    BKP_TamperPinCmd(ENABLE);
  }
}

/**
  * @brief  Writes data Backup DRx registers.
  * @param  FirstBackupData: data to be written to Backup data registers.
  * @retval None
  */
void WriteToBackupReg(uint16_t FirstBackupData)
{
  uint32_t index = 0;

  for (index = 0; index < 21; index++)
  {
    ((volatile uint32_t*)&BKP->DR1)[index] = FirstBackupData + (index * 0x5A);
  }
}

/**
  * @brief  Checks if the Backup DRx registers values are correct or not.
  * @param  FirstBackupData: data to be compared with Backup data registers.
  * @retval 
  *         - 0: All Backup DRx registers values are correct
  *         - Value different from 0: Number of the first Backup register
  *           which value is not correct
  */
uint32_t CheckBackupReg(uint16_t FirstBackupData)
{
  uint32_t index = 0;

  for (index = 0; index < 21; index++)
  {
    if (((volatile uint32_t*)&BKP->DR1)[index] != (FirstBackupData + (index * 0x5A)))
    {
      return (index + 1);
    }
  }

  return 0;
}

/**
  * @brief  Checks if the Backup DRx registers are reset or not.
  * @param  None
  * @retval 
  *          - 0: All Backup DRx registers are reset
  *          - Value different from 0: Number of the first Backup register
  *            not reset
  */
uint32_t IsBackupRegReset(void)
{
  uint32_t index = 0;

  for (index = 0; index < 21; index++)
  {
    if (((volatile uint32_t*)&BKP->DR1)[index] != 0x0000)
    {
      return (index + 1);
    }
  }

  return 0;
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

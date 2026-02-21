/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"
#include <stdio.h>
#include "bsp_uart1.h"
#include "drv_es8316.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t audio_buffer[8000 * 2];
uint32_t audio_index = 0;
volatile uint32_t record_flag = 0;

/* Private function prototypes -----------------------------------------------*/
void BSP_I2S_Init(void);
void StartPlay(void);
void StartRecord(void);

/* Private functions ---------------------------------------------------------*/

int main(void)
{
  /* Init UART1 to 115200-8n1 for print message */
  uart1_init(96000000, 115200);

  printf("I2S Record and Play Example\r\n");

  if (es8316_init() != 0)
  {
    printf("Error! ES8316 not detected!!!\r\n");
    while (1);
  }
  printf("ES8316 is detected.\r\n");

  BSP_I2S_Init();

  printf("Enter any key to record...\r\n");
  getchar();

  printf("Start Record...\r\n");
  StartRecord();

  /* Wait for the recording to complete */
  while (record_flag == 0);

  printf("Start playing...\r\n");
  StartPlay();

  /* Infinite loop */
  while (1)
  {
  }
}

void BSP_I2S_Init(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOA |RCC_APB1Periph_GPIOB |RCC_APB1Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_BMX2 |RCC_APB2Periph_I2S, ENABLE);

  I2S_DeInit();
  /*
    PA15 (I2S_WS)
    PB3 (I2S_SCLK)
    PC6 (I2S_MCLK)
    PB15 (I2S_SD0)  Audio data input
    PB5 (I2S_SD1)  Audio data output
  */
  GPIO_Init(GPIOA, GPIO_Pin_15, GPIO_MODE_AF |GPIO_OTYPE_PP |GPIO_PUPD_NOPULL |GPIO_SPEED_HIGH |GPIO_AF3);
  GPIO_Init(GPIOB, GPIO_Pin_3 |GPIO_Pin_5 |GPIO_Pin_15, GPIO_MODE_AF |GPIO_OTYPE_PP |GPIO_PUPD_NOPULL |GPIO_SPEED_HIGH |GPIO_AF3);
  GPIO_Init(GPIOC, GPIO_Pin_6, GPIO_MODE_AF |GPIO_OTYPE_PP |GPIO_PUPD_NOPULL |GPIO_SPEED_HIGH |GPIO_AF3);
}

void StartPlay(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  es8316_config_play_path();

  /*
    target Fs = 8000Hz
    MCLK = 256Fs
    MCLK Divider = 96MHz / MCLK = 47
    MCLK = 96Mhz / 47 = 2042533Hz
    I2S_SCLK = 16*2*Fs
    I2S Divider = MCLK / I2S_SCLK = 8
  */
  RCC_MCLKConfig(RCC_MCLKSource_MAINCLK_Div47, ENABLE);
  RCC_I2SCLKConfig(RCC_MCLK_Div8, ENABLE);

  I2S_DeInit();
  I2S_Cmd(ENABLE);
  I2S_Channel_ITConfig(1, 0xFF, DISABLE);
  I2S_Channel_ITConfig(1, I2S_IT_TXFE, ENABLE);
  I2S_Channel_TransmitConfig(1, I2S_DataFormat_16b);
  I2S_Channel_TransmitFIFOConfig(1, 0);
  I2S_Channel_TransmitCmd(1, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = I2S_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  I2S_TransmitterCmd(ENABLE);
  I2S_ClockConfig(I2S_WordSelectSize_16, I2S_SclkGate_16);
  I2S_ClockCmd(ENABLE);
}

void StartRecord(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  es8316_config_record_path();

  /*
    target Fs = 8000Hz
    MCLK = 256Fs
    MCLK Divider = 96MHz / MCLK = 47
    MCLK = 96Mhz / 47 = 2042533Hz
    I2S_SCLK = 16*2*Fs
    I2S Divider = MCLK / I2S_SCLK = 8
  */
  RCC_MCLKConfig(RCC_MCLKSource_MAINCLK_Div47, ENABLE);
  RCC_I2SCLKConfig(RCC_MCLK_Div8, ENABLE);

  I2S_DeInit();
  I2S_Cmd(ENABLE);
  I2S_Channel_ITConfig(0, 0xFF, DISABLE);
  I2S_Channel_ITConfig(0, I2S_IT_RXDA, ENABLE);
  I2S_Channel_ReceiveConfig(0, I2S_DataFormat_16b);
  I2S_Channel_ReceiveFIFOConfig(0, 0);
  I2S_Channel_ReceiveCmd(0, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = I2S_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  I2S_ReceiverCmd(ENABLE);
  I2S_ClockConfig(I2S_WordSelectSize_16, I2S_SclkGate_16);
  I2S_ClockCmd(ENABLE);
}


void I2S_IRQHandler(void)
{
  if (I2S_Channel_GetITStatus(0, I2S_IT_RXDA) != RESET)
  {
    audio_buffer[audio_index] = I2S_Channel_ReadLeftData(0);
    audio_buffer[audio_index + 1] = I2S_Channel_ReadRightData(0);
    audio_index += 2;
    if (audio_index >= (sizeof(audio_buffer) / sizeof(audio_buffer[0])))
    {
      I2S_DeInit();
      audio_index = 0;
      record_flag = 1;
    }
  }
  if (I2S_Channel_GetITStatus(1, I2S_IT_TXFE) != RESET)
  {
    I2S_Channel_WriteLeftData(1, audio_buffer[audio_index]);
    I2S_Channel_WriteRightData(1, audio_buffer[audio_index + 1]);
    audio_index += 2;
    if (audio_index >= (sizeof(audio_buffer) / sizeof(audio_buffer[0])))
      audio_index = 0;
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

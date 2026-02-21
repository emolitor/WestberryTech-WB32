/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"
#include <stdio.h>
#include "bsp_uart1.h"
#include "drv_es8316.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern const unsigned char wav_data[132344];
const unsigned char* audio_data = 0;
uint32_t audio_data_length = 0;
uint32_t audio_index = 0;

/* Private function prototypes -----------------------------------------------*/
void BSP_I2S_Init(void);
void StartPlay(void);

/* Private functions ---------------------------------------------------------*/

int main(void)
{
  /* Init UART1 to 115200-8n1 for print message */
  uart1_init(96000000, 115200);

  printf("I2S Audio Play Example\r\n");

  audio_data = &wav_data[0x2C];
  audio_data_length = *((volatile uint32_t*)&wav_data[0x28]);
  audio_index = 0;

  if (es8316_init() != 0)
  {
    printf("Error! ES8316 not detected!!!\r\n");
    while (1);
  }
  printf("ES8316 is detected.\r\n");

  BSP_I2S_Init();

  printf("Enter any key to continue...\r\n");
  getchar();

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
    PB5 (I2S_SD1)  Audio data output
  */
  GPIO_Init(GPIOA, GPIO_Pin_15, GPIO_MODE_AF |GPIO_OTYPE_PP |GPIO_PUPD_NOPULL |GPIO_SPEED_HIGH |GPIO_AF3);
  GPIO_Init(GPIOB, GPIO_Pin_3 |GPIO_Pin_5, GPIO_MODE_AF |GPIO_OTYPE_PP |GPIO_PUPD_NOPULL |GPIO_SPEED_HIGH |GPIO_AF3);
  GPIO_Init(GPIOC, GPIO_Pin_6, GPIO_MODE_AF |GPIO_OTYPE_PP |GPIO_PUPD_NOPULL |GPIO_SPEED_HIGH |GPIO_AF3);
}

void StartPlay(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  es8316_config_play_path();

  /*
    target Fs = 22050Hz
    MCLK = 256Fs
    MCLK Divider = 96MHz / MCLK = 17
    MCLK = 96Mhz / 17 = 5647058Hz
    I2S_SCLK = 16*2*Fs
    I2S Divider = MCLK / I2S_SCLK = 8
  */
  RCC_MCLKConfig(RCC_MCLKSource_MAINCLK_Div17, ENABLE);
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

void I2S_IRQHandler(void)
{
  if (I2S_Channel_GetITStatus(1, I2S_IT_TXFE) != RESET)
  {
    I2S_Channel_WriteLeftData(1, (audio_data[audio_index + 1] << 8) | audio_data[audio_index]);
    I2S_Channel_WriteRightData(1, (audio_data[audio_index + 3] << 8) | audio_data[audio_index + 2]);

    audio_index += 4;
    if(audio_index >= audio_data_length)
    {
      audio_index = 0;
    }
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

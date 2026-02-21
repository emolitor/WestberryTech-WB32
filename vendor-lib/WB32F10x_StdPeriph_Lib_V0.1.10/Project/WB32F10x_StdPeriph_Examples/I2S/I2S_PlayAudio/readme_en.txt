/**
  @page I2S_PlayAudio I2S_PlayAudio example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    I2S/I2S_PlayAudio/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the I2S I2S_PlayAudio example.
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, WESTBERRY SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
  * INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM
  * THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
   @endverbatim

@par Example Description 

This example describes how to use the I2S peripheral to play audio.

In this example, Main clock is from PLL clock and it is configured at 96MHz. The PLL clock is based on 8MHz HSE. APB1 and APB2 clock is from Main clock.

In this example, the ES8316 CODEC chip is used to play audio.
When this routine runs, it will print out relevant information through UART1, and enter any character to start audio playback.
When the audio is played, you will hear "left channel" on the left channel and "right channel" on the right channel.

The audio sampling frequency in this example is 22050Hz.


@par Directory contents 

  - I2S/I2S_PlayAudio/I2S_PlayAudio.uvprojx     MDK5 project file
  - I2S/I2S_PlayAudio/I2S_PlayAudio.uvoptx      MDK5 project options file
  - I2S/I2S_PlayAudio/JLinkSettings.ini         Jlink settings file
  - I2S/I2S_PlayAudio/main.c                    Main program
  - I2S/I2S_PlayAudio/drv_es8316.c              ES8316 driver
  - I2S/I2S_PlayAudio/drv_es8316.h              ES8316 driver header file
  - I2S/I2S_PlayAudio/wav_data.c                WAV audio data


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open I2S_PlayAudio.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

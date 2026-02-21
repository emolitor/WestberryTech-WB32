/**
  @page I2S_RecordPlayAudio I2S_RecordPlayAudio example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    I2S/I2S_RecordPlayAudio/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the I2S I2S_RecordPlayAudio example.
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

This example describes how to use the I2S peripheral to record and play audio.

In this example, Main clock is from PLL clock and it is configured at 96MHz. The PLL clock is based on 8MHz HSE. APB1 and APB2 clock is from Main clock.

In this example, the ES8316 CODEC chip is used to record and play audio.
When this routine runs, the relevant information will be printed out through UART1. After entering any character, the program will start to record 1 second of sound from the MIC at a sampling rate of 8kHz.
When the recording is finished, it will start to repeat the sound recorded just now from the earphones and speakers.

The sampling frequency of audio in this example is 8000 Hz.


@par Directory contents 

  - I2S/I2S_RecordPlayAudio/I2S_RecordPlayAudio.uvprojx   MDK5 project file
  - I2S/I2S_RecordPlayAudio/I2S_RecordPlayAudio.uvoptx    MDK5 project options file
  - I2S/I2S_RecordPlayAudio/JLinkSettings.ini             Jlink settings file
  - I2S/I2S_RecordPlayAudio/main.c                        Main program
  - I2S/I2S_RecordPlayAudio/drv_es8316.c                  ES8316 driver
  - I2S/I2S_RecordPlayAudio/drv_es8316.h                  ES8316 driver header file


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open I2S_RecordPlayAudio.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

/**
  @page TIM_InputCapture TIM InputCapture example 
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    TIM/TIM_InputCapture/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the TIM_InputCapture example.
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

This example shows how to use the TIM peripheral to measure the frequency of an 
external signal.\n
The system clock works at 96Mhz and is generated through PLL configuration.
 The PLL clock source is an external 8Mhz crystal oscillator. 
 The frequency division ratio of APB1 is 1, and the frequency division ratio of APB2 is 1.\n

TIMxCLK frequency is set to  SystemCoreClock (Hz)，the Prescaler is 0 \n
TIM1 counter clock is SystemCoreClock (Hz)。\n
SystemCoreClock is set to 96MHz。\n

TIM1 is configured in Input Capture Mode:Channel 2 of TIM1 is connected to an external signal.\n
Use TIM1's CC2 interrupt request to measure the frequency. So when entering TIM1_IRQHandler frequency has been calculated. \n
"TIM1Freq" = External signal frequency:\n
TIM1Freq = TIM1 counter clock / Capture in Hz，\n
Capture is the difference between the two captures of TIM1 captures.\n


@par Directory contents 

  - TIM/TIM_InputCapture/TIM_InputCapture.uvprojx    MDK5 project file
  - TIM/TIM_InputCapture/TIM_InputCapture.uvoptx     MDK5 project options file
  - TIM/TIM_InputCapture/JLinkSettings.ini           Jlink settings file
  - TIM/TIM_InputCapture/main.c                      Main program


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz
    - Pin connection :
      - TIM1_CH1 pin (PA.08) Connect the external signal to be measured.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open TIM_InputCapture.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

/**
  @page TIM_TIM1_PWMOutput TIM TIM1 PWMOutput example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    TIM/TIM_TIM1_PWMOutput/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the TIM_TIM1_PWMOutput example.
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

This example shows how to configure the TIM peripheral in PWM (Pulse Width Modulation) mode.\n
The system clock works at 96Mhz and is generated through PLL configuration. 
The PLL clock source is an external 8Mhz crystal oscillator. 
The frequency division ratio of APB1 is 1, and the frequency division ratio of APB2 is 1.\n
The TIM1CLK frequency is set to SystemCoreClock，to get TIM1 counter
clock at 96MHz，the Prescaler is computed as following: \n
   - Prescaler = (TIM1CLK / TIM1 counter clock) - 1 \n
TIM1 Frequency = TIM1 counter clock / (ARR + 1) \n
               = 96MHz / 65536 = 1.46KHz \n
The TIM1->CCR1 register value is equal to 0x7FFF，so the TIM1 Channel 1 generates a 
PWM signal with a frequency equal to 1.46KHzand a duty cycle equal to 50%:\n
TIM1 Channel1 duty cycle =（TIM1_CCR1 / TIM1_ARR + 1）* 100 = 50% \n
The PWM waveform can be displayed using an oscilloscope. 

@par Directory contents 

  - TIM/TIM_TIM1_PWMOutput/TIM_TIM1_PWMOutput.uvprojx    MDK5 project file
  - TIM/TIM_TIM1_PWMOutput/TIM_TIM1_PWMOutput.uvoptx     MDK5 project options file
  - TIM/TIM_TIM1_PWMOutput/JLinkSettings.ini             Jlink settings file
  - TIM/TIM_TIM1_PWMOutput/main.c                        Main program


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz
    -  Connect the TIM1 pins to an oscilloscope to monitor the different waveforms:
      - TIM1_CH1 pin  (PA.08)
  

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open TIM_TIM1_PWMOutput.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

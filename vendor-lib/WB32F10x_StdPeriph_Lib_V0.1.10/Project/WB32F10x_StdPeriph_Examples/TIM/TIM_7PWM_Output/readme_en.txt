/**
  @page TIM_7PWM_Output TIM 7PWM Output example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    TIM/TIM_7PWM_Output/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the TIM_7PWM_Output example.
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

This example shows how to configure the TIM1 peripheral to generate 7 PWM signals 
with 4 different duty cycles (50%, 37.5%, 25% and 12.5%).

In this example, Main clock is from PLL clock and it is configured at 96MHz. The PLL clock is based on 8MHz HSE. APB1 and APB2 clock is from Main clock.

The objective is to generate 7 PWM signal at 1.46 KHz:
  - TIM1_Period = (SystemCoreClock / 17570) - 1
The channel 1 and channel 1N duty cycle is set to 50%
The channel 2 and channel 2N duty cycle is set to 37.5%
The channel 3 and channel 3N duty cycle is set to 25%
The channel 4 duty cycle is set to 12.5%
The Timer pulse is calculated as follows:
  - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100

The TIM1 waveform can be displayed using an oscilloscope.


@par Directory contents 

  - TIM/TIM_7PWM_Output/TIM_7PWM_Output.uvprojx    MDK5 project file
  - TIM/TIM_7PWM_Output/TIM_7PWM_Output.uvoptx     MDK5 project options file
  - TIM/TIM_7PWM_Output/JLinkSettings.ini          Jlink settings file
  - TIM/TIM_7PWM_Output/main.c                     Main program


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz
    - Connect the TIM1 pins to an oscilloscope to monitor the different waveforms:
      - TIM1_CH1  pin (PA8)
      - TIM1_CH1N pin (PB13)
      - TIM1_CH2  pin (PA9)
      - TIM1_CH2N pin (PB14)
      - TIM1_CH3  pin (PA10)
      - TIM1_CH3N pin (PB15)
      - TIM1_CH4  pin (PB11)


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open TIM_7PWM_Output.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

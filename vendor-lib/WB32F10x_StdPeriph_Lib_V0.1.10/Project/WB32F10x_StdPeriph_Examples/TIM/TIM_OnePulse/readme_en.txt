/**
  @page TIM_OnePulse TIM OnePulse example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    TIM/TIM_OnePulse/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the TIM_OnePulse example.
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

This example shows how to use the TIM peripheral to generate a One pulse Mode 
after a Rising edge of an external signal is received in Timer Input pin.\n
The system clock works at 96Mhz and is generated through PLL configuration. 
The PLL clock source is an external 8Mhz crystal oscillator.
 The frequency division ratio of APB1 is 1, and the frequency division ratio of APB2 is 1.\n
TIM1CLK = SystemCoreClock：\n
  -Prescaler = 0 \n
The Autoreload value is 65535（TIM1->ARR），so the maximum frequency value to 
trigger the TIM1 input is 96000000 / 65535 = 1464Hz。\n
The TIM1 is configured as follows：\n
The One Pulse mode is used, the external signal is connected to TIM1_CH2 pin (PA.09), 
the rising edge is used as active edge, the One Pulse signal is output 
on TIM1_CH1 (PA.08).\n

The TIM_Pulse defines the delay value, the delay value is fixed to:\n
delay = CCR1  / TIM1 counter clock\n
      = 16383 / 96000000 = 170.65 us。\n
The （TIM_period - TIM_pulse）defines the One Pulse value, the pulse value is fixed to:\n
One Pulse value =（Tim_Period - Tim_Pulse）/ Tim1 counter clock  \n
           =（65535    - 16383）   / 96000000 = 512us。\n 

@par Directory contents 

  - TIM/TIM_OnePulse/TIM_OnePulse.uvprojx    MDK5 project file
  - TIM/TIM_OnePulse/TIM_OnePulse.uvoptx     MDK5 project options file
  - TIM/TIM_OnePulse/JLinkSettings.ini     Jlink settings file
  - TIM/TIM_OnePulse/main.c                Main program


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz
    - Pin connection:
      - TIM1_CH1 pin  (PA.08) Connect to oscilloscope
      - TIM1_CH2 pin  (PA.09) Connect to external signal

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open TIM_OnePulse.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

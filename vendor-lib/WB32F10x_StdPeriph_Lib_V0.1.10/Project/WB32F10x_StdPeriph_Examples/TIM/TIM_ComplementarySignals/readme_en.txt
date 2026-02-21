/**
  @page TIM_ComplementarySignals TIM ComplementarySignals example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    TIM/TIM_ComplementarySignals/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the TIM_ComplementarySignals example.
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

This example shows how to configure the TIM1 peripheral to generate three 
complementary TIM1 signals, to insert a defined dead time value, to use the break 
feature and to lock the desired parameters.\n
The system clock works at 96Mhz and is generated through PLL configuration. 
The PLL clock source is an external 8Mhz crystal oscillator. 
The frequency division ratio of APB1 is 1, and the frequency division ratio of APB2 is 1.\n
TIM1CLK is fixed to SystemCoreClock, the TIM1 Prescaler is equal to 0 so the 
TIM1 counter clock used is SystemCoreClock.\n

The objective is to generate PWM signal at 1.46 KHz:\n
  -TIM1_Period  =（SystemCoreClock / 65536）\n

The Three Duty cycles are computed as the following description: \n
The channel 1 duty cycle is set to 50% so channel 1N is set to 50%.\n
The channel 2 duty cycle is set to 50% so channel 2N is set to 50%.\n
The channel 3 duty cycle is set to 50% so channel 3N is set to 50%.\n

A dead time equal to 11/SystemCoreClock is inserted between the different 
complementary signals, and the Lock level 1 is selected.
The break Polarity is used at High level.\n

@par Directory contents 

  - TIM/TIM_ComplementarySignals/TIM_ComplementarySignals.uvprojx    MDK5 project file
  - TIM/TIM_ComplementarySignals/TIM_ComplementarySignals.uvoptx     MDK5 project options file
  - TIM/TIM_ComplementarySignals/JLinkSettings.ini                   Jlink settings file
  - TIM/TIM_ComplementarySignals/main.c                              Main program


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz
    - Connect the TIM1 pin to the oscilloscope to monitor different waveforms:
      - TIM1_CH1  pin (PA.08)
      - TIM1_CH1N pin (PB.13)  
      - TIM1_CH2  pin (PA.09)  
      - TIM1_CH2N pin (PB.15)
      - TIM1_CH3  pin (PA.10)  
      - TIM1_CH3N pin (PB.14)    
    - Connect TIM1 break pin TIM1_BKIN pin (PA.06) to the VCC to generate Break.
  

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open TIM_ComplementarySignals.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

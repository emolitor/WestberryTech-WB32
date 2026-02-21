/**
  @page TIM_TIM1_OCToggle TIM TIM1 OCToggle example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    TIM/TIM_TIM1_OCToggle/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the TIM_TIM1_OCToggle example.
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

This example shows how to configure the TIM1 peripheral to generate two different 
signals with two different frequencies.\n
The system clock works at 96Mhz and is generated through PLL configuration. 
The PLL clock source is an external 8Mhz crystal oscillator. 
The frequency division ratio of APB1 is 1, and the frequency division ratio of APB2 is 1.\n
The TIM1CLK frequency is set to SystemCoreClock，and we want to get TIM1
counter clock at 96 MHz,so the Prescaler is computed as following:\n
  -Prescaler=（TIM1CLK / TIM1 counter clock）- 1 \n
The TIM1->CCR1 register value is equal to 32768：\n
CC1 update rate = TIM1 counter clock / CCR1VAL = 1.46KHz，\n
so the TIM1 Channel 1 generates a periodic signal with a frequency equal to1.46KHz.\n
The TIM1->CCR2 register value is equal to 16384：\n
CC2 update rate = TIM1 counter clock  / CCR2VAL = 2.92KHz \n
so the TIM1 Channel 2 generates a periodic signal with a frequency equal to 2.92KHz.\n
The TIM1->CCR3 register value is equal to 16384：\n
CC3 update rate  = TIM1 counter clock / CCR3VAL = 5.85KHz \n
so the TIM1 Channel 3 generates a periodic signal with a frequency equal to 5.85KHz.\n
The TIM1->CCR4 register value is equal to 16384：\n
CC4 update rate = TIM1 counter clock/ CCR4VAL = 11.7KHz \n
so the TIM1 Channel 4 generates a periodic signal with a frequency equal to 11.7KHz.\n

@par Directory contents 

  - TIM/TIM_TIM1_OCToggle/TIM_TIM1_OCToggle.uvprojx    MDK5 project file
  - TIM/TIM_TIM1_OCToggle/TIM_TIM1_OCToggle.uvoptx     MDK5 project options file
  - TIM/TIM_TIM1_OCToggle/JLinkSettings.ini            Jlink settings file
  - TIM/TIM_TIM1_OCToggle/main.c                       Main program


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz
    - Connect the TIM1 pins to an oscilloscope to monitor the different waveforms:
      - TIM1_CH1 pin (PA.08)
      - TIM1_CH2 pin (PA.09)  
      - TIM1_CH3 pin (PA.10)  
      - TIM1_CH4 pin (PA.11)      

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open TIM_TIM1_OCToggle.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

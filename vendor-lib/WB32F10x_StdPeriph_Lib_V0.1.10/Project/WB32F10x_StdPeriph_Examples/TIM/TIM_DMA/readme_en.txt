/**
  @page TIM_DMA TIM DMA example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    TIM/TIM_DMA/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the TIM_DMA example.
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

This example describes how to use DMA and TIM1 update request to transfer data from memory to TIM1->CCR2.\n
The system clock works at 96Mhz and is generated through PLL configuration. 
The PLL clock source is an external 8Mhz crystal oscillator. 
The frequency division ratio of APB1 is 1, and the frequency division ratio of APB2 is 1.\n
TIM1CLK = SystemCoreClock，Prescaler = 0，TIM1 counter clock = SystemCoreClock。\n
The objective is to configure TIM1 channel 3 to generate complementary PWM
signal with a frequency equal to 375 KHz:\n

@par Directory contents 

  - TIM/TIM_DMA/TIM_DMA.uvprojx       MDK5 project file
  - TIM/TIM_DMA/TIM_DMA.uvoptx        MDK5 project options file
  - TIM/TIM_DMA/JLinkSettings.ini     Jlink settings file
  - TIM/TIM_DMA/main.c                Main program


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz
    - Connect the TIM1 pin to the oscilloscope to monitor the waveform:
      - TIM1_CH2  pin (PA.09)    
  

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open TIM_DMA.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

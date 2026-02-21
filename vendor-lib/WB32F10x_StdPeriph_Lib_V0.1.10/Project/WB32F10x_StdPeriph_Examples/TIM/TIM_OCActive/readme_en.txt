/**
  @page TIM_OCActive TIM OCActive example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    TIM/TIM_OCActive/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the TIM_OCActive example.
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

This example shows how to configure the TIM peripheral to generate four different 
signals with four different delays.\n
The system clock works at 96Mhz and is generated through PLL configuration. 
The PLL clock source is an external 8Mhz crystal oscillator. 
The frequency division ratio of APB1 is 1, and the frequency division ratio of APB2 is 1.\n

TIM1CLK frequency is set to SystemCoreClock (Hz),and the objective is
to get TIM3 counter clock at 1MHz so the Prescaler is computed as following:\n
   - Prescaler = (96000000 / 1000000) - 1\n

The TIM1 CCR1 register value is equal to 800:\n
TIM1_CH1 delay = CCR1_Val / TIM1 counter clock  = 800 us \n
so the TIM1channel1 generates a signal with a delay equal to 800us.\n

The TIM1 CCR2 register value is equal to 400:\n
TIM1_CH2 delay = CCR2_Val / TIM1 counter clock  = 400 us \n
so the TIM1 Channel2 generates a signal with a delay equal to 400us.\n

The TIM1 CCR3 register value is equal to 200:
TIM1_CH3 delay = CCR3_Val / TIM1 counter clock  = 200 us \n
so the TIM1 Channel3 generates a signal with a delay equal to 200us.\n

The TIM1 CCR4 register value is equal to 100:\n
TIM1_CH4 delay = CCR4_Val / TIM1 counter clock  = 100 us \n
so the TIM1 Channel4 generates a signal with a delay equal to100us.\n

@par Directory contents 

  - TIM/TIM_OCActive/TIM_OCActive.uvprojx    MDK5 project file
  - TIM/TIM_OCActive/TIM_OCActive.uvoptx     MDK5 project options file
  - TIM/TIM_OCActive/JLinkSettings.ini     Jlink settings file
  - TIM/TIM_OCActive/main.c                Main program


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz
    -Connect the TIM1 pins to an oscilloscope to monitor the different waveforms:
      - TIM1_CH1 pin  (PA.08)
      - TIM1_CH2  pin (PA.09)  
      - TIM1_CH3  pin (PA.10)  
      - TIM1_CH4  pin (PA.11)    

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open TIM_OCActive.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

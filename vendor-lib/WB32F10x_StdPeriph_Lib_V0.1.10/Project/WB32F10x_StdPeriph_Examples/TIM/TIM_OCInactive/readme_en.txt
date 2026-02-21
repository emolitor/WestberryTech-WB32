/**
  @page TIM_OCInactive TIM OCInactive example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    TIM/TIM_OCInactive/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the TIM_OCInactive example.
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

This example shows how to configure the TIM peripheral in Output Compare Inactive 
mode with the corresponding Interrupt requests for each channel.\n
The system clock works at 96Mhz and is generated through PLL configuration. 
The PLL clock source is an external 8Mhz crystal oscillator. 
The frequency division ratio of APB1 is 1, and the frequency division ratio of APB2 is 1.\n

The TIM1CLK frequency is set to SystemCoreClock / 96（Hz）, and the objective is
to get TIM2 counter clock at 1 KHz so the Prescaler is computed as following:\n
  -Prescaler=（TIM1CLK / TIM1 counter clock）- 1\n
  
The TIM2 CCR1 register value is equal to 800:\n
TIM1_CC1 delay = CCR1_VAL/TIM1 counter clock=800us \n
so the PA.08 is reset after a delay equal to 800 us.\n

The TIM1 CCR2 register value is equal to 400：\n
TIM1_CC2 delay = CCR2_VAL/TIM1 counter clock=400us \n
so the PA.09 is reset after a delay equal to 400 us.\n

The TIM1 CCR3 register value is equal to 200：\n
TIM1_CC3 delay = CCR3_VAL/TIM1 counter clock=200us \n
so the PA.10 is reset after a delay equal to 200 us.\n

The TIM1 CCR4 register value is equal to 100：\n
TIM1_CC4 delay = CCR4_VAL/TIM1 counter clock=100us \n
so the PA.11 is reset after a delay equal to 100 us. \n


@par Directory contents 

  - TIM/TIM_OCInactive/TIM_OCInactive.uvprojx    MDK5 project file
  - TIM/TIM_OCInactive/TIM_OCInactive.uvoptx     MDK5 project options file
  - TIM/TIM_OCInactive/JLinkSettings.ini     Jlink settings file
  - TIM/TIM_OCInactive/main.c                Main program


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz
    - Connect the TIM1 pin to the oscilloscope to monitor different waveforms:
      - TIM1_CH1 pin (PA.08)
      - TIM1_CH2 pin (PA.09)  
      - TIM1_CH3 pin (PA.10)  
      - TIM1_CH4 pin (PA.11)    
  

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open TIM_OCInactive.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

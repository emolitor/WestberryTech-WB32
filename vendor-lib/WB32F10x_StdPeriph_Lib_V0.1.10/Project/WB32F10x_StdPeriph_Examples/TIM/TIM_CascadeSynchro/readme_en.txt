/**
  @page TIM_CascadeSynchro TIM_CascadeSynchro example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    TIM/TIM_CascadeSynchro/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the TIM_CascadeSynchro example.
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

This example shows how to synchronize TIM peripherals in cascade mode.

In this example three timers are used:

Timers synchronisation in cascade mode:

1/TIM2 is configured as Master Timer:
 - PWM Mode is used
 - The TIM2 Update event is used as Trigger Output  

2/TIM3 is slave for TIM2 and Master for TIM4,
 - PWM Mode is used
 - The ITR1(TIM2) is used as input trigger 
 - External Clock Mode 1 is used, so clock of slave counter
	  are controlled by the Master trigger output signal(TIM2 update event).
 - The TIM3 Update event is used as Trigger Output. 

3/TIM4 is slave for TIM3,
 - PWM Mode is used
 - The ITR2(TIM3) is used as input trigger
 - External Clock Mode 1 is used, so clock of slave counter are controlled by the
   Master trigger output signal(TIM3 update event).

In this example, TIMXCLK is fixed at 96MHz, and the TIM2 counter clock is 96MHz. \n
The main timer TIM2 runs at TIM2 frequency:\n
TIM2 frequency = (TIM2 counter clock) / (TIM2 period + 1) = 375kHz, duty cycle = TIM2_CCR1 / (TIM2_ARR + 1) = 25%. \n
TIM3 frequency = (TIM3 counter clock) / (TIM3 period + 1) = 93.75kHz, duty cycle = TIM3_CCR1 / (TIM3_ARR + 1) = 25%. \n
TIM4 frequency = (TIM4 counter clock) / (TIM4 period + 1) = 23.4375kHz, duty cycle = TIM4_CCR1 / (TIM4_ARR + 1) = 25%. \n


@par Directory contents 

  - TIM/TIM_CascadeSynchro/TIM_CascadeSynchro.uvprojx    MDK5 project file
  - TIM/TIM_CascadeSynchro/TIM_CascadeSynchro.uvoptx     MDK5 project options file
  - TIM/TIM_CascadeSynchro/JLinkSettings.ini             Jlink settings file
  - TIM/TIM_CascadeSynchro/main.c                        Main program


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz
    - Connect the following pins to an oscilloscope to monitor the different waveforms:
      - TIM2_CH1 (PA0)
      - TIM3_CH1 (PA6)
      - TIM4_CH1 (PB0)


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open TIM_CascadeSynchro.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

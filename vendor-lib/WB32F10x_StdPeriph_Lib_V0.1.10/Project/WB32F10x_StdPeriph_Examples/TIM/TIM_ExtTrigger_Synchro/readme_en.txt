/**
  @page TIM_ExtTrigger_Synchro TIM ExtTrigger Synchro example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    TIM/TIM_ExtTrigger_Synchro/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the TIM_ExtTrigger_Synchro example.
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

This example shows how to synchronize TIM peripherals with external triggers in cascade mode.\n
The system clock works at 96Mhz and is generated through PLL configuration. 
The PLL clock source is an external 8Mhz crystal oscillator. 
The frequency division ratio of APB1 is 1, and the frequency division ratio of APB2 is 1.\n
In this example, three timers are used:\n
1/TIM1 is configured as the main timer:\n
  -Toggle Mode is used \n
  -TIM1 ENABLE event used as trigger output\n

2/TIM1 is configured as Slave Timer for an external Trigger connected to TIM1.\n
  TI2 pin (TIM1 CH2 configured as input pin):\n
  -The TIM1 TI2FP2 is used as Trigger Input\n
  -Rising edge is used to start and stop the TIM1: Gated Mode.\n

3/TIM3 is slave for TIM1 and Master for TIM4,\n
  -Toggle Mode is used \n
  -The ITR1(TIM1) is used as input trigger  \n
  -Gated mode is used, so start and stop of slave counter
    are controlled by the Master trigger output signal(TIM1 enable event).\n
-The TIM3 enable event is used as Trigger Output.\n

4/TIM4 is slave for TIM3,\n
  -Toggle Mode is used \n
  -The ITR2(TIM3) is used as input trigger\n
  -Gated mode is used, so start and stop of slave counter
    are controlled by the Master trigger output signal(TIM3 enable event).\n

The running time of the three timers is:\n
TIMx frequency=TIMx lock counter /（TIMx_perio + 1）= 732.2Hz。\n
The starts and stops of the TIM1 counters are controlled by the external trigger.\n
The TIM3 starts and stops are controlled by the TIM1, and the TIM4 starts and 
stops are controlled by the TIM3. \n

@par Directory contents 

  - TIM/TIM_ExtTrigger_Synchro/TIM_ExtTrigger_Synchro.uvprojx    MDK5 project file
  - TIM/TIM_ExtTrigger_Synchro/TIM_ExtTrigger_Synchro.uvoptx     MDK5 project options file
  - TIM/TIM_ExtTrigger_Synchro/JLinkSettings.ini                 Jlink settings file
  - TIM/TIM_ExtTrigger_Synchro/main.c                            Main program


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz
    - Connect the TIM1 pin to the oscilloscope to monitor the waveform::
      - TIM1_CH1 pin (PA.08)
      - TIM3_CH1 pin (PA.06)  
      - TIM4_CH1 pin (PB.06) 
    - Connect TIM1_CH2 pin (PA.09) ExtTrigger Signal。
  

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open TIM_ExtTrigger_Synchro.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

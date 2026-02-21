/**
  @page TIM_PWM_Input TIM PWM Input example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    TIM/TIM_PWM_Input/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the TIM_PWM_Input example.
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

This example shows how to use the TIM peripheral to measure the frequency and 
duty cycle of an external signal. \n
The system clock works at 96Mhz and is generated through PLL configuration. 
The PLL clock source is an external 8Mhz crystal oscillator. 
The frequency division ratio of APB1 is 1, and the frequency division ratio of APB2 is 1.\n
The TIMXCLK frequency is set to SystemCoreClock（Hz），the Prescaler is 0，so the TIM1 counter clock is SystemCoreClock（Hz).\n 
SystemCoreClock is set to 96MHz。\n
TIM1 is configured in PWM Input Mode: the external signal is connected to 
TIM1 Channel1 used as input pin.\n
To measure the frequency and the duty cycle we use the TIM1 CC1 interrupt request,\n
so In the TIM1_IRQHandler routine, the frequency and the duty cycle of the external 
signal are computed. \n
The "Frequency" variable contains the external signal frequency:\n
Frequency = TIM1 counter clock / (TIM1_CCR1 + 1)，unit：Hz，\n
The "DutyCycle" variable contains the external signal duty cycle:\n
DutyCycle = ((float)(IC2Value + 1) * 100) / (IC1Value + 1);\n
The minimum frequency value to measure is92Hz。\n

@par Directory contents 

  - TIM/TIM_PWM_Input/TIM_PWM_Input.uvprojx    MDK5 project file
  - TIM/TIM_PWM_Input/TIM_PWM_Input.uvoptx     MDK5 project options file
  - TIM/TIM_PWM_Input/JLinkSettings.ini        Jlink settings file
  - TIM/TIM_PWM_Input/main.c                   Main program


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz
    - Pin connection:
      - TIM1_CH1 pin  (PA.08) Connect to sample pin

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open TIM_PWM_Input.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

/**
  @page NVIC_IRQ_Mask NVIC IRQ Mask example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    NVIC/NVIC_IRQ_Mask/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the NVIC IRQ Mask example.
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

This example describes the use of the Nested Vectored Interrupt Controller (NVIC) 
IRQ Channels configuration and how to mask/activate different IRQs.\n
The system clock works at 96Mhz and is generated through PLL configuration. 
The PLL clock source is an external 8Mhz crystal oscillator. 
The frequency division ratio of APB1 is 1, and the frequency division ratio of APB2 is 1. \n
-Configure 3 TIM (TIM2..TIM4) timers to generate interrupts on each counter update event. \n
- The three timers are linked to their correspondant Update IRQ channel.\n
-Assignment of a ascendant IRQ priority for each IRQ channel :\n 
-TIM2 has a preemption priority of 0.\n
-TIM3 has a preemption priority of 1.\n
-TIM4 has a preemption priority of 2.\n
-In each interrupt routine: \n
-TIM2 toggles PA8\n
-TIM3 toggles PA9 \n
-TIM4 toggles PA10 \n

- The KEY and WAKEUP buttons are used to boost the execution priority as follows:\n
-The KEY button is used in GPIO mode and at each KEY button press, the execution 
    priority is raised to 0 and turn LED4 ON. This prevents all exceptions with 
    configurable priority from activating, other than through the HardFault fault 
    escalation mechanism. As consequence, all LEDs stop toggling as TIM2, TIM3 
    and TIM4 IRQs are prevented from activation. 。\n
Pressing again the KEY button will release the priority boosting, turn LED2 
    OFF and will allow all exceptions with configurable priority to be activated 
    and TIM2, TIM3 and TIM4 can be generated again and the LEDs restart toggling.\n
This execution priority is made using the CMSIS functions "__disable_irq()" 
    and "__enable_irq()". 
   These two functions are managing the Cortex-M3 PRIMASK special register.\n
  - The WAKEUP button is used in EXTI mode and at each WAKEUP button press, the execution 
    priority is masked to 0x40 using the BASEPRI register.\n
    A non-zero value will act as a priority mask, affecting the execution priority 
    when the priority defined by BASEPRI is the same or higher than the current 
    executing priority. \n
As consequence, PA9 and PA10 stop toggling as TIM3 and TIM4 IRQs are 
    prevented from activation. \n
Pressing again the WAKEUP button will configure the BASEPRI register to 0, 
    thus it has no effect on the current priority and TIM3 and TIM4 can be 
    generated again and PA9 and PA10 restart toggling.\n
This execution priority is made using the CMSIS functions "__set_rasepri()". \n
This function is managing the Cortex-M3 BASEPRI special register.\n
Setting the BASEPRI register has no effect when the execution priority is
    raised to 0 using the "__disable_irq()" function.\n

@par Directory contents 

  - NVIC/NVIC_IRQ_Mask/NVIC_IRQ_Mask.uvprojx    MDK5 project file
  - NVIC/NVIC_IRQ_Mask/NVIC_IRQ_Mask.uvoptx     MDK5 project options file
  - NVIC/NVIC_IRQ_Mask/JLinkSettings.ini        Jlink settings file
  - NVIC/NVIC_IRQ_Mask/main.c                   Main program

@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz
    - The chip pins are connected as follows:
      - PA8  Pin connected to oscilloscope
      - PA9  Pin connected to oscilloscope
      - PA10 Pin connected to oscilloscope
      - PB11 Pin connected to LED2
      - PA0  Pin connected to WakeUp button（KEY1）
      - PA1  Pin connected to KEY2

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open NVIC_IRQ_Mask.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

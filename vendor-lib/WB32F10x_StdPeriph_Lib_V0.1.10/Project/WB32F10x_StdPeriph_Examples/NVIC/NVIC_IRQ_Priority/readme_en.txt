/**
  @page NVIC_IRQ_Priority NVIC IRQ Priority example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    NVIC/NVIC_IRQ_Priority/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the NVIC_IRQ_Priority example.
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

This example describes the use of the Nested Vectored Interrupt Controller (NVIC).\n
The system clock works at 96Mhz and is generated through PLL configuration. 
The PLL clock source is an external 8Mhz crystal oscillator. 
The frequency division ratio of APB1 is 1, and the frequency division ratio of APB2 is 1. \n
  - Configuration of 2 EXTI Lines (Wakeup button EXTI Line & Key button EXTI Line)
  to generate an interrupt on each falling edge and use the SysTick interrupt.\n
  -These interrupts are configured with the following parameters:\n

  -Wakeup button EXTI Line: \n
  -Preemptionpriority = Preemptionpriorityvalue \n
  -SubPriority = 0 \n

  -Key button EXTI Line: \n
  -Preempt priority = 0 \n
  -SubPriority = 1 \n

  -SysTick Handler：\n
  -PreemptionPriority = !PreemptionPriorityValue \n
  -SubPriority = 0 \n

First, the PreemptionPriorityValue is equal to 0, the Wakeup button EXTI Line 
has higher preemption priority than the SysTick handler. \n
In the key button EXTI Line interrupt routine the Wakeup button EXTI Line and 
SysTick preemption priorities are inverted. \n
In the Wakeup button EXTI Line interrupt routine the pending bit of the SysTick 
interrupt is set this will cause SysTick ISR to preempt the Wakeup button EXTI 
Line ISR only if it has higher preemption priority.\n
The system behaves as following:\n
1) The first time Key button EXTI Line interrupt occurs the SysTick preemption 
become higher than Wakeup button EXTI Line one. So when the Wakeup button EXTI 
Line interrupt occurs, the SysTick ISR is executed and the PreemptionOccured 
variable become TRUE and led1  start toggling.\n
2）When the next Key button EXTI Line interrupt occurs the SysTick preemption
become lower than Wakeup button EXTI Line one. So when the Wakeup button EXTI Line
interrupt occurs, the PreemptionOccured variable became FALSE and  led1
stop toggling.

@par Directory contents 

  - NVIC/NVIC_IRQ_Priority/NVIC_IRQ_Priority.uvprojx    MDK5 project file
  - NVIC/NVIC_IRQ_Priority/NVIC_IRQ_Priority.uvoptx     MDK5 project options file
  - NVIC/NVIC_IRQ_Priority/JLinkSettings.ini            Jlink settings file
  - NVIC/NVIC_IRQ_Priority/main.c                       Main program

@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz
    - The chip pins are connected as follows:
      - PA0  pin is connected to wakeup key
      - PA1  pin is connected to key2
      - PB14 pin is connected to LED1

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open NVIC_IRQ_Priority.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

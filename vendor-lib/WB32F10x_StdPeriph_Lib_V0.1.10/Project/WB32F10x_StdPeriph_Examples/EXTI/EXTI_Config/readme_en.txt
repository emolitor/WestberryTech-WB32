/**
  @page EXTI_Config EXTI_Config example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    EXTI/EXTI_Config/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the EXTI EXTI_Config example.
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

This example describes how to configure external interrupt lines.

In this example, Main clock is from PLL clock and it is configured at 96MHz. The PLL clock is based on 8MHz HSE. APB1 and APB2 clock is from Main clock.

In this example, EXTI0 is configured to generate an interrupt on the rising edge. EXTI6 is configured to generate an interrupt on the falling edge.

In this example:
 - EXTI0 is mapped to PA0
 - EXTI6 is mapped to PB6

After EXTI configuration, a software interrupt is generated on the EXTI0 toggles LED1.
After that,
  when rising edge is detected on EXTI0, LED1 toggles
  when falling edge is detected on EXTI6, LED2 toggles


@par Directory contents 

  - EXTI/EXTI_Config/EXTI_Config.uvprojx    MDK5 project file
  - EXTI/EXTI_Config/EXTI_Config.uvoptx     MDK5 project options file
  - EXTI/EXTI_Config/JLinkSettings.ini      Jlink settings file
  - EXTI/EXTI_Config/main.c                 Main program


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz
    - The chip pins are connected as follows：
      - PB14 pin is connected to LED1
      - PB13 pin is connected to LED2
      - PA0 pin is connected to KEY1
      - PB6 pin is connected to KEY2


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open EXTI_Config.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

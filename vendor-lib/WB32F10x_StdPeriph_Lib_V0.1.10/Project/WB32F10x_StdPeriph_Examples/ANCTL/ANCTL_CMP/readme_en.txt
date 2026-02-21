/**
  @page ANCTL_CMP ANCTL CMP example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    ANCTL/ANCTL_CMP/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the ANCTL_CMP example.
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

This example describes how to use the analog comparator.

In this example, Main clock is from PLL clock and it is configured at 96MHz. The PLL clock is based on 8MHz HSE. APB1 and APB2 clock is from Main clock.

The program behaves as follows:
1. Configures PB4 and PB6 as analog mode.
2. Configures PB4 as CMPA positive input and PB6 as CMPA negative input.
3. If the positive input voltage of CMPA is greater than the negative input voltage, LED1 turn on and LED2 turn off. Otherwise, LED1 turn off and LED2 turn on.


@par Directory contents 

  - ANCTL/ANCTL_CMP/ANCTL_CMP.uvprojx    MDK5 project file
  - ANCTL/ANCTL_CMP/ANCTL_CMP.uvoptx     MDK5 project options file
  - ANCTL/ANCTL_CMP/JLinkSettings.ini    Jlink settings file
  - ANCTL/ANCTL_CMP/main.c               Main program


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz
    - The chip pins are connected as follows：
      - PB14 pin is connected to LED1
      - PB13 pin is connected to LED2
      - PB4 pin is connected to an analog signal
      - PB6 pin is connected to another analog signal


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open ANCTL_CMP.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */
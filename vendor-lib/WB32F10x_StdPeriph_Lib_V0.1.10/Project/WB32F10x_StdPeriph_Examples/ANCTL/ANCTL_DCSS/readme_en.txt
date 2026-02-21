/**
  @page ANCTL_DCSS ANCTL DCSS example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    ANCTL/ANCTL_DCSS/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the ANCTL_DCSS example.
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

This example describes how to use the HSE clock failure detection feature.

In this example, Main clock is from PLL clock and it is configured at 96MHz. The PLL clock is based on 8MHz HSE. APB1 and APB2 clock is from Main clock.

Once the DCSS is enabled and if the HSE clock fails, the DCSS interrupt occurs and an NMI is automatically generated.
In this example, LED1 blink when the DCSS event does not occur, LED2 blink after the DCSS event occurs.


@par Directory contents 

  - ANCTL/ANCTL_DCSS/ANCTL_DCSS.uvprojx    MDK5 project file
  - ANCTL/ANCTL_DCSS/ANCTL_DCSS.uvoptx     MDK5 project options file
  - ANCTL/ANCTL_DCSS/JLinkSettings.ini     Jlink settings file
  - ANCTL/ANCTL_DCSS/main.c                Main program


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz
    - The chip pins are connected as follows：
      - PB14 pin is connected to LED1
      - PB13 pin is connected to LED2


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open ANCTL_DCSS.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */
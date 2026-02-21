/**
  @page PWR_SLEEP PWR_SLEEP example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    PWR/PWR_SLEEP/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the PWR PWR_SLEEP example.
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

This example describes how to enters the SLEEP mode and wake-up from this mode using external interrupt.

In this example, Main clock is from PLL clock and it is configured at 96MHz. The PLL clock is based on 8MHz HSE. APB1 and APB2 clock is from Main clock.

This example turns on LED1 and turns off LED2 at frist;After waiting for a period of time,  PA0 is configured as an external interrupt pin and enter the SLEEP mode.
Wake-up the SLEEP mode in external interrupt by pressing a button connected to PA0 pin.
The program continues to run, and LED1 is toggling.

@par Directory contents 

  - PWR/PWR_SLEEP/PWR_SLEEP.uvprojx     MDK5 project file
  - PWR/PWR_SLEEP/PWR_SLEEP.uvoptx      MDK5 project options file
  - PWR/PWR_SLEEP/JLinkSettings.ini     Jlink settings file
  - PWR/PWR_SLEEP/main.c                Main program


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz
    - The chip pins are connected as follows：
      - PB14 pin is connected to LED1
      - PB13 pin is connected to LED2
      - PA0 pin is connected to KEY1


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open PWR_SLEEP.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

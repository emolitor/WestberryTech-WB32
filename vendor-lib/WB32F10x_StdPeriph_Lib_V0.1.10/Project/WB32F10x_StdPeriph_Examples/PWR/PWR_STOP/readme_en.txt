/**
  @page PWR_STOP PWR_STOP example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    PWR/PWR_STOP/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the PWR PWR_STOP example.
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

This example describes how to enters the STOP mode and wake-up from this mode using external interrupt.

In this example：
1. Turn on HSE to use it as the clock source of PLL and the PLL clock used as the main clock is set to 96 MHz.
2. LED1 turn on, LED2 turn off.
3. waits for a period of time, PA0 pin is configured to generate an interrupt on  rising edge and enters the STOP mode.
4. Wake-up the STOP mode in external interrupt by pressing a button connected to PA0 pin.
5. LED2 is toggled In EXTI0 interrupt （LED2 turn on）.
6. When enters the STOP mode,the RTC configuration is not kept,so there is need to configure the RTC.
7. LED1 toggling.


@par Directory contents 

  - PWR/PWR_STOP/PWR_STOP.uvprojx       MDK5 project file
  - PWR/PWR_STOP/PWR_STOP.uvoptx        MDK5 project options file
  - PWR/PWR_STOP/JLinkSettings.ini      Jlink settings file
  - PWR/PWR_STOP/main.c                 Main program


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz
    - The chip pins are connected as follows：
      - PB14 pin is connected to LED1
      - PB13 pin is connected to LED2
      - PA0 pin is connected to KEY1


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open PWR_STOP.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

/**
  @page IWDG_Reset IWDG_Reset example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    IWDG/IWDG_Reset/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the IWDG IWDG_Reset example.
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

This example describes how to use the IWDG.

In this example, Main clock is from PLL clock and it is configured at 96MHz. The PLL clock is based on 8MHz HSE. APB1 and APB2 clock is from Main clock.

Note that there is any problem with IWDG:
1, Once IWDG is enabled, it cannot be disabled even if a reset occurs. \n
Solution: Configure the IWDG timeout setting to maximum before the program runs, and continuously reload the IWDG counter after that.

2, If IWDG is enabled, Once a reset occurs, the IWDG domain needs three LSI clock cycles to be ready. \n
Solution: After the LSI is ready, wait about 1ms before configuring the IWDG.

3, Continuously performing IWDG reload counter operations may not reload IWDG counter \n
Solution: It is best to reload IWDG counter when the RVU bit is 0. (RVU bit indicate the reload counter operation is finish)


@par Directory contents 

  - IWDG/IWDG_Reset/IWDG_Reset.uvprojx     MDK5 project file
  - IWDG/IWDG_Reset/IWDG_Reset.uvoptx      MDK5 project options file
  - IWDG/IWDG_Reset/JLinkSettings.ini      Jlink settings file
  - IWDG/IWDG_Reset/main.c                 Main program


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open IWDG_Reset.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

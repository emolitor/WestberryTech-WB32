/**
  @page RNG_Example RNG_Example example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    RNG/RNG_Example/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the RNG RNG_Example example.
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

This example describes how to use the RNG peripheral to generate random numbers.

In this example, Main clock is from PLL clock and it is configured at 96MHz. The PLL clock is based on 8MHz HSE. APB1 and APB2 clock is from Main clock.


@par Directory contents 

  - RNG/RNG_Example/RNG_Example.uvprojx   MDK5 project file
  - RNG/RNG_Example/RNG_Example.uvoptx    MDK5 project options file
  - RNG/RNG_Example/JLinkSettings.ini     Jlink settings file
  - RNG/RNG_Example/main.c                Main program


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open RNG_Example.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

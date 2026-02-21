/**
  @page GPIO_BitBand GPIO_BitBand example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    GPIO/GPIO_BitBand/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the GPIO GPIO_BitBand example.
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

This example describes how to use the bit-banding feature to read GPIO input and control GPIO output.

In this example, Main clock is from PLL clock and it is configured at 96MHz. The PLL clock is based on 8MHz HSE. APB1 and APB2 clock is from Main clock.


@par Directory contents 

  - GPIO/GPIO_BitBand/GPIO_BitBand.uvprojx    MDK5 project file
  - GPIO/GPIO_BitBand/GPIO_BitBand.uvoptx     MDK5 project options file
  - GPIO/GPIO_BitBand/JLinkSettings.ini       Jlink settings file
  - GPIO/GPIO_BitBand/main.c                  Main program


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open GPIO_BitBand.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

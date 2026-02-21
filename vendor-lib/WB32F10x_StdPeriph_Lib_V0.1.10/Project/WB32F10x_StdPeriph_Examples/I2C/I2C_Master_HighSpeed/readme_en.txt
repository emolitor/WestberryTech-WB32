/**
  @page I2C_Master_HighSpeed I2C_Master_HighSpeed example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    I2C/I2C_Master_HighSpeed/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the I2C I2C_Master_HighSpeed example.
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

This example describes I2C2's high-speed mode.

In this example, Main clock is from PLL clock and it is configured at 96MHz. The PLL clock is based on 8MHz HSE. APB1 and APB2 clock is from Main clock.

This example describes use DAC5571 as a Slave device.The I2C high-speed mode needs to be switched from the fast-mode to the high-speed mode, so in this example, the timing of the fast mode needs to be configured at the same time.


@par Directory contents 

  - I2C/I2C_Master_HighSpeed/I2C_Master_HighSpeed.uvprojx   MDK5 project file
  - I2C/I2C_Master_HighSpeed/I2C_Master_HighSpeed.uvoptx    MDK5 project options file
  - I2C/I2C_Master_HighSpeed/JLinkSettings.ini              Jlink settings file
  - I2C/I2C_Master_HighSpeed/main.c                         Main program


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz
    - UART1 (PA9, PA10) is connected to PC serial port

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open I2C_Master_HighSpeed.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

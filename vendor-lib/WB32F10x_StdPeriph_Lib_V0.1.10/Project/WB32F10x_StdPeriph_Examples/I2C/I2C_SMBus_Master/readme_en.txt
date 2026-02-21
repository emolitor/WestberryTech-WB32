/**
  @page I2C_SMBus_Master I2C_SMBus_Master example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    I2C/I2C_SMBus_Master/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the I2C I2C_SMBus_Master example.
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

This example describes how to use the I2C SMBus mode.

In this example, Main clock is from PLL clock and it is configured at 96MHz. The PLL clock is based on 8MHz HSE. APB1 and APB2 clock is from Main clock.

This example takes driving MAX1617A as an example.First read the ID of MAX1617A by the SMBus bus,
Then set the alarm temperature to 30℃ and read the current temperature.
When MAX1617A detects that the current temperature reaches the alarm temperature and generates an ALERT signal, 
it will clear the ALERT signal through the SMBus Alert Response address protocol.

@par Directory contents 

  - I2C/I2C_SMBus_Master/I2C_SMBus_Master.uvprojx   MDK5 project file
  - I2C/I2C_SMBus_Master/I2C_SMBus_Master.uvoptx    MDK5 project options file
  - I2C/I2C_SMBus_Master/JLinkSettings.ini          Jlink settings file
  - I2C/I2C_SMBus_Master/main.c                     Main program


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz
    - UART1 (PA9, PA10) is connected to PC serial port
    - I2C1 pin and MAX1617A are connected as follows:
      - PB6 (I2C1_SCL) pin is connected to MAX1617A SMBCLK (pin14)
      - PB7 (I2C1_SDA) pin is connected to MAX1617A SMBDATA (pin12)
      - PB5 (I2C1_SMBAL) pin is connected to MAX1617A /ALERT (pin11)


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open I2C_SMBus_Master.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

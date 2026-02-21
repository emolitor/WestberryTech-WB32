/**
  @page I2C_MasterDMARx_SlaveDMATx I2C_MasterDMARx_SlaveDMATx example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    I2C/I2C_MasterDMARx_SlaveDMATx/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the I2C I2C_MasterDMARx_SlaveDMATx example.
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

This example describes The I2C Master receives data using DMA and The I2C slave transmits data using DMA.

In this example, Main clock is from PLL clock and it is configured at 96MHz. The PLL clock is based on 8MHz HSE. APB1 and APB2 clock is from Main clock.

In this example, I2C1 as slave and I2C2 as Master,I2C1 and I2C2 need to be connected; \n
DMAC2 Channel 0 is used as I2C1 slave to transmits data; \n
DMAC2 Channel 1 is used as I2C2 Master to transmits data; \n
DMAC2 Channel 2 is used as I2C2 Master to receives data;\n
At the end of the data transfer, a data consistency check will be performed in master and slave sides,
and print out the check result through UART1.


@par Directory contents 

  - I2C/I2C_MasterDMARx_SlaveDMATx/I2C_MasterDMARx_SlaveDMATx.uvprojx   MDK5 project file
  - I2C/I2C_MasterDMARx_SlaveDMATx/I2C_MasterDMARx_SlaveDMATx.uvoptx    MDK5 project options file
  - I2C/I2C_MasterDMARx_SlaveDMATx/JLinkSettings.ini                    Jlink settings file
  - I2C/I2C_MasterDMARx_SlaveDMATx/main.c                               Main program


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz
    - UART1 (PA9, PA10) is connected to PC serial port
    - PB6 (I2C1_SCL) is connected to PB10 (I2C2_SCL)
    - PB6 (I2C1_SCL) is connected to PB10 (I2C2_SCL)


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open I2C_MasterDMARx_SlaveDMATx.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

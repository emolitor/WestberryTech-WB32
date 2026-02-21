/**
  @page QSPI_SPI_FLASH QSPI_SPI_FLASH example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    SPI/QSPI_SPI_FLASH/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the SPI QSPI_SPI_FLASH example.
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

This example describes how to use the SPI firmware library communicate with a SPI FLASH.

In this example, Main clock is from PLL clock and it is configured at 96MHz. The PLL clock is based on 8MHz HSE. APB1 and APB2 clock is from Main clock.

This example performs the following operations:
1. Initialize and configure the QSPI bit rate to 12 Mbit/s.
2. Read the SPI Flash ID and print via UART.
3. Using SPI FLASH driver the program performs an erase of the sector to be accessed,a write of a WriteData, defined in the main.c file, to the memory followed by a read.
4. Data read from the memory stored in the ReadBuffer are compared with the expected values of the WriteData and print the result of this comparison.


@par Directory contents 

  - SPI/QSPI_SPI_FLASH/QSPI_SPI_FLASH.uvprojx   MDK5 project file
  - SPI/QSPI_SPI_FLASH/QSPI_SPI_FLASH.uvoptx    MDK5 project options file
  - SPI/QSPI_SPI_FLASH/JLinkSettings.ini        Jlink settings file
  - SPI/QSPI_SPI_FLASH/main.c                   Main program
  - SPI/QSPI_SPI_FLASH/drv_spi_flash.c          SPI Flash driver
  - SPI/QSPI_SPI_FLASH/drv_spi_flash.h          SPI Flash driver header file


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz
    - The chip pins and the QSPI FLASH are connected as follows:
      - QSPI_NSS0 (PA4) pin is connected to SPI Flash CS (pin1)
      - QSPI_SCK (PA5) pin is connected to SPI Flash CLK (pin6)
      - QSPI_MI_IO1 (PA6) pin is connected to SPI Flash DO (pin2)
      - QSPI_MO_IO0 (PA7) pin is connected to SPI Flash DI (pin5)
      - SPI Flash WP (pin3) is connected to Vdd
      - SPI Flash HOLD (pin7) is connected to Vdd


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open QSPI_SPI_FLASH.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

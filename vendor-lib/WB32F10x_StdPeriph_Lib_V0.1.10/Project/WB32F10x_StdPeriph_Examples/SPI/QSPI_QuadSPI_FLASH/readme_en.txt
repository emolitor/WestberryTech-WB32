/**
  @page QSPI_QuadSPI_FLASH QSPI_QuadSPI_FLASH example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    SPI/QSPI_QuadSPI_FLASH/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the SPI QSPI_QuadSPI_FLASH example.
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

The first step consist in reading the SPI Flash ID.Then read the ID via Quad SPI and print via UART.

Using SPI FLASH driver the program performs an erase of the sector to be accessed,a write of a WriteData, defined in the main.c file, to the memory followed by a read.
Then data read from the memory stored in the ReadBuffer are compared with the expected values of the WriteData and print the result of this comparison.

In order to avoid TX FIFO underflow or RX FIFO overflow, the transmission rate of QSPI should not be too fast,
so in this example, the QSPI bit rate is configured to 2.4 Mbit/s.


@par Directory contents 

  - SPI/QSPI_QuadSPI_FLASH/QSPI_QuadSPI_FLASH.uvprojx   MDK5 project file
  - SPI/QSPI_QuadSPI_FLASH/QSPI_QuadSPI_FLASH.uvoptx    MDK5 project options file
  - SPI/QSPI_QuadSPI_FLASH/JLinkSettings.ini            Jlink settings file
  - SPI/QSPI_QuadSPI_FLASH/main.c                       Main program
  - SPI/QSPI_QuadSPI_FLASH/drv_spi_flash_quad.c         QSPI Flash driver 
  - SPI/QSPI_QuadSPI_FLASH/drv_spi_flash_quad.h         QSPI Flash driver header file


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz
    -The QSPI pins and the QSPI FLASH are connected as follows:
      - QSPI_NSS0 (PA4) pin is connected to QSPI Flash CS (pin1)
      - QSPI_SCK (PA5) pin is connected to QSPI Flash CLK (pin6)
      - QSPI_MI_IO1 (PA6) pin is connected to QSPI Flash DO(IO1) (pin2)
      - QSPI_MO_IO0 (PA7) pin is connected to QSPI Flash DI(IO0) (pin5)
      - QSPI_IO2 (PB0) pin is connected to QSPI Flash /WP(IO2) (pin3)
      - QSPI_IO3 (PB1) pin is connected to QSPI Flash /HOLD(IO3) (pin7)


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open QSPI_QuadSPI_FLASH.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

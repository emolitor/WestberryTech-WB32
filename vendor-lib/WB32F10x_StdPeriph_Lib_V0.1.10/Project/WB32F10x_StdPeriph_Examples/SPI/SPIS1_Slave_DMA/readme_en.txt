/**
  @page SPIS1_Slave_DMA SPIS1_Slave_DMA example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    SPI/SPIS1_Slave_DMA/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the SPI SPIS1_Slave_DMA example.
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

This example and QSPI_Master_Interrupt example describe how to realize the communication between SPI master and SPI slave using interrupt.

In this example, Main clock is from PLL clock and it is configured at 96MHz. The PLL clock is based on 8MHz HSE. APB1 and APB2 clock is from Main clock.

- Hardware Description

To use this example, you need to prepare two WB32 boards (let's call them BoardA and BoardB) then connect these two boards through SPI lines and GND. \n
In addition, connect the UART1 of the two boards to the PC to realize the communication with the board. \n
The BoardA needs to download the QSPI_Master_Interrupt program, and the BoardB needs to download the SPIS1_Slave_Interrupt program.

@verbatim
*------------------------------------------------------------------------------*
|                BoardA                                 BoardB                 |
|         ____________________                   ____________________          |
|        |                    |                 |                    |         |
|        |     __________     |                 |     __________     |         |
|        |    |          |____|_______NSS_______|____|          |    |         |
|        |    |SPI Master|____|_______SCK_______|____|SPI Slave |    |         |
|        |    |  Device  |____|_______MOSI______|____|  Device  |    |         |
|        |    |__________|____|_______MISO______|____|__________|    |         |
|        |                    |                 |                    |         |
|        |                    |                 |                    |         |
|        |             GND O--|-----------------|--O GND             |         |
|        |                    |                 |                    |         |
|        |                    |                 |                    |         |
|        | UART1              |                 |              UART1 |         |
|        |____________________|                 |____________________|         |
|                                                                              |
*------------------------------------------------------------------------------*
@endverbatim

- Software Description

In QSPI_Master_Interrupt program, QSPI peripheral is configured as Master full duplex with interrupt,and the bit rate is configured as 1 Mbit/s. \n
In SPIS1_Slave_Interrupt program, SPIS1 peripheral is configured as Slave Full duplex with interrupt.

When the two boards are powered, they will print relevant information through the UART1 and wait for input signal from the UART1 before continuing to run. \n
Frist,Input any character into the UART1 of BoardB (SPI slave) to make BoardB continue to run. \n
Then,Then input any character into the UART1 of BoardA (SPI master) to make BoardA start to initiate transmission. \n
When the transmission is complete, both BoardA and BoardB will print out the data they received.


@par Directory contents 

  - SPI/SPIS1_Slave_DMA/SPIS1_Slave_DMA.uvprojx   MDK5 project file
  - SPI/SPIS1_Slave_DMA/SPIS1_Slave_DMA.uvoptx    MDK5 project options file
  - SPI/SPIS1_Slave_DMA/JLinkSettings.ini         Jlink settings file
  - SPI/SPIS1_Slave_DMA/main.c                    Main program


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz
    - Need two WB32 boards
    - connect the UART1 of the two boards to the PC
    - The two boards are connected as follows:
      - Connect BoardA PA4 (QSPI_NSS0) to BoardB PA4 (SPIS1_NSS)
      - Connect BoardA PA5 (QSPI_SCK) to BoardB PA5 (SPIS1_SCK)
      - Connect BoardA PA6 (QSPI_MI_IO1) to BoardB PA6 (SPIS1_SO)
      - Connect BoardA PA7 (QSPI_MO_IO0) to BoardB PA7 (SPIS1_SI)


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open SPIS1_Slave_DMA.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

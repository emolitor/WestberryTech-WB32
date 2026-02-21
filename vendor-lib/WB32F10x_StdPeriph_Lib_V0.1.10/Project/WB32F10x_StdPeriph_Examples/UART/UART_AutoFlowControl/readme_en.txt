/**
  @page UART_AutoFlowControl UART_AutoFlowControl example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    UART/UART_AutoFlowControl/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the UART UART_AutoFlowControl example.
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

This example shows how to use UART auto flow control function (Auto RTS and Auto CTS).

In this example, Main clock is from PLL clock and it is configured at 96MHz. The PLL clock is based on 8MHz HSE. APB1 and APB2 clock is from Main clock.

RTS: Output Signal, Modem Control Request To Send. If the output is low, it indicates that the UART is ready to receive data.
CTS: Input Signal, Clear To Send Modem Status. If the input level is high, the next data transmission is blocked at the end of the current data transmission.

The UART1 is configured as follow:
  - BaudRate: 9600
  - Word Length: 8
  - Stop Bit: 1
  - No parity
  - Auto flow control enabled
  - FIFO is enabled


@par Directory contents 

  - UART/UART_AutoFlowControl/UART_AutoFlowControl.uvprojx  MDK5 project file
  - UART/UART_AutoFlowControl/UART_AutoFlowControl.uvoptx   MDK5 project options file
  - UART/UART_AutoFlowControl/JLinkSettings.ini             Jlink settings file
  - UART/UART_AutoFlowControl/main.c                        Main program


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz
    - UART1 (PA9, PA10) is connected to PC serial port


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open UART_AutoFlowControl.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

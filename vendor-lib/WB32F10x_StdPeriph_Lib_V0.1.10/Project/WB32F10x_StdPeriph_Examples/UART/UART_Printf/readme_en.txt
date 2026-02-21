/**
  @page UART_Printf UART_Printf example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    UART/UART_Printf/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the UART UART_Printf example.
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

This example shows how to retarget the standard input and output function to the UART1.

In this example, Main clock is from PLL clock and it is configured at 96MHz. The PLL clock is based on 8MHz HSE. APB1 and APB2 clock is from Main clock.

The code to implement the standard input and output retarget function is in \Utilities\Common\Retarget_uart1.c.

The UART1 is configured as follow:
  - BaudRate: 115200
  - Word Length: 8
  - Stop Bit: 1
  - No parity
  - Auto flow control disabled
  - FIFO is enabled


@par Directory contents 

  - UART/UART_Printf/UART_Printf.uvprojx   MDK5 project file
  - UART/UART_Printf/UART_Printf.uvoptx    MDK5 project options file
  - UART/UART_Printf/JLinkSettings.ini     Jlink settings file
  - UART/UART_Printf/main.c                Main program


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz
    - UART1 (PA9, PA10) is connected to PC serial port


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open UART_Printf.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

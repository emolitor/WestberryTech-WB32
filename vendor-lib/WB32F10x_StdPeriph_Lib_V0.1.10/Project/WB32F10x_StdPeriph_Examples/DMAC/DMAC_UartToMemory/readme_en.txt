/**
  @page DMAC_UartToMemory DMAC_UartToMemory example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    DMAC/DMAC_UartToMemory/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the DMAC DMAC_UartToMemory example.
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

This example describes how to use DMAC to transfer data received by UART to Memory.

In this example, Main clock is from PLL clock and it is configured at 96MHz. The PLL clock is based on 8MHz HSE. APB1 and APB2 clock is from Main clock.

In this example, UART2->RBR is used as the source of DMA transfer, and memDst is used as the target of DMA transfer.
When this example runs, it will print out relevant information through UART1, and enter any character to start DMA transmission.
Then send 40 characters to UART2 through PC, DMA transfer will end,
Then it will print out the string received from UART2 through UART1.


@par Directory contents 

  - DMAC/DMAC_UartToMemory/DMAC_UartToMemory.uvprojx    MDK5 project file
  - DMAC/DMAC_UartToMemory/DMAC_UartToMemory.uvoptx     MDK5 project options file
  - DMAC/DMAC_UartToMemory/JLinkSettings.ini            Jlink settings file
  - DMAC/DMAC_UartToMemory/main.c                       Main program


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz
    - UART1 (PA9, PA10) is connected to PC serial port
    - UART2 RX (PA3) is connected to PC serial port

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open DMAC_UartToMemory.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

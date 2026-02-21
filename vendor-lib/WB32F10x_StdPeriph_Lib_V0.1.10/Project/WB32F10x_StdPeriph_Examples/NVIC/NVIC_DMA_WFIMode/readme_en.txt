/**
  @page NVIC_DMA_WFIMode NVIC_DMA_WFIMode example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    NVIC/NVIC_DMA_WFIMode/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the NVIC NVIC_DMA_WFIMode example.
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

This example describes how to wake up from this mode through the DMA transfer end interrupt,when DMA transfer is enabled and the system enters WFI mode.

In this example, Main clock is from PLL clock and it is configured at 96MHz. The PLL clock is based on 8MHz HSE. APB1 and APB2 clock is from Main clock.

Press the button (PB6) to make the core enter the WFI mode, and LED1 stops switching. To wake up from WFI mode,
You must send the ASCII sequence (0, 1, 2, 3, 4, 5, 6, 7, 8, 9) to UART1.
DMAC transfers these bytes from the UART1 receiving data register to a predefined buffer, and then generates an interrupt,
The system exits the WFI mode, and LED1 restarts to switch. If the correct sequence is received, LED2 is on, otherwise LED2 is off.


@par Directory contents 

  - NVIC/NVIC_DMA_WFIMode/NVIC_DMA_WFIMode.uvprojx    MDK5 project file
  - NVIC/NVIC_DMA_WFIMode/NVIC_DMA_WFIMode.uvoptx     MDK5 project options file
  - NVIC/NVIC_DMA_WFIMode/JLinkSettings.ini           Jlink settings file
  - NVIC/NVIC_DMA_WFIMode/main.c                      Main program


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz
    - The chip pins are connected as follows：
      - PB14 pin is connected to LED1
      - PB13 pin is connected to LED2
      - PB6 pin is connected to KEY


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open NVIC_DMA_WFIMode.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

/**
  @page BKP_Tamper BKP Tamper example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    BKP/BKP_Tamper/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the BKP_Tamper example.
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

This example shows how to write/read data to/from Backup data registers and demonstrates the Tamper detection feature.

In this example, Main clock is from PLL clock and it is configured at 96MHz. The PLL clock is based on 8MHz HSE. APB1 and APB2 clock is from Main clock.

The program behaves as follows:
1. Configures the Tamper pin to be active low, and enables the Tamper interrupt.
2. Writes the data to all Backup data registers, then check whether the data were correctly written. If no, LED1 blink.
3. On applying a low level on the Tamper pin (PC13), the Backup data registers are reset and the Tamper interrupt is generated.
The corresponding ISR then checks whether the Backup data registers are cleared. If yes, LED2 on, otherwise LED1 turns on.


@par Directory contents 

  - BKP/BKP_Tamper/BKP_Tamper.uvprojx    MDK5 project file
  - BKP/BKP_Tamper/BKP_Tamper.uvoptx     MDK5 project options file
  - BKP/BKP_Tamper/JLinkSettings.ini     Jlink settings file
  - BKP/BKP_Tamper/main.c                Main program


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz
    - The chip pins are connected as follows：
      - PB14 pin is connected to LED1
      - PB13 pin is connected to LED2


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open BKP_Tamper.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */
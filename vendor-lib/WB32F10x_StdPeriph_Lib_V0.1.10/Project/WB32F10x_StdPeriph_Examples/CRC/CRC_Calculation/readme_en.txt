/**
  @page CRC_Calculation CRC_Calculation example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    CRC/CRC_Calculation/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the CRC CRC_Calculation example.
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

This example describes how to use the CRC module to calculate the CRC checksum of byte arrays, half-word (16-bit) arrays and word (32-bit) arrays based on different polynomials and different calculation configurations.

In this example, Main clock is from PLL clock and it is configured at 96MHz. The PLL clock is based on 8MHz HSE. APB1 and APB2 clock is from Main clock.


@par Directory contents 

  - CRC/CRC_Calculation/CRC_Calculation.uvprojx   MDK5 project file
  - CRC/CRC_Calculation/CRC_Calculation.uvoptx    MDK5 project options file
  - CRC/CRC_Calculation/JLinkSettings.ini         Jlink settings file
  - CRC/CRC_Calculation/main.c                    Main program
  - CRC/CRC_Calculation/computeBytes.c            Compute the CRC checksum of byte arrays
  - CRC/CRC_Calculation/computeHalfWords.c        Compute the CRC checksum of half-word arrays
  - CRC/CRC_Calculation/computeWords.c            Compute the CRC checksum of word arrays
  - CRC/CRC_Calculation/compute.h                 compute*.c header file


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open CRC_Calculation.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

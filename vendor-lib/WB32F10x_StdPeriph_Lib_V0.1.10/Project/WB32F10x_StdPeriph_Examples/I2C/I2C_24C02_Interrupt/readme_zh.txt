/**
  @page I2C_24C02_Interrupt I2C_24C02_Interrupt example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    I2C/I2C_24C02_Interrupt/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the I2C I2C_24C02_Interrupt example.
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, WESTBERRY SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
  * INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM
  * THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
   @endverbatim

@par 例程描述 

本例程提供了如何使用I2C固件库的基本示例以及与EEPROM 24C02通信的驱动程序（以中断方式实现）。

本例程系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。

在本例程中演示了24C02的 Byte Write, Page Write, Acknowledge Polling, Random Read 和 Sequential Read 等操作。


@par 目录内容 

  - I2C/I2C_24C02_Interrupt/I2C_24C02_Interrupt.uvprojx   MDK5工程文件
  - I2C/I2C_24C02_Interrupt/I2C_24C02_Interrupt.uvoptx    MDK5工程配置文件
  - I2C/I2C_24C02_Interrupt/JLinkSettings.ini             Jlink配置文件
  - I2C/I2C_24C02_Interrupt/main.c                        主程序


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振
    - UART1 (PA9, PA10) 连接到PC


@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 I2C_24C02_Interrupt.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

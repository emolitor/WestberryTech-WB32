/**
  @page UART_CharacterTimeout UART_CharacterTimeout example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    UART/UART_CharacterTimeout/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the UART UART_CharacterTimeout example.
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

本例程演示UART字符超时功能。

本例程系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。

UART模块支持字符超时功能。 \n
当RX FIFO中至少有一个字符，并且在一段时间内没有字符输入或读出时， \n
会触发字符超时中断。要使用该功能，需要启用FIFO，启用UART RDA中断。

UART1 配置如下：
  - 波特率： 115200
  - 数据位： 8
  - 停止位： 1
  - 无奇偶校验
  - 禁用自动流控
  - 启用 FIFO


@par 目录内容 

  - UART/UART_CharacterTimeout/UART_CharacterTimeout.uvprojx  MDK5工程文件
  - UART/UART_CharacterTimeout/UART_CharacterTimeout.uvoptx   MDK5工程配置文件
  - UART/UART_CharacterTimeout/JLinkSettings.ini              Jlink配置文件
  - UART/UART_CharacterTimeout/main.c                         主程序


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振
    - UART1 (PA9, PA10) 连接到PC


@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 UART_CharacterTimeout.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

/**
  @page UART_Interrupt UART_Interrupt example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    UART/UART_Interrupt/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the UART UART_Interrupt example.
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

本例程演示如何使用UART的接收和发送中断。

本例程系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。

本例程开始运行时会使用UART发送中断发出一段信息，之后禁用发送中断； \n
同时，本例程还启用了UART接收中断，在接收中断中接收上位机发送的数据， \n
每当接收到100个数据后就会将该数据发送回上位机。

UART1 配置如下：
  - 波特率： 115200
  - 数据位： 8
  - 停止位： 1
  - 无奇偶校验
  - 禁用自动流控


@par 目录内容 

  - UART/UART_Interrupt/UART_Interrupt.uvprojx    MDK5工程文件
  - UART/UART_Interrupt/UART_Interrupt.uvoptx     MDK5工程配置文件
  - UART/UART_Interrupt/JLinkSettings.ini         Jlink配置文件
  - UART/UART_Interrupt/main.c                    主程序


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振
    - UART1 (PA9, PA10) 连接到PC


@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 UART_Interrupt.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

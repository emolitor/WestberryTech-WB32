/**
  @page UART_9BitDataTransfer UART_9BitDataTransfer example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    UART/UART_9BitDataTransfer/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the UART UART_9BitDataTransfer example.
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

本例程演示UART 9-bit Data Transfer功能。

本例程系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。

在本例程中，您需要将UART2的TX和RX连接在一起，作为一个回环。\n
UART2被配置为只接收发送给地址0xD1的数据，然后向0xD1地址发送数据， \n
由于UART2的TX和RX时短接在一起的，那么UART2就会接收到发出的数据。 \n

UART2 配置如下：
  - 波特率： 115200
  - 数据位： 8
  - 停止位： 1
  - 无奇偶校验
  - 禁用自动流控
  - 启用 FIFO


@par 目录内容 

  - UART/UART_9BitDataTransfer/UART_9BitDataTransfer.uvprojx  MDK5工程文件
  - UART/UART_9BitDataTransfer/UART_9BitDataTransfer.uvoptx   MDK5工程配置文件
  - UART/UART_9BitDataTransfer/JLinkSettings.ini              Jlink配置文件
  - UART/UART_9BitDataTransfer/main.c                         主程序


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振
    - UART1 (PA9, PA10) 连接到PC
    - UART2 的 TX (PA2) 和 RX (PA3) 连接在一起


@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 UART_9BitDataTransfer.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

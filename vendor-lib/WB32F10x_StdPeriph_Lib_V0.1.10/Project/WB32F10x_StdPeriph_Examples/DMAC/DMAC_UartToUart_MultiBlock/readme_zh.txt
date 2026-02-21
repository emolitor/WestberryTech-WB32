/**
  @page DMAC_UartToUart_MultiBlock DMAC_UartToUart_MultiBlock example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    DMAC/DMAC_UartToUart_MultiBlock/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the DMAC DMAC_UartToUart_MultiBlock example.
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

本例程演示如何使用DMAC将UART接收到的数据传输到UART。

本例程系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。

在本例程中，UART2->RBR作为DMA传输的源，UART2->THR作为DMA传输的目标。
DMAC2 Channel 0开启了自动源重载功能和自动目标重载功能。
当本例程运行后，会通过UART1打印出相关信息，输入任意字符启动DMA传输。
然后DMAC会将UART2接收到的数据传输到UART2，再发送出去。


@par 目录内容 

  - DMAC/DMAC_UartToUart_MultiBlock/DMAC_UartToUart_MultiBlock.uvprojx  MDK5工程文件
  - DMAC/DMAC_UartToUart_MultiBlock/DMAC_UartToUart_MultiBlock.uvoptx   MDK5工程配置文件
  - DMAC/DMAC_UartToUart_MultiBlock/JLinkSettings.ini                   Jlink配置文件
  - DMAC/DMAC_UartToUart_MultiBlock/main.c                              主程序


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振
    - UART1 (PA9, PA10) 连接到PC
    - UART2 (PA2, PA3) 连接到PC

@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 DMAC_UartToUart_MultiBlock.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

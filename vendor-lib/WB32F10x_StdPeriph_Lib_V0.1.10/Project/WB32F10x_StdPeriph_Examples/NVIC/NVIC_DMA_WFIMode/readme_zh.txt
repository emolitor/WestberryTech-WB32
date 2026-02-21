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

@par 例程描述 

本例程演示在启用DMA传输的情况下系统进入WFI模式，如何通过DMA传输结束中断从该模式唤醒。

本例程系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。

按下按键（PB6）令内核进入WFI模式，LED1停止切换。要从WFI模式唤醒，
您必须将ASCII序列（0, 1, 2, 3, 4, 5, 6, 7, 8, 9）发送到UART1。
DMAC将这些字节从UART1接收数据寄存器传输到预定义的缓冲区，然后生成一个中断，
使系统退出WFI模式，LED1重新开始切换。如果收到正确的序列，则LED2点亮，否则LED2熄灭。


@par 目录内容 

  - NVIC/NVIC_DMA_WFIMode/NVIC_DMA_WFIMode.uvprojx    MDK5工程文件
  - NVIC/NVIC_DMA_WFIMode/NVIC_DMA_WFIMode.uvoptx     MDK5工程配置文件
  - NVIC/NVIC_DMA_WFIMode/JLinkSettings.ini           Jlink配置文件
  - NVIC/NVIC_DMA_WFIMode/main.c                      主程序


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振
    - 芯片管脚连接如下：
      - PB14 管脚连接到 LED1
      - PB13 管脚连接到 LED2
      - PB6 管脚连接到 KEY


@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 NVIC_DMA_WFIMode.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

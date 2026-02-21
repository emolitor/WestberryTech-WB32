/**
  @page SPIS2_Slave_DMA SPIS2_Slave_DMA example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    SPI/SPIS2_Slave_DMA/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the SPI SPIS2_Slave_DMA example.
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

本例程与SPIM2_Master_DMA例程演示如何以DMA的方式完成SPI主机和SPI从机之间的通信。

本例程系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。

- 硬件描述

要使用此例程，您需要两块WB32开发板（让我们称之为开发板A和开发板B），然后连接两块开发板的SPI接口和GND。 \n
另外需要将两个开发板的UART1连接到PC，以实现与开发板的交互。 \n
开发板A需加载SPIM2_Master_DMA程序，开发板B需加载SPIS2_Slave_DMA程序。

@verbatim
*------------------------------------------------------------------------------*
|                BoardA                                 BoardB                 |
|         ____________________                   ____________________          |
|        |                    |                 |                    |         |
|        |     __________     |                 |     __________     |         |
|        |    |          |____|_______NSS_______|____|          |    |         |
|        |    |SPI Master|____|_______SCK_______|____|SPI Slave |    |         |
|        |    |  Device  |____|_______MOSI______|____|  Device  |    |         |
|        |    |__________|____|_______MISO______|____|__________|    |         |
|        |                    |                 |                    |         |
|        |                    |                 |                    |         |
|        |             GND O--|-----------------|--O GND             |         |
|        |                    |                 |                    |         |
|        |                    |                 |                    |         |
|        | UART1              |                 |              UART1 |         |
|        |____________________|                 |____________________|         |
|                                                                              |
*------------------------------------------------------------------------------*
@endverbatim

- 软件描述

在SPIM2_Master_DMA程序中，SPIM2外设被配置为SPI主机，使用DMA发送和接收数据，通信速率被配置为 12 Mbit/s。 \n
在SPIS2_Slave_DMA程序中，SPIS2外设被配置为SPI从机，使用DMA发送和接收数据。

当两个开发板上电运行后，都会通过串口打印相关信息，等待串口输入才会继续运行。 \n
首先，向开发板B（SPI从机）的串口输入任意字符，令开发板B继续运行。 \n
然后，再向开发板A（SPI主机）的串口输入任意字符，令开发板A开始发起传输。 \n
当传输完成后，开发板A和开发板B都会打印出它们各自接收到的数据。


@par 目录内容 

  - SPI/SPIS2_Slave_DMA/SPIS2_Slave_DMA.uvprojx   MDK5工程文件
  - SPI/SPIS2_Slave_DMA/SPIS2_Slave_DMA.uvoptx    MDK5工程配置文件
  - SPI/SPIS2_Slave_DMA/JLinkSettings.ini         Jlink配置文件
  - SPI/SPIS2_Slave_DMA/main.c                    主程序


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振
    - 需要两块开发板
    - 两块开发板都需要将其各自的UART1连接到PC
    - 两块开发板之间的连接如下：
      - 开发板A的 PC0 (SPIM2_NSS0) 管脚连接到 开发板B的 PC0 (SPIS2_NSS)
      - 开发板A的 PC1 (SPIM2_SCK) 管脚连接到 开发板B的 PC1 (SPIS2_SCK)
      - 开发板A的 PC2 (SPIM2_MI) 管脚连接到 开发板B的 PC2 (SPIS2_SO)
      - 开发板A的 PC3 (SPIM2_MO) 管脚连接到 开发板B的 PC3 (SPIS2_SI)


@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 SPIS2_Slave_DMA.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

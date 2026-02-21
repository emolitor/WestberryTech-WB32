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

@par 例程描述 

本例程演示如何向备份数据寄存器写入/读取数据，以及侵入检测功能。

本例程系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。

本例程执行以下操作：
1. 将Tamper管脚配置为低电平清除所有备份数据寄存器，并启用侵入检测中断。
2. 将数据写入所有备份数据寄存器，然后检查数据是否正确写入。如果没有正确写入，LED1闪烁。
3. 当Tamper管脚(PC13)接低电平时，备份数据寄存器会复位并且产生侵入中断。
在相应的中断处理程序中检查备份数据寄存器是否清除；如果清除了，则LED2点亮，否则LED1点亮。


@par 目录内容 

  - BKP/BKP_Tamper/BKP_Tamper.uvprojx    MDK5工程文件
  - BKP/BKP_Tamper/BKP_Tamper.uvoptx     MDK5工程配置文件
  - BKP/BKP_Tamper/JLinkSettings.ini     Jlink配置文件
  - BKP/BKP_Tamper/main.c                主程序


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振
    - 芯片管脚连接如下：
      - PB14 管脚连接到 LED1
      - PB13 管脚连接到 LED2


@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 BKP_Tamper.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */
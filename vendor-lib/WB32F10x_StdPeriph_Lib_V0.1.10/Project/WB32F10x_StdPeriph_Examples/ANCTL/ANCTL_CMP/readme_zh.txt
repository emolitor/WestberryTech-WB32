/**
  @page ANCTL_CMP ANCTL CMP example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    ANCTL/ANCTL_CMP/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the ANCTL_CMP example.
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

本例程演示如何使用比较器功能。

本例程系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。

本例程执行以下操作：
1. 将PB4，PB6配置为模拟功能。
2. 将CMPA的正端输入配置为PB4，负端输入配置为PB6。
3. 当CMPA的正端输入电压大于负端输入电压时，点亮LED1，熄灭LED2。否则，熄灭LED1，点亮LED2。


@par 目录内容 

  - ANCTL/ANCTL_CMP/ANCTL_CMP.uvprojx    MDK5工程文件
  - ANCTL/ANCTL_CMP/ANCTL_CMP.uvoptx     MDK5工程配置文件
  - ANCTL/ANCTL_CMP/JLinkSettings.ini    Jlink配置文件
  - ANCTL/ANCTL_CMP/main.c               主程序


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振
    - 芯片管脚连接如下：
      - PB14 管脚连接到 LED1
      - PB13 管脚连接到 LED2
      - PB4 管脚接一模拟信号
      - PB6 管脚接另一模拟信号


@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 ANCTL_CMP.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */
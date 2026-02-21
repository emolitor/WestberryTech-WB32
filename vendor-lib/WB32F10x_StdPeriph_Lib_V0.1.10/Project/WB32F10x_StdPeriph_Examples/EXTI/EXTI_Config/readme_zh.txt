/**
  @page EXTI_Config EXTI_Config example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    EXTI/EXTI_Config/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the EXTI EXTI_Config example.
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

本例程演示如何配置外部中断线。

本例程系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。

本例程中EXTI0被配置为上升沿产生中断，EXTI6被配置为下降沿产生中断。

在本例程中：
 - EXTI0 映射到 PA0
 - EXTI6 映射到 PB6

配置EXTI后，软件触发EXTI0产生中断以改变LED1的状态。
在这之后，
  每当EXTI0检测到上升沿时，LED1翻转其状态
  每当EXTI6检查到下降沿时，LED2翻转其状态


@par 目录内容 

  - EXTI/EXTI_Config/EXTI_Config.uvprojx    MDK5工程文件
  - EXTI/EXTI_Config/EXTI_Config.uvoptx     MDK5工程配置文件
  - EXTI/EXTI_Config/JLinkSettings.ini      Jlink配置文件
  - EXTI/EXTI_Config/main.c                 主程序


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振
    - 芯片管脚连接如下：
      - PB14 管脚连接到 LED1
      - PB13 管脚连接到 LED2
      - PA0 管脚连接到 KEY1
      - PB6 管脚连接到 KEY2


@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 EXTI_Config.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

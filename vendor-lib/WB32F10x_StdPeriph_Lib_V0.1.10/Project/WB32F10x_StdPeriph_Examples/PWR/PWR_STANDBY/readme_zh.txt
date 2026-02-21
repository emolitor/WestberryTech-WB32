/**
  @page PWR_STANDBY PWR_STANDBY example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    PWR/PWR_STANDBY/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the PWR PWR_STANDBY example.
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

本例程演示如何进入和退出低功耗STANDBY模式。

本例程系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。

本例程初始点亮LED1，等待一段时间后启用WKUP管脚并进入低功耗STANDBY模式。
进入STANDBY模式后，管脚状态丢失，LED1熄灭。
然后可以通过按下与WKUP管脚连接的按键或按下复位按键以退出STANDBY模式。
退出STANDBY模式程序重头运行，重复上述过程。


@par 目录内容 

  - PWR/PWR_STANDBY/PWR_STANDBY.uvprojx     MDK5工程文件
  - PWR/PWR_STANDBY/PWR_STANDBY.uvoptx      MDK5工程配置文件
  - PWR/PWR_STANDBY/JLinkSettings.ini       Jlink配置文件
  - PWR/PWR_STANDBY/main.c                  主程序


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振
    - 芯片管脚连接如下：
      - PB14 管脚连接到 LED1
      - PA0 管脚连接到按键


@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 PWR_STANDBY.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

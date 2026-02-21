/**
  @page PWR_STOP PWR_STOP example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    PWR/PWR_STOP/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the PWR PWR_STOP example.
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

本例程演示如何进入低功耗STOP模式和通过外部中断退出低功耗STOP模式。

本例程执行以下操作：
1. 打开HSE，HSE作为PLL的时钟源，PLL倍频到96MHz，PLL时钟作为主时钟。
2. 点亮LED1，熄灭LED2。
3. 等待一段时间后，配置PA0上升沿中断并进入低功耗STOP模式。
4. 此时处于低功耗STOP模式。按下与PA0连接的按键产生EXTI0中断以唤醒芯片。
5. 在EXTI0中断中，切换LED2的状态（LED2点亮）。
6. 由于在进入低功耗STOP模式时关闭了一些时钟配置，所以退出低功耗STOP模式后重新配置相关时钟。
7. LED1闪烁。


@par 目录内容 

  - PWR/PWR_STOP/PWR_STOP.uvprojx       MDK5工程文件
  - PWR/PWR_STOP/PWR_STOP.uvoptx        MDK5工程配置文件
  - PWR/PWR_STOP/JLinkSettings.ini      Jlink配置文件
  - PWR/PWR_STOP/main.c                 主程序


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振
    - 芯片管脚连接如下：
      - PB14 管脚连接到 LED1
      - PB13 管脚连接到 LED2
      - PA0 管脚连接到按键


@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 PWR_STOP.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

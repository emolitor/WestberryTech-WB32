/**
  @page TIM_ComplementarySignals TIM ComplementarySignals example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    TIM/TIM_ComplementarySignals/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the TIM_ComplementarySignals example.
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

这个例子演示了如何配置TIM1外围设备来生成三个互补的TIM1信号，插入一个定义的死区时间值，使用中断功能和锁定所需参数。\n
系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。\n
TIM1CLK固定在系统时钟上，TIM1预定分频0，因此使用的TIM1计数器时钟是SystemCoreClock。\n

目标是在1.46 KHz下生成PWM信号：\n
  -时间间隔 =（SystemCoreClock / 65536）\n

三个占空比的计算如下：\n
Channel1占空比设置为50%，因此Channel1N设置为50%。\n
Channel2占空比设置为50%，因此Channel2N设置为50%。\n
Channel3占空比设置为50%，因此Channel3N设置为50%。\n

在不同的互补信号之间插入一个等于 11 / SystemCoreClock 的死区时间，并选择锁级别1。Break极性设为高。\n

@par 目录内容 

  - TIM/TIM_ComplementarySignals/TIM_ComplementarySignals.uvprojx    MDK5工程文件
  - TIM/TIM_ComplementarySignals/TIM_ComplementarySignals.uvoptx     MDK5工程配置文件
  - TIM/TIM_ComplementarySignals/JLinkSettings.ini                   Jlink配置文件
  - TIM/TIM_ComplementarySignals/main.c                              主程序


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振
    - 将TIM1引脚连接到示波器上，以监测不同的波形:
      - TIM1_CH1  pin (PA.08)
      - TIM1_CH1N pin (PB.13)  
      - TIM1_CH2  pin (PA.09)  
      - TIM1_CH2N pin (PB.15)
      - TIM1_CH3  pin (PA.10)  
      - TIM1_CH3N pin (PB.14)    
    - 连接 TIM1 break pin TIM1_BKIN pin (PA.06) to the VCC 来生成Break。
  

@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 TIM_ComplementarySignals.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

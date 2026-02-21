/**
  @page TIM_7PWM_Output TIM 7PWM Output example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    TIM/TIM_7PWM_Output/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the TIM_7PWM_Output example.
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

本例说明如何配置TIM1外围设备以生成7个PWM信号。4种不同的工作循环（50%、37.5%、25%和12.5%）。\n
系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。\n
TIM1CLK = 系统核心时钟，预分频器 = 0，TIM1计数器时钟 = 系统核心时钟\n
目标是在1.46KHz下产生7个PWM信号：\n
  -时间间隔 = SystemCoreClock / 65536\n
通道1和通道1N占空比设置为50%。\n
通道2和通道2N占空比设置为37.5%。\n
通道3和通道3N占空比设置为25%。\n
通道4占空比设置为12.5%。\n
使用示波器可以显示TIM1波形。\n

@par 目录内容 

  - TIM/TIM_7PWM_Output/TIM_7PWM_Output.uvprojx    MDK5工程文件
  - TIM/TIM_7PWM_Output/TIM_7PWM_Output.uvoptx     MDK5工程配置文件
  - TIM/TIM_7PWM_Output/JLinkSettings.ini          Jlink配置文件
  - TIM/TIM_7PWM_Output/main.c                     主程序


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振
    - 将TIM1引脚连接到示波器上，以监测不同的波形:
      - TIM1_CH1  pin (PA8)
      - TIM1_CH1N pin (PB13)
      - TIM1_CH2  pin (PA9)
      - TIM1_CH2N pin (PB14)
      - TIM1_CH3  pin (PA10)
      - TIM1_CH3N pin (PB15)
      - TIM1_CH4  pin (PB11)


@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 TIM_7PWM_Output.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

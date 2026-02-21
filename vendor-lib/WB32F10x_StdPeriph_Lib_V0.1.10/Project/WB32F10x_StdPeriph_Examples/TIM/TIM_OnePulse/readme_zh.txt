/**
  @page TIM_OnePulse TIM OnePulse example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    TIM/TIM_OnePulse/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the TIM_OnePulse example.
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

这个例子说明了如何使用TIM外围设备在定时器输入引脚中接收到外部信号的上升沿后生成单脉冲模式。\n
系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。\n
TIM1CLK = SystemCoreClock：\n
  -预分频器 = 0 \n
自动重载值为65535（TIM1->ARR），所以触发TIM1输入的最大频率值是96000000 / 65535 = 1464Hz。\n
TIM1配置如下：\n
使用单脉冲模式，外部信号连接至TIM1_CH2引脚（PA.09）。\n
上升沿作为有源边，输出单脉冲信号\n
在TIM1_CH1（PA.08）上。\n
Tim_脉冲定义延迟值，延迟值固定为：\n
delay = CCR1  / TIM1计数器时钟\n
      = 16383 / 96000000 = 170.65 us。\n
（TIM_period - TIM_pulse）定义一个脉冲值，脉冲值固定为：\n
一个脉冲值 =（Tim_周期 - Tim_脉冲）/ Tim1 计数器时钟 \n
           =（65535    - 16383）   / 96000000 = 512us。\n 

@par 目录内容 

  - TIM/TIM_OnePulse/TIM_OnePulse.uvprojx    MDK5工程文件
  - TIM/TIM_OnePulse/TIM_OnePulse.uvoptx     MDK5工程配置文件
  - TIM/TIM_OnePulse/JLinkSettings.ini     Jlink配置文件
  - TIM/TIM_OnePulse/main.c                主程序


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振
    - 引脚连接:
      - TIM1_CH1 pin  (PA.08) 接到示波器
      - TIM1_CH2 pin  (PA.09) 接到外部信号  

@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 TIM_OnePulse.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

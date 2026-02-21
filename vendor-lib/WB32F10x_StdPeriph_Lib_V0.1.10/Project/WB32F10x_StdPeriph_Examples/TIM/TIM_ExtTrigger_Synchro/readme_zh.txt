/**
  @page TIM_ExtTrigger_Synchro TIM ExtTrigger Synchro example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    TIM/TIM_ExtTrigger_Synchro/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the TIM_ExtTrigger_Synchro example.
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

本例展示了如何在级联模式下将TIM外围设备与外部触发器同步。\n
系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。\n
在本例中，使用了三个计时器：\n
1/TIM1配置为主定时器：\n
  -使用TOGGLE模式 \n
  -TIM1 ENABLE事件用作触发器输出 \n

2/TIM1被配置为连接到TIM1的外部触发器的从属计时器。\n
TI2引脚（Tim1 CH2配置为输入引脚）：\n
  -TIM1 TI2FP2用作触发器输入\n
  -上升沿用于启动和停止TIM1：门模式。\n

3/TIM3是TIM1的从机，TIM4的主机，\n
  -使用TOGGLE模式 \n
  -ITR1（TIM1）用作输入触发器 \n
  -使用门控模式，用作从计数器的启动和停止 \n
由主触发输出信号（TIM1启用事件）控制。\n
-TIM3启用事件用作触发器输出。\n

4/TIM4是TIM3的从机，\n
  -使用TOGGLE模式 \n
  -ITR2（TIM3）用作输入触发器\n
  -使用门控模式，用作从计数器的启动和停止 \n
由主触发输出信号（TIM3启用事件）控制。\n

三个计时器的运行时间为：\n
TIMx频率=TIMx时钟计数器 /（TIMx_周期 + 1）= 732.2Hz。\n
TIM1计数器的启动和停止由外部触发器控制。\n
TIM3启动和停止由TIM1控制，TIM4启动和停止由TIM3控制。\n

@par 目录内容 

  - TIM/TIM_ExtTrigger_Synchro/TIM_ExtTrigger_Synchro.uvprojx    MDK5工程文件
  - TIM/TIM_ExtTrigger_Synchro/TIM_ExtTrigger_Synchro.uvoptx     MDK5工程配置文件
  - TIM/TIM_ExtTrigger_Synchro/JLinkSettings.ini                 Jlink配置文件
  - TIM/TIM_ExtTrigger_Synchro/main.c                            主程序


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振
    - 将TIM1引脚连接到示波器上，以监测不同的波形:
      - TIM1_CH1 pin (PA.08)
      - TIM3_CH1 pin (PA.06)  
      - TIM4_CH1 pin (PB.06) 
    - 连接 TIM1_CH2 pin (PA.09) ExtTrigger Signal。
  

@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 TIM_ExtTrigger_Synchro.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

/**
  @page TIM_CascadeSynchro TIM_CascadeSynchro example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    TIM/TIM_CascadeSynchro/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the TIM_CascadeSynchro example.
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

本例程演示如何在TIM外设之间级联同步。

本例程系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。

在本例程中，使用了三个定时器：

定时器之间的级联同步关系如下：

1, TIM2作为主定时器：
    - 配置为PWM模式
    - TIM2更新事件作为触发输出

2, TIM3作为TIM2的从机和TIM4的主机。
    - 配置为PWM模式
    - ITR1(TIM2) 作为触发输入
    - 从模式配置为外部时钟模式1，由主机触发输出（TIM2更新事件）驱动计数器
    - TIM3更新事件作为触发输出

3, TIM4作为TIM3的从机。
    - 配置为PWM模式
    - ITR2(TIM3) 作为触发输入
    - 从模式配置为外部时钟模式1，由主机触发输出（TIM3更新事件）驱动计数器

在本例程中，TIM2的计数器时钟频率是96MHz。

TIM2频率 = (TIM2计数器时钟) / (TIM2_ARR + 1) = 375kHz，占空比 = TIM2_CCR1 / (TIM2_ARR + 1) = 25%  \n
TIM3频率 = (TIM2频率) / (TIM3_ARR + 1) = 93.75kHz，占空比 = TIM3_CCR1 / (TIM3_ARR + 1) = 25%  \n
TIM4频率 = (TIM3频率) / (TIM4_ARR + 1) = 23.4375kHz，占空比 = TIM4_CCR1 / (TIM4_ARR + 1) = 25%  \n


@par 目录内容 

  - TIM/TIM_CascadeSynchro/TIM_CascadeSynchro.uvprojx    MDK5工程文件
  - TIM/TIM_CascadeSynchro/TIM_CascadeSynchro.uvoptx     MDK5工程配置文件
  - TIM/TIM_CascadeSynchro/JLinkSettings.ini             Jlink配置文件
  - TIM/TIM_CascadeSynchro/main.c                        主程序


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振
    - 将下列引脚连接到示波器上，以监测不同的波形:
      - TIM2_CH1 (PA0)
      - TIM3_CH1 (PA6)
      - TIM4_CH1 (PB0)


@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 TIM_CascadeSynchro.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

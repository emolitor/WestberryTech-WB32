/**
  @page TIM_6Steps TIM 6Steps example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    TIM/TIM_6Steps/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the TIM_6Steps example.
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
这个例程展示了如何配置TIM1外设来生成6步输出。\n
系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。\n
TIM1外设可以预先编程下一个TIM1输出行为的配置（步骤），当COM使用（换向）事件，同时更改所有通道的配置。\n
COM时间可以通过设置TIM1_EGR COM bit 来软件生成或者由硬件的TRC rising edge。\n
在这个例程中，软件COM EVENT 通过滴答定时器中断每100ms生成1次。\n
Break极性设置为高。\n

下面表格描述了TIM1 channel的状态:
@verbatim
              -----------------------------------------------
             | Step1 | Step2 | Step3 | Step4 | Step5 | Step6 |
   ----------------------------------------------------------
  |Channel1  |   1   |   0   |   0   |   0   |   0   |   1   |
   ----------------------------------------------------------
  |Channel1N |   0   |   0   |   1   |   1   |   0   |   0   |
   ----------------------------------------------------------
  |Channel2  |   0   |   0   |   0   |   1   |   1   |   0   |
   ----------------------------------------------------------
  |Channel2N |   1   |   1   |   0   |   0   |   0   |   0   |
   ----------------------------------------------------------
  |Channel3  |   0   |   1   |   1   |   0   |   0   |   0   |
   ----------------------------------------------------------
  |Channel3N |   0   |   0   |   0   |   0   |   1   |   1   |
   ----------------------------------------------------------
 @endverbatim

@par 目录内容 

  - TIM/TIM_6Steps/TIM_6Steps.uvprojx    MDK5工程文件
  - TIM/TIM_6Steps/TIM_6Steps.uvoptx     MDK5工程配置文件
  - TIM/TIM_6Steps/JLinkSettings.ini     Jlink配置文件
  - TIM/TIM_6Steps/main.c                主程序


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振
    - 将TIM1引脚连接到示波器上，以监测不同的波形:
      - TIM1_CH1 pin  (PA8)
      - TIM1_CH1N pin (PB13)
      - TIM1_CH2  pin (PA9)
      - TIM1_CH2N pin (PB14)
      - TIM1_CH3  pin (PA10)
      - TIM1_CH3N pin (PB15)
    - 连接 TIM1 break pin TIM1_BKIN pin (PB12) to the VCC 来生成Break。


@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 TIM_6Steps.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

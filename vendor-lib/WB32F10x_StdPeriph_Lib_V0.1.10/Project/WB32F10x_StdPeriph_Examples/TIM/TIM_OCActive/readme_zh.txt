/**
  @page TIM_OCActive TIM OCActive example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    TIM/TIM_OCActive/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the TIM_OCActive example.
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

本例程演示如何配置TIM外设生成四路不同延时的信号。\n
系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。\n

TIM1CLK 频率设置为 SystemCoreClock (Hz)。目的是获得1MHz的TIM1计数器时钟，因此预分频器的计算如下:\n
   - Prescaler = (96000000 / 1000000) - 1\n

TIM1 CCR1 寄存器值设为 800:\n
TIM1_CH1 delay = CCR1_Val / TIM1 counter clock  = 800 us \n
所以TIM1channel1生成延时为800us延时的信号。\n

TIM1 CCR2 寄存器值设为 400:\n
TIM1_CH2 delay = CCR2_Val / TIM1 counter clock  = 400 us \n
所以TIM1 Channel2生成延时为400us延时的信号。\n

TIM1 CCR3 寄存器值设为 200:
TIM1_CH3 delay = CCR3_Val / TIM1 counter clock  = 200 us \n
所以TIM1 Channel3生成延时为200us延时的信号。\n

TIM1 CCR4 寄存器值设为 100:\n
TIM1_CH4 delay = CCR4_Val / TIM1 counter clock  = 100 us \n
所以TIM1 Channel4生成延时为100us延时的信号。\n

@par 目录内容 

  - TIM/TIM_OCActive/TIM_OCActive.uvprojx    MDK5工程文件
  - TIM/TIM_OCActive/TIM_OCActive.uvoptx     MDK5工程配置文件
  - TIM/TIM_OCActive/JLinkSettings.ini     Jlink配置文件
  - TIM/TIM_OCActive/main.c                主程序


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振
    - 将TIM1引脚连接到示波器上，以监测不同的波形:
      - TIM1_CH1 pin  (PA.08)
      - TIM1_CH2  pin (PA.09)  
      - TIM1_CH3  pin (PA.10)  
      - TIM1_CH4  pin (PA.11)    

@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 TIM_OCActive.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

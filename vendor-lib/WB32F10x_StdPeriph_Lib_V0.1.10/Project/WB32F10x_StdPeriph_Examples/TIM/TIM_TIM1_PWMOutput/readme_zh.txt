/**
  @page TIM_TIM1_PWMOutput TIM TIM1 PWMOutput example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    TIM/TIM_TIM1_PWMOutput/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the TIM_TIM1_PWMOutput example.
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

本例程演示了如何配置TIM外设生成PWM（脉冲调制）\n
系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。\n
将TIM1CLK频率设置为SystemCoreClock，以获得96MHz下的TIM1计数器时钟，预分频器计算如下： \n
   - Prescaler = (TIM1CLK / TIM1 counter clock) - 1 \n
TIM1 Frequency = TIM1 counter clock / (ARR + 1) \n
               = 96MHz / 65536 = 1.46KHz \n
TIM1->CCR1寄存器值等于0x7FFF，因此TIM1通道1生成频率等于1.46KHz、占空比等于50%的PWM信号：\n
TIM1 Channel1占空比=（TIM1_CCR1 / TIM1_ARR + 1）* 100 = 50% \n
可以使用示波器显示脉宽调制波形。 

@par 目录内容 

  - TIM/TIM_TIM1_PWMOutput/TIM_TIM1_PWMOutput.uvprojx    MDK5工程文件
  - TIM/TIM_TIM1_PWMOutput/TIM_TIM1_PWMOutput.uvoptx     MDK5工程配置文件
  - TIM/TIM_TIM1_PWMOutput/JLinkSettings.ini             Jlink配置文件
  - TIM/TIM_TIM1_PWMOutput/main.c                        主程序


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振
    - 将TIM1引脚连接到示波器上，以监测波形:
      - TIM1_CH1 pin  (PA.08)
  

@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 TIM_TIM1_PWMOutput.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

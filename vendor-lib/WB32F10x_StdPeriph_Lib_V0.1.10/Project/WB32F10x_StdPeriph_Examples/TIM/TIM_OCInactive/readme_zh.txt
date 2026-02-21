/**
  @page TIM_OCInactive TIM OCInactive example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    TIM/TIM_OCInactive/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the TIM_OCInactive example.
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

本例程展示了如何配置TIM外设输出 Output Compare Inactive mode以及每个通道的相应中断请求。\n
系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。\n

将TIM1CLK频率设置为SystemCoreClock / 96（Hz），目标是以1MHz的频率获得TIM1计数器时钟，因此预分频器的计算如下：\n
  -预分频器=（TIM1CLK / TIM1计数器时钟）- 1\n
  
TIM1 CCR1寄存器值等于800：\n
TIM1_CC1 delay = CCR1_VAL/TIM1 counter clock=800us \n
因此，PA.08在延迟等于800 us后重置。\n

TIM1 CCR2寄存器值等于400：\n
TIM1_CC2 delay = CCR2_VAL/TIM1 counter clock=400us \n
因此，PA.09在延迟等于400 us后重置。\n

TIM1 CCR3寄存器值等于200：\n
TIM1_CC3 delay = CCR3_VAL/TIM1 counter clock=200us \n
因此，PA.10在延迟等于200 us后重置。\n

TIM1 CCR4寄存器值等于100：\n
TIM1_CC4 delay = CCR4_VAL/TIM1 counter clock=100us \n
因此，PA.11在延迟等于100 us后重置。 \n


@par 目录内容 

  - TIM/TIM_OCInactive/TIM_OCInactive.uvprojx    MDK5工程文件
  - TIM/TIM_OCInactive/TIM_OCInactive.uvoptx     MDK5工程配置文件
  - TIM/TIM_OCInactive/JLinkSettings.ini     Jlink配置文件
  - TIM/TIM_OCInactive/main.c                主程序


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振
    - 将TIM1引脚连接到示波器上，以监测不同的波形:
      - TIM1_CH1 pin (PA.08)
      - TIM1_CH2 pin (PA.09)  
      - TIM1_CH3 pin (PA.10)  
      - TIM1_CH4 pin (PA.11)    
  

@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 TIM_OCInactive.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

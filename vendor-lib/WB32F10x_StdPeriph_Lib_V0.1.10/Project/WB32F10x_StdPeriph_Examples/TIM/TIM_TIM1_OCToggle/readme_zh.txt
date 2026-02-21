/**
  @page TIM_TIM1_OCToggle TIM TIM1 OCToggle example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    TIM/TIM_TIM1_OCToggle/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the TIM_TIM1_OCToggle example.
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

这个例子展示了如何配置TIM1外设以生成两个不同频率的两个不同信号。\n
系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。\n
TIM1CLK频率设置为SystemCoreClock，我们希望得到96 MHz的TIM1计数器时钟，因此预分频器的计算如下：\n
  -预分频器=（TIM1CLK / TIM1计数器时钟）- 1 \n
TIM1->CCR1寄存器值等于32768：\n
CC1更新率 = TIM1计数器时钟 / CCR1VAL = 1.46KHz，\n
因此，TIM1通道1产生一个频率等于1.46KHz的周期信号。\n
TIM1->CCR2寄存器等于16384：\n
CC2更新率 = TIM1计数器时钟 / CCR2VAL = 2.92KHz \n
因此，TIM1通道2产生一个频率等于2.92KHz的周期信号。\n
TIM1->CCR3寄存器等于16384：\n
CC3更新率 = TIM1计数器时钟 / CCR3VAL = 5.85KHz \n
因此，TIM1通道3产生一个频率等于5.85KHz的周期信号。\n
TIM1->CCR4寄存器等于16384：\n
CC4更新率 = TIM1计数器时钟 / CCR4VAL = 11.7KHz \n
因此，TIM1通道4产生一个频率等于11.7KHz的周期信号。\n

@par 目录内容 

  - TIM/TIM_TIM1_OCToggle/TIM_TIM1_OCToggle.uvprojx    MDK5工程文件
  - TIM/TIM_TIM1_OCToggle/TIM_TIM1_OCToggle.uvoptx     MDK5工程配置文件
  - TIM/TIM_TIM1_OCToggle/JLinkSettings.ini            Jlink配置文件
  - TIM/TIM_TIM1_OCToggle/main.c                       主程序


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
 - 使用 MDK5 打开此例程文件夹中的 TIM_TIM1_OCToggle.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

/**
  @page TIM_InputCapture TIM InputCapture example 
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    TIM/TIM_InputCapture/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the TIM_InputCapture example.
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

这个例程展示了如何使用TIM外设测量外部频率。\n
系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。\n

TIMxCLK 频率 设置为 SystemCoreClock (Hz)，分频系数设为0 \n
TIM1 计数时钟为 SystemCoreClock (Hz)。\n
SystemCoreClock 设置为96MHz。\n

TIM1 配置为输入捕获模式：TIM1的通道2连接到外部信号。\n
用TIM1的CC2中断请求测量频率。所以当进入TIM1_IRQHandler频率已计算完成。\n
"TIM1Freq" = 外部信号频率:\n
TIM1Freq = TIM1 counter clock / Capture in Hz，\n
Capture 是两次捕获TIM1 captures的差值。\n


@par 目录内容 

  - TIM/TIM_InputCapture/TIM_InputCapture.uvprojx    MDK5工程文件
  - TIM/TIM_InputCapture/TIM_InputCapture.uvoptx     MDK5工程配置文件
  - TIM/TIM_InputCapture/JLinkSettings.ini           Jlink配置文件
  - TIM/TIM_InputCapture/main.c                      主程序


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振
    - 引脚连接 :
      - TIM1_CH1 pin (PA.08) 接外部要测量的信号

@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 TIM_InputCapture.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

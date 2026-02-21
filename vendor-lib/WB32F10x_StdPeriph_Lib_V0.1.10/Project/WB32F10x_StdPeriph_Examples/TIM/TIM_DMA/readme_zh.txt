/**
  @page TIM_DMA TIM DMA example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    TIM/TIM_DMA/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the TIM_DMA example.
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

这个例子描述了如何使用DMA和TIM1更新请求将数据从内存传输到TIM1->CCR2。\n
系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。\n
TIM1CLK = SystemCoreClock，预分频器 = 0，TIM1计数器时钟 = SystemCoreClock。\n
目标是配置TIM1通道2，以生成频率等于375KHz的PWM信号。\n

@par 目录内容 

  - TIM/TIM_DMA/TIM_DMA.uvprojx       MDK5工程文件
  - TIM/TIM_DMA/TIM_DMA.uvoptx        MDK5工程配置文件
  - TIM/TIM_DMA/JLinkSettings.ini     Jlink配置文件
  - TIM/TIM_DMA/main.c                主程序


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振
    - 将TIM1引脚连接到示波器上，以监测波形:
      - TIM1_CH2  pin (PA.09)    
  

@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 TIM_DMA.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

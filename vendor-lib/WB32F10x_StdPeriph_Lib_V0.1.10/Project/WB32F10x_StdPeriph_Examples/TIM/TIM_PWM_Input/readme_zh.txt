/**
  @page TIM_PWM_Input TIM PWM Input example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    TIM/TIM_PWM_Input/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the TIM_PWM_Input example.
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

本例演示如何使用TIM外围设备测量外部信号的频率和占空比。 \n
系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。\n
TIMXCLK频率设置为SystemCoreClock（Hz），预分频器为0，因此TIM1计数器时钟为SystemCoreClock（Hz）。\n
SystemCoreClock设置为96MHz。\n
TIM1配置为PWM输入模式：外部信号连接至TIM1 Channel1用作输入引脚。\n
为了测量频率和占空比，我们使用TIM1 CC1中断请求，\n
因此，在TIM1_IRQHandler 程序中，计算外部信号的频率和占空比。\n
“Frequency”变量包含外部信号频率：\n
Frequency = TIM1计数器时钟 / (TIM1_CCR1 + 1)，单位：Hz，\n
“DutyCycle”变量包含外部信号占空比：\n
DutyCycle = ((float)(IC2Value + 1) * 100) / (IC1Value + 1);\n
测量的最小频率值为92Hz。\n

@par 目录内容 

  - TIM/TIM_PWM_Input/TIM_PWM_Input.uvprojx    MDK5工程文件
  - TIM/TIM_PWM_Input/TIM_PWM_Input.uvoptx     MDK5工程配置文件
  - TIM/TIM_PWM_Input/JLinkSettings.ini        Jlink配置文件
  - TIM/TIM_PWM_Input/main.c                   主程序


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振
    - 引脚连接:
      - TIM1_CH1 pin  (PA.08) 连接到采样引脚

@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 TIM_PWM_Input.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

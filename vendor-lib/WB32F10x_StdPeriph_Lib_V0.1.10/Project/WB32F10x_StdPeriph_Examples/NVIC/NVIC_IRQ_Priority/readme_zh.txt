/**
  @page NVIC_IRQ_Priority NVIC IRQ Priority example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    NVIC/NVIC_IRQ_Priority/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the NVIC_IRQ_Priority example.
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

这个例子演示了嵌套向量中断控制器（NVIC）的使用：\n
系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。\n
  -配置2条Exit line（唤醒按钮Exit line和按键Exit line），在对应降沿上生成一个中断，并使用SysTick中断。\n
  -这些中断用以下参数配置：\n

  -唤醒按钮外线：\n
  -Preemptionpriority = Preemptionpriorityvalue \n
  -次优度 = 0 \n

  -按键外线：\n
  -抢占优先级 = 0 \n
  -次优度 = 1 \n

  -SysTick处理程序：\n
  -抢占优先权 =！抢占优先级值 \n
  -次优度 = 0 \n

首先，抢占优先级值等于0，唤醒按钮Exti line的抢占优先级高于SysTick处理程序。\n
在键按钮Exti Line中断程序中，唤醒按钮Exti Line和Systick Preemption优先级颠倒。\n
在唤醒按钮Exti Line Interrupt例行程序中，设置了SysTick中断的挂起位，这将导致SysTick ISR仅在具有更高的抢占优先级时抢占唤醒按钮Exti Line ISR。\n
系统行为如下：\n
1）第一次出现按键Exti line中断时，SysTick抢占比唤醒按钮Exti line高。因此，当唤醒按钮Exti线路中断发生时，系统会执行Systick ISR，抢占发生的变量变为真，LED1开始切换。\n
2）当下一个键按钮Exti line中断发生时，SysTick抢占比唤醒按钮Exti line 低。\n
因此，当唤醒按钮Exti line中断发生时，Preemptionoccured变量变为0，LED1停止切换。\n
然后在无限循环中）重复此行为。\n

@par 目录内容 

  - NVIC/NVIC_IRQ_Priority/NVIC_IRQ_Priority.uvprojx    MDK5工程文件
  - NVIC/NVIC_IRQ_Priority/NVIC_IRQ_Priority.uvoptx     MDK5工程配置文件
  - NVIC/NVIC_IRQ_Priority/JLinkSettings.ini            Jlink配置文件
  - NVIC/NVIC_IRQ_Priority/main.c                       主程序

@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振
    - 管脚连接如下：
      - PA0  管脚连接到 wakeup key
      - PA1  管脚连接到 key2
      - PB14 管脚连接到 LED1

@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 NVIC_IRQ_Priority.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

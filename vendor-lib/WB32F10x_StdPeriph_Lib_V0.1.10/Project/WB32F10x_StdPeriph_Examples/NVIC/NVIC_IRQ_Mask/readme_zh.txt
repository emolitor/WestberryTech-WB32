/**
  @page NVIC_IRQ_Mask NVIC IRQ Mask example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    NVIC/NVIC_IRQ_Mask/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the NVIC IRQ Mask example.
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

本例演示了嵌套矢量中断控制器（NVIC）IRQ通道配置的使用，以及如何屏蔽/激活不同的IRQ：\n
系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。\n
-配置3个TIM（TIM2..TIM4）计时器，在每个计数器更新事件上生成中断。\n
-这三个计时器链接到相应的IRQ更新通道。\n
-分配每个IRQ通道的上升IRQ优先级：\n
-TIM2的优先权为0。\n
-TIM3的优先权为1。\n
-TIM4的优先权为2。\n
-在每个中断程序中：\n
-TIM2切换PA8 \n
-TIM3切换PA9 \n
-TIM4切换PA10 \n

-按键和唤醒按钮用于提高执行优先级，如下所示：\n
-按键在GPIO模式下使用，每次按键时，执行优先级提升至0并打开LED。这可以防止所有具有可配置优先级的异常激活，而不是通过硬故障升级机制激活。\n
因此，PA8，PA9，PA10停止切换，因为TIM2、TIM3和TIM4 IRQ被阻止激活。\n
再次按下按键将释放优先级提升，关闭LED2，并允许激活所有具有可配置优先级的异常，再次生成TIM2、TIM3和TIM4，GPIO重新启动切换。\n
此执行优先级是使用CMSIS函数“__disable_irq()”和“__enable_irq()”实现的。这两个功能是管理Cortex-M3 Primask专用寄存器。\n

-唤醒按钮在EXTI模式下使用，每次按下唤醒按钮时，使用BASEPRI寄存器将执行优先级屏蔽为0x40。\n
当BASEPRI定义的优先级与当前执行优先级相同或更高时，非零值将充当优先级掩码，影响执行优先级。\n
因此，PA9和PA10停止切换，因为TIM3_IRQ和TIM4_IRQ无法激活。\n
再次按下唤醒按钮将把BASEPRI寄存器配置为0，因此对当前优先级没有影响，可以再次生成TIM3和TIM4，并重新启动PA9和PA10切换。\n
此执行优先级是使用CMSIS函数“__set_rasepri()”设置的。\n
此功能用于管理Cortex-M3 BASEPRI专用寄存器。\n
当使用“__disable_irq()”函数将执行优先级提升为0时，设置BASEPRI寄存器无效。\n

@par 目录内容 

  - NVIC/NVIC_IRQ_Mask/NVIC_IRQ_Mask.uvprojx    MDK5工程文件
  - NVIC/NVIC_IRQ_Mask/NVIC_IRQ_Mask.uvoptx     MDK5工程配置文件
  - NVIC/NVIC_IRQ_Mask/JLinkSettings.ini        Jlink配置文件
  - NVIC/NVIC_IRQ_Mask/main.c                   主程序

@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振
    - 管脚连接如下：
      - PA8  管脚连接到示波器
      - PA9  管脚连接到示波器
      - PA10 管脚连接到示波器
      - PB11 管脚连接到LED2
      - PA0  管脚连接到WakeUp按键（KEY1）
      - PA1  管脚连接到KEY2

@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 NVIC_IRQ_Mask.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

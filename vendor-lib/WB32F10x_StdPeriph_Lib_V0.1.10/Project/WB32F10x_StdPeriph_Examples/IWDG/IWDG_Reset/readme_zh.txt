/**
  @page IWDG_Reset IWDG_Reset example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    IWDG/IWDG_Reset/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the IWDG IWDG_Reset example.
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

本例程演示如何使用IWDG。

本例程系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。

需要注意目前IWDG存在的一些限制： \n
1, 一旦启用了IWDG，即使发生复位也无法禁用它。 \n
解决方法：在程序运行之前将IWDG超时设置配置为最大，并且之后要不断重载IWDG计数器。

2, 如果IWDG启动之后发生复位，那么IWDG域需要3个LSI时钟周期才能就绪。 \n
解决方法：在LSI就绪后，配置IWDG之前，等待大概1毫秒。

3, 不断执行IWDG reload counter操作有几率无法reload IWDG counter. \n
解决方法：在执行reload IWDG counter操作之前确保RVU位是0。（RVU位指示reload counter操作是否完成，）


@par 目录内容 

  - IWDG/IWDG_Reset/IWDG_Reset.uvprojx     MDK5工程文件
  - IWDG/IWDG_Reset/IWDG_Reset.uvoptx      MDK5工程配置文件
  - IWDG/IWDG_Reset/JLinkSettings.ini      Jlink配置文件
  - IWDG/IWDG_Reset/main.c                 主程序


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振


@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 IWDG_Reset.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

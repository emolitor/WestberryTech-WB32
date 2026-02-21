/**
  @page BKP_Backup_Data BKP Backup Data example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    BKP/BKP_Backup_Data/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the BKP_Backup_Data example.
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

本例程演示如何将用户数据存储在备份数据寄存器中。

本例程系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。

由于在关闭VDD时，备份域（BKP）仍由VBAT供电，因此如果电池连接到VBAT pin，则其内容不会丢失。


@par 目录内容 

  - BKP/BKP_Backup_Data/BKP_Backup_Data.uvprojx    MDK5工程文件
  - BKP/BKP_Backup_Data/BKP_Backup_Data.uvoptx     MDK5工程配置文件
  - BKP/BKP_Backup_Data/JLinkSettings.ini          Jlink配置文件
  - BKP/BKP_Backup_Data/main.c                     主程序


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振
    - 芯片管脚连接如下：
      - PB14 管脚连接到 LED1
      - PB13 管脚连接到 LED2


@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 BKP_Backup_Data.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */
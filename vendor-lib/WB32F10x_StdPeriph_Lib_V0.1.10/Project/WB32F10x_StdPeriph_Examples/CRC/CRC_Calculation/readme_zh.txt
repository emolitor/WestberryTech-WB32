/**
  @page CRC_Calculation CRC_Calculation example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    CRC/CRC_Calculation/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the CRC CRC_Calculation example.
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

本例程演示如何使用CRC模块基于不同的多项式，不同的计算配置去计算字节数组，半字(16-bit)数组和字(32-bit)数组的CRC校验和。

本例程系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。


@par 目录内容 

  - CRC/CRC_Calculation/CRC_Calculation.uvprojx   MDK5工程文件
  - CRC/CRC_Calculation/CRC_Calculation.uvoptx    MDK5工程配置文件
  - CRC/CRC_Calculation/JLinkSettings.ini         Jlink配置文件
  - CRC/CRC_Calculation/main.c                    主程序
  - CRC/CRC_Calculation/computeBytes.c            计算字节数组CRC的程序
  - CRC/CRC_Calculation/computeHalfWords.c        计算半字数组CRC的程序
  - CRC/CRC_Calculation/computeWords.c            计算字数组CRC的程序
  - CRC/CRC_Calculation/compute.h                 compute*.c 的头文件


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振


@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 CRC_Calculation.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

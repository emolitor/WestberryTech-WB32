/**
  @page ADC_DMA ADC_DMA example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    ADC/ADC_DMA/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the ADC ADC_DMA example.
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

本例程演示如何使用ADC和DMAC将规则组通道连续转换的结果从ADC传输到内存。

本例程系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。

在本例程中使用了2个ADC的通道，并定义了一个名为ADCConvertedDataBuffer的数组，用以存储这2个通道10次转换的结果。
由DMAC将ADC转换的数据持续不断的搬运到ADCConvertedDataBuffer数组中。


@par 目录内容 

  - ADC/ADC_DMA/ADC_DMA.uvprojx        MDK5工程文件
  - ADC/ADC_DMA/ADC_DMA.uvoptx         MDK5工程配置文件
  - ADC/ADC_DMA/JLinkSettings.ini      Jlink配置文件
  - ADC/ADC_DMA/main.c                 主程序


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振


@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 ADC_DMA.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

/**
  @page I2S_RecordPlayAudio I2S_RecordPlayAudio example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    I2S/I2S_RecordPlayAudio/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the I2S I2S_RecordPlayAudio example.
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

本例程演示如何使用I2S模块录制和播放音频。

本例程系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。

在本例程中使用 ES8316 CODEC 芯片录制和播放音频。
当本例程运行后，会通过UART1打印出相关信息，输入任意字符后，程序会开始以8kHz采样率从MIC录制1秒的声音。
当录音完成后，将会开始从耳机和喇叭重复播放刚才录制的声音。

在本例程中音频的采样频率是8000Hz。


@par 目录内容 

  - I2S/I2S_RecordPlayAudio/I2S_RecordPlayAudio.uvprojx   MDK5工程文件
  - I2S/I2S_RecordPlayAudio/I2S_RecordPlayAudio.uvoptx    MDK5工程配置文件
  - I2S/I2S_RecordPlayAudio/JLinkSettings.ini             Jlink配置文件
  - I2S/I2S_RecordPlayAudio/main.c                        主程序
  - I2S/I2S_RecordPlayAudio/drv_es8316.c                  ES8316驱动程序
  - I2S/I2S_RecordPlayAudio/drv_es8316.h                  ES8316驱动程序头文件


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振


@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 I2S_RecordPlayAudio.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

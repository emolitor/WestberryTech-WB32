/**
  @page I2C_MasterDMARx_SlaveDMATx I2C_MasterDMARx_SlaveDMATx example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    I2C/I2C_MasterDMARx_SlaveDMATx/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the I2C I2C_MasterDMARx_SlaveDMATx example.
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

本例程演示I2C主机DMA接收和I2C从机DMA发送。

本例程系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。

在本例程中，I2C1作为从机，I2C2作为主机，I2C1和I2C2需要连接在一起； \n
DMAC2 Channel 0 用作I2C1从机发送； \n
DMAC2 Channel 1 用作I2C2主机发送； \n
DMAC2 Channel 2 用作I2C2主机接收； \n
当DMA传输完成后，检查I2C2接收到的数据是否与I2C1发出的数据相同，
并通过UART1打印出检查结果。


@par 目录内容 

  - I2C/I2C_MasterDMARx_SlaveDMATx/I2C_MasterDMARx_SlaveDMATx.uvprojx   MDK5工程文件
  - I2C/I2C_MasterDMARx_SlaveDMATx/I2C_MasterDMARx_SlaveDMATx.uvoptx    MDK5工程配置文件
  - I2C/I2C_MasterDMARx_SlaveDMATx/JLinkSettings.ini                    Jlink配置文件
  - I2C/I2C_MasterDMARx_SlaveDMATx/main.c                               主程序


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振
    - UART1 (PA9, PA10) 连接到PC
    - PB6 (I2C1_SCL) 和 PB10 (I2C2_SCL) 连接在一起
    - PB7 (I2C1_SDA) 和 PB11 (I2C2_SDA) 连接在一起


@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 I2C_MasterDMARx_SlaveDMATx.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

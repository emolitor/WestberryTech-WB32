/**
  @page QSPI_QuadSPI_FLASH QSPI_QuadSPI_FLASH example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    SPI/QSPI_QuadSPI_FLASH/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the SPI QSPI_QuadSPI_FLASH example.
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

本例程演示如何通过Quad SPI接口与SPI FLASH通信。

本例程系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。

首先是读取SPI Flash ID并打印输出；然后通过Quad SPI接口读取ID并打印。

然后使用SPI FLASH驱动程序擦除要访问的扇区，将WriteData中的数据通过Quad SPI接口写入存储器；
然后通过Quad SPI接口读取存储器，数据存放在ReadBuffer中。然后将ReadBuffer的数据与WriteData中的期望值进行比较。并打印输出比较结果。

为了避免TX FIFO下溢或RX FIFO上溢，QSPI的通信速率不应过快；所以在本例中，QSPI通信速率被配置为 2.4 Mbit/s。


@par 目录内容 

  - SPI/QSPI_QuadSPI_FLASH/QSPI_QuadSPI_FLASH.uvprojx   MDK5工程文件
  - SPI/QSPI_QuadSPI_FLASH/QSPI_QuadSPI_FLASH.uvoptx    MDK5工程配置文件
  - SPI/QSPI_QuadSPI_FLASH/JLinkSettings.ini            Jlink配置文件
  - SPI/QSPI_QuadSPI_FLASH/main.c                       主程序
  - SPI/QSPI_QuadSPI_FLASH/drv_spi_flash_quad.c         QSPI Flash驱动程序
  - SPI/QSPI_QuadSPI_FLASH/drv_spi_flash_quad.h         QSPI Flash驱动程序头文件


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振
    - QSPI管脚和QSPI FLASH连接如下：
      - QSPI_NSS0 (PA4) 管脚连接到 QSPI Flash CS (pin1)
      - QSPI_SCK (PA5) 管脚连接到 QSPI Flash CLK (pin6)
      - QSPI_MI_IO1 (PA6) 管脚连接到 QSPI Flash DO(IO1) (pin2)
      - QSPI_MO_IO0 (PA7) 管脚连接到 QSPI Flash DI(IO0) (pin5)
      - QSPI_IO2 (PB0) 管脚连接到 QSPI Flash /WP(IO2) (pin3)
      - QSPI_IO3 (PB1) 管脚连接到 QSPI Flash /HOLD(IO3) (pin7)


@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 QSPI_QuadSPI_FLASH.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

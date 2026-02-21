/**
  @page USB_Mass_Storage_SRAM USB_Mass_Storage_SRAM example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    USB/USB_Mass_Storage_SRAM/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the USB USB_Mass_Storage_SRAM example.
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

本例程演示如何使用USB模块实现U盘的功能（内部RAM作为存储）。

本例程系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。

本例程运行后，LED1和LED2会点亮，当通过USB连接到电脑时，将会看到资源管理器里出现了一个新加入的U盘设备。
当电脑休眠时，USB会进入Suspend状态，LED2熄灭；当电脑被唤醒后，USB会恢复，LED2点亮。


@par 目录内容 

  - USB/USB_Mass_Storage_SRAM/USB_Mass_Storage_SRAM.uvprojx     MDK5工程文件
  - USB/USB_Mass_Storage_SRAM/USB_Mass_Storage_SRAM.uvoptx      MDK5工程配置文件
  - USB/USB_Mass_Storage_SRAM/JLinkSettings.ini                 Jlink配置文件
  - USB/USB_Mass_Storage_SRAM/main.c                            主程序
  - USB/USB_Mass_Storage_SRAM/usb_msc_def.h                     MSC类相关定义
  - USB/USB_Mass_Storage_SRAM/usbd_msc.c                        MSC类请求处理程序
  - USB/USB_Mass_Storage_SRAM/usbd_msc.h                        MSC类请求处理程序头文件
  - USB/USB_Mass_Storage_SRAM/usbd_msc_scsi.c                   MSC类请求SCSI命令处理程序
  - USB/USB_Mass_Storage_SRAM/usbd_msc_scsi.h                   MSC类请求SCSI命令处理程序头文件
  - USB/USB_Mass_Storage_SRAM/usbd_user.c                       USBD用户程序
  - USB/USB_Mass_Storage_SRAM/usbd_user.h                       USBD用户程序头文件
  - USB/USB_Mass_Storage_SRAM/usbd_user_desc.c                  USBD描述符
  - USB/USB_Mass_Storage_SRAM/usbd_user_msc.c                   USBD用户MSC类程序
  - USB/USB_Mass_Storage_SRAM/usbd_user_msc.h                   USBD用户MSC类程序头文件


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振


@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 USB_Mass_Storage_SRAM.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

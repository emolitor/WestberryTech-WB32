/**
  @page USB_CDC_Echo USB_CDC_Echo example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    USB/USB_CDC_Echo/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the USB USB_CDC_Echo example.
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

本例程演示如何使用USB模块实现串口设备。

本例程系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。

本例程运行后，LED1和LED2会点亮，当通过USB连接到电脑时，将会在设备管理器中看到加入了一个串口。
通过串口调试工具发送数据，将会回环到PC。
当电脑休眠时，USB会进入Suspend状态，LED2熄灭；当电脑被唤醒后，USB会恢复，LED2点亮。

在 Windows 10 操作系统中，当USB串口设备连接到电脑时，会自动安装串口驱动。

在 Windows 7 操作系统中，需要手动安装串口驱动，在本例程中提供了安装串口驱动的必要文件（USB_Serial.inf 和 USB_Serial.cat）。
当USB串口设备连接到电脑时，在操作系统的设备管理器中，选择更新USB串口设备的驱动程序，定位到串口驱动文件所在位置即可安装。


@par 目录内容 

  - USB/USB_CDC_Echo/USB_CDC_Echo.uvprojx     MDK5工程文件
  - USB/USB_CDC_Echo/USB_CDC_Echo.uvoptx      MDK5工程配置文件
  - USB/USB_CDC_Echo/JLinkSettings.ini        Jlink配置文件
  - USB/USB_CDC_Echo/main.c                   主程序
  - USB/USB_CDC_Echo/usb_cdc_def.h            CDC类相关定义
  - USB/USB_CDC_Echo/usbd_cdc.c               CDC类请求处理程序
  - USB/USB_CDC_Echo/usbd_cdc.h               CDC类请求处理程序头文件
  - USB/USB_CDC_Echo/usbd_user.c              USBD用户程序
  - USB/USB_CDC_Echo/usbd_user.h              USBD用户程序头文件
  - USB/USB_CDC_Echo/usbd_user_cdc.c          USBD用户CDC类程序
  - USB/USB_CDC_Echo/usbd_user_cdc.h          USBD用户CDC类程序头文件
  - USB/USB_CDC_Echo/usbd_user_desc.c         USBD描述符
  - USB/USB_CDC_Echo/USB_Serial.cat           Windows 串口驱动文件
  - USB/USB_CDC_Echo/USB_Serial.inf           Windows 串口驱动文件


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振


@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 USB_CDC_Echo.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

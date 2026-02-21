/**
  @page USB_HID_Mouse USB_HID_Mouse example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    USB/USB_HID_Mouse/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the USB USB_HID_Mouse example.
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

本例程演示如何使用USB模块实现鼠标。

本例程系统时钟工作在96Mhz，通过PLL配置产生。PLL时钟源是外部8Mhz晶振。APB1的分频比是1，APB2的分频比是1。


本例程运行后，LED1和LED2会点亮，当通过USB连接到电脑时，将会在设备管理器中看到加入了一个鼠标设备。
每按下KEY1按键，鼠标会向右移动；KEY2按键代表鼠标左键。
当电脑休眠时，USB会进入Suspend状态，LED2熄灭；当电脑被唤醒后，USB会恢复，LED2点亮。
另外，在Suspend状态按下KEY2按键可以远程唤醒电脑。


@par 目录内容 

  - USB/USB_HID_Mouse/USB_HID_Mouse.uvprojx     MDK5工程文件
  - USB/USB_HID_Mouse/USB_HID_Mouse.uvoptx      MDK5工程配置文件
  - USB/USB_HID_Mouse/JLinkSettings.ini         Jlink配置文件
  - USB/USB_HID_Mouse/main.c                    主程序
  - USB/USB_HID_Mouse/usb_hid_def.h             HID类相关定义
  - USB/USB_HID_Mouse/usbd_hid.c                HID类请求处理程序
  - USB/USB_HID_Mouse/usbd_hid.h                HID类请求处理程序头文件
  - USB/USB_HID_Mouse/usbd_user.c               USBD用户程序
  - USB/USB_HID_Mouse/usbd_user.h               USBD用户程序头文件
  - USB/USB_HID_Mouse/usbd_user_hid.c           USBD用户HID类程序
  - USB/USB_HID_Mouse/usbd_user_hid.h           USBD用户HID类程序头文件
  - USB/USB_HID_Mouse/usbd_user_desc.c          USBD描述符


@par 硬件和软件环境 

  - 硬件平台搭建
    - HSE 连接外部8MHz晶振


@par 如何使用？ 

为了使程序工作，您必须执行以下操作 :
 - 使用 MDK5 打开此例程文件夹中的 USB_HID_Mouse.uvprojx 工程
 - 重新编译该工程并下载到目标存储器中
 - 运行此例程
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

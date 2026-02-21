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

@par Example Description 

This example describes how to use USB module to implement a mouse.

In this example, Main clock is from PLL clock and it is configured at 96MHz. The PLL clock is based on 8MHz HSE. APB1 and APB2 clock is from Main clock.


After running this example, LED1 and LED2 will light up, and when connected to the computer via USB, you will see a mouse device added in the device manager.
When you press the KEY1 button, the mouse will move to the right; the KEY2 button represents the left mouse button.
When the computer sleeps, the USB will enter the Suspend state and LED2 will go out; when the computer is woken up, the USB will resume and LED2 will light up.
In addition, pressing the KEY2 button in the Suspend state can remotely wake up the computer.


@par Directory contents 

  - USB/USB_HID_Mouse/USB_HID_Mouse.uvprojx     MDK5 project file
  - USB/USB_HID_Mouse/USB_HID_Mouse.uvoptx      MDK5 project options file
  - USB/USB_HID_Mouse/JLinkSettings.ini         Jlink settings file
  - USB/USB_HID_Mouse/main.c                    Main program
  - USB/USB_HID_Mouse/usb_hid_def.h             HID class defines
  - USB/USB_HID_Mouse/usbd_hid.c                HID class request handler
  - USB/USB_HID_Mouse/usbd_hid.h                HID class request handler header file
  - USB/USB_HID_Mouse/usbd_user.c               USBD user application
  - USB/USB_HID_Mouse/usbd_user.h               USBD user application header file
  - USB/USB_HID_Mouse/usbd_user_hid.c           USBD user HID class request handler
  - USB/USB_HID_Mouse/usbd_user_hid.h           USBD user HID class request handler header file
  - USB/USB_HID_Mouse/usbd_user_desc.c          USBD descriptors


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open USB_HID_Mouse.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

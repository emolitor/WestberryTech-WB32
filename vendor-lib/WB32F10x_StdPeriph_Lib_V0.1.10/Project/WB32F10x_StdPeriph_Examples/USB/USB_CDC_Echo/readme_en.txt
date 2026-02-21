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

@par Example Description 

This example describes how to use USB module to implement a Virtual COM Port.

In this example, Main clock is from PLL clock and it is configured at 96MHz. The PLL clock is based on 8MHz HSE. APB1 and APB2 clock is from Main clock.

After running this example, LED1 and LED2 will light up, and when connecting to the computer via USB, you will see that a serial port has been added in the device manager.
Sending data through the serial port debugging tool will loop back to the PC.
When the computer sleeps, the USB will enter the Suspend state and LED2 will go out; when the computer is woken up, the USB will resume and LED2 will light up.

In the Windows 10 operating system, when the USB serial device is connected to the computer, the serial driver will be installed automatically.

In the Windows 7 operating system, you need to manually install the serial port driver. In this example, the necessary files (USB_Serial.inf and USB_Serial.cat) for installing the serial port driver are provided.
When the USB serial device is connected to the computer, in the device manager of the operating system, select to update the driver of the USB serial device and locate the location of the serial driver file to install.


@par Directory contents 

  - USB/USB_CDC_Echo/USB_CDC_Echo.uvprojx     MDK5 project file
  - USB/USB_CDC_Echo/USB_CDC_Echo.uvoptx      MDK5 project options file
  - USB/USB_CDC_Echo/JLinkSettings.ini        Jlink settings file
  - USB/USB_CDC_Echo/main.c                   Main program
  - USB/USB_CDC_Echo/usb_cdc_def.h            CDC class defines
  - USB/USB_CDC_Echo/usbd_cdc.c               CDC class request handler
  - USB/USB_CDC_Echo/usbd_cdc.h               CDC class request handler header file
  - USB/USB_CDC_Echo/usbd_user.c              USBD user application
  - USB/USB_CDC_Echo/usbd_user.h              USBD user application header file
  - USB/USB_CDC_Echo/usbd_user_cdc.c          USBD user CDC class request handler
  - USB/USB_CDC_Echo/usbd_user_cdc.h          USBD user CDC class request handler header file
  - USB/USB_CDC_Echo/usbd_user_desc.c         USBD descriptors
  - USB/USB_CDC_Echo/USB_Serial.cat           Windows serial port driver file
  - USB/USB_CDC_Echo/USB_Serial.inf           Windows serial port driver file


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open USB_CDC_Echo.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

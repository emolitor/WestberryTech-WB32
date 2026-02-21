/**
  @page USB_Mass_Storage_SPI_FLASH USB_Mass_Storage_SPI_FLASH example
  
  @verbatim
  *************** (C) COPYRIGHT 2020 - 2023 Westberry Technology ***************
  * @file    USB/USB_Mass_Storage_SPI_FLASH/readme.txt 
  * @author  Westberry Application Team
  * @version V0.1.10
  * @date    05-January-2023
  * @brief   Description of the USB USB_Mass_Storage_SPI_FLASH example.
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

This example describes how to use USB module to implement a USB flash disk (SPI FLASH as storage).

In this example, Main clock is from PLL clock and it is configured at 96MHz. The PLL clock is based on 8MHz HSE. APB1 and APB2 clock is from Main clock.

After running this example, LED1 and LED2 will light up, and when you connect to your computer via USB, you will see that a new USB disk device has appeared in the Explorer.
When the computer sleeps, the USB will enter the Suspend state and LED2 will go out; when the computer is woken up, the USB will resume and LED2 will light up.


@par Directory contents 

  - USB/USB_Mass_Storage_SPI_FLASH/USB_Mass_Storage_SPI_FLASH.uvprojx     MDK5 project file
  - USB/USB_Mass_Storage_SPI_FLASH/USB_Mass_Storage_SPI_FLASH.uvoptx      MDK5 project options file
  - USB/USB_Mass_Storage_SPI_FLASH/JLinkSettings.ini                      Jlink settings file
  - USB/USB_Mass_Storage_SPI_FLASH/main.c                                 Main program
  - USB/USB_Mass_Storage_SPI_FLASH/usb_msc_def.h                          MSC class defines
  - USB/USB_Mass_Storage_SPI_FLASH/usbd_msc.c                             MSC class request handler
  - USB/USB_Mass_Storage_SPI_FLASH/usbd_msc.h                             MSC class request handler header file
  - USB/USB_Mass_Storage_SPI_FLASH/usbd_msc_scsi.c                        MSC class request SCSI command handler
  - USB/USB_Mass_Storage_SPI_FLASH/usbd_msc_scsi.h                        MSC class request SCSI command handler header file
  - USB/USB_Mass_Storage_SPI_FLASH/usbd_user.c                            USBD user application
  - USB/USB_Mass_Storage_SPI_FLASH/usbd_user.h                            USBD user application header file
  - USB/USB_Mass_Storage_SPI_FLASH/usbd_user_desc.c                       USBD descriptors
  - USB/USB_Mass_Storage_SPI_FLASH/usbd_user_msc.c                        USBD user MSC class request handler
  - USB/USB_Mass_Storage_SPI_FLASH/usbd_user_msc.h                        USBD user MSC class request handler header file
  - USB/USB_Mass_Storage_SPI_FLASH/drv_spi_flash.c                        SPI Flash driver program
  - USB/USB_Mass_Storage_SPI_FLASH/drv_spi_flash.h                        SPI Flash driver program header


@par Hardware and Software environment 

  - Hardware environment
    - HSE crystal is 8MHz


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open USB_Mass_Storage_SPI_FLASH.uvprojx from this example folder by Keil MDK5
 - Rebuild all files and load your image into target memory
 - Run the example 
   
 * <h3><center>&copy; COPYRIGHT 2020 - 2023 Westberry Technology</center></h3>
 */

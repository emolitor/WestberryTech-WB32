#include "wb32f10x.h"
#include <stdlib.h>
#include "usb_def.h"
#include "usbd_user_msc.h"


#define WBVAL(x)                          (x & 0xFF),((x >> 8) & 0xFF)

const uint8_t USBD_DeviceDescriptor[] = 
{
  0x12,                                 /* bLength */
  USB_DESC_TYPE_DEVICE,                 /* bDescriptorType */
  WBVAL(0x0200), /* 2.00 */             /* bcdUSB */

  /* This is an Interface Class Defined Device */
  0x00,                                 /* bDeviceClass */
  0x00,                                 /* bDeviceSubClass */
  0x00,                                 /* bDeviceProtocol */

  0x40,                                 /* bMaxPacketSize0 0x40 = 64 */
  WBVAL(0x8888),                        /* idVendor */
  WBVAL(0x2003),                        /* idProduct */
  WBVAL(0x0100),                        /* bcdDevice */
  1,                                    /* iManufacturer */
  2,                                    /* iProduct */
  3,                                    /* iSerialNumber */
  0x01                                  /* bNumConfigurations: one possible configuration*/
};


const uint8_t USBD_ConfigDescriptor[32] =
{
  /* Configuration 1 */
  0x09,                                 /* bLength */
  USB_DESC_TYPE_CONFIGURATION,          /* bDescriptorType */
  WBVAL(32),                            /* wTotalLength */
  0x01,                                 /* bNumInterfaces: 1 interface */
  0x01,                                 /* bConfigurationValue: 0x01 is used to select this configuration */
  0,                                    /* iConfiguration: no string to describe this configuration */
  USB_CONFIG_BUS_POWERED,               /* bmAttributes */
  USB_CONFIG_POWER_MA(100),             /* bMaxPower, device power consumption  100mA */

  /************* Interface Descriptor **********/
  0x09,                                 /* bLength */
  USB_DESC_TYPE_INTERFACE,              /* bDescriptorType */
  USBD_USER_MSC_IF_NUM,                 /* bInterfaceNumber */
  0x00,                                 /* bAlternateSetting */
  0x02,                                 /* bNumEndpoints */
  0x08,                                 /* bInterfaceClass: Mass Storage Class */
  0x06,                                 /* bInterfaceSubClass: SCSI transparent */
  0x50,                                 /* bInterfaceProtocol: Bulk-Only Transport */
  0,                                    /* iInterface */

  /************* Endpoint Descriptor ***********/
  0x07,                                 /* bLength */
  USB_DESC_TYPE_ENDPOINT,               /* bDescriptorType */
  USB_ENDPOINT_IN(0x01),                /* bEndpointAddress */
  USB_ENDPOINT_TYPE_BULK,               /* bmAttributes */
  WBVAL(0x0040),                        /* wMaxPacketSize 0x40 = 64 */
  0x00,                                 /* bInterval */

  /************* Endpoint Descriptor ***********/
  0x07,                                 /* bLength */
  USB_DESC_TYPE_ENDPOINT,               /* bDescriptorType */
  USB_ENDPOINT_OUT(0x01),               /* bEndpointAddress */
  USB_ENDPOINT_TYPE_BULK,               /* bmAttributes */
  WBVAL(0x0040),                        /* wMaxPacketSize 0x40 = 64 */
  0x00,                                 /* bInterval */

};

const uint8_t USBD_StringDescriptor_LangID[4] =
{
  0x04,
  USB_DESC_TYPE_STRING,
  0x09,
  0x04
}; /* LangID = 0x0409: U.S. English */

const uint8_t USBD_StringDescriptor_Manufacturer[26] = 
{
  26,                                                 /* bLength */
  USB_DESC_TYPE_STRING,                               /* bDescriptorType */
  /* Manufacturer: "Your Company" */
  'Y', 0, 'o', 0, 'u', 0, 'r', 0, ' ', 0, 'C', 0, 'o', 0, 'm', 0,
  'p', 0, 'a', 0, 'n', 0, 'y', 0
};

const uint8_t USBD_StringDescriptor_Product[34] = 
{
  34,                                                 /* bLength */
  USB_DESC_TYPE_STRING,                               /* bDescriptorType */
  /* Product: "USB Mass Storage" */
  'U', 0, 'S', 0, 'B', 0, ' ', 0, 'M', 0, 'a', 0, 's', 0, 's', 0,
  ' ', 0, 'S', 0, 't', 0, 'o', 0, 'r', 0, 'a', 0, 'g', 0, 'e', 0
};

const uint8_t USBD_StringDescriptor_SerialNumber[18] =
{
  18,                                                 /* bLength */
  USB_DESC_TYPE_STRING,                               /* bDescriptorType */
  /* SerialNumber: "00000001" */
  '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '1', 0
};

const uint8_t *USBD_StringDescriptors[] = 
{
  USBD_StringDescriptor_LangID,
  USBD_StringDescriptor_Manufacturer,
  USBD_StringDescriptor_Product,
  USBD_StringDescriptor_SerialNumber,
};

/**
 * @brief  Returns the device descriptor.
 * @param  length: Pointer to data length variable.
 * @return Pointer to the device descriptor buffer.
 */
uint8_t* USBD_User_GetDeviceDescriptor(uint16_t* length)
{
  *length = sizeof(USBD_DeviceDescriptor);
  return ((uint8_t*)USBD_DeviceDescriptor);
}

/**
 * @brief  Returns the specified configuration descriptor.
 * @param  index: specifies the index of configuration descriptor.
 * @param  length: Pointer to data length variable.
 * @return Pointer to the specified configuration descriptor buffer.
 */
uint8_t* USBD_User_GetConfigDescriptor(uint8_t index, uint16_t* length)
{
  if (index < 1)
  {
    *length = (USBD_ConfigDescriptor[3] << 8) | USBD_ConfigDescriptor[2];
    return ((uint8_t*)USBD_ConfigDescriptor);
  }

  return NULL;
}

/**
 * @brief  Returns the specified string descriptor.
 * @param  index: specifies the index of string descriptor.
 * @param  length: Pointer to data length variable.
 * @return Pointer to the specified string descriptor buffer.
 */
uint8_t* USBD_User_GetStringDescriptor(uint8_t index, uint16_t* length)
{
  if (index < (sizeof(USBD_StringDescriptors) / sizeof(USBD_StringDescriptors[0])))
  {
    *length = USBD_StringDescriptors[index][0];
    return ((uint8_t*)USBD_StringDescriptors[index]);
  }

  return NULL;
}

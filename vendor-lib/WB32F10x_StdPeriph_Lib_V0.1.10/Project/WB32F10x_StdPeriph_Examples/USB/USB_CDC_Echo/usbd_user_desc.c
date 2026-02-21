#include "wb32f10x.h"
#include <stdlib.h>
#include "usb_def.h"
#include "usb_cdc_def.h"
#include "usbd_user_cdc.h"


#define WBVAL(x)                          (x & 0xFF),((x >> 8) & 0xFF)

const uint8_t USBD_DeviceDescriptor[] = 
{
  0x12,                                 /* bLength */
  USB_DESC_TYPE_DEVICE,                 /* bDescriptorType */
  WBVAL(0x0110), /* 1.10 */             /* bcdUSB */

  0x02,                                 /* bDeviceClass: Communication device class */
  0x00,                                 /* bDeviceSubClass */
  0x00,                                 /* bDeviceProtocol */

  0x40,                                 /* bMaxPacketSize0 0x40 = 64 */
  WBVAL(0x8888),                        /* idVendor */
  WBVAL(0x2001),                        /* idProduct */
  WBVAL(0x0100),                        /* bcdDevice */
  1,                                    /* iManufacturer */
  2,                                    /* iProduct */
  3,                                    /* iSerialNumber */
  0x01                                  /* bNumConfigurations: one possible configuration*/
};


const uint8_t USBD_ConfigDescriptor[67] =
{
  /* Configuration 1 */
  0x09,                                 /* bLength */
  USB_DESC_TYPE_CONFIGURATION,          /* bDescriptorType */
  WBVAL(67),                            /* wTotalLength */
  0x02,                                 /* bNumInterfaces: 2 interface */
  0x01,                                 /* bConfigurationValue: 0x01 is used to select this configuration */
  0,                                    /* iConfiguration: no string to describe this configuration */
  USB_CONFIG_SELF_POWERED,              /* bmAttributes */
  USB_CONFIG_POWER_MA(100),             /* bMaxPower, device power consumption  100mA */

  /************* Interface Descriptor **********/
  0x09,                                 /* bLength */
  USB_DESC_TYPE_INTERFACE,              /* bDescriptorType */
  USBD_CDC_ACM_CIF_NUM,                 /* bInterfaceNumber */
  0x00,                                 /* bAlternateSetting */
  0x01,                                 /* bNumEndpoints */
  0x02,                                 /* bInterfaceClass: Communications Interface Class */
  0x02,                                 /* bInterfaceSubClass: Abstract Control Model */
  0x01,                                 /* bInterfaceProtocol: AT Commands: V.250 etc */
  0,                                    /* iInterface */

  /************* Header Functional Descriptor **********/
  0x05,                                 /* bFunctionLength */
  USB_DESC_TYPE_CDC_CS_INTERFACE,       /* bDescriptorType */
  0x00,                                 /* bDescriptorSubtype */
  WBVAL(0x0110),                        /* bcdCDC */

  /************* Abstract Control Management Functional Descriptor **********/
  0x04,                                 /* bFunctionLength */
  USB_DESC_TYPE_CDC_CS_INTERFACE,       /* bDescriptorType */
  0x02,                                 /* bDescriptorSubtype */
  0x06,                                 /* bmCapabilities */

  /************* Union Functional Descriptor **********/
  0x05,                                 /* bFunctionLength */
  USB_DESC_TYPE_CDC_CS_INTERFACE,       /* bDescriptorType */
  0x06,                                 /* bDescriptorSubtype */
  USBD_CDC_ACM_CIF_NUM,                 /* bControlInterface */
  USBD_CDC_ACM_DIF_NUM,                 /* bSubordinateInterface0 */

  /************* Call Management Functional Descriptor **********/
  0x05,                                 /* bFunctionLength */
  USB_DESC_TYPE_CDC_CS_INTERFACE,       /* bDescriptorType */
  0x01,                                 /* bDescriptorSubtype */
  0x00,                                 /* bmCapabilities */
  USBD_CDC_ACM_DIF_NUM,                 /* bDataInterface */

  /************* Endpoint Descriptor ***********/
  0x07,                                 /* bLength */
  USB_DESC_TYPE_ENDPOINT,               /* bDescriptorType */
  USB_ENDPOINT_IN(0x01),                /* bEndpointAddress */
  USB_ENDPOINT_TYPE_INTERRUPT,          /* bmAttributes */
  WBVAL(0x0010),                        /* wMaxPacketSize 0x10 = 16 */
  0x08,                                 /* bInterval: 8ms */

  /************* Interface Descriptor **********/
  0x09,                                 /* bLength */
  USB_DESC_TYPE_INTERFACE,              /* bDescriptorType */
  USBD_CDC_ACM_DIF_NUM,                 /* bInterfaceNumber */
  0x00,                                 /* bAlternateSetting */
  0x02,                                 /* bNumEndpoints */
  0x0A,                                 /* bInterfaceClass: Data Interface Class */
  0x00,                                 /* bInterfaceSubClass */
  0x00,                                 /* bInterfaceProtocol */
  0,                                    /* iInterface */

  /************* Endpoint Descriptor ***********/
  0x07,                                 /* bLength */
  USB_DESC_TYPE_ENDPOINT,               /* bDescriptorType */
  USB_ENDPOINT_IN(0x02),                /* bEndpointAddress */
  USB_ENDPOINT_TYPE_BULK,               /* bmAttributes */
  WBVAL(0x0040),                        /* wMaxPacketSize 0x40 = 64 */
  0x00,                                 /* bInterval */

  /************* Endpoint Descriptor ***********/
  0x07,                                 /* bLength */
  USB_DESC_TYPE_ENDPOINT,               /* bDescriptorType */
  USB_ENDPOINT_OUT(0x02),               /* bEndpointAddress */
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

const uint8_t USBD_StringDescriptor_Product[22] = 
{
  22,                                                 /* bLength */
  USB_DESC_TYPE_STRING,                               /* bDescriptorType */
  /* Product: "USB Serial" */
  'U', 0, 'S', 0, 'B', 0, ' ', 0, 'S', 0, 'e', 0, 'r', 0, 'i', 0,
  'a', 0, 'l', 0
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

#include "wb32f10x.h"
#include <stdlib.h>
#include "usb_def.h"
#include "usb_hid_def.h"
#include "usbd_user_hid.h"


#define WBVAL(x)                          (x & 0xFF),((x >> 8) & 0xFF)

/*
  Mouse Input Report Format:
    [0] b0 Left Mouse Button
        b1 Right Mouse Button
        b2 Middle Mouse Button
    [1] X-Axis
    [2] Y-Axis
    [3] Wheel
*/
const uint8_t USBD_MouseHIDReportDescriptor[] = 
{
  0x05, 0x01,       /* USAGE_PAGE (Generic Desktop) */
  0x09, 0x02,       /* USAGE (Mouse) */
  0xA1, 0x01,       /* COLLECTION (Application) */
  0x09, 0x01,       /* USAGE (Pointer) */
  0xA1, 0x00,       /* COLLECTION (Physical) */
  0x05, 0x09,       /* USAGE_PAGE (Button) */
  0x19, 0x01,       /* USAGE_MINIMUM (Button 1) */
  0x29, 0x03,       /* USAGE_MAXIMUM (Button 3) */
  0x15, 0x00,       /* LOGICAL_MINIMUM (0) */
  0x25, 0x01,       /* LOGICAL_MAXIMUM (1) */
  0x95, 0x03,       /* REPORT_COUNT (3) */
  0x75, 0x01,       /* REPORT_SIZE (1) */
  0x81, 0x02,       /* INPUT (Data,Var,Abs) */
  0x95, 0x01,       /* REPORT_COUNT (1) */
  0x75, 0x05,       /* REPORT_SIZE (5) */
  0x81, 0x03,       /* INPUT (Cnst,Var,Abs) */
  0x05, 0x01,       /* USAGE_PAGE (Generic Desktop) */
  0x09, 0x30,       /* USAGE (X) */
  0x09, 0x31,       /* USAGE (Y) */
  0x09, 0x38,       /* USAGE (Wheel) */
  0x15, 0x81,       /* LOGICAL_MINIMUM (-127) */
  0x25, 0x7F,       /* LOGICAL_MAXIMUM (127) */
  0x75, 0x08,       /* REPORT_SIZE (8) */
  0x95, 0x03,       /* REPORT_COUNT (3) */
  0x81, 0x06,       /* INPUT (Data,Var,Rel) */
  0xC0,             /* END_COLLECTION */
  0xC0,             /* END_COLLECTION */
};

const uint8_t USBD_DeviceDescriptor[] = 
{
  0x12,                                 /* bLength */
  USB_DESC_TYPE_DEVICE,                 /* bDescriptorType */
  WBVAL(0x0110), /* 1.10 */             /* bcdUSB */

  /* This is an Interface Class Defined Device */
  0x00,                                 /* bDeviceClass */
  0x00,                                 /* bDeviceSubClass */
  0x00,                                 /* bDeviceProtocol */

  0x40,                                 /* bMaxPacketSize0 0x40 = 64 */
  WBVAL(0x8888),                        /* idVendor */
  WBVAL(0x2002),                        /* idProduct */
  WBVAL(0x0100),                        /* bcdDevice */
  1,                                    /* iManufacturer */
  2,                                    /* iProduct */
  3,                                    /* iSerialNumber */
  0x01                                  /* bNumConfigurations: one possible configuration*/
};


const uint8_t USBD_ConfigDescriptor[34] =
{
  /* Configuration 1 */
  0x09,                                 /* bLength */
  USB_DESC_TYPE_CONFIGURATION,          /* bDescriptorType */
  WBVAL(34),                            /* wTotalLength */
  0x01,                                 /* bNumInterfaces: 1 interface */
  0x01,                                 /* bConfigurationValue: 0x01 is used to select this configuration */
  0,                                    /* iConfiguration: no string to describe this configuration */
  USB_CONFIG_BUS_POWERED|USB_CONFIG_REMOTE_WAKEUP,               /* bmAttributes */
  USB_CONFIG_POWER_MA(100),             /* bMaxPower, device power consumption  100mA */

  /* offset: 9 */
  /************* Interface Descriptor **********/
  0x09,                                 /* bLength */
  USB_DESC_TYPE_INTERFACE,              /* bDescriptorType */
  USBD_HID_IF_NUM,                      /* bInterfaceNumber */
  0x00,                                 /* bAlternateSetting */
  0x01,                                 /* bNumEndpoints */
  0x03,                                 /* bInterfaceClass: HID */
  0x01,                                 /* bInterfaceSubClass: Boot Interface */
  0x02,                                 /* bInterfaceProtocol: Mouse */
  0,                                    /* iInterface */

  /* offset: 18 */
  /************* HID Descriptor **********/
  0x09,                                 /* bLength */
  HID_HID_DESCRIPTOR_TYPE,              /* bDescriptorType */
  WBVAL(0x0110),                        /* bcdHID */
  0x00,                                 /* bCountryCode */
  0x01,                                 /* bNumDescriptors */
  HID_REPORT_DESCRIPTOR_TYPE,           /* bDescriptorType */
  WBVAL(sizeof(USBD_MouseHIDReportDescriptor)),   /* wDescriptorLength */

  /* offset: 27 */
  /************* Endpoint Descriptor ***********/
  0x07,                                 /* bLength */
  USB_DESC_TYPE_ENDPOINT,               /* bDescriptorType */
  USB_ENDPOINT_IN(0x01),                /* bEndpointAddress */
  USB_ENDPOINT_TYPE_INTERRUPT,          /* bmAttributes */
  WBVAL(0x0040),                        /* wMaxPacketSize 0x40 = 64 */
  0x0A,                                 /* bInterval */
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

const uint8_t USBD_StringDescriptor_Product[20] = 
{
  20,                                                 /* bLength */
  USB_DESC_TYPE_STRING,                               /* bDescriptorType */
  /* Product: "USB Mouse" */
  'U', 0, 'S', 0, 'B', 0, ' ', 0, 'M', 0, 'o', 0, 'u', 0, 's', 0,
  'e', 0
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

uint8_t* USBD_User_GetHIDDescriptor(uint8_t desc_type, uint16_t* length)
{
  switch (desc_type)
  {
    case HID_HID_DESCRIPTOR_TYPE:
      *length = 9;
      return (((uint8_t*)USBD_ConfigDescriptor) + 18);
      // break;
    case HID_REPORT_DESCRIPTOR_TYPE:
      *length = sizeof(USBD_MouseHIDReportDescriptor);
      return ((uint8_t*)USBD_MouseHIDReportDescriptor);
      // break;
  }

  return NULL;
}

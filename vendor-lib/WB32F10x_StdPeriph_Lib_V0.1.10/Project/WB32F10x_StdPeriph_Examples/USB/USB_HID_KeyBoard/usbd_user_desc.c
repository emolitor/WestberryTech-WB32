#include "wb32f10x.h"
#include <stdlib.h>
#include "usb_def.h"
#include "usb_hid_def.h"
#include "usbd_user_hid.h"


#define WBVAL(x)                          (x & 0xFF),((x >> 8) & 0xFF)

/*
  Keyboard Input Report Format:
  [0] b0 Keyboard LeftControl
      b1 Keyboard LeftShift
      b2 Keyboard LeftAlt
      b3 Keyboard Left GUI
      b4 Keyboard RightControl
      b5 Keyboard RightShift
      b6 Keyboard RightAlt
      b7 Keyboard Right GUI
  [1] Unknown
  [2]~[7] 普通键键值数组。
      当没有键按下时，全部6字节值都为0。
      当只有一个普通键按下时，这6个字节中的第一字节值即为该按键的键值（具体的键值请看HID的用途表文档），
      当有多个普通键同时按下时，则同时返回这些键的键值。
      如果按下的键太多，则这6个字节都为0xFF（不能返回0x00，这样会让操作系统认为所有键都已经释放）。
      至于键值在数组中的先后顺序是无所谓的，操作系统会负责检查是否有新键按下。

  LEDs Output Report Format:
  [0] b0 LED Num Lock
      b1 LED Caps Lock
      b2 LED Scroll Lock
      b3 LED Compose
      b4 LED Kana
*/
const uint8_t USBD_KeyboardHIDReportDescriptor[] = 
{
  0x05, 0x01,           // Usage Page (Generic Desktop)
  0x09, 0x06,           // Usage (Keyboard)
  0xA1, 0x01,           // Collection (Application)
  0x05, 0x07,           //     Usage Page (Keyboard/Keypad)
  0x19, 0xE0,           //     Usage Minimum (Keyboard Left Control)
  0x29, 0xE7,           //     Usage Maximum (Keyboard Right GUI)
  0x15, 0x00,           //     Logical Minimum (0)
  0x25, 0x01,           //     Logical Maximum (1)
  0x75, 0x01,           //     Report Size (1)
  0x95, 0x08,           //     Report Count (8)
  0x81, 0x02,           //     Input (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
  0x75, 0x08,           //     Report Size (8)
  0x95, 0x01,           //     Report Count (1)
  0x81, 0x01,           //     Input (Cnst,Ary,Abs)
  0x19, 0x00,           //     Usage Minimum (Undefined)
  0x2A, 0xFF, 0x00,     //     Usage Maximum
  0x15, 0x00,           //     Logical Minimum (0)
  0x26, 0xFF, 0x00,     //     Logical Maximum (255)
  0x75, 0x08,           //     Report Size (8)
  0x95, 0x06,           //     Report Count (6)
  0x81, 0x00,           //     Input (Data,Ary,Abs)
  0x05, 0x08,           //     Usage Page (LEDs)
  0x19, 0x01,           //     Usage Minimum (Num Lock)
  0x29, 0x05,           //     Usage Maximum (Kana)
  0x15, 0x00,           //     Logical Minimum (0)
  0x25, 0x01,           //     Logical Maximum (1)
  0x75, 0x01,           //     Report Size (1)
  0x95, 0x05,           //     Report Count (5)
  0x91, 0x02,           //     Output (Data,Var,Abs,NWrp,Lin,Pref,NNul,NVol,Bit)
  0x95, 0x03,           //     Report Count (3)
  0x91, 0x01,           //     Output (Cnst,Ary,Abs,NWrp,Lin,Pref,NNul,NVol,Bit)
  0xC0,                 // End Collection
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
  WBVAL(0x2012),                        /* idProduct */
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
  0x01,                                 /* bInterfaceProtocol: Keyboard */
  0,                                    /* iInterface */

  /* offset: 18 */
  /************* HID Descriptor **********/
  0x09,                                 /* bLength */
  HID_HID_DESCRIPTOR_TYPE,              /* bDescriptorType */
  WBVAL(0x0110),                        /* bcdHID */
  0x00,                                 /* bCountryCode */
  0x01,                                 /* bNumDescriptors */
  HID_REPORT_DESCRIPTOR_TYPE,           /* bDescriptorType */
  WBVAL(sizeof(USBD_KeyboardHIDReportDescriptor)),   /* wDescriptorLength */

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

const uint8_t USBD_StringDescriptor_Product[26] = 
{
  26,                                                 /* bLength */
  USB_DESC_TYPE_STRING,                               /* bDescriptorType */
  /* Product: "USB Keyboard" */
  'U', 0, 'S', 0, 'B', 0, ' ', 0, 'K', 0, 'e', 0, 'y', 0, 'b', 0,
  'o', 0, 'a', 0, 'r', 0, 'd', 0
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
      *length = sizeof(USBD_KeyboardHIDReportDescriptor);
      return ((uint8_t*)USBD_KeyboardHIDReportDescriptor);
      // break;
  }

  return NULL;
}

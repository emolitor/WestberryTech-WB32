/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_CDC_DEF_H
#define __USB_CDC_DEF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef __PACKED_STRUCT _USB_CDC_LINE_CODING {
  uint32_t dwDTERate;
  uint8_t  bCharFormat;
  uint8_t  bParityType;
  uint8_t  bDataBits;
} USB_CDC_LINE_CODING;

/* Exported constants --------------------------------------------------------*/
#define USB_DESC_TYPE_CDC_CS_INTERFACE        0x24U
#define USB_DESC_TYPE_CDC_CS_ENDPOINT         0x25U

#define USB_CDC_REQUEST_SEND_ENCAPSULATED_COMMAND           0x00
#define USB_CDC_REQUEST_GET_ENCAPSULATED_RESPONSE           0x01
#define USB_CDC_REQUEST_SET_COMM_FEATURE                    0x02
#define USB_CDC_REQUEST_GET_COMM_FEATURE                    0x03
#define USB_CDC_REQUEST_CLEAR_COMM_FEATURE                  0x04
#define USB_CDC_REQUEST_SET_AUX_LINE_STATE                  0x10
#define USB_CDC_REQUEST_SET_HOOK_STATE                      0x11
#define USB_CDC_REQUEST_PULSE_SETUP                         0x12
#define USB_CDC_REQUEST_SEND_PULSE                          0x13
#define USB_CDC_REQUEST_SET_PULSE_TIME                      0x14
#define USB_CDC_REQUEST_RING_AUX_JACK                       0x15
#define USB_CDC_REQUEST_SET_LINE_CODING                     0x20
#define USB_CDC_REQUEST_GET_LINE_CODING                     0x21
#define USB_CDC_REQUEST_SET_CONTROL_LINE_STATE              0x22
#define USB_CDC_REQUEST_SEND_BREAK                          0x23
#define USB_CDC_REQUEST_SET_RINGER_PARMS                    0x30
#define USB_CDC_REQUEST_GET_RINGER_PARMS                    0x31
#define USB_CDC_REQUEST_SET_OPERATION_PARMS                 0x32
#define USB_CDC_REQUEST_GET_OPERATION_PARMS                 0x33
#define USB_CDC_REQUEST_SET_LINE_PARMS                      0x34
#define USB_CDC_REQUEST_GET_LINE_PARMS                      0x35
#define USB_CDC_REQUEST_DIAL_DIGITS                         0x36
#define USB_CDC_REQUEST_SET_UNIT_PARAMETER                  0x37
#define USB_CDC_REQUEST_GET_UNIT_PARAMETER                  0x38
#define USB_CDC_REQUEST_CLEAR_UNIT_PARAMETER                0x39
#define USB_CDC_REQUEST_GET_PROFILE                         0x3A
#define USB_CDC_REQUEST_SET_ETHERNET_MULTICAST_FILTERS      0x40
#define USB_CDC_REQUEST_SET_ETHERNET_PMP_FILTER             0x41
#define USB_CDC_REQUEST_GET_ETHERNET_PMP_FILTER             0x42
#define USB_CDC_REQUEST_SET_ETHERNET_PACKET_FILTER          0x43
#define USB_CDC_REQUEST_GET_ETHERNET_STATISTIC              0x44
#define USB_CDC_REQUEST_SET_ATM_DATA_FORMAT                 0x50
#define USB_CDC_REQUEST_GET_ATM_DEVICE_STATISTICS           0x51
#define USB_CDC_REQUEST_SET_ATM_DEFAULT_VC                  0x52
#define USB_CDC_REQUEST_GET_ATM_VC_STATISTICS               0x53

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __USB_CDC_DEF_H */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_MSC_DEF_H
#define __USB_MSC_DEF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Bulk-only Command Block Wrapper */
typedef __PACKED_STRUCT _USB_MSC_CBW
{
  uint32_t dSignature;
  uint32_t dTag;
  uint32_t dDataLength;
  uint8_t  bmFlags;
  uint8_t  bLUN;
  uint8_t  bCBLength;
  uint8_t  CB[16];
} USB_MSC_CBW;

/* Bulk-only Command Status Wrapper */
typedef __PACKED_STRUCT _USB_MSC_CSW
{
  uint32_t dSignature;
  uint32_t dTag;
  uint32_t dDataResidue;
  uint8_t  bStatus;
} USB_MSC_CSW;

/* Exported constants --------------------------------------------------------*/

/* MSC Subclass Codes */
#define MSC_SUBCLASS_SCSI                 0x06

/* MSC Protocol Codes */
#define MSC_PROTOCOL_BULK_ONLY            0x50

/* MSC class-specific request */
#define MSC_REQUEST_MASS_STORAGE_RESET    0xFF
#define MSC_REQUEST_GET_MAX_LUN           0xFE


#define MSC_CBW_SIGNATURE             0x43425355
#define MSC_CSW_SIGNATURE             0x53425355

/* CSW Status Definitions */
#define CSW_CMD_PASSED                0x00
#define CSW_CMD_FAILED                0x01
#define CSW_PHASE_ERROR               0x02



/* SCSI Commands */
#define SCSI_TEST_UNIT_READY                0x00
#define SCSI_REQUEST_SENSE                  0x03
#define SCSI_FORMAT_UNIT                    0x04
#define SCSI_INQUIRY                        0x12
#define SCSI_MODE_SELECT6                   0x15
#define SCSI_MODE_SENSE6                    0x1A
#define SCSI_START_STOP_UNIT                0x1B
#define SCSI_SEND_DIAGNOSTIC                0x1D
#define SCSI_PREVENT_ALLOW_MEDIUM_REMOVAL   0x1E
#define SCSI_READ_FORMAT_CAPACITIES         0x23
#define SCSI_READ_CAPACITY10                0x25
#define SCSI_READ_CAPACITY                  0x25
#define SCSI_READ10                         0x28
#define SCSI_WRITE10                        0x2A
#define SCSI_VERIFY10                       0x2F
#define SCSI_SYNCHRONIZE_CACHE10            0x35
#define SCSI_READ12                         0xA8
#define SCSI_WRITE12                        0xAA
#define SCSI_MODE_SELECT10                  0x55
#define SCSI_MODE_SENSE10                   0x5A
#define SCSI_SYNCHRONIZE_CACHE16            0x91
#define SCSI_ATA_COMMAND_PASS_THROUGH12     0xA1
#define SCSI_ATA_COMMAND_PASS_THROUGH16     0x85
#define SCSI_SERVICE_ACTION_IN12            0xAB
#define SCSI_SERVICE_ACTION_IN16            0x9E
#define SCSI_SERVICE_ACTION_OUT12           0xA9
#define SCSI_SERVICE_ACTION_OUT16           0x9F


#define SCSI_SENSE_KEY_NO_SENSE                    0x00
#define SCSI_SENSE_KEY_RECOVERED_ERROR             0x01
#define SCSI_SENSE_KEY_NOT_READY                   0x02
#define SCSI_SENSE_KEY_MEDIUM_ERROR                0x03
#define SCSI_SENSE_KEY_HARDWARE_ERROR              0x04
#define SCSI_SENSE_KEY_ILLEGAL_REQUEST             0x05
#define SCSI_SENSE_KEY_UNIT_ATTENTION              0x06
#define SCSI_SENSE_KEY_DATA_PROTECT                0x07
#define SCSI_SENSE_KEY_BLANK_CHECK                 0x08
#define SCSI_SENSE_KEY_VENDOR_SPECIFIC             0x09
#define SCSI_SENSE_KEY_COPY_ABORTED                0x0A
#define SCSI_SENSE_KEY_ABORTED_COMMAND             0x0B
#define SCSI_SENSE_KEY_VOLUME_OVERFLOW             0x0D
#define SCSI_SENSE_KEY_MISCOMPARE                  0x0E

#define SCSI_ASC_NO_ADDITIONAL_SENSE_INFO          0x00
#define SCSI_ASC_INVALID_FIELD_IN_CDB              0x24


/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __USB_MSC_DEF_H */

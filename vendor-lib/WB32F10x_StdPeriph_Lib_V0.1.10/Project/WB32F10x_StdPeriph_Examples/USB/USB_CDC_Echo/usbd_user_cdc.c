#include "wb32f10x.h"
#include "usbd_user_cdc.h"

static USB_CDC_LINE_CODING cdc_acm_line_coding = { 9600, 0, 0, 8 };

/**
 * @brief  Handle SetLineCoding request of CDC class.
 * @return true - Success, false - Error
 */
bool USBD_User_CDC_SetLineCoding(const USB_CDC_LINE_CODING *line_coding)
{
  // Store requested settings to local variable
  cdc_acm_line_coding = *line_coding;
  return true;
}

/**
 * @brief  Handle GetLineCoding request of CDC class.
 * @return true - Success, false - Error
 */
bool USBD_User_CDC_GetLineCoding(USB_CDC_LINE_CODING *line_coding)
{
  *line_coding = cdc_acm_line_coding;
  return true;
}

/**
 * @brief  Handle SetControlLineState request of CDC class.
 * @return true - Success, false - Error
 */
bool USBD_User_CDC_SetControlLineState(uint16_t control_signal_bitmap)
{
  return true;
}

/**
 * @brief  Handle SendBreak request of CDC class.
 * @return true - Success, false - Error
 */
bool USBD_User_CDC_SendBreak(uint16_t duration_of_break)
{
  return true;
}

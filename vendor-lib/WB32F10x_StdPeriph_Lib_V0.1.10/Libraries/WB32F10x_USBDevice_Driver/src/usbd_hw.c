/**
 * @file    usbd_hw.c
 * @author  Westberry Application Team
 * @attention
 * 
 * Copyright (c) 2020 - 2023 Westberry Technology (ChangZhou) Corp., Ltd
 * All rights reserved.
 * 
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, WESTBERRY TECHNOLOGY SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 */

/* Includes ------------------------------------------------------------------*/
#include "usbd_hw.h"
#include "usb_def.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
 * @brief  Read data from the specified endpoint.
 * @param  ep_addr: specifies which endpoint read from.
 *         This value must be less than or equal to 3.
 * @param  pbuf: pointer to data buffer.
 * @param  size: length of data to be read.
 * @return None
 */
void USBD_HW_ReadEP(uint8_t ep_addr, uint8_t* pbuf, uint16_t size)
{
  uint32_t ep_fifo_addr = (uint32_t)&USB->FIFO[ep_addr & 0x0F];
  while(size)
  {
    *pbuf++ = *((volatile uint8_t*)ep_fifo_addr);
    size--;
  }
}

/**
 * @brief  Write data to the specified endpoint.
 * @param  ep_addr: specifies which endpoint to write.
 *         This value must be less than or equal to 3.
 * @param  pdata: pointer to the buffer containing the data to be written.
 * @param  len: length of data to be written.
 * @return None
 */
void USBD_HW_WriteEP(uint8_t ep_addr, const uint8_t* pdata, uint16_t len)
{
  uint32_t ep_fifo_addr = (uint32_t)&USB->FIFO[ep_addr & 0x0F];
  while (len)
  {
    *((volatile uint8_t*)ep_fifo_addr) = *pdata++;
    len--;
  }
}

/**
 * @brief  Transmits data over an IN endpoint.
 * @param  ep_addr: endpoint address (eg. 0x81).
 * @param  pdata: pointer to the buffer containing the data to be transmitted.
 * @param  len: length of data to be transmitted.
 * @return None
 */
void USBD_HW_Transmit(uint8_t ep_addr, const uint8_t* pdata, uint16_t len)
{
  uint8_t ep_num;
  USBD_HW_WriteEP(ep_addr, pdata, len);
  ep_num = ep_addr & 0x0F;
  if ((ep_num != 0) && (ep_addr & 0x80))
  {
    USB->INDEX = ep_num;
    USB->INCSR1 = USB_INCSR1_INPKTRDY;
  }
}

/**
 * @brief  Get the last received packet size of an OUT endpoint.
 * @param  ep_addr: endpoint address (eg. 0x01).
 * @return The last received packet size of an OUT endpoint.
 */
uint16_t USBD_HW_GetRxDataCount(uint8_t ep_addr)
{
  uint8_t ep_num;
  ep_num = ep_addr & 0x0F;
  if ((ep_num != 0) && (!(ep_addr & 0x80)))
  {
    USB->INDEX = ep_num;
    return ((USB->OUTCOUNTH << 8) | USB->OUTCOUNTL);
  }
  else
    return 0;
}

/**
 * @brief  Ready to receive from an OUT endpoint.
 * @param  ep_addr: endpoint address (eg. 0x01).
 * @return None
 */
void USBD_HW_ReadyToReceive(uint8_t ep_addr)
{
  uint8_t ep_num;
  ep_num = ep_addr & 0x0F;
  if ((ep_num != 0) && (!(ep_addr & 0x80)))
  {
    USB->INDEX = ep_num;
    USB->OUTCSR1 &= ~USB_OUTCSR1_OUTPKTRDY;
  }
}

/**
 * @brief  Set Stall condition of a non-control endpoint.
 * @param  ep_addr: endpoint address.
 * @return None
 */
void USBD_HW_SetStallEP(uint8_t ep_addr)
{
  uint8_t ep_num;
  ep_num = ep_addr & 0x0F;
  if (ep_num != 0)
  {
    USB->INDEX = ep_num;
    if (ep_addr & 0x80) {   // IN Endpoint
      USB->INCSR1 = USB_INCSR1_SENDSTALL;
    }
    else {                  // OUT Endpoint
      USB->OUTCSR1 = USB_OUTCSR1_SENDSTALL;
    }
  }
}

/**
 * @brief  Clear Stall condition of a non-control endpoint.
 * @param  ep_addr: endpoint address.
 * @return None
 */
void USBD_HW_ClrStallEP(uint8_t ep_addr)
{
  uint8_t ep_num;
  ep_num = ep_addr & 0x0F;
  if (ep_num != 0)
  {
    USB->INDEX = ep_num;
    if (ep_addr & 0x80) {   // IN Endpoint
      USB->INCSR1 = USB_INCSR1_CLRDATATOG;
    }
    else {                  // OUT Endpoint
      USB->OUTCSR1 = USB_OUTCSR1_CLRDATATOG;
    }
  }
}

/**
 * @brief  Get stall status of non-control endpoint.
 * @param  ep_addr: endpoint address.
 * @return stall status (1: stall, 0: not stall).
 */
uint8_t USBD_HW_IsStalled(uint8_t ep_addr)
{
  uint8_t ep_num;
  ep_num = ep_addr & 0x0F;
  if (ep_num != 0)
  {
    USB->INDEX = ep_num;
    if (ep_addr & 0x80) {   // IN Endpoint
      if (USB->INCSR1 & USB_INCSR1_SENDSTALL) {
        return 1;
      }
    }
    else {                  // OUT Endpoint
      if (USB->OUTCSR1 & USB_OUTCSR1_SENDSTALL) {
        return 1;
      }
    }
  }
  return 0;
}


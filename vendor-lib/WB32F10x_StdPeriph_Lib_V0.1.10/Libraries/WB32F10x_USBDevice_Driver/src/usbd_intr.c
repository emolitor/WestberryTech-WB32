/**
 * @file    usbd_intr.c
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
#include "wb32f10x.h"
#include <stdlib.h>
#include "usbd_core.h"
#include "usbd_user.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  EP0_STATE_IDLE,
  EP0_STATE_TX,
  EP0_STATE_RX,
  EP0_STATE_STALL,
} Endpoint0State_t;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static volatile Endpoint0State_t Endpoint0State;
/* Private function prototypes -----------------------------------------------*/
void USBD_EndPoint0(void);

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Handle USBD Interrupts.
 * @return None
 */
void Handle_USBD_INT(uint8_t IntrUSB, uint8_t IntrIn, uint8_t IntrOut)
{
  if (IntrUSB & USB_INTRUSB_SOFIS) {
    /* SOF interrupt */
    USBD_User_SOF();
  }

  if (IntrUSB & USB_INTRUSB_RSUIS) {
    /* Resume interrupt */
    USBD_Core_Resume();
  }

  if (IntrUSB & USB_INTRUSB_RSTIS) {
    /* Reset interrupt */
    Endpoint0State = EP0_STATE_IDLE;
    USBD_Core_Reset();
  }

  if (IntrIn & USB_INTRIN_EP0) {
    /* Endpoint 0 interrupt */
    USBD_EndPoint0();
  }

  if (IntrIn & USB_INTRIN_IN1) {
    USBD_User_EP1_IN();
  }

  if (IntrIn & USB_INTRIN_IN2) {
    USBD_User_EP2_IN();
  }

  if (IntrIn & USB_INTRIN_IN3) {
    USBD_User_EP3_IN();
  }

  if (IntrOut & USB_INTROUT_OUT1) {
    USBD_User_EP1_OUT();
  }

  if (IntrOut & USB_INTROUT_OUT2) {
    USBD_User_EP2_OUT();
  }

  if (IntrOut & USB_INTROUT_OUT3) {
    USBD_User_EP3_OUT();
  }

  if (IntrUSB & USB_INTRUSB_SUSIS) {
    /* Suspend interrupt */
    USBD_Core_Suspend();
  }
}

/**
 * @brief  Handle Endpoint 0 Event
 * @return None
 */
void USBD_EndPoint0(void)
{
  uint8_t ControlReg;

  USB->INDEX = 0;
  ControlReg = USB->CSR0;

  if (ControlReg & USB_CSR0_SENTSTALL)
  {
    USB->CSR0 = 0x00;
    Endpoint0State = EP0_STATE_IDLE;
  }

  if (ControlReg & USB_CSR0_SETUPEND)
  {
    USB->CSR0 = USB_CSR0_SVDSETUPEND;
    Endpoint0State = EP0_STATE_IDLE;
  }

  if (Endpoint0State == EP0_STATE_IDLE)
  {
    if (ControlReg & USB_CSR0_OUTPKTRDY)
    {
      UsbdControlStage NextStage = USBD_CONTROL_STAGE_STALL;

      USBD_HW_ReadEP(0x00, (uint8_t*)&UsbdCoreInfo.SetupPacket, 8);
      UsbdCoreInfo.DataPtr = NULL;
      UsbdCoreInfo.DataSize = UsbdCoreInfo.SetupPacket.wLength;
      UsbdCoreInfo.DataSent = 0;

      if ((UsbdCoreInfo.SetupPacket.bmRequestType & USB_REQUEST_TYPE_Msk) == USB_REQUEST_TYPE_STANDARD) {
        NextStage = USBD_EndPoint0_Setup_StdReq();
      }

      if (NextStage == USBD_CONTROL_STAGE_STALL) {
        NextStage = USBD_User_EndPoint0_Setup();
      }

      USB->INDEX = 0;
      switch (NextStage)
      {
        case USBD_CONTROL_STAGE_IN_DATA:
          USB->CSR0 = USB_CSR0_SVDOUTPKTRDY;
          Endpoint0State = EP0_STATE_TX;
          break;
        case USBD_CONTROL_STAGE_OUT_DATA:
          USB->CSR0 = USB_CSR0_SVDOUTPKTRDY;
          Endpoint0State = EP0_STATE_RX;
          break;
        case USBD_CONTROL_STAGE_STATUS:
          USB->CSR0 = USB_CSR0_SVDOUTPKTRDY | USB_CSR0_DATAEND;
          Endpoint0State = EP0_STATE_IDLE;
          break;
        default:
          USB->CSR0 = USB_CSR0_SVDOUTPKTRDY | USB_CSR0_SENDSTALL;
          Endpoint0State = EP0_STATE_STALL;
          break;
      }
    }
  }

  if (Endpoint0State == EP0_STATE_TX)
  {
    if (!(ControlReg & USB_CSR0_INPKTRDY))
    {
      ControlReg = USB->CSR0;

      if ((!(ControlReg & USB_CSR0_SETUPEND)) || (!(ControlReg & USB_CSR0_OUTPKTRDY)))
      {
        ControlReg = USB_CSR0_INPKTRDY;

        if (UsbdCoreInfo.DataSize < UsbdMaxPacketSize0)
        {
          USBD_HW_WriteEP(0x80, UsbdCoreInfo.DataPtr, UsbdCoreInfo.DataSize);
          UsbdCoreInfo.DataSent += UsbdCoreInfo.DataSize;
          UsbdCoreInfo.DataSize = 0;
          ControlReg |= USB_CSR0_DATAEND;
          Endpoint0State = EP0_STATE_IDLE;
        }
        else
        {
          USBD_HW_WriteEP(0x80, UsbdCoreInfo.DataPtr, UsbdMaxPacketSize0);
          UsbdCoreInfo.DataPtr  += UsbdMaxPacketSize0;
          UsbdCoreInfo.DataSize -= UsbdMaxPacketSize0;
          UsbdCoreInfo.DataSent += UsbdMaxPacketSize0;
        }

        if (UsbdCoreInfo.DataSent == UsbdCoreInfo.SetupPacket.wLength)
        {
          ControlReg |= USB_CSR0_DATAEND;
          Endpoint0State = EP0_STATE_IDLE;
        }
        USB->CSR0 = ControlReg;
      }
    }
  }

  if (Endpoint0State == EP0_STATE_RX)
  {
    ControlReg = USB->CSR0;

    if (ControlReg & USB_CSR0_OUTPKTRDY)
    {
      ControlReg = USB_CSR0_SVDOUTPKTRDY;
      if (UsbdCoreInfo.DataSize < UsbdMaxPacketSize0)
      {
        USBD_HW_ReadEP(0x00, UsbdCoreInfo.DataPtr, UsbdCoreInfo.DataSize);
        UsbdCoreInfo.DataSent += UsbdCoreInfo.DataSize;
        UsbdCoreInfo.DataSize = 0;
      }
      else
      {
        USBD_HW_ReadEP(0x00, UsbdCoreInfo.DataPtr, UsbdMaxPacketSize0);
        UsbdCoreInfo.DataPtr  += UsbdMaxPacketSize0;
        UsbdCoreInfo.DataSize -= UsbdMaxPacketSize0;
        UsbdCoreInfo.DataSent += UsbdMaxPacketSize0;
      }

      if (UsbdCoreInfo.DataSize == 0)
      {
        ControlReg |= USB_CSR0_DATAEND;

        if (USBD_User_EndPoint0_Out() != USBD_CONTROL_STAGE_STATUS)
        {
          ControlReg |= USB_CSR0_SENDSTALL;
          Endpoint0State = EP0_STATE_STALL;
        }
        else
        {
          Endpoint0State = EP0_STATE_IDLE;
        }
        USB->INDEX = 0;
      }

      USB->CSR0 = ControlReg;
    }
  }
}

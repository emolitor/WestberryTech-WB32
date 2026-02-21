#include "wb32f10x.h"
#include "usbd_user.h"
#include "usbd_cdc.h"
#include "usbd_user_cdc.h"

volatile uint32_t bulk_in_pending_flag = 0;
volatile uint32_t bulk_in_zlp_flag = 0;


/**
 * @brief  Initializes the USB Device.
 * @return None
 */
void USBD_User_Init(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC_SFM, ENABLE);

  /* Enable USB peripheral clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_USB, ENABLE);

  /* Reset USB peripheral */
  RCC_AHBPeriphResetCmd(RCC_AHBPeriph_USB, ENABLE);
  RCC_AHBPeriphResetCmd(RCC_AHBPeriph_USB, DISABLE);

  /* Configure USB FIFO clock source */
  RCC_USBFIFOCLKConfig(RCC_USBFIFOCLKSource_USBCLK);

  /* Enable USB FIFO clock */
  RCC_USBFIFOCLKCmd(ENABLE);

  /* Configure and enable USB PHY */
  SFM->USBPCON = 0x02;

  /* Configure and enable USBCLK */
  RCC_USBCLKConfig(RCC_USBCLKSource_MAINCLK_Div2, ENABLE);

  USB->INTRUSBE = 0x00;
  USB->INTRINE = 0x00;
  USB->INTROUTE = 0x00;

  /* Configure and enable USB interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USB_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief  Deinitializes the USB Device.
 * @return None
 */
void USBD_User_DeInit(void)
{
  /* Disable USB interrupt channel */
  NVIC_DisableIRQ(USB_IRQn);

  /* Disable USBCLK */
  RCC_USBCLKConfig(0, DISABLE);

  /* Disable USB PHY */
  PWR_UnlockANA();
  ANCTL->USBPCR &= 0xFFFFFFE0;
  PWR_LockANA();

  /* Disable USB FIFO clock */
  RCC_USBFIFOCLKCmd(DISABLE);

  /* Disable USB peripheral clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_USB, DISABLE);
}

/**
 * @brief  Connects the device to the USB host.
 * @return None
 */
void USBD_User_Connect(void)
{
  /* Enable BMX1, GPIOA clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 | RCC_APB1Periph_GPIOA, ENABLE);
  /* Configure the drive current of PA11 and PA12 */
  GPIO_DriveCurrentConfig(GPIOA, GPIO_Pin_11 |GPIO_Pin_12, 0x03);
  /* Configure PA11 and PA12 as Alternate function mode */
  GPIO_Init(GPIOA, GPIO_Pin_11 |GPIO_Pin_12, GPIO_MODE_AF | GPIO_AF3);

  USB->POWER = USB_POWER_SUSEN;
  USB->INTRUSBE = USB_INTRUSBE_RSTIE | USB_INTRUSBE_RSUIE | USB_INTRUSBE_SUSIE;
}

/**
 * @brief  Disconnects the device from the USB host.
 * @return None
 */
void USBD_User_Disconnect(void)
{
  /* Enable BMX1, GPIOA clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 | RCC_APB1Periph_GPIOA, ENABLE);
  /* Configure PA11 and PA12 as input mode */
  GPIO_Init(GPIOA, GPIO_Pin_11 |GPIO_Pin_12, GPIO_MODE_IN);
  /* Configure PA12(D+) as open-drain output mode and output low level */
  GPIO_Init(GPIOA, GPIO_Pin_12, GPIO_MODE_OUT |GPIO_OTYPE_OD);
  GPIO_ResetBits(GPIOA, GPIO_Pin_12);
}

/**
 * @brief  USB Reset Event Service Routine.
 * @return None
 */
void USBD_User_Reset(void)
{
  USB->POWER = USB_POWER_SUSEN;
  USB->INTRINE = USB_INTRINE_EP0E;
  USB->INTROUTE = 0x00;

  bulk_in_pending_flag = 0;
  bulk_in_zlp_flag = 0;
}

/**
 * @brief  USB Resume Event Service Routine.
 * @return None
 */
void USBD_User_Resume(void)
{
  GPIO_ResetBits(GPIOB, GPIO_Pin_13);   // Turn on LED2
}

/**
 * @brief  USB Suspend Event Service Routine.
 * @return None
 */
void USBD_User_Suspend(void)
{
  GPIO_SetBits(GPIOB, GPIO_Pin_13);     // Turn off LED2
}

/**
 * @brief  USB SOF Event Service Routine.
 * @return None
 */
void USBD_User_SOF(void)
{
}


/**
 * @brief  Configures device.
 * @param  cfgidx: the configuration index.
 * @return true - Success, false - Error
 */
bool USBD_User_SetConfig(uint8_t cfgidx)
{
  if (cfgidx == 1)
  {
    // Configure IN Endpoint 1 (Interrupt)
    USB->INDEX = 0x01;
    USB->INCSR2 = 0x00;
    USB->INMAXP = (16 >> 3);
    USB->INCSR1 = USB_INCSR1_CLRDATATOG;
    USB->INCSR1 = USB_INCSR1_FLUSHFIFO;
    USB->INCSR1 = USB_INCSR1_FLUSHFIFO;
    USB->INTRINE |= (0x01 << 1);

    // Configure IN Endpoint 2 (Bulk)
    USB->INDEX = 0x02;
    USB->INCSR2 = 0x00;
    USB->INMAXP = (64 >> 3);
    USB->INCSR1 = USB_INCSR1_CLRDATATOG;
    USB->INCSR1 = USB_INCSR1_FLUSHFIFO;
    USB->INCSR1 = USB_INCSR1_FLUSHFIFO;
    USB->INTRINE |= (0x01 << 2);

    // Configure OUT Endpoint 2 (Bulk)
    USB->INDEX = 0x02;
    USB->OUTCSR2 = 0x00;
    USB->OUTMAXP = (64 >> 3);
    USB->OUTCSR1 = USB_OUTCSR1_CLRDATATOG;
    USB->OUTCSR1 = USB_OUTCSR1_FLUSHFIFO;
    USB->OUTCSR1 = USB_OUTCSR1_FLUSHFIFO;
    USB->INTROUTE |= (0x01 << 2);

    return true;
  }

  return false;
}

/**
 * @brief  Clear current configuration.
 * @param  cfgidx: the configuration index.
 * @note   If cfgidx is 0, this function should clear all configuration.
 * @return None
 */
void USBD_User_ClearConfig(uint8_t cfgidx)
{
  USB->INTRINE &= ~(0x01 << 1);
  USB->INTRINE &= ~(0x01 << 2);
  USB->INTROUTE &= ~(0x01 << 2);
}


/**
 * @brief  Handle the setup device requests (Except the recipient is device).
 * @return The next control stage.
 */
UsbdControlStage USBD_User_EndPoint0_Setup(void)
{
  UsbdControlStage next_stage = USBD_CONTROL_STAGE_STALL;

  if ((UsbdCoreInfo.SetupPacket.bmRequestType & (USB_REQUEST_TYPE_Msk | USB_REQUEST_RECIPIENT_Msk)) == 
      (USB_REQUEST_TYPE_CLASS | USB_REQUEST_RECIPIENT_INTERFACE))
  {
    if ((UsbdCoreInfo.SetupPacket.wIndexL == USBD_CDC_ACM_CIF_NUM) ||
        (UsbdCoreInfo.SetupPacket.wIndexL == USBD_CDC_ACM_DIF_NUM)) {
      next_stage = USBD_EndPoint0_Setup_CDC_Req();
    }
  }

  return next_stage;
}

/**
 * @brief  Handle the out device requests.
 * @return The next control stage.
 */
UsbdControlStage USBD_User_EndPoint0_Out(void)
{
  UsbdControlStage next_stage = USBD_CONTROL_STAGE_STALL;

  if ((UsbdCoreInfo.SetupPacket.bmRequestType & (USB_REQUEST_TYPE_Msk | USB_REQUEST_RECIPIENT_Msk)) == 
      (USB_REQUEST_TYPE_CLASS | USB_REQUEST_RECIPIENT_INTERFACE))
  {
    if ((UsbdCoreInfo.SetupPacket.wIndexL == USBD_CDC_ACM_CIF_NUM) ||
        (UsbdCoreInfo.SetupPacket.wIndexL == USBD_CDC_ACM_DIF_NUM)) {
      next_stage = USBD_EndPoint0_Out_CDC_Req();
    }
  }

  return next_stage;
}



/**
 * @brief  IN Endpoint 1 Service Routine.
 * @return None
 */
void USBD_User_EP1_IN(void)
{
}

/**
 * @brief  IN Endpoint 2 Service Routine.
 * @return None
 */
void USBD_User_EP2_IN(void)
{
  if (bulk_in_pending_flag)
  {
    if (bulk_in_zlp_flag)
    {
      USBD_HW_Transmit(0x82, 0, 0);
      bulk_in_zlp_flag = 0;
    }
    else
    {
      /* Ready to get next BULK out */
      USBD_HW_ReadyToReceive(0x02);
      bulk_in_pending_flag = 0;
    }
  }
}

/**
 * @brief  IN Endpoint 3 Service Routine.
 * @return None
 */
void USBD_User_EP3_IN(void)
{
}

/**
 * @brief  OUT Endpoint 1 Service Routine.
 * @return None
 */
void USBD_User_EP1_OUT(void)
{
}

/**
 * @brief  OUT Endpoint 2 Service Routine.
 * @return None
 */
uint8_t  EchoBuffer[64];
void USBD_User_EP2_OUT(void)
{
  USB->INDEX = 0x02;
  if (USB->OUTCSR1 & USB_OUTCSR1_OUTPKTRDY)
  {
    uint16_t len;
    len = USBD_HW_GetRxDataCount(0x02);
    USBD_HW_ReadEP(0x02, EchoBuffer, len);

    USBD_HW_Transmit(0x82, EchoBuffer, len);
    bulk_in_pending_flag = 1;

    if (len % 64 == 0)
      bulk_in_zlp_flag = 1;
    else
      bulk_in_zlp_flag = 0;
  }
}

/**
 * @brief  OUT Endpoint 3 Service Routine.
 * @return None
 */
void USBD_User_EP3_OUT(void)
{
}

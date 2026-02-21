/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"
#include "usbd_core.h"
#include "usbd_user_hid.h"
#include "gpio_bit_band.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint32_t key1_state = 0;
volatile uint32_t last_key1_state = 0;
volatile uint32_t key2_state = 0;
volatile uint32_t last_key2_state = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Delay n ticks.
 * @param  n_ticks: Number of ticks to delay.
 * @note   Since SysTick_LOAD is a 24-bit register, 
 *         the n_ticks must be less than 16777216
 * @return None
 */
void SysTick_DelayNticks(uint32_t n_ticks)
{
  uint32_t status;
  /* Configure the reload value */
  SysTick->LOAD = n_ticks;
  /* Clear the counter */
  SysTick->VAL = 0x00;
  /* The core clock as clock source and enable SysTick */
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
  /* Wait for count to 0 */
  do {
    status = SysTick->CTRL;
  } while((status & SysTick_CTRL_ENABLE_Msk) && (!(status & SysTick_CTRL_COUNTFLAG_Msk)));
  /* Disable SysTick */
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

int keyboard_send_report(void)
{
  int error = -1;

  __disable_irq();

  if (UsbdCoreInfo.DeviceState == USBD_STATE_CONFIGURED)
  {
    if (hid_report_xfer_flag == 0)
    {
      USBD_HW_Transmit(0x81, hid_report_buf, 8);
      hid_report_xfer_flag = 1;
      error = 0;
    }
    else
      error = 1;
  }
  else
    error = 2;

  __enable_irq();

  return error;
}

int main(void)
{
  uint32_t key1_event, key2_event;
  SystemCoreClockUpdate();
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  /* Initializes KEY pins */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOA, ENABLE);
  GPIO_Init(GPIOA, GPIO_Pin_0, GPIO_MODE_IN |GPIO_PUPD_DOWN);
  GPIO_Init(GPIOA, GPIO_Pin_1, GPIO_MODE_IN |GPIO_PUPD_UP);

  /* Initializes and turn on LED1 and LED2 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOB, ENABLE);
  GPIO_Init(GPIOB, GPIO_Pin_13 |GPIO_Pin_14, GPIO_MODE_OUT |GPIO_OTYPE_PP |GPIO_PUPD_NOPULL |GPIO_SPEED_HIGH);
  GPIO_ResetBits(GPIOB, GPIO_Pin_13 |GPIO_Pin_14);


  USBD_Init();
  USBD_Connect();

  /* Infinite loop */
  while (1)
  {
    if (PAin(0) != 0) {
      SysTick_DelayNticks((SystemCoreClock / 1000) * 10);   // delay 10ms
      if (PAin(0) != 0)
        key1_state = 1;
    }
    else {
      SysTick_DelayNticks((SystemCoreClock / 1000) * 10);   // delay 10ms
      if (PAin(0) == 0)
        key1_state = 0;
    }
    
    if (PAin(1) == 0) {
      SysTick_DelayNticks((SystemCoreClock / 1000) * 10);   // delay 10ms
      if (PAin(1) == 0) {
        key2_state = 1;
      }
    }
    else {
      SysTick_DelayNticks((SystemCoreClock / 1000) * 10);   // delay 10ms
      if (PAin(1) != 0)
        key2_state = 0;
    }

    key1_event = 0;
    key2_event = 0;
    if (last_key1_state != key1_state)
    {
      if (key1_state != 0)
        key1_event = 1;   // KEY1 down event
      else
        key1_event = 2;   // KEY1 up event
    }
    if (last_key2_state != key2_state)
    {
      if (key2_state != 0)
        key2_event = 1;   // KEY1 down event
      else
        key2_event = 2;   // KEY1 up event
    }
    last_key1_state = key1_state;
    last_key2_state = key2_state;

    if (key1_event || key2_event)
    {
      if (key1_event == 1)
        hid_report_buf[0] |= 0x01;    // LeftControl Key down
      else if (key1_event == 2)
        hid_report_buf[0] &= ~0x01;   // LeftControl Key up

      if (key2_event == 1)
        hid_report_buf[2] = 0x04;     // 'A' Key down
      else if (key2_event == 2)
        hid_report_buf[2] = 0x00;     // 'A' Key up

      while (keyboard_send_report() == 1);
    }

    if ((UsbdCoreInfo.DeviceState == USBD_STATE_SUSPENDED) &&
        (key2_event == 1))
    {
      __disable_irq();
      if (UsbdCoreInfo.DeviceState == USBD_STATE_SUSPENDED)
      {
        if (UsbdCoreInfo.RemoteWakeup != 0)
        {
          /* Remote Wakeup */
          USB->POWER |= USB_POWER_RESUME;
          SysTick_DelayNticks((SystemCoreClock / 1000) * 10);   // delay 10ms
          USB->POWER &= ~USB_POWER_RESUME;
          USBD_Core_Resume();
        }
      }
      __enable_irq();
    }

  }
}

void USB_IRQHandler(void)
{
  uint8_t IntrUSB;
  uint8_t IntrIn;
  uint8_t IntrOut;

  IntrUSB = USB->INTRUSB;
  IntrIn  = USB->INTRIN;
  IntrOut = USB->INTROUT;

  Handle_USBD_INT(IntrUSB, IntrIn, IntrOut);
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

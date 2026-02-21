/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"
#include "usbd_core.h"
#include "drv_spi_flash.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

int main(void)
{
  SystemCoreClockUpdate();
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  /* Initializes and turn on LED1 and LED2 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOB, ENABLE);
  GPIO_Init(GPIOB, GPIO_Pin_13 |GPIO_Pin_14, GPIO_MODE_OUT |GPIO_OTYPE_PP |GPIO_PUPD_NOPULL |GPIO_SPEED_HIGH);
  GPIO_ResetBits(GPIOB, GPIO_Pin_13 |GPIO_Pin_14);

  /* Initialize the SPI FLASH driver */
  SpiFlash_Init();

  /* Get SPI Flash ID */
  SpiFlash_ReadIdentification();

  USBD_Init();
  USBD_Connect();

  /* Infinite loop */
  while (1)
  {
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

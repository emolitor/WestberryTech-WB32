/* Includes ------------------------------------------------------------------*/
#include "wb32f10x.h"
#include "bsp_uart1.h"
#include <stdio.h>
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t slave_rx_buf[100] = {0};
uint16_t master_command_buf[100] = {0};
const uint8_t master_tx_data[100] = 
{
  0x52,0x79,0x0C,0xAF,0xC0,0x99,0x44,0x71,0x9D,0x30,0xAD,0x68,0xF7,0x95,0xF8,0x38,
  0xF5,0x9C,0x19,0xDD,0xEC,0xBC,0x6C,0xF0,0x71,0xD8,0xE4,0x1D,0x6F,0x7F,0x08,0xD7,
  0x92,0x61,0x36,0x36,0xE3,0x08,0x7E,0xED,0x44,0x22,0x72,0x36,0x84,0x27,0xF5,0x98,
  0x95,0x7C,0xFE,0x4F,0x17,0x8A,0xD3,0x6C,0x43,0xB0,0xBC,0xEC,0x07,0x99,0x81,0x29,
  0x28,0x32,0xFD,0x3B,0x8D,0xCD,0x02,0x97,0xAE,0x80,0xE6,0xC9,0xE5,0x3B,0x9A,0x24,
  0x97,0x31,0xAF,0x57,0x00,0x5E,0xCC,0x1B,0xEB,0x5F,0x2C,0x07,0x68,0x21,0x72,0xFD,
  0x8B,0xDE,0x66,0x74
};

/* Private function prototypes -----------------------------------------------*/
void I2C1_Slave_Configuration(void);
void I2C1_Slave_DMA_Rx_Configuration(uint8_t* rx_buf, uint32_t rx_len);
void I2C2_Master_Configuration(void);
void I2C2_Master_DMA_Tx_Configuration(const uint16_t* tx_command, uint32_t command_len);

/* Private functions ---------------------------------------------------------*/

int main(void)
{
  uint32_t iter;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* Init UART1 to 115200-8n1 for print message */
  uart1_init(96000000, 115200);

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMAC2Bridge, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_BMX2 |RCC_APB2Periph_DMAC2, ENABLE);
  DMAC_DeInit(DMAC2);
  DMAC_Cmd(DMAC2, ENABLE);

  I2C1_Slave_Configuration();

  I2C2_Master_Configuration();

  I2C1_Slave_DMA_Rx_Configuration(slave_rx_buf, 100);

  for(iter = 0; iter < 100; iter++) {
    master_command_buf[iter] = master_tx_data[iter];
  }
  master_command_buf[99] |= I2C_DATA_CMD_STOP;
  I2C2_Master_DMA_Tx_Configuration(master_command_buf, 100);


  DMAC_ChannelCmd(DMAC2, DMAC_Channel_0, ENABLE);
  DMAC_ChannelCmd(DMAC2, DMAC_Channel_1, ENABLE);

  I2C_Cmd(I2C1, ENABLE);
  I2C_DMACmd(I2C1, I2C_DMAReq_Rx, ENABLE);

  I2C_Cmd(I2C2, ENABLE);
  I2C_DMACmd(I2C2, I2C_DMAReq_Tx, ENABLE);

  while(DMAC_GetChannelCmdStatus(DMAC2, DMAC_Channel_1) != DISABLE);
  while(DMAC_GetChannelCmdStatus(DMAC2, DMAC_Channel_0) != DISABLE);

  if(memcmp(slave_rx_buf, master_tx_data, 100) == 0) {
    printf("Master send to slave success!\r\n");
  }
  else {
    printf("Master send to slave fail!\r\n");
  }

  /* Infinite loop */
  while (1)
  {
  }
}

void I2C1_Slave_Configuration(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_BMX2 |RCC_APB2Periph_I2C1, ENABLE);

  /*
    PB6 (I2C1_SCL)
    PB7 (I2C1_SDA)
  */
  GPIO_Init(GPIOB, GPIO_Pin_6 |GPIO_Pin_7, GPIO_MODE_AF |GPIO_OTYPE_OD |GPIO_PUPD_UP |GPIO_SPEED_HIGH |GPIO_AF4);

  I2C_DeInit(I2C1);
  I2C_Init(I2C1, I2C_CON_STOP_DET_IFADDRESSED | I2C_CON_SPEED_STANDARD);
  I2C_OwnAddressConfig(I2C1, 0xB4 >> 1);
  I2C_RxFIFOThresholdConfig(I2C1, 0);
  I2C_TxFIFOThresholdConfig(I2C1, 0);
  I2C_DMATxReqLevelConfig(I2C1, 4);   /* Must adhere to this equation: DMAC.CTLLx.DEST_MSIZE <= I2C.FIFO_DEPTH - I2C.DMA_TDLR */
  I2C_DMARxReqLevelConfig(I2C1, 3);   /* Must adhere to this equation: DMAC.CTLLx.SRC_MSIZE = I2C.DMA_RDLR + 1 */

  I2C1->FS_SPKLEN = 4;    /* tSP = 4 / 96MHz = 41.67ns */
  I2C1->SDA_SETUP = 10;   /* tSU;DAT = 10 / 96MHz = 104.17ns */
  I2C1->SDA_HOLD = 29;    /* tHD;DAT = 29 / 96MHz = 302.08ns */
}

void I2C1_Slave_DMA_Rx_Configuration(uint8_t* rx_buf, uint32_t rx_len)
{
  DMAC_Channel_InitTypeDef DMAC_Channel_InitStruct;

  DMAC_Channel_InitStruct.DMAC_SourceBaseAddr = (uint32_t)&I2C1->DATA_CMD;
  DMAC_Channel_InitStruct.DMAC_DestinationBaseAddr = (uint32_t)rx_buf;
  DMAC_Channel_InitStruct.DMAC_Interrupt = DMAC_Interrupt_Disable;
  DMAC_Channel_InitStruct.DMAC_SourceTransferWidth = DMAC_SourceTransferWidth_8b;
  DMAC_Channel_InitStruct.DMAC_DestinationTransferWidth = DMAC_DestinationTransferWidth_8b;
  DMAC_Channel_InitStruct.DMAC_SourceAddrInc = DMAC_SourceAddrInc_NoChange;
  DMAC_Channel_InitStruct.DMAC_DestinationAddrInc = DMAC_DestinationAddrInc_Increment;
  DMAC_Channel_InitStruct.DMAC_SourceTransactionLength = DMAC_SourceTransactionLength_4;
  DMAC_Channel_InitStruct.DMAC_DestinationTransactionLength = DMAC_DestinationTransactionLength_4;
  DMAC_Channel_InitStruct.DMAC_TransferTypeAndFlowControl = DMAC_TransferTypeAndFlowControl_PeripheralToMemory_DMAC;
  DMAC_Channel_InitStruct.DMAC_SourceMasterInterface = DMAC_SourceMasterInterface_APB;
  DMAC_Channel_InitStruct.DMAC_DestinationMasterInterface = DMAC_DestinationMasterInterface_AHB;
  DMAC_Channel_InitStruct.DMAC_BlockTransferSize = rx_len;
  DMAC_Channel_InitStruct.DMAC_SourceHandshakingInterfaceSelect = DMAC_SourceHandshakingInterfaceSelect_Hardware;
  DMAC_Channel_InitStruct.DMAC_DestinationHandshakingInterfaceSelect = DMAC_DestinationHandshakingInterfaceSelect_Hardware;
  DMAC_Channel_InitStruct.DMAC_SourceHandshakingInterfacePolarity = DMAC_SourceHandshakingInterfacePolarity_High;
  DMAC_Channel_InitStruct.DMAC_DestinationHandshakingInterfacePolarity = DMAC_DestinationHandshakingInterfacePolarity_High;
  DMAC_Channel_InitStruct.DMAC_AutomaticSourceReload = DMAC_AutomaticSourceReload_Disable;
  DMAC_Channel_InitStruct.DMAC_AutomaticDestinationReload = DMAC_AutomaticDestinationReload_Disable;
  DMAC_Channel_InitStruct.DMAC_FlowControlMode = DMAC_FlowControlMode_0;
  DMAC_Channel_InitStruct.DMAC_FIFOMode = DMAC_FIFOMode_0;
  DMAC_Channel_InitStruct.DMAC_ChannelPriority = 0;
  DMAC_Channel_InitStruct.DMAC_ProtectionControl = 0x1;
  DMAC_Channel_InitStruct.DMAC_SourceHardwareHandshakingInterfaceAssign = DMAC_HardwareHandshakingInterface_I2C1_RX;
  DMAC_Channel_InitStruct.DMAC_DestinationHardwareHandshakingInterfaceAssign = 0;
  DMAC_Channel_InitStruct.DMAC_MaximumAMBABurstLength = 0;
  DMAC_Channel_Init(DMAC2, DMAC_Channel_0, &DMAC_Channel_InitStruct);
}

void I2C2_Master_Configuration(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_BMX2 |RCC_APB2Periph_I2C2, ENABLE);

  /*
    PB10 (I2C2_SCL)
    PB11 (I2C2_SDA)
  */
  GPIO_Init(GPIOB, GPIO_Pin_10 |GPIO_Pin_11, GPIO_MODE_AF |GPIO_OTYPE_OD |GPIO_PUPD_UP |GPIO_SPEED_HIGH |GPIO_AF4);

  I2C_DeInit(I2C2);
  I2C_ITConfig(I2C2, 0xFFFF, DISABLE);
  I2C_Init(I2C2, I2C_CON_MASTER_MODE |I2C_CON_SPEED_STANDARD |I2C_CON_RESTART_EN |I2C_CON_SLAVE_DISABLE);
  I2C_TargetAddressConfig(I2C2, 0xB4 >> 1);
  I2C_RxFIFOThresholdConfig(I2C2, 0);
  I2C_TxFIFOThresholdConfig(I2C2, 0);
  I2C_DMATxReqLevelConfig(I2C2, 4);   /* Must adhere to this equation: DMAC.CTLLx.DEST_MSIZE <= I2C.FIFO_DEPTH - I2C.DMA_TDLR */
  I2C_DMARxReqLevelConfig(I2C2, 3);   /* Must adhere to this equation: DMAC.CTLLx.SRC_MSIZE = I2C.DMA_RDLR + 1 */

  I2C2->SS_SCL_HCNT = 373;  /* tHIGH = (373 + FS_SPKLEN + 7) / 96MHz = 4us */
  I2C2->SS_SCL_LCNT = 451;  /* tLOW = (451 + 1) / 96MHz = 4.708us */
  I2C2->FS_SPKLEN = 4;      /* tSP = 4 / 96MHz = 41.67ns */
  I2C2->SDA_SETUP = 24;     /* tSU;DAT = 24 / 96MHz = 250ns */
  I2C2->SDA_HOLD = 29;      /* tHD;DAT = 29 / 96MHz = 302.08ns */
}

void I2C2_Master_DMA_Tx_Configuration(const uint16_t* tx_command, uint32_t command_len)
{
  DMAC_Channel_InitTypeDef DMAC_Channel_InitStruct;

  DMAC_Channel_InitStruct.DMAC_SourceBaseAddr = (uint32_t)tx_command;
  DMAC_Channel_InitStruct.DMAC_DestinationBaseAddr = (uint32_t)&I2C2->DATA_CMD;
  DMAC_Channel_InitStruct.DMAC_Interrupt = DMAC_Interrupt_Disable;
  DMAC_Channel_InitStruct.DMAC_SourceTransferWidth = DMAC_SourceTransferWidth_16b;
  DMAC_Channel_InitStruct.DMAC_DestinationTransferWidth = DMAC_DestinationTransferWidth_16b;
  DMAC_Channel_InitStruct.DMAC_SourceAddrInc = DMAC_SourceAddrInc_Increment;
  DMAC_Channel_InitStruct.DMAC_DestinationAddrInc = DMAC_DestinationAddrInc_NoChange;
  DMAC_Channel_InitStruct.DMAC_SourceTransactionLength = DMAC_SourceTransactionLength_4;
  DMAC_Channel_InitStruct.DMAC_DestinationTransactionLength = DMAC_DestinationTransactionLength_4;
  DMAC_Channel_InitStruct.DMAC_TransferTypeAndFlowControl = DMAC_TransferTypeAndFlowControl_MemoryToPeripheral_DMAC;
  DMAC_Channel_InitStruct.DMAC_SourceMasterInterface = DMAC_SourceMasterInterface_AHB;
  DMAC_Channel_InitStruct.DMAC_DestinationMasterInterface = DMAC_DestinationMasterInterface_APB;
  DMAC_Channel_InitStruct.DMAC_BlockTransferSize = command_len;
  DMAC_Channel_InitStruct.DMAC_SourceHandshakingInterfaceSelect = DMAC_SourceHandshakingInterfaceSelect_Hardware;
  DMAC_Channel_InitStruct.DMAC_DestinationHandshakingInterfaceSelect = DMAC_DestinationHandshakingInterfaceSelect_Hardware;
  DMAC_Channel_InitStruct.DMAC_SourceHandshakingInterfacePolarity = DMAC_SourceHandshakingInterfacePolarity_High;
  DMAC_Channel_InitStruct.DMAC_DestinationHandshakingInterfacePolarity = DMAC_DestinationHandshakingInterfacePolarity_High;
  DMAC_Channel_InitStruct.DMAC_AutomaticSourceReload = DMAC_AutomaticSourceReload_Disable;
  DMAC_Channel_InitStruct.DMAC_AutomaticDestinationReload = DMAC_AutomaticDestinationReload_Disable;
  DMAC_Channel_InitStruct.DMAC_FlowControlMode = DMAC_FlowControlMode_0;
  DMAC_Channel_InitStruct.DMAC_FIFOMode = DMAC_FIFOMode_0;
  DMAC_Channel_InitStruct.DMAC_ChannelPriority = 0;
  DMAC_Channel_InitStruct.DMAC_ProtectionControl = 0x1;
  DMAC_Channel_InitStruct.DMAC_SourceHardwareHandshakingInterfaceAssign = 0;
  DMAC_Channel_InitStruct.DMAC_DestinationHardwareHandshakingInterfaceAssign = DMAC_HardwareHandshakingInterface_I2C2_TX;
  DMAC_Channel_InitStruct.DMAC_MaximumAMBABurstLength = 0;
  DMAC_Channel_Init(DMAC2, DMAC_Channel_1, &DMAC_Channel_InitStruct);
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

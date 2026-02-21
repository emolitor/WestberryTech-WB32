/* Includes ------------------------------------------------------------------*/
#include "drv_spi_flash_quad.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DUMMY_BYTE     0xFF

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Initializes the peripherals used by the SPI FLASH driver.
 * @param  None
 * @return None
 */
void SpiFlash_Init(void)
{
  SPI_InitTypeDef SPI_InitStructure;

  /* Enable the GPIOA, GPIOB and QSPI clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOA |RCC_APB1Periph_GPIOB |RCC_APB1Periph_QSPI, ENABLE);

  /*
    PA4 (QSPI_NSS0)
    PA5 (QSPI_SCK)
    PA6 (QSPI_MI_IO1)
    PA7 (QSPI_MO_IO0)
    PB0 (QSPI_IO2)
    PB1 (QSPI_IO3)
  */
  GPIO_Init(GPIOA, GPIO_Pin_4 |GPIO_Pin_5 |GPIO_Pin_6 |GPIO_Pin_7, GPIO_MODE_AF |GPIO_OTYPE_PP |GPIO_PUPD_NOPULL |GPIO_SPEED_HIGH |GPIO_AF5);
  GPIO_Init(GPIOB, GPIO_Pin_0 |GPIO_Pin_1, GPIO_MODE_AF |GPIO_OTYPE_PP |GPIO_PUPD_NOPULL |GPIO_SPEED_HIGH |GPIO_AF5);

//  GPIO_DriveCurrentConfig(GPIOA, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, 0x03);
//  GPIO_DriveCurrentConfig(GPIOB, GPIO_Pin_0 | GPIO_Pin_1, 0x03);

  /* QSPI configuration */
  SPI_DeInit(QSPI);
  SPI_InitStructure.SPI_TransferMode = SPI_TransferMode_TxAndRx;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_BaudRatePrescaler = 40;   /* In order to avoid TX FIFO underflow or RX FIFO overflow, SPI_BaudRatePrescaler needs to bigger */
  SPI_InitStructure.SPI_FrameFormat = SPI_FrameFormat_SPI;
  SPI_Init(QSPI, &SPI_InitStructure);
  SPI_NSSConfig(QSPI, SPI_NSS_0, ENABLE);
}

/**
 * @brief  Read JEDEC ID (9Fh).
 * @param  None
 * @return 24bit JEDEC ID (Eg. the w25q32 id is 0xEF4016)
 */
uint32_t SpiFlash_ReadIdentification(void)
{
  uint32_t jedec_id;

  SPI_TransferModeConfig(QSPI, SPI_TransferMode_EepromRead);
  SPI_ReceiveDataLengthConfig(QSPI, 3);
  SPI_Cmd(QSPI, ENABLE);

  SPI_WriteData(QSPI, 0x9F);    // Read JEDEC ID (9Fh)

  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_RFNE) == RESET);
  jedec_id = SPI_ReadData(QSPI);
  jedec_id <<= 8;
  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_RFNE) == RESET);
  jedec_id |= SPI_ReadData(QSPI);
  jedec_id <<= 8;
  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_RFNE) == RESET);
  jedec_id |= SPI_ReadData(QSPI);

  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_BUSY) != RESET);
  SPI_Cmd(QSPI, DISABLE);

  return jedec_id;
}

/**
 * @brief  Read Manufacturer / Device ID (90h)
 * @param  None
 * @return [15:8] is Manufacturer ID, [7:0] is Device ID
 *         (Eg. the w25q32 id is 0xEF15)
 */
uint16_t SpiFlash_ReadMidDid(void)
{
  uint16_t mid_did = 0;

  SPI_TransferModeConfig(QSPI, SPI_TransferMode_EepromRead);
  SPI_ReceiveDataLengthConfig(QSPI, 2);
  SPI_Cmd(QSPI, ENABLE);

  SPI_WriteData(QSPI, 0x90);    // Read Manufacturer / Device ID (90h)
  SPI_WriteData(QSPI, 0x00);
  SPI_WriteData(QSPI, 0x00);
  SPI_WriteData(QSPI, 0x00);

  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_RFNE) == RESET);
  mid_did = SPI_ReadData(QSPI);
  mid_did <<= 8;
  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_RFNE) == RESET);
  mid_did |= SPI_ReadData(QSPI);

  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_BUSY) != RESET);
  SPI_Cmd(QSPI, DISABLE);

  return mid_did;
}

/**
 * @brief  Read Status Register-1 (05h)
 * @param  None
 * @return Status Register-1
 */
uint8_t SpiFlash_ReadStatusReg1(void)
{
  uint8_t status;

  SPI_TransferModeConfig(QSPI, SPI_TransferMode_EepromRead);
  SPI_ReceiveDataLengthConfig(QSPI, 1);
  SPI_Cmd(QSPI, ENABLE);

  SPI_WriteData(QSPI, 0x05);    // Read Status Register-1 (05h)
  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_RFNE) == RESET);
  status = SPI_ReadData(QSPI);

  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_BUSY) != RESET);
  SPI_Cmd(QSPI, DISABLE);

  return status;
}

/**
 * @brief  Read Status Register-2 (35h)
 * @param  None
 * @return Status Register-2
 */
uint8_t SpiFlash_ReadStatusReg2(void)
{
  uint8_t status;

  SPI_TransferModeConfig(QSPI, SPI_TransferMode_EepromRead);
  SPI_ReceiveDataLengthConfig(QSPI, 1);
  SPI_Cmd(QSPI, ENABLE);

  SPI_WriteData(QSPI, 0x35);    // Read Status Register-2 (35h)
  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_RFNE) == RESET);
  status = SPI_ReadData(QSPI);

  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_BUSY) != RESET);
  SPI_Cmd(QSPI, DISABLE);

  return status;
}

/**
 * @brief  Read Status Register-3 (15h)
 * @param  None
 * @return Status Register-3
 */
uint8_t SpiFlash_ReadStatusReg3(void)
{
  uint8_t status;

  SPI_TransferModeConfig(QSPI, SPI_TransferMode_EepromRead);
  SPI_ReceiveDataLengthConfig(QSPI, 1);
  SPI_Cmd(QSPI, ENABLE);

  SPI_WriteData(QSPI, 0x15);    // Read Status Register-3 (15h)
  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_RFNE) == RESET);
  status = SPI_ReadData(QSPI);

  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_BUSY) != RESET);
  SPI_Cmd(QSPI, DISABLE);

  return status;
}

/**
 * @brief  Polls the status of the Write In Progress (WIP) flag in the FLASH's
 *         status register and loop until write operation has completed.
 * @param  None
 * @return None
 */
void SpiFlash_WaitReady(void)
{
  while(SpiFlash_ReadStatusReg1() & 0x01);  // check the WIP bit
}

/**
 * @brief  Enables the write access to the FLASH.
 * @param  None
 * @return None
 */
void SpiFlash_WriteEnable(void)
{
  SPI_TransferModeConfig(QSPI, SPI_TransferMode_TxOnly);
  SPI_Cmd(QSPI, ENABLE);

  SPI_WriteData(QSPI, 0x06);    // Write Enable (06h)

  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_TFE) == RESET);
  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_BUSY) != RESET);
  SPI_Cmd(QSPI, DISABLE);
}

/**
 * @brief  Write Status Register (01h)
 * @param  value: The data to write.
 * @return None
 */
void SpiFlash_WriteStatusReg(uint16_t value)
{
  SpiFlash_WriteEnable();

  SPI_TransferModeConfig(QSPI, SPI_TransferMode_TxOnly);
  SPI_Cmd(QSPI, ENABLE);

  SPI_WriteData(QSPI, 0x01);    // Write Status Register (01h)
  SPI_WriteData(QSPI, (uint8_t)(value));
  SPI_WriteData(QSPI, (uint8_t)(value >> 8));

  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_TFE) == RESET);
  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_BUSY) != RESET);
  SPI_Cmd(QSPI, DISABLE);

  SpiFlash_WaitReady();
}

/**
 * @brief  Write Status Register-1 (01h)
 * @param  value: The data to write.
 * @return None
 */
void SpiFlash_WriteStatusReg1(uint8_t value)
{
  SpiFlash_WriteEnable();

  SPI_TransferModeConfig(QSPI, SPI_TransferMode_TxOnly);
  SPI_Cmd(QSPI, ENABLE);

  SPI_WriteData(QSPI, 0x01);    // Write Status Register-1 (01h)
  SPI_WriteData(QSPI, value);

  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_TFE) == RESET);
  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_BUSY) != RESET);
  SPI_Cmd(QSPI, DISABLE);

  SpiFlash_WaitReady();
}

/**
 * @brief  Write Status Register-2 (31h)
 * @param  value: The data to write.
 * @return None
 */
void SpiFlash_WriteStatusReg2(uint8_t value)
{
  SpiFlash_WriteEnable();

  SPI_TransferModeConfig(QSPI, SPI_TransferMode_TxOnly);
  SPI_Cmd(QSPI, ENABLE);

  SPI_WriteData(QSPI, 0x31);    // Write Status Register-2 (31h)
  SPI_WriteData(QSPI, value);

  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_TFE) == RESET);
  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_BUSY) != RESET);
  SPI_Cmd(QSPI, DISABLE);

  SpiFlash_WaitReady();
}

/**
 * @brief  Reads a block of data from the FLASH.
 * @param  address: FLASH's internal address to read from.
 * @param  buffer: pointer to the buffer that receives the data read from the FLASH.
 * @param  length: number of bytes to read from the FLASH.
 * @return None
 */
void SpiFlash_ReadData(uint32_t address, uint8_t* buffer, uint32_t length)
{
  SPI_TransferModeConfig(QSPI, SPI_TransferMode_EepromRead);
  SPI_ReceiveDataLengthConfig(QSPI, length);
  SPI_Cmd(QSPI, ENABLE);

  SPI_WriteData(QSPI, 0x03);    // Read Data (03h)
  SPI_WriteData(QSPI, (uint8_t)(address >> 16));
  SPI_WriteData(QSPI, (uint8_t)(address >> 8));
  SPI_WriteData(QSPI, (uint8_t)(address));

  while(length--)
  {
    while(SPI_GetFlagStatus(QSPI, SPI_FLAG_RFNE) == RESET);
    *buffer++ = SPI_ReadData(QSPI);
  }

  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_BUSY) != RESET);
  SPI_Cmd(QSPI, DISABLE);
}

/**
 * @brief  Erases the entire FLASH.
 * @param  None
 * @return None
 */
void SpiFlash_ChipErase(void)
{
  SpiFlash_WriteEnable();

  SPI_TransferModeConfig(QSPI, SPI_TransferMode_TxOnly);
  SPI_Cmd(QSPI, ENABLE);

  SPI_WriteData(QSPI, 0xC7);    // Chip Erase (C7h)

  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_TFE) == RESET);
  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_BUSY) != RESET);
  SPI_Cmd(QSPI, DISABLE);

  SpiFlash_WaitReady();
}

/**
 * @brief  Erases the specified FLASH sector.
 * @param  address: address of the sector to erase.
 * @return None
 */
void SpiFlash_SectorErase(uint32_t address)
{
  SpiFlash_WriteEnable();

  SPI_TransferModeConfig(QSPI, SPI_TransferMode_TxOnly);
  SPI_Cmd(QSPI, ENABLE);

  SPI_WriteData(QSPI, 0x20);    // Sector Erase (20h)
  SPI_WriteData(QSPI, (uint8_t)(address >> 16));
  SPI_WriteData(QSPI, (uint8_t)(address >> 8));
  SPI_WriteData(QSPI, (uint8_t)(address));

  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_TFE) == RESET);
  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_BUSY) != RESET);
  SPI_Cmd(QSPI, DISABLE);

  SpiFlash_WaitReady();
}


/**
 * @brief  Writes data to one page (256B).
 * @note   The number of byte can't exceed the FLASH page remain size.
 * @param  address: FLASH's internal address to write to.
 * @param  buffer: pointer to the buffer containing the data to be written
 *         to the FLASH.
 * @param  length: number of bytes to write to the FLASH.
 * @return None
 */
void SpiFlash_PageProgram(uint32_t address, const uint8_t* buffer, uint16_t length)
{
  SpiFlash_WriteEnable();

  SPI_TransferModeConfig(QSPI, SPI_TransferMode_TxOnly);
  SPI_Cmd(QSPI, ENABLE);

  SPI_WriteData(QSPI, 0x02);    // Page Program (02h)
  SPI_WriteData(QSPI, (uint8_t)(address >> 16));
  SPI_WriteData(QSPI, (uint8_t)(address >> 8));
  SPI_WriteData(QSPI, (uint8_t)(address));

  while(length--)
  {
    while(SPI_GetFlagStatus(QSPI, SPI_FLAG_TFNF) == RESET);
    SPI_WriteData(QSPI, *buffer++);
  }

  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_TFE) == RESET);
  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_BUSY) != RESET);
  SPI_Cmd(QSPI, DISABLE);

  SpiFlash_WaitReady();
}

/**
 * @brief  Writes block of data to the FLASH.
 * @param  address: FLASH's internal address to write to.
 * @param  buffer: pointer to the buffer containing the data to be written
 *         to the FLASH.
 * @param  length: number of bytes to write to the FLASH.
 * @return None
 */
void SpiFlash_WriteBuffer(uint32_t address, const uint8_t* buffer, uint32_t length)
{
  uint32_t write_len;
  write_len = 256 - (address % 256);    /* calculate the start page remain size */
  if(length <= write_len) write_len = length;

  while(1)
  {
    SpiFlash_PageProgram(address, buffer, write_len);
    if(length == write_len) break;    /* Write complete */
    else {
      buffer += write_len;
      address += write_len;

      length -= write_len;
      if(length > 256) write_len = 256;
      else write_len = length;
    }
  }
}

/**
 * @brief  Read Manufacturer / Device ID through Dual I/O
 * @param  None
 * @return [15:8] is Manufacturer ID, [7:0] is Device ID
 *         (Eg. the w25q32 id is 0xEF15)
 */
uint16_t SpiFlash_DualReadMidDid(void)
{
  SPI_EnhancedSpiInitTypeDef SPI_EnhancedSpiInitStructure;
  uint16_t mid_did = 0;

  SPI_TransferModeConfig(QSPI, SPI_TransferMode_RxOnly);
  SPI_ReceiveDataLengthConfig(QSPI, 2);
  SPI_SpiModeConfig(QSPI, SPI_SpiMode_Dual);
  SPI_EnhancedSpiInitStructure.SPI_EnhancedSpiTransferType = SPI_EnhancedSpiTransferType_1;
  SPI_EnhancedSpiInitStructure.SPI_EnhancedSpiInstructionLength = SPI_EnhancedSpiInstructionLength_8b;
  SPI_EnhancedSpiInitStructure.SPI_EnhancedSpiAddressLength = SPI_EnhancedSpiAddressLength_32b;
  SPI_EnhancedSpiInitStructure.SPI_EnhancedSpiWaitCycles = 0;
  SPI_EnhancedSpiConfig(QSPI, &SPI_EnhancedSpiInitStructure);
  SPI_Cmd(QSPI, ENABLE);

  SPI_WriteData(QSPI, 0x92);    // Read Manufacturer / Device ID Dual I/O (92h)
  SPI_WriteData(QSPI, 0x0000);
  SPI_WriteData(QSPI, 0x00FA);

  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_RFNE) == RESET);
  mid_did = SPI_ReadData(QSPI);
  mid_did <<= 8;
  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_RFNE) == RESET);
  mid_did |= SPI_ReadData(QSPI);

  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_BUSY) != RESET);
  SPI_Cmd(QSPI, DISABLE);

  SPI_SpiModeConfig(QSPI, SPI_SpiMode_Standard);
  return mid_did;
}

/**
 * @brief  Reads a block of data from the FLASH through Dual I/O.
 * @param  address: FLASH's internal address to read from.
 * @param  buffer: pointer to the buffer that receives the data read from the FLASH.
 * @param  length: number of bytes to read from the FLASH.
 * @return None
 */
void SpiFlash_DualFastRead(uint32_t address, uint8_t* buffer, uint32_t length)
{
  SPI_EnhancedSpiInitTypeDef SPI_EnhancedSpiInitStructure;
  uint32_t iter;

  SPI_TransferModeConfig(QSPI, SPI_TransferMode_RxOnly);
  SPI_ReceiveDataLengthConfig(QSPI, length);
  SPI_SpiModeConfig(QSPI, SPI_SpiMode_Dual);
  SPI_EnhancedSpiInitStructure.SPI_EnhancedSpiTransferType = SPI_EnhancedSpiTransferType_1;
  SPI_EnhancedSpiInitStructure.SPI_EnhancedSpiInstructionLength = SPI_EnhancedSpiInstructionLength_8b;
  SPI_EnhancedSpiInitStructure.SPI_EnhancedSpiAddressLength = SPI_EnhancedSpiAddressLength_32b;
  SPI_EnhancedSpiInitStructure.SPI_EnhancedSpiWaitCycles = 0;
  SPI_EnhancedSpiConfig(QSPI, &SPI_EnhancedSpiInitStructure);
  SPI_Cmd(QSPI, ENABLE);

  SPI_WriteData(QSPI, 0xBB);    // Dual I/O Fast Read (BBH)
  SPI_WriteData(QSPI, (uint16_t)(address >> 8));
  SPI_WriteData(QSPI, (uint16_t)((address << 8) | 0xFA));

  for(iter = 0; iter < length; iter++)
  {
    while(SPI_GetFlagStatus(QSPI, SPI_FLAG_RFNE) == RESET);
    *(buffer + iter) = SPI_ReadData(QSPI);
  }

  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_BUSY) != RESET);
  SPI_Cmd(QSPI, DISABLE);

  SPI_SpiModeConfig(QSPI, SPI_SpiMode_Standard);
}

/**
 * @brief  Read Manufacturer / Device ID through Quad I/O
 * @param  None
 * @return [15:8] is Manufacturer ID, [7:0] is Device ID
 *         (Eg. the w25q32 id is 0xEF15)
 */
uint16_t SpiFlash_QuadReadMidDid(void)
{
  SPI_EnhancedSpiInitTypeDef SPI_EnhancedSpiInitStructure;
  uint16_t mid_did = 0;

  SPI_TransferModeConfig(QSPI, SPI_TransferMode_RxOnly);
  SPI_ReceiveDataLengthConfig(QSPI, 2);
  SPI_SpiModeConfig(QSPI, SPI_SpiMode_Quad);
  SPI_EnhancedSpiInitStructure.SPI_EnhancedSpiTransferType = SPI_EnhancedSpiTransferType_1;
  SPI_EnhancedSpiInitStructure.SPI_EnhancedSpiInstructionLength = SPI_EnhancedSpiInstructionLength_8b;
  SPI_EnhancedSpiInitStructure.SPI_EnhancedSpiAddressLength = SPI_EnhancedSpiAddressLength_32b;
  SPI_EnhancedSpiInitStructure.SPI_EnhancedSpiWaitCycles = 4;
  SPI_EnhancedSpiConfig(QSPI, &SPI_EnhancedSpiInitStructure);
  SPI_Cmd(QSPI, ENABLE);

  SPI_WriteData(QSPI, 0x94);    // Read Manufacturer / Device ID Quad I/O (94h)
  SPI_WriteData(QSPI, 0x0000);
  SPI_WriteData(QSPI, 0x00FA);

  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_RFNE) == RESET);
  mid_did = SPI_ReadData(QSPI);
  mid_did <<= 8;
  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_RFNE) == RESET);
  mid_did |= SPI_ReadData(QSPI);

  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_BUSY) != RESET);
  SPI_Cmd(QSPI, DISABLE);

  SPI_SpiModeConfig(QSPI, SPI_SpiMode_Standard);
  return mid_did;
}

/**
 * @brief  Reads a block of data from the FLASH through Quad I/O.
 * @param  address: FLASH's internal address to read from.
 * @param  buffer: pointer to the buffer that receives the data read from the FLASH.
 * @param  length: number of bytes to read from the FLASH.
 * @return None
 */
void SpiFlash_QuadFastRead(uint32_t address, uint8_t* buffer, uint32_t length)
{
  SPI_EnhancedSpiInitTypeDef SPI_EnhancedSpiInitStructure;
  uint32_t iter;

  SPI_TransferModeConfig(QSPI, SPI_TransferMode_RxOnly);
  SPI_ReceiveDataLengthConfig(QSPI, length);
  SPI_SpiModeConfig(QSPI, SPI_SpiMode_Quad);
  SPI_EnhancedSpiInitStructure.SPI_EnhancedSpiTransferType = SPI_EnhancedSpiTransferType_1;
  SPI_EnhancedSpiInitStructure.SPI_EnhancedSpiInstructionLength = SPI_EnhancedSpiInstructionLength_8b;
  SPI_EnhancedSpiInitStructure.SPI_EnhancedSpiAddressLength = SPI_EnhancedSpiAddressLength_32b;
  SPI_EnhancedSpiInitStructure.SPI_EnhancedSpiWaitCycles = 4;
  SPI_EnhancedSpiConfig(QSPI, &SPI_EnhancedSpiInitStructure);
  SPI_Cmd(QSPI, ENABLE);

  SPI_WriteData(QSPI, 0xEB);    // Quad I/O Fast Read (EBH)
  SPI_WriteData(QSPI, (uint16_t)(address >> 8));
  SPI_WriteData(QSPI, (uint16_t)((address << 8) | 0xFA));

  for(iter = 0; iter < length; iter++)
  {
    while(SPI_GetFlagStatus(QSPI, SPI_FLAG_RFNE) == RESET);
    *(buffer + iter) = SPI_ReadData(QSPI);
  }

  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_BUSY) != RESET);
  SPI_Cmd(QSPI, DISABLE);

  SPI_SpiModeConfig(QSPI, SPI_SpiMode_Standard);
}

/**
 * @brief  Writes data to one page (256B) through Quad I/O.
 * @note   The number of byte can't exceed the FLASH page remain size.
 * @param  address: FLASH's internal address to write to.
 * @param  buffer: pointer to the buffer containing the data to be written
 *         to the FLASH.
 * @param  length: number of bytes to write to the FLASH.
 * @return None
 */
void SpiFlash_QuadPageProgram(uint32_t address, const uint8_t* buffer, uint16_t length)
{
  SPI_EnhancedSpiInitTypeDef SPI_EnhancedSpiInitStructure;

  SpiFlash_WriteEnable();

  SPI_TransferModeConfig(QSPI, SPI_TransferMode_TxOnly);
  SPI_SpiModeConfig(QSPI, SPI_SpiMode_Quad);
  SPI_EnhancedSpiInitStructure.SPI_EnhancedSpiTransferType = SPI_EnhancedSpiTransferType_0;
  SPI_EnhancedSpiInitStructure.SPI_EnhancedSpiInstructionLength = SPI_EnhancedSpiInstructionLength_8b;
  SPI_EnhancedSpiInitStructure.SPI_EnhancedSpiAddressLength = SPI_EnhancedSpiAddressLength_24b;
  SPI_EnhancedSpiInitStructure.SPI_EnhancedSpiWaitCycles = 0;
  SPI_EnhancedSpiConfig(QSPI, &SPI_EnhancedSpiInitStructure);
  SPI_Cmd(QSPI, ENABLE);

  SPI_WriteData(QSPI, 0x32);    // Quad Input Page Program (32h)
  SPI_WriteData(QSPI, (uint16_t)(address >> 8));
  SPI_WriteData(QSPI, (uint8_t)(address));

  while(length--)
  {
    while(SPI_GetFlagStatus(QSPI, SPI_FLAG_TFNF) == RESET);
    SPI_WriteData(QSPI, *buffer++);
  }
  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_TFE) == RESET);
  while(SPI_GetFlagStatus(QSPI, SPI_FLAG_BUSY) != RESET);
  SPI_Cmd(QSPI, DISABLE);
  SPI_SpiModeConfig(QSPI, SPI_SpiMode_Standard);

  SpiFlash_WaitReady();
}

/**
 * @brief  Writes block of data to the FLASH through Quad I/O.
 * @param  address: FLASH's internal address to write to.
 * @param  buffer: pointer to the buffer containing the data to be written
 *         to the FLASH.
 * @param  length: number of bytes to write to the FLASH.
 * @return None
 */
void SpiFlash_QuadWriteBuffer(uint32_t address, const uint8_t* buffer, uint32_t length)
{
  uint32_t write_len;
  write_len = 256 - (address % 256);    /* calculate the start page remain size */
  if(length <= write_len) write_len = length;

  while(1)
  {
    SpiFlash_QuadPageProgram(address, buffer, write_len);
    if(length == write_len) break;    /* Write complete */
    else {
      buffer += write_len;
      address += write_len;

      length -= write_len;
      if(length > 256) write_len = 256;
      else write_len = length;
    }
  }
}


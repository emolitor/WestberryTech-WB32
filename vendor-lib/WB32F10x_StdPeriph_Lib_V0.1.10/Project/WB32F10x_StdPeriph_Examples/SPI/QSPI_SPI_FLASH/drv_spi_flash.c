/* Includes ------------------------------------------------------------------*/
#include "drv_spi_flash.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DUMMY_BYTE     0xFF

/* Private macro -------------------------------------------------------------*/
/* Select FLASH: Chip Select pin low */
#define SPI_FLASH_CS_LOW()    GPIO_ResetBits(GPIOA, GPIO_Pin_4)
/* Deselect FLASH: Chip Select pin high */
#define SPI_FLASH_CS_HIGH()   GPIO_SetBits(GPIOA, GPIO_Pin_4)

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Sends a byte through the SPI interface and return the byte received
 *         from the SPI bus.
 * @param  byte: byte to send.
 * @return The value of the received byte.
 */
uint8_t SPI_FLASH_ReadWriteByte(uint8_t Data)
{
  while (SPI_GetFlagStatus(QSPI, SPI_FLAG_TFE) == RESET);
  
  SPI_WriteData(QSPI, Data);
  
  while (SPI_GetFlagStatus(QSPI, SPI_FLAG_RFNE) == RESET);
  
  return SPI_ReadData(QSPI);
}

/**
 * @brief  Initializes the peripherals used by the SPI FLASH driver.
 * @param  None
 * @return None
 */
void SpiFlash_Init(void)
{
  SPI_InitTypeDef SPI_InitStructure;

  /* Enable the GPIOA and QSPI clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BMX1 |RCC_APB1Periph_GPIOA |RCC_APB1Periph_QSPI, ENABLE);

  /*
    PA4 (QSPI_NSS0)
    PA5 (QSPI_SCK)
    PA6 (QSPI_MI_IO1)
    PA7 (QSPI_MO_IO0)
  */
  GPIO_Init(GPIOA, GPIO_Pin_4, GPIO_MODE_OUT |GPIO_OTYPE_PP |GPIO_PUPD_NOPULL |GPIO_SPEED_HIGH);
  SPI_FLASH_CS_HIGH();
  GPIO_Init(GPIOA, GPIO_Pin_5 |GPIO_Pin_6 |GPIO_Pin_7, GPIO_MODE_AF |GPIO_OTYPE_PP |GPIO_PUPD_NOPULL |GPIO_SPEED_HIGH |GPIO_AF5);

  /* SPI configuration */
  SPI_DeInit(QSPI);
  SPI_InitStructure.SPI_TransferMode = SPI_TransferMode_TxAndRx;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_BaudRatePrescaler = 8;
  SPI_InitStructure.SPI_FrameFormat = SPI_FrameFormat_SPI;
  SPI_Init(QSPI, &SPI_InitStructure);
  SPI_NSSConfig(QSPI, SPI_NSS_0, ENABLE);
  SPI_Cmd(QSPI, ENABLE);
}

/**
 * @brief  Read JEDEC ID (9Fh).
 * @param  None
 * @return 24bit JEDEC ID (Eg. the w25q32 id is 0xEF4016)
 */
uint32_t SpiFlash_ReadIdentification(void)
{
  uint32_t jedec_id = 0;

  /* Select FLASH: Chip Select pin low */
  SPI_FLASH_CS_LOW();

  SPI_FLASH_ReadWriteByte(0x9F);  // Read JEDEC ID (9Fh)
  jedec_id = SPI_FLASH_ReadWriteByte(DUMMY_BYTE);
  jedec_id <<= 8;
  jedec_id |= SPI_FLASH_ReadWriteByte(DUMMY_BYTE);
  jedec_id <<= 8;
  jedec_id |= SPI_FLASH_ReadWriteByte(DUMMY_BYTE);

  /* Deselect FLASH: Chip Select pin high */
  SPI_FLASH_CS_HIGH();

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

  /* Select FLASH: Chip Select pin low */
  SPI_FLASH_CS_LOW();

  SPI_FLASH_ReadWriteByte(0x90);    // Read Manufacturer / Device ID (90h)
  SPI_FLASH_ReadWriteByte(0x00);
  SPI_FLASH_ReadWriteByte(0x00);
  SPI_FLASH_ReadWriteByte(0x00);

  mid_did = SPI_FLASH_ReadWriteByte(DUMMY_BYTE);
  mid_did <<= 8;
  mid_did |= SPI_FLASH_ReadWriteByte(DUMMY_BYTE);

  /* Deselect FLASH: Chip Select pin high */
  SPI_FLASH_CS_HIGH();

  return mid_did;
}

/**
 * @brief  Read Unique ID (4Bh)
 * @param  uid_buf: pointer to the buffer used to receive the unique id.
 * @param  length: number of bytes to receive.
 * @return None
 */
void SpiFlash_ReadUniqueID(uint8_t* uid_buf, uint32_t length)
{
  uint32_t idx;

  /* Select FLASH: Chip Select pin low */
  SPI_FLASH_CS_LOW();

  SPI_FLASH_ReadWriteByte(0x4B);    // Read Unique ID (4Bh)
  SPI_FLASH_ReadWriteByte(0x00);
  SPI_FLASH_ReadWriteByte(0x00);
  SPI_FLASH_ReadWriteByte(0x00);
  SPI_FLASH_ReadWriteByte(0x00);

  for (idx = 0; idx < length; idx++)
  {
    uid_buf[idx] = SPI_FLASH_ReadWriteByte(DUMMY_BYTE);
  }

  /* Deselect FLASH: Chip Select pin high */
  SPI_FLASH_CS_HIGH();
}

/**
 * @brief  Read Status Register-1 (05h)
 * @param  None
 * @return Status Register-1
 */
uint8_t SpiFlash_ReadStatusReg1(void)
{
  uint8_t status = 0;

  /* Select FLASH: Chip Select pin low */
  SPI_FLASH_CS_LOW();

  SPI_FLASH_ReadWriteByte(0x05);    // Read Status Register-1 (05h)
  status = SPI_FLASH_ReadWriteByte(DUMMY_BYTE);

  /* Deselect FLASH: Chip Select pin high */
  SPI_FLASH_CS_HIGH();

  return status;
}

/**
 * @brief  Read Status Register-2 (35h)
 * @param  None
 * @return Status Register-2
 */
uint8_t SpiFlash_ReadStatusReg2(void)
{
  uint8_t status = 0;

  /* Select FLASH: Chip Select pin low */
  SPI_FLASH_CS_LOW();

  SPI_FLASH_ReadWriteByte(0x35);    // Read Status Register-2 (35h)
  status = SPI_FLASH_ReadWriteByte(DUMMY_BYTE);

  /* Deselect FLASH: Chip Select pin high */
  SPI_FLASH_CS_HIGH();

  return status;
}

/**
 * @brief  Read Status Register-3 (15h)
 * @param  None
 * @return Status Register-3
 */
uint8_t SpiFlash_ReadStatusReg3(void)
{
  uint8_t status = 0;
  
  /* Select FLASH: Chip Select pin low */
  SPI_FLASH_CS_LOW();

  SPI_FLASH_ReadWriteByte(0x15);    // Read Status Register-3 (15h)
  status = SPI_FLASH_ReadWriteByte(DUMMY_BYTE);

  /* Deselect FLASH: Chip Select pin high */
  SPI_FLASH_CS_HIGH();

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
  /* Select FLASH: Chip Select pin low */
  SPI_FLASH_CS_LOW();

  SPI_FLASH_ReadWriteByte(0x06);    // Write Enable (06h)

  /* Deselect FLASH: Chip Select pin high */
  SPI_FLASH_CS_HIGH();
}

/**
 * @brief  Write Status Register (01h)
 * @param  value: The data to write.
 * @return None
 */
void SpiFlash_WriteStatusReg(uint16_t value)
{
  SpiFlash_WriteEnable();

  /* Select FLASH: Chip Select pin low */
  SPI_FLASH_CS_LOW();

  SPI_FLASH_ReadWriteByte(0x01);    // Write Status Register (01h)
  SPI_FLASH_ReadWriteByte((uint8_t)(value));
  SPI_FLASH_ReadWriteByte((uint8_t)(value >> 8));

  /* Deselect FLASH: Chip Select pin high */
  SPI_FLASH_CS_HIGH();

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

  /* Select FLASH: Chip Select pin low */
  SPI_FLASH_CS_LOW();

  SPI_FLASH_ReadWriteByte(0x01);    // Write Status Register-1 (01h)
  SPI_FLASH_ReadWriteByte(value);

  /* Deselect FLASH: Chip Select pin high */
  SPI_FLASH_CS_HIGH();

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

  /* Select FLASH: Chip Select pin low */
  SPI_FLASH_CS_LOW();

  SPI_FLASH_ReadWriteByte(0x31);    // Write Status Register-2 (31h)
  SPI_FLASH_ReadWriteByte(value);

  /* Deselect FLASH: Chip Select pin high */
  SPI_FLASH_CS_HIGH();

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
  /* Select FLASH: Chip Select pin low */
  SPI_FLASH_CS_LOW();

  SPI_FLASH_ReadWriteByte(0x03);    // Read Data (03h)
  SPI_FLASH_ReadWriteByte((uint8_t)(address >> 16));
  SPI_FLASH_ReadWriteByte((uint8_t)(address >> 8));
  SPI_FLASH_ReadWriteByte((uint8_t)(address));

  while(length--)
  {
    *buffer = SPI_FLASH_ReadWriteByte(DUMMY_BYTE);  /* Read a byte from the FLASH */
    buffer++;
  }

  /* Deselect FLASH: Chip Select pin high */
  SPI_FLASH_CS_HIGH();
}

/**
 * @brief  Reads a block of data from the FLASH at Higher Speed.
 * @param  address: FLASH's internal address to read from.
 * @param  buffer: pointer to the buffer that receives the data read from the FLASH.
 * @param  length: number of bytes to read from the FLASH.
 * @return None
 */
void SpiFlash_FastRead(uint32_t address, uint8_t* buffer, uint32_t length)
{
  /* Select FLASH: Chip Select pin low */
  SPI_FLASH_CS_LOW();

  SPI_FLASH_ReadWriteByte(0x0B);    // Fast Read (0Bh)
  SPI_FLASH_ReadWriteByte((uint8_t)(address >> 16));
  SPI_FLASH_ReadWriteByte((uint8_t)(address >> 8));
  SPI_FLASH_ReadWriteByte((uint8_t)(address));
  SPI_FLASH_ReadWriteByte(DUMMY_BYTE);

  while(length--)
  {
    *buffer = SPI_FLASH_ReadWriteByte(DUMMY_BYTE);  /* Read a byte from the FLASH */
    buffer++;
  }

  /* Deselect FLASH: Chip Select pin high */
  SPI_FLASH_CS_HIGH();
}

/**
 * @brief  Erases the entire FLASH.
 * @param  None
 * @return None
 */
void SpiFlash_ChipErase(void)
{
  SpiFlash_WriteEnable();
  
  /* Select FLASH: Chip Select pin low */
  SPI_FLASH_CS_LOW();
  SPI_FLASH_ReadWriteByte(0xC7);    // Chip Erase (C7h)

  /* Deselect FLASH: Chip Select pin high */
  SPI_FLASH_CS_HIGH();
  
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

  /* Select FLASH: Chip Select pin low */
  SPI_FLASH_CS_LOW();

  SPI_FLASH_ReadWriteByte(0x20);    // Sector Erase (20h)
  SPI_FLASH_ReadWriteByte((uint8_t)(address >> 16));
  SPI_FLASH_ReadWriteByte((uint8_t)(address >> 8));
  SPI_FLASH_ReadWriteByte((uint8_t)(address));

  /* Deselect FLASH: Chip Select pin high */
  SPI_FLASH_CS_HIGH();

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

  /* Select FLASH: Chip Select pin low */
  SPI_FLASH_CS_LOW();

  SPI_FLASH_ReadWriteByte(0x02);    // Page Program (02h)
  SPI_FLASH_ReadWriteByte((uint8_t)(address >> 16));
  SPI_FLASH_ReadWriteByte((uint8_t)(address >> 8));
  SPI_FLASH_ReadWriteByte((uint8_t)(address));

  while(length--)
  {
    SPI_FLASH_ReadWriteByte(*buffer++);
  }

  /* Deselect FLASH: Chip Select pin high */
  SPI_FLASH_CS_HIGH();

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


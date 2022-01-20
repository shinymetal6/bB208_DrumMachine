/*
 * w25qxx.h
 *
 *  Created on: Oct 18, 2021
 *      Author: fil
 */

#ifndef SRC_BB2XX_BSP_COMPONENTS_W25QXX_H_
#define SRC_BB2XX_BSP_COMPONENTS_W25QXX_H_

#define 	W25Q_DUMMY_BYTE 		0xA5
//#define		READ_COMMAND			0x03
#define		FAST_READ_COMMAND		0x0B
#define		WRITE_COMMAND			0x02
#define		WRITE_DISABLE_COMMAND	0x04
#define		WRITE_ENABLE_COMMAND	0x06
#define		READ_ID_COMMAND			0x9f
#define		GETSTATUS_COMMAND		0x05
#define		SECTOR_ERASE_COMMAND	0x20
#define		BLOCK64K_ERASE_COMMAND	0xD8
#define		CHIP_ERASE_COMMAND		0xc7

extern	SPI_HandleTypeDef 	hspi4;
#define	FLASH_SPI_PORT		hspi4

#define		F128			0x18
#define		F256			0x19
#define		PAGE_SIZE		256
#define		SECTOR_SIZE		4096
#define		F128_SIZE		(1024*1024*16)
#define		F128_NUMSECTORS	(F128_SIZE/SECTOR_SIZE)

#define		SPI_TIMEOUT		10000

extern	uint32_t w25qxx_ReadID(void);
extern	void w25qxx_EraseSector(uint32_t Sector);
extern	void w25qxx_EraseMultiSector(uint32_t Sector, uint32_t SectorNum);
extern	void w25qxx_EraseBlock64k(uint32_t Address);
extern	void w25qxx_WriteBytes(uint8_t* pBuffer, uint32_t Address, uint32_t size);
extern	void w25qxx_ReadBytesDMA(uint8_t* pBuffer, uint32_t Address, uint32_t size);
extern	void w25qxx_ReadBytes(uint8_t* pBuffer, uint32_t Address, uint32_t size);
extern	uint32_t	w25qxx_AddressToSector(uint32_t	Address);
extern	uint32_t	w2qxx_SectorToAddress(uint32_t	Sector);
extern	void w25qxx_SPI_RxEnd_Callback(void);
extern	uint8_t w25qxx_PollDmaTransferEnd(void);
extern	void w25qxx_SetDmaTransferInProgress(void);

#endif /* SRC_BB2XX_BSP_COMPONENTS_W25QXX_H_ */

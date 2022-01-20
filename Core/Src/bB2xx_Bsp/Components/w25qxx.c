/*
 * w25qxx.c
 *
 *  Created on: Oct 18, 2021
 *      Author: fil
 */


#include "main.h"
#include "../Core/bB2xx_System.h"
#include "w25qxx.h"

static uint8_t	w2qxx_SpiTX(uint8_t	Data)
{
uint8_t	ret;
	HAL_SPI_TransmitReceive(&FLASH_SPI_PORT,&Data,&ret,1,100);
	return ret;
}

static void w2qxx_WaitForWriteEnd(void)
{
uint8_t		StatusRegister1;
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_RESET);
	w2qxx_SpiTX(GETSTATUS_COMMAND);
	do
	{
		StatusRegister1 = w2qxx_SpiTX(W25Q_DUMMY_BYTE);
		HAL_Delay(1);
	}
	while ((StatusRegister1 & 0x01) == 0x01);
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_SET);
}

static void w2qxx_WriteEnable(void)
{
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_RESET);
	w2qxx_SpiTX(WRITE_ENABLE_COMMAND);
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_SET);
}

void w2qxx_WriteDisable(void)
{
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_RESET);
	w2qxx_SpiTX(WRITE_DISABLE_COMMAND);
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_SET);
}

uint32_t	w2qxx_SectorToAddress(uint32_t	Sector)
{
	return Sector * SECTOR_SIZE;
}

uint32_t	w25qxx_AddressToSector(uint32_t	Address)
{
	return Address / SECTOR_SIZE;
}

static uint32_t send_cmd_addr(uint8_t cmd,uint32_t Address)
{
uint32_t	ret = 0;
	w2qxx_SpiTX(cmd);
	if ( SystemParameters.flash_capacity == F256 )
		ret = w2qxx_SpiTX((Address & 0xFF000000) >> 24) << 24;
	ret |= w2qxx_SpiTX((Address & 0xFF0000) >> 16) << 16;
	ret |= w2qxx_SpiTX((Address & 0xFF00) >> 8) << 8;
	ret |= w2qxx_SpiTX(Address & 0xFF);
	return ret;
}

void w25qxx_EraseSector(uint32_t Sector)
{
uint32_t	SectorAddr;
	SectorAddr = Sector * SECTOR_SIZE;
	w2qxx_WriteEnable();
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_RESET);
	send_cmd_addr(SECTOR_ERASE_COMMAND,SectorAddr);
	HAL_Delay(1);
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_SET);
	HAL_Delay(50);
	w2qxx_WaitForWriteEnd();
	HAL_Delay(50);
}

void w25qxx_EraseBlock64k(uint32_t Address)
{
	w2qxx_WriteEnable();
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_RESET);
	send_cmd_addr(BLOCK64K_ERASE_COMMAND,Address);
	HAL_Delay(1);
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_SET);
	HAL_Delay(50);
	w2qxx_WaitForWriteEnd();
	HAL_Delay(50);
}

void w25qxx_EraseMultiSector(uint32_t Sector, uint32_t SectorNum)
{
uint32_t	i;
	for(i=0;i<SectorNum;i++)
		w25qxx_EraseSector(Sector+i);
}

void w25qxx_ReadBytes(uint8_t* pBuffer, uint32_t Address, uint32_t size)
{
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_RESET);
	//send_cmd_addr(READ_COMMAND,Address);
	send_cmd_addr(FAST_READ_COMMAND,Address);
	w2qxx_SpiTX(0);
	HAL_SPI_Receive(&FLASH_SPI_PORT,pBuffer,size,2000);
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_SET);
	//bB2xx_uSecDelay(50);
}

static void w25qxx_WritePage(uint8_t* pBuffer, uint32_t Address, uint32_t size)
{
	w2qxx_WriteEnable();
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_RESET);
	send_cmd_addr(WRITE_COMMAND,Address);
	HAL_SPI_Transmit(&FLASH_SPI_PORT,pBuffer,size,100);
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_SET);
	HAL_Delay(3);
	w2qxx_WaitForWriteEnd();
}

void w25qxx_WriteBytes(uint8_t* pBuffer, uint32_t Address, uint32_t size)
{
	while(size > PAGE_SIZE)
	{
		w25qxx_WritePage(pBuffer, Address, PAGE_SIZE);
		size -= PAGE_SIZE;
		Address += PAGE_SIZE;
		pBuffer += PAGE_SIZE;
	}
	if ( size > 0 )
		w25qxx_WritePage(pBuffer, Address, size);
}

uint32_t w25qxx_ReadID(void)
{
uint32_t fake_addr, ret_val;
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_RESET);
	w2qxx_SpiTX(READ_ID_COMMAND);
	fake_addr = (W25Q_DUMMY_BYTE << 24 )|(W25Q_DUMMY_BYTE << 16 )|(W25Q_DUMMY_BYTE << 8 )|(W25Q_DUMMY_BYTE << 0 );
	ret_val = send_cmd_addr(READ_ID_COMMAND,fake_addr) >> 8;
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_SET);
	return ret_val;
}

uint8_t w2qxx_PollChipErase(void)
{
uint8_t		StatusRegister1; // 1 if operation in progress, 0 if finished
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_RESET);
	w2qxx_SpiTX(GETSTATUS_COMMAND);
	StatusRegister1 = w2qxx_SpiTX(W25Q_DUMMY_BYTE);
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_SET);
	return StatusRegister1;
//	while ((StatusRegister1 & 0x01) == 0x01);
}

void w2qxx_ChipErase(void)
{
	w2qxx_WriteEnable();
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_RESET);
	w2qxx_SpiTX(CHIP_ERASE_COMMAND);
	HAL_GPIO_WritePin(FLASH_SS_GPIO_Port,FLASH_SS_Pin,GPIO_PIN_SET);
	HAL_Delay(1);
}


/*
 * drum_flash.c
 *
 *  Created on: Dec 16, 2021
 *      Author: fil
 */


#include "main.h"
#include "drum.h"
#include "drum_flash.h"
#include "enc_btns_leds.h"
#include "../Images/icons_memory.h"
#include <string.h>
#include <stdio.h>

RAMD3_RAM_AREA	__attribute__ ((aligned (16)))	UserParameters_TypeDef		UserParameters;

void drum_flash_get_usrparams(void)
{
	bzero((uint8_t *)&UserParameters,sizeof(UserParameters));
	w25qxx_ReadBytes((uint8_t *)&UserParameters, USRPARAMS_PTR, sizeof(UserParameters));
	Delay_Value_Draw(0);
	Delay_Weight_Draw(0);
	BPM_Value_Draw(0);
}

void drum_flash_store_usrparams(void)
{
	LD1_OnOff(LED_ON);
	w25qxx_EraseSector(w25qxx_AddressToSector(USRPARAMS_PTR));
	w25qxx_WriteBytes((uint8_t *)&UserParameters, USRPARAMS_PTR, sizeof(UserParameters));
	HAL_Delay(10);
	drum_flash_get_usrparams();
	LD1_OnOff(LED_OFF);
}

void drum_flash_store_digits( void )
{
	LD1_OnOff(LED_ON);
	w25qxx_EraseMultiSector(w25qxx_AddressToSector(BLUE_DIGIT_TYPE1_PTR),WHOLE_DIGIT_SECTORS);
	HAL_Delay(10);
	w25qxx_WriteBytes((uint8_t *)&normal_digits_22x40[0][0], BLUE_DIGIT_TYPE1_PTR, DIGITS_SIZE*10);
	LD1_OnOff(LED_OFF);
	HAL_Delay(100);
	LD1_OnOff(LED_ON);
	w25qxx_EraseMultiSector(w25qxx_AddressToSector(RED_DIGIT_TYPE1_PTR),WHOLE_DIGIT_SECTORS);
	HAL_Delay(10);
	w25qxx_WriteBytes((uint8_t *)&hilight_digits_22x40[0][0], RED_DIGIT_TYPE1_PTR, DIGITS_SIZE*10);
	LD1_OnOff(LED_OFF);
	HAL_Delay(100);
	LD1_OnOff(LED_ON);
	w25qxx_EraseMultiSector(w25qxx_AddressToSector(BLUE_DIGIT_TYPE2_PTR),WHOLE_DIGIT_SECTORS);
	HAL_Delay(10);
	w25qxx_WriteBytes((uint8_t *)&normal_digits_16x29[0][0], BLUE_DIGIT_TYPE2_PTR, DIGITS_SIZE*10);
	LD1_OnOff(LED_OFF);
	HAL_Delay(100);
	LD1_OnOff(LED_ON);
	w25qxx_EraseMultiSector(w25qxx_AddressToSector(RED_DIGIT_TYPE2_PTR),WHOLE_DIGIT_SECTORS);
	HAL_Delay(10);
	w25qxx_WriteBytes((uint8_t *)&hilight_digits_16x29[0][0], RED_DIGIT_TYPE2_PTR, DIGITS_SIZE*10);
	HAL_Delay(100);
	LD1_OnOff(LED_OFF);
}

void drum_flash_load_digits( void )
{
	w25qxx_ReadBytes((uint8_t *)&normal_digits_22x40[0],  BLUE_DIGIT_TYPE1_PTR, WHOLE_DIGIT_SIZE);
	w25qxx_ReadBytes((uint8_t *)&hilight_digits_22x40[0], RED_DIGIT_TYPE1_PTR, WHOLE_DIGIT_SIZE);
	w25qxx_ReadBytes((uint8_t *)&normal_digits_16x29[0],  BLUE_DIGIT_TYPE2_PTR, WHOLE_DIGIT_SIZE);
	w25qxx_ReadBytes((uint8_t *)&hilight_digits_16x29[0], RED_DIGIT_TYPE2_PTR, WHOLE_DIGIT_SIZE);
}

void drum_flash_store_icons( void )
{
uint8_t		icon_number;

	for(icon_number=0;icon_number<ICON1_NUMBER;icon_number++)
	{
		LD1_OnOff(LED_ON);
		w25qxx_EraseSector(w25qxx_AddressToSector(ICON_BOTH_PTR+(icon_number*SECTOR_SIZE)));
		HAL_Delay(10);
		w25qxx_WriteBytes((uint8_t *)&icons_50x20_normal[icon_number][0], ICON_BOTH_PTR+(icon_number*SECTOR_SIZE), SECTOR_SIZE);
		LD1_OnOff(LED_OFF);
		HAL_Delay(100);
	}
}

void drum_flash_load_icons( void )
{
uint8_t		icon_number;

	for(icon_number=0;icon_number<ICON1_NUMBER;icon_number++)
	{
		bzero(icons_50x20_normal[icon_number],sizeof(icons_50x20_normal[icon_number]));
		w25qxx_ReadBytes((uint8_t *)&icons_50x20_normal[icon_number],  ICON_BOTH_PTR+(icon_number*SECTOR_SIZE), ICON1_SIZE-3*ICON1_W);
	}
}

void drum_flash_get_usrsequence(void)
{
}

void drum_flash_load(void)
{
	if ( SystemParameters.valid_params == VALID_PARAMS_FLAG )
	{
		drum_flash_load_digits();
		drum_flash_load_icons();
		drum_flash_get_usrparams();
	}
	return;
}

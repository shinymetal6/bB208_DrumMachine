/*
 * bB2xx_flash.c
 *
 *  Created on: Oct 29, 2021
 *      Author: fil
 */

#include "main.h"
#include "bB2xx_flash.h"
#include <string.h>
#include <stdio.h>

uint32_t				flash_id;

uint8_t bB2xx_flash_identify(void)
{
	flash_id = w25qxx_ReadID();
	flash_id = w25qxx_ReadID();
	flash_id = w25qxx_ReadID();
	SystemParameters.flash_capacity = (uint8_t )(flash_id & 0xff);
	if ( SystemParameters.flash_capacity != F128 )
		return 1;
	return 0;
}

uint8_t bB2xx_flash_init(void)
{
uint8_t	ret_val;
	ret_val = bB2xx_flash_identify();
	w25qxx_ReadBytes((uint8_t *)&SystemParameters, PARAMS_PTR_F128, sizeof(SystemParameters));
	return ret_val;
}

void bB2xx_flash_store_sysparams(void)
{
	w25qxx_EraseSector(w25qxx_AddressToSector(PARAMS_PTR_F128));
	w25qxx_WriteBytes((uint8_t *)&SystemParameters, PARAMS_PTR_F128, sizeof(SystemParameters));
	HAL_Delay(10);
	w25qxx_ReadBytes((uint8_t *)&SystemParameters, PARAMS_PTR_F128, sizeof(SystemParameters));
}

void bB2xx_flash_get_sysparams(void)
{
	w25qxx_ReadBytes((uint8_t *)&SystemParameters, PARAMS_PTR_F128, sizeof(SystemParameters));
	if (( SystemParameters.id[0] == 'b') && ( SystemParameters.id[1] == 'B') && ( SystemParameters.id[2] == '2')&& ( SystemParameters.flash_capacity == F128))
	{
		SystemParameters.valid_params = VALID_PARAMS_FLAG;
		return;
	}
	else
	{
		SystemParameters.id[0] = 'b';
		SystemParameters.id[1] = 'B';
		SystemParameters.id[2] = '2';
		SystemParameters.id[3] = 'x';
		SystemParameters.touch_x_minimum = 0;
		SystemParameters.touch_y_minimum = 0;
		SystemParameters.flash_capacity = F128;
		SystemParameters.valid_params = INVALID_PARAMS_FLAG;
		bB2xx_flash_store_sysparams();
	}
}

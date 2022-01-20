/*
 * adc.c
 *
 *  Created on: Dec 22, 2021
 *      Author: fil
 */

#include "main.h"
#include "adc.h"

NOCACHE_RAM	uint16_t adc_buffer[4];
void InitADC(void)
{
	HAL_ADC_Start_DMA(&CONTROL_DMA, (uint32_t *)adc_buffer, 4);
}

void GetControls(uint16_t *rvar)
{
	*rvar = adc_buffer[0];
	rvar++;
	*rvar = adc_buffer[1];
	rvar++;
	*rvar = adc_buffer[2];
	rvar++;
	*rvar = adc_buffer[3];
	rvar++;
}

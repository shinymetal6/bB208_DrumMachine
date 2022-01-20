/*
 * spdif.c
 *
 *  Created on: Nov 9, 2021
 *      Author: fil
 */

#include "main.h"
#include "spdif.h"


void InitSPDIF(void)
{
	uint32_t	i;

	for(i=0;i<SPDIF_SIGNAL_LEN;i+=2)
		spdif_buffer[i] = spdif_buffer[i+1] = i;
	HAL_SAI_Transmit_DMA(&SPDIF_OUT, (uint8_t *)&signal_out_spdif[0], SPDIF_SIGNAL_LEN);
}

static void SPDIF_IrqProcessSamples(void)
{
uint32_t	i,limitlow,limithi;
	if (( SystemVar.audio_flags & AUDIO_SPDIF_READY_FLAG) == AUDIO_SPDIF_READY_FLAG)
	{
		if (( SystemVar.audio_flags & AUDIO_SPDIF_HALF_FLAG) == AUDIO_SPDIF_HALF_FLAG)
			limitlow = 0;
		else
			limitlow = SPDIF_SIGNAL_LEN/2;
		limithi = limitlow + SPDIF_SIGNAL_LEN/2;;

		for(i=limitlow;i<limithi;i++)
			signal_out_spdif[i] = spdif_buffer[i];
		SystemVar.audio_flags &= ~AUDIO_SPDIF_READY_FLAG;
	}
}

void SPDIF_TxHalfCpltCallback(void)
{
	SystemVar.audio_flags |= (AUDIO_SPDIF_HALF_FLAG | AUDIO_SPDIF_READY_FLAG);
	SPDIF_IrqProcessSamples();
}

void SPDIF_TxCpltCallback(void)
{
	SystemVar.audio_flags &= ~AUDIO_SPDIF_HALF_FLAG;
	SystemVar.audio_flags |= AUDIO_SPDIF_READY_FLAG;
	SPDIF_IrqProcessSamples();
}




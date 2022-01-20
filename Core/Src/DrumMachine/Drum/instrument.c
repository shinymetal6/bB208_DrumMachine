/*
 * instrument.c
 *
 *  Created on: Dec 14, 2021
 *      Author: fil
 */

#include "main.h"
#include "drum_flash.h"
#include "../drum_machine.h"
#include "fatfs.h"
#include "instrument.h"
#include "drum_buffers.h"
#include "delay.h"
#include <strings.h>

Instrument_TypeDef		Instrument[NUM_INSTRUMENTS];

WAV_FormatTypeDef 		WavInfo;

uint16_t	buf0_t[DRUM_BUFFER_SIZE] , buf1_t[DRUM_BUFFER_SIZE];
uint32_t	err=0,called=0;


uint32_t InitInstruments(void)
{
uint8_t instrument_number;
	for(instrument_number=0;instrument_number<NUM_INSTRUMENTS;instrument_number++)
	{
		w25qxx_ReadBytes((uint8_t *)&Instrument[instrument_number], INSTRUMENT_SIZE*instrument_number, sizeof(Instrument[instrument_number]));
		if ( Instrument[instrument_number].FileFlag == FILE_FLAG_VALID )
		{
			Instrument[instrument_number].WavPtr = (INSTRUMENT_SIZE*instrument_number) + SECTOR_SIZE;
			Instrument[instrument_number].PlayedSize = 0;
			Instrument[instrument_number].Flags = 0;
			if ( instrument_number == 0 )
			{
				if ( Instrument[instrument_number].SampleRate == 44100 )
					SAMPLE_TIMER.Instance->ARR = 1811/2;
				if ( Instrument[instrument_number].SampleRate == 22050 )
					SAMPLE_TIMER.Instance->ARR = 1811;
			}
			w25qxx_ReadBytes((uint8_t *)&s_drum_buffers[instrument_number][0], Instrument[instrument_number].WavPtr+Instrument[instrument_number].PlayedSize, DRUM_BUFFER_SIZE*2);
		}
	}
	return 0;
}

void compare_buf_0_1(void)
{
uint32_t	i;
	called++;
	for(i=0;i<DRUM_BUFFER_SIZE;i++)
	{
		if ( s_drum_buffers[0][i] != s_drum_buffers[1][i])
		{
			err++;
			return;
		}
	}
}

uint32_t GetBufferLen(void)
{
	return DRUM_BUFFER_SIZE*2;
}


void mixer_out(void)
{
uint32_t		instrument_number,i;
float			sample_dac1,sample_dac2;

	for(i=0;i<DRUM_BUFFER_SIZE;i++)
	{
		sample_dac1 = sample_dac2 = 32767.0F;
		for(instrument_number=0;instrument_number<NUM_INSTRUMENTS;instrument_number++)
		{
			sample_dac1 += (((float )s_drum_buffers[instrument_number][i] / (float )Sequencer_Descriptor.number_of_instruments)*DrumMachineVar.volume);
		}
		intermediate_dac1[i] = (uint16_t )DelayLine(sample_dac1);
	}
}

void ProcessDACOut(void)
{
uint32_t		instrument_number,i;

 	//__disable_irq();
	for(instrument_number=0;instrument_number<NUM_INSTRUMENTS;instrument_number++)
	{
		if (( (Instrument[instrument_number].Flags & WAVPLAY_SAMPLE_ACTIVE) == WAVPLAY_SAMPLE_ACTIVE ) && ( Instrument[instrument_number].FileFlag == FILE_FLAG_VALID ))
		{
			if ( Instrument[instrument_number].PlayedSize >= Instrument[instrument_number].FileSize )
			{
				Instrument[instrument_number].PlayedSize = 0;
				Instrument[instrument_number].Flags &= ~WAVPLAY_SAMPLE_ACTIVE;
				for(i=0;i<DRUM_BUFFER_SIZE;i++)
					s_drum_buffers[instrument_number][i] = F_NO_AUDIO_OUT;
			}
			else
			{
				w25qxx_ReadBytes((uint8_t *)&s_drum_buffers[instrument_number][0], Instrument[instrument_number].WavPtr+Instrument[instrument_number].PlayedSize, DRUM_BUFFER_SIZE*2);
				Instrument[instrument_number].PlayedSize += DRUM_BUFFER_SIZE*2;
			}
		}
		else
		{
			for(i=0;i<DRUM_BUFFER_SIZE;i++)
				s_drum_buffers[instrument_number][i] = F_NO_AUDIO_OUT;
			Instrument[instrument_number].PlayedSize = 0;
		}
	}
	mixer_out();
 	//__enable_irq();
	return ;
}




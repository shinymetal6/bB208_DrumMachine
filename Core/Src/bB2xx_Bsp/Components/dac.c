/*
 * dac.c
 *
 *  Created on: Nov 9, 2021
 *      Author: fil
 */

#include "main.h"
#include "dac.h"
#include "sine_wave.h"


__weak uint32_t GetBufferLen(void)
{
	return DAC_WAVETABLE_SIZE;
}

__weak void ProcessDACOut(void)
{

}

__weak void InitDac(void)
{
	SystemVar.buffer_length = GetBufferLen();
	SystemVar.hbuffer_length = SystemVar.buffer_length / 2;
	HAL_DAC_Start_DMA(&DAC_DMA, DAC_CHANNEL_1, (uint32_t *)signal_out_dac1, SystemVar.buffer_length, DAC_ALIGN_12B_L);
	HAL_DAC_Start_DMA(&DAC_DMA, DAC_CHANNEL_2, (uint32_t *)signal_out_dac2, SystemVar.buffer_length, DAC_ALIGN_12B_L);
	HAL_TIM_Base_Start_IT(&SAMPLE_TIMER);
}

void DAC_IrqProcessSamples(void)
{
uint32_t	i,k,limitlow,limithi;

	HAL_GPIO_WritePin(FLAG_GPIO_Port, FLAG_Pin, GPIO_PIN_SET);
	if (( SystemVar.audio_flags & AUDIO_DAC_READY_FLAG) == AUDIO_DAC_READY_FLAG)
	{
		limitlow = (SystemVar.audio_flags & AUDIO_DAC_HALF_FLAG) ? 0 : SystemVar.hbuffer_length;
		limithi = limitlow + SystemVar.hbuffer_length;
		ProcessDACOut();

		for(i=limitlow,k=0;i<limithi;i++,k++)
		{
			signal_out_dac1[i] = intermediate_dac1[k];
		}
		SystemVar.audio_flags &= ~AUDIO_DAC_READY_FLAG;
	}
	SystemVar.audio_flags |= AUDIO_IRQ_FLAG;
	HAL_GPIO_WritePin(FLAG_GPIO_Port, FLAG_Pin, GPIO_PIN_RESET);
}


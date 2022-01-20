/*
 * i2s.c
 *
 *  Created on: Nov 10, 2021
 *      Author: fil
 */

#include "main.h"
#include "i2s.h"

void InitI2S(void)
{
uint32_t	i;
	for(i=0;i<I2S_SIGNAL_LEN;i+=2)
		i2s_buffer[i] = i2s_buffer[i+1] = i;
	HAL_I2S_Transmit_DMA(&I2S_OUT_DMA, (uint16_t *)signal_out_i2s, I2S_SIGNAL_LEN/2);
}

static void I2S_IrqProcessSamples(void)
{
uint32_t	i,limitlow,limithi;
	if (( SystemVar.audio_flags & AUDIO_I2S_READY_FLAG) == AUDIO_I2S_READY_FLAG)
	{
		if (( SystemVar.audio_flags & AUDIO_I2S_HALF_FLAG) == AUDIO_I2S_HALF_FLAG)
			limitlow = 0;
		else
			limitlow = I2S_SIGNAL_LEN/2;
		limithi = limitlow + I2S_SIGNAL_LEN/2;;

		for(i=limitlow;i<limithi;i++)
			signal_out_i2s[i] = i2s_buffer[i];
		SystemVar.audio_flags &= ~AUDIO_I2S_READY_FLAG;
	}
}

void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
	SystemVar.audio_flags |= (AUDIO_I2S_HALF_FLAG | AUDIO_I2S_READY_FLAG);
	I2S_IrqProcessSamples();
}

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
	SystemVar.audio_flags &= ~AUDIO_I2S_HALF_FLAG;
	SystemVar.audio_flags |= AUDIO_I2S_READY_FLAG;
	I2S_IrqProcessSamples();
}

void I2S_GetAudioBuf(void)
{
#ifdef USE_MDMA
	/*
	SystemVar.mdma_flags &= ~MDMA_COMPLETE_FLAG;
	SystemVar.mdma_flags |= MDMA_RUNNING_FLAG;
	*/
	HAL_MDMA_Start_IT(&DAC_TO_I2S_DMA, (uint32_t )&signal_out_dac1, (uint32_t )&i2s_buffer, 64, 1);
	HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON,PWR_SLEEPENTRY_WFI);
	HAL_MDMA_Start_IT(&DAC_TO_I2S_DMA, (uint32_t )&signal_out_dac2, (uint32_t )&i2s_buffer[1], 64, 1);
	HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON,PWR_SLEEPENTRY_WFI);
	/*
	SystemVar.mdma_flags &= ~MDMA_RUNNING_FLAG;
	SystemVar.mdma_flags |= MDMA_COMPLETE_FLAG;
	*/
#else
	uint32_t	i,k;
	for(i=0,k=0;i<I2S_SIGNAL_LEN;i+=2,k++)
	{
		i2s_buffer[i]   = signal_out_dac1[k];
		i2s_buffer[i+1] = signal_out_dac2[k];
	}
#endif
}


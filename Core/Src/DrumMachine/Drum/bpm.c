/*
 * bpm.c
 *
 *  Created on: Nov 17, 2021
 *      Author: fil
 */


#include "main.h"
#include "bpm.h"
#include "../Hmi/menus.h"

static void DrawValue(uint32_t 	h,uint32_t t,uint32_t u,uint8_t hilight)
{
	if ( hilight == 0 )
	{
		ILI9341_DrawBitmap(BPM_DIGIT_XPOSH,BPM_DIGIT_YPOS,(uint8_t *)&normal_digits_22x40[h]);
		ILI9341_DrawBitmap(BPM_DIGIT_XPOST,BPM_DIGIT_YPOS,(uint8_t *)&normal_digits_22x40[t]);
		ILI9341_DrawBitmap(BPM_DIGIT_XPOSU,BPM_DIGIT_YPOS,(uint8_t *)&normal_digits_22x40[u]);
	}
	else
	{
		ILI9341_DrawBitmap(BPM_DIGIT_XPOSH,BPM_DIGIT_YPOS,(uint8_t *)&hilight_digits_22x40[h]);
		ILI9341_DrawBitmap(BPM_DIGIT_XPOST,BPM_DIGIT_YPOS,(uint8_t *)&hilight_digits_22x40[t]);
		ILI9341_DrawBitmap(BPM_DIGIT_XPOSU,BPM_DIGIT_YPOS,(uint8_t *)&hilight_digits_22x40[u]);
	}
}

void BPM_Value_Draw(uint8_t hilight)
{
uint32_t 	h,t,u;  // hundreds,tens,units

	u = UserParameters.bpm%10;
	t = (UserParameters.bpm/10)%10;
	h = (UserParameters.bpm/100)%10;
	DrawValue(	h,t,u,hilight);
}

void BPM_ExtValue_Draw(void)
{
uint32_t 	h,t,u;  // hundreds,tens,units

	u = DrumMachineVar.measured_bpm%10;
	t = (DrumMachineVar.measured_bpm/10)%10;
	h = (DrumMachineVar.measured_bpm/100)%10;
	DrawValue(	h,t,u,0);
}

void BPM_Value_Change(void)
{
	if (( DrumMachineVar.encoder_flags & ENCODER_ROTATION_FLAG) == ENCODER_ROTATION_FLAG)
	{
		if (( DrumMachineVar.encoder_flags & ENCODER_INCREMENT_FLAG) == ENCODER_INCREMENT_FLAG)
		{
			UserParameters.bpm++;
			if ( UserParameters.bpm > MAX_BEAT )
				UserParameters.bpm = MAX_BEAT;
		}
		if (( DrumMachineVar.encoder_flags & ENCODER_DECREMENT_FLAG) == ENCODER_DECREMENT_FLAG)
		{
			UserParameters.bpm--;
			if ( UserParameters.bpm < MIN_BEAT )
				UserParameters.bpm = MIN_BEAT;
		}
	}
	BPM_Value_Draw(1);
}

void GetMeasuredBPM(void)
{
float	tim_val;
	tim_val = (float )TIM_BPM_MEASURE->CNT/ 66.67F;
	DrumMachineVar.measured_bpm = (uint32_t )( 60.0F * (60.0F/tim_val));
	TIM_BPM_MEASURE->CNT = 0;
}

void BPMEnableCapture(void)
{
	TIM_BPM_MEASURE->CR1 |= 1;
}

void BPMDisableCapture(void)
{
	TIM_BPM_MEASURE->CR1 &= ~1;
}

void BPMInit(void)
{
	HAL_TIM_Base_Start(&BPM_MEASURE_TIMER);
	TIM_BPM_MEASURE->CR1 &= ~1;
	TIM_BPM_MEASURE->CNT = 0;
}


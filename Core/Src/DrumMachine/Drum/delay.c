/*
 * delay.c
 *
 *  Created on: Nov 17, 2021
 *      Author: fil
 */


#include "main.h"
#include "delay.h"
#include "../Hmi/menus.h"

RAMD2_RAM_AREA	__attribute__ ((aligned (16))) float	delay_line[DELAY_LINE_SIZE];

void Delay_Value_Draw(uint8_t hilight)
{
uint32_t 	h,t,u;  // hundreds,tens,units

	u = UserParameters.delay_value%10;
	t = (UserParameters.delay_value/10)%10;
	h = (UserParameters.delay_value/100)%10;

	if ( hilight == 0 )
	{
		ILI9341_DrawBitmap(DLY_DIGIT_XPOSH,DLY_DIGIT_YPOS,(uint8_t *)&normal_digits_22x40[h]);
		ILI9341_DrawBitmap(DLY_DIGIT_XPOST,DLY_DIGIT_YPOS,(uint8_t *)&normal_digits_22x40[t]);
		ILI9341_DrawBitmap(DLY_DIGIT_XPOSU,DLY_DIGIT_YPOS,(uint8_t *)&normal_digits_22x40[u]);
	}
	else
	{
		ILI9341_DrawBitmap(DLY_DIGIT_XPOSH,DLY_DIGIT_YPOS,(uint8_t *)&hilight_digits_22x40[h]);
		ILI9341_DrawBitmap(DLY_DIGIT_XPOST,DLY_DIGIT_YPOS,(uint8_t *)&hilight_digits_22x40[t]);
		ILI9341_DrawBitmap(DLY_DIGIT_XPOSU,DLY_DIGIT_YPOS,(uint8_t *)&hilight_digits_22x40[u]);
	}
}

void Delay_Weight_Draw(uint8_t hilight)
{
uint32_t 	t,u;  // hundreds,tens,units

	u = UserParameters.delay_weight_int%10;
	t = (UserParameters.delay_weight_int/10)%10;

	if ( hilight == 0 )
	{
		ILI9341_DrawBitmap(DLYW_DIGIT_XPOST,DLYW_DIGIT_YPOS,(uint8_t *)&normal_digits_22x40[t]);
		ILI9341_DrawBitmap(DLYW_DIGIT_XPOSU,DLYW_DIGIT_YPOS,(uint8_t *)&normal_digits_22x40[u]);
	}
	else
	{
		ILI9341_DrawBitmap(DLYW_DIGIT_XPOST,DLYW_DIGIT_YPOS,(uint8_t *)&hilight_digits_22x40[t]);
		ILI9341_DrawBitmap(DLYW_DIGIT_XPOSU,DLYW_DIGIT_YPOS,(uint8_t *)&hilight_digits_22x40[u]);
	}
}


void Delay_Value_Change(void)
{
	if (( DrumMachineVar.encoder_flags & ENCODER_ROTATION_FLAG) == ENCODER_ROTATION_FLAG)
	{
		if (( DrumMachineVar.encoder_flags & ENCODER_INCREMENT_FLAG) == ENCODER_INCREMENT_FLAG)
		{
			UserParameters.delay_value++;
			if ( UserParameters.delay_value > 900 )
				UserParameters.delay_value = 900;
		}
		if (( DrumMachineVar.encoder_flags & ENCODER_DECREMENT_FLAG) == ENCODER_DECREMENT_FLAG)
		{
			UserParameters.delay_value--;
			if ( UserParameters.delay_value < 1 )
				UserParameters.delay_value = 1;
		}
	}
	DrumMachineVar.encoder_flags = 0;
	Delay_Value_Draw(1);
}

void Delay_Weight_Change(void)
{
	if (( DrumMachineVar.encoder_flags & ENCODER_ROTATION_FLAG) == ENCODER_ROTATION_FLAG)
	{
		if (( DrumMachineVar.encoder_flags & ENCODER_INCREMENT_FLAG) == ENCODER_INCREMENT_FLAG)
		{
			UserParameters.delay_weight_int++;
			if ( UserParameters.delay_weight_int > 99 )
				UserParameters.delay_weight_int = 99;
		}
		if (( DrumMachineVar.encoder_flags & ENCODER_DECREMENT_FLAG) == ENCODER_DECREMENT_FLAG)
		{
			UserParameters.delay_weight_int--;
			if ( UserParameters.delay_weight_int < 1 )
				UserParameters.delay_weight_int = 1;
		}
	}
	DrumMachineVar.encoder_flags = 0;
	UserParameters.delay_weight = (float )UserParameters.delay_weight_int / 100.0F;
	Delay_Weight_Draw(1);
}

float DelayLine( float sample )
{
float		out_sample = 0;
uint16_t	delay_extraction_pointer;

	delay_extraction_pointer = (UserParameters.delay_insertion_pointer - (UserParameters.delay_value*128)) & DELAY_POINTER_MASK;

	out_sample = (sample * (1.0F - UserParameters.delay_weight)) + (delay_line[delay_extraction_pointer] * UserParameters.delay_weight);
	if (( UserParameters.delay_type & DELAY_TYPE_FLANGER) == DELAY_TYPE_FLANGER)
		delay_line[UserParameters.delay_insertion_pointer] = sample;
	else if (( UserParameters.delay_type & DELAY_TYPE_ECHO) == DELAY_TYPE_ECHO )
		delay_line[DrumMachineVar.delay_insertion_pointer] = out_sample;
	else
		out_sample = sample;
	UserParameters.delay_insertion_pointer++;
	return out_sample;
}



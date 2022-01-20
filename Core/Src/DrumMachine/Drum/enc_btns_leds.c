/*
 * enc_btns_leds.c
 *
 *  Created on: Dec 22, 2021
 *      Author: fil
 */

#include "main.h"
#include "enc_btns_leds.h"


void encoder_rotation_callback(void)
{
	DrumMachineVar.last_encval = DrumMachineVar.encval;
	DrumMachineVar.encval = TIM4->CNT;
	if ( DrumMachineVar.last_encval != DrumMachineVar.encval)
	{
		DrumMachineVar.encoder_flags &= ~(ENCODER_INCREMENT_FLAG | ENCODER_DECREMENT_FLAG);
		if ( DrumMachineVar.last_encval < DrumMachineVar.encval)
			DrumMachineVar.encoder_flags |= ENCODER_INCREMENT_FLAG;
		else
			DrumMachineVar.encoder_flags |= ENCODER_DECREMENT_FLAG;
		DrumMachineVar.encoder_flags |= ENCODER_ROTATION_FLAG;
	}
}

void encoder_switch_callback(void)
{
	DrumMachineVar.encoder_flags |= ENCODER_SW_FLAG;
}

void sw1_callback(void)
{
	DrumMachineVar.buttons_flags |= SW1_PRESSED;
}

void sw2_callback(void)
{
	DrumMachineVar.buttons_flags |= SW2_PRESSED;
}

void LD1_OnOff(uint8_t value)
{
	  HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, value);
}

void LD7_OnOff(uint8_t value)
{
	  HAL_GPIO_WritePin(LD7_GPIO_Port, LD7_Pin, value);
}

void LD8_OnOff(uint8_t value)
{
	  HAL_GPIO_WritePin(LD8_GPIO_Port, LD8_Pin, value);
}

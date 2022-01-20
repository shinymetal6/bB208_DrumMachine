/*
 * sequencer.c
 *
 *  Created on: Sep 24, 2021
 *      Author: fil
 */

#include "main.h"
#include "sequencer.h"
#include "drum_flash.h"
#include "instrument.h"
#include "bpm.h"
#include "enc_btns_leds.h"
#include "../Hmi/menus.h"
#include "../Hmi/graph.h"

#include <string.h>

__attribute__ ((aligned (16)))  uint16_t sequencer_steps[SEQUENCER_MAX_SIZE];
__attribute__ ((aligned (16)))  Sequencer_DescriptorTypeDef	Sequencer_Descriptor;

extern	Instrument_TypeDef		Instrument[];

uint32_t	bpm2irq[MAX_BEAT];
uint8_t	ld1_status = 6;

void SequencerUpdateBeat(void)
{
uint32_t 	h,t,u;  // hundreds,tens,units
uint16_t	step;

	step =  DrumMachineVar.sequencer_step + 1;
	u = step%10;
	t = (step/10)%10;
	h = (step/100)%10;

	ILI9341_DrawBitmap(SEQ_CURRENTBEAT_DIGIT_XPOSH,SEQ_DIGIT_YPOS,(uint8_t *)&normal_digits_16x29[h]);
	ILI9341_DrawBitmap(SEQ_CURRENTBEAT_DIGIT_XPOST,SEQ_DIGIT_YPOS,(uint8_t *)&normal_digits_16x29[t]);
	ILI9341_DrawBitmap(SEQ_CURRENTBEAT_DIGIT_XPOSU,SEQ_DIGIT_YPOS,(uint8_t *)&normal_digits_16x29[u]);
}

void SequencerUpdateNumberOfBeat(void)
{
uint32_t 	h,t,u;  // hundreds,tens,units
uint16_t	len;

	len =  UserParameters.sequencer_length;
	u = len%10;
	t = (len/10)%10;
	h = (len/100)%10;

	ILI9341_DrawBitmap(SEQ_NUMBEAT_DIGIT_XPOSH,SEQ_DIGIT_YPOS,(uint8_t *)&normal_digits_16x29[h]);
	ILI9341_DrawBitmap(SEQ_NUMBEAT_DIGIT_XPOST,SEQ_DIGIT_YPOS,(uint8_t *)&normal_digits_16x29[t]);
	ILI9341_DrawBitmap(SEQ_NUMBEAT_DIGIT_XPOSU,SEQ_DIGIT_YPOS,(uint8_t *)&normal_digits_16x29[u]);

	ILI9341_WriteString(SEQ_BEATNUM_X,SEQ_BEATNUM_Y,"Beat",Font_7x10,ILI9341_WHITE,ILI9341_BLACK);
	ILI9341_WriteString(SEQ_OFBEAT_X,SEQ_OFBEAT_Y,"Of",Font_7x10,ILI9341_WHITE,ILI9341_BLACK);
}

uint8_t		instr[NUM_INSTRUMENTS];
void SequencerInit(void)
{
uint16_t	instrument,val,i;

	w25qxx_ReadBytes((uint8_t *)&Sequencer_Descriptor, USRSEQUENCE_PTR, sizeof(Sequencer_Descriptor));
	if ( strncmp((char *)Sequencer_Descriptor.header,"Seq_2xx",HEADER_LEN-1) == 0 )
	{
		UserParameters.bpm = Sequencer_Descriptor.sequencer_beat;
		DrumMachineVar.sequencer_preload = TIM_ARR_1_BPM / UserParameters.bpm;
		TIM_SEQUENCERL->ARR = DrumMachineVar.sequencer_preload;
		DrumMachineVar.delay_flags = Sequencer_Descriptor.delay_type;
		UserParameters.delay_value = Sequencer_Descriptor.delay_value;
		UserParameters.delay_weight_int = Sequencer_Descriptor.delay_weight;
		UserParameters.delay_weight = (float )UserParameters.delay_weight_int / 100.0F;

		UserParameters.sequencer_length = Sequencer_Descriptor.sequencer_length;
		DrumMachineVar.sequencer_counter = 0;
		DrumMachineVar.sequencer_step = 0;
		DrumMachineVar.sequencer_last_step = Sequencer_Descriptor.sequencer_length-1;

		Sequencer_Descriptor.number_of_instruments = 0;
		for(instrument=0;instrument<NUM_INSTRUMENTS;instrument++)
			instr[instrument] = 0;
		for(i=0;i<Sequencer_Descriptor.sequencer_length;i++)
		{
			for(instrument=0;instrument<NUM_INSTRUMENTS;instrument++)
			{
				val = 1 << instrument;
				if (( Sequencer_Descriptor.sequencer_steps[i] & val) != 0 )
					instr[instrument]++;
			}
		}

		for(instrument=0;instrument<NUM_INSTRUMENTS;instrument++)
		{
			if ( instr[instrument] != 0 )
				Sequencer_Descriptor.number_of_instruments++;
		}

		BPM_Value_Draw(0);
		Delay_Weight_Draw(0);
		Delay_Value_Draw(0);
		SequencerUpdateNumberOfBeat();
		SequencerUpdateBeat();
		HAL_TIM_Base_Start_IT(&TIM_SEQUENCER);
		TIM_SEQUENCERL->CR1 &= ~1;
		TIM_SEQUENCERL->CNT = 0;
		if ((DrumMachineVar.sequencer_flags & SEQUENCER_TRIGGEROUTLVL ) ==  SEQUENCER_TRIGGEROUTLVL )
			HAL_GPIO_WritePin(TRIGGER_OUT_GPIO_Port, TRIGGER_OUT_Pin, GPIO_PIN_RESET);
		else
			HAL_GPIO_WritePin(TRIGGER_OUT_GPIO_Port, TRIGGER_OUT_Pin, GPIO_PIN_SET);
	}
}

void SequencerRestart(void)
{
	LD1_OnOff(LED_OFF);
	DrumMachineVar.sequencer_counter = 0;
	DrumMachineVar.sequencer_step = 0;
	DrumMachineVar.sequencer_last_step = 0;
	DrumMachineVar.sequencer_flags |= SEQUENCER_ENABLE;
	GraphUpdateGraph();
	SequencerUpdateBeat();
	TIM_SEQUENCERL->CNT = 0;
	TIM_SEQUENCERL->CR1 |= 1;
}

void SequencerInternalStart(void)
{
	DrumMachineVar.sequencer_preload = TIM_ARR_1_BPM / UserParameters.bpm;
	TIM_SEQUENCERL->ARR = DrumMachineVar.sequencer_preload;
	SequencerRestart();
}

void SequencerInternalStop(void)
{
	GraphUpdateGraph();
	DrumMachineVar.sequencer_flags &= ~SEQUENCER_ENABLE;
	DrumMachineVar.sequencer_counter = 0;
	DrumMachineVar.sequencer_step = 0;
	DrumMachineVar.sequencer_last_step = 0;
	ld1_status = 0;
	LD1_OnOff(LED_OFF);
	TIM_SEQUENCERL->CR1 &= ~1;
	TIM_SEQUENCERL->CNT = 0;
	SequencerUpdateBeat();
}

void SequencerExternalStart(void)
{
	SequencerRestart();
}

void SequencerExternalStop(void)
{
	GraphUpdateGraph();
	DrumMachineVar.sequencer_flags &= ~SEQUENCER_ENABLE;
	DrumMachineVar.sequencer_counter = 0;
	DrumMachineVar.sequencer_step = 0;
	DrumMachineVar.sequencer_last_step = 0;
	ld1_status = 0;
	LD1_OnOff(LED_OFF);
	TIM_SEQUENCERL->CR1 &= ~1;
	TIM_SEQUENCERL->CNT = 0;
	SequencerUpdateBeat();
}

void SequencerCallback(void)
{
uint16_t	i , val;

	if (( DrumMachineVar.sequencer_flags & SEQUENCER_ENABLE ) == SEQUENCER_ENABLE )
	{
		if (Sequencer_Descriptor.sequencer_length != 0 )
		{
			for(i=0;i<NUM_INSTRUMENTS;i++)
			{
				val = 1 << i;
				if (( Sequencer_Descriptor.sequencer_steps[DrumMachineVar.sequencer_step] & val) != 0 )
				{
					Instrument[i].PlayedSize = 0;
					Instrument[i].Flags |= WAVPLAY_SAMPLE_ACTIVE;
				}
			}

			if ( DrumMachineVar.sequencer_step == DrumMachineVar.sequencer_trigout_sample )
			{
				if ((DrumMachineVar.sequencer_flags & SEQUENCER_TRIGGEROUTLVL ) ==  SEQUENCER_TRIGGEROUTLVL )
				{
					HAL_GPIO_WritePin(TRIGGER_OUT_GPIO_Port, TRIGGER_OUT_Pin, GPIO_PIN_SET);
					LD7_OnOff(LED_ON);
				}
				else
				{
					HAL_GPIO_WritePin(TRIGGER_OUT_GPIO_Port, TRIGGER_OUT_Pin, GPIO_PIN_RESET);
					LD7_OnOff(LED_OFF);
				}
			}
			else
			{
				if ((DrumMachineVar.sequencer_flags & SEQUENCER_TRIGGEROUTLVL ) ==  SEQUENCER_TRIGGEROUTLVL )
				{
					HAL_GPIO_WritePin(TRIGGER_OUT_GPIO_Port, TRIGGER_OUT_Pin, GPIO_PIN_RESET);
					LD7_OnOff(LED_OFF);
				}
				else
				{
					HAL_GPIO_WritePin(TRIGGER_OUT_GPIO_Port, TRIGGER_OUT_Pin, GPIO_PIN_SET);
					LD7_OnOff(LED_ON);
				}

			}
			DrumMachineVar.sequencer_step ++;
			if ( DrumMachineVar.sequencer_step >= UserParameters.sequencer_length )
			{
				DrumMachineVar.sequencer_step = 0;
				if ( (DrumMachineVar.system & SYSTEM_SEQUENCER_SSHOT ) == SYSTEM_SEQUENCER_SSHOT )
				{
					//DrumMachineVar.system &= ~SYSTEM_SEQUENCER_SSHOT;
					SequencerInternalStop();
					LD8_OnOff(LED_OFF);
					WriteStatusLine("Ready");
				}
			}
		}
	}
}

static void common_sequencer_callbacks(void)
{
	LD1_OnOff(LED_ON);
	ld1_status = 3;
	DrumMachineVar.sequencer_last_step = DrumMachineVar.sequencer_step;
	SequencerCallback();
	DrumMachineVar.sequencer_flags |= SEQUENCER_DONE;
}

static void gate_sequencer_callbacks(void)
{
	if (( DrumMachineVar.sequencer_flags & (SEQUENCER_GATEH | SEQUENCER_GATEL )) != 0)
	{
		if (( DrumMachineVar.sequencer_flags & SEQUENCER_GATEH) == SEQUENCER_GATEH)
		{
			if (HAL_GPIO_ReadPin(GATE_GPIO_Port,GATE_Pin) != 0 )
				common_sequencer_callbacks();
		}
		else
		{
			if (HAL_GPIO_ReadPin(GATE_GPIO_Port,GATE_Pin) == 0 )
				common_sequencer_callbacks();
		}
	}
	else
		common_sequencer_callbacks();
}

void sigin2_callbacks( uint16_t pin_value )
{
	if (( DrumMachineVar.system & SYSTEM_SEQUENCER_EXTERNAL ) == SYSTEM_SEQUENCER_EXTERNAL )
	{
		if ((DrumMachineVar.sequencer_flags & SEQUENCER_TRIGGERINH ) ==  SEQUENCER_TRIGGERINH)
		{
			if ( pin_value != 0)
			{
				gate_sequencer_callbacks();
				GetMeasuredBPM();
			}
		}
		if ((DrumMachineVar.sequencer_flags & SEQUENCER_TRIGGERINL ) ==  SEQUENCER_TRIGGERINL)
		{
			if ( pin_value == 0)
			{
				gate_sequencer_callbacks();
				GetMeasuredBPM();
			}
		}
	}
}

void ext_additional_timer_callbacks(TIM_HandleTypeDef *htim)
{
	if ( htim == &TIM_SEQUENCER )
	{
		if ((( DrumMachineVar.system & SYSTEM_SEQUENCER_INTERNAL ) == SYSTEM_SEQUENCER_INTERNAL ) || (( DrumMachineVar.system & SYSTEM_SEQUENCER_SSHOT ) == SYSTEM_SEQUENCER_SSHOT ) )
		{
			DrumMachineVar.sequencer_preload = TIM_ARR_1_BPM / UserParameters.bpm;
			TIM_SEQUENCERL->ARR = DrumMachineVar.sequencer_preload;
			gate_sequencer_callbacks();
		}
	}
	if ( htim == &DEBOUNCE_10MSEC)
	{
		if ( ld1_status != 0 )
		{
			ld1_status --;
			if ( ld1_status == 0 )
				LD1_OnOff(LED_OFF);
		}
	}
}


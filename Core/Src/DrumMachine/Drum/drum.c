/*
 * drum.c
 *
 *  Created on: Dec 16, 2021
 *      Author: fil
 */


#include "main.h"
#include "drum.h"
#include "drum_flash.h"
#include "enc_btns_leds.h"
#include "instrument.h"
#include "bpm.h"
#include "trigger.h"
#include "gate.h"
#include "../Hmi/graph.h"
#include "../Hmi/menus.h"
#include "../Hmi/vol_meter.h"

DrumMachineVar_TypeDef	DrumMachineVar;

static void InitApplicationTimers(void)
{
}

void InitDac(void)
{
	SystemVar.buffer_length = GetBufferLen();
	SystemVar.hbuffer_length = SystemVar.buffer_length / 2;
	HAL_DAC_Start_DMA(&DAC_DMA, DAC_CHANNEL_1, (uint32_t *)signal_out_dac1, SystemVar.buffer_length, DAC_ALIGN_12B_L);
	HAL_TIM_Base_Start_IT(&SAMPLE_TIMER);
}


void controls_callbacks( void )
{
	DrumMachineVar.volume_cntr++;
	if ( DrumMachineVar.volume_cntr == 3 )
	{
		DrumMachineVar.volume_flags |= VOLUME_FLAG_READY;
	}
	if ( DrumMachineVar.volume_cntr == 5 )
		DrumMachineVar.volume_cntr = 4;
}

static void Init_InitialVars(void)
{
	DrumMachineVar.sequencer_flags &= ~(SEQUENCER_TRIGGERINH | SEQUENCER_TRIGGERINL );
	DrumMachineVar.sequencer_flags |= SEQUENCER_TRIGGERINH;
	DrumMachineVar.sequencer_flags |= SEQUENCER_TRIGGEROUTLVL;
	DrumMachineVar.sequencer_mode &= ~SECMODE_SEQUENCER_EXTERNAL;
	DrumMachineVar.sequencer_mode |= SECMODE_SEQUENCER_LOOP;
}

static void Init_FinalVars(void)
{
	DrumMachineVar.system |= SYSTEM_MENU_INCDEC;
	LD1_OnOff(LED_OFF);
	LD7_OnOff(LED_OFF);
	LD8_OnOff(LED_OFF);
	DrumMachineVar.volume_cntr = 0;
	DrumMachineVar.last_volume_pot = 11;
	WriteStatusLine("Ready");
}

void application_init(void)
{

	CheckForUpdate();
	drum_flash_load();
	Init_InitialVars();
	ILI9341_Init();
	ILI9341_FillScreen(ILI9341_BLACK);
	InitBackLight();
	InitInstruments();
	MenuDisplayInit();
	BPMInit();
	SequencerInit();
	GraphInit();
	DrawInitialVolMeter();
	InitApplicationTimers();
	InitDac();
	//InitSPDIF();
	//InitI2S();
	Init_FinalVars();
	drum_flash_load_icons();

}

void application_loop(void)
{
	while(1)
	{
		HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON,PWR_SLEEPENTRY_WFI);
		if (( DrumMachineVar.buttons_flags & SW1_PRESSED ) == SW1_PRESSED)
		{
			if (( DrumMachineVar.sequencer_mode & SECMODE_SEQUENCER_EXTERNAL ) == 0 )
			{
				if (( DrumMachineVar.sequencer_flags & SEQUENCER_ENABLE ) == SEQUENCER_ENABLE )
				{
					SequencerRestart();
				}
			}
			DrumMachineVar.buttons_flags &= ~SW1_PRESSED;
		}

		if (( DrumMachineVar.buttons_flags & SW2_PRESSED ) == SW2_PRESSED)
		{
			if (( DrumMachineVar.sequencer_mode & SECMODE_SEQUENCER_EXTERNAL ) == 0 )
			{
				if (( DrumMachineVar.sequencer_flags & SEQUENCER_ENABLE ) != SEQUENCER_ENABLE )
				{
					SequencerInternalStart();
					LD8_OnOff(LED_ON);
				}
				else
				{
					SequencerInternalStop();
					LD8_OnOff(LED_OFF);
				}
			}
			if (( DrumMachineVar.sequencer_mode & SECMODE_SEQUENCER_EXTERNAL ) == SECMODE_SEQUENCER_EXTERNAL )
			{
				if (( DrumMachineVar.sequencer_flags & SEQUENCER_ENABLE ) != SEQUENCER_ENABLE )
				{
					DrumMachineVar.sequencer_flags |= SEQUENCER_ENABLE;
					SequencerExternalStart();
					LD8_OnOff(LED_ON);
				}
				else
				{
					DrumMachineVar.sequencer_flags &= ~SEQUENCER_ENABLE;
					SequencerExternalStop();
					LD8_OnOff(LED_OFF);
				}
			}
			DrumMachineVar.buttons_flags &= ~SW2_PRESSED;
		}

		if (( DrumMachineVar.sequencer_flags & SEQUENCER_DONE ) == SEQUENCER_DONE)
		{
			if (( DrumMachineVar.sequencer_mode & SECMODE_SEQUENCER_EXTERNAL ) == SECMODE_SEQUENCER_EXTERNAL )
			{
				BPMEnableCapture();
				BPM_ExtValue_Draw();
			}
			else
			{
				BPMDisableCapture();
			}
			SequencerUpdateBeat();
			GraphUpdateGraph();
			DrumMachineVar.sequencer_flags &= ~SEQUENCER_DONE;
		}
		if ( SysTimer100msecCheckExpired() != 0 )
		{
			if ( DrumMachineVar.uservalue_changed != 0 )
			{
				DrumMachineVar.uservalue_changed--;
				if ( DrumMachineVar.uservalue_changed == 0 )
				{
					drum_flash_store_usrparams();
				}
			}
			if (( DrumMachineVar.volume_flags & VOLUME_FLAG_READY ) == VOLUME_FLAG_READY)
			{
				GetControls((uint16_t *)&DrumMachineVar.rvar);
				DrumMachineVar.volume = (float)DrumMachineVar.rvar[VOLUME_POT] / 255.0F;
				DrumMachineVar.volume_pot = (uint8_t )(DrumMachineVar.volume*10.0F);
				DrumMachineVar.detune_pot = (uint8_t )(((float)DrumMachineVar.rvar[DETUNE_POT] / 255.0F)*10.0F);
				if ( DrumMachineVar.volume_pot != DrumMachineVar.last_volume_pot)
				{
					DrawVolMeterValue(DrumMachineVar.volume_pot);
				}
				DrumMachineVar.last_volume_pot = DrumMachineVar.volume_pot;
			}
			SysTimer100msecClearFlag();
		}
		if (( DrumMachineVar.encoder_flags & ENCODER_ROTATION_FLAG ) == ENCODER_ROTATION_FLAG)
		{
			if (( DrumMachineVar.system & SYSTEM_MENU_INCDEC) == SYSTEM_MENU_INCDEC)
				MenuEncoderNavigate();
			else
			{
				if (( DrumMachineVar.encoder_navigation & ENCNAV_BPM_INCDEC) == ENCNAV_BPM_INCDEC)
				{
					BPM_Value_Change();
				}
				if (( DrumMachineVar.encoder_navigation & ENCNAV_DELAYVAL_INCDEC) == ENCNAV_DELAYVAL_INCDEC)
				{
					Delay_Value_Change();
				}
				if (( DrumMachineVar.encoder_navigation & ENCNAV_DELAYWEIGHT_INCDEC) == ENCNAV_DELAYWEIGHT_INCDEC)
				{
					Delay_Weight_Change();
				}
				if (( DrumMachineVar.encoder_navigation & ENCNAV_MODE_INCDEC) == ENCNAV_MODE_INCDEC)
				{
					SequencerModeChange();
				}
				if (( DrumMachineVar.encoder_navigation & ENCNAV_TRIGINSRC_INCDEC) == ENCNAV_TRIGINSRC_INCDEC)
				{
					TriggerInMode_Change();
				}
				if (( DrumMachineVar.encoder_navigation & ENCNAV_TRIGOUTLVL_INCDEC) == ENCNAV_TRIGOUTLVL_INCDEC)
				{
					TriggerOutLevel_Change();
				}
				if (( DrumMachineVar.encoder_navigation & ENCNAV_TRIGOUTBEAT_INCDEC) == ENCNAV_TRIGOUTBEAT_INCDEC)
				{
					TriggerOutPosition_Change();
				}
				if (( DrumMachineVar.encoder_navigation & ENCNAV_GATE_INCDEC) == ENCNAV_GATE_INCDEC)
				{
					GateInLevel_Change();
				}
			}
			DrumMachineVar.encoder_flags &= ~ENCODER_ROTATION_FLAG;
		}
		if (( DrumMachineVar.encoder_flags & ENCODER_SW_FLAG ) == ENCODER_SW_FLAG)
		{
			if (( DrumMachineVar.system & SYSTEM_MENU_INCDEC) == SYSTEM_MENU_INCDEC)
				MeuEncoderChangeMenu();
			else
			{
				if (( DrumMachineVar.encoder_navigation & ENCNAV_BPM_INCDEC) == ENCNAV_BPM_INCDEC)
				{
					BPM_Value_Draw(0);
					DrumMachineVar.encoder_navigation &= ~ENCNAV_BPM_INCDEC;
				}
				if (( DrumMachineVar.encoder_navigation & ENCNAV_DELAYVAL_INCDEC) == ENCNAV_DELAYVAL_INCDEC)
				{
					Delay_Value_Draw(0);
					DrumMachineVar.encoder_navigation &= ~ENCNAV_DELAYVAL_INCDEC;
				}
				if (( DrumMachineVar.encoder_navigation & ENCNAV_DELAYWEIGHT_INCDEC) == ENCNAV_DELAYWEIGHT_INCDEC)
				{
					Delay_Weight_Draw(0);
					DrumMachineVar.encoder_navigation &= ~ENCNAV_DELAYWEIGHT_INCDEC;
				}
				if (( DrumMachineVar.encoder_navigation & ENCNAV_MODE_INCDEC) == ENCNAV_MODE_INCDEC)
				{
					DrumMachineVar.encoder_navigation &= ~ENCNAV_MODE_INCDEC;
					SequencerDrawMode(0);
				}
				if (( DrumMachineVar.encoder_navigation & ENCNAV_TRIGINSRC_INCDEC) == ENCNAV_TRIGINSRC_INCDEC)
				{
					DrumMachineVar.encoder_navigation &= ~ENCNAV_TRIGINSRC_INCDEC;
				}
				if (( DrumMachineVar.encoder_navigation & ENCNAV_TRIGOUTBEAT_INCDEC) == ENCNAV_TRIGOUTBEAT_INCDEC)
				{
					DrumMachineVar.encoder_navigation &= ~ENCNAV_TRIGOUTBEAT_INCDEC;
				}
				if (( DrumMachineVar.encoder_navigation & ENCNAV_TRIGOUTLVL_INCDEC) == ENCNAV_TRIGOUTLVL_INCDEC)
				{
					DrumMachineVar.encoder_navigation &= ~ENCNAV_TRIGOUTLVL_INCDEC;
				}
				if (( DrumMachineVar.encoder_navigation & ENCNAV_GATE_INCDEC) == ENCNAV_GATE_INCDEC)
				{
					DrumMachineVar.encoder_navigation &= ~ENCNAV_GATE_INCDEC;
				}
				DrumMachineVar.uservalue_changed = STORE_USERVALUES_TIMEOUT;
				DrumMachineVar.system |= SYSTEM_MENU_INCDEC;
			}
			DrumMachineVar.encoder_flags &= ~ENCODER_SW_FLAG;
		}
	}
}

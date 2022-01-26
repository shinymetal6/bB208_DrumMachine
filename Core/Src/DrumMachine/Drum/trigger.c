/*
 * trigger.c
 *
 *  Created on: Jan 24, 2022
 *      Author: fil
 */

#include "main.h"
#include "trigger.h"
#include "../Hmi/menus.h"
#include "../Images/icons_memory.h"

void TriggerDrawInMode(uint8_t hilight)
{
	/*
	if ( hilight == 0)
	{
		if ((DrumMachineVar.sequencer_flags & (SEQUENCER_TRIGGERINH | SEQUENCER_TRIGGERINL ) ) ==  (SEQUENCER_TRIGGERINH | SEQUENCER_TRIGGERINL ) )
		{
			ILI9341_DrawBitmap(ICON_TRIGIN_X,ICONS_Y,(uint8_t *)&icons_50x20_normal[BOTH_INDEX]);
		}
		else if ((DrumMachineVar.sequencer_flags & (SEQUENCER_TRIGGERINH | SEQUENCER_TRIGGERINL ) ) ==  0 )
		{
			ILI9341_DrawBitmap(ICON_TRIGIN_X,ICONS_Y,(uint8_t *)&icons_50x20_normal[INT_INDEX]);
		}
		else if ((DrumMachineVar.sequencer_flags & SEQUENCER_TRIGGERINH ) ==  SEQUENCER_TRIGGERINH )
		{
			ILI9341_DrawBitmap(ICON_TRIGIN_X,ICONS_Y,(uint8_t *)&icons_50x20_normal[RISING_INDEX]);
		}
		else
		{
			ILI9341_DrawBitmap(ICON_TRIGIN_X,ICONS_Y,(uint8_t *)&icons_50x20_normal[FALLING_INDEX]);
		}
	}
	else
	{
		if ((DrumMachineVar.sequencer_flags & (SEQUENCER_TRIGGERINH | SEQUENCER_TRIGGERINL ) ) ==  (SEQUENCER_TRIGGERINH | SEQUENCER_TRIGGERINL ) )
		{
			ILI9341_DrawBitmap(ICON_TRIGIN_X,ICONS_Y,(uint8_t *)&icons_50x20_selected[BOTH_INDEX]);
		}
		else if ((DrumMachineVar.sequencer_flags & (SEQUENCER_TRIGGERINH | SEQUENCER_TRIGGERINL ) ) ==  0 )
		{
			ILI9341_DrawBitmap(ICON_TRIGIN_X,ICONS_Y,(uint8_t *)&icons_50x20_selected[INT_INDEX]);
		}
		else if ((DrumMachineVar.sequencer_flags & SEQUENCER_TRIGGERINH ) ==  SEQUENCER_TRIGGERINH )
		{
			ILI9341_DrawBitmap(ICON_TRIGIN_X,ICONS_Y,(uint8_t *)&icons_50x20_selected[RISING_INDEX]);
		}
		else
		{
			ILI9341_DrawBitmap(ICON_TRIGIN_X,ICONS_Y,(uint8_t *)&icons_50x20_selected[FALLING_INDEX]);
		}

	}
	*/
uint16_t color;
	if ( hilight == 0)
		color = ILI9341_WHITE;
	else
		color = ILI9341_RED;
	if ((DrumMachineVar.sequencer_flags & (SEQUENCER_TRIGGERINH | SEQUENCER_TRIGGERINL ) ) ==  (SEQUENCER_TRIGGERINH | SEQUENCER_TRIGGERINL ) )
	{
		ILI9341_WriteString(ICON_TRIGIN_X,ICONS_Y,"BOTH   ",Font_7x10,color,ILI9341_BLACK);
	}
	else if ((DrumMachineVar.sequencer_flags & (SEQUENCER_TRIGGERINH | SEQUENCER_TRIGGERINL ) ) ==  0 )
	{
		ILI9341_WriteString(ICON_TRIGIN_X,ICONS_Y,"NONE   ",Font_7x10,color,ILI9341_BLACK);
	}
	else if ((DrumMachineVar.sequencer_flags & SEQUENCER_TRIGGERINH ) ==  SEQUENCER_TRIGGERINH )
	{
		ILI9341_WriteString(ICON_TRIGIN_X,ICONS_Y,"RISING ",Font_7x10,color,ILI9341_BLACK);
	}
	else
	{
		ILI9341_WriteString(ICON_TRIGIN_X,ICONS_Y,"FALLING",Font_7x10,color,ILI9341_BLACK);
	}
}

void TriggerInMode_Change(void)
{
	if ((DrumMachineVar.sequencer_flags & (SEQUENCER_TRIGGERINH | SEQUENCER_TRIGGERINL ) ) ==  0 )
	{
		DrumMachineVar.sequencer_flags |= SEQUENCER_TRIGGERINL;			// goto L
		DrumMachineVar.sequencer_mode |= SECMODE_SEQUENCER_EXTERNAL;
	}
	else if ((DrumMachineVar.sequencer_flags & (SEQUENCER_TRIGGERINH | SEQUENCER_TRIGGERINL ) ) ==  SEQUENCER_TRIGGERINL )
	{
		DrumMachineVar.sequencer_flags &= ~SEQUENCER_TRIGGERINL;		// goto H
		DrumMachineVar.sequencer_flags |= SEQUENCER_TRIGGERINH;
		DrumMachineVar.sequencer_mode |= SECMODE_SEQUENCER_EXTERNAL;
	}
	else if ((DrumMachineVar.sequencer_flags & (SEQUENCER_TRIGGERINH | SEQUENCER_TRIGGERINL ) ) ==  SEQUENCER_TRIGGERINH )
	{
		DrumMachineVar.sequencer_flags |= SEQUENCER_TRIGGERINL;			// goto BOTH
		DrumMachineVar.sequencer_flags |= SEQUENCER_TRIGGERINH;
		DrumMachineVar.sequencer_mode |= SECMODE_SEQUENCER_EXTERNAL;
	}
	else if ((DrumMachineVar.sequencer_flags & (SEQUENCER_TRIGGERINH | SEQUENCER_TRIGGERINL )  ) ==  (SEQUENCER_TRIGGERINH | SEQUENCER_TRIGGERINL )  )
	{
		DrumMachineVar.sequencer_flags &= ~SEQUENCER_TRIGGERINL;		// goto NONE -> INTERNAL
		DrumMachineVar.sequencer_flags &= ~SEQUENCER_TRIGGERINH;
		DrumMachineVar.sequencer_mode &= ~SECMODE_SEQUENCER_EXTERNAL;
	}
	TriggerDrawInMode(1);
}

void TriggerDrawOutLevel(uint8_t hilight)
{
uint16_t color;
	if ( hilight == 0)
		color = ILI9341_WHITE;
	else
		color = ILI9341_RED;
	if ((DrumMachineVar.sequencer_flags & SEQUENCER_TRIGGEROUTLVL ) ==  SEQUENCER_TRIGGEROUTLVL )
		ILI9341_WriteString(ICON_TRIGOUT_X,ICONS_Y,"HIGH",Font_7x10,color,ILI9341_BLACK);
	else
		ILI9341_WriteString(ICON_TRIGOUT_X,ICONS_Y,"LOW ",Font_7x10,color,ILI9341_BLACK);
}

void TriggerOutLevel_Change(void)
{
	if ((DrumMachineVar.sequencer_flags & SEQUENCER_TRIGGEROUTLVL ) ==  SEQUENCER_TRIGGEROUTLVL )
	{
		DrumMachineVar.sequencer_flags &= ~SEQUENCER_TRIGGEROUTLVL;
	}
	else
	{
		DrumMachineVar.sequencer_flags |= SEQUENCER_TRIGGEROUTLVL;
	}
	TriggerDrawOutLevel(1);
}

void TriggerOutPosition_Change(void)
{
	GraphSetMarkerInactive(DrumMachineVar.sequencer_trigout_sample);
	if (( DrumMachineVar.encoder_flags & ENCODER_ROTATION_FLAG) == ENCODER_ROTATION_FLAG)
	{
		if (( DrumMachineVar.encoder_flags & ENCODER_INCREMENT_FLAG) == ENCODER_INCREMENT_FLAG)
		{
			DrumMachineVar.sequencer_trigout_sample ++;
			if ( DrumMachineVar.sequencer_trigout_sample >= UserParameters.sequencer_length )
				DrumMachineVar.sequencer_trigout_sample = 0;
		}
		else
		{
			if ( DrumMachineVar.sequencer_trigout_sample == 0 )
				DrumMachineVar.sequencer_trigout_sample = UserParameters.sequencer_length-1;
			else
				DrumMachineVar.sequencer_trigout_sample --;
		}
	}
	GraphSetMarkerActive(DrumMachineVar.sequencer_trigout_sample);
}

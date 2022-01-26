/*
 * gate.c
 *
 *  Created on: Jan 24, 2022
 *      Author: fil
 */

#include "main.h"
#include "gate.h"
#include "../Hmi/menus.h"
#include "../Images/icons_memory.h"

void GateDrawInLevel(uint8_t hilight)
{
	/*
	if ( hilight == 0)
	{
		if ((DrumMachineVar.sequencer_flags & (SEQUENCER_GATEH | SEQUENCER_GATEL ) ) ==  0 )
			ILI9341_DrawBitmap(ICON_GATEIN_X,ICONS_Y,(uint8_t *)&icons_50x20_normal[NONE_INDEX]);
		else if ((DrumMachineVar.sequencer_flags & (SEQUENCER_GATEH | SEQUENCER_GATEL ) ) ==  SEQUENCER_GATEL )
			ILI9341_DrawBitmap(ICON_GATEIN_X,ICONS_Y,(uint8_t *)&icons_50x20_normal[LOW_INDEX]);
		else if ((DrumMachineVar.sequencer_flags & (SEQUENCER_GATEH | SEQUENCER_GATEL ) ) ==  SEQUENCER_GATEH )
			ILI9341_DrawBitmap(ICON_GATEIN_X,ICONS_Y,(uint8_t *)&icons_50x20_normal[HIGH_INDEX]);
	}
	else
	{
		if ((DrumMachineVar.sequencer_flags & (SEQUENCER_GATEH | SEQUENCER_GATEL ) ) ==  0 )
			ILI9341_DrawBitmap(ICON_GATEIN_X,ICONS_Y,(uint8_t *)&icons_50x20_selected[NONE_INDEX]);
		else if ((DrumMachineVar.sequencer_flags & (SEQUENCER_GATEH | SEQUENCER_GATEL ) ) ==  SEQUENCER_GATEL )
			ILI9341_DrawBitmap(ICON_GATEIN_X,ICONS_Y,(uint8_t *)&icons_50x20_selected[LOW_INDEX]);
		else if ((DrumMachineVar.sequencer_flags & (SEQUENCER_GATEH | SEQUENCER_GATEL ) ) ==  SEQUENCER_GATEH )
			ILI9341_DrawBitmap(ICON_GATEIN_X,ICONS_Y,(uint8_t *)&icons_50x20_selected[HIGH_INDEX]);
	}
	*/
uint16_t color;
	if ( hilight == 0)
		color = ILI9341_WHITE;
	else
		color = ILI9341_RED;

	if ((DrumMachineVar.sequencer_flags & (SEQUENCER_GATEH | SEQUENCER_GATEL ) ) ==  0 )
		ILI9341_WriteString(ICON_GATEIN_X,ICONS_Y,"NONE",Font_7x10,color,ILI9341_BLACK);
	else if ((DrumMachineVar.sequencer_flags & (SEQUENCER_GATEH | SEQUENCER_GATEL ) ) ==  SEQUENCER_GATEL )
		ILI9341_WriteString(ICON_GATEIN_X,ICONS_Y,"LOW ",Font_7x10,color,ILI9341_BLACK);
	else if ((DrumMachineVar.sequencer_flags & (SEQUENCER_GATEH | SEQUENCER_GATEL ) ) ==  SEQUENCER_GATEH )
		ILI9341_WriteString(ICON_GATEIN_X,ICONS_Y,"HIGH",Font_7x10,color,ILI9341_BLACK);
}

void GateInLevel_Change(void)
{
	if ((DrumMachineVar.sequencer_flags & (SEQUENCER_GATEH | SEQUENCER_GATEL ) ) ==  0 )
	{
		DrumMachineVar.sequencer_flags |= SEQUENCER_GATEL;			// goto L
	}
	else if ((DrumMachineVar.sequencer_flags & (SEQUENCER_GATEH | SEQUENCER_GATEL ) ) ==  SEQUENCER_GATEL )
	{
		DrumMachineVar.sequencer_flags &= ~SEQUENCER_GATEL;		// goto H
		DrumMachineVar.sequencer_flags |= SEQUENCER_GATEH;
	}
	else if ((DrumMachineVar.sequencer_flags & (SEQUENCER_GATEH | SEQUENCER_GATEL )  ) ==  SEQUENCER_GATEH  )
	{
		DrumMachineVar.sequencer_flags &= ~SEQUENCER_GATEL;		// goto NONE -> INTERNAL
		DrumMachineVar.sequencer_flags &= ~SEQUENCER_GATEH;
	}
	GateDrawInLevel(1);
}



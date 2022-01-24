/*
 * menus.c
 *
 *  Created on: Sep 20, 2021
 *      Author: fil
 */

#include "main.h"
#include "menus.h"
#include "../Drum/enc_btns_leds.h"
#include "../Drum/trigger.h"
#include "../Drum/gate.h"
#include "../Drum/sequencer.h"
#include <stdio.h>

MenuVars_TypeDef	MenuVars;

Menu_TypeDef	MenuTop[] =
{
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y,
			"Sequencer",
			MENU_ACTIVE_COLOR,
		},
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y+1*MENU_FONT_HEIGHT,
			"Echo - Reverb",
			MENU_INACTIVE_COLOR,
		},
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y+2*MENU_FONT_HEIGHT,
			"Global",
			MENU_INACTIVE_COLOR,
		},
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y+3*MENU_FONT_HEIGHT,
			"Settings",
			MENU_INACTIVE_COLOR,
		},
		{
			0,0,
		},
};

Menu_TypeDef	MenuSequence[] =
{
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y,
			"Sequencer Mode",
			MENU_ACTIVE_COLOR,
		},
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y+1*MENU_FONT_HEIGHT,
			"Trigger IN",
			MENU_ACTIVE_COLOR,
		},
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y+2*MENU_FONT_HEIGHT,
			"Trigger OUT",
			MENU_INACTIVE_COLOR,
		},
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y+3*MENU_FONT_HEIGHT,
			"Trigger OUT beat 1",
			MENU_INACTIVE_COLOR,
		},
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y+4*MENU_FONT_HEIGHT,
			"Sequencer Gate",
			MENU_INACTIVE_COLOR,
		},
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y+5*MENU_FONT_HEIGHT,
			"Return",
			MENU_INACTIVE_COLOR,
		},
		{
			0,0,
		},
};

Menu_TypeDef	MenuDelay[] =
{
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y,
			"Set Delay",
			MENU_ACTIVE_COLOR,
		},
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y+MENU_FONT_HEIGHT,
			"Set Weight",
			MENU_INACTIVE_COLOR,
		},
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y+2*MENU_FONT_HEIGHT,
			"Set Type",
			MENU_INACTIVE_COLOR,
		},
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y+3*MENU_FONT_HEIGHT,
			"Return",
			MENU_INACTIVE_COLOR,
		},
		{
			0,0,
		},
};

Menu_TypeDef	MenuGlobals[] =
{
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y,
			"BPM",
			MENU_INACTIVE_COLOR,
		},
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y+1*MENU_FONT_HEIGHT,
			"Return",
			MENU_INACTIVE_COLOR,
		},
		{
			0,0,
		},
};
Menu_TypeDef	MenuSettings[] =
{
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y,
			"Copy Samples from SD",
			MENU_ACTIVE_COLOR,
		},
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y+1*MENU_FONT_HEIGHT,
			"Copy Sequence from SD",
			MENU_ACTIVE_COLOR,
		},
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y+2*MENU_FONT_HEIGHT,
			"Load Digits from SD",
			MENU_INACTIVE_COLOR,
		},
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y+3*MENU_FONT_HEIGHT,
			"Load Icons from SD",
			MENU_INACTIVE_COLOR,
		},
		{
			MENU_LINE_0_X,
			MENU_LINE_0_Y+4*MENU_FONT_HEIGHT,
			"Return",
			MENU_INACTIVE_COLOR,
		},
		{
			0,0,
		},
};

void ClearStatusLine(void)
{
char 		status_line[24];
uint16_t	i;
	for(i=0;i<24;i++)
		status_line[i] = 0x20;
	status_line[23] = 0;
	ILI9341_WriteString(STATUS_BAR_X,STATUS_BAR_Y,(char *)status_line,Font_7x10,ILI9341_WHITE,ILI9341_BLACK);
}

void WriteStatusLine(char *line)
{
char 		status_line[24];
uint16_t	i;

	ClearStatusLine();
	for(i=0;i<24;i++)
		status_line[i] = *line++;
	status_line[23] = 0;
	ILI9341_WriteString(STATUS_BAR_X,STATUS_BAR_Y,(char *)status_line,Font_7x10,ILI9341_WHITE,ILI9341_BLACK);
}

static void DisplayStringAt(uint16_t x , uint16_t y, uint8_t *text,uint8_t normal_color)
{
	if ( normal_color == 0 )
		ILI9341_WriteString(x,y,(char *)text,Font_11x18,ILI9341_CYAN,ILI9341_BLACK);
	else
		ILI9341_WriteString(x,y,(char *)text,Font_11x18,ILI9341_GREEN,ILI9341_BLACK);
}

static void EraseStringAt(uint16_t x , uint16_t y, uint8_t *text)
{
	ILI9341_WriteString(x,y,(char *)text,Font_11x18,ILI9341_BLACK,ILI9341_BLACK);
}

void MenuHilightItem(Menu_TypeDef *menu)
{
	if ( MenuVars.next_menu_item > menu[0].items)
		MenuVars.next_menu_item = 0;
	DisplayStringAt(menu[MenuVars.selected_menu_item].linex,menu[MenuVars.selected_menu_item].liney,menu[MenuVars.selected_menu_item].text,MENU_NORMAL_COLOR);
	MenuVars.selected_menu_item = MenuVars.next_menu_item;
	DisplayStringAt(menu[MenuVars.selected_menu_item].linex,menu[MenuVars.selected_menu_item].liney,menu[MenuVars.selected_menu_item].text,MENU_HILIGHT_COLOR);
}

void MenuDisplayMenu(Menu_TypeDef *menu )
{
uint8_t	i=0;
	for(i=0;i<=MenuVars.current_menu[0].items;i++)
		EraseStringAt(MenuVars.current_menu[i].linex,MenuVars.current_menu[i].liney, MenuVars.current_menu[i].text);
	for(i=0;i<=menu[0].items;i++)
		DisplayStringAt(menu[i].linex,menu[i].liney, menu[i].text,i);
	MenuVars.next_menu_item = 0;
	MenuVars.selected_menu_item = 0;
	MenuVars.current_menu = menu;
}

void MenuDisplayInit(void)
{
uint8_t	i=0;
	ILI9341_WriteString(TITLE_BAR_X,TITLE_BAR_Y,"bB208 Drum Machine",Font_11x18,ILI9341_BLUE,ILI9341_BLACK);
	ILI9341_SetBrightness(50);

	for(i=0;i< MENU_MAXLINENUM;i++)
	{
		if(MenuTop[i].liney != 0 )
		{
			MenuVars.current_menu = (Menu_TypeDef * )&MenuTop;
			DisplayStringAt(MenuTop[i].linex,MenuTop[i].liney,MenuTop[i].text,i);
			MenuTop[0].items = i;
		}
		else
			break;
	}
	for(i=0;i< MENU_MAXLINENUM;i++)
	{
		if(MenuSequence[i].liney != 0 )
			MenuSequence[0].items = i;
		else
			break;
	}
	for(i=0;i< MENU_MAXLINENUM;i++)
	{
		if(MenuDelay[i].liney != 0 )
			MenuDelay[0].items = i;
		else
			break;
	}
	for(i=0;i< MENU_MAXLINENUM;i++)
	{
		if(MenuGlobals[i].liney != 0 )
			MenuGlobals[0].items = i;
		else
			break;
	}
	for(i=0;i< MENU_MAXLINENUM;i++)
	{
		if(MenuSettings[i].liney != 0 )
			MenuSettings[0].items = i;
		else
			break;
	}
	ILI9341_WriteString(BPM_TEXT_X,BPM_TEXT_Y,"BPM",Font_7x10,ILI9341_WHITE,ILI9341_BLACK);
	ILI9341_WriteString(DLY_TEXT_X,DLY_TEXT_Y,"Dly",Font_7x10,ILI9341_WHITE,ILI9341_BLACK);
	ILI9341_WriteString(DLYW_TEXT_X,DLYW_TEXT_Y,"W",Font_7x10,ILI9341_WHITE,ILI9341_BLACK);

	Delay_Value_Draw(0);
	Delay_Weight_Draw(0);
	BPM_Value_Draw(0);

	SequencerDrawMode(0);
	TriggerDrawMode(0);
	TriggerDrawOutLevel(0);
	GateDrawInLevel(0);
}

void MenuEncoderNavigate(void)
{
	if (( DrumMachineVar.encoder_flags & ENCODER_ROTATION_FLAG) == ENCODER_ROTATION_FLAG)
	{
		if (( DrumMachineVar.encoder_flags & ENCODER_DECREMENT_FLAG) == ENCODER_DECREMENT_FLAG)
		{
			if ( MenuVars.next_menu_item == 0 )
				MenuVars.next_menu_item = MenuVars.current_menu[0].items;
			else
				MenuVars.next_menu_item--;
			DrumMachineVar.encoder_flags &= ~ENCODER_DECREMENT_FLAG;
		}
		if (( DrumMachineVar.encoder_flags & ENCODER_INCREMENT_FLAG) == ENCODER_INCREMENT_FLAG)
		{
			MenuVars.next_menu_item++;
			if ( MenuVars.next_menu_item > MenuVars.current_menu[0].items )
				MenuVars.next_menu_item = 0;
			DrumMachineVar.encoder_flags &= ~ENCODER_INCREMENT_FLAG;
		}
		DrumMachineVar.encoder_flags &= ~ENCODER_ROTATION_FLAG;
	}
	MenuHilightItem(MenuVars.current_menu);
}

void update_sequencer_mode(void)
{

}

void MeuEncoderChangeMenu(void)
{
	switch(MenuVars.menu_state)
	{
	case	MENU_TOP	:
		if ( MenuVars.next_menu_item == 0 )
		{
			MenuDisplayMenu(MenuSequence);
			MenuVars.menu_state = MENU_SEQUENCE;
		}
		if ( MenuVars.next_menu_item == 1 )
		{
			MenuDisplayMenu(MenuDelay);
			MenuVars.menu_state = MENU_DELAY;
		}
		if ( MenuVars.next_menu_item == 2 )
		{
			MenuDisplayMenu(MenuGlobals);
			MenuVars.menu_state = MENU_GLOBALS;
		}
		if ( MenuVars.next_menu_item == 3 )
		{
			MenuDisplayMenu(MenuSettings);
			MenuVars.menu_state = MENU_SETTINGS;
		}
		break;
	case	MENU_SEQUENCE	:
		if ( MenuVars.next_menu_item == 0)
		{
			DrumMachineVar.encoder_navigation |= ENCNAV_MODE_INCDEC;
			DrumMachineVar.system &= ~SYSTEM_MENU_INCDEC;
			SequencerDrawMode(1);
			return;
		}
		if ( MenuVars.next_menu_item == 1)
		{
			DrumMachineVar.encoder_navigation |= ENCNAV_TRIGINSRC_INCDEC;
			DrumMachineVar.system &= ~SYSTEM_MENU_INCDEC;
			return;
		}
		if ( MenuVars.next_menu_item == 2)
		{
			DrumMachineVar.encoder_navigation |= ENCNAV_TRIGOUTLVL_INCDEC;
			DrumMachineVar.system &= ~SYSTEM_MENU_INCDEC;
			return;
		}
		if ( MenuVars.next_menu_item == 3)
		{
			DrumMachineVar.encoder_navigation |= ENCNAV_TRIGOUTBEAT_INCDEC;
			DrumMachineVar.system &= ~SYSTEM_MENU_INCDEC;
			return;
		}
		if ( MenuVars.next_menu_item == 4)
		{
			DrumMachineVar.encoder_navigation |= ENCNAV_GATE_INCDEC;
			DrumMachineVar.system &= ~SYSTEM_MENU_INCDEC;
			return;
		}
		if ( MenuVars.next_menu_item == MenuSequence[0].items)
		{
			MenuVars.menu_state = MENU_TOP;
			ClearStatusLine();
			MenuDisplayMenu(MenuTop);
		}
		break;
	case	MENU_DELAY	:
		if ( MenuVars.next_menu_item == 0)
		{
			DrumMachineVar.encoder_navigation |= ENCNAV_DELAYVAL_INCDEC;
			DrumMachineVar.system &= ~SYSTEM_MENU_INCDEC;
			Delay_Value_Draw(1);
			return;
		}
		if ( MenuVars.next_menu_item == 1)
		{
			DrumMachineVar.encoder_navigation |= ENCNAV_DELAYWEIGHT_INCDEC;
			DrumMachineVar.system &= ~SYSTEM_MENU_INCDEC;
			Delay_Weight_Draw(1);
			return;
		}
		if ( MenuVars.next_menu_item == 2)
		{
			if (( UserParameters.delay_type & DELAY_TYPE_FLANGER) == DELAY_TYPE_FLANGER)
			{
				UserParameters.delay_type |= DELAY_TYPE_ECHO;
				UserParameters.delay_type &= ~DELAY_TYPE_FLANGER;
				ILI9341_WriteString(STATUS_BAR_DLYX,STATUS_BAR_Y,"E",Font_7x10,ILI9341_WHITE,ILI9341_BLACK);
			}
			else if (( UserParameters.delay_type & DELAY_TYPE_ECHO) == DELAY_TYPE_ECHO)
			{
				UserParameters.delay_type &= ~DELAY_TYPE_FLANGER;
				UserParameters.delay_type &= ~DELAY_TYPE_ECHO;
				ILI9341_WriteString(STATUS_BAR_DLYX,STATUS_BAR_Y,"N",Font_7x10,ILI9341_WHITE,ILI9341_BLACK);
			}
			else
			{
				UserParameters.delay_type |= DELAY_TYPE_FLANGER;
				ILI9341_WriteString(STATUS_BAR_DLYX,STATUS_BAR_Y,"F",Font_7x10,ILI9341_WHITE,ILI9341_BLACK);
			}
			return;
		}
		if ( MenuVars.next_menu_item == MenuDelay[0].items)
		{
			MenuVars.menu_state = MENU_TOP;
			MenuDisplayMenu(MenuTop);
		}
		break;
	case	MENU_GLOBALS	:
		if ( MenuVars.next_menu_item == 0)
		{
			DrumMachineVar.encoder_navigation |= ENCNAV_BPM_INCDEC;
			DrumMachineVar.system &= ~SYSTEM_MENU_INCDEC;
			BPM_Value_Draw(1);
			return;
		}
		if ( MenuVars.next_menu_item == MenuGlobals[0].items)
		{
			MenuVars.menu_state = MENU_TOP;
			MenuDisplayMenu(MenuTop);
		}
		break;
	case	MENU_SETTINGS	:

		if ( MenuVars.next_menu_item == 0)
		{
			SequencerInternalStop();
			CopySamplesFromSD();
			return;
		}
		if ( MenuVars.next_menu_item == 1)
		{
			SequencerInternalStop();
			CopySequenceFromSD();
			return;
		}
		if ( MenuVars.next_menu_item == 2)
		{
			SequencerInternalStop();
			CopyDigitsFromSD();
			return;
		}
		if ( MenuVars.next_menu_item == 3)
		{
			SequencerInternalStop();
			CopyIconsFromSD();
			return;
		}
		if ( MenuVars.next_menu_item == MenuSettings[0].items)
		{
			MenuVars.menu_state = MENU_TOP;
			MenuDisplayMenu(MenuTop);
		}


		break;
	}
}




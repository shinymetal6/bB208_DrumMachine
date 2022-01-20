/*
 * menus.h
 *
 *  Created on: Nov 15, 2021
 *      Author: fil
 */

#ifndef SRC_BB2XX_BSP_HMI_MENUS_H_
#define SRC_BB2XX_BSP_HMI_MENUS_H_

#include "graph.h"

#define	MENU_FONT_HEIGHT		18
#define	MENU_LINE_MAX_LEN		32
#define	MENU_LINE_0_X			1
#define	MENU_LINE_0_Y			32
#define	MENU_NORMAL_COLOR		1
#define	MENU_HILIGHT_COLOR		0
#define	MENU_INACTIVE_COLOR		ILI9341_WHITE
#define	MENU_ACTIVE_COLOR		ILI9341_CYAN
#define	MENU_DELETE_COLOR		ILI9341_BLACK
#define	MENU_MAXLINENUM			12

#define	LCD_RESOLUTION_X		ILI9341_WIDTH
#define	LCD_RESOLUTION_Y		ILI9341_HEIGHT

#define	DIGIT_SPACE				DIGITS_16x29_H+2
#define	DIGIT_HSPACE			2
#define	DIGIT_VSPACE			2
#define	TITLE_BAR_X				0
#define	TITLE_BAR_Y				0
#define	STATUS_BAR_X			0
#define	STATUS_BAR_Y			(ILI9341_HEIGHT-STATUS_BAR_HEIGHT)
#define	STATUS_BAR_HEIGHT		12
#define	STATUS_BAR_STATUS1		27
#define	STATUS_BAR_LEN			(7*STATUS_BAR_STATUS1)
#define	STATUS_BAR_XDELTA		10
#define	STATUS_BAR_TRIGX		((7*STATUS_BAR_STATUS1)+0*STATUS_BAR_XDELTA)
#define	STATUS_BAR_GATEX		((7*STATUS_BAR_STATUS1)+1*STATUS_BAR_XDELTA)
#define	STATUS_BAR_DLYX			((7*STATUS_BAR_STATUS1)+2*STATUS_BAR_XDELTA)
#define	STATUS_BAR_SEQTYPE		((7*STATUS_BAR_STATUS1)+3*STATUS_BAR_XDELTA)
#define	STATUS_BAR_SEQOUTX		((7*STATUS_BAR_STATUS1)+4*STATUS_BAR_XDELTA)

//#define	APP_DIGIT_YPOS			(LCD_RESOLUTION_Y/2 -35)
#define	APP_DIGIT_YPOS			(GRAPH_Y - 2*(DIGITS_16x29_H+DIGIT_VSPACE) - 4)
//#define	SEQ_DIGIT_YPOS			(LCD_RESOLUTION_Y/2 + 0)
#define	SEQ_DIGIT_YPOS			(GRAPH_Y - 1*(DIGITS_16x29_H+DIGIT_VSPACE) - 4)

#define	APP_TEXT_OFFSET			9

#define	BPM_DIGIT_XPOSH			30
#define	BPM_DIGIT_XPOST			(BPM_DIGIT_XPOSH+DIGITS_16x29_W+DIGIT_HSPACE)
#define	BPM_DIGIT_XPOSU			(BPM_DIGIT_XPOST+DIGITS_16x29_W+DIGIT_HSPACE)
#define	BPM_DIGIT_YPOS			APP_DIGIT_YPOS
#define	BPM_TEXT_X				0
#define	BPM_TEXT_Y				(APP_DIGIT_YPOS+APP_TEXT_OFFSET)

#define	DLY_DIGIT_XPOSH			(BPM_DIGIT_XPOSU+50)
#define	DLY_DIGIT_XPOST			(DLY_DIGIT_XPOSH+DIGITS_16x29_W+DIGIT_HSPACE)
#define	DLY_DIGIT_XPOSU			(DLY_DIGIT_XPOST+DIGITS_16x29_W+DIGIT_HSPACE)
#define	DLY_DIGIT_YPOS			APP_DIGIT_YPOS
#define	DLY_TEXT_X				(BPM_DIGIT_XPOSU+DIGITS_16x29_W+9)
#define	DLY_TEXT_Y				(APP_DIGIT_YPOS+APP_TEXT_OFFSET)

#define	DLYW_DIGIT_XPOST		(DLY_DIGIT_XPOSU+2*DIGITS_16x29_W+DIGIT_HSPACE)
#define	DLYW_DIGIT_XPOSU		(DLYW_DIGIT_XPOST+DIGITS_16x29_W+DIGIT_HSPACE)
#define	DLYW_DIGIT_YPOS			APP_DIGIT_YPOS
#define	DLYW_TEXT_X				(DLY_DIGIT_XPOSU+DIGITS_16x29_W+5)
#define	DLYW_TEXT_Y				(APP_DIGIT_YPOS+APP_TEXT_OFFSET)

#define	SEQ_BEATNUM_X 					0
#define	SEQ_BEATNUM_Y 					SEQ_DIGIT_YPOS+9

#define	SEQ_CURRENTBEAT_DIGIT_XPOSH		30
#define	SEQ_CURRENTBEAT_DIGIT_XPOST		(SEQ_CURRENTBEAT_DIGIT_XPOSH+DIGITS_16x29_W+DIGIT_HSPACE)
#define	SEQ_CURRENTBEAT_DIGIT_XPOSU		(SEQ_CURRENTBEAT_DIGIT_XPOST+DIGITS_16x29_W+DIGIT_HSPACE)

#define	SEQ_OFBEAT_X 					(SEQ_CURRENTBEAT_DIGIT_XPOSU+DIGITS_16x29_W + 10)
#define	SEQ_OFBEAT_Y 					SEQ_DIGIT_YPOS+9

#define	SEQ_NUMBEAT_DIGIT_XPOSH			(SEQ_CURRENTBEAT_DIGIT_XPOSU + 50 )
#define	SEQ_NUMBEAT_DIGIT_XPOST			(SEQ_NUMBEAT_DIGIT_XPOSH+DIGITS_16x29_W+DIGIT_HSPACE)
#define	SEQ_NUMBEAT_DIGIT_XPOSU			(SEQ_NUMBEAT_DIGIT_XPOST+DIGITS_16x29_W+DIGIT_HSPACE)

#define	DESCRIPTOR_AREA_X		0
#define	DESCRIPTOR_AREA_Y		DLY_DIGIT_YPOS+DIGIT_Y

typedef struct {
	uint8_t			linex,liney;
	uint8_t			text[MENU_LINE_MAX_LEN];
	uint16_t		color;
	uint8_t			items;
	uint8_t			state_next_index;
}Menu_TypeDef;

typedef struct {
	uint8_t			menu_state;
	uint8_t			next_menu_item;
	uint8_t			selected_menu_item;
	Menu_TypeDef 	*current_menu;
}MenuVars_TypeDef;


/* states */
typedef enum
{
	  MENU_TOP,
	  MENU_SEQUENCE,
	  MENU_DELAY,
	  MENU_GLOBALS,
	  MENU_SETTINGS
}MenuState_Typdef;

extern	void ClearStatusLine(void);
extern	void WriteStatusLine(char *line);

#endif /* SRC_BB2XX_BSP_HMI_MENUS_H_ */

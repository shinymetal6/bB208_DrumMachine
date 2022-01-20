/*
 * graph.h
 *
 *  Created on: Jan 8, 2022
 *      Author: fil
 */

#ifndef SRC_DRUMMACHINE_HMI_GRAPH_H_
#define SRC_DRUMMACHINE_HMI_GRAPH_H_

#include "menus.h"

#define	GRAPH_X			(0)
//#define	GRAPH_Y			(SEQ_DIGIT_YPOS+DIGITS_16x29_H+6)
#define	GRAPH_Y			(LCD_RESOLUTION_Y-STATUS_BAR_HEIGHT-(GRAPH_YSTEP*NUM_INSTRUMENTS)-4)
#define	GRAPH_YSTEP		4
#define	GRAPH_AREA_Y	(GRAPH_Y)
#define	GRAPH_AREA_X	15

#define	COLOR_INACTIVE		ILI9341_COLOR565(0x3f,0x3f,0x3f)
#define	COLOR_ACTIVE		ILI9341_GREEN
#define	COLOR_PLAY			ILI9341_RED
#define	COLOR_MARKER_ACTIVE	ILI9341_CYAN

extern	void GraphInit(void);
extern	void GraphUpdateGraph(void);
extern	void GraphSetMarkerInactive(uint16_t sequencer_step);
extern	void GraphSetMarkerActive(uint16_t sequencer_step);

#endif /* SRC_DRUMMACHINE_HMI_GRAPH_H_ */

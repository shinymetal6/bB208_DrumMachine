/*
 * vol_meter.h
 *
 *  Created on: Jan 19, 2022
 *      Author: fil
 */

#ifndef SRC_DRUMMACHINE_HMI_VOL_METER_H_
#define SRC_DRUMMACHINE_HMI_VOL_METER_H_

#include "graph.h"

#define	VOL_METER_STEPS			10
#define	VOL_METER_CELL_INIWIDTH	2
#define	VOL_METER_CELLHEIGHT	4
#define	VOL_METER_CELLSPACE		2
#define	VOL_METER_X				(LCD_RESOLUTION_X-VOL_METER_CELL_INIWIDTH)
#define	VOL_METER_Y				(GRAPH_Y - (VOL_METER_CELLHEIGHT+VOL_METER_CELLSPACE+4))


#define	METER_COLOR_INACTIVE	ILI9341_COLOR565(0x3f,0x3f,0x3f)
#define	METER_GREEN				ILI9341_GREEN
#define	METER_YELLOW			ILI9341_YELLOW
#define	METER_RED				ILI9341_RED


extern	void DrawInitialVolMeter(void);
extern	void DrawVolMeterValue(uint16_t	value);

#endif /* SRC_DRUMMACHINE_HMI_VOL_METER_H_ */

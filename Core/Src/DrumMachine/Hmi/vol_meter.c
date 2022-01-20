/*
 * vol_meter.c
 *
 *  Created on: Jan 19, 2022
 *      Author: fil
 */

#include "main.h"
#include "vol_meter.h"

typedef struct {
	uint16_t	x,y,w,color;
}VolMatrix_TypeDef;

VolMatrix_TypeDef	VolMatrix[VOL_METER_STEPS];

void DrawVolMeterValue(uint16_t	value)
{
uint16_t	i,j,y,color;

	if ( value > 9 )
		return;
	for(i=0;i<VOL_METER_STEPS;i++)
	{
		if ( i <= value )
			color = VolMatrix[i].color;
		else
			color = METER_COLOR_INACTIVE;
		y = VolMatrix[i].y;
		for(j=0;j<VOL_METER_CELLHEIGHT;j++,y--)
			ILI9341_DrawLine(VolMatrix[i].x, y, VolMatrix[i].x+VolMatrix[i].w, y, color);
	}
}

void DrawInitialVolMeter(void)
{
uint32_t	i,x=VOL_METER_X,y=VOL_METER_Y,w=VOL_METER_CELL_INIWIDTH;

	for(i=0;i<VOL_METER_STEPS;i++,y++)
	{
		VolMatrix[i].x = x;
		VolMatrix[i].y = y;
		VolMatrix[i].w = w;
		x = (VolMatrix[i].x - 1);
		y = (VolMatrix[i].y - VOL_METER_CELLHEIGHT - VOL_METER_CELLSPACE);
		w += 1;

		VolMatrix[i].color = ILI9341_GREEN;
		if ( i > 4 )
			VolMatrix[i].color = ILI9341_YELLOW;
		if ( i > 7 )
			VolMatrix[i].color = ILI9341_RED;
	}
	DrawVolMeterValue(0);
	/*
	for(i=0;i<10;i++)
		DrawVolMeterValue(i);
		*/
}



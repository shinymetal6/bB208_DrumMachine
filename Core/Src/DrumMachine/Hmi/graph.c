/*
 * graph.c
 *
 *  Created on: Jan 8, 2022
 *      Author: fil
 */

#include "main.h"
#include "graph.h"
#include "../Drum/sequencer.h"


typedef struct {
	uint16_t	x,y,w,h;
}DotMatrix_TypeDef;
#define	MAX_BEAT_GRAPH	32
#define	GRAPH_OFFSET	10

DotMatrix_TypeDef	DotMatrix[MAX_BEAT_GRAPH][NUM_INSTRUMENTS],SequenceMarker[MAX_BEAT_GRAPH];

void GraphSetSequenceInactive(uint16_t sequencer_instrument,uint16_t sequencer_step)
{
uint16_t k;
	for(k=0;k<DotMatrix[sequencer_step][sequencer_instrument].h;k++)
		ILI9341_DrawLine(DotMatrix[sequencer_step][sequencer_instrument].x, DotMatrix[sequencer_step][sequencer_instrument].y+k, DotMatrix[sequencer_step][sequencer_instrument].x+DotMatrix[sequencer_step][sequencer_instrument].w, DotMatrix[sequencer_step][sequencer_instrument].y+k, COLOR_INACTIVE);
}

void GraphSetSequenceActive(uint16_t sequencer_instrument,uint16_t sequencer_step)
{
uint16_t k;
	for(k=0;k<DotMatrix[sequencer_step][sequencer_instrument].h;k++)
		ILI9341_DrawLine(DotMatrix[sequencer_step][sequencer_instrument].x, DotMatrix[sequencer_step][sequencer_instrument].y+k, DotMatrix[sequencer_step][sequencer_instrument].x+DotMatrix[sequencer_step][sequencer_instrument].w, DotMatrix[sequencer_step][sequencer_instrument].y+k, COLOR_ACTIVE);
}

void GraphSetSequencePlay(uint16_t sequencer_instrument,uint16_t sequencer_step)
{
uint16_t k;
	for(k=0;k<DotMatrix[sequencer_step][sequencer_instrument].h;k++)
		ILI9341_DrawLine(DotMatrix[sequencer_step][sequencer_instrument].x, DotMatrix[sequencer_step][sequencer_instrument].y+k, DotMatrix[sequencer_step][sequencer_instrument].x+DotMatrix[sequencer_step][sequencer_instrument].w, DotMatrix[sequencer_step][sequencer_instrument].y+k, COLOR_PLAY);
}

void GraphUpdateGraph(void)
{
uint16_t	instrument,val;
uint16_t 	step,last_step;

	step = DrumMachineVar.sequencer_last_step;
	if ( step == 0 )
		last_step = Sequencer_Descriptor.sequencer_length-1;
	else
		last_step = step-1;

	for(instrument=0;instrument<NUM_INSTRUMENTS;instrument++)
	{
		val = 1 << instrument;
		if (( Sequencer_Descriptor.sequencer_steps[last_step] & val) == 0 )
			GraphSetSequenceInactive(instrument,last_step);
		else
			GraphSetSequenceActive(instrument,last_step);
		if (( Sequencer_Descriptor.sequencer_steps[step] & val) != 0 )
		{
			if (( DrumMachineVar.sequencer_flags & SEQUENCER_ENABLE ) == SEQUENCER_ENABLE )
				GraphSetSequencePlay(instrument,step);
			else
				GraphSetSequenceActive(instrument,step);
		}
		else
			GraphSetSequenceInactive(instrument,step);
	}
}

void GraphInitialInitGraph(void)
{
uint16_t	instrument,val,i;

	for(i=0;i<Sequencer_Descriptor.sequencer_length;i++)
	{
		for(instrument=0;instrument<NUM_INSTRUMENTS;instrument++)
		{
			val = 1 << instrument;
			if (( Sequencer_Descriptor.sequencer_steps[i] & val) == 0 )
				GraphSetSequenceInactive(instrument,i);
			else
				GraphSetSequenceActive(instrument,i);
		}
	}
}

void GraphSetMarkerInactive(uint16_t sequencer_step)
{
uint16_t k;
	for(k=0;k<SequenceMarker[sequencer_step].h;k++)
		ILI9341_DrawLine(SequenceMarker[sequencer_step].x, SequenceMarker[sequencer_step].y+k, SequenceMarker[sequencer_step].x+SequenceMarker[sequencer_step].w, SequenceMarker[sequencer_step].y+k, COLOR_INACTIVE);
}

void GraphSetMarkerActive(uint16_t sequencer_step)
{
uint16_t k;
	for(k=0;k<SequenceMarker[sequencer_step].h;k++)
		ILI9341_DrawLine(SequenceMarker[sequencer_step].x, SequenceMarker[sequencer_step].y+k, SequenceMarker[sequencer_step].x+SequenceMarker[sequencer_step].w, SequenceMarker[sequencer_step].y+k, COLOR_MARKER_ACTIVE);
}

void GraphInitialInitSequenceMarker(void)
{
uint16_t	i;

	for(i=0;i<Sequencer_Descriptor.sequencer_length;i++)
	{
		if ( i == DrumMachineVar.sequencer_trigout_sample )
			GraphSetMarkerActive(i);
		else
			GraphSetMarkerInactive(i);
	}
}


void GraphInit(void)
{
uint16_t	instrument_xstep = (LCD_RESOLUTION_X-GRAPH_OFFSET)/UserParameters.sequencer_length;
uint16_t	instrument_ystep = GRAPH_YSTEP;
uint16_t	instrument_x = GRAPH_OFFSET;
uint16_t	instrument_y = GRAPH_Y;
uint16_t	i,j;
uint16_t			common_w,common_h;

	common_w = instrument_xstep-2;
	common_h = instrument_ystep-2;

	for(i=0;i<NUM_INSTRUMENTS;i++)
	{
		instrument_x = GRAPH_OFFSET;
		for(j=0;j<UserParameters.sequencer_length;j++)
		{
			DotMatrix[j][i].x = instrument_x+1;
			DotMatrix[j][i].y = instrument_y+1;
			DotMatrix[j][i].w = common_w;
			DotMatrix[j][i].h = common_h;
			instrument_x += instrument_xstep;
		}
		instrument_y += instrument_ystep;
	}
	for(j=0;j<UserParameters.sequencer_length;j++)
	{
		SequenceMarker[j].x = DotMatrix[j][0].x;
		SequenceMarker[j].y = DotMatrix[j][0].y-4;
		SequenceMarker[j].w = DotMatrix[j][0].w;
		SequenceMarker[j].h = DotMatrix[j][0].h;
	}
	GraphInitialInitGraph();
	GraphInitialInitSequenceMarker();
}


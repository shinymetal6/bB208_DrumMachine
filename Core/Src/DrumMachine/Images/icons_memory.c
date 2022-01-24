/*
 * icons_memory.c
 *
 *  Created on: Jan 24, 2022
 *      Author: fil
 */

#include "main.h"
#include "icons_memory.h"


IconVar_TypeDef	IconVar[ICON1_NUMBER] =
{
		{
				"nboth.bmp",
				"sboth.bmp",
				BOTH_INDEX
		},
		{
				"nfalling.bmp",
				"sfalling.bmp",
				FALLING_INDEX
		},
		{
				"nint.bmp",
				"sint.bmp",
				INT_INDEX
		},
		{
				"nlow.bmp",
				"slow.bmp",
				LOW_INDEX
		},
		{
				"nrising.bmp",
				"srising.bmp",
				RISING_INDEX
		},
		{
				"next.bmp",
				"sext.bmp",
				EXT_INDEX
		},
		{
				"nhigh.bmp",
				"shigh.bmp",
				HIGH_INDEX
		},
		{
				"nloop.bmp",
				"sloop.bmp",
				LOOP_INDEX
		},
		{
				"nnone.bmp",
				"snone.bmp",
				NONE_INDEX
		},
		{
				"nsingle.bmp",
				"ssingle.bmp",
				SINGLE_INDEX
		},
};

DTCM_RAM_AREA	__attribute__ ((aligned (16))) uint8_t icons_50x20_normal[ICON1_NUMBER][ICON1_SIZE];
DTCM_RAM_AREA	__attribute__ ((aligned (16))) uint8_t icons_50x20_selected[ICON1_NUMBER][ICON1_SIZE];

/*
 * audio_buffers.c
 *
 *  Created on: Nov 10, 2021
 *      Author: fil
 */

#include "main.h"
#include "audio_buffers.h"

RAMD3_RAM_AREA	__attribute__ ((aligned (16)))	uint16_t		intermediate_dac1[DAC_SIGNAL_LEN/2];
RAMD3_RAM_AREA	__attribute__ ((aligned (16)))	uint16_t		intermediate_dac2[DAC_SIGNAL_LEN/2];
RAMD3_RAM_AREA	__attribute__ ((aligned (16)))	uint16_t		signal_out_dac1[DAC_SIGNAL_LEN];
RAMD3_RAM_AREA	__attribute__ ((aligned (16)))	uint16_t		signal_out_dac2[DAC_SIGNAL_LEN];


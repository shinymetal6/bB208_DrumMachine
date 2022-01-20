/*
 * drum_buffers.c
 *
 *  Created on: Dec 14, 2021
 *      Author: fil
 */

#include "main.h"
#include "drum_buffers.h"
#include "instrument.h"

NOCACHE_RAM	__attribute__ ((aligned (16)))	uint16_t	drum_buffers[NUM_INSTRUMENTS][DRUM_BUFFER_SIZE];
NOCACHE_RAM	__attribute__ ((aligned (16)))	int16_t		s_drum_buffers[NUM_INSTRUMENTS][DRUM_BUFFER_SIZE];
NOCACHE_RAM	__attribute__ ((aligned (16)))	int16_t		i_wav_data[DRUM_BUFFER_SIZE];

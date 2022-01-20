/*
 * control_buffer.c
 *
 *  Created on: Nov 10, 2021
 *      Author: fil
 */

#include "main.h"
#include "control_buffer.h"

NOCACHE_RAM	__attribute__ ((aligned (16)))	uint16_t	control_pot[4];

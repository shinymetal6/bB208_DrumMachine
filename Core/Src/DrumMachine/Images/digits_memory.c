/*
 * digits_memory.c
 *
 *  Created on: Nov 12, 2021
 *      Author: fil
 */

#include "main.h"
#include "digits_memory.h"

/*
RAMD2_RAM_AREA	__attribute__ ((aligned (16))) uint8_t normal_digits_22x40[10][DIGITS_SIZE];
RAMD2_RAM_AREA	__attribute__ ((aligned (16))) uint8_t hilight_digits_22x40[10][DIGITS_SIZE];
RAMD2_RAM_AREA	__attribute__ ((aligned (16))) uint8_t normal_digits_16x29[10][DIGITS_SIZE];
RAMD2_RAM_AREA	__attribute__ ((aligned (16))) uint8_t hilight_digits_16x29[10][DIGITS_SIZE];
*/
DTCM_RAM_AREA	__attribute__ ((aligned (16))) uint8_t normal_digits_22x40[10][DIGITS_SIZE];
DTCM_RAM_AREA	__attribute__ ((aligned (16))) uint8_t hilight_digits_22x40[10][DIGITS_SIZE];
DTCM_RAM_AREA	__attribute__ ((aligned (16))) uint8_t normal_digits_16x29[10][DIGITS_SIZE];
DTCM_RAM_AREA	__attribute__ ((aligned (16))) uint8_t hilight_digits_16x29[10][DIGITS_SIZE];


/*
 * drum_buffers.h
 *
 *  Created on: Dec 14, 2021
 *      Author: fil
 */

#ifndef SRC_DRUMMACHINE_DRUM_DRUM_BUFFERS_H_
#define SRC_DRUMMACHINE_DRUM_DRUM_BUFFERS_H_


/* the single sample buffer size */
#define	DRUM_BUFFER_SIZE			256
extern	uint16_t	drum_buffers[NUM_INSTRUMENTS][DRUM_BUFFER_SIZE];
extern	int16_t		s_drum_buffers[NUM_INSTRUMENTS][DRUM_BUFFER_SIZE];
extern	int16_t		i_wav_data[DRUM_BUFFER_SIZE];

#endif /* SRC_DRUMMACHINE_DRUM_DRUM_BUFFERS_H_ */

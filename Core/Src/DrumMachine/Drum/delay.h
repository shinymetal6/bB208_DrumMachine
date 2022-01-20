/*
 * delay.h
 *
 *  Created on: Nov 17, 2021
 *      Author: fil
 */

#ifndef SRC_DRUMMACHINE_DRUM_DELAY_H_
#define SRC_DRUMMACHINE_DRUM_DELAY_H_

#define	MAX_DELAY	999
#define	MAX_DELAYW	90
#define	MIN_DELAYW	10

#define	DELAY_LINE_SIZE		65536
#define	DELAY_POINTER_MASK	0xffff

extern	float DelayLine( float sample );

#endif /* SRC_APPLICATION_DELAY_H_ */

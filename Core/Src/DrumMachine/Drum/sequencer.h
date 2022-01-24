/*
 * sequencer.h
 *
 *  Created on: Sep 24, 2021
 *      Author: fil
 */

#ifndef SRC_DRUMMACHINE_SEQUENCER_H_
#define SRC_DRUMMACHINE_SEQUENCER_H_

#define	SEQUENCER_MAX_SIZE	1024
#define	HEADER_LEN			8

extern	void SequencerInit(void);
extern	void SequencerCallback(void);

typedef struct {
	uint8_t		header[8];
	uint16_t	sequencer_length;
	uint16_t	sequencer_beat;
	uint16_t	delay_type;
	uint16_t	delay_value;
	uint16_t	delay_weight;
	uint16_t	flag;
	uint16_t	sequencer_steps[SEQUENCER_MAX_SIZE];
	uint16_t	number_of_instruments;
}Sequencer_DescriptorTypeDef;


/* sequencer file example , simple 4/4 , 4 beat
hihat on 0
bassdrum on 1
snare on 2

I I I I I I I I I I I I I I
n n n n n n n n n n n n n n
s s s s s s s s s s s s s s
t t t t t t t t t t t t t t
r r r r r r r r r r r r r r
u u u u u u u u u u u u u u
m m m m m m m m m m m m m m
e e e e e e e e e e e e e e
n n n n n n n n n n n n n n
t t t t t t t t t t t t t t
0 1 2 3 4 5 6 7 8 9 a b c d

LOOPLEN	4
1 0 0 0 0 0 0 0 0 0 0 0 0 0
1 1 0 0 0 0 0 0 0 0 0 0 0 0
1 0 1 0 0 0 0 0 0 0 0 0 0 0
1 0 0 0 0 0 0 0 0 0 0 0 0 0
*/

extern	uint16_t sequencer_steps[SEQUENCER_MAX_SIZE];

#define	TIM_ARR_1_BPM	7199999UL

extern	TIM_HandleTypeDef htim5;
#define	TIM_SEQUENCER	htim5
#define	TIM_SEQUENCERL	TIM5

extern	void SequencerInternalStart(void);
extern	void SequencerInternalStop(void);
extern	void SequencerUpdateBeat(void);
extern	void SequencerRestart(void);
extern	void SequencerExternalStart(void);
extern	void SequencerExternalStop(void);
extern	void SequencerModeChange(void);
extern	void SequencerDrawMode(uint8_t hilight);

#endif /* SRC_BB808_SEQUENCER_H_ */

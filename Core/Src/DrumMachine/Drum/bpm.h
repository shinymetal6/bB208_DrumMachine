/*
 * bpm.h
 *
 *  Created on: Nov 17, 2021
 *      Author: fil
 */

#ifndef SRC_DRUMMACHINE_BPM_H_
#define SRC_DRUMMACHINE_BPM_H_


/* value for MAX_BEAT */
#define	BPM_UNIT	334
/* value for 1 BPM */
#define	BPM_1_BPM	60120
/* Logic : write in counter register the value (BPM_1_BPM - (<bpm> * BPM_UNIT) where <bpm> must be less than MAX_BEAT */
#define	BPM_60_BPM	(60*BPM_UNIT)

extern	TIM_HandleTypeDef 			htim13;
#define	BPM_MEASURE_TIMER			htim13
#define	TIM_BPM_MEASURE				TIM13

extern	void BPMInit(void);
extern	void GetMeasuredBPM(void);
extern	void BPMEnableCapture(void);
extern	void BPMDisableCapture(void);
extern	void BPM_ExtValue_Draw(void);

#endif /* SRC_APPLICATION_BPM_H_ */

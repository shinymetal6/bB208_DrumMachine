/*
 * trigger.h
 *
 *  Created on: Jan 24, 2022
 *      Author: fil
 */

#ifndef SRC_DRUMMACHINE_DRUM_TRIGGER_H_
#define SRC_DRUMMACHINE_DRUM_TRIGGER_H_

extern	void TriggerInMode_Change(void);
extern	void TriggerOutLevel_Change(void);
extern	void TriggerOutPosition_Change(void);
extern	void TriggerDrawInMode(uint8_t hilight);
extern	void TriggerDrawOutLevel(uint8_t hilight);


#endif /* SRC_DRUMMACHINE_DRUM_TRIGGER_H_ */

/*
 * enc_btns_leds.h
 *
 *  Created on: Dec 22, 2021
 *      Author: fil
 */

#ifndef SRC_DRUMMACHINE_DRUM_ENC_BTNS_LEDS_H_
#define SRC_DRUMMACHINE_DRUM_ENC_BTNS_LEDS_H_

#define		LED_ON					0
#define		LED_OFF					1

extern	void encoder_rotation_callback(void);
extern	void encoder_switch_callback(void);
extern	void LD1_OnOff(uint8_t value);
extern	void LD7_OnOff(uint8_t value);
extern	void LD8_OnOff(uint8_t value);
extern	void sw1_callback(void);
extern	void sw2_callback(void);

#endif /* SRC_DRUMMACHINE_DRUM_ENC_BTNS_LEDS_H_ */

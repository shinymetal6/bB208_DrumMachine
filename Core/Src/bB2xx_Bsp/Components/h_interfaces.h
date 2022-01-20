/*
 * h_interfaces.h
 *
 *  Created on: Nov 10, 2021
 *      Author: fil
 */

#ifndef SRC_BB2XX_BSP_COMPONENTS_H_INTERFACES_H_
#define SRC_BB2XX_BSP_COMPONENTS_H_INTERFACES_H_

extern	void encoder_rotation_callback(void);
extern	void encoder_switch_callback(void);
extern	void LD1_OnOff(uint8_t value);
extern	void LD7_OnOff(uint8_t value);
extern	void LD8_OnOff(uint8_t value);
extern	void sw1_callback(void);
extern	void sw2_callback(void);

#endif /* SRC_BB2XX_BSP_COMPONENTS_H_INTERFACES_H_ */

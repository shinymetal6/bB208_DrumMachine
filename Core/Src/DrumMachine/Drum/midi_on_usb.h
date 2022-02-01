/*
 * midi_on_usb.h
 *
 *  Created on: Jan 26, 2022
 *      Author: fil
 */

#ifndef SRC_DRUMMACHINE_DRUM_MIDI_ON_USB_H_
#define SRC_DRUMMACHINE_DRUM_MIDI_ON_USB_H_

#define	MIDI_RXBUF_SIZE			256
#define	USB_TXBUF_SIZE			256

#define	CMD_MASK				0xF0
#define	CHANNEL_MASK			0x0F

#define	CABLEN_MASK				0xF0
#define	CIN_MASK				0x0F

#define	MISC					0x00
#define	CABLE					0x01
#define	TWO_BYTES				0x02
#define	THREE_BYTES				0x03
#define	SYSEX_STARTC			0x04
#define	SYSEX_END_1				0x05
#define	SYSEX_END_2				0x06
#define	SYSEX_END_3				0x07

#define	NOTE_OFF				0x08
#define	NOTE_ON					0x09
#define	POLY_PRESSURE			0x0A
#define	CONTROL_CHANGE			0x0B
#define	PROGRAM_CHANGE			0x0C
#define	CHANNEL_PRESSURE		0x0D
#define	PITCH_BEND				0x0E
#define	SINGLEBYTE				0x0F

#define	SYSEX_START				0xF0
#define	SYSEX_END				0xF7

extern	void usb_init(void);

#endif /* SRC_DRUMMACHINE_DRUM_MIDI_ON_USB_H_ */

/*
 * midi_parser.h
 *
 *  Created on: Jan 31, 2022
 *      Author: fil
 */

#ifndef SRC_DRUMMACHINE_DRUM_MIDI_PARSER_H_
#define SRC_DRUMMACHINE_DRUM_MIDI_PARSER_H_

#define	MIDI_RXBUF_SIZE			256
#define	USB_TXBUF_SIZE			256
extern	uint32_t	usb_packet_ready;
extern	uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);
extern	uint8_t	USB_RxBuffer[64];
extern	char	USB_TxBuffer[USB_TXBUF_SIZE];
extern	uint32_t USB_RxBufferLen;
extern	void CheckUSB(void);

#define	SYSEX_MAX_LEN			64

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

#define	SYSEX_CMD_FIRSTWRITESEQUENCE	0x01
#define	SYSEX_CMD_WRITESEQUENCE			0x02
#define	SYSEX_CMD_LASTWRITESEQUENCE		0x03
#define	SYSEX_CMD_WDATALEN			32

#define	SYSEX_CMD_OFFSET			1
#define	SYSEX_INSTRUMENT_OFFSET		2
#define	SYSEX_DATA_OFFSET			3


typedef struct {
	uint8_t		command;
	uint8_t		num_instrument;
	uint8_t		data[SYSEX_CMD_WDATALEN];
}SysExCmd_TypeDef;



extern	void UsbMidiParser(void);

#endif /* SRC_DRUMMACHINE_DRUM_MIDI_PARSER_H_ */

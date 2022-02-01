/*
 * midi_on_usb.c
 *
 *  Created on: Jan 26, 2022
 *      Author: fil
 */

#include "main.h"
#include "midi_on_usb.h"
#include "../Hmi/menus.h"
#include <string.h>

uint8_t	usb_rx_buf[64];
uint8_t	midi_rx_buf[64] , midi_tx_buf[64];

void usb_init(void)
{
	set_usb_rx_buf(usb_rx_buf);
	DrumMachineVar.midi_rx_buf = midi_rx_buf;
	DrumMachineVar.midi_tx_buf = midi_tx_buf;
	DrumMachineVar.midi_rx_len = DrumMachineVar.midi_tx_len = 0;
}

uint8_t UsbMidiParseNoteOFF(uint8_t channel ,uint8_t midi_note , uint8_t velocity)
{
	return 4;
}

uint8_t UsbMidiParseNoteON(uint8_t	channel , uint8_t midi_note , uint8_t velocity)
{
	return 4;
}

uint8_t UsbMidiParseSysEx(uint8_t	channel , uint8_t sub_command,uint32_t len)
{
	/*
uint8_t	i,j=1,k=0;

	if ( len > SYSEX_MAX_LEN )
		return 0;
	if ( SystemFlags.midi_rx_buffer[1] != SYSEX_START )
		return 0;
	SystemFlags.sysex_len = 0;
	for(i=0;i<len;i+=4)
	{
		if ( SystemFlags.midi_rx_buffer[i] == SYSEX_STARTC )
		{
			SystemFlags.sysex_buffer[k] = SystemFlags.midi_rx_buffer[j];
			SystemFlags.sysex_buffer[k+1] = SystemFlags.midi_rx_buffer[j+1];
			SystemFlags.sysex_buffer[k+2] = SystemFlags.midi_rx_buffer[j+2];
			k +=3;
			j +=4;
			SystemFlags.sysex_len+=3;
		}
		if ( SystemFlags.midi_rx_buffer[i] == SYSEX_END_1 )
		{
			SystemFlags.sysex_buffer[k] = SystemFlags.midi_rx_buffer[j];
			SystemFlags.sysex_len+=0;
			i = len;
		}
		if ( SystemFlags.midi_rx_buffer[i] == SYSEX_END_2 )
		{
			SystemFlags.sysex_buffer[k] = SystemFlags.midi_rx_buffer[j];
			SystemFlags.sysex_buffer[k+1] = SystemFlags.midi_rx_buffer[j+1];
			SystemFlags.sysex_len+=1;
			i = len;
		}
		if ( SystemFlags.midi_rx_buffer[i] == SYSEX_END_3 )
		{
			SystemFlags.sysex_buffer[k] = SystemFlags.midi_rx_buffer[j];
			SystemFlags.sysex_buffer[k+1] = SystemFlags.midi_rx_buffer[j+1];
			SystemFlags.sysex_buffer[k+2] = SystemFlags.midi_rx_buffer[j+2];
			SystemFlags.sysex_len+=2;
			i = len;
		}
	}
	if ( SystemFlags.sysex_buffer[SystemFlags.sysex_len] == SYSEX_END )
		UsbSysExApplyValues();
		*/
	return len;
}


void UsbMidiParser(void)
{
uint8_t		cmd,channel,midi_note,velocity;
uint32_t	l_index=0,len;

	len = DrumMachineVar.midi_rx_len;
	DrumMachineVar.midi_rx_len = 0;
	while ( l_index < len )
	{
		cmd = DrumMachineVar.midi_rx_buf[l_index] & CIN_MASK;
		channel = DrumMachineVar.midi_rx_buf[l_index+1] & CHANNEL_MASK;
		midi_note = DrumMachineVar.midi_rx_buf[l_index+2];
		velocity = DrumMachineVar.midi_rx_buf[l_index+3];
		switch(cmd)
		{
			case NOTE_OFF			:
				l_index += UsbMidiParseNoteOFF(channel,midi_note,velocity);
				break;
			case NOTE_ON			:
				l_index += UsbMidiParseNoteON (channel,midi_note,velocity);
				break;
			case SYSEX_END_1			:
				break;
			case SYSEX_STARTC			:
			case SYSEX_END_2			:
			case SYSEX_END_3			:
				l_index += UsbMidiParseSysEx (channel,cmd,len);
				break;
				/*
			case CONTROL_CHANGE			:
				l_index += UsbMidiParseControlChange (SystemFlags.midi_rx_buffer[l_index+2],SystemFlags.midi_rx_buffer[l_index+3]);
				break;
			case PROGRAM_CHANGE			:
				l_index += UsbMidiParseProgramChange (SystemFlags.midi_rx_buffer[l_index+2],SystemFlags.midi_rx_buffer[l_index+3]);
				break;
			case POLY_PRESSURE			:
				l_index += UsbMidiParsePolyPressure (channel,midi_note,velocity);
				break;
			case PITCH_BEND			:
				l_index += UsbMidiParsePitchBend (channel,midi_note,velocity);
				break;
			case SINGLEBYTE			:
				l_index += UsbMidiParseSingleByte (channel,midi_note,velocity);
				break;
			case MISC			:
				l_index += UsbMidiParseMisc (channel,midi_note,velocity);
				break;
			case CABLE			:
				l_index += UsbMidiParseCable (channel,midi_note,velocity);
				break;
			case TWO_BYTES			:
				l_index += UsbMidiParseTwoBytes (channel,midi_note,velocity);
				break;
			case THREE_BYTES			:
				l_index += UsbMidiParseThreeBytes (channel,midi_note,velocity);
				break;
				*/
			default			:	l_index += 4; break;
		}
	}
}


void application_usb_callback(uint8_t* Buf, uint16_t len )
{
	memcpy(DrumMachineVar.midi_rx_buf,Buf,len);
	DrumMachineVar.midi_rx_len = len;
	UsbMidiParser();
}



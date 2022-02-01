/*
 * midi_parser.c
 *
 *  Created on: Jan 31, 2022
 *      Author: fil
 */


#include "main.h"
#include "midi_parser.h"
#include "enc_btns_leds.h"
#include "delay.h"
#include <string.h>
#include <stdio.h>

__attribute__ ((aligned (4)))	uint8_t		midi_packet[MIDI_RXBUF_SIZE];
__attribute__ ((aligned (4)))	uint8_t		midi_tx_buf[MIDI_RXBUF_SIZE];
__attribute__ ((aligned (4)))	uint8_t		flashsector_from_sysex[SECTOR_SIZE];
__attribute__ ((aligned (4)))	uint8_t		sequence_file[SECTOR_SIZE];
uint32_t	faddr;
uint8_t		line[256];
uint16_t	crptr;


void decode_seq_file(void)
{
uint32_t	params=0,i,j,val,sequence_file_idx=0;
uint8_t 	len;
uint8_t 	delay_type[16];
int		 	seq[16];

	crptr = 0;
	while (sequence_file[crptr] != 0 )
	{
		for ( i=0;i<256;i++ )
		{
			if (( sequence_file[crptr+i] == 0x0a ) || ( sequence_file[crptr+i] == 0 ))
			{
				line[i] = 0;
				crptr += (i+1);
				break;
			}
			line[i] = sequence_file[crptr+i];
		}

		if ( (len = strlen((char * )line)) != 0 )
		{
			if ( params == 0 )
			{
				if ( sscanf((char * )line,"BEAT %d",(int *)&seq[0]) == 1 )
				{
					if ( seq[0] <= MAX_BEAT)
						Sequencer_Descriptor.sequencer_beat = seq[0];
				}
				else if ( sscanf((char * )line,"DELAY %s %d %d",delay_type,(int *)&seq[0],(int *)&seq[1]) == 3 )
				{
					if ( strcmp((char * )delay_type,"ECHO") == 0 )
						Sequencer_Descriptor.delay_type = DELAY_TYPE_ECHO;
					else if ( strcmp((char * )delay_type,"FLANGER") == 0 )
						Sequencer_Descriptor.delay_type = DELAY_TYPE_FLANGER;
					else
						Sequencer_Descriptor.delay_type = DELAY_TYPE_NONE;
					if ( seq[0] <= MAX_DELAY)
						Sequencer_Descriptor.delay_value = seq[0];
					else
						Sequencer_Descriptor.delay_value = 0;
					if (( seq[1] <= MAX_DELAYW) && ( seq[1] >= MIN_DELAYW))
						Sequencer_Descriptor.delay_weight = seq[1];
					else
						Sequencer_Descriptor.delay_weight = (MAX_DELAYW+MIN_DELAYW)/2;
				}
				else if ( sscanf((char * )line,"BB208_SEQUENCE %d",(int *)&seq[0]) == 1 )
				{
					Sequencer_Descriptor.sequencer_length = seq[0];
					params = 1;
				}
			}
			else
			{
				if ( sscanf((char * )line,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",&seq[0],&seq[1],&seq[2],&seq[3],&seq[4],&seq[5],&seq[6],&seq[7],&seq[8],&seq[9],&seq[10],&seq[11],&seq[12],&seq[13],&seq[14],&seq[15]) == 16 )
				{
					val = 0;
					for(j=0;j<16;j++)
					{
						if ( seq[j] != 0 )
							val |= (1 << j);
					}
					Sequencer_Descriptor.sequencer_steps[sequence_file_idx] = val;
					sequence_file_idx++;
				}
			}
		}
	}
}

uint16_t SYSEX_ConvertMidiBufOut(uint8_t *buf_out , uint16_t len)
{
uint32_t	r=0,k=0;

	midi_tx_buf[k] = SYSEX_STARTC;
	k++;
	midi_tx_buf[k] = SYSEX_START;
	k++;
	r = 0;
	while ( r < len )
	{
		midi_tx_buf[k] = buf_out[r];
		r++;
		k++;
		if ( (k & 0x03 ) == 0)
		{
			if (( len - r ) == 2)
				midi_tx_buf[k] = SYSEX_END_3;
			else if (( len - r ) == 1)
				midi_tx_buf[k] = SYSEX_END_2;
			else if (( len - r ) == 0)
				midi_tx_buf[k] = SYSEX_END_1;
			else
				midi_tx_buf[k] = SYSEX_STARTC;
			k++;
		}
	}
	midi_tx_buf[k] = SYSEX_END;
	while ((k & 0x03 ) != 0 )
		k++;
	return k;
}

//#define		TEST_MIDI_OUT	1
#ifdef test01
void DumpProgram(uint8_t program_number)
{
uint16_t	i;
uint32_t	Len;

	for(i=0;i<MIDI_PROGRAM_SIZE;i++)
		midi_tx_buf[i] = 0;

#ifdef TEST_MIDI_OUT
	midi_tx_buf[0] = SYSEX_STARTC;
	midi_tx_buf[1] = SYSEX_START;
	midi_tx_buf[2] = 'a';
	midi_tx_buf[3] = 'b';
	midi_tx_buf[4] = SYSEX_STARTC;
	midi_tx_buf[5] = 'c';
	midi_tx_buf[6] = 'd';
	midi_tx_buf[7] = 'e';
	midi_tx_buf[8] = SYSEX_STARTC;
	midi_tx_buf[9] = 'f';
	midi_tx_buf[10] = 'g';
	midi_tx_buf[11] = 'h';
	midi_tx_buf[12] = SYSEX_STARTC;
	midi_tx_buf[13] = 'i';
	midi_tx_buf[14] = 'j';
	midi_tx_buf[15] = 'k';
	midi_tx_buf[16] = SYSEX_STARTC;
	midi_tx_buf[17] = 'l';
	midi_tx_buf[18] = 'm';
	midi_tx_buf[19] = 'n';
	midi_tx_buf[20] = SYSEX_STARTC;
	midi_tx_buf[21] = 'o';
	midi_tx_buf[22] = 'p';
	midi_tx_buf[23] = 'q';
	midi_tx_buf[24] = SYSEX_STARTC;
	midi_tx_buf[25] = 'r';
	midi_tx_buf[26] = 's';
	midi_tx_buf[27] = 't';
	midi_tx_buf[28] = SYSEX_STARTC;
	midi_tx_buf[29] = 'u';
	midi_tx_buf[30] = 'v';
	midi_tx_buf[31] = 'w';
	midi_tx_buf[32] = SYSEX_STARTC;
	midi_tx_buf[33] = 'x';
	midi_tx_buf[34] = 'y';
	midi_tx_buf[35] = 'z';
	midi_tx_buf[36] = SYSEX_STARTC;
	midi_tx_buf[37] = '0';
	midi_tx_buf[38] = '1';
	midi_tx_buf[39] = '2';
	midi_tx_buf[40] = SYSEX_END_3;
	midi_tx_buf[41] = '3';
	midi_tx_buf[42] = '4';
	midi_tx_buf[43] = SYSEX_END;
	Len = MIDI_PROGRAM_SIZE;
#else
	ee24_read_program(program_number);
	Len = SYSEX_ConvertMidiBufOut((uint8_t *)&Program,sizeof(Program));
#endif

	MIDI_Transmit_FS(midi_tx_buf,  Len);
}

void DumpCurrentProgram(void)
{
uint16_t	i;
uint32_t	Len;

	for(i=0;i<MIDI_PROGRAM_SIZE;i++)
		midi_tx_buf[i] = 0;
	ee24_get_current_program();
	Len = SYSEX_ConvertMidiBufOut((uint8_t *)&CurrentProgram,sizeof(Program));
	MIDI_Transmit_FS(midi_tx_buf,  Len);
}
#endif

uint8_t UsbMidiParseNoteOFF(uint8_t channel ,uint8_t midi_note , uint8_t velocity)
{
	return 4;
}

uint8_t UsbMidiParseNoteON(uint8_t	channel , uint8_t midi_note , uint8_t velocity)
{
	return 4;
}

void write_flash(uint32_t address)
{
	faddr = address;
}

uint8_t UsbMidiParseSysEx(uint8_t	channel , uint8_t sub_command,uint32_t len)
{
uint8_t		i,j=1,k=0;
uint16_t	flashbuf_index;
uint32_t	flash_address;

	if ( len > SYSEX_MAX_LEN )
		return 0;
	if ( DrumMachineVar.midi_packet[1] != SYSEX_START )
		return 0;
	DrumMachineVar.sysex_len = 0;
	for(i=0;i<len;i+=4)
	{
		if ( DrumMachineVar.midi_packet[i] == SYSEX_STARTC )
		{
			DrumMachineVar.sysex_buffer[k] = DrumMachineVar.midi_packet[j];
			DrumMachineVar.sysex_buffer[k+1] = DrumMachineVar.midi_packet[j+1];
			DrumMachineVar.sysex_buffer[k+2] = DrumMachineVar.midi_packet[j+2];
			k +=3;
			j +=4;
			DrumMachineVar.sysex_len+=3;
		}
		if ( DrumMachineVar.midi_packet[i] == SYSEX_END_1 )
		{
			DrumMachineVar.sysex_buffer[k] = DrumMachineVar.midi_packet[j];
			DrumMachineVar.sysex_len+=0;
			i = len;
		}
		if ( DrumMachineVar.midi_packet[i] == SYSEX_END_2 )
		{
			DrumMachineVar.sysex_buffer[k] = DrumMachineVar.midi_packet[j];
			DrumMachineVar.sysex_buffer[k+1] = DrumMachineVar.midi_packet[j+1];
			DrumMachineVar.sysex_len+=1;
			i = len;
		}
		if ( DrumMachineVar.midi_packet[i] == SYSEX_END_3 )
		{
			DrumMachineVar.sysex_buffer[k] = DrumMachineVar.midi_packet[j];
			DrumMachineVar.sysex_buffer[k+1] = DrumMachineVar.midi_packet[j+1];
			DrumMachineVar.sysex_buffer[k+2] = DrumMachineVar.midi_packet[j+2];
			DrumMachineVar.sysex_len+=2;
			i = len;
		}
	}
	if ( DrumMachineVar.sysex_buffer[DrumMachineVar.sysex_len] == SYSEX_END )
	{

		switch (DrumMachineVar.sysex_buffer[SYSEX_CMD_OFFSET] )
		{
		case	SYSEX_CMD_FIRSTWRITESEQUENCE:
			LD7_OnOff(LED_ON);
			DrumMachineVar.sysex_wbuf_counter = DrumMachineVar.sysex_wsector_stored = 0;
		case	SYSEX_CMD_WRITESEQUENCE:
			for(i=0;i<SYSEX_CMD_WDATALEN;i++)
				sequence_file[DrumMachineVar.sysex_wbuf_counter + i] = DrumMachineVar.sysex_buffer[SYSEX_DATA_OFFSET+i];
			DrumMachineVar.sysex_wbuf_counter += SYSEX_CMD_WDATALEN;
			break;
		case	SYSEX_CMD_LASTWRITESEQUENCE:
			DrumMachineVar.sysex_wbuf_counter = DrumMachineVar.sysex_wsector_stored = 0;
			decode_seq_file();
			LD7_OnOff(LED_OFF);
			break;
		default:
			break;
		}
	}
	return len;
}

uint8_t	UsbMidiParseControlChange(uint8_t channel ,uint8_t cc_index,uint8_t cc_value) // channel unused
{
	return 4;
}

uint8_t UsbMidiParseUnused(uint8_t channel ,uint8_t midi_note , uint8_t velocity)
{
	return 4;
}

void UsbMidiParser(void)
{
uint8_t		cmd,channel,midi_note,velocity;
uint32_t	l_index=0,len;

	len = DrumMachineVar.midi_rx_len;
	DrumMachineVar.midi_rx_len = 0;
	while ( l_index < len )
	{
		cmd = DrumMachineVar.midi_packet[l_index] & CIN_MASK;
		channel = DrumMachineVar.midi_packet[l_index+1] & CHANNEL_MASK;
		midi_note = DrumMachineVar.midi_packet[l_index+2];
		velocity = DrumMachineVar.midi_packet[l_index+3];
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
			case CONTROL_CHANGE			:
				l_index += UsbMidiParseControlChange (channel,midi_note,velocity);
				break;
			case PROGRAM_CHANGE			:
				l_index += UsbMidiParseUnused (channel,midi_note,velocity);
				break;
			case POLY_PRESSURE			:
				l_index += UsbMidiParseUnused (channel,midi_note,velocity);
				break;
			case PITCH_BEND			:
				l_index += UsbMidiParseUnused (channel,midi_note,velocity);
				break;
			case SINGLEBYTE			:
				l_index += UsbMidiParseUnused (channel,midi_note,velocity);
				break;
			case MISC			:
				l_index += UsbMidiParseUnused (channel,midi_note,velocity);
				break;
			case CABLE			:
				l_index += UsbMidiParseUnused (channel,midi_note,velocity);
				break;
			case TWO_BYTES			:
				l_index += UsbMidiParseUnused (channel,midi_note,velocity);
				break;
			case THREE_BYTES			:
				l_index += UsbMidiParseUnused (channel,midi_note,velocity);
				break;
			default			:	l_index += 4; break;
		}
	}
}




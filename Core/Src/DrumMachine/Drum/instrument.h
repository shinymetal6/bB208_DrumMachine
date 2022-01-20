/*
 * instrument.h
 *
 *  Created on: Dec 14, 2021
 *      Author: fil
 */

#ifndef SRC_DRUMMACHINE_DRUM_INSTRUMENT_H_
#define SRC_DRUMMACHINE_DRUM_INSTRUMENT_H_

#include "fatfs.h"
#include "../drum_machine.h"

#define AUDIO_OUT_BUFFER_SIZE			2048
#define AUDIO_OUT_HALFBUFFER_SIZE		(AUDIO_OUT_BUFFER_SIZE/2)

//#define	NO_AUDIO_OUT	32767
#define	NO_AUDIO_OUT	0
#define	F_NO_AUDIO_OUT	(float )(NO_AUDIO_OUT)

typedef struct {
	uint32_t	FileFlag;
	uint32_t	HeaderPtr;
	uint32_t	WavPtr;
	uint8_t		WavName[16];
	uint32_t 	FileSize;
	uint32_t 	PlayedSize;
	uint32_t 	SampleRate;
	uint8_t  	MidiKey;
	uint8_t  	Flags;
	//FIL 		WavFile;
}Instrument_TypeDef;

/* FileFlag */
#define	FILE_FLAG_VALID	0x57617665

/* Flags */
#define	WAVPLAY_STATE_FLAG_HALF		0x01
#define	WAVPLAY_STATE_FLAG_FULL		0x02
#define	WAVPLAY_SAMPLE_ACTIVE		0x80


typedef struct {
	uint32_t ChunkID;       /* 0 */
	uint32_t FileSize;      /* 4 */
	uint32_t FileFormat;    /* 8 */
	uint32_t SubChunk1ID;   /* 12 */
	uint32_t SubChunk1Size; /* 16*/
	uint16_t AudioFormat;   /* 20 */
	uint16_t NbrChannels;   /* 22 */
	uint32_t SampleRate;    /* 24 */
	uint32_t ByteRate;      /* 28 */
	uint16_t BlockAlign;    /* 32 */
	uint16_t BitPerSample;  /* 34 */
	uint32_t SubChunk2ID;   /* 36 */
	uint32_t SubChunk2Size; /* 40 */
}WAV_FormatTypeDef;

extern	Instrument_TypeDef		Instrument[NUM_INSTRUMENTS];

extern	uint32_t InitInstruments(void);
extern	void ProcessInstruments(void);
extern	uint32_t GetBufferLen(void);


#endif /* SRC_DRUMMACHINE_DRUM_INSTRUMENT_H_ */

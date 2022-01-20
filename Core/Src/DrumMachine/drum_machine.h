/*
 * drum_machine.h
 *
 *  Created on: Dec 10, 2021
 *      Author: fil
 */

#ifndef SRC_DRUMMACHINE_DRUM_MACHINE_H_
#define SRC_DRUMMACHINE_DRUM_MACHINE_H_


#define	NUM_INSTRUMENTS				16
#define	SAMPLE_NAME_MAX_LEN			12
#define	INSTRUMENT_SIZE				0x80000
#define	INSTRUMENT_64K_BLOCK		0x10000
#define	INSTRUMENT_NUM64K_BLOCKS	8
#define	INSTRUMENT_SECTORS			128
#define	HEADER_SIZE					32
#define MAX_BEAT					480
#define MIN_BEAT					6
#include "Drum/sequencer.h"
#include "Drum/drum_buffers.h"
#include "Drum/sdcard.h"
#include "Images/digits.h"
#include "Images/digits_memory.h"

extern	Sequencer_DescriptorTypeDef	Sequencer_Descriptor;

typedef struct {
	uint8_t			system;
	uint8_t			encoder_flags;
	uint8_t			buttons_flags;
	int16_t 		last_encval;
	int16_t 		encval;
	uint8_t			delay_flags;
	uint16_t		delay_insertion_pointer;
	uint16_t		delay_value;
	float			delay_weight;
	uint8_t			uservalue_changed;
	uint8_t			sequencer_flags;
	uint32_t		sequencer_preload;
	uint16_t		sequencer_counter;
	uint16_t		sequencer_step;
	uint16_t		sequencer_last_step;
	uint8_t			sequencer_trigout_sample;
	uint8_t			volume_cntr;
	uint8_t			volume_pot;
	uint8_t			last_volume_pot;
	float			volume;
	uint8_t			detune_pot;
	uint8_t			last_detune_pot;
	uint32_t		measured_bpm;
	int16_t			rvar[4];
}DrumMachineVar_TypeDef;

/* system */
#define	SYSTEM_MENU_INCDEC			0x80
#define	SYSTEM_SEQUENCER_INTERNAL	0x40
#define	SYSTEM_SEQUENCER_EXTERNAL	0x20
#define	SYSTEM_SEQUENCER_SSHOT		0x10
#define	SYSTEM_VOLUME_READY			0x08
#define	SYSTEM_BPM_INCDEC			0x04
#define	SYSTEM_DELAYWEIGHT_INCDEC	0x02
#define	SYSTEM_DELAYVAL_INCDEC		0x01

/* encoder_flags */
#define	ENCODER_ROTATION_FLAG		0x80
#define	ENCODER_INCREMENT_FLAG		0x40
#define	ENCODER_DECREMENT_FLAG		0x20
#define	ENCODER_SW_FLAG				0x01

/* buttons_flags */
#define	SW1_PRESSED					0x01
#define	SW2_PRESSED					0x02

/* uservalue_changed */
#define	STORE_USERVALUES_TIMEOUT	10

/* sequencer_flags */
#define	SEQUENCER_TRIGGERINH		0x01
#define	SEQUENCER_TRIGGERINL		0x02
#define	SEQUENCER_TRIGGEROUTLVL		0x04
#define	SEQUENCER_ONSAMPLE			0x08
#define	SEQUENCER_GATEH				0x10
#define	SEQUENCER_GATEL				0x20
#define	SEQUENCER_ENABLE			0x40
#define	SEQUENCER_DONE				0x80

extern	DrumMachineVar_TypeDef	DrumMachineVar;

typedef struct {
	uint16_t		delay_value;
	uint8_t			delay_type;
	uint16_t		delay_weight_int;
	float			delay_weight;
	uint16_t		delay_insertion_pointer;
	uint16_t		bpm;
	uint16_t		sequencer_length;
}UserParameters_TypeDef;

/* sequencer_flags */
#define	DELAY_TYPE_NONE		0x00
#define	DELAY_TYPE_ECHO		0x01
#define	DELAY_TYPE_FLANGER	0x02

/* pot definitions */
#define	VOLUME_POT	2
#define	DETUNE_POT	1

extern	UserParameters_TypeDef		UserParameters;


extern	void MenuDisplayInit(void);
extern	void MenuEncoderNavigate(void);
extern	void MeuEncoderChangeMenu(void);

extern	void Delay_Value_Draw(uint8_t hilight);
extern	void Delay_Weight_Draw(uint8_t hilight);
extern	void Delay_Value_Change(void);
extern	void Delay_Weight_Change(void);
extern	void BPM_Value_Draw(uint8_t hilight);
extern	void BPM_Value_Change(void);
extern	void bB2xx_uSecDelay(uint16_t us);

#endif /* SRC_APPLICATION_DRUM_MACHINE_H_ */

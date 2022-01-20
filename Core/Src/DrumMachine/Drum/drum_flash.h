/*
 * drum_flash.h
 *
 *  Created on: Dec 16, 2021
 *      Author: fil
 */

#ifndef SRC_DRUMMACHINE_DRUM_DRUM_FLASH_H_
#define SRC_DRUMMACHINE_DRUM_DRUM_FLASH_H_

/*
 * memory structure for 128mBit case , 16 mBytes total
 * 0xfff000 - 0xffffff : system parameters , 1 sector ,4096 bytes, reserved by system
 * 0xffe000 - 0xffefff : user parameters , 1 sector , 4096 bytes
 * 0xffd000 - 0xffdfff : user sequence , 1 sector , 4096 bytes
 * 0xffcfff - 0xf00000 : images , 253 sectors
 * 						0xf00000 - 0xf04fff : blue digits 22x40
 * 						0xf05000 - 0xf09fff : red digits 22x40
 * 						0xf0a000 - 0xf0efff : green digits 22x40
 * 						0xf14000 - 0xf18fff : yellow digits 22x40
 * 						0xf19000 - 0xf1dfff : orange digits 22x40
 * 						0xf1e000 - 0xf22fff : blue digits 16x29
 * 						0xf23000 - 0xf27fff : red digits 16x29
 * 						0xf28000 - 0xf2cfff : green digits 16x29
 * 						0xf2d000 - 0xf31fff : yellow digits 16x29
 * 						0xf32000 - 0xf36fff : orange digits 16x29
 * 						0xf37000 - 0xffcfff : logo and other images
 * 0x000000 - 0xefffff : samples , 3840 sectors
 */
#define		USRPARAM_NUMSECTORS	1
#define		USRSEQ_NUMSECTORS	1
#define		DIGIT_NUMSECTORS	5

#define		SYSPARAMS_PTR			(0xfff000)
#define		USRPARAMS_PTR			(SYSPARAMS_PTR-SECTOR_SIZE*USRPARAM_NUMSECTORS)
#define		USRSEQUENCE_PTR			(USRPARAMS_PTR-SECTOR_SIZE*USRSEQ_NUMSECTORS)
#define		IMAGE_PTR				(0xf00000)
#define		BLUE_DIGIT_TYPE1_PTR	IMAGE_PTR
#define		RED_DIGIT_TYPE1_PTR		(BLUE_DIGIT_TYPE1_PTR+SECTOR_SIZE*DIGIT_NUMSECTORS)
#define		GREEN_DIGIT_TYPE1_PTR	(RED_DIGIT_TYPE1_PTR+SECTOR_SIZE*DIGIT_NUMSECTORS)
#define		YELLOW_DIGIT_TYPE1_PTR	(GREEN_DIGIT_TYPE1_PTR+SECTOR_SIZE*DIGIT_NUMSECTORS)
#define		ORANGE_DIGIT_TYPE1_PTR	(YELLOW_DIGIT_TYPE1_PTR+SECTOR_SIZE*DIGIT_NUMSECTORS)
#define		BLUE_DIGIT_TYPE2_PTR	(ORANGE_DIGIT_TYPE1_PTR+SECTOR_SIZE*DIGIT_NUMSECTORS)
#define		RED_DIGIT_TYPE2_PTR		(BLUE_DIGIT_TYPE2_PTR+SECTOR_SIZE*DIGIT_NUMSECTORS)
#define		GREEN_DIGIT_TYPE2_PTR	(RED_DIGIT_TYPE2_PTR+SECTOR_SIZE*DIGIT_NUMSECTORS)
#define		YELLOW_DIGIT_TYPE2_PTR	(GREEN_DIGIT_TYPE2_PTR+SECTOR_SIZE*DIGIT_NUMSECTORS)
#define		ORANGE_DIGIT_TYPE2_PTR	(YELLOW_DIGIT_TYPE2_PTR+SECTOR_SIZE*DIGIT_NUMSECTORS)
#define		LOGO_PTR				(ORANGE_DIGIT_TYPE2_PTR+SECTOR_SIZE*DIGIT_NUMSECTORS)
#define		SAMPLE_PTR				0
#define		SAMPLE_END				(0xf00000)

#define		DIGIT_BLUE_IDX		0
#define		DIGIT_RED_IDX		1
#define		DIGIT_GREEN_IDX		2
#define		DIGIT_YELLOW_IDX	3
#define		DIGIT_ORANGE_IDX	4

extern	void drum_flash_load(void);
extern	void drum_flash_store_digits( void );
extern	void drum_flash_load_digits(void);
extern	void drum_flash_store_usrparams(void);
extern	void drum_flash_get_usrsparams(void);

#endif /* SRC_DRUMMACHINE_DRUM_DRUM_FLASH_H_ */

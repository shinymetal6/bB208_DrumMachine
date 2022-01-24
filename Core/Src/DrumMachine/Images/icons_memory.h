/*
 * icons_memory.h
 *
 *  Created on: Jan 24, 2022
 *      Author: fil
 */

#ifndef SRC_DRUMMACHINE_IMAGES_ICONS_MEMORY_H_
#define SRC_DRUMMACHINE_IMAGES_ICONS_MEMORY_H_


#define	ICON1_NUMBER	10
#define	ICON1_W 		50
#define	ICON1_H			20
#define	ICON1_SIZE		2138


typedef struct {
	char	normal_name[12];
	char	selected_name[12];
	uint8_t	index;
}IconVar_TypeDef;

extern	IconVar_TypeDef	IconVar[ICON1_NUMBER];
extern	uint8_t icons_50x20_normal[ICON1_NUMBER][ICON1_SIZE];
extern	uint8_t icons_50x20_selected[ICON1_NUMBER][ICON1_SIZE];

#define	BOTH_INDEX		0
#define	FALLING_INDEX	1
#define	INT_INDEX		2
#define	LOW_INDEX		3
#define	RISING_INDEX	4
#define	EXT_INDEX		5
#define	HIGH_INDEX		6
#define	LOOP_INDEX		7
#define	NONE_INDEX		8
#define	SINGLE_INDEX	9

#endif /* SRC_DRUMMACHINE_IMAGES_ICONS_MEMORY_H_ */

/*
 * bB2xx_System.h
 *
 *  Created on: Oct 18, 2021
 *      Author: fil
 */

#ifndef SRC_BB2XX_BSP_CORE_BB2XX_SYSTEM_H_
#define SRC_BB2XX_BSP_CORE_BB2XX_SYSTEM_H_

#include	"Components/ili9341.h"


/*
Regions
	DTCMRAM   (xrw)    : ORIGIN = 0x20000000,    LENGTH = 128K	Cacheable
	ITCMRAM   (xrw)    : ORIGIN = 0x00000000,    LENGTH = 64K	Cacheable
	RAM_D1    (xrw)    : ORIGIN = 0x24000000,    LENGTH = 512K	Cacheable
	RAM_D2    (xrw)    : ORIGIN = 0x30000000,    LENGTH = 288K	Cacheable
	RAM_D3    (xrw)    : ORIGIN = 0x38000000,    LENGTH = 64K	NON cacheable
*/
#define	CACHEENABLE	1

#ifdef CACHEENABLE
#define	DTCM_RAM_AREA				__attribute__((section(".dtcm_ram_area" )))
#define	ITCM_RAM_AREA				__attribute__((section(".itcm_ram_area" )))
#define	RAMD3_RAM_AREA				__attribute__((section(".ram_D3" )))
#define	RAMD2_RAM_AREA				__attribute__((section(".ram_D2" )))
#define	RAMD1_RAM_AREA				__attribute__((section(".ram_D1" )))
#define	FLASH_AREA					__attribute__((section(".table" )))
#define	NOCACHE_RAM					__attribute__((section(".ram_D3" )))
#else
#define	DTCM_RAM_AREA				__attribute__((section(".dtcm_ram_area" )))
#define	ITCM_RAM_AREA				__attribute__((section(".ram_D1" )))
#define	RAMD3_RAM_AREA				__attribute__((section(".ram_D1" )))
#define	RAMD2_RAM_AREA				__attribute__((section(".ram_D1" )))
#define	RAMD1_RAM_AREA				__attribute__((section(".ram_D1" )))
#define	NOCACHE_RAM					__attribute__((section(".ram_D1" )))
#define	FLASH_AREA					__attribute__((section(".table" )))
#endif

extern	TIM_HandleTypeDef 			htim16;
#define	SYS_USEC_DELAY				htim16
extern	TIM_HandleTypeDef 			htim17;
#define	SYSTICK_100MSEC				htim17
extern	TIM_HandleTypeDef 			htim14;
#define	DEBOUNCE_10MSEC				htim14
extern	TIM_HandleTypeDef 			htim2;
#define	BACKLIGHT_TIMER				htim2
#define	BACKLIGHT_TIMER_CHANNEL		TIM_CHANNEL_4
#define	BACKLIGHT_TIMER_REGISTER	BACKLIGHT_TIMER.Instance->CCR4
extern	TIM_HandleTypeDef 			htim4;
#define	ENCODER_TIMER				htim4
extern	TIM_HandleTypeDef 			htim7;
#define	SAMPLE_TIMER				htim7
extern	TIM_HandleTypeDef 			htim15;
#define	CONTROL_TIMER				htim15
extern	ADC_HandleTypeDef 			hadc1;
#define	CONTROL_DMA					hadc1
extern	DAC_HandleTypeDef 			hdac1;
#define	DAC_DMA						hdac1
extern	SAI_HandleTypeDef 			hsai_BlockA1;
#define	SPDIF_OUT					hsai_BlockA1
extern	DMA_HandleTypeDef 			hdma_sai1_a;
#define	SPDIF_OUT_DMA				hdma_sai1_a
extern	I2S_HandleTypeDef 			hi2s2;
#define	I2S_OUT_DMA					hi2s2

#define	DEBOUNCE_VALUE				3

#define	DAC_SIGNAL_LEN		512
#define	SPDIF_SIGNAL_LEN	512
#define	I2S_SIGNAL_LEN		512

#define	DRUM_MACHINE	1
#define	HAS_DELAY		1

typedef struct {
	uint8_t			timers_flag;
	uint8_t			tim100msec_counter;
	uint8_t			tim1Sec_counter;
	uint8_t			microsd_flags;
	uint8_t 		touch_flags;
	uint8_t 		touch_disable_window;
	uint8_t			encoder_sw_debounce;
	uint8_t			encoder_rotation_debounce;
	uint8_t			buttons_flags;
	uint8_t			sw1_debounce;
	uint8_t			sw2_debounce;
	uint8_t			current_brightness;
	uint8_t			audio_flags;
	uint8_t			control_flags;
	int16_t			buffer_length , hbuffer_length;
	int16_t			rvar[4];
	uint8_t			usb_flags;
	uint8_t			*usb_rx_ptr;
	uint16_t		usb_rx_len;
}SystemVar_TypeDef;

/* usb_flags */
#define	USB_RXPKT_READY				0x80

/* timers_flag */
#define	TIMER_100MS_FLAG			0x01
#define	TIMER_1SEC_FLAG				0x02
/* microsd_flags */
#define	MICROSD_NOT_PRESENT_FLAG	0x80
/* touch_flags */
#define	TOUCH_PENDOWN_FLAG			0x80
#define	TOUCH_CALIBRATED_FLAG		0x40
#define	TOUCH_PENDING_FLAG			0x01

/* buttons_flags */
#define	SW1_MASK					0x40
#define	SW2_MASK					0x80

/* audio_flags */
#define	AUDIO_DAC_HALF_FLAG			0x01
#define	AUDIO_DAC_READY_FLAG		0x02
#define	AUDIO_SPDIF_HALF_FLAG		0x04
#define	AUDIO_SPDIF_READY_FLAG		0x08
#define	AUDIO_I2S_HALF_FLAG			0x10
#define	AUDIO_I2S_READY_FLAG		0x20
#define	AUDIO_IRQ_FLAG				0x80

/* control_flags */
#define	CONTROL_ADC_HALF_FLAG		0x01
#define	CONTROL_ADC_READY_FLAG		0x02

#define		TOUCH_DISABLE_WINDOW	2	// 500 mSec delay after touch released the touch is considered not active

typedef struct {
	uint8_t			id[8];
	uint8_t			flash_capacity;
	uint16_t 		touch_x_minimum;
	uint16_t 		touch_x_correction;
	uint16_t 		touch_y_minimum;
	uint16_t 		touch_y_correction;
	uint16_t 		valid_params;
}SystemParameters_TypeDef;
/* valid_params */
#define	VALID_PARAMS_FLAG	0xbeef
#define	INVALID_PARAMS_FLAG	0xdead

extern	SystemVar_TypeDef			SystemVar;
extern	SystemParameters_TypeDef	SystemParameters;

extern	void bB2xx_Init(void);
extern	void bB2xx_Loop(void);
extern	void SysTimer100msecCallback(void);
extern	void bB2xx_Set_NO_MicroSD_Flag(void);
extern	uint32_t SysTimer100msecCheckExpired(void);
extern	void SysTimer100msecClearFlag(void);
extern	void SysDebounceTimer(void);
extern	void InitBackLight(void);

#endif /* SRC_BB2XX_BSP_CORE_BB2XX_SYSTEM_H_ */

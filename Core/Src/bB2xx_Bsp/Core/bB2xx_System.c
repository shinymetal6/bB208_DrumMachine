/*
 * bB2xx_System.c
 *
 *  Created on: Oct 18, 2021
 *      Author: fil
 */

#include "main.h"
#include "bB2xx_System.h"
#include <strings.h>

DTCM_RAM_AREA	__attribute__ ((aligned (16)))	SystemVar_TypeDef			SystemVar;
DTCM_RAM_AREA	__attribute__ ((aligned (16)))	SystemParameters_TypeDef	SystemParameters;

void SysLedActivities(uint8_t val)
{
	  HAL_GPIO_WritePin(SYSTEM_LED_GPIO_Port, SYSTEM_LED_Pin, val);
}

void SysTimer100msecCallback(void)
{
	SystemVar.timers_flag |= TIMER_100MS_FLAG;
	SystemVar.tim100msec_counter++;
	if ( SystemVar.tim100msec_counter >= 9 )
	{
		SystemVar.timers_flag |= TIMER_1SEC_FLAG;
		SysLedActivities(GPIO_PIN_RESET);
		SystemVar.tim100msec_counter = 0;
	}
	else
		SysLedActivities(GPIO_PIN_SET);
}

uint32_t SysTimer100msecCheckExpired(void)
{
	return SystemVar.timers_flag & TIMER_100MS_FLAG;
}

void SysTimer100msecClearFlag(void)
{
	SystemVar.timers_flag &= ~TIMER_100MS_FLAG;

}

void SysDebounceTimer(void)
{
	if ( SystemVar.encoder_sw_debounce != 0 )
	{
		if ( HAL_GPIO_ReadPin(ENCODER_BTN_GPIO_Port,ENCODER_BTN_Pin) == 0 )
		{
			SystemVar.encoder_sw_debounce--;
			if ( SystemVar.encoder_sw_debounce == 0 )
			{
				encoder_switch_callback();
			}
		}
		else
			SystemVar.encoder_sw_debounce = 0;
	}
	if ( SystemVar.encoder_rotation_debounce != 0 )
	{
		SystemVar.encoder_rotation_debounce--;
		if ( SystemVar.encoder_rotation_debounce == 0 )
		{
			encoder_rotation_callback();
		}
	}

	if ( SystemVar.sw1_debounce != 0 )
	{
		SystemVar.sw1_debounce--;
		if ( SystemVar.sw1_debounce == 0 )
		{
			SystemVar.buttons_flags |= SW1_MASK;
			sw1_callback();
		}
	}
	else
	{
		if ( HAL_GPIO_ReadPin(SW1_GPIO_Port,SW1_Pin) == 1 )
		{
			SystemVar.sw1_debounce = 0;
			SystemVar.buttons_flags &= ~SW1_MASK;
		}
	}
	if ( SystemVar.sw2_debounce != 0 )
	{
		SystemVar.sw2_debounce--;
		if ( SystemVar.sw2_debounce == 0 )
		{
			SystemVar.buttons_flags |= SW2_MASK;
			sw2_callback();
		}
	}
	else
	{
		if ( HAL_GPIO_ReadPin(SW2_GPIO_Port,SW2_Pin) == 1 )
		{
			SystemVar.sw2_debounce = 0;
			SystemVar.buttons_flags &= ~SW2_MASK;
		}
	}
}

void bB2xx_uSecDelay(uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&SYS_USEC_DELAY,0);  // set the counter value a 0
	while (__HAL_TIM_GET_COUNTER(&SYS_USEC_DELAY) < us);  // wait for the counter to reach the us input in the parameter
}

static void InitSystemTimers(void)
{
	HAL_TIM_Base_Start_IT(&SYSTICK_100MSEC);
	HAL_TIM_Base_Start_IT(&DEBOUNCE_10MSEC);
	HAL_TIM_Base_Start(&SYS_USEC_DELAY);
	HAL_TIM_Base_Start_IT(&CONTROL_TIMER);
	HAL_TIM_Encoder_Start_IT(&ENCODER_TIMER, TIM_CHANNEL_ALL);
}

void InitBackLight(void)
{
	HAL_TIM_PWM_Start(&BACKLIGHT_TIMER, BACKLIGHT_TIMER_CHANNEL);
	ILI9341_SetBrightness(0);
}

void InitSystemVars(void)
{
	bzero((uint8_t *)&SystemVar,sizeof(SystemVar));
}


__weak void application_init(void)
{

}

__weak void application_loop(void)
{

}

void bB2xx_Init(void)
{
	/*
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CYCCNT = 0;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
	*/
	InitSystemVars();
	InitSystemTimers();
	bB2xx_flash_init();
	bB2xx_flash_get_sysparams();
	InitADC();
	application_init();
}

void bB2xx_Set_NO_MicroSD_Flag(void)
{
	SystemVar.microsd_flags |= MICROSD_NOT_PRESENT_FLAG;
}

void bB2xx_Loop(void)
{
	application_loop();
}


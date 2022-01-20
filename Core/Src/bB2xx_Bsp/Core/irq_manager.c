/*
 * irq_manager.c
 *
 *  Created on: Nov 9, 2021
 *      Author: fil
 */

#include "main.h"
#include "irq_manager.h"

void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hsai)
{
	SPDIF_TxHalfCpltCallback();
}

void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsai)
{
	SPDIF_TxCpltCallback();
}

__weak	void ext_additional_timer_callbacks(TIM_HandleTypeDef *htim)
{

}

__weak	void sigin2_callbacks( uint16_t GPIO_Pin )
{

}

__weak	void controls_callbacks( void )
{

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if ( htim == &SYSTICK_100MSEC)
	{
		if (( SystemVar.buttons_flags & SW1_MASK ) == 0)
		{
			if ( HAL_GPIO_ReadPin(SW1_GPIO_Port,SW1_Pin) == 0 )
			{
				if ( SystemVar.sw1_debounce == 0 )
				{
					SystemVar.sw1_debounce = 3;
				}
			}
		}
		if (( SystemVar.buttons_flags & SW2_MASK ) == 0)
		{
			if ( HAL_GPIO_ReadPin(SW2_GPIO_Port,SW2_Pin) == 0 )
			{
				if ( SystemVar.sw2_debounce == 0 )
				{
					SystemVar.sw2_debounce = 3;
				}
			}
		}
		SysTimer100msecCallback();
	}
	if ( htim == &DEBOUNCE_10MSEC)
	{
		SysDebounceTimer();
	}
	ext_additional_timer_callbacks(htim);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if ( htim == &ENCODER_TIMER)
	{
		SystemVar.encoder_rotation_debounce = 3;
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if ( GPIO_Pin == ENCODER_BTN_Pin)
	{
		SystemVar.encoder_sw_debounce = 3;
	}
	if ( GPIO_Pin == TOUCH_IRQ_Pin)
	{
		ILI9341_TouchCallback();
	}
	if ( GPIO_Pin == TRIGGER_IN_Pin)
	{
		sigin2_callbacks(HAL_GPIO_ReadPin(TRIGGER_IN_GPIO_Port,TRIGGER_IN_Pin));
	}

}

void HAL_DAC_ConvHalfCpltCallbackCh1(DAC_HandleTypeDef *hdac)
{
	SystemVar.audio_flags |= (AUDIO_DAC_HALF_FLAG | AUDIO_DAC_READY_FLAG);
	DAC_IrqProcessSamples();
}

void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef *hdac)
{
	SystemVar.audio_flags &= ~AUDIO_DAC_HALF_FLAG;
	SystemVar.audio_flags |= AUDIO_DAC_READY_FLAG;
	DAC_IrqProcessSamples();
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
	SystemVar.control_flags = (CONTROL_ADC_HALF_FLAG | CONTROL_ADC_READY_FLAG);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	SystemVar.control_flags = CONTROL_ADC_READY_FLAG;
	controls_callbacks();
}


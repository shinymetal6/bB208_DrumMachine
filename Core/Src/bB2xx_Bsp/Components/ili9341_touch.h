/*
 * ili9341_touch.h
 *
 *  Created on: Oct 27, 2021
 *      Author: fil
 */

#ifndef SRC_BB2XX_BSP_COMPONENTS_ILI9341_TOUCH_H_
#define SRC_BB2XX_BSP_COMPONENTS_ILI9341_TOUCH_H_

#include <stdbool.h>
#include "main.h"

/*** Redefine if necessary ***/

// Warning! Use SPI bus with < 1.3 Mbit speed, better ~650 Kbit to be safe.
extern SPI_HandleTypeDef hspi1;
#define ILI9341_TOUCH_SPI_PORT hspi1

#define ILI9341_TOUCH_IRQ_Pin       TOUCH_IRQ_Pin // Arduino D5
#define ILI9341_TOUCH_IRQ_GPIO_Port TOUCH_IRQ_GPIO_Port
#define ILI9341_TOUCH_CS_Pin        TOUCH_CS_Pin // Arduino D2
#define ILI9341_TOUCH_CS_GPIO_Port  TOUCH_CS_GPIO_Port

// change depending on screen orientation
#define ILI9341_TOUCH_SCALE_X 240
#define ILI9341_TOUCH_SCALE_Y 320

// to calibrate uncomment UART_Printf line in ili9341_touch.c
#define ILI9341_TOUCH_MIN_RAW_X 1500
#define ILI9341_TOUCH_MAX_RAW_X 31000
#define ILI9341_TOUCH_MIN_RAW_Y 3276
#define ILI9341_TOUCH_MAX_RAW_Y 30110


extern	uint32_t ILI9341_TouchGetCoordinates(uint32_t* raw_x, uint32_t* raw_y);
extern	void ILI9341_TouchCallback(void);

#endif /* SRC_BB2XX_BSP_COMPONENTS_ILI9341_TOUCH_H_ */

/*
 * ili9341.h
 *
 *  Created on: Oct 21, 2021
 *      Author: fil
 */

#ifndef SRC_BB2XX_BSP_COMPONENTS_ILI9341_H_
#define SRC_BB2XX_BSP_COMPONENTS_ILI9341_H_

#include <Hmi/fonts.h>
#include <stdbool.h>

#define ILI9341_MADCTL_MY  0x80
#define ILI9341_MADCTL_MX  0x40
#define ILI9341_MADCTL_MV  0x20
#define ILI9341_MADCTL_ML  0x10
#define ILI9341_MADCTL_RGB 0x00
#define ILI9341_MADCTL_BGR 0x08
#define ILI9341_MADCTL_MH  0x04

/*** Redefine if necessary ***/
extern	SPI_HandleTypeDef 		hspi1;
#define ILI9341_SPI_PORT		hspi1
#define ILI9341_TOUCH_IRQ		EXTI9_5_IRQn

#define ILI9341_RES_Pin       	LCDRESET_Pin
#define ILI9341_RES_GPIO_Port 	LCDRESET_GPIO_Port
#define ILI9341_CS_Pin       	LCDCS_Pin
#define ILI9341_CS_GPIO_Port  	LCDCS_GPIO_Port
#define ILI9341_DC_Pin        	LCDDC_Pin
#define ILI9341_DC_GPIO_Port  	LCDDC_GPIO_Port

// default orientation

#define ILI9341_WIDTH  240
#define ILI9341_HEIGHT 320
#define ILI9341_ROTATION (ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR)

// rotate right
/*
#define ILI9341_WIDTH  320
#define ILI9341_HEIGHT 240
#define ILI9341_ROTATION (ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR)
*/

// rotate left
/*
#define ILI9341_WIDTH  320
#define ILI9341_HEIGHT 240
#define ILI9341_ROTATION (ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR)
*/

// upside down
/*
#define ILI9341_WIDTH  240
#define ILI9341_HEIGHT 320
#define ILI9341_ROTATION (ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR)
*/

/****************************/

#define	FILLSIZE	ILI9341_WIDTH*ILI9341_HEIGHT*2
#define	TXSIZE		8192


// Color definitions
#define	ILI9341_BLACK   0x0000
#define	ILI9341_BLUE    0x001F
#define	ILI9341_RED     0xF800
#define	ILI9341_GREEN   0x07E0
#define ILI9341_CYAN    0x07FF
#define ILI9341_MAGENTA 0xF81F
#define ILI9341_YELLOW  0xFFE0
#define ILI9341_WHITE   0xFFFF
#define ILI9341_COLOR565(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))

// call before initializing any SPI devices
extern	void ILI9341_Unselect();
extern	void ILI9341_Init(void);
extern	void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
extern	void ILI9341_WriteString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor);
extern	void ILI9341_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
extern	void ILI9341_FillScreen(uint16_t color);
extern	void ILI9341_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data);
extern	void ILI9341_InvertColors(uint8_t invert);
extern	void ILI9341_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
extern	void ILI9341_DrawCircle(uint16_t x0,uint16_t y0,uint16_t radius,uint16_t color);
extern	void ILI9341_SPI_TxEnd_Callback(void);
extern	void ILI9341_DrawBitmap(uint16_t x, uint16_t y, uint8_t *pbmp);
extern	void ILI9341_SetBrightness(uint8_t percent);


#endif /* SRC_BB2XX_BSP_COMPONENTS_ILI9341_H_ */

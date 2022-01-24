/*
 * sdcard.h
 *
 *  Created on: Nov 15, 2021
 *      Author: fil
 */

#ifndef SRC_DRUMMACHINE_DRUM_SDCARD_H_
#define SRC_DRUMMACHINE_DRUM_SDCARD_H_

typedef struct {
	uint32_t	valid;
	uint32_t	instrument_number;
	char		filename[16];
	uint8_t  	MidiKey;
}SDCardFiles_TypeDef;

typedef struct
{
    uint16_t  bfType ;     //File logo. Only the 'bm', used to identify BMP bitmap type 2
    uint32_t  bfSize ;       //File size, four bytes 4
    uint16_t  bfReserved1 ;//Reserved 2
    uint16_t  bfReserved2 ;//Reserved 2
    uint32_t  bfOffBits ;  //From the file start to the offset between the bitmap data, this is stored in this paragraph is file information 4
}BITMAPFILEHEADER ;            //Bitmap file header

//Bitmap information head
typedef struct
{
	uint32_t biSize ;           //Description The number of words required for the BitmapInfoheader structure.
	int32_t  biWidth ;           //Describe the width of the image, in pixels
	int32_t  biHeight ;       //Describe the height of the image, in pixels
    uint16_t  biPlanes ;       //For the target device, the number of positions, which will always be set to 1
    uint16_t  biBitCount ;       //Description Bit number / pixel, its value is 1, 4, 8, 16, 24, or 32
    uint32_t biCompression ;  //Describe the type of image data compression. Its value can be one of the following:
    /*Bi_RGB: No compression;
           * BI_RLE8: Each pixel 8 bit RLE compression encoding, compressed format consists of 2 bytes (repeat pixel count and color index);
           * BI_RLE4: RLE compression coding of each pixel 4 bit, compressed format consists of 2 bytes
               * Bi_bitfields: The bit of each pixel is determined by the specified mask.*/
    uint32_t biSizeImage ;//Describe the size of the image, in bytes. When using the BI_RGB format, it can be set to 0
    int32_t  biXPelsPerMeter ;//Description Level resolution, indicate pixels / meter
    int32_t  biYPelsPerMeter ;//Describe the vertical resolution, expressed with pixels / meter
    uint32_t biClrUsed ;       //Note the number of color indexes in color tables actually used by bitmaps
    uint32_t biClrImportant ; //Number of color indexes with important impact on image display, if 0, are important.
}BITMAPINFOHEADER;

extern	uint8_t CheckForUpdate(void);
extern	void CopySamplesFromSD(void);
extern	void CopySequenceFromSD(void);
extern	void CopyDigitsFromSD(void);
extern	void CopyIconsFromSD(void);

#endif /* SRC_DRUMMACHINE_DRUM_SDCARD_H_ */

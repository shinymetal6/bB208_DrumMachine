/*
 * sdcard.c
 *
 *  Created on: Nov 15, 2021
 *      Author: fil
 */


#include "main.h"
#include "drum_flash.h"
#include "../drum_machine.h"
#include "../Hmi/menus.h"
#include "sdcard.h"
#include "fatfs.h"
#include "drum_flash.h"
#include "instrument.h"
#include "delay.h"
#include "enc_btns_leds.h"
#include <stdio.h>
#include <string.h>

FATFS fs;  // file system
FIL fil; // File
FILINFO fno;
FRESULT fresult;  // result
UINT br, bw;  // File read/write count

/**** capacity related *****/
FATFS *pfs;
DWORD fre_clust;
uint32_t total, free_space;

/***** format related *****/
WAV_FormatTypeDef 				wav_info;
Instrument_TypeDef				Instrument[NUM_INSTRUMENTS],spi_Instrument;
FIL								WavFile[NUM_INSTRUMENTS],WFile,ConfFile,SeqFile,ImgFile;
uint8_t							line[256];


void DisableSampleTimer(void)
{
	SAMPLE_TIMER.Instance->CR1 &= ~TIM_CR1_CEN;
	HAL_Delay(100);
}

void EnableSampleTimer(void)
{
	SAMPLE_TIMER.Instance->CR1 |= TIM_CR1_CEN;
}

uint32_t Mount_SD (const TCHAR* path)
{
	fresult = f_mount(&fs, path, 0);
	if (fresult != FR_OK)
		return 1;
	return 0;
}

uint32_t Unmount_SD (const TCHAR* path)
{
	fresult = f_mount(NULL, path, 1);
	if (fresult != FR_OK)
		return 1;
	return 0;
}

FRESULT Check_File (char *filename)
{
	return f_stat (filename, &fno);
}

//RAMD1_RAM_AREA	uint8_t		spi_buf[SECTOR_SIZE],verify_spi_buf[SECTOR_SIZE];
RAMD1_RAM_AREA	uint16_t	spi_buf[SECTOR_SIZE/2],verify_spi_buf[SECTOR_SIZE/2];

uint8_t	check_if_wav_present(uint8_t instrument_number)
{
	w25qxx_ReadBytes((uint8_t *)&spi_Instrument, Instrument[instrument_number].HeaderPtr, sizeof(Instrument[instrument_number]));

	if ( spi_Instrument.FileSize != Instrument[instrument_number].FileSize )
		return 1;
	if ( spi_Instrument.SampleRate != Instrument[instrument_number].SampleRate )
		return 1;
	if ( spi_Instrument.MidiKey != Instrument[instrument_number].MidiKey )
		return 1;
	if ( strcmp((char *)spi_Instrument.WavName,(char *)Instrument[instrument_number].WavName) != 0 )
		return 1;
	return 0;
}

uint8_t	write_status_line[32];
uint8_t extrabuf[64];
uint8_t	wavline[5];

FRESULT Store_WavFile (char *filename,uint8_t instrument_number,uint8_t midi_key)
{
uint32_t 	bytesread,sector_number,instrument_address,i,byte_count;
	if(f_open(&WFile, (char *)filename, FA_OPEN_EXISTING | FA_READ) == FR_OK)
	{
		if(f_read(&WFile, &wav_info, sizeof(wav_info), (void *)&bytesread) == FR_OK)
		{
			wavline[0] = wav_info.SubChunk2ID & 0xff;
			wavline[1] = (wav_info.SubChunk2ID >> 8 ) & 0xff;
			wavline[2] = (wav_info.SubChunk2ID >> 16) & 0xff;
			wavline[3] = (wav_info.SubChunk2ID >> 24) & 0xff;
			wavline[4] = 0;
		    if ( strcmp("data",(char *)wavline) != 0)
		    {
		    	f_read(&WFile,extrabuf,wav_info.SubChunk2Size, (void *)&bytesread);
		    	f_read(&WFile,wavline,4, (void *)&bytesread);
		        if ( strcmp("data",(char *)wavline) != 0)
		            return FR_INVALID_OBJECT;
		    	f_read(&WFile,&byte_count,4, (void *)&bytesread);
		    }
			sprintf((char *)Instrument[instrument_number].WavName,(char *)filename);
			Instrument[instrument_number].FileFlag = FILE_FLAG_VALID;
			Instrument[instrument_number].MidiKey = midi_key;
			Instrument[instrument_number].FileSize = byte_count;
			Instrument[instrument_number].SampleRate = wav_info.SampleRate;
			Instrument[instrument_number].PlayedSize = 0;
			Instrument[instrument_number].Flags = 0;
			Instrument[instrument_number].HeaderPtr = INSTRUMENT_SIZE*instrument_number;
			Instrument[instrument_number].WavPtr    = Instrument[instrument_number].HeaderPtr+SECTOR_SIZE;

			sprintf((char *)write_status_line,"Erasing instrument %d",(int )instrument_number);
			write_status_line[23] = 0;
			WriteStatusLine((char *)write_status_line);

			for(i=0;i<INSTRUMENT_NUM64K_BLOCKS;i++)
				w25qxx_EraseBlock64k((instrument_number*INSTRUMENT_SIZE)+(INSTRUMENT_64K_BLOCK*i));
			w25qxx_WriteBytes((uint8_t *)&Instrument[instrument_number], Instrument[instrument_number].HeaderPtr, SECTOR_SIZE);

			instrument_address = Instrument[instrument_number].WavPtr;
			for(sector_number=1;sector_number < INSTRUMENT_SECTORS;sector_number++)
			{
				memset(write_status_line,0x20,32);
				write_status_line[23] = 0;
				sprintf((char *)write_status_line,"W %d %s %02x",(int )sector_number,(char *)Instrument[instrument_number].WavName,(int )Instrument[instrument_number].MidiKey);
				WriteStatusLine((char *)write_status_line);

				for(i=0;i<SECTOR_SIZE/2;i++)
					spi_buf[i] = NO_AUDIO_OUT;
				if(f_read(&WFile,	(uint8_t *)&spi_buf[0],SECTOR_SIZE,(void *)&bytesread) != FR_OK)
				{
					f_close(&WFile);
					LD1_OnOff(LED_ON);
					WriteStatusLine("Disk error");
					return FR_DISK_ERR;
				}
				if ( bytesread != 0 )
				{
					w25qxx_WriteBytes((uint8_t *)&spi_buf[0], instrument_address, SECTOR_SIZE);
					instrument_address += SECTOR_SIZE;
				}
				else
				{
					sector_number = INSTRUMENT_SECTORS;
				}
			}
			f_close(&WFile);
			return FR_OK;
		}
	}
	return FR_NO_FILE;
}


SDCardFiles_TypeDef	SDCardFiles[NUM_INSTRUMENTS];

FRESULT Read_SamplesFiles (char *filename)
{
uint32_t	i,len;
char 		*ret_val;

	if(f_open(&ConfFile, filename, FA_OPEN_EXISTING | FA_READ) != FR_OK)
		return FR_NO_FILE;
	__set_BASEPRI(1 << (8 - __NVIC_PRIO_BITS));
	for(i=0;i<NUM_INSTRUMENTS;i++)
	{
		SDCardFiles[i].valid = 0;
		ret_val = f_gets((char * )line,256,&ConfFile);
		if ( ret_val != NULL )
		{
			if ( (len = strlen((char * )line)) != 0 )
			{
				if ( sscanf((char * )line,"%d %s %2x",(int *)&SDCardFiles[i].instrument_number,SDCardFiles[i].filename,(int *)&SDCardFiles[i].MidiKey) == 3 )
				{
					SDCardFiles[i].valid = 1;
				}
			}
		}
	}
	f_close(&ConfFile);

	for(i=0;i<NUM_INSTRUMENTS;i++)
	{
		if ( SDCardFiles[i].valid == 1 )
		{
			if ( (fresult = Check_File(SDCardFiles[i].filename)) != FR_OK)
			    return FR_DISK_ERR;
			if ( Store_WavFile(SDCardFiles[i].filename,SDCardFiles[i].instrument_number,SDCardFiles[i].MidiKey) != FR_OK)
			{
				__set_BASEPRI(0);	// back to normal
				return FR_DISK_ERR;
			}
		}
	}
	__set_BASEPRI(0);	// back to normal
	return FR_OK;
}

void Read_SeqFile(char *filename)
{
uint32_t	params=0,j,val,line_idx=0,end=0;
uint8_t 	len;
uint8_t 	delay_type[16];
int		 	seq[16];

	if(f_open(&SeqFile, filename, FA_OPEN_EXISTING | FA_READ) != FR_OK)
		return;
	__set_BASEPRI(1 << (8 - __NVIC_PRIO_BITS));
	while(end == 0)
	{
		f_gets((char * )line,256,&SeqFile);
		if ( (len = strlen((char * )line)) != 0 )
		{
			if ( params == 0 )
			{
				if ( sscanf((char * )line,"BEAT %d",(int *)&seq[0]) == 1 )
				{
					if ( seq[0] <= MAX_BEAT)
						Sequencer_Descriptor.sequencer_beat = seq[0];
				}
				else if ( sscanf((char * )line,"DELAY %s %d %d",delay_type,(int *)&seq[0],(int *)&seq[1]) == 3 )
				{
					if ( strcmp((char * )delay_type,"ECHO") == 0 )
						Sequencer_Descriptor.delay_type = DELAY_TYPE_ECHO;
					else if ( strcmp((char * )delay_type,"FLANGER") == 0 )
						Sequencer_Descriptor.delay_type = DELAY_TYPE_FLANGER;
					else
						Sequencer_Descriptor.delay_type = DELAY_TYPE_NONE;
					if ( seq[0] <= MAX_DELAY)
						Sequencer_Descriptor.delay_value = seq[0];
					else
						Sequencer_Descriptor.delay_value = 0;
					if (( seq[1] <= MAX_DELAYW) && ( seq[1] >= MIN_DELAYW))
						Sequencer_Descriptor.delay_weight = seq[1];
					else
						Sequencer_Descriptor.delay_weight = (MAX_DELAYW+MIN_DELAYW)/2;
				}
				else if ( sscanf((char * )line,"BB208_SEQUENCE %d",(int *)&seq[0]) == 1 )
				{
					Sequencer_Descriptor.sequencer_length = seq[0];
					params = 1;
				}
			}
			else
			{
				if ( sscanf((char * )line,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",&seq[0],&seq[1],&seq[2],&seq[3],&seq[4],&seq[5],&seq[6],&seq[7],&seq[8],&seq[9],&seq[10],&seq[11],&seq[12],&seq[13],&seq[14],&seq[15]) == 16 )
				{
					val = 0;
					for(j=0;j<16;j++)
					{
						if ( seq[j] != 0 )
							val |= (1 << j);
					}
					Sequencer_Descriptor.sequencer_steps[line_idx] = val;
					line_idx++;
					if ( line_idx >= Sequencer_Descriptor.sequencer_length )
						end = 1;
				}
			}
		}
	}
	Sequencer_Descriptor.flag = 0xdead;
	sprintf((char *)Sequencer_Descriptor.header,"Seq_2xx");
	w25qxx_EraseSector(w25qxx_AddressToSector(USRSEQUENCE_PTR));
	w25qxx_WriteBytes((uint8_t *)&Sequencer_Descriptor, USRSEQUENCE_PTR, sizeof(Sequencer_Descriptor));
	f_close(&SeqFile);
	__set_BASEPRI(0);	// back to normal

	UserParameters.bpm = Sequencer_Descriptor.sequencer_beat;
	UserParameters.delay_value = Sequencer_Descriptor.delay_value;
	UserParameters.delay_weight = (float )Sequencer_Descriptor.delay_weight / 100.0F;;
	UserParameters.sequencer_length = Sequencer_Descriptor.sequencer_length;

	BPM_Value_Draw(0);
	Delay_Weight_Draw(0);
	Delay_Value_Draw(0);
}

void CopySamplesFromSD(void)
{
	DisableSampleTimer();
	Read_SamplesFiles("bb_sam.txt");
	WriteStatusLine("Samples copied");
	EnableSampleTimer();
}

void CopySequenceFromSD(void)
{
	DisableSampleTimer();
	Read_SeqFile("bb_seq.txt");
	WriteStatusLine("Sequence copied");
	EnableSampleTimer();
}

char filename[32];

FRESULT Read_DigitFile (char *filename,uint8_t *image_ptr)
{
uint32_t 	bytesread;

	if ( (fresult = Check_File(filename)) != FR_OK)
	    return fresult;
	if((fresult = f_open(&ImgFile, (char *)filename, FA_OPEN_EXISTING | FA_READ)) == FR_OK)
	{
		f_read(&ImgFile, image_ptr, DIGITS_SIZE, (void *)&bytesread);
		f_close(&ImgFile);
	}
	return fresult;
}

void CopyDigitsFromSD(void)
{
uint8_t		digit_number;
uint8_t		*image_ptr;

	DisableSampleTimer();
	WriteStatusLine("Reading digits ...");

	for(digit_number=0;digit_number<10;digit_number++)
	{
		sprintf(filename,"system/images/16x29/blue%d.bmp",digit_number);
		image_ptr = &normal_digits_22x40[digit_number][0];
		Read_DigitFile(filename,image_ptr);
		sprintf(filename,"system/images/16x29/red%d.bmp",digit_number);
		image_ptr = &hilight_digits_22x40[digit_number][0];
		Read_DigitFile(filename,image_ptr);
		sprintf(filename,"system/images/16x29/green%d.bmp",digit_number);
		image_ptr = &normal_digits_16x29[digit_number][0];
		Read_DigitFile(filename,image_ptr);
		sprintf(filename,"system/images/16x29/orange%d.bmp",digit_number);
		image_ptr = &hilight_digits_16x29[digit_number][0];
		Read_DigitFile(filename,image_ptr);
	}
	WriteStatusLine("Writing digits ...");
	drum_flash_store_digits();
	drum_flash_load_digits();
	EnableSampleTimer();
	BPM_Value_Draw(0);
	Delay_Weight_Draw(0);
	Delay_Value_Draw(0);
	WriteStatusLine("Digits copied");
}

uint8_t CheckForUpdate(void)
{
	if ( Mount_SD("/") != FR_OK)
	    return 1;
	//CopyDigitsFromSD();
	return 0;
}

#ifdef TOBEADDED
FRESULT Scan_SD (char* pat)
{
    DIR dir;
    UINT i;
    char *path = malloc(20*sizeof (char));
    sprintf (path, "%s",pat);

    fresult = f_opendir(&dir, path);                       /* Open the directory */
    if (fresult == FR_OK)
    {
        for (;;)
        {
            fresult = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (fresult != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR)     /* It is a directory */
            {
            	if (!(strcmp ("SYSTEM~1", fno.fname))) continue;
            	char *buf = malloc(30*sizeof(char));
            	sprintf (buf, "Dir: %s\r\n", fno.fname);
            	Send_Uart(buf);
            	free(buf);
                i = strlen(path);
                sprintf(&path[i], "/%s", fno.fname);
                fresult = Scan_SD(path);                     /* Enter the directory */
                if (fresult != FR_OK) break;
                path[i] = 0;
            }
            else
            {   /* It is a file. */
           	   char *buf = malloc(30*sizeof(char));
               sprintf(buf,"File: %s/%s\n", path, fno.fname);
               Send_Uart(buf);
               free(buf);
            }
        }
        f_closedir(&dir);
    }
    free(path);
    return fresult;
}

/* Only supports removing files from home directory */
FRESULT Format_SD (void)
{
    DIR dir;
    char *path = malloc(20*sizeof (char));
    sprintf (path, "%s","/");

    fresult = f_opendir(&dir, path);                       /* Open the directory */
    if (fresult == FR_OK)
    {
        for (;;)
        {
            fresult = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (fresult != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR)     /* It is a directory */
            {
            	if (!(strcmp ("SYSTEM~1", fno.fname))) continue;
            	fresult = f_unlink(fno.fname);
            	if (fresult == FR_DENIED) continue;
            }
            else
            {   /* It is a file. */
               fresult = f_unlink(fno.fname);
            }
        }
        f_closedir(&dir);
    }
    free(path);
    return fresult;
}




FRESULT Write_File (char *name, char *data)
{

	/**** check whether the file exists or not ****/
	fresult = f_stat (name, &fno);
	if (fresult != FR_OK)
	{
		char *buf = malloc(100*sizeof(char));
		sprintf (buf, "ERROR!!! *%s* does not exists\n\n", name);
		Send_Uart (buf);
	    free(buf);
	    return fresult;
	}

	else
	{
	    /* Create a file with read write access and open it */
	    fresult = f_open(&fil, name, FA_OPEN_EXISTING | FA_WRITE);
	    if (fresult != FR_OK)
	    {
	    	char *buf = malloc(100*sizeof(char));
	    	sprintf (buf, "ERROR!!! No. %d in opening file *%s*\n\n", fresult, name);
	    	Send_Uart(buf);
	        free(buf);
	        return fresult;
	    }

	    else
	    {
	    	fresult = f_write(&fil, data, strlen(data), &bw);
	    	if (fresult != FR_OK)
	    	{
	    		char *buf = malloc(100*sizeof(char));
	    		sprintf (buf, "ERROR!!! No. %d while writing to the FILE *%s*\n\n", fresult, name);
	    		Send_Uart(buf);
	    		free(buf);
	    	}

	    	/* Close file */
	    	fresult = f_close(&fil);
	    	if (fresult != FR_OK)
	    	{
	    		char *buf = malloc(100*sizeof(char));
	    		sprintf (buf, "ERROR!!! No. %d in closing file *%s* after writing it\n\n", fresult, name);
	    		Send_Uart(buf);
	    		free(buf);
	    	}
	    	else
	    	{
	    		char *buf = malloc(100*sizeof(char));
	    		sprintf (buf, "File *%s* is WRITTEN and CLOSED successfully\n", name);
	    		Send_Uart(buf);
	    		free(buf);
	    	}
	    }
	    return fresult;
	}
}

FRESULT Read_File (char *name)
{
	/**** check whether the file exists or not ****/
	fresult = f_stat (name, &fno);
	if (fresult != FR_OK)
	{
		char *buf = malloc(100*sizeof(char));
		sprintf (buf, "ERRROR!!! *%s* does not exists\n\n", name);
		Send_Uart (buf);
		free(buf);
	    return fresult;
	}

	else
	{
		/* Open file to read */
		fresult = f_open(&fil, name, FA_READ);

		if (fresult != FR_OK)
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "ERROR!!! No. %d in opening file *%s*\n\n", fresult, name);
		    Send_Uart(buf);
		    free(buf);
		    return fresult;
		}

		/* Read data from the file
		* see the function details for the arguments */

		char *buffer = malloc(sizeof(f_size(&fil)));
		fresult = f_read (&fil, buffer, f_size(&fil), &br);
		if (fresult != FR_OK)
		{
			char *buf = malloc(100*sizeof(char));
			free(buffer);
		 	sprintf (buf, "ERROR!!! No. %d in reading file *%s*\n\n", fresult, name);
		  	Send_Uart(buffer);
		  	free(buf);
		}

		else
		{
			Send_Uart(buffer);
			free(buffer);

			/* Close file */
			fresult = f_close(&fil);
			if (fresult != FR_OK)
			{
				char *buf = malloc(100*sizeof(char));
				sprintf (buf, "ERROR!!! No. %d in closing file *%s*\n\n", fresult, name);
				Send_Uart(buf);
				free(buf);
			}
			else
			{
				char *buf = malloc(100*sizeof(char));
				sprintf (buf, "File *%s* CLOSED successfully\n", name);
				Send_Uart(buf);
				free(buf);
			}
		}
	    return fresult;
	}
}

FRESULT Create_File (char *name)
{
	fresult = f_stat (name, &fno);
	if (fresult == FR_OK)
	{
		char *buf = malloc(100*sizeof(char));
		sprintf (buf, "ERROR!!! *%s* already exists!!!!\n use Update_File \n\n",name);
		Send_Uart(buf);
		free(buf);
	    return fresult;
	}
	else
	{
		fresult = f_open(&fil, name, FA_CREATE_ALWAYS|FA_READ|FA_WRITE);
		if (fresult != FR_OK)
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "ERROR!!! No. %d in creating file *%s*\n\n", fresult, name);
			Send_Uart(buf);
			free(buf);
		    return fresult;
		}
		else
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "*%s* created successfully\n Now use Write_File to write data\n",name);
			Send_Uart(buf);
			free(buf);
		}

		fresult = f_close(&fil);
		if (fresult != FR_OK)
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "ERROR No. %d in closing file *%s*\n\n", fresult, name);
			Send_Uart(buf);
			free(buf);
		}
		else
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "File *%s* CLOSED successfully\n", name);
			Send_Uart(buf);
			free(buf);
		}
	}
    return fresult;
}

FRESULT Update_File (char *name, char *data)
{
	/**** check whether the file exists or not ****/
	fresult = f_stat (name, &fno);
	if (fresult != FR_OK)
	{
		char *buf = malloc(100*sizeof(char));
		sprintf (buf, "ERROR!!! *%s* does not exists\n\n", name);
		Send_Uart (buf);
		free(buf);
	    return fresult;
	}

	else
	{
		 /* Create a file with read write access and open it */
	    fresult = f_open(&fil, name, FA_OPEN_APPEND | FA_WRITE);
	    if (fresult != FR_OK)
	    {
	    	char *buf = malloc(100*sizeof(char));
	    	sprintf (buf, "ERROR!!! No. %d in opening file *%s*\n\n", fresult, name);
	    	Send_Uart(buf);
	        free(buf);
	        return fresult;
	    }

	    /* Writing text */
	    fresult = f_write(&fil, data, strlen (data), &bw);
	    if (fresult != FR_OK)
	    {
	    	char *buf = malloc(100*sizeof(char));
	    	sprintf (buf, "ERROR!!! No. %d in writing file *%s*\n\n", fresult, name);
	    	Send_Uart(buf);
	    	free(buf);
	    }

	    else
	    {
	    	char *buf = malloc(100*sizeof(char));
	    	sprintf (buf, "*%s* UPDATED successfully\n", name);
	    	Send_Uart(buf);
	    	free(buf);
	    }

	    /* Close file */
	    fresult = f_close(&fil);
	    if (fresult != FR_OK)
	    {
	    	char *buf = malloc(100*sizeof(char));
	    	sprintf (buf, "ERROR!!! No. %d in closing file *%s*\n\n", fresult, name);
	    	Send_Uart(buf);
	    	free(buf);
	    }
	    else
	    {
	    	char *buf = malloc(100*sizeof(char));
	    	sprintf (buf, "File *%s* CLOSED successfully\n", name);
	    	Send_Uart(buf);
	    	free(buf);
	     }
	}
    return fresult;
}

FRESULT Remove_File (char *name)
{
	/**** check whether the file exists or not ****/
	fresult = f_stat (name, &fno);
	if (fresult != FR_OK)
	{
		char *buf = malloc(100*sizeof(char));
		sprintf (buf, "ERROR!!! *%s* does not exists\n\n", name);
		Send_Uart (buf);
		free(buf);
		return fresult;
	}

	else
	{
		fresult = f_unlink (name);
		if (fresult == FR_OK)
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "*%s* has been removed successfully\n", name);
			Send_Uart (buf);
			free(buf);
		}

		else
		{
			char *buf = malloc(100*sizeof(char));
			sprintf (buf, "ERROR No. %d in removing *%s*\n\n",fresult, name);
			Send_Uart (buf);
			free(buf);
		}
	}
	return fresult;
}

FRESULT Create_Dir (char *name)
{
    fresult = f_mkdir(name);
    if (fresult == FR_OK)
    {
    	char *buf = malloc(100*sizeof(char));
    	sprintf (buf, "*%s* has been created successfully\n", name);
    	Send_Uart (buf);
    	free(buf);
    }
    else
    {
    	char *buf = malloc(100*sizeof(char));
    	sprintf (buf, "ERROR No. %d in creating directory *%s*\n\n", fresult,name);
    	Send_Uart(buf);
    	free(buf);
    }
    return fresult;
}

void Check_SD_Space (void)
{
    /* Check free space */
    f_getfree("", &fre_clust, &pfs);

    total = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
    char *buf = malloc(30*sizeof(char));
    sprintf (buf, "SD CARD Total Size: \t%lu\n",total);
    Send_Uart(buf);
    free(buf);
    free_space = (uint32_t)(fre_clust * pfs->csize * 0.5);
    buf = malloc(30*sizeof(char));
    sprintf (buf, "SD CARD Free Space: \t%lu\n",free_space);
    Send_Uart(buf);
    free(buf);
}

#endif

/* 
 * 240p Test Suite
 * Copyright (C)2014 Artemio Urbina (Wii GX)
 *
 * This file is part of the 240p Test Suite
 *
 * The 240p Test Suite is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The 240p Test Suite is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public LicenseLoadPCMToBuffer
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
 

#include <fat.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <mxml.h>
#include "options.h"
#include "image.h"
#include "controller.h"
#include "font.h"


#define SD_OPTIONS_PATH "sd:/AudioPlayer"
#define OPTIONS_FILE "options.xml"
#define FS_SD	1

u8 FSActive = 0;

#ifdef WII_VERSION

#include <sdcard/wiisd_io.h>

#define USB_OPTIONS_PATH "usb:/AudioPlayer"
#define FS_USB	2

bool InitSD()
{
	if(!__io_wiisd.startup() || !__io_wiisd.isInserted())
		return false;

	return(fatMountSimple("sd", &__io_wiisd));
}
 
void DeInitSD() 
{
	fatUnmount("sd:/");
	__io_wiisd.shutdown();
} 

bool InitUSB()
{ 
	bool isMounted = false;
	bool isInserted = false;
	
	time_t start = time(0);

	while(time(0) - start < 10) // 10 sec
	{
		if(__io_usbstorage.startup() && __io_usbstorage.isInserted())
				break;
		usleep(200000); // 1/5 sec
	}
	
	if(!__io_usbstorage.startup() || !__io_usbstorage.isInserted())
		return false;
	
	isMounted = fatMountSimple("usb", &__io_usbstorage);	
	
	if(!isMounted)
	{
		fatUnmount("usb:/");
		isMounted = fatMountSimple("usb", &__io_usbstorage);	
		isInserted = __io_usbstorage.isInserted();
		if(isInserted)
		{
			int retry = 10;
 
			while(retry)
			{  
				isMounted = fatMountSimple("usb", &__io_usbstorage);
				if(isMounted) 
					break;
				sleep(1);
				retry--; 
			}
		}
	}
	
	return(isMounted);
} 

void DeInitUSB()
{
	fatUnmount("usb:/");
	__io_usbstorage.shutdown(); 
}


u8 InitFS()
{
	int fatMounted = 0;
	int	fsType = 0;
	char *path = NULL;
			
	if(FSActive)
		return 1;
	
	if(InitSD())
	{
		fatMounted = 1;
		path = SD_OPTIONS_PATH;
		fsType = FS_SD;
	}
	else
	{
		if(InitUSB())
		{
			fatMounted = 1;
			path = USB_OPTIONS_PATH;
			fsType = FS_USB;
		}
	}
	
	if (fatMounted)
	{			
		DIR *dir = NULL;
				
		dir = opendir(path);
		if (dir) 
		{			
			closedir(dir);
		}
		else
		{		
			mkdir(path, S_IRWXU);
		}
		chdir(path);
		FSActive = fsType;
	}
	else
		FSActive = 0;
		
	return FSActive;
}

void CloseFS()
{	
	if(FSActive)
	{
		if(FSActive == FS_SD)
			DeInitSD();
		if(FSActive == FS_USB)
			DeInitUSB();
		FSActive = 0;
	}
}
#endif

#ifdef GC_VERSION

#define FS_SDA	3
#define FS_SDB	4
#define FS_SD2	5

#include <sdcard/gcsd.h>

bool InitSDA()
{
	if(!__io_gcsda.startup() || !__io_gcsda.isInserted())
		return false;

	return(fatMountSimple("sd", &__io_gcsda));
}

bool InitSDB()
{
	if(!__io_gcsdb.startup() || !__io_gcsdb.isInserted())
		return false;

	return(fatMountSimple("sd", &__io_gcsdb));
}

bool InitSD2()
{
	if(!__io_gcsd2.startup() || !__io_gcsd2.isInserted())
		return false;

	return(fatMountSimple("sd", &__io_gcsd2));
}

void DeInitSDA() 
{
	fatUnmount("sd");
	__io_gcsda.shutdown();
}

void DeInitSDB() 
{
	fatUnmount("sd");
	__io_gcsdb.shutdown();
} 

void DeInitSD2() 
{
	fatUnmount("sd");
	__io_gcsd2.shutdown();
} 

u8 InitFS()
{
	int fatMounted = 0;
	int	fsType = 0;
	char *path = NULL;
			
	if(FSActive)
		return 1;
	
	if(InitSDA())
	{
		fatMounted = 1;
		path = SD_OPTIONS_PATH;
		fsType = FS_SDA;
	}
	else if(InitSDB())
	{
		fatMounted = 1;
		path = SD_OPTIONS_PATH;
		fsType = FS_SDB;
	}
	else if(InitSD2())
	{
		fatMounted = 1;
		path = SD_OPTIONS_PATH;
		fsType = FS_SD2;
	}
	
	if (fatMounted)
	{			
		DIR *dir = NULL;
				
		dir = opendir(path);
		if (dir) 
		{			
			closedir(dir);
		}
		else
		{		
			mkdir(path, S_IRWXU);
		}
		chdir(path);
		FSActive = fsType;
	}
	else
		FSActive = 0;
		
	return FSActive;
}

void CloseFS()
{	
	if(FSActive)
	{
		if(FSActive == FS_SDA)
			DeInitSDA();
		if(FSActive == FS_SDB)
			DeInitSDB();
		if(FSActive == FS_SD2)
			DeInitSD2();
		FSActive = 0;
	}
}

#endif

u8 FileExists(char *filename)
{
	FILE *file = NULL;
	
	if(!InitFS())
		return 0;
	
	file = fopen(filename, "r");
	if(!file)
	{
		CloseFS();
		return 0;
	}
	
	fclose(file);
	CloseFS();
	return 1;
}

/********************************************************/
/* WAV data structures */

typedef struct	RIFF_HEADER
{
	/* RIFF Chunk Descriptor */
	uint8_t 		RIFF[4];		/* RIFF Header Magic header */
	uint32_t		ChunkSize;		/* RIFF Chunk Size */
	uint8_t 		WAVE[4];		/* WAVE Header */
} riff_hdr;

typedef struct	SYB_CHUNK
{
	/* sub-chunk */
	uint8_t 		chunkID[4]; 	/* Chunk ID */
	uint32_t		Size;			/* Size of the SubChunk */
} sub_chunk;

#define	WAVE_FORMAT_PCM			0x0001
#define	WAVE_FORMAT_IEEE_FLOAT	0x0003
#define	WAVE_FORMAT_ALAW		0x0006
#define	WAVE_FORMAT_MULAW		0x0007
#define WAVE_FORMAT_EXTENSIBLE	0xFFFE

#define	FMT_TYPE_1	16
#define	FMT_TYPE_2	18
#define	FMT_TYPE_3	40

#define	FMT_TYPE_1_SIZE	0
#define	FMT_TYPE_2_SIZE	2
#define	FMT_TYPE_3_SIZE	24

#define	FMT_EXTRA_SIZE 24

typedef struct	FMT_HEADER
{
	/* "fmt" sub-chunk*/
	uint8_t 		fmt[4]; 		/* FMT header */
	uint32_t		Subchunk1Size;	/* Size of the fmt chunk */
	uint16_t		AudioFormat;	/* Audio format 1=PCM,6=mulaw,7=alaw,	  257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM */
	uint16_t		NumOfChan;		/* Number of channels 1=Mono 2=Sterio */
	uint32_t		SamplesPerSec;	/* Sampling Frequency in Hz */
	uint32_t		bytesPerSec;	/* bytes per second */
	uint16_t		blockAlign; 	/* 2=16-bit mono, 4=16-bit stereo */
	uint16_t		bitsPerSample;	/* Number of bits per sample */
} fmt_hdr;

typedef struct	FMT_HEADER_EXT1
{
	/* "fmt" sub-chunk */
	uint16_t		extSize;		/* Size of the extension: 18 */
} fmt_hdr_ext1;

typedef struct	FMT_HEADER_EXT2
{
	/* "fmt" sub-chunk */
	uint16_t		extSize;		/* Size of the extension: 22 */
	uint16_t		wValidBitsPerSample; /* at most 8*M */
	uint32_t		dwChannelMask;	/* Speaker position mask */
	/*	GUID (16 bytes, first two bytes are the data format code) */
	uint16_t		formatCode; /* formatcode */
	uint8_t			SubFormat[14];
} fmt_hdr_ext2;

typedef struct	DATA_HEADER
{
	/* "data" sub-chunk */
	uint8_t 		DataID[4]; /* "data"  string */
	uint32_t		DataSize;	/* Sampled data length */
} data_hdr;

typedef struct	FACT_CK
{
	/* "fact" sub-chunk for WAVE_FORMAT_EXTENSIBLE */
	uint8_t 		DataID[4]; /* "fact"  string */
	uint32_t		DataSize;	/* Chunk size: minimum 4 */
	uint32_t		dwSampleLength;  /* Number of samples (per channel) */
} fact_ck;

typedef struct	WAV_HEADER
{
	riff_hdr	riff;
	fmt_hdr		fmt;
	data_hdr	data;
} wav_hdr;

/********************************************************/

uint32_t EndianCorrect32bits(uint32_t num)
{
	uint32_t swapped = 0;
	
	swapped = ((num>>24)&0xff) | // move byte 3 to byte 0
			  ((num<<8)&0xff0000) | // move byte 1 to byte 2
			  ((num>>8)&0xff00) | // move byte 2 to byte 1
			  ((num<<24)&0xff000000); // byte 0 to byte 3
	return swapped;
}

uint16_t EndianCorrect16bits(uint16_t num)
{
	uint16_t swapped = 0;
	
	swapped = (num>>8) | (num<<8);
	return swapped;
}

void swapPCMEndianess(uint16_t *samples, size_t size)
{
	size_t i = 0;
	
	for(i = 0; i < size; i++)
		samples[i] = EndianCorrect16bits(samples[i]);
		
}

void cleanup(FILE *file, int *errType, int val)
{
	fclose(file);
	CloseFS();
	*errType = val;
}

#define HEADER_BUFFER 4096

int copyFromBuffer(void *dest, uint8_t *buffer, size_t size, int *buffpos, FILE *file, int *errType, int errCode)
{
	if(*buffpos + size > HEADER_BUFFER)
	{
		cleanup(file, errType, errCode);
		free(buffer);
		return 0;
	}
	
	memcpy(dest, buffer+*buffpos, size);
	*buffpos += size;
	return 1;
}

char *errors[] = {
	"Could not initialize FS",
	"Could not open file",
	"Not enough memory for header",
	"Could not read from file",
	"Could not copy from buffer 4",
	"Not a RIFF file",
	"Not a WAVE file",
	"Could not copy from buffer 7",
	"Could not copy from buffer 8",
	"File is not Stereo",
	"Not a PCM file",
	"Not a 16 bit file",
	"File is not at 48khz",
	"Could not copy from buffer 13",
	"Could not copy from buffer 14",
	"Could not copy from buffer 15",
	"Wave extensible not supported on GC",
	"Could not copy from buffer 17",
	"Empty Data Chunk in WAV file",
	"Could not advance to Data in WAV file",
	"Not enough RAM (File too big)",
	"Could not read Data in WAV file",
	NULL
};

uint8_t *LoadPCMToBuffer(char *filename, ulong *size, int *errType)
{
	FILE *file = NULL;
	ulong file_size = 0;
	uint8_t *file_buffer = NULL, *hbuffer = NULL;
	riff_hdr riff;
	fmt_hdr fmt;
	fmt_hdr_ext1 fmtExtra;
	fmt_hdr_ext2 fmtExtra2;
	data_hdr data;
	int found = 0, buffpos = 0;
	
	*errType = 0;
	
	if(!InitFS())
		return NULL;

	file = fopen(filename, "rb");
	if(!file)
	{
		CloseFS();
		*errType = 1;
		return NULL;
	}
	
	hbuffer = (uint8_t*)malloc(sizeof(uint8_t)*HEADER_BUFFER);
	if(!hbuffer)
	{
		cleanup(file, errType, 2);
		return NULL;
	}
	
	if(fread(hbuffer, sizeof(uint8_t), HEADER_BUFFER, file) != sizeof(uint8_t)*HEADER_BUFFER)
	{
		cleanup(file, errType, 3);
		return NULL;
	}
	
	rewind(file);
	
	if(!copyFromBuffer(&riff, hbuffer, sizeof(riff_hdr), &buffpos, file, errType, 4))
		return NULL;	
	
	fmt.Subchunk1Size = EndianCorrect32bits(fmt.Subchunk1Size);
	if(strncmp((char*)riff.RIFF, "RIFF", 4) != 0)
	{
		cleanup(file, errType, 5);
		return NULL;
	}
	
	if(strncmp((char*)riff.WAVE, "WAVE", 4) != 0)
	{
		cleanup(file, errType, 6);
		return NULL;
	}
	
	// look for fmt chunk
	found = 0;
	do
	{
		sub_chunk	schunk;

		if(!copyFromBuffer(&schunk, hbuffer, sizeof(sub_chunk), &buffpos, file, errType, 7))
			return NULL;
		
		schunk.Size = EndianCorrect32bits(schunk.Size);
		if(strncmp((char*)schunk.chunkID, "fmt", 3) != 0)
		{
			buffpos += schunk.Size*sizeof(uint8_t);
		}
		else
		{
			buffpos -= sizeof(sub_chunk);
			found = 1;
		}
	}while(!found);
	
	if(!copyFromBuffer(&fmt, hbuffer, sizeof(fmt_hdr), &buffpos, file, errType, 8))
		return NULL;
		
	fmt.AudioFormat = EndianCorrect16bits(fmt.AudioFormat);
	fmt.NumOfChan = EndianCorrect16bits(fmt.NumOfChan);
	fmt.blockAlign = EndianCorrect16bits(fmt.blockAlign);
	fmt.bitsPerSample = EndianCorrect16bits(fmt.bitsPerSample);
	
	fmt.Subchunk1Size = EndianCorrect32bits(fmt.Subchunk1Size);
	fmt.SamplesPerSec = EndianCorrect32bits(fmt.SamplesPerSec);
	fmt.bytesPerSec = EndianCorrect32bits(fmt.bytesPerSec);
	
	if(fmt.NumOfChan != 2)
	{
		cleanup(file, errType, 9);
		return NULL;
	}
	
	if(fmt.AudioFormat != WAVE_FORMAT_PCM)
	{
		cleanup(file, errType, 10);
		return NULL;
	}
	
	if(fmt.bitsPerSample != 16)
	{
		cleanup(file, errType, 11);
		return NULL;
	}
	
	if(fmt.SamplesPerSec != 48000)
	{
		cleanup(file, errType, 12);
		return NULL;
	}
	
	switch(fmt.Subchunk1Size)
	{
		case FMT_TYPE_1:
			break;
		case FMT_TYPE_2:
			if(!copyFromBuffer(&fmtExtra, hbuffer, sizeof(fmt_hdr_ext1), &buffpos, file, errType, 13))
				return NULL;
			break;
		case FMT_TYPE_3:
			if(!copyFromBuffer(&fmtExtra2, hbuffer, sizeof(fmt_hdr_ext2), &buffpos, file, errType, 14))
				return NULL;
			break;
		default:
			if(fmt.Subchunk1Size + 8 > sizeof(fmt_hdr))  // Add the fmt and chunksize length: 8 bytes
				buffpos += fmt.Subchunk1Size + 8 - sizeof(fmt_hdr);
			break;
	}
	
	// look for data chunk
	found = 0;
	do
	{
		sub_chunk	schunk;

		if(!copyFromBuffer(&schunk, hbuffer, sizeof(sub_chunk), &buffpos, file, errType, 15))
			return NULL;
		schunk.Size = EndianCorrect32bits(schunk.Size);
		if(strncmp((char*)schunk.chunkID, "data", 4) != 0)
		{
			buffpos += schunk.Size*sizeof(uint8_t);
		}
		else
		{
			buffpos -= sizeof(sub_chunk);
			found = 1;
		}

		// Read Total Fact if available
		if(fmt.AudioFormat == WAVE_FORMAT_EXTENSIBLE)
		{			
			cleanup(file, errType, 16);
			return NULL;
		}
	}while(!found);
	
	if(!copyFromBuffer(&data, hbuffer, sizeof(data_hdr), &buffpos, file, errType, 17))
		return NULL;
	
	data.DataSize = EndianCorrect32bits(data.DataSize);
	if(data.DataSize == 0)
	{
		cleanup(file, errType, 18);
		return NULL;
	}
	
	// advance file to position
	if(fread(hbuffer, sizeof(uint8_t), buffpos, file) != sizeof(uint8_t)*buffpos)
	{
		free(hbuffer);
		cleanup(file, errType, 19);
		return NULL;
	}
	
	free(hbuffer);
	hbuffer= NULL;

	file_size = data.DataSize;
	
	file_buffer = (uint8_t*)malloc(sizeof(uint8_t)*file_size);
	if(!file_buffer)
	{		
		cleanup(file, errType, 20);
		return NULL;
	}
	
	if(fread(file_buffer, sizeof(uint8_t), file_size, file) != file_size) 
	{		
		cleanup(file, errType, 21);
		return NULL;
	}
	
	swapPCMEndianess((uint16_t*)file_buffer, file_size/2);
	
	fclose(file);
	CloseFS();
	
	*size = file_size;
	return file_buffer;
}

uint8_t *LoadFileToBuffer(char *filename, ulong *size)
{
	FILE *file = NULL;
	ulong file_size = 0;
	uint8_t *file_buffer = NULL;
	
	if(!InitFS())
		return NULL;

	file = fopen(filename, "r");
	if(!file)
	{
		CloseFS();
		return NULL;
	}
	fseek(file , 0, SEEK_END);
	file_size = ftell(file);	
	rewind(file);
	
	if(!file_size)
	{		
		fclose(file);
		CloseFS();
		return NULL;
	}
	
	file_buffer = (uint8_t*)malloc(sizeof(uint8_t)*file_size);
	if(!file_buffer)
	{		
		fclose(file);
		CloseFS();
		return NULL;
	}
	
	if(fread(file_buffer, sizeof(uint8_t), file_size, file) != file_size) 
	{		
		fclose(file);
		CloseFS();
		return NULL;
	}
	
	fclose(file);
	CloseFS();
	
	*size = file_size;
	return file_buffer;
}

u8 LoadFileToMemoryAddress(char *filename, ulong *size, void *memory, ulong memsize)
{
	FILE *file = NULL;
	ulong file_size = 0;
	
	if(!InitFS())
		return 0;

	file = fopen(filename, "r");
	if(!file)
	{
		CloseFS();
		return 0;
	}
	fseek(file , 0, SEEK_END);
	file_size = ftell(file);	
	rewind(file);
	
	if(!file_size)
	{		
		fclose(file);
		CloseFS();
		return 0;
	}
	
	if(file_size > memsize)
	{
		fclose(file);
		CloseFS();
		return 0;
	}
	
	if(fread(memory, sizeof(char), file_size, file) != file_size) 
	{		
		fclose(file);
		CloseFS();
		return 0;
	}
	
	fclose(file);
	CloseFS();
	
	*size = file_size;
	return 1;
}

u8 LoadOptions()
{
	/*
	FILE *file = NULL;	
	long file_size = 0;
	mxml_node_t *xml;
	mxml_node_t *node;	
	
	if(!InitFS())
		return 0;	
		
	file = fopen(OPTIONS_FILE, "r");
	if(!file)
	{
		CloseFS();
		return 0;
	}
		
	fseek(file , 0, SEEK_END);
	file_size = ftell(file);	
	rewind(file);
	
	if(!file_size)
	{		
		fclose(file);
		CloseFS();
		return 0;
	}
		
	xml = mxmlLoadFile(NULL, file, MXML_INTEGER_CALLBACK);
	fclose(file);
	
	if(!xml)
	{
		CloseFS();
		return 0;	
	}
	   	
	node = mxmlFindElement(xml, xml, "Activate480p", NULL, NULL, MXML_DESCEND);
	if (node && mxmlGetType(node) == MXML_ELEMENT && !strcmp(mxmlGetText(node, 0), "Activate480p"))
	{		
		if(node->child)
			Options.Activate480p = node->child->value.integer;			
	}
		
	node = mxmlFindElement(xml, xml, "UseTrapFilter", NULL, NULL, MXML_DESCEND);
	if (node && mxmlGetType(node) == MXML_ELEMENT && !strcmp(node->value.element.name, "UseTrapFilter"))
	{		
		if(node->child)
			Options.TrapFilter = node->child->value.integer;					
	}
#ifdef WII_VERSION	
	node = mxmlFindElement(xml, xml, "UseDeflickerFilter", NULL, NULL, MXML_DESCEND);
	if (node && node->type == MXML_ELEMENT && !strcmp(node->value.element.name, "UseDeflickerFilter"))
	{		
		if(node->child)
			Options.FlickerFilter = node->child->value.integer;					
	}
	
	node = mxmlFindElement(xml, xml, "SFCClassicController", NULL, NULL, MXML_DESCEND);
	if (node && node->type == MXML_ELEMENT && !strcmp(node->value.element.name, "SFCClassicController"))
	{		
		if(node->child)
			Options.SFCClassicController = node->child->value.integer;			
	}
#endif
	
	node = mxmlFindElement(xml, xml, "ScanlineEvenOdd", NULL, NULL, MXML_DESCEND);
	if (node && node->type == MXML_ELEMENT && !strcmp(node->value.element.name, "ScanlineEvenOdd"))
	{
		if(node->child)
			SetScanlinesEvenOrOdd(node->child->value.integer);			
	}	
	
	node = mxmlFindElement(xml, xml, "ScanlineIntensity", NULL, NULL, MXML_DESCEND);
	if (node && node->type == MXML_ELEMENT && !strcmp(node->value.element.name, "ScanlineIntensity"))
	{
		if(node->child) 	  
			SetRawScanlineIntensity(node->child->value.integer);			
	}	
	
	node = mxmlFindElement(xml, xml, "EnablePAL", NULL, NULL, MXML_DESCEND);
	if (node && node->type == MXML_ELEMENT && !strcmp(node->value.element.name, "EnablePAL"))
	{
		if(node->child) 	  
			Options.EnablePAL = node->child->value.integer;						
	}	
	
	node = mxmlFindElement(xml, xml, "EnablePALBG", NULL, NULL, MXML_DESCEND);
	if (node && node->type == MXML_ELEMENT && !strcmp(node->value.element.name, "EnablePALBG"))
	{
		if(node->child) 	  
			Options.EnablePALBG = node->child->value.integer;						
	}	
	
	node = mxmlFindElement(xml, xml, "PalBackR", NULL, NULL, MXML_DESCEND);
	if (node && node->type == MXML_ELEMENT && !strcmp(node->value.element.name, "PalBackR"))
	{
		if(node->child) 	  
			Options.PalBackR = node->child->value.integer;						
	}	
	
	node = mxmlFindElement(xml, xml, "PalBackG", NULL, NULL, MXML_DESCEND);
	if (node && node->type == MXML_ELEMENT && !strcmp(node->value.element.name, "PalBackG"))
	{
		if(node->child) 	  
			Options.PalBackG = node->child->value.integer;						
	}	
	
	node = mxmlFindElement(xml, xml, "PalBackB", NULL, NULL, MXML_DESCEND);
	if (node && node->type == MXML_ELEMENT && !strcmp(node->value.element.name, "PalBackB"))
	{
		if(node->child) 	  
			Options.PalBackB = node->child->value.integer;						
	}	
	
	node = mxmlFindElement(xml, xml, "PALline23", NULL, NULL, MXML_DESCEND);
	if (node && node->type == MXML_ELEMENT && !strcmp(node->value.element.name, "PALline23"))
	{
		if(node->child) 	  
			Options.PALline23 = node->child->value.integer;						
		Set576iLine23Option(Options.PALline23);
	}	
	
	node = mxmlFindElement(xml, xml, "Force480p", NULL, NULL, MXML_DESCEND);
	if (node && node->type == MXML_ELEMENT && !strcmp(node->value.element.name, "Force480p"))
	{
		if(node->child) 	  
			Options.Force480p = node->child->value.integer;						
	}	
	
	if(xml)
		mxmlDelete(xml);
		
	CloseFS();
	*/
	return 1;
}

const char *xml_cb(mxml_node_t *node, int where)
{
	const char *element;
	
	element = mxmlGetElement(node);
	
	if(!strcmp(element, "options240p"))
	{
		if(where == MXML_WS_BEFORE_OPEN ||
			where == MXML_WS_AFTER_CLOSE ||
			where == MXML_WS_AFTER_OPEN)
			return ("\n");
	}
	else
	{
		if(where == MXML_WS_BEFORE_OPEN)
			return ("\t");
		if(where == MXML_WS_AFTER_CLOSE)
			return ("\n");
	}
	
	/*
	* Otherwise return NULL for no added whitespace...
	*/

	return (NULL);
}

u8 SaveOptions()
{
	/*
	FILE *file = NULL;	
	mxml_node_t *xml = NULL;			// <?xml ... ?>
	mxml_node_t *options240p = NULL;	// <options240p>
	mxml_node_t *node = NULL;			// each <node>

	if(!InitFS())
		return 0;	
		
	file = fopen(OPTIONS_FILE, "w");
	if(!file)
	{	
		CloseFS();
		return 0;
	}
			
	xml = mxmlNewXML("1.0");
	if(!xml)
	{
		fclose(file);
		CloseFS();
		return 0;
	}
	
	options240p = mxmlNewElement(xml, "options240p");
	if(!options240p)
	{
		mxmlDelete(xml);
		CloseFS();
		return 0;
	}	
	
	node = mxmlNewElement(options240p, "UseDeflickerFilter");
	mxmlNewInteger(node, Options.FlickerFilter);		
	
#ifdef WII_VERSION		
	node = mxmlNewElement(options240p, "UseTrapFilter");
	mxmlNewInteger(node, Options.TrapFilter);
	
	node = mxmlNewElement(options240p, "SFCClassicController");
	mxmlNewInteger(node, Options.SFCClassicController);
#endif

	node = mxmlNewElement(options240p, "Activate480p");
	mxmlNewInteger(node, Options.Activate480p);
	
	node = mxmlNewElement(options240p, "ScanlineEvenOdd");
	mxmlNewInteger(node, (int)ScanlinesEven());
	node = mxmlNewElement(options240p, "ScanlineIntensity");
	mxmlNewInteger(node, (int)GetRawScanlineValue());	
	
	node = mxmlNewElement(options240p, "EnablePAL");
	mxmlNewInteger(node, Options.EnablePAL);	
	node = mxmlNewElement(options240p, "EnablePALBG");
	mxmlNewInteger(node, Options.EnablePALBG);	
	node = mxmlNewElement(options240p, "PalBackR");
	mxmlNewInteger(node, Options.PalBackR);	
	node = mxmlNewElement(options240p, "PalBackG");
	mxmlNewInteger(node, Options.PalBackG);	
	node = mxmlNewElement(options240p, "PalBackB");
	mxmlNewInteger(node, Options.PalBackB);	
	node = mxmlNewElement(options240p, "PALline23");
	mxmlNewInteger(node, Options.PALline23);	
	node = mxmlNewElement(options240p, "Force480p");
	mxmlNewInteger(node, Options.Force480p);	
	
	// Removing this option from the save file, since it looks terrible
	//node = mxmlNewElement(options240p, "PALScale576");
	//mxmlNewInteger(node, Options.PALScale576);		
	
	mxmlSaveFile(xml, file, xml_cb);	
	fclose(file);
		
	CloseFS();
	mxmlDelete(xml);
	*/
		
	return 1;
}

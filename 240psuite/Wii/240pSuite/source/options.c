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
 * You should have received a copy of the GNU General Public License
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
#include "wav.h"


#define SD_OPTIONS_PATH "sd:/240pSuite"
#define OPTIONS_FILE "options.xml"
#define FS_SD	1

u8 FSActive = 0;

#ifdef WII_VERSION

#include <sdcard/wiisd_io.h>

#define USB_OPTIONS_PATH "usb:/240pSuite"
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

#define HEADER_BUFFER 4096

int copyFromBuffer(void *dest, uint8_t *buffer, size_t size, int *buffpos)
{
	if(*buffpos + size > HEADER_BUFFER)
		return 0;
	
	memcpy(dest, buffer+*buffpos, size);
	*buffpos += size;
	return 1;
}

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

u8 ParseWAVFile(FILE *file, uint32_t *size)
{
	fmt_hdr fmt;
	riff_hdr riff;
	data_hdr data;
	fmt_hdr_ext1 fmtExtra;
	fmt_hdr_ext2 fmtExtra2;
	int found = 0, buffpos = 0;
	uint8_t *hbuffer = NULL;
	
	hbuffer = (uint8_t*)malloc(sizeof(uint8_t)*HEADER_BUFFER);
	if(!hbuffer)
		return 0;
	
	if(fread(hbuffer, sizeof(uint8_t), HEADER_BUFFER, file) != sizeof(uint8_t)*HEADER_BUFFER)
	{
		free(hbuffer);
		return 0;
	}
	
	rewind(file);
	
	if(!copyFromBuffer(&riff, hbuffer, sizeof(riff_hdr), &buffpos))
	{
		free(hbuffer);
		return 0;
	}
	
	fmt.Subchunk1Size = EndianCorrect32bits(fmt.Subchunk1Size);
	if(strncmp((char*)riff.RIFF, "RIFF", 4) != 0)
	{
		free(hbuffer);
		return 0;
	}
	
	if(strncmp((char*)riff.WAVE, "WAVE", 4) != 0)
	{
		free(hbuffer);
		return 0;
	}
	
	// look for fmt chunk
	found = 0;
	do
	{
		sub_chunk	schunk;

		if(!copyFromBuffer(&schunk, hbuffer, sizeof(sub_chunk), &buffpos))
		{
			free(hbuffer);
			return 0;
		}
		
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
	
	if(!copyFromBuffer(&fmt, hbuffer, sizeof(fmt_hdr), &buffpos))
	{
		free(hbuffer);
		return 0;
	}
		
	fmt.AudioFormat = EndianCorrect16bits(fmt.AudioFormat);
	fmt.NumOfChan = EndianCorrect16bits(fmt.NumOfChan);
	fmt.blockAlign = EndianCorrect16bits(fmt.blockAlign);
	fmt.bitsPerSample = EndianCorrect16bits(fmt.bitsPerSample);
	
	fmt.Subchunk1Size = EndianCorrect32bits(fmt.Subchunk1Size);
	fmt.SamplesPerSec = EndianCorrect32bits(fmt.SamplesPerSec);
	fmt.bytesPerSec = EndianCorrect32bits(fmt.bytesPerSec);
	
	if(fmt.NumOfChan != 2 || fmt.AudioFormat != WAVE_FORMAT_PCM || 
		fmt.bitsPerSample != 16 ||fmt.SamplesPerSec != 48000)
	{
		free(hbuffer);
		return 0;
	}
	
	switch(fmt.Subchunk1Size)
	{
		case FMT_TYPE_1:
			break;
		case FMT_TYPE_2:
			if(!copyFromBuffer(&fmtExtra, hbuffer, sizeof(fmt_hdr_ext1), &buffpos))
			{
				free(hbuffer);
				return 0;
			}
			break;
		case FMT_TYPE_3:
			if(!copyFromBuffer(&fmtExtra2, hbuffer, sizeof(fmt_hdr_ext2), &buffpos))
			{
				free(hbuffer);
				return 0;
			}
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

		if(!copyFromBuffer(&schunk, hbuffer, sizeof(sub_chunk), &buffpos))
		{
			free(hbuffer);
			return 0;
		}
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
			free(hbuffer);
			return 0;
		}
	}while(!found);
	
	if(!copyFromBuffer(&data, hbuffer, sizeof(data_hdr), &buffpos))
	{
		free(hbuffer);
		return 0;
	}
	
	data.DataSize = EndianCorrect32bits(data.DataSize);
	if(data.DataSize == 0)
	{
		free(hbuffer);
		return 0;
	}
	
	// advance file to position
	if(fread(hbuffer, sizeof(uint8_t), buffpos, file) != sizeof(uint8_t)*buffpos)
	{
		free(hbuffer);
		return 0;
	}
	
	free(hbuffer);
	hbuffer= NULL;

	*size = data.DataSize;
	return 1;
}

u8 *LoadFileToBuffer(char *filename, ulong *size)
{
	FILE *file = NULL;
	uint32_t file_size = 0;
	u8 *file_buffer = NULL;
	
	if(!InitFS())
		return NULL;

	file = fopen(filename, "r");
	if(!file)
	{
		CloseFS();
		return NULL;
	}
	
	if(!ParseWAVFile(file, &file_size))
	{
		CloseFS();
		return NULL;
	}
	
	file_buffer = (u8*)malloc(sizeof(u8)*file_size);
	if(!file_buffer)
	{		
		fclose(file);
		CloseFS();
		return NULL;
	}
	
	if(fread(file_buffer, sizeof(u8), file_size, file) != file_size) 
	{		
		free(file_buffer);
		fclose(file);
		CloseFS();
		return NULL;
	}
	
	fclose(file);
	CloseFS();
	
	swapPCMEndianess((uint16_t*)file_buffer, file_size/2);
	
	*size = file_size;
	return file_buffer;
}

u8 LoadFileToMemoryAddress(char *filename, ulong *size, void *memory, ulong memsize)
{
	FILE *file = NULL;
	uint32_t  file_size = 0;
	
	if(!InitFS())
		return 0;

	file = fopen(filename, "r");
	if(!file)
	{
		CloseFS();
		return 0;
	}
	
	if(!ParseWAVFile(file, &file_size))
	{
		CloseFS();
		return 0;
	}
	
	if(file_size > memsize)
	{
		fclose(file);
		CloseFS();
		return 0;
	}
	
	if(fread(memory, sizeof(u8), file_size, file) != file_size) 
	{		
		fclose(file);
		CloseFS();
		return 0;
	}
	
	fclose(file);
	CloseFS();
	
	swapPCMEndianess((uint16_t*)memory, file_size/2);
	
	*size = file_size;
	return 1;
}

u8 LoadOptions()
{
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
		
	xml = mxmlLoadFile(NULL, file, MXML_NO_CALLBACK);
	fclose(file);
	
	if(!xml)
	{
		CloseFS();
		return 0;	
	}
	   	
	node = mxmlFindElement(xml, xml, "Activate480p", NULL, NULL, MXML_DESCEND);
	if (node && mxmlGetType(node) == MXML_ELEMENT)
	{		
		const char *value = mxmlGetText(node, NULL);
		if(value)
			Options.Activate480p = atoi(value);
	}
		
	node = mxmlFindElement(xml, xml, "UseTrapFilter", NULL, NULL, MXML_DESCEND);
	if (node && mxmlGetType(node) == MXML_ELEMENT)
	{		
		const char *value = mxmlGetText(node, NULL);
		if(value)
			Options.TrapFilter = atoi(value);
	}
	
#ifdef WII_VERSION	
	node = mxmlFindElement(xml, xml, "UseDeflickerFilter", NULL, NULL, MXML_DESCEND);
	if (node && mxmlGetType(node) == MXML_ELEMENT)
	{		
		const char *value = mxmlGetText(node, NULL);
		if(value)
			Options.FlickerFilter = atoi(value);
	}
	
	node = mxmlFindElement(xml, xml, "SFCClassicController", NULL, NULL, MXML_DESCEND);
	if (node && mxmlGetType(node) == MXML_ELEMENT)
	{		
		const char *value = mxmlGetText(node, NULL);
		if(value)
			Options.SFCClassicController = atoi(value);
	}
#endif
	
	node = mxmlFindElement(xml, xml, "ScanlineEvenOdd", NULL, NULL, MXML_DESCEND);
	if (node && mxmlGetType(node) == MXML_ELEMENT)
	{
		const char *value = mxmlGetText(node, NULL);
		if(value)
			SetScanlinesEvenOrOdd(atoi(value));
	}	
	
	node = mxmlFindElement(xml, xml, "ScanlineIntensity", NULL, NULL, MXML_DESCEND);
	if (node && mxmlGetType(node) == MXML_ELEMENT)
	{
		const char *value = mxmlGetText(node, NULL);
		if(value)
			SetRawScanlineIntensity(atoi(value));			
	}	
	
	node = mxmlFindElement(xml, xml, "EnablePAL", NULL, NULL, MXML_DESCEND);
	if (node && mxmlGetType(node) == MXML_ELEMENT)
	{
		const char *value = mxmlGetText(node, NULL);
		if(value)
			Options.EnablePAL = atoi(value);			
	}	
	
	node = mxmlFindElement(xml, xml, "EnablePALBG", NULL, NULL, MXML_DESCEND);
	if (node && mxmlGetType(node) == MXML_ELEMENT)
	{
		const char *value = mxmlGetText(node, NULL);
		if(value)
			Options.EnablePALBG = atoi(value);			
	}	
	
	node = mxmlFindElement(xml, xml, "PalBackR", NULL, NULL, MXML_DESCEND);
	if (node && mxmlGetType(node) == MXML_ELEMENT)
	{
		const char *value = mxmlGetText(node, NULL);
		if(value)
			Options.PalBackR = atoi(value);			
	}	
	
	node = mxmlFindElement(xml, xml, "PalBackG", NULL, NULL, MXML_DESCEND);
	if (node && mxmlGetType(node) == MXML_ELEMENT)
	{
		const char *value = mxmlGetText(node, NULL);
		if(value)
			Options.PalBackG = atoi(value);			
	}	
	
	node = mxmlFindElement(xml, xml, "PalBackB", NULL, NULL, MXML_DESCEND);
	if (node && mxmlGetType(node) == MXML_ELEMENT)
	{
		const char *value = mxmlGetText(node, NULL);
		if(value)
			Options.PalBackB = atoi(value);			
	}	
	
	node = mxmlFindElement(xml, xml, "PALline23", NULL, NULL, MXML_DESCEND);
	if (node && mxmlGetType(node) == MXML_ELEMENT)
	{
		const char *value = mxmlGetText(node, NULL);
		if(value)
			Options.PALline23 = atoi(value);			
		Set576iLine23Option(Options.PALline23);
	}	
	
	node = mxmlFindElement(xml, xml, "Force480p", NULL, NULL, MXML_DESCEND);
	if (node && mxmlGetType(node) == MXML_ELEMENT)
	{
		const char *value = mxmlGetText(node, NULL);
		if(value)
			Options.Force480p = atoi(value);			
	}	
	
	if(xml)
		mxmlDelete(xml);
		
	CloseFS();
	return 1;
}

const char *xml_cb(mxml_node_t *node, int where)
{
	const char *element;
	
	element = mxmlGetElement(node);
	
	if(!strcmp(element, "options240p") ||
		!strncmp(element, "?xml", 4))
	{
		if(where == MXML_WS_AFTER_CLOSE ||
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

// Helper function to add an integer attribute to the element
int addIntAttribute(mxml_node_t *parent, const char *name, int value)
{
	char valueStr[20]; 
	mxml_node_t *attr = mxmlNewElement(parent, name);
	
	if(!attr)
		return 0;
	
	snprintf(valueStr, sizeof(valueStr), "%d", value);
	if(mxmlNewText(attr, 0, valueStr))
		return 1;
	return 0;
}

u8 SaveOptions()
{
	FILE *file = NULL;
	mxml_node_t *xml = NULL;			 // <?xml ... ?>
	mxml_node_t *options240p = NULL;	  // <options240p>

	if (!InitFS())
		return 0;

	file = fopen(OPTIONS_FILE, "w");
	if (!file)
	{
		CloseFS();
		return 0;
	}

	xml = mxmlNewXML("1.0");
	if (!xml)
	{
		fclose(file);
		CloseFS();
		return 0;
	}

	options240p = mxmlNewElement(xml, "options240p");
	if (!options240p)
	{
		mxmlDelete(xml);
		fclose(file);
		CloseFS();
		return 0;
	}

	if (!addIntAttribute(options240p, "UseDeflickerFilter", Options.FlickerFilter))
	{
		mxmlDelete(xml);
		fclose(file);
		CloseFS();
		return 0;
	}

#ifdef WII_VERSION
	if (!addIntAttribute(options240p, "UseTrapFilter", Options.TrapFilter) ||
		!addIntAttribute(options240p, "SFCClassicController", Options.SFCClassicController))
	{
		mxmlDelete(xml);
		fclose(file);
		CloseFS();
		return 0;
	}
#endif

	if (!addIntAttribute(options240p, "Activate480p", Options.Activate480p) ||
		!addIntAttribute(options240p, "ScanlineEvenOdd", (int)ScanlinesEven()) ||
		!addIntAttribute(options240p, "ScanlineIntensity", (int)GetRawScanlineValue()) ||
		!addIntAttribute(options240p, "EnablePAL", Options.EnablePAL) ||
		!addIntAttribute(options240p, "EnablePALBG", Options.EnablePALBG) ||
		!addIntAttribute(options240p, "PalBackR", Options.PalBackR) ||
		!addIntAttribute(options240p, "PalBackG", Options.PalBackG) ||
		!addIntAttribute(options240p, "PalBackB", Options.PalBackB) ||
		!addIntAttribute(options240p, "PALline23", Options.PALline23) ||
		!addIntAttribute(options240p, "Force480p", Options.Force480p))
	{
		mxmlDelete(xml);
		fclose(file);
		CloseFS();
		return 0;
	}

	// Removing this option from the save file, since it looks terrible
	// addIntAttribute(options240p, "PALScale576", Options.PALScale576);

	if(mxmlSaveFile(xml, file, xml_cb))
	{
		fclose(file);
		CloseFS();
		mxmlDelete(xml);
		return 0;
	}
	fclose(file);

	CloseFS();
	mxmlDelete(xml);

	return 1;
}

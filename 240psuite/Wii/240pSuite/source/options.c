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

u8 *LoadFileToBuffer(char *filename, ulong *size)
{
	FILE *file = NULL;
	ulong file_size = 0;
	u8 *file_buffer = NULL;
	
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
	
	file_buffer = (u8*)malloc(sizeof(u8)*file_size);
	if(!file_buffer)
	{		
		fclose(file);
		CloseFS();
		return NULL;
	}
	
	if(fread(file_buffer, sizeof(u8), file_size, file) != file_size) 
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
	if (node && node->type == MXML_ELEMENT && !strcmp(node->value.element.name, "Activate480p"))
	{		
	    if(node->child)
	        Options.Activate480p = node->child->value.integer;		    
	}
		
	node = mxmlFindElement(xml, xml, "UseTrapFilter", NULL, NULL, MXML_DESCEND);
	if (node && node->type == MXML_ELEMENT && !strcmp(node->value.element.name, "UseTrapFilter"))
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
	
	// Removing this option from the save file, since it looks terrible
	/*
	node = mxmlFindElement(xml, xml, "PALScale576", NULL, NULL, MXML_DESCEND);
	if (node && node->type == MXML_ELEMENT && !strcmp(node->value.element.name, "PALScale576"))
	{
		if(node->child)       
            EnableStretchedPALModes(node->child->value.integer); 		
	}
	*/	
	
	if(xml)
		mxmlDelete(xml);
		
	CloseFS();
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
	FILE *file = NULL;	
	mxml_node_t *xml = NULL;			/* <?xml ... ?> */
    mxml_node_t *options240p = NULL;	/* <options240p> */
    mxml_node_t *node = NULL;			/* each <node> */    

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
        
	return 1;
}

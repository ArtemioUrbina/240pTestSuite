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

#define OPTIONS_PATH "sd:/240pSuite"
#define OPTIONS_FILE "options.xml"

u8 FSActive = 0;

#ifdef WII_VERSION

u8 LoadOptions()
{
	FILE *file;	
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
		
	node = mxmlFindElement(xml, xml, "ShowWiiRegion", NULL, NULL, MXML_DESCEND);
	if (node && node->type == MXML_ELEMENT && !strcmp(node->value.element.name, "ShowWiiRegion"))
	{		
		if(node->child)
		    Options.ShowWiiRegion = node->child->value.integer;				    
	}	
	
	node = mxmlFindElement(xml, xml, "SFCClassicController", NULL, NULL, MXML_DESCEND);
	if (node && node->type == MXML_ELEMENT && !strcmp(node->value.element.name, "SFCClassicController"))
	{		
        if(node->child)
            Options.SFCClassicController = node->child->value.integer;		    
	}
	
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
	
	if(xml)
		mxmlDelete(xml);
		
	CloseFS();
	return 1;
}

u8 SaveOptions()
{
	FILE *file;	
	mxml_node_t *xml;    		/* <?xml ... ?> */
    mxml_node_t *options240p;   /* <options240p> */
    mxml_node_t *node;   		/* each <node> */    

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

	node = mxmlNewElement(options240p, "Activate480p");
	mxmlNewInteger(node, Options.Activate480p);
	node = mxmlNewElement(options240p, "ShowWiiRegion");
	mxmlNewInteger(node, Options.ShowWiiRegion);
	//node = mxmlNewElement(options240p, "CorrectFor169");
	//mxmlNewInteger(node, 0, Options.CorrectFor169);	
	node = mxmlNewElement(options240p, "SFCClassicController");
	mxmlNewInteger(node, Options.SFCClassicController);
	
	node = mxmlNewElement(options240p, "ScanlineEvenOdd");
	mxmlNewInteger(node, (int)ScanlinesEven());
	node = mxmlNewElement(options240p, "ScanlineIntensity");
	mxmlNewInteger(node, (int)GetRawScanlineValue());	
		
    mxmlSaveFile(xml, file, MXML_NO_CALLBACK);	
		
	mxmlDelete(xml);
    fclose(file);	
		
	CloseFS();
        
	return 1;
}

u8 InitFS()
{
	int retry = 0;
	int fatMounted = 0;
			
	if(FSActive)
		return 1;
		
	while (!fatMounted && (retry < 12))
	{		
		fatMounted = fatInitDefault();
		usleep(250000);
		retry++;
	}
	
	if (fatMounted)
	{			
		DIR *dir = NULL;
				
		dir = opendir(OPTIONS_PATH);
		if (dir) 
		{			
			closedir(dir);
		}
		else
		{		
			mkdir(OPTIONS_PATH,S_IRWXU);
		}
		chdir(OPTIONS_PATH);
		FSActive = 1;
	}
	else
		FSActive = 0;
		
	return FSActive;
}

void CloseFS()
{	
	if(FSActive)
	{
		fatUnmount("sd");
		FSActive = 0;
	}
}

#else

u8 LoadOptions()
{
	return 0;
}

u8 SaveOptions()
{
	return 0;
}

#endif
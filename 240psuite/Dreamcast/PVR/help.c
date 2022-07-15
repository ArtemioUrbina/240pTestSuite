/* 
 * 240p Test Suite
 * Copyright (C)2011-2022 Artemio Urbina
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA	02111-1307	USA
 */

#include <kos.h>
#include <stdio.h>

#include "controller.h"
#include "image.h"
#include "font.h"
#include "vmodes.h"
#include "vmu.h"

#include "help.h"
#include "menu.h"
char *HelpData = NULL;
extern uint16 DrawMenu;

#define LINESPERPAGE	16

char *LoadHelpFile(char *filename, char ***pages, int *npages)
{
	int	i = 0, lines = 0, linecount = 0, currpage = 0;
	long 	size = 0;
	FILE 	*fp = NULL;
	char 	*buffer = NULL;

	fp = fopen(filename, "r");
	if(!fp)
	{
		dbglog(DBG_ERROR, "Could not load %s help file\n", filename);
		return NULL;
	}
	fseek(fp, 0L, SEEK_END);
	size = ftell(fp)+1;
	buffer = (char*)malloc(sizeof(char)*size);
	if(!buffer)
	{
		fclose(fp);
		dbglog(DBG_ERROR, "Could not load %s help file to RAM\n", filename);
		return NULL;
	}
	fseek(fp, 0L, SEEK_SET);
	memset(buffer, 0x0, sizeof(char)*size);
	fread(buffer, sizeof(char), size-1, fp);
	fclose(fp);

	for(i = 0; i < size; i++)
		if(buffer[i] == '\n')
			lines ++;

	*npages = (lines / LINESPERPAGE);
	if(lines % LINESPERPAGE && lines > LINESPERPAGE)
		(*npages) ++;
	if(*npages < 1)
		*npages = 1;

	*pages = (char**)malloc(sizeof(char*)*(*npages));
	if(!*pages)
	{
		dbglog(DBG_ERROR, "Could not malloc array of pages %s\n", filename);
		free(buffer);
		buffer = NULL;
		return NULL;
	}

	*pages[0] = buffer;
	currpage = 1;
	if(*npages > 1)
	{
		for(i = 0; i < size; i++)
		{
			if(buffer[i] == '\n')
			{
				linecount ++;
				if(linecount == LINESPERPAGE)
				{
					linecount = 0;
					buffer[i] = 0x0;
					if(currpage < *npages)
						(*pages)[currpage++] = buffer+i+1;
				}
			}
		}
	}

	return buffer;
}

/*
 * Detect pattern #I/rd/FILENAME#I
 * if found, clear it from help and
 * load image to array
 */ 
ImagePtr FindImageInPage(char *str)
{
	int			catching_filename = 0, pos = 0;
	char 		filename[PATH_LEN];
	ImagePtr	image = NULL;
	
	memset(filename, 0, sizeof(char)*PATH_LEN);
	while(*str) 
	{	
		if(*str == '#')
		{
			str++;
			
			if(*str == 'I')
			{
				catching_filename = !catching_filename;
				
				*str = ' ';				// Clear 'I'
				*(str-1) = ' ';			// Clear '#'
				str++;
			}
		}
		else
		{
			if(catching_filename)
			{
				filename[pos] = *str;
				*str = ' ';				// Clear filename from help
				pos++;
			}
			str++;
		}
	}
	if(!pos)
		return (ImagePtr)NULL;
	image = LoadKMG(filename, 0);
	if(!image)
		return (ImagePtr)NULL;
	return image;
}

void ParseHelpImages(char **pages, int npages, ImagePtr *images)
{
	int page = 0;
	
	for(page = 0; page < npages; page++)
		images[page] = FindImageInPage(pages[page]);
}

void HelpWindow(char *filename, ImagePtr screen)
{
	int 			done = 0, page = 0, joycnt = 0;
	int				npages = 0, image = 0, oldpage = -1;
	uint16			pressed = 0;		
	ImagePtr		back = NULL, *images = NULL;
	char			*buffer = NULL, **pages = NULL;
	controller		*st;
	char			vmuMsg[20];

	buffer = LoadHelpFile(filename, &pages, &npages);
	if(!buffer)
		return;

	back = LoadKMG("/rd/help.kmg.gz", 0);
	if(back)
		back->alpha = 0.75f;
	
	// Load images if needed
	if(npages)
	{
		images = (ImagePtr*)malloc(sizeof(ImagePtr)*npages);
		if(!images)
		{
			dbglog(DBG_ERROR, "Could not malloc image array for Help\n");
			return;
		}
		for(image = 0; image < npages; image++)
			images[image] = (ImagePtr)NULL;
		ParseHelpImages(pages, npages, images);
	}
		
	while(!done && !EndProgram) 
	{
		maple_device_t	*dev = NULL;
		
		StartScene();
		if(screen)
			DrawImage(screen);
		DrawImage(back);
		DrawStringS(34, 42, 1.0f, 1.0f, 1.0f, pages[page]);
		if(images[page])
			DrawImage(images[page]);

		DrawStringSCentered(200, 0.4f, 0.9f, 0.4f, "Press B to close");
		EndScene();
		
		VMURefresh("   Help", vmuMsg);

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_B)
				done = 1;
			if (pressed & CONT_DPAD_LEFT)
				page --;
			if (pressed & CONT_DPAD_RIGHT)
				page ++;
			if (pressed & CONT_RTRIGGER)
				page ++;
			if (pressed & CONT_LTRIGGER)
				page --;
				
			if(st->joyx != 0)
			{
				if(++joycnt > 10)
				{
					if(st->joyx > 0)
						page ++;
					if(st->joyx < 0)
						page --;
		
					joycnt = 0;
				}
			}
			else
				joycnt = 0;
		}
		dev = maple_enum_type(0, MAPLE_FUNC_LIGHTGUN);
		if(dev)
		{
			cont_state_t	*state;
			
            if((state = (cont_state_t *)maple_dev_status(dev)))
			{
				if(state->buttons & CONT_B ||
					state->buttons & CONT_START)
					done = 1;
			}
		}

		if(page > npages - 1)
			page = npages - 1;
		if(page < 0)
			page = 0;
		if(page != oldpage)
		{
			sprintf(vmuMsg, "Page: %d/%d", page + 1, npages);
			oldpage = page;
			refreshVMU = 1;
		}
	}

	if(buffer)
		free(buffer);
	if(pages)
		free(pages);
	for(image = 0; image < npages; image++)
	{
		if(images[image])
			FreeImage(&images[image]);
	}
	if(images)
		free(images);
	FreeImage(&back);
}



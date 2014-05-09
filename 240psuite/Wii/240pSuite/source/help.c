/* 
 * 240p Test Suite
 * Copyright (C)2011 Artemio Urbina
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

#include <stdio.h>

#include "image.h"
#include "font.h"
#include "video.h"

#include "help.h"
#include <wiiuse/wpad.h>

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
		fprintf(stderr, "Could not load %s help file\n", filename);
		return NULL;
	}
	fseek(fp, 0L, SEEK_END);
	size = ftell(fp)+1;
	buffer = (char*)malloc(sizeof(char)*size);
	if(!buffer)
	{
		fclose(fp);
		fprintf(stderr, "Could not load %s help file to RAM\n", filename);
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
		fprintf(stderr, "Could not malloc array of pages %s\n", filename);
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

u32 HelpWindow(char *filename, ImagePtr screen)
{
	int 			done = 0, npages = 0, page = 0;
	u32				pressed = 0;		
	ImagePtr		back;
	char			*buffer = NULL, **pages = NULL;

	/*
	buffer = LoadHelpFile(filename, &pages, &npages);
	if(!buffer)
		return 1;
	*/

	back = LoadImage(HELPIMG, 1);
	back->alpha = 0xAA;
			
	while(!done) 
	{
		WPAD_ScanPads();
		
		pressed = WPAD_ButtonsDown(0);
		
					
		if (pressed & WPAD_BUTTON_B)
			done =	1;								
		if (pressed & WPAD_BUTTON_LEFT)
			page --;
		if (pressed & WPAD_BUTTON_RIGHT)
			page ++;

		if(page > npages - 1)
			page = npages - 1;

		if(page < 0)
			page = 0;

		StartScene();
		if(screen)
			DrawImage(screen);
		DrawImage(back);
		//DrawStringS(34, 42, 0xff, 0xff, 0xff, pages[page]); 

		DrawStringS(100, 200, 0xee, 0xee, 0xee, "Press B to return"); 
		EndScene();

		VIDEO_Flush();
		VIDEO_WaitVSync();

	}
	if(buffer)
		free(buffer);
	if(pages)
		free(pages);
	FreeImage(&back);
	return pressed;
}



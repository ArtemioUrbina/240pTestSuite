/* 
 * 240p Test Suite
 * Copyright (C)2016 Artemio Urbina (N64)
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

#include "help.h"
#include "menu.h"

char *HelpData = NULL;
extern int DrawMenu;

#define LINESPERPAGE	16

char *LoadHelpFile(char *filename, char ***pages, int *npages)
{
	int		i = 0, lines = 0, linecount = 0, currpage = 0;
	long	size = 0;
	int 	fp = 0;
	char 	*buffer = NULL;

	if(!filename)
		return(NULL);
		
	fp = dfs_open(filename);
	if(fp == DFS_ESUCCESS)
	{
		//fprintf(stderr, "Could not load %s help file\n", filename);
		return NULL;
	}
	
	size = dfs_size(fp);
	if(size < 8)
        size = 8;
    else 
	{
		if(size % 8 != 0)
			size += (8 - (size % 8));
	}	
#ifdef USE_N64MEM
	buffer = (char*)n64_malloc(size);
#else
	buffer = (char*)malloc(size);
#endif
	if(!buffer)
	{
		dfs_close(fp);
		//fprintf(stderr, "Could not load %s help file to RAM\n", filename);
		return NULL;
	}
#ifdef USE_N64MEM	
	n64_memset(buffer, 0x20, size);
#else
	memset(buffer, 0x20, size);
#endif
	if(dfs_read(buffer, 1, size, fp) < 0)
	{
		dfs_close(fp);
#ifdef USE_N64Mem
		n64_free(buffer);
#else
		free(buffer);
#endif
		buffer = NULL;
		return NULL;
	}

 	dfs_close(fp);

	for(i = 0; i < size; i++)
		if(buffer[i] == '\n')
			lines ++;

	*npages = (lines / LINESPERPAGE);
	if(lines % LINESPERPAGE && lines > LINESPERPAGE)
		(*npages) ++;
	if(*npages < 1)
		*npages = 1;

#ifdef USE_N64MEM
	*pages = (char**)n64_malloc(sizeof(char*)*(*npages));
#else
	*pages = (char**)malloc(sizeof(char*)*(*npages));
#endif
	if(!*pages)
	{
		//fprintf(stderr, "Could not malloc array of pages %s\n", filename);
#ifdef USE_N64Mem
		n64_free(buffer);
#else
		free(buffer);
#endif
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

void  HelpWindow(char *filename, int usebuffer)
{
	int 			done = 0, npages = 0, page = 0;	
	sprite_t 		*back = NULL;
	char			*buffer = NULL, **pages = NULL;
	struct controller_data keys;

	if(!usebuffer)
	{
		CopyScreen();
		WaitVsync();
	}
	
	buffer = LoadHelpFile(filename, &pages, &npages);
	if(!buffer)
		return;

	back = LoadImage("/help.bin");
	if(!back)
		return;
		
	while(!done) 
	{
		GetDisplay();

		drawScreenBufferDMA(0, 0);
		
		rdp_texture_start();
		rdp_DrawImage(0, 37, back);
		rdp_end();  
		
		DrawStringS(34, 42, 0xff, 0xff, 0xff, pages[page]); 

		DrawStringS(110, 200, 0xBB, 0xBB, 0xBB, "Press B to return"); 
		WaitVsync();

		controller_scan();
		keys = Controller_ButtonsDown();

		if(keys.c[0].B || keys.c[0].start)
			done = 1;
		if(keys.c[0].left)
			page--;
		if(keys.c[0].right)
			page++;
		if(keys.c[0].L)
			page--;
		if(keys.c[0].R)
			page++;

		if(page > npages - 1)
			page = npages - 1;
		if(page < 0)
			page = 0;
	}

	if(buffer)
#ifdef USE_N64Mem
		n64_free(buffer);
#else
		free(buffer);
#endif
	if(pages)
#ifdef USE_N64Mem
		n64_free(pages);
#else
		free(pages);
#endif
	FreeImage(&back);
}



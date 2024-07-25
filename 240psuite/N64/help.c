/* 
 * 240p Test Suite for Nintendo 64
 * Copyright (C)2024 Artemio Urbina
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

#include "help.h"
#include "menu.h"

char *helpData = NULL;

#define LINESPERPAGE	24

char *loadHelpFile(char *filename, char ***pages, int *npages) {
	int		i = 0, lines = 0, linecount = 0, currpage = 0;
	long	size = 0;
	int 	fp = 0;
	char 	*buffer = NULL;

	if(!filename)
		return(NULL);
		
	fp = dfs_open(filename);
	if(fp == DFS_ESUCCESS) {
		return NULL;
	}
	
	size = dfs_size(fp);
	if(size < 8)
        size = 8;
    else {
		if(size % 8 != 0)
			size += (8 - (size % 8));
	}	

	buffer = (char*)malloc(size);
	if(!buffer)	{
		dfs_close(fp);
		return NULL;
	}

	memset(buffer, 0x20, size);

	if(dfs_read(buffer, 1, size, fp) < 0) {
		dfs_close(fp);
		free(buffer);
		buffer = NULL;
		return NULL;
	}

 	dfs_close(fp);

	for(i = 0; i < size; i++) {
		if(buffer[i] == '\n')
			lines ++;
	}

	*npages = (lines / LINESPERPAGE);
	if(lines % LINESPERPAGE && lines > LINESPERPAGE)
		(*npages) ++;
	if(*npages < 1)
		*npages = 1;

	*pages = (char**)malloc(sizeof(char*)*(*npages));
	if(!*pages)	{
		free(buffer);
		buffer = NULL;
		return NULL;
	}

	*pages[0] = buffer;
	currpage = 1;
	if(*npages > 1)	{
		for(i = 0; i < size; i++) {
			if(buffer[i] == '\n') {
				linecount ++;
				if(linecount == LINESPERPAGE) {
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

void helpWindow(char *filename) {
	int 				done = 0, npages = 0, page = 0;	
	int					opage = 0, redraw = current_buffers;
	image	 			*back = NULL;
	char				*buffer = NULL, **pages = NULL;
	joypad_buttons_t	keys;

	if(!filename)
		return;
	
	buffer = loadHelpFile(filename, &pages, &npages);
	if(!buffer)
		return;

	back = loadImage("rom:/help.sprite");
	if(!back) {
		free(buffer);
		free(pages);
		return;
	}

#ifdef DEBUG_BENCHMARK
	resetIdle();
#endif

	while(!done) {
		getDisplay();

		if(redraw) {
			rdpqStart();
			rdpqDrawImage(back);
			rdpqEnd();
			
			drawStringS(32, 16, 0xff, 0xff, 0xff, pages[page]);
			drawStringS(95, 216, 0xBB, 0xBB, 0xBB, "Press B to return");
			redraw--;
		}
		waitVsync();

		joypad_poll();
		keys = controllerButtonsDown();

		if(keys.b || keys.start)
			done = 1;
		if(keys.d_left)
			page--;
		if(keys.d_right)
			page++;
		if(keys.l)
			page--;
		if(keys.r)
			page++;

		if(page > npages - 1)
			page = npages - 1;
		if(page < 0)
			page = 0;
	
		if(page != opage) {
#ifdef DEBUG_BENCHMARK
			resetIdle();
#endif
			redraw = current_buffers;
			opage = page;
		}
	}

	if(buffer) {
		free(buffer);
		buffer = NULL;
	}

	if(pages) {
		free(pages);
		pages = NULL;
	}

	freeImage(&back);
}



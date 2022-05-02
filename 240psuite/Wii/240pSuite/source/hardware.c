/* 
 * r
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

#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "font.h"
#include "video.h"
#include "options.h"
#include "controller.h"

#include "hardware.h"

#include "help.h"
#include "menu.h"

void ControllerTest()
{
	int 		done = 0;
	u32			pressed;		
	ImagePtr	back;	
	
	back = LoadImage(BACKIMG, 0);
	if(!back)
		return;
			
	while(!done && !EndProgram) 
	{				
		StartScene();
		        
		DrawImage(back);
		
        EndScene();
		
		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
				
		if (pressed & PAD_BUTTON_B)
			done =	1;								
		
	}
	FreeImage(&back);
	return;
}

/*
CRC 32 based on work by Christopher Baker <https://christopherbaker.net>
*/

uint32_t _state = ~0L;

static const uint32_t crc32_table[] = {
    0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
    0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
    0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
    0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
};


void CRC32_reset()
{
    _state = ~0L;
}


void CRC32_update(uint8_t data)
{
    uint8_t tbl_idx = 0;

    tbl_idx = _state ^ (data >> (0 * 4));
    _state = (*(uint32_t*)(crc32_table + (tbl_idx & 0x0f)) ^ (_state >> 4));
    tbl_idx = _state ^ (data >> (1 * 4));
    _state = (*(uint32_t*)(crc32_table + (tbl_idx & 0x0f)) ^ (_state >> 4));
}


uint32_t CRC32_finalize()
{
    return ~_state;
}

/**********************************/

uint32_t CalculateCRC(uint32_t startAddress, uint32_t size)
{
	uint8_t *start = NULL;
	uint32_t address = 0, checksum = 0;
	CRC32_reset();

	start = (void*)startAddress;
	for (address = 0; address < size; address ++)
	{
		uint8_t data;
		
		data = start[address];

		CRC32_update(data);
	}

	checksum = CRC32_finalize();
	return checksum;
}

#define VISIBLE_HORZ	16
#define VISIBLE_VERT	26

#ifdef WII_VERSION
#define	MAX_LOCATIONS	8
#else
#define	MAX_LOCATIONS	6
#endif

void MemoryViewer()
{
	int 		done = 0, ascii = 0, locpos = 0, docrc = 0;
	u32			pressed;		
	uint32_t	address = 0, crc = 0;
#ifdef WII_VERSION
	uint32_t	locations[MAX_LOCATIONS] = { 0x80000000, 0xC0000000, 0x90000000, 0xD0000000, 0xc8000000, 0xCC000000, 0xe0000000, 0xfff00000 };
#else
	uint32_t	locations[MAX_LOCATIONS] = { 0x80000000, 0xC0000000, 0xc8000000, 0xCC000000, 0xe0000000, 0xfff00000 };
#endif
	
	address = locations[0];
	while(!done && !EndProgram) 
	{
		int 	i = 0, j = 0;
		uint8_t *mem = NULL;
		char 	buffer[10];
		
		StartScene();

		mem = (uint8_t*)address;
		
		if(docrc)
			crc = CalculateCRC(address, VISIBLE_HORZ*VISIBLE_VERT);
	
		sprintf(buffer, "%08X", address);
		DrawStringS(VISIBLE_HORZ*3*fw, 0, 0x00, 0xff, 0x00, buffer);
		
		sprintf(buffer, "%08X", address+VISIBLE_HORZ*VISIBLE_VERT);
		DrawStringS(VISIBLE_HORZ*3*fw, (VISIBLE_VERT-1)*fh, 0x00, 0xff, 0x00, buffer);

		if(docrc)
		{
			sprintf(buffer, "%08X", crc);
			DrawStringS(VISIBLE_HORZ*3*fw, (VISIBLE_VERT/2)*fh, 0xff, 0xff, 0x00, buffer);		
		}
		
		for(i = 0; i < VISIBLE_VERT; i++)
		{
			for(j = 0; j < VISIBLE_HORZ; j++)
			{
				if(!ascii)
					sprintf(buffer, "%02X", mem[i*VISIBLE_HORZ+j]);
				else
				{
					uint16_t c;
					
					memset(buffer, 0, sizeof(char)*10);
					
					buffer[0] = 20;				// Space
					c = mem[i*VISIBLE_HORZ+j];
					if(c >= 32 && c <= 127)		// ASCII range
						buffer[0] = c;
				}
				DrawStringS(3*j*fw, i*fh, 0xff, 0xff, 0xff, buffer);
			}
		}
        EndScene();
		
		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
		
		if (pressed & PAD_BUTTON_LEFT)	
		{
			if(address > locations[0])
				address -= VISIBLE_HORZ*VISIBLE_VERT;
			else
				address = locations[0];
		}
		
		if (pressed & PAD_BUTTON_RIGHT)
		{
			address += VISIBLE_HORZ*VISIBLE_VERT;
			
			if(address >= 0xFFFFFFFF)
				address = 0xFFFFFFFF-VISIBLE_HORZ*VISIBLE_VERT;
		}
		
		if (pressed & PAD_BUTTON_UP)	
		{
			if(address > locations[0]+0x10000)
				address -= 0x10000;
			else
				address = locations[0];
		}
		
		if (pressed & PAD_BUTTON_DOWN)
		{
			address += 0x10000;
			
			if(address >= 0xFFFFFFFF)
				address = 0xFFFFFFFF-VISIBLE_HORZ*VISIBLE_VERT;
		}
		
		if (pressed & PAD_BUTTON_B)
			done =	1;
			
		if (pressed & PAD_BUTTON_A)
		{
			locpos ++;
			if(locpos == MAX_LOCATIONS)
				locpos = 0;
			address = locations[locpos];
		}
			
		if (pressed & PAD_BUTTON_Y)
			ascii =	!ascii;
			
		if (pressed & PAD_BUTTON_X)
			docrc =	!docrc;
	
		if ( pressed & PAD_BUTTON_START ) 		
		{
			DrawMenu = 1;					
			HelpData = MEMORYHELP;
		}		
	}
	return;
}

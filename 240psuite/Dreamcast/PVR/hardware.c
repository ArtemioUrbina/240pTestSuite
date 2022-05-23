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

#include <kos.h>
#include <stdlib.h>
#include <sys/cdefs.h>

#include "image.h"
#include "font.h"
#include "vmodes.h"
#include "vmu.h"
#include "vmufs.h"

#include "controller.h"
#include "help.h"
#include "menu.h"

#include "hardware.h"

/*
u16 CheckController(u16 joypad, u16 oldButtons, u16 type, u16 x, u16 y, u8 ctrlID)
{
	u16	pos = 0;
	u32	pressed;
	char id[4];

	if(type != JOY_TYPE_PAD3 && type != JOY_TYPE_PAD6)
		return 0;
		
	buttons = JOY_readJoypad(joypad);
	
	VDP_Start();
	
	intToHex(ctrlID+1, id, 1);
	VDP_drawTextBG(APLAN, "-C -", TILE_ATTR(PAL1, 0, 0, 0), x+3, y-1);
	VDP_drawTextBG(APLAN, id, TILE_ATTR(PAL1, 0, 0, 0), x+5, y-1);
	
	VDP_drawTextBG(APLAN, "U", TILE_ATTR(buttons & BUTTON_UP ? PAL3 : PAL0, 0, 0, 0), x+1, y);
	VDP_drawTextBG(APLAN, "L", TILE_ATTR(buttons & BUTTON_LEFT ? PAL3 : PAL0, 0, 0, 0), x, y+1);
	VDP_drawTextBG(APLAN, "R", TILE_ATTR(buttons & BUTTON_RIGHT ? PAL3 : PAL0, 0, 0, 0), x+2, y+1);
	VDP_drawTextBG(APLAN, "D", TILE_ATTR(buttons & BUTTON_DOWN ? PAL3 : PAL0, 0, 0, 0), x+1, y+2);
	
	VDP_drawTextBG(APLAN, "S", TILE_ATTR(buttons & BUTTON_START ? PAL3 : PAL0, 0, 0, 0), x+4, y+1);
		
	if(type == JOY_TYPE_PAD6)
	{
		VDP_drawTextBG(APLAN, "X", TILE_ATTR(buttons & BUTTON_X ? PAL3 : PAL0, 0, 0, 0), x+6, y);
		VDP_drawTextBG(APLAN, "Y", TILE_ATTR(buttons & BUTTON_Y ? PAL3 : PAL0, 0, 0, 0), x+7, y);
		VDP_drawTextBG(APLAN, "Z", TILE_ATTR(buttons & BUTTON_Z ? PAL3 : PAL0, 0, 0, 0), x+8, y);
		VDP_drawTextBG(APLAN, "M", TILE_ATTR(buttons & BUTTON_MODE ? PAL3 : PAL0, 0, 0, 0), x+9, y-1);
		
		pos = y+2;
	}
	else
		pos = y+1;
	

	VDP_drawTextBG(APLAN, "A", TILE_ATTR(buttons & BUTTON_A ? PAL3 : PAL0, 0, 0, 0), x+6, pos);
	VDP_drawTextBG(APLAN, "B", TILE_ATTR(buttons & BUTTON_B ? PAL3 : PAL0, 0, 0, 0), x+7, pos);
	VDP_drawTextBG(APLAN, "C", TILE_ATTR(buttons & BUTTON_C ? PAL3 : PAL0, 0, 0, 0), x+8, pos);
	
	VDP_End();
	
	return buttons;
}
*/

/*
void ControllerTest()
{
	int 		done = 0;
	u32			pressed;
	s8			value;
	ImagePtr	back;
	char 		buffer[10];
	
	back = LoadImage(BACKIMG, 0);
	if(!back)
		return;
			
	while(!done && !EndProgram) 
	{				
		StartScene();
		        
		DrawImage(back);
		
		//Analog:
		value = PAD_StickX(0);
		sprintf(buffer, "%d", value);
		DrawStringS(100, 100, 0x00, 0xff, 0x00, buffer);
		value  = PAD_StickY(0);
		sprintf(buffer, "%d", value);
		DrawStringS(140, 100, 0x00, 0xff, 0x00, buffer);

		//C-stick:
		value = PAD_SubStickX(0);
		sprintf(buffer, "%d", value);
		DrawStringS(200, 100, 0x00, 0xff, 0x00, buffer);
		value = PAD_SubStickY(0);
		sprintf(buffer, "%d", value);
		DrawStringS(240, 100, 0x00, 0xff, 0x00, buffer);
		
        EndScene();
		
		ControllerScan();
		
		pressed = Controller_ButtonsDown(0);
				
		if (pressed & PAD_BUTTON_B)
			done =	1;								
		
	}
	FreeImage(&back);
	return;
}
*/

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
#define MAX_LOCATIONS	6

void MemoryViewer()
{
	int 		done = 0, ascii = 0, locpos = 0, docrc = 0, factor = 1;
	int			joycntx = 0, joycnty = 0;
	uint16		pressed;		
	uint32		address = 0, crc = 0;
	uint32		locations[MAX_LOCATIONS] = { 0x00000000, 0x04000000, 0x0C000000, 0x10000000, 0x14000000, 0x18000000 };
	controller	*st;
		
	address = locations[0];
	updateVMU("MemView", "", 1);
	while(!done && !EndProgram) 
	{
		int 	i = 0, j = 0;
		uint8_t *mem = NULL;
		char 	buffer[10];
		
		if(vmode >= HIGH_RES)
			factor = 2;
		else
			factor = 1;
			
		StartScene();

		mem = (uint8_t*)address;
		
		if(docrc)
			crc = CalculateCRC(address, VISIBLE_HORZ*factor*VISIBLE_VERT*factor);
	
		sprintf(buffer, "%08" PRIx32, address);
		DrawStringS(VISIBLE_HORZ*factor*3*fw, 0, 0.0f, 1.0f, 0.0f, buffer);
		
		sprintf(buffer, "%08" PRIx32, address+VISIBLE_HORZ*factor*VISIBLE_VERT*factor);
		DrawStringS(VISIBLE_HORZ*factor*3*fw, (VISIBLE_VERT*factor-1)*fh, 0.0f, 1.0f, 0.0f, buffer);

		if(docrc)
		{
			sprintf(buffer, "%08" PRIx32, crc);
			DrawStringS(VISIBLE_HORZ*factor*3*fw, (VISIBLE_VERT*factor/2)*fh, 1.0f, 1.0f, 0.0f, buffer);		
		}
		
		for(i = 0; i < VISIBLE_VERT*factor; i++)
		{
			for(j = 0; j < VISIBLE_HORZ*factor; j++)
			{	
				if(!ascii)
					sprintf(buffer, "%02X", mem[i*VISIBLE_HORZ*factor+j]);
				else
				{
					uint8_t c;
					
					memset(buffer, 0, sizeof(char)*10);

					c = mem[i*VISIBLE_HORZ*factor+j];
					if(c >= 32 && c <= 126)	
						buffer[0] = (char)c;			// ASCII range
					else
						buffer[0] = (char)32;			// Space
				}
				DrawStringS(3*j*fw, i*fh, 1.0f, 1.0f, 1.0f, buffer);
			}
		}
        EndScene();
		
		st = ReadController(0, &pressed);
		
		JoystickDirectios(st, &pressed, &joycntx, &joycnty);
		if (pressed & CONT_DPAD_LEFT)	
		{
			if(address > locations[0])
				address -= VISIBLE_HORZ*factor*VISIBLE_VERT*factor;
			else
				address = locations[0];
		}
		
		if (pressed & CONT_DPAD_RIGHT)
		{
			address += VISIBLE_HORZ*factor*VISIBLE_VERT*factor;

			if(address >= 0xFFFFFFFF) // 0x817fffff
				address = 0xFFFFFFFF-VISIBLE_HORZ*factor*VISIBLE_VERT*factor;
		}
		
		if (pressed & CONT_DPAD_UP)	
		{
			if(address > locations[0]+0x10000)
				address -= 0x10000;
			else
				address = locations[0];
		}
		
		if (pressed & CONT_DPAD_DOWN)
		{
			address += 0x10000;
			
			if(address >= 0xFFFFFFFF) // 0x817fffff
				address = 0xFFFFFFFF-VISIBLE_HORZ*factor*VISIBLE_VERT*factor;
		}
		
		if (pressed & CONT_B)
			done =	1;
			
		if (pressed & CONT_A)
		{
			locpos ++;
			if(locpos == MAX_LOCATIONS)
				locpos = 0;
			address = locations[locpos];
		}
			
		if (pressed & CONT_Y)
			ascii =	!ascii;
			
		if (pressed & CONT_X)
			docrc =	!docrc;

		if (pressed & CONT_START)
			ShowMenu(GENERALHELP)	;
	}
	return;
}

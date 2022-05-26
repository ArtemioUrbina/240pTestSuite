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

// 0-7 empty?

#define 	CONT_FIXED_CAPABILITY_RTRIG   		(1<<8)
#define 	CONT_FIXED_CAPABILITY_LTRIG   		(1<<9)
#define 	CONT_FIXED_CAPABILITY_ANALOG_X   	(1<<10)
#define 	CONT_FIXED_CAPABILITY_ANALOG_Y   	(1<<11)

#define 	CONT_FIXED_CAPABILITY_ANALOG2_X   	(1<<12)
#define 	CONT_FIXED_CAPABILITY_ANALOG2_Y   	(1<<13)

// 14 & 15?

#define 	CONT_FIXED_CAPABILITY_Z   			(1<<16)
#define 	CONT_FIXED_CAPABILITY_Y   			(1<<17)
#define 	CONT_FIXED_CAPABILITY_X   			(1<<18)
#define 	CONT_FIXED_CAPABILITY_D   			(1<<19)

#define 	CONT_FIXED_CAPABILITY_DPAD2_UP   	(1<<20)
#define 	CONT_FIXED_CAPABILITY_DPAD2_DOWN   	(1<<21)
#define 	CONT_FIXED_CAPABILITY_DPAD2_LEFT   	(1<<22)
#define 	CONT_FIXED_CAPABILITY_DPAD2_RIGHT   (1<<23)


#define 	CONT_FIXED_CAPABILITY_C   			(1<<24)
#define 	CONT_FIXED_CAPABILITY_B   			(1<<25)
#define 	CONT_FIXED_CAPABILITY_A   			(1<<26)
#define 	CONT_FIXED_CAPABILITY_START   		(1<<27)

#define 	CONT_FIXED_CAPABILITY_DPAD_UP   	(1<<28)
#define 	CONT_FIXED_CAPABILITY_DPAD_DOWN   	(1<<29)
#define 	CONT_FIXED_CAPABILITY_DPAD_LEFT   	(1<<30)
#define 	CONT_FIXED_CAPABILITY_DPAD_RIGHT   	(1<<31)

void DiplayController(int num, float x, float y)
{
	int				isPressed = 0;
	int				hasAnalogX = 0, hasAnalogY = 0, hasAnalog2X = 0, hasAnalog2Y = 0;
	cont_state_t	*st;
	maple_device_t	*dev = NULL;
	char			msg[256];
	float			orig_x = x;
	
	dev = maple_enum_type(num, MAPLE_FUNC_CONTROLLER);
	if(!dev)
		return;
		
	DrawStringS(x, y, 0.0f, 1.0f, 0.0f, dev->info.product_name);
	y += fh;
	
	/*
	sprintf(msg, "0x%08lx: %s",
			dev->info.functions, maple_pcaps(dev->info.functions))
	DrawStringS(x, y, 1.0f, 1.0f, 1.0f, msg);
	y += fh;
	*/
	
	sprintf(msg, "Power[%d-%d]mW", 
			dev->info.standby_power, dev->info.max_power);
	DrawStringS(x, y, 0.0f, 0.7f, 0.6f, msg);
	y += fh;
	
	/*
	sprintf(msg, "\"%s\" AC: 0x%0x", 
			dev->info.product_license, dev->info.area_code);
	DrawStringS(x, y, 1.0f, 1.0f, 1.0f, msg);
	y += fh;
	*/
	
	// Tells us the device type in 
	// isFishingRod/isMaracas/isStockController
	DetectContollerType(dev);
	
	st = (cont_state_t*)maple_dev_status(dev);
	if(!st)
		return;
		
	if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_ANALOG_X)
		hasAnalogX = 1;
	if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_ANALOG_Y)
		hasAnalogY = 1;
	if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_ANALOG2_X)
		hasAnalog2X = 1;
	if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_ANALOG2_Y)
		hasAnalog2Y = 1;
	
	x += 2*fw;

	if(isMaracas == 0)
	{
		int hadtriggers = 0;
		// Draw Triggers
		if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_LTRIG)
		{
			sprintf(msg, "%03d", st->ltrig);
			DrawStringS(x+1*fw, y, 1.0f, 1.0f, 1.0f, msg);
			hadtriggers = 1;
		}
		if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_RTRIG)
		{
			sprintf(msg, "%03d", st->rtrig);
			DrawStringS(x+7*fw, y, 1.0f, 1.0f, 1.0f, msg);
			hadtriggers = 1;
		}
		if(hadtriggers)
			y += fh;

		// Regular DC controller 	0xfe060f00
		// Fishing Rod 				0xfe063f00
		if(!(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_C))
		{
			// Draw Up and Y
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD_UP)
			{
				isPressed = st->buttons & CONT_DPAD_UP;
				DrawStringS(x+ 2*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "U");
			}
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_Y)
			{
				isPressed = st->buttons & CONT_Y;
				DrawStringS(x+ 8*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "Y");
			}
			y += fh;
		
			// Draw Left, Right, Start, X and B
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD_LEFT)
			{
				isPressed = st->buttons & CONT_DPAD_LEFT;
				DrawStringS(x+ 1*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "L");
			}
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD_RIGHT)
			{
				isPressed = st->buttons & CONT_DPAD_RIGHT;
				DrawStringS(x+ 3*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "R");
			}
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_START)
			{
				isPressed = st->buttons & CONT_START;
				DrawStringS(x+ 5*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "S");
			}
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_X)
			{
				isPressed = st->buttons & CONT_X;
				DrawStringS(x+ 7*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "X");
			}
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_B)
			{
				isPressed = st->buttons & CONT_B;
				DrawStringS(x+ 9*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "B");
			}
			y += fh;
		
			// Draw Down and A
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD_DOWN)
			{
				isPressed = st->buttons & CONT_DPAD_DOWN;
				DrawStringS(x+ 2*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "D");
			}
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_A)
			{
				isPressed = st->buttons & CONT_A;
				DrawStringS(x+ 8*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "A");
			}
		}
		else // Arcade Stick?
		{
			// Draw Up and X, Y, Z
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD_UP)
			{
				isPressed = st->buttons & CONT_DPAD_UP;
				DrawStringS(x+ 2*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "U");
			}
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_X)
			{
				isPressed = st->buttons & CONT_X;
				DrawStringS(x+ 7*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "X");
			}
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_Y)
			{
				isPressed = st->buttons & CONT_Y;
				DrawStringS(x+ 9*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "Y");
			}
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_Z)
			{
				isPressed = st->buttons & CONT_Z;
				DrawStringS(x+11*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "Z");
			}
			y += fh;
		
			// Draw Left, Right, Start
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD_LEFT)
			{
				isPressed = st->buttons & CONT_DPAD_LEFT;
				DrawStringS(x+ 1*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "L");
			}
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD_RIGHT)
			{
				isPressed = st->buttons & CONT_DPAD_RIGHT;
				DrawStringS(x+ 3*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "R");
			}
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_START)
			{
				isPressed = st->buttons & CONT_START;
				DrawStringS(x+ 5*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "S");
			}
			y += fh;
		
			// Draw Down and A. B, C
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD_DOWN)
			{
				isPressed = st->buttons & CONT_DPAD_DOWN;
				DrawStringS(x+ 2*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "D");
			}
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_A)
			{
				isPressed = st->buttons & CONT_A;
				DrawStringS(x+ 7*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "A");
			}
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_B)
			{
				isPressed = st->buttons & CONT_B;
				DrawStringS(x+ 9*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "B");
			}
			if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_C)
			{
				isPressed = st->buttons & CONT_C;
				DrawStringS(x+11*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "C");
			}
		}
	}
	else
	{
		// Maracas 0x0f093c00

		// Left Maraca
		if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_C)
		{
			isPressed = st->buttons & CONT_C;
			DrawStringS(x- 1*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "C");
		}
		if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_B)
		{
			isPressed = st->buttons & CONT_B;
			DrawStringS(x+ 1*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "B");
		}
		if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_Z)
		{
			isPressed = st->buttons & CONT_Z;
			DrawStringS(x+ 3*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "Z");
		}
		// Right Maraca
		
		if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_START)
		{
			isPressed = st->buttons & CONT_START;
			DrawStringS(x+ 7*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "S");
		}
		if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_A)
		{
			isPressed = st->buttons & CONT_A;
			DrawStringS(x+ 9*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "A");
		}
		if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_D)
		{
			isPressed = st->buttons & CONT_D;
			DrawStringS(x+11*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "D");
		}
	}
	y += fh;
	
	x = orig_x;
	// Center the Analog if only one is present
	if(!hasAnalog2X && !hasAnalog2Y)
		x += 4*fw;
	
	// Draw Analog Joystick
	if(hasAnalogY)
	{
		// Analog Up
		sprintf(msg, "%03d", st->joyy < 0 ? -1*st->joyy : 0);
		DrawStringS(x+2*fw, y, 1.0f, 1.0f, 1.0f, msg);
		y += fh;
	}
	
	if(hasAnalogX)
	{
		// Analog Right & Left
		sprintf(msg, "%03d %03d", st->joyx < 0 ? -1*st->joyx : 0, st->joyx > 0 ? st->joyx : 0);
		DrawStringS(x, y, 1.0f, 1.0f, 1.0f, msg);
		y += fh;
	}
	
	// Analog Down
	if(hasAnalogY)
	{
		sprintf(msg, "%03d", st->joyy > 0 ? st->joyy : 0);
		DrawStringS(x+2*fw, y, 1.0f, 1.0f, 1.0f, msg);
	}
	
	// Rewind 2 lines towards the top
	if(hasAnalog2X || hasAnalog2Y)
	{
		y -= 2*fh;
		x += 8*fw;
	}
	
	// Second Analog controller
	if(hasAnalog2Y)
	{
		// Analog Up
		sprintf(msg, "%03d", st->joy2y < 0 ? -1*st->joy2y : 0);
		DrawStringS(x+2*fw, y, 1.0f, 1.0f, 1.0f, msg);
		y += fh;
	}
	
	// Analog Right & Left
	if(hasAnalog2X)
	{
		sprintf(msg, "%03d %03d", st->joy2x < 0 ? -1*st->joy2x : 0, st->joy2x > 0 ? st->joy2x : 0);
		DrawStringS(x, y, 1.0f, 1.0f, 1.0f, msg);
		y += fh;
	}
	
	// Analog Down
	if(hasAnalog2Y)
	{
		sprintf(msg, "%03d", st->joy2y > 0 ? st->joy2y : 0);
		DrawStringS(x+2*fw, y, 1.0f, 1.0f, 1.0f, msg);
	}
	//Do we have the next few ones?
	x = orig_x;
	y += fh;
	
	// Adds existing but without known implementations...
	// Second d-pad and D button
	if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD2_UP)
	{
		isPressed = st->buttons & CONT_DPAD2_UP;
		DrawStringS(x+ 3*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "U");
	}
	if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD2_DOWN)
	{
		isPressed = st->buttons & CONT_DPAD2_DOWN;
		DrawStringS(x+ 5*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "D");
	}
	if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD2_LEFT)
	{
		isPressed = st->buttons & CONT_DPAD2_LEFT;
		DrawStringS(x+ 7*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "L");
	}
	if(dev->info.function_data[0] & CONT_FIXED_CAPABILITY_DPAD2_RIGHT)
	{
		isPressed = st->buttons & CONT_DPAD2_RIGHT;
		DrawStringS(x+ 9*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "R");
	}
	if(!isMaracas && dev->info.function_data[0] & CONT_FIXED_CAPABILITY_D)
	{
		isPressed = st->buttons & CONT_D;
		DrawStringS(x+11*fw, y, isPressed ? 0.0f : 1.0f, 1.0f, isPressed ? 0.0f : 1.0f, "D");
	}
}

void ControllerTest()
{
	int 		done = 0;
	uint16		pressed;
	ImagePtr	back;
	controller	*st;

	back = LoadKMG("/rd/ControlBack.kmg.gz", 0);
	if(!back)
		return;
	//back->alpha = 0.5f;
	while(!done && !EndProgram) 
	{				
		float x = 40, y = 30, w = 30*fw, h = 10*fh;
		
		StartScene();
		DrawImage(back);
		DrawStringS(120, 20, 0.0f, 1.0f, 0.0f, "Controller Test"); 		
		
		DiplayController(0, x, y);
		DiplayController(1, x+w, y);
		DiplayController(2, x, y+h);
		DiplayController(3, x+w, y+h);
		
		DrawStringS(60, y+2*h, 0.0f, 1.0f, 0.0f, "Press Left & Start on Controller 1 to Exit"); 
		
        EndScene();
		
		VMURefresh("Controller", "");
		
		st = ReadController(0, &pressed);
				
		if (st->buttons & CONT_START  &&
			st->buttons & CONT_DPAD_LEFT)
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
#define MAX_LOCATIONS	17

void MemoryViewer()
{
	int 		done = 0, ascii = 0, locpos = 0, docrc = 0, factor = 1;
	int			joycntx = 0, joycnty = 0;
	uint16		pressed;		
	uint32		address = 0, crc = 0;
	uint32		locations[MAX_LOCATIONS] = { 	0x00000000, 0x00200000,
												0x00240000, 0x04000000,
												0x0C000000, 0x0C000100, // KallistiOS 
												0x0C004000, 0x0C008000, // KallistiOS 
												0x0C008300, 0x0C00b800, // KallistiOS 
												0x0C00e400, 0x0C010000, // KallistiOS 
												0x10000000, 0x10800000,
												0x14000000, 0x1C000000, 0x1FFFFFFF};
	char		*names[MAX_LOCATIONS] = { 	"Boot ROM (2 Mb)", "Flash ROM", 
											"System devices", "Texture (8 Mb)",
											"KOS Sys State", "KOS Syscalls",
											"KOS Reserved", "IP.BIN",
											"KOS Bootstrap 1", "KOS Bootstrap 2",
											"KOS Unused", "KOS Application",
											"System memory", "Tile Accelerator",
											"Texture memory", "G2 devices", "NULL"};
	controller	*st;
		
	address = locations[0];
	while(!done && !EndProgram) 
	{
		int 	i = 0, j = 0, pos = -1;
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
		
		for(i = 0; i < MAX_LOCATIONS; i++)
		{
			if(address < locations[i])
			{
				pos = i - 1;
				break;
			}
		}
		if(pos != -1)
			DrawStringS(VISIBLE_HORZ*factor*3*fw, (VISIBLE_VERT*factor/2)*(fh-1), 1.0f, 1.0f, 0.0f, names[pos]);
		
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
		
		VMURefresh("MemView", "");
		st = ReadController(0, &pressed);
		
		JoystickDirectios(st, &pressed, &joycntx, &joycnty);
		if (pressed & CONT_DPAD_LEFT)	
		{
			if(address > locations[0])
				address -= VISIBLE_HORZ*factor*VISIBLE_VERT*factor;
			else
				address = locations[0];
				
			if(address >= 0x670000 && address < 0x04000000)
				address = 0x670000;
		}
		
		if (pressed & CONT_DPAD_RIGHT)
		{
			address += VISIBLE_HORZ*factor*VISIBLE_VERT*factor;

			if(address >= 0xFFFFFFFF) 
				address = 0xFFFFFFFF-VISIBLE_HORZ*factor*VISIBLE_VERT*factor;
				
			if(address >= 0x670000 && address < 0x04000000)
				address = 0x04000000;
		}
		
		if (pressed & CONT_DPAD_UP)	
		{
			if(address > locations[0]+0x10000)
				address -= 0x10000;
			else
				address = locations[0];
				
			if(address >= 0x670000 && address < 0x04000000)
				address = 0x670000;
		}
		
		if (pressed & CONT_DPAD_DOWN)
		{
			address += 0x10000;
			
			if(address >= 0xFFFFFFFF) // 0x817fffff
				address = 0xFFFFFFFF-VISIBLE_HORZ*factor*VISIBLE_VERT*factor;

			if(address >= 0x670000 && address < 0x04000000)
				address = 0x04000000;
		}
		
		if (pressed & CONT_B)
			done =	1;
			
		if (pressed & CONT_A)
		{
			locpos ++;
			if(locpos == MAX_LOCATIONS - 1)
				locpos = 0;
			address = locations[locpos];
		}
			
		if (pressed & CONT_Y)
			ascii =	!ascii;
			
		if (pressed & CONT_X)
			docrc =	!docrc;

		if (pressed & CONT_START)
			ShowMenu(MEMVIEWHELP);
	}
	return;
}

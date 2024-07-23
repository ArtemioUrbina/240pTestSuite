/* 
 * 240p Test Suite
 * Copyright (C)2011-2023 Artemio Urbina
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
#include "gba.h"

#include "hardware.h"

#include "help.h"
#include "menu.h"

/*
typedef struct gc_control_st{
	u8 connected;
	u16 bttn;
	s8 x;
	s8 y;
	s8 sx;
	s8 sy;
	u8 l;
	u8 r;
} GC_STATUS;

void read_gc(GC_STATUS *gc_ctrl)
{
	int chan = 0;
	u32	pads = 0;
	
	pads = PAD_ScanPads();
	for(chan = PAD_CHAN0; chan <= PAD_CHAN3; chan++)
	{
		memset(&gc_ctrl[chan], 0, sizeof(GC_STATUS));
		//if(pads & (1 << chan))
		{
			gc_ctrl[chan].connected = 1;
			gc_ctrl[chan].bttn = PAD_ButtonsHeld(chan);
			gc_ctrl[chan].x = PAD_StickX(chan);
			gc_ctrl[chan].y = PAD_StickY(chan);
			gc_ctrl[chan].l = PAD_TriggerL(chan);
			gc_ctrl[chan].r = PAD_TriggerR(chan);
			gc_ctrl[chan].sx = PAD_SubStickX(chan);
			gc_ctrl[chan].sy = PAD_SubStickY(chan);
		}
	}
}

void DrawGCController(u16 x, u16 y, GC_STATUS *gc_ctrl, u16 chan)
{
	char 		buffer[10];
	
	sprintf(buffer, "%d", gc_ctrl[chan].x);
	DrawStringS(x, y, 0x00, 0xff, 0x00, buffer);
	sprintf(buffer, "%d", gc_ctrl[chan].y);
	DrawStringS(x+20, y, 0x00, 0xff, 0x00, buffer);

	//C-stick:
	sprintf(buffer, "%d", gc_ctrl[chan].sx);
	DrawStringS(x+40, y, 0x00, 0xff, 0x00, buffer);
	sprintf(buffer, "%d", gc_ctrl[chan].sy);
	DrawStringS(x+80, y, 0x00, 0xff, 0x00, buffer);
}

#ifdef WII_VERSION
typedef struct wii_control_st{
	u8 connected;
	int bttn;
	u32 exp;
	WPADData *data;
	unsigned int irx;
	unsigned int iry;
	float pitch;
	float roll;
} WII_STATUS;

void read_wii(WII_STATUS *wii_ctrl)
{
	int chan = 0;
	u32 wmotes = 0;
	
	wmotes = WPAD_ScanPads();
	for(chan = WPAD_CHAN_0; chan <= WPAD_MAX_WIIMOTES; chan++)
	{
		memset(&wii_ctrl[chan], 0, sizeof(WII_STATUS));
		
		if(wmotes & (1 << chan))
			wii_ctrl[chan].connected = 1;
		if(wii_ctrl[chan].connected && WPAD_Probe(chan, &wii_ctrl[chan].exp))
		{
			WPAD_SetDataFormat(chan, WPAD_FMT_BTNS_ACC_IR);
			wii_ctrl[chan].bttn = WPAD_ButtonsHeld(chan);
			wii_ctrl[chan].data = WPAD_Data(chan);
			if(wii_ctrl[chan].data)
			{
				wii_ctrl[chan].irx = wii_ctrl[chan].data->ir.x;
				wii_ctrl[chan].iry = wii_ctrl[chan].data->ir.y;
				wii_ctrl[chan].pitch = wii_ctrl[chan].data->orient.pitch;
				wii_ctrl[chan].roll = wii_ctrl[chan].data->orient.roll;
				switch (wii_ctrl[chan].exp) {
					case WPAD_EXP_NUNCHUK:
					break;
					case WPAD_EXP_CLASSIC:
					break;
					case WPAD_EXP_GUITARHERO3:
					break;
					case WPAD_EXP_NONE:
					default:
					break;
				}
			}
		}		
	}
}
#endif

void ControllerTest()
{
	int 		done = 0;
	u32			pressed;
	ImagePtr	back;
	GC_STATUS	gc_ctrl[4];
#ifdef WII_VERSION
	WII_STATUS	wii_ctrl[4];
#endif
	
	back = LoadImage(BACKIMG, 0);
	if(!back)
		return;

	memset(&gc_ctrl, 0, sizeof(GC_STATUS)*4);
#ifdef WII_VERSION
	memset(&wii_ctrl, 0, sizeof(WII_STATUS)*4);
#endif
	while(!done && !EndProgram) 
	{	
		StartScene();

		DrawImage(back);
		
		DrawGCController(100, 100, gc_ctrl, 0);
		DrawGCController(100, 120, gc_ctrl, 1);
		DrawGCController(100, 140, gc_ctrl, 2);
		DrawGCController(100, 160, gc_ctrl, 3);
		
		EndScene();
		
		read_gc(gc_ctrl);
#ifdef WII_VERSION
		//read_wii(wii_ctrl);
#endif
		ControllerScan();
		pressed = Controller_ButtonsDown(0);
		// TODO exit
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

#ifdef WII_VERSION
#define	MAX_LOCATIONS	4
#else
#define	MAX_LOCATIONS	1
#endif

void MemoryViewer()
{
	int 		done = 0, ascii = 0, locpos = 0, docrc = 0;
	u32			pressed;		
	uint32_t	address = 0, crc = 0;
#ifdef WII_VERSION
	uint32_t	locations[MAX_LOCATIONS] = { 0x80000000, 0x90000000, 0xD0000000, 0xfff00000 };
#else
	uint32_t	locations[MAX_LOCATIONS] = { 0x80000000 };
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
					
					buffer[0] = 32;				// Space
					c = mem[i*VISIBLE_HORZ+j];
					if(c >= 32 && c <= 126)		// ASCII range
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

#ifdef WII_VERSION		
			if(address >= 0xFFFFFFFF) // 0x817fffff
				address = 0xFFFFFFFF-VISIBLE_HORZ*VISIBLE_VERT;
#else
			if(address >= 0x817fffff) 
				address = 0x817fffff-VISIBLE_HORZ*VISIBLE_VERT;
#endif
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
			
#ifdef WII_VERSION		
			if(address >= 0xFFFFFFFF) // 0x817fffff
				address = 0xFFFFFFFF-VISIBLE_HORZ*VISIBLE_VERT;
#else
			if(address >= 0x817fffff) 
				address = 0x817fffff-VISIBLE_HORZ*VISIBLE_VERT;
#endif
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

int ScanPorts()
{
	int port = 0;
	
	for(port = 0; port < 4; port ++)
		if(checkGBAConnected(port))
			return port;
	return -1;
}

void GBALink()
{
	int 		done = 0, port = -1, doscan = 1, firstscan = 1;
	u32			pressed;
	ImagePtr	back;
	char		msg[256];
	
	back = LoadImage(GBAIMG, 0);
	if(!back)
		return;

	InitGBARAM();
	back->alpha = 128;
	while(!done && !EndProgram) 
	{	
		if(doscan)
		{
			port = ScanPorts();
			if(port != -1)
				sprintf(msg, "GBA detected on Port %d.\nPress A to start transfer", port + 1);
			else
			{
				if(!firstscan)
					DrawMessageBox(back, "Game Boy Advance not found\nPlease connect GBA via Link Cable");
				else
					firstscan = 0;
				sprintf(msg, "Press A to scan for a GBA on any GC port.");
			}	
			doscan = 0;
		}
		
		StartScene();
		DrawImage(back);
		DrawStringC(40, 0x00, 0xff, 0x00, "Transfer GBA 160p Suite via Link Cable");
		DrawStringC(40+fh, 0x00, 0xaa, 0xaa, "by pinobatch v0.23"); 
		DrawStringS(80, 120, 0xff, 0xff, 0xff, msg); 
		DrawStringC(200, 0xee, 0xee, 0xee, "Press B to close"); 
		
		EndScene();
		
		ControllerScan();
		pressed = Controller_ButtonsDown(0);

		if (pressed & PAD_BUTTON_A)
		{
			char tmsg[256];
			if(port != -1)
			{
				switch(GBASendROM(back, port)) {
					case GBA_TRANSFER_OK:
						sprintf(tmsg, "Transfer complete");
						break;
					case GBA_INVALID_PORT:
						sprintf(tmsg, "Invalid Port %d", port);
						break;
					case GBA_ROM_TOO_BIG:
						sprintf(tmsg, "ROM is too big for GBA RAM");
						break;
					case GBA_BIOS_TIMEOUT:
						sprintf(tmsg, "BIOS is not ready.\nBoot GBA without cartridge.");
						break;
					case GBA_INVALID_SESSION:
						sprintf(tmsg, "BIOS didn't send session key.");
						break;
					case GBA_TRANSFER_ERROR:
						sprintf(tmsg, "Transfer failed due to an interruption.");
						break;
					case GBA_FINISH_ERROR:
						sprintf(tmsg, "Transfer not received by GBA.");
						break;
					default:
						sprintf(tmsg, "Unknown error. Please retry.");
						break;
				}
				DrawMessageBox(back, tmsg);
			}
			else
				doscan = 1;
		}
		
		if (pressed & PAD_BUTTON_START)
		{
			DrawMenu = 1;
			HelpData = GBA_TRANSF_HELP;
		}
		
		if (pressed & PAD_BUTTON_B)
			done =	1;
	}
	
	FreeImage(&back);
	ReleaseGBARAM();
	return;
}
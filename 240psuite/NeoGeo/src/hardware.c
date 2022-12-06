/* 
 * 240p Test Suite for the Neo Geo
 * by Artemio Urbina and Dustin Dembrosky (Dasutin)
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdlib.h>
#include <stdio.h>
#include <DATlib.h>
#include <input.h>
#include "types.h"
#include "ng.h"
#include "string_ng.h"
#include "externs.h"
#include "tests.h"
#include "help.h"
#include "tools.h"
#include "sound.h"

inline void DrawController(u16 x, u16 y, BYTE input, BYTE start, BYTE ps, BYTE select, BYTE mvscredit, BYTE credit)
{
	fixPrint(x+4, y,   input & JOY_UP ? fontColorRed : fontColorWhite, 3, "Up");
	fixPrint(x,   y+1, input & JOY_LEFT ? fontColorRed : fontColorWhite, 3, "Left");
	fixPrint(x+6, y+1, input & JOY_RIGHT ? fontColorRed : fontColorWhite, 3, "Right");
	fixPrint(x+3, y+2, input & JOY_DOWN ? fontColorRed : fontColorWhite, 3, "Down");

	fixPrint(x+13, y, ps & start ? fontColorRed : fontColorWhite, 3, "Start");
	if (isMVS)
	{
		// Alow drawing select with MVS1F which have AES ports and UNIBIOS in AES mode
		fixPrint(x+13, y+1, ps & select ? fontColorRed : fontColorWhite, 3, ps & select ? "Select" : "      ");
		fixPrint(x+13, y+2, !(mvscredit & credit) ? fontColorRed : fontColorWhite, 3, "Credit");
	}
	else
		fixPrint(x+13, y+2, ps & select ? fontColorRed : fontColorWhite, 3, "Select");

	fixPrint(x+22, y+1, input & JOY_A ? fontColorRed : fontColorWhite, 3, "A");
	fixPrint(x+23, y+1, input & JOY_B ? fontColorRed : fontColorWhite, 3, "B");
	fixPrint(x+24, y+1, input & JOY_C ? fontColorRed : fontColorWhite, 3, "C");
	fixPrint(x+25, y+1, input & JOY_D ? fontColorRed : fontColorWhite, 3, "D");
}

#define DC_X	7

void ht_controller_test()
{
	int done = 0, hardDip2 = 0, lastDip2 = 0, y = 0, enable4p = 0;
	picture image;
	BYTE mvssel = 0, mvscredit = 0, detected4p = 0;

	gfxClear();

	pictureInit(&image, &back, 1, 16, 0, 0,FLIP_NONE);
	palJobPut(16,back.palInfo->count,back.palInfo->data);

	detected4p = setup4P();

	while (!done)
	{
		SCClose();
		waitVBlank();

		if (isMVS)
		{
			mvscredit = volMEMBYTE(REG_STATUS_A); 
			mvssel    = volMEMBYTE(REG_STATUS_B); 
			hardDip2  = getHardDipValue(DP_CHUTES);
			if (hardDip2 != lastDip2)
			{
				suiteClearFixLayer();
				lastDip2 = hardDip2;
			}
		}

		if (isMVS && (detected4p || hardDip2      || // hard dip 2 status
			volMEMBYTE(BIOS_4P_MODE)    == 0xFF   || // Main 4P flag, is set when hard dip 2 is on and 4P board is found. 
			volMEMBYTE(BIOS_4P_PLUGGED) == 0xFF   || // 4P compatible bios will check for 4P board regardless of dip2 switch status. 
			volMEMBYTE(SD_FORCE_4P)))				 // Soft dip failback, so the user can force it in MVS mode
		{
			y = 9;
			enable4p = 1;
		} else {
			y = 13;
			enable4p = 0;
		}

		p1 = volMEMBYTE(P1_CURRENT);    // Controller 1
		p2 = volMEMBYTE(P2_CURRENT);    // Controller 2
		if (enable4p)
		{
			p1b = volMEMBYTE(P1B_CURRENT);  // Controller 3
			p2b = volMEMBYTE(P2B_CURRENT);  // Controller 4
		}
		ps  = volMEMBYTE(PS_CURRENT);
		p1e = volMEMBYTE(P1_EDGE);
		p2e = volMEMBYTE(P2_EDGE);

		if (isMVS)
		{
			if(!AES_AS_MVS)
				fixPrint(29, 8, !(mvscredit & MVS_SERV_B) ? fontColorRed : fontColorWhite, 3, "Service");
			if (is4S || is6S)
			{
				fixPrint(4,  24, !(mvssel & MVS_SEL1) ? fontColorRed : fontColorWhite, 3, "Sel 1");
				fixPrint(31, 24, !(mvssel & MVS_SEL2) ? fontColorRed : fontColorWhite, 3, "Sel 2");
			}
		}

		// Controller 1
		DrawController(DC_X, y, p1, P1_START, ps, P1_SELECT, AES_AS_MVS ? mvssel : mvscredit, AES_AS_MVS ? MVS_SEL1 : P1_CREDIT);
		y += 4;

		// Controller 2
		DrawController(DC_X, y, p2, P2_START, ps, P2_SELECT, AES_AS_MVS ? mvssel : mvscredit, AES_AS_MVS ? MVS_SEL2 : P2_CREDIT);
		if(enable4p)
		{
			y += 4;
			// Controller 3
			DrawController(DC_X, y, p1b, P1B_START, ps, P1B_SELECT, mvscredit, P3_CREDIT);
			y += 4;

			// Controller 4
			DrawController(DC_X, y, p2b, P2B_START, ps, P2B_SELECT, mvscredit, P4_CREDIT);
		}

		fixPrint(9, 26, fontColorGreen, 3, "Use START+LEFT to exit");

		if (HELD_START && HELD_LEFT)
			done = 1;
	}
}

/*
CRC 32 based on work by Christopher Baker <https://christopherbaker.net>
*/

u32 _state = ~0L;

static const u32 crc32_table[] = {
	0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
	0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
	0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
	0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
};

void CRC32_reset()
{
	_state = ~0L;
}

void CRC32_update(u8 data)
{
	u8 tbl_idx = 0;

	tbl_idx = _state ^ (data >> (0 * 4));
	_state = (*(u32*)(crc32_table + (tbl_idx & 0x0f)) ^ (_state >> 4));
	tbl_idx = _state ^ (data >> (1 * 4));
	_state = (*(u32*)(crc32_table + (tbl_idx & 0x0f)) ^ (_state >> 4));
}

u32 CRC32_finalize()
{
	return ~_state;
}

/**********************************/

u32 CalculateCRC(u32 startAddress, u32 size)
{
	u8 *start = NULL;
	u32 address = 0, checksum = 0;
	CRC32_reset();

	start = (void*)startAddress;
	for (address = 0; address < size; address ++)
	{
		u8 data;
		
		data = start[address];

		CRC32_update(data);
	}

	checksum = CRC32_finalize();
	return checksum;
}

#ifdef __cd__
u32 CalculateCRCBlink(u32 startAddress, u32 size, blinker *blinkdata)
{
	u8 *start = NULL;
	u32 address = 0, checksum = 0;
	int cycles = 0, cycle = 0;

	CRC32_reset();

	cycles = size/780;
	start = (void*)startAddress;
	for (address = 0; address < size; address ++)
	{
		u8 data;
		
		data = start[address];

		CRC32_update(data);
		if(blinkdata)
		{
			cycle ++;
			if(cycle >= cycles && getVideoline() > 0x1EF)
			{
				if(SD_blink_cycle(blinkdata))
				{
					SCClose();
					waitVBlank();
				}
				cycle = 0;
			}
		}
	}

	checksum = CRC32_finalize();
	return checksum;
}
#endif

#define MAX_LOCATIONS 11

void ht_memory_viewer(u32 address)
{
	int done = 0, redraw = 1, docrc = 0, locpos = 1, pos = 0, ascii = 0, start = 0;
	u32 crc = 0, locations[MAX_LOCATIONS] = { 0, 0x100000, 0x10F300, 0x110000, 0x200000, 0x300000, 
											0x400000, 0x402000, 0x800000, 0xC00000, 0xD00000 };

	gfxClear();

	for (pos = 0; pos < MAX_LOCATIONS; pos++)
	{
		if (locations[pos] == address)
		{
			locpos = pos;
			break;
		}
	}

	if(vmode_snk)
		start = 1;

	while (!done)
	{
		if (redraw)
		{
			int		i = 0, j = 0;
			u8		*mem = NULL;
			char	buffer[10];

			memset(buffer, 0, sizeof(char)*10);
			mem = (u8*)address;

			suiteClearFixLayer();

			if (docrc)
				crc = CalculateCRC(address, 0x1C0);

			intToHex(address, buffer, 8);
			fixPrint(32-start, 2, fontColorRed, 3, buffer);
			intToHex(address+448, buffer, 8);
			fixPrint(32-start, 29, fontColorRed, 3, buffer);

			if (docrc)
			{
				intToHex(crc, buffer, 8);
				fixPrint(32, 16, fontColorGreen, 3, buffer);
			}

			for (i = 0; i < 30; i++)
			{
				for (j = 0; j < 16; j++)
				{
					if (!ascii)
					{
						intToHex(mem[i*16+j], buffer, 2);
						fixPrint(start+j*2, i+2, fontColorWhite, 3, buffer);
					} else {
						u8 c;
					
						c = mem[i*16+j];
						// ASCII range
						if (c >= 32 && c <= 126)	
							fixPrintf(start+j*2, i+2, fontColorWhite, 3, "%c", (char)c);
					}
				}
			}
			redraw = 0;
		}

		SCClose();
		waitVBlank();

		readController();

		if (BTTN_EXIT)
			done = 1;

		if (BTTN_MAIN)
		{
			docrc = !docrc;
			redraw = 1;
		}

		if (BTTN_OPTION_1)
		{
			locpos ++;
			if (locpos == MAX_LOCATIONS)
				locpos = 0;
			address = locations[locpos];
			redraw = 1;
		}

		if (BTTN_OPTION_2)
		{
			ascii = !ascii;
			redraw = 1;
		}

		if (PRESSED_LEFT)
		{
			if (address >= 0x1C0)
				address -= 0x1C0;
			else
				address = 0;

			redraw = 1;
		}

		if (PRESSED_RIGHT)
		{
			address += 0x1C0;	
			redraw = 1;
		}

		if (PRESSED_UP)
		{
			if (address >= 0x10000)
				address -= 0x10000;
			else
				address = 0;

			redraw = 1;
		}

		if (PRESSED_DOWN)
		{
			address += 0x10000;
			redraw = 1;
		}

		if(checkHelp(HELP_MEMVIEW))
			redraw = 1;
	}
	return;
}

void ht_test_ng_ram()
{
	int done = 0;
	picture image;

	gfxClear();

	pictureInit(&image, &back, 1, 16, 0, 0,FLIP_NONE);
	palJobPut(16,back.palInfo->count,back.palInfo->data);

	while (!done)
	{
		SCClose();
		waitVBlank();

		readController();

		if (BTTN_EXIT)
			done = 1;
	}
}

typedef struct bios_data {
	u8		type;
	u32		crc;
	char	*name;
	char	*text;
} BIOSID;

#define BIOS_SNK_MVS	1
#define BIOS_SNK_AES	2
#define BIOS_SNK_NGCD	3
#define BIOS_HACK		4
#define BIOS_HACK_NGCD	5

const BIOSID bioslist[] = {
#ifndef __cd__
{	BIOS_HACK,
	0x465F5764,
	"uni-bios_4_0.rom",
	"Universe Bios (Hack, Ver. 4.0)" },
{	BIOS_HACK,
	0x2BB7B46A,
	"uni-bios_3_3.rom",
	"Universe Bios (Hack, Ver. 3.3)" },
{	BIOS_HACK,
	0xEAD2EFFE,
	"uni-bios_3_2.rom",
	"Universe Bios (Hack, Ver. 3.2)" },
{	BIOS_HACK,
	0xBA8F4B1E,
	"uni-bios_3_1.rom",
	"Universe Bios (Hack, Ver. 3.1)" },
{	BIOS_HACK,
	0xCB54AAD7,
	"uni-bios_3_0.rom",
	"Universe Bios (Hack, Ver. 3.0)" },
{	BIOS_HACK,
	0xE5224EBD,
	"uni-bios_2_3.rom",
	"Universe Bios (Hack, Ver. 2.3)" },
{	BIOS_HACK,
	0x3775739E,
	"uni-bios_2_3o.rom",
	"Universe Bios (Hack, Ver. 2.3 - Alt)" },
{	BIOS_HACK,
	0xC6F8AC92,
	"uni-bios_2_2.rom",
	"Universe Bios (Hack, Ver. 2.2)" },
{	BIOS_HACK,
	0xF341E486,
	"uni-bios_2_1.rom",
	"Universe Bios (Hack, Ver. 2.1)" },
{	BIOS_HACK,
	0x406F79B2,
	"uni-bios_2_0.rom",
	"Universe Bios (Hack, Ver. 2.0)" },
{	BIOS_HACK,
	0xD8A97133,
	"uni-bios_1_3.rom",
	"Universe Bios (Hack, Ver. 1.3)" },
{	BIOS_HACK,
	0x6C4BACD6,
	"uni-bios_1_2.rom",
	"Universe Bios (Hack, Ver. 1.2)" },
{	BIOS_HACK,
	0xED81E4EB,
	"uni-bios_1_2o.rom",
	"Universe Bios (Hack, Ver. 1.2 - Alt)" },
{	BIOS_HACK,
	0xAFBC316A,
	"uni-bios_1_1.rom",
	"Universe Bios (Hack, Ver. 1.1)" },
{	BIOS_HACK,
	0x9DE9D5F1,
	"uni-bios_1_0.rom",
	"Universe Bios (Hack, Ver. 1.0)" },
{	BIOS_HACK,
	0xEA57D3AD,
	"neopen.sp1",
	"NeopenBios v0.1" },
{	BIOS_SNK_MVS,
	0xEE4E56EF,
	"vs-bios.rom",
	"Japan MVS (Ver. 3)" },
{	BIOS_SNK_MVS,
	0x66BC1D26,
	"sp-j2.sp1",
	"Japan MVS (Ver. 2)" },
{	BIOS_SNK_MVS,
	0xC00A0476,
	"sp1.jipan.1024",
	"Japan MVS (Ver. 1)" },
{	BIOS_SNK_MVS,
	0x6893A277,
	"japan-j3.bin",
	"Japan MV1B" },
{	BIOS_SNK_MVS,
	0xF1E44B08,
	"sp1-j3.bin",
	"Japan MVS (J3, alt)" },
{	BIOS_SNK_MVS,
	0x4C747A4D,
	"sp-j3.sp1",
	"Japan NEO-MVH MV1C" },
{	BIOS_SNK_MVS,
	0x15192F9F,
	"sp-s2.sp1",
	"US/Europe MVS (Ver. 2)" },
{	BIOS_SNK_MVS,
	0x7E65EA24,
	"sp-s.sp1",
	"US/Europe MVS (Ver. 1)" },
{	BIOS_SNK_MVS,
	0xEFD21CD4,
	"sp-45.sp1",
	"Asia NEO-MVH MV1C" },
{	BIOS_SNK_MVS,
	0xCD0F00E7,
	"sp-s3.sp1",
	"Asia MV1B" },
{	BIOS_SNK_MVS,
	0xCAB95DE9,
	"sp-u2.sp1",
	"US MVS (Ver. 2?)" },
{	BIOS_SNK_MVS,
	0xB907061C,
	"sp-e.sp1",
	"US MVS (Ver. 1)" },
{	BIOS_SNK_MVS,
	0xE86773D2,
	"sp1-u2",
	"US MVS (4 slot, Ver 2)" },
{	BIOS_SNK_MVS,
	0xCB2E44A4,
	"sp1-u4.bin",
	"US MVS (U4)" },
{	BIOS_SNK_MVS,
	0x8F5EBA5E,
	"sp1-u3.bin",
	"US MVS (U3)" },
{	BIOS_SNK_MVS,
	0xF7C94873,
	"sp-1v1_3db8c.bin",
	"Custom Japanese Hotel" },
{	BIOS_SNK_AES,
	0xF481E11,
	"neo-po.bin",
	"Japan AES" },
{	BIOS_SNK_AES,
	0x2C50CBCA,
	"neo-epo.bin",
	"Export AES" },
{	BIOS_SNK_AES,
	0x7A0D4410,
	"neodebug.rom",
	"Development System ROM" },
#else
{	BIOS_SNK_NGCD,
	0x56C9ECBA,				// 0xC36A47C0
	"top-sp1.bin",
	"Top loading Neo-Geo CD" },
{	BIOS_SNK_NGCD,
	0x6FD2A520,				// 0xCAC62307
	"front-sp1.bin",
	"Front loading Neo-Geo CD" },
{	BIOS_SNK_NGCD,
	0x33697892,				// 0xDF9DE490
	"neocd.bin",
	"Neo Geo CDZ" },
{	BIOS_HACK_NGCD,
	0x75FB6DD4,				// 0xFFB3127
	"uni-bioscd32.rom",
	"Universe Bios (Hack, Ver. 3.2)" },
{	BIOS_HACK_NGCD,
	0x879163E9,				// 0xFF3ABC59
	"uni-bioscd33.rom",
	"Universe Bios (Hack, Ver. 3.3)" },
// these might change a lot, taken from real HW
// since sdloader patches the BIOS on the fly
{	BIOS_HACK_NGCD,
	0x00A651C2,
	"sdloader.rom",
	"Top Loader patched by SDLoader" },
// SMKDAN versions from 2010
{	BIOS_HACK_NGCD,
	0x72E41278,				// 0x1101F313
	"neocd_sf.rom",
	"Front Loader BIOS (SMKDAN)" },
{	BIOS_HACK_NGCD,
	0x566E3D63,				// 0x1C299CC
	"neocd_st.rom",
	"CDZ BIOS (SMKDAN)" },
{	BIOS_HACK_NGCD,
	0x81B9D0C3,				// 0x42F6B6C4
	"neocd_sz.rom",
	"CDZ BIOS (SMKDAN)" },
#endif
{	0, 0, NULL, NULL } };

// search known BIOS
const BIOSID *GetBIOSbyCRC(u32 checksum)
{
	u8 i = 0;
	
	while(bioslist[i].crc != 0)
	{
		if(checksum == bioslist[i].crc)
			return &bioslist[i];
		i++;
	}
	return NULL;
}

u8 detectUNIBIOSfast(u32 address)
{
	u16 *bios = (u16*)address;

#ifndef __cd__
	if (bios[0x58] == 0x4E55)
		return 1;
#else
	if (bios[0x400B0] == 0x4E55)
		return 1;
#endif
	return 0;
}

#ifndef __cd__
void cleanBIOSStr(char *str, u8 size)
{
	u8 i = 0;

	for (i = 0; i < size; i++) {
		u8 c;

		c = str[i];
		if (c >= 32 && c <= 126) { // ASCII range
			str[i] = (char)c;
		} else {
			str[i] = ' ';
		}
	}
}

void byteSwap(u8 *data, u8 size)
{
	int pos = 0;

	while (pos < size)
	{
		u8 t;

		t = data[pos];
		data[pos] = data[pos+1];
		data[pos+1] = t;
		pos += 2;
	}
}

u8 getBIOS_region(u32 address)
{
	u8 *bios = (u8*)address;

	return(bios[0x000401]);
}

u8 getBIOS_HW(u32 address)
{
	u8 *bios = (u8*)address;

	return(bios[0x000400]);
}

void displayBIOS(u32 address, u8 swap)
{
	int  line = 0, len = 32, start = 0x82, x = 4;
	char buffer[34];

	if (detectUNIBIOSfast(address))
	{
		len = 16;
		start = 0xA0;
		x = 12;
	}

	for (line = 0; line < 4; line++)
	{
		memcpy(buffer, (void*)(address+start+line*len), len);
		if (swap)
			byteSwap(buffer, len);
		buffer[len] = '\0';
		cleanBIOSStr(buffer, len);
		fixPrintf(x, 10+line, fontColorGreen, 3, buffer);
	}
}

void drawBIOSHeader(u32 address, short x, short y)
{
	u8				bios_HW, bios_region;

	fixPrint(x, y, fontColorGreen, 3, "Header:");
	bios_HW = getBIOS_HW(address);
	switch(bios_HW)
	{
		case SYSTEM_AES:
			fixPrint(x+8, y, fontColorWhite, 3, "AES");
		break;

		case SYSTEM_MVS:
			fixPrint(x+8, y, fontColorWhite, 3, "MVS");
		break;

		default:
			fixPrint(x+8, y, fontColorWhite, 3, "???");
		break;
	}

	bios_region = getBIOS_region(address);
	switch(bios_region)
	{
		case SYSTEM_EUROPE:
			fixPrint(x+8, y+1, fontColorWhite, 3, "World");
		break;

		case SYSTEM_JAPAN:
			fixPrint(x+8, y+1, fontColorWhite, 3, "Japan");
		break;

		case SYSTEM_USA:
			fixPrint(x+8, y+1, fontColorWhite, 3, "USA");
		break;

		default:
			fixPrint(x+8, y+1, fontColorWhite, 3, "???");
		break;
	}
}
#endif

void ht_check_ng_bios_crc(u32 address)
{
	int				done = 0;
#ifndef __cd__
	int				swap = 0;
#endif
	u32				crc = 0;
	char			buffer[34];
	const BIOSID 	*bios = NULL;
#ifdef __cd__
	blinker			blinkdata;
	int				sprindex = 0;
#endif

	gfxClear();

#ifndef __cd__
	draw_background();

	// Print BIOS lines
	if (detectUNIBIOSfast(address))
		swap = 1;

	displayBIOS(address, swap);
	drawBIOSHeader(address, 13, 23);
#else
	sprindex = draw_background_w_gil(&blinkdata);
#endif
	menu_footer();

	fixPrintf(12, 16, fontColorGreen, 3, "Please Wait...");

	SCClose();
	waitVBlank();

#ifndef __cd__
	crc = CalculateCRC(address, BIOS_SIZE);
#else
	crc = CalculateCRCBlink(address, BIOS_SIZE, &blinkdata);
	clear_gillian(sprindex, &blinkdata);
#endif
	intToHex(crc, buffer, 8);
	fixPrintf(12, 16, fontColorGreen, 3, "CRC:  ");
	fixPrintf(17, 16, fontColorWhite, 3, "0x%s ", buffer);

	bios = GetBIOSbyCRC(crc);
	if (bios)
	{
		fixPrintf(6, 18, fontColorWhite, 3, bios->name);
		fixPrintf(6, 19, fontColorWhite, 3, bios->text);
	} else {
		fixPrintf(13, 18, fontColorWhite, 3, "Unknown BIOS");
		fixPrintf(13, 19, fontColorWhite, 3, "Please report it");
		if (detectUNIBIOSfast(address))
		{
			fixPrintf(7, 21, fontColorGreen, 3, "Non-free versions of UNIBIOS");
			fixPrintf(7, 22, fontColorGreen, 3, "can't be recognized by CRC");
		}
#ifndef __cd__
		else
		{
			fixPrintf(7, 21, fontColorGreen, 3, "SDLoader patches the BIOS");
			fixPrintf(7, 22, fontColorGreen, 3, "and these are not identified");
		}
#endif
	}

	while (!done)
	{
		SCClose();
		waitVBlank();

		readController();

		if (BTTN_EXIT)
			done = 1;

#ifndef __cd__
		if (BTTN_MAIN)
		{
			swap = !swap;
			displayBIOS(address, swap);
		}
#endif
	}
}

void ht_displayregs()
{
	int done = 0, redraw = 1;

	while (!done)
	{
#ifndef __cd__
		int y = 12;
#else
		int y = 10;
#endif

		if (redraw)
		{
			gfxClear();
			draw_background();
			redraw = 0;
		}

		SCClose();
		waitVBlank();

		readController();

		fixPrintC(6, fontColorGreen, 3, "HW Registers");

		displayRegWord(4, y++, "REG_LSPCMODE", REG_LSPCMODE);
		y++;
		displayRegByte(4, y++, "REG_STATUS_A", REG_STATUS_A);
		displayRegByte(4, y++, "REG_STATUS_B", REG_STATUS_B);
		displayRegByte(4, y++, "BIOS_MVS_FLAG", BIOS_MVS_FLAG);
		displayRegByte(4, y++, "REG_SYSTYPE", REG_SYSTYPE);
		displayRegByte(4, y++, "REG_DIPSW", REG_DIPSW);
#ifdef __cd__
		displayRegWord(4, y++, "REG_CDCONFIG", REG_CDCONFIG);
		y++;
#endif

		if (DEBUG_ENABLED)
		{
			displayRegByte(4, y++, "BIOS_COUNTRY", BIOS_COUNTRY_CODE);
			fixPrintf(4, y, fontColorGreen, 3, "Soft Dips:");
			fixPrintf(26, y, fontColorWhite, 3, "%01d%01d%01d%01d %01d%01d%01d%01d",
				getSoftDipvalue(SD_MVS_DEMO),
				getSoftDipvalue(SD_DISP_CREDITS),
				getSoftDipvalue(SD_FORCE_4P),
				getSoftDipvalue(SD_DFLT_320_H),
				getSoftDipvalue(SD_WARNING_ENABLE),
				getSoftDipvalue(SD_MV1C_STEREO),
				getSoftDipvalue(SD_UNUSED_7),
				getSoftDipvalue(SD_UNUSED_8));
		}

		menu_footer();

		if (BTTN_EXIT)
			done = 1;
	}
	return;
}

// _CDDA_FLAG needs to be set to 0x02
// in cdt0cd.s for this to work
// or there will be writes to Z80 RAM
// ($FEF8 and $FEF9 would be set to 00)

void ht_z80RAMtest()
{
	int done = 0, redraw = 1, execute = 0;

	if(!verifyZ80Version())
		draw_warning("Incorrect M1 ROM.\nAudio will be wrong.", 1, 16, 1);
	else {
		if(disable_z80_check)
			draw_warning("Z80 Check disabled\nIf this test fails\nthat's a probable cause.", 1, 16, 1);
	}

	while (!done)
	{
		if (redraw)
		{
			gfxClear();
			draw_background();
			
			fixPrintC(6, fontColorGreen, 3, "Z80 RAM Test");
			execute = 1;
			redraw = 0;
		}

		SCClose();
		waitVBlank();

		readController();

		if (BTTN_EXIT)
			done = 1;

		if (BTTN_MAIN || execute)
		{
			int status = 0, unknown = 0;

			fixPrintC(15, fontColorRed, 3, "               ");
			status = sendZ80command(RAMTESTCMD);
			if(status == Z80COMMAND_OK(RAMTESTCMD))
				fixPrintC(15, fontColorGreen, 3, "Z80 RAM OK");
			else if(status == Z80COMMAND_FAIL(RAMTESTCMD))
				fixPrintC(15, fontColorRed, 3, "RAM FAILED");
			else if(status == Z80COMMAND_TIME(RAMTESTCMD))
				fixPrintC(15, fontColorYellow, 3, "Z80 Timed Out");
			else
			{
				fixPrintC(15, fontColorRed, 3, "Unknown status");
				unknown = 1;
			}
			fixPrintC(26, fontColorWhite, 3, "Press B to exit");

			if (unknown || DEBUG_ENABLED)
			{
				char buffer[4];

				intToHex(status, buffer, 2);
				fixPrintf(12, 17, fontColorWhite, 3, "Z80 replied: 0x%s", buffer);
			}
			execute = 0;
		}
	}
	return;
}

void ht_showInternalVars()
{
	int done = 0, redraw = 1;

	while (!done)
	{
		int x = 12, y = 8;

		if (redraw)
		{
			gfxClear();
			draw_background();
			redraw = 0;
		}

		SCClose();
		waitVBlank();

		readController();

		fixPrintC(6, fontColorGreen, 3, "Suite Internal Vars");

		displayValue(x, y++, "isMVS", isMVS);
#ifndef __cd__
		displayValue(x, y++, "is4S", is4S);
		displayValue(x, y++, "is6S", is6S);
		displayValue(x, y++, "isMulti", isMulti);
#endif
		displayValue(x, y++, "hwChange", hwChange);
		displayValue(x, y++, "IRE107", allowIRE107);
		displayValue(x, y++, "vmode_snk", vmode_snk);
		displayValue(x, y++, "isPAL", isPAL);
		displayValue(x, y++, "usePAL256", usePAL256);
		displayValue(x, y++, "isPALinMVS", isPALinMVS);
		displayValue(x, y++, "e_shadow", enable_shadow);
		displayValue(x, y++, "first_grid", first_grid);
		displayValue(x, y++, "first_overs", first_overscan);
		displayValue(x, y++, "first_cramp", first_colorramp);
		displayValue(x, y++, "min_z80", min_z80_timout);
		displayValue(x, y++, "max_z80", max_z80_timout);
		displayValue(x, y++, "color_bg", fill_color_bg);
#ifdef __cd__
		displayValue(x, y++, "isCDFront", isCDFront);
		displayValue(x, y++, "isCDZ", isCDZ);
		displayValue(x, y++, "ngcd_region", ngcd_region);
#endif

		menu_footer();

		if (BTTN_EXIT)
			done = 1;
	}
	return;
}
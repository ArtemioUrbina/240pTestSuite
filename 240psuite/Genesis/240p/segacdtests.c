/* 
 * 240p Test Suite
 * Copyright (C)2011-2014 Artemio Urbina
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

#include "segacdtests.h"
#include "main.h"
#include "res.h"

#define BIOS_SIZE	0x20000

// Detect RetroArch emulation Issue when accessing BIOS for CRC
#define BIOS_EMU_ISSUE

// Allow use of Mirror BIOS for CRC, not used
//#define BIOS_MIRROR_ALLOW

#ifndef SEGACD
u8 hIntPatchNeeded = 1;
#else
u8 hIntPatchNeeded = 0;  // Don't do it when booting from Sega CD
#endif

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

int memcmp(const void *s1, const void *s2, int n)
{
    unsigned char u1, u2;

    for ( ; n-- ; s1++, s2++) {
		u1 = * (unsigned char *) s1;
		u2 = * (unsigned char *) s2;
		if ( u1 != u2) {
			return (u1-u2);
		}
    }
    return 0;
}

#ifndef SEGACD
// As suggested by Leo Oliveira
int DetectSCDviaExpansion()
{
	volatile char *pointer = NULL;
	u16	ExpansionNotConnected = 0, data = 0;
	
	pointer = (char *)0xA10001;
	data = *pointer;
	ExpansionNotConnected = (data & 0x20);

	return(!ExpansionNotConnected);
}
#endif

#ifdef BIOS_EMU_ISSUE
int DetectEmulationIssue()
{
#ifdef SEGACD
	uint8_t *bios = (uint8_t *)0;
#else
	uint8_t *bios = (uint8_t *)0x400000;
#endif
	
	// RetroArch GenPlus GX has 0x00
	if(bios[0x70] != 0xFF)
		return 1;
	
	return(0);
}
#endif

// from ChillyWilly Example of Mode1 playback
/*
 * Check for CD BIOS
 * When a cart is inserted in the MD, the CD hardware is mapped to
 * 0x400000 instead of 0x000000. So the BIOS ROM is at 0x400000, the
 * Program RAM bank is at 0x420000, and the Word RAM is at 0x600000.
 */
static volatile uint8_t *DetectSCDBIOS(uint32_t address)
{
    uint8_t *bios;
	
    bios = (uint8_t *)0x015800+address;
    if (memcmp(bios + 0x6D, "SEGA", 4))
    {
        bios = (uint8_t *)0x016000+address;
        if (memcmp(bios + 0x6D, "SEGA", 4))
        {
            // check for WonderMega/X'Eye
            if (memcmp(bios + 0x6D, "WONDER", 6))
            {
                bios = (uint8_t *)0x01AD00+address; // might also be 0x40D500
                // check for LaserActive
                if (memcmp(bios + 0x6D, "SEGA", 4))
					return 0;
            }
        }
    }
    return bios;
}

#ifndef SEGACD
int DetectBSSCDBIOS(uint32_t address)
{
    uint8_t *bios;
	
    bios = (uint8_t *)0x0100+address;
    if(memcmp(bios, "ESAG", 4))
		return 0;
    return 1;
}
#endif

uint32_t CalculateCRC(uint32_t startAddress, uint32_t size, u8 patch)
{
	uint8_t *bios = NULL;
	uint32_t address = 0, checksum = 0;
#ifdef BIOS_EMU_ISSUE
	uint16_t emuPatch = 0;

	emuPatch = DetectEmulationIssue();
#endif
	CRC32_reset();

	bios = (void*)startAddress;
	for (address = 0; address < size; address ++)
	{
		uint8_t data;
		
		data = bios[address];

#ifndef SEGACD		
		// The BIOS is overlapped with a shadow of the HINT register by 
		// the ASIC in cart mode, and that alters the CRC
		if(patch == 1)
		{
			if(address == 0x72)
				data = 0xFD;
			if(address == 0x73)
				data = 0x0C;
		}
		// Byteswapped ?
		if(patch == 2)
		{
			if(address == 0x72)
				data = 0x0C;
			if(address == 0x73)
				data = 0xFD;
		}
#endif

#ifdef BIOS_EMU_ISSUE
		// There is an issue under some emulators, patch it
		if(emuPatch && address == 0x70)
		{
			data = 0xFF;
			emuPatch = 0;
		}
#endif
		CRC32_update(data);
	}

	checksum = CRC32_finalize();
	return checksum;
}

void ShowMessageAndData(char *message, uint32_t value, int len, int palmsg, int xpos, int ypos)
{
	int			msglen = 0;
	
	msglen = strlen(message);
	VDP_Start();
	VDP_drawTextBG(APLAN, message, TILE_ATTR(palmsg, 0, 0, 0), xpos, ypos);
	if(len)
	{
		char	buffer[40];
		
		intToHex(value, buffer, len);
		VDP_drawTextBG(APLAN, " 0x", TILE_ATTR(PAL0, 0, 0, 0), xpos+msglen, ypos);
		VDP_drawTextBG(APLAN, buffer, TILE_ATTR(PAL0, 0, 0, 0), xpos+msglen+3, ypos);
	}
	VDP_End();
}

/*
void ShowSEGABIOSData(uint32_t address)
{
	char buffer[10];
    int possible[] = { 0x01586D, 0x01606D, 0x01AD6D, 0x00D56D }, i;

	for(i = 0; i < 4; i++)
	{
		uint8_t *bios = NULL;
		
		bios = (uint8_t*)(possible[i]+address);
	
		ShowMessageAndData("BIOS", (uint32_t)bios, 6, PAL0, 4, 6+i*3);
		
		VDP_Start();
		VDP_drawTextBG(APLAN, "ASCII:", TILE_ATTR(PAL1, 0, 0, 0), 19, 7+i*3);
		buffer[0] = bios[0];
		buffer[1] = bios[1];
		buffer[2] = bios[2];
		buffer[3] = bios[3];
		buffer[4] = '\0';
		VDP_drawTextBG(APLAN, buffer, TILE_ATTR(PAL0, 0, 0, 0), 26, 7+i*3);
		
		VDP_drawTextBG(APLAN, "Hex:", TILE_ATTR(PAL1, 0, 0, 0), 21, 8+i*3);
		intToHex(bios[0], buffer+0, 2);
		intToHex(bios[1], buffer+2, 2);
		intToHex(bios[2], buffer+4, 2);
		intToHex(bios[3], buffer+6, 2);
		VDP_drawTextBG(APLAN, buffer, TILE_ATTR(PAL0, 0, 0, 0), 26, 8+i*3);
		VDP_End();
	}
	
    return;
}
*/

void PrintBIOSInfo(uint32_t address)
{
	uint8_t *bios = NULL;	
	char	buffer[50];
	int		i = 0, j = 0, data[] = { 16, 16, 17, 15, 16, 48, 15, -1, 16, -4, -4, -4, -4, -2, -2, -4, -4, -12, -40, 16, 0};
	

	bios = (uint8_t*)(address+0x0100);

	while(data[i] != 0)
	{
		if(data[i] > 0)
		{
			memcpy(buffer, bios, sizeof(uint8_t)*data[i]);
			buffer[data[i]] = '\0';
			VDP_Start();
			VDP_drawTextBG(APLAN, buffer, TILE_ATTR(PAL0, 0, 0, 0), 12, 8+j);
			VDP_End();
			j++;
		}
		bios += data[i] > 0 ? data[i] : -1*data[i];
		i++;
	}
}

int Test8bitRegionRegion(u8 value, u32 startaddress, u32 size)
{
	u8		*ram = NULL;
	u32		byte = 0;
	
	ram = (u8*)startaddress;
	for(byte = 0; byte < size; byte++)
	{
		ram[byte] = value;
		if(ram[byte] != value)
			return byte;
	}
	
	return MEMORY_OK;
}

int Test16bitRegion(u16 value, u32 startaddress, u32 size)
{
	u16		*ram = NULL;
	u32		word = 0;
	
	ram = (u16*)startaddress;
	size = size/2;
	for(word = 0; word < size; word++)
	{
		ram[word] = value;
		if(ram[word] != value)
			return word*2;
	}
	
	return MEMORY_OK;
}

int CheckRAMWithValue(u32 start, u32 end, u16 value, int pos, u16 numbits)
{
	int memoryFail = 0;
	
	ShowMessageAndData("Setting to", value, numbits == IS_8BIT ? 2 : 4, PAL1, 12, pos);
	
	if(numbits == IS_16BIT)
		memoryFail = Test16bitRegion(value, start, end - start);
	else
		memoryFail = Test8bitRegionRegion((u8)(value & 0x00FF), start, end - start);
	
	if(memoryFail != MEMORY_OK)
	{
		ShowMessageAndData("FAILED", memoryFail, 6, PAL3, 12, pos+1);
		return 0;
	}
	
	VDP_Start();
	VDP_drawTextBG(APLAN, "W/R OK", TILE_ATTR(PAL2, 0, 0, 0), 16, pos+1);
	VDP_End();
	return 1;
}

u16 WaitKey(char *message)
{
	u16 buttons, oldButtons = 0xffff, pressedButtons, close = 0;
	
	VDP_Start();
	VDP_drawTextBG(APLAN, message ? message : "     PRESS ANY BUTTON     ", TILE_ATTR(PAL1, 0, 0, 0), 8, 22);
	VDP_End();

	while(!close)
	{
		buttons = JOY_readJoypad(JOY_ALL);
		pressedButtons = buttons & ~oldButtons;
		oldButtons = buttons;

		if(pressedButtons & BUTTON_A)
			return BUTTON_A;
		if(pressedButtons & BUTTON_B)
			return BUTTON_B;
		if(pressedButtons & BUTTON_C)
			return BUTTON_C;
		if(pressedButtons & BUTTON_START)
			return BUTTON_START;
		VDP_waitVSync();
	}
	return 0;
}


/*************** Z80 *******/


void Z80RamTest()
{
	DrawMainBG();
	
	ShowMessageAndData("Z80 RAM", 0xA00000, 6, PAL1, 11	, 7);
	if(doZ80Lock)
		Z80_requestBus(1);			
	CheckRAMWithValue(0xA00000, 0xA02000, 0xFF, 10, IS_8BIT);
	CheckRAMWithValue(0xA00000, 0xA02000, 0x55, 12, IS_8BIT);
	CheckRAMWithValue(0xA00000, 0xA02000, 0xAA, 14, IS_8BIT);
	CheckRAMWithValue(0xA00000, 0xA02000, 0x00, 16, IS_8BIT);
	if(doZ80Lock)
		Z80_releaseBus();

	WaitKey(NULL);
}

/*************** end Z80 **********/


typedef struct bios_data {
    uint32_t crc;
    char *name;
} BIOSID;


const static BIOSID bioslist[] = {
{ 0xC6D10268, "M1 US 1.10" },	// mpr-15045b.bin
{ 0x529AC15A, "M1 EU 1.00" },	// megacd_model1_bios_1_00_e.bin
{ 0x79F85384, "M1 JP 1.0S" },	// mpr-14088h.bin swapped in MAME as 0x3773D5AA
{ 0x9BCE40B2, "M1 JP 1.0G" },	// epr-14088b.bin swapped in MAME as 0x69ED6CCD
{ 0xF18DDE5B, "M1 JP 1.0L" },	// mpr-14088c.bin swapped in MAME as 0x03134289
{ 0x2EA250C0, "M1 JP 1.0O" },	// epr-14088d.bin swapped in MAME as 0xDFA95EE9
{ 0x9D2DA8F2, "M1 JP 1.01" },	// epr-14088e.bin
{ 0x4BE18FF6, "M1 JP 1.11" },	// mpr-14837.bin Nyqvist dump, previously noted as SCD2, that's incorrect
{ 0x550F30BB, "M1 AS 1.0" },	// epr-14536h.bin
{ 0x2E49D72C, "M2 US 2.11X" },	// mpr-15764-t.bin
{ 0x8AF65F58, "M2 US 2.00" },	// us_scd2_930314.bin
{ 0x9F6F6276, "M2 US 2.00W" },	// segacd_model2_bios_2_00w_u.bin
{ 0x4D5CB8DA, "M2 EU 2.00W" },	// mpr-15512a.bin swapped in MAME as 0x53F1757C
{ 0x0507B590, "M2 EU 2.00" },	// mpr-15512.bin swapped in MAME as 0xCB76F114
{ 0xC1AA217F, "M2 EU 2.11X" },	// mpr-15811-t.bin swapped in MAME as 0x391a80d2, MK-4102A-50 Sony pcb
{ 0xDD6CC972, "M2 JP 2.00C" },	// mpr-15398.bin swapped in MAME as 0x1E4344E6
{ 0x8052C7A0, "AIWA JP 2.11" },	// mpr-15768-t.bin
{ 0x50CD3D23, "LA 1.04" },		// laseractive_bios_1_04_u.bin
{ 0x3B10CF41, "LA 1.02" },		// laseractive_bios_1_02_u.bin
{ 0x474AAA44, "LA JP 1.05A" },	// mega-ld 1.05 bios.bin
{ 0x1493522C, "LA JP 1.05B" },	// PD6126E dump by zaxour 93.833923% identical to above from PAC-S1 (JP) 
{ 0x00EEDB3A, "LA JP 1.02" },	// laseractive_bios_1_02_j.bin
{ 0x290F8E33, "X'EYE US 2.00"},	// g304.bin
{ 0xD21FE71D, "WM JP 1.00" },	// g301.bin
{ 0x2B19972F, "WM 2.00" },		// wondermega_m2_bios_2_00_j.bin
{ 0xD48C44B5, "CDX US 2.21X" },	// segacdx_bios_2_21_u.bin
{ 0xAACB851E, "MMEGA 2.21" },	// opr-16140.bin
{ 0, NULL } } ; 

typedef struct biosRF_data {
    uint32_t crcrf;
    uint32_t crcog;
} BIOSRF;

// V1
const static BIOSRF biosnamesRF1[] = {
{ 0xA94CFE69, 0xC6D10268 },  // hack of mpr-15045b.bin 0xC6D10268 named us_scd1_921011_regionfree_v1.bin
{ 0xB03B4FF7, 0x529AC15A },  // hack of megacd_model1_bios_1_00_e.bin 0x529AC15A named eu_mcd1_921027_regionfree_v1.bin
{ 0x193CB234, 0x550F30BB },  // hack of epr-14536h.bin 0x550F30BB named jp_mcd1_911228_region_free_v1.bin
{ 0x17AF77AC, 0x8AF65F58 },  // hack of us_scd2_930314.bin 0x8AF65F58 named us_scd2_930314_regionfree_v1.bin
{ 0x3B0FB51B, 0x0DD6CC97 },  // hack of mpr-15398.bin 0xDD6CC97 named JP_MegaCD2_22121992_regionfree_v1.bin
{ 0x2F56F336, 0x0507B590 },  // hack of mpr-15512.bin 0x0507B590 named EU_MegaCD2_30031993_regionfree_v1.bin
{ 0x848685FA, 0x290F8E33 },  // hack of g304.bin 0x290F8E33 named US_X'EYE_27121993_regionfree_v1.bin
{ 0xD2BD281F, 0xD21FE71D },  // hack of g301.bin 0xD21FE71D named JP_Wondermega_02061992_regionfree_v1.bin
{ 0x5B3A5F22, 0xD48C44B5 },  // hack of segacdx_bios_2_21_u.bin 0xD48C44B5 named US_SEGA_CDX_930907_regionfree_v1.bin
{ 0, 0 } } ; 

// V2
const static BIOSRF biosnamesRF2[] = {
{ 0x73784705, 0xC6D10268 },  // hack of mpr-15045b.bin 0xC6D10268 named us_scd1_921011_regfree.bin
{ 0xC82EE650, 0x529AC15A },  // hack of megacd_model1_bios_1_00_e.bin 0x529AC15A named eu_mcd1_921027_regfree.bin
{ 0x8E6943FA, 0x79F85384 },  // hack of mpr-14088h.bin 0x79F85384 named jp_mcd1_911228_regfree.bin
{ 0x1AEE2672, 0x9D2DA8F2 },  // hack of epr-14088e.bin 0x9D2DA8F2 named jp_mcd1_911217_regfree.bin
{ 0x29776266, 0x550F30BB },  // hack of epr-14536h.bin 0x550F30BB named jp_mcd1_911228_pal_regfree.bin
{ 0x6442BA81, 0x2E49D72C },  // hack of mpr-15764-t.bin 0x2E49D72C named us_scd2_930621_regfree.bin
{ 0x95118134, 0x8AF65F58 },  // hack of us_scd2_930314.bin 0x8AF65F58 named us_scd2_930314_regfree.bin
{ 0x7E86C5DA, 0x9F6F6276 },  // hack of segacd_model2_bios_2_00w_u.bin 0x9F6F6276 named us_scd2_930601_regfree.bin
{ 0x056663AD, 0x4D5CB8DA },  // hack of mpr-15512a.bin 0x4D5CB8DA named eu_mcd2_930601_regfree.bin
{ 0x316F3D80, 0x0507B590 },  // hack of mpr-15512.bin 0x0507B590 named eu_mcd2_930330_regfree.bin
{ 0x9E140799, 0xDD6CC972 },  // hack of mpr-15398.bin 0xDD6CC972 named jp_mcd2_921222_regfree.bin
{ 0x8F34F0EE, 0x8052C7A0 },  // hack of mpr-15768-t.bin 0x8052C7A0 named MPR-15768-T_regfree.bin
{ 0xB72F16D7, 0x290F8E33 },  // hack of g304.bin 0x290F8E33 named us_xeye_931227_regfree.bin
{ 0xD650FD93, 0xD21FE71D },  // hack of g301.bin 0xD21FE71D named jp_wmg_920206_regfree.bin
{ 0x3F5112F3, 0x2B19972F },  // hack of wondermega_m2_bios_2_00_j.bin 0x2B19972F named WONDERMEGA-G303_regfree.BIN
{ 0xC7EB4CC9, 0xD48C44B5 },  // hack of segacdx_bios_2_21_u.bin 0xD48C44B5 named us__cdx_930907_regfree.bin
{ 0xC6AD0AD7, 0xAACB851E },  // hack of opr-16140.bin 0xAACB851E named eu_mmg_930916_regfree.bin
{ 0, 0 } } ; 

typedef struct biosSwapped_data {
    uint32_t crcog;
    uint32_t crcsw;
} BIOS_SW;

// Reversed, MAME
const static BIOS_SW biosSwapped[] = {
{ 0x9BCE40B2, 0x69ED6CCD }, // epr-14088b.bin
{ 0x2EA250C0, 0xDFA95EE9 }, // epr-14088d.bin
{ 0x9D2DA8F2, 0xE2E70BC8 }, // epr-14088e.bin
{ 0x550F30BB, 0x47FA8EAC }, // epr-14536h.bin
{ 0xD21FE71D, 0xCF1D926B }, // g301.bin
{ 0x290F8E33, 0x6AEDAEA7 }, // g304.bin
{ 0x00EEDB3A, 0x885B4AD2 }, // laseractive_bios_1_02_j.bin
{ 0x3B10CF41, 0x3885B28C }, // laseractive_bios_1_02_u.bin
{ 0x50CD3D23, 0x2365BFBB }, // laseractive_bios_1_04_u.bin
{ 0x474AAA44, 0x046B1332 }, // mega-ld 1.05 bios.bin
{ 0x1493522C, 0xFA64FC8E }, // mega-ld 1.05 bios PD6126E
{ 0x529AC15A, 0x2BF760AF }, // megacd_model1_bios_1_00_e.bin
{ 0xF18DDE5B, 0x03134289 }, // mpr-14088c.bin
{ 0x79F85384, 0x3773D5AA }, // mpr-14088h.bin
{ 0xC6D10268, 0x2461B5ED }, // mpr-15045b.bin
{ 0x4BE18FF6, 0xE0A6179B }, // 920501 Nyqvist dump, not documented
{ 0xDD6CC972, 0x1E4344E6 }, // mpr-15398.bin
{ 0x0507B590, 0xCB76F114 }, // mpr-15512.bin
{ 0x4D5CB8DA, 0x53F1757C }, // mpr-15512a.bin
{ 0xC1AA217F, 0x391a80d2 }, // mpr-15811-t.bin
{ 0x2E49D72C, 0x9AAB8FE3 }, // mpr-15764-t.bin
{ 0x8052C7A0, 0x1E628066 }, // mpr-15768-t.bin
{ 0xAACB851E, 0x527E310B }, // opr-16140.bin
{ 0xD48C44B5, 0xD1EE6282 }, // segacdx_bios_2_21_u.bin
{ 0x8AF65F58, 0x0C653035 }, // us_scd2_930314.bin
{ 0x2B19972F, 0x9E13FDA6 }, // wondermega_m2_bios_2_00_j.bin
{ 0, 0 } } ; 

// Reversed Region Free V1
const static BIOS_SW biosSwapRF1[] = {
{ 0x17AF77AC, 0x8718BBDD }, // us_scd2_930314_regionfree_v1.bin
{ 0x2F56F336, 0xE7FC3195 }, // EU_MegaCD2_30031993_regionfree_v1.bin
{ 0x3B0FB51B, 0x2934AC09 }, // JP_MegaCD2_22121992_regionfree_v1.bin
{ 0xD2BD281F, 0x0D1F20CA }, // JP_Wondermega_02061992_regionfree_v1.bin
{ 0x5B3A5F22, 0x35BEABD7 }, // US_SEGA_CDX_930907_regionfree_v1.bin
{ 0x848685FA, 0x80466E1D }, // US_X'EYE_27121993_regionfree_v1.bin
{ 0xB03B4FF7, 0x5AC18566 }, // eu_mcd1_921027_regionfree_v1.bin
{ 0x193CB234, 0xAE2E2E77 }, // jp_mcd1_911228_region_free_v1.bin
{ 0xA94CFE69, 0x1F6E6CD6 }, // us_scd1_921011_regionfree_v1.bin
{ 0, 0 } } ; 

// Reversed Region Free V2
const static BIOS_SW biosSwapRF2[] = {
{ 0x8F34F0EE, 0x7FD5E1A6 }, // MPR-15768-T_regfree.bin
{ 0x3F5112F3, 0xC73A3A00 }, // WONDERMEGA-G303_regfree.BIN
{ 0xC82EE650, 0x564DBBFD }, // eu_mcd1_921027_regfree.bin
{ 0x316F3D80, 0xEE231235 }, // eu_mcd2_930330_regfree.bin
{ 0x056663AD, 0xCE516F6E }, // eu_mcd2_930601_regfree.bin
{ 0xC6AD0AD7, 0xFBFED8FC }, // eu_mmg_930916_regfree.bin
{ 0x1AEE2672, 0x7ED8334C }, // jp_mcd1_911217_regfree.bin
{ 0x29776266, 0x80505D5B }, // jp_mcd1_911228_pal_regfree.bin
{ 0x8E6943FA, 0x8D0869A5 }, // jp_mcd1_911228_regfree.bin
{ 0x9E140799, 0x1F43DC9C }, // jp_mcd2_921222_regfree.bin
{ 0xD650FD93, 0x6E96D5B5 }, // jp_wmg_920206_regfree.bin
{ 0xC7EB4CC9, 0xD5F14824 }, // us__cdx_930907_regfree.bin
{ 0x73784705, 0xA9E527D8 }, // us_scd1_921011_regfree.bin
{ 0x95118134, 0xBAFF6FE2 }, // us_scd2_930314_regfree.bin
{ 0x7E86C5DA, 0xB66A44F2 }, // us_scd2_930601_regfree.bin
{ 0x6442BA81, 0xA8ABBF26 }, // us_scd2_930621_regfree.bin
{ 0xB72F16D7, 0x8079FBD4 }, // us_xeye_931227_regfree.bin
{ 0, 0 } } ; 

	// search known Original BIOS
char *GetBIOSNamebyCRC(uint32_t checksum)
{
	int i = 0;
	
	while(bioslist[i].crc != 0)
	{		
		if(checksum == bioslist[i].crc)
			return bioslist[i].name;
		i++;
	}
	return NULL;
}

uint32_t GetBypeSwappedbyCRC(uint32_t checksum, const BIOS_SW* blist)
{
	int i = 0;
	
	while(blist[i].crcog != 0)
	{		
		if(checksum == blist[i].crcog)
			return blist[i].crcsw;
		i++;
	}
	return 0;
}
	
uint32_t GetOriginalbyCRC(uint32_t checksum, const BIOS_SW* blist)
{
	int i = 0;
	
	while(blist[i].crcog != 0)
	{		
		if(checksum == blist[i].crcsw)
			return blist[i].crcog;
		i++;
	}
	return 0;
}

int FindRegionFreeBios(uint32_t checksum, const BIOSRF* blist)
{
	int i = 0;
	
	while(blist[i].crcrf != 0)
	{		
		if(checksum == blist[i].crcrf)
		{
			char *name = NULL;
			
			name = GetBIOSNamebyCRC(blist[i].crcog);
			VDP_Start();
			VDP_drawTextBG(APLAN, "Region Free", TILE_ATTR(PAL2, 0, 0, 0), 6, 20);
			VDP_drawTextBG(APLAN, name, TILE_ATTR(PAL2, 0, 0, 0), 18, 20);
			VDP_End();
			return 1;
		}
		i++;
	}
	return 0;
}

#ifndef SEGACD
int FindByteSwappedBios(uint32_t checksum, const BIOS_SW* blist)
{
	uint32_t f_checksum = 0, bschecksum = 0;
	
	f_checksum = GetOriginalbyCRC(checksum, blist);
	if(f_checksum)
	{
		char *name = NULL;
		
		name = GetBIOSNamebyCRC(f_checksum);
		if(name)
		{
			ShowMessageAndData("CD BIOS CRC32:", checksum, 8, PAL1, 6, 19);

			bschecksum = GetBypeSwappedbyCRC(checksum, biosSwapped);
			
			VDP_Start();
			VDP_drawTextBG(APLAN, "Byte swapped", TILE_ATTR(PAL2, 0, 0, 0), 6, 20);
			VDP_drawTextBG(APLAN, name, TILE_ATTR(PAL2, 0, 0, 0), 19, 20);
			if(bschecksum)
				ShowMessageAndData("MAME CRC32:", bschecksum, 8, PAL1, 9, 21);
			VDP_End();
		}
		return 1;
	}
	return 0;
}
#endif

void doBIOSID(uint32_t checksum, uint32_t address)
{
	char 		*name = NULL;
	uint32_t 	bschecksum = 0;
	
	name = GetBIOSNamebyCRC(checksum);
	if(name)
	{	
		bschecksum = GetBypeSwappedbyCRC(checksum, biosSwapped);
		VDP_Start();
		if(bschecksum)
			ShowMessageAndData("MAME CRC32:", bschecksum, 8, PAL1, 9, 20);
		VDP_drawTextBG(APLAN, name, TILE_ATTR(PAL2, 0, 0, 0), 14, bschecksum == 0 ? 20 : 21);
		VDP_End();
		return;
	}
	
	// search Region Free BIOS v2
	if(FindRegionFreeBios(checksum, biosnamesRF2))
		return;
	// search Region Free BIOS v1
	if(FindRegionFreeBios(checksum, biosnamesRF1))
		return;
	
#ifndef SEGACD
	// Check for incorrectly burned Byte Swapped BIOS
	// Cart only of course, only run if detected
	if(DetectBSSCDBIOS(address))
	{
		uint32_t	bschecksum = 0;
		
		//ShowSEGABIOSData(address);
		
		VDP_Start();
		VDP_drawTextBG(APLAN, " working...", TILE_ATTR(PAL0, 0, 0, 0), 20, 19);
		VDP_End();
		
		// Calculate CRC with byteswapped HINT register
		bschecksum = CalculateCRC(address, BIOS_SIZE, 2);
		
		ShowMessageAndData("CD BIOS CRC32:", bschecksum, 8, PAL1, 6, 19);
		
		// search swapped Official BIOS CRCs
		if(FindByteSwappedBios(bschecksum, biosSwapped))
			return;
			
		// search swapped Region Free BIOS v2
		if(FindByteSwappedBios(bschecksum, biosSwapRF2))
			return;
			
		// search swapped Region Free BIOS v2
		if(FindByteSwappedBios(bschecksum, biosSwapRF1))
			return;
		
		//ShowMessageAndData("CD BIOS CRC32:", checksum, 8, PAL1, 6, 19);
	}
#endif
	
	// No match! check if we find the SEGA string and report
	if(DetectSCDBIOS(address))
	{
		VDP_Start();
		VDP_drawTextBG(APLAN, "Unknown BIOS, please report CRC", TILE_ATTR(PAL1, 0, 0, 0), 4, 20);
		VDP_End();
	}
	else
	{
		VDP_Start();
		VDP_drawTextBG(APLAN, "No BIOS found", TILE_ATTR(PAL1, 0, 0, 0), 12, 20);
		VDP_End();
	}
	return;
}

void CheckSegaCDBIOSCRC()
{
	uint8_t		cmd = 0;
	uint32_t	checksum = 0;
	uint32_t 	address = 0;
	
#ifndef SEGACD
	address = 0x400000;
#endif

	do
	{
		ShowMessageAndData("Sega CD BIOS Data at", address, PAL1, 8, 6, 4);
		PrintBIOSInfo(address);

#ifndef BIOS_MIRROR_ALLOW		
		cmd = WaitKey("'A' for CRC 'B' to exit");
#else
		cmd = WaitKey("'A' for CRC 'C' for Mirror");
		if(cmd == BUTTON_C)
		{
			if(address & 0x40000)
#ifndef SEGACD
				address = 0x400000;
#else
				address = 0;
#endif
			else
				address += 0x40000;
		}
#endif
		if(cmd == BUTTON_B)
			return;
	}
	while(cmd != BUTTON_A);
	
	VDP_Start();
	VDP_drawTextBG(APLAN, "Calculating, please wait", TILE_ATTR(PAL1, 0, 0, 0), 7, 19);
	VDP_End();

#ifndef SEGACD
	// If in cart mode, patch the BIOS due to HINT
	checksum = CalculateCRC(address, BIOS_SIZE, 1);
#else
	checksum = CalculateCRC(address, BIOS_SIZE, 0);
#endif

#ifdef BIOS_EMU_ISSUE
	if(DetectEmulationIssue())
		ShowMessageAndData("Emu issue (0x00)@:", address+0x70, 8, PAL1, 5, 18);
#endif
		
	ShowMessageAndData("CD BIOS CRC32:", checksum, 8, PAL1, 6, 19);
		
	VDP_waitVSync();	
	doBIOSID(checksum, address);

	WaitKey(NULL);
}

#define MMOD_REGISTER	0xA12002
#define HINT_REGISTER	0xA12006
#define CFLG_REGISTER	0xA1200E
#define CCMD_REGISTER	0xA12010

#ifndef SEGACD

void SetSCDRegisterBYTE(u32 reg, u8 value)
{
	u8 	*scdctrl = (u8 *)reg; 
	
	*scdctrl = value;
}

u8 GetSCDRegisterBYTE(u32 reg)
{
	u8 	*scdctrl = (u8 *)reg; 
	
	return(*scdctrl);
}

int checkRegisterBYTE(u32 reg, u8 value, int pos)
{
	u8 ret = 0;
	
	ShowMessageAndData("Setting to", value, 2, PAL1, 11, pos);
	
	SetSCDRegisterBYTE(reg, value);
	ret = GetSCDRegisterBYTE(reg);
	if(ret != value)
	{
		ShowMessageAndData("FAILED", ret, 2, PAL3, 14, pos+1);
		return 0;
	}
	
	VDP_Start();
	VDP_drawTextBG(APLAN, "W/R OK", TILE_ATTR(PAL2, 0, 0, 0), 16, pos+1);
	VDP_End();
	return 1;
}

void checkRegisterBYTEValues(char *title, u32 reg)
{
	int good = 0;
	
	ShowMessageAndData(title, reg, 8, PAL1, 7, 8);
	
	good += checkRegisterBYTE(reg, 0x55, 10);
	good += checkRegisterBYTE(reg, 0xAA, 12);
	good += checkRegisterBYTE(reg, 0xFF, 14);
	if(good) checkRegisterBYTE(reg, 0x00, 16);
	WaitKey(NULL);
}

#endif

void SetSCDRegisterWORD(u32 reg, u16 value)
{
	u16 	*scdctrl = (u16 *)reg; 
	
	*scdctrl = value;
}

u16 GetSCDRegisterWORD(u32 reg)
{
	u16 	*scdctrl = (u16 *)reg; 
	
	return(*scdctrl);
}

int checkRegisterWORD(u32 reg, u16 value, int pos)
{
	u16 ret = 0;
	
	ShowMessageAndData("Setting to", value, 4, PAL1, 11, pos);
	
	SetSCDRegisterWORD(reg, value);
	ret = GetSCDRegisterWORD(reg);
	if(ret != value)
	{
		ShowMessageAndData("FAILED", ret, 4, PAL3, 13, pos+1);
		return 0;
	}
	
	VDP_Start();
	VDP_drawTextBG(APLAN, "W/R OK", TILE_ATTR(PAL2, 0, 0, 0), 16, pos+1);
	VDP_End();
	return 1;
}

void checkRegisterWORDValues(char *title, u32 reg)
{
	int good = 0;
	
	ShowMessageAndData(title, reg, 8, PAL1, 7, 8);
	
	VDP_Start();
	VDP_clearTileMapRect(APLAN, 5, 10, 30, 28);
	VDP_End();
	
	good += checkRegisterWORD(reg, 0x5555, 10);
	good += checkRegisterWORD(reg, 0xAAAA, 12);
	good += checkRegisterWORD(reg, 0xFFFF, 14);
	if(good) checkRegisterWORD(reg, 0x0000, 16);
	WaitKey(NULL);
}


void CheckCtrlRegisters()
{
	int i = 0;
	
	for(i = 0; i < 16; i+=2)
		checkRegisterWORDValues("Command Register", CCMD_REGISTER+i);
}

int configHint()
{
	SetSCDRegisterWORD(HINT_REGISTER, 0xFD0C);
	if(GetSCDRegisterWORD(HINT_REGISTER) == 0xFD0C)
		hIntPatchNeeded = 0;
	else
		hIntPatchNeeded = 1;
		
	return(!hIntPatchNeeded);
}

int checkHintShadow()
{
#ifdef SEGACD
	uint8_t *bios = (uint8_t *)0;
#else
	uint8_t *bios = (uint8_t *)0x400000;
#endif
	
	if(bios[0x72] == 0xFD && bios[0x73] == 0x0C)
		hIntPatchNeeded = 0;
	else
		hIntPatchNeeded = 1;
	
	return(!hIntPatchNeeded);
}

void CheckHintRegister()
{
	if(configHint())
		ShowMessageAndData("HINT set OK", HINT_REGISTER, 8, PAL1, 9, 12);
	else
		ShowMessageAndData("HINT set FAILED", HINT_REGISTER, 8, PAL3, 7, 12);
	if(checkHintShadow())
		ShowMessageAndData("HINT Shadow OK", 0x72, 8, PAL1, 6, 13);
	else
		ShowMessageAndData("HINT Shadow FAILED", 0x72, 8, PAL3, 4, 13);
	WaitKey(NULL);
}

#ifndef SEGACD

int TestBankingProgramRAMFast()
{
	int		i;
	u8		*ram = (u8*)0x420000;
	
	for(i = 0; i < 4; i++)
	{
		u8 value;
		
		// Change Program RAM Bank
		value = (0x0A+i)*0x11;
		SetSCDRegisterWORD(MMOD_REGISTER, i << 6);
		memset(ram, 0, sizeof(u8)*0x1BF);
		ram[0x000] = value;
		ram[0x00F] = value;
		ram[0x1B0] = value;
		ram[0x1BF] = value;
	}
	
	for(i = 0; i < 4; i++)
	{
		u8 value;
		
		value = (0x0A+i)*0x11;
		// Change Program RAM Bank
		SetSCDRegisterWORD(MMOD_REGISTER, i << 6);
		if(ram[0x000] != value)
			return 0;
		if(ram[0x00F] != value)
			return 0;
		if(ram[0x1B0] != value)
			return 0;
		if(ram[0x1BF] != value)
			return 0;
	}
	return 1;
}

int TestBankingProgramRAMRegisters()
{
	int		i;
	u16		value = 0, ret = 0;
	
	for(i = 0; i < 4; i++)
	{
		// Change Program RAM Bank
		value = i << 6;
		SetSCDRegisterWORD(MMOD_REGISTER, value);
		ret = GetSCDRegisterWORD(MMOD_REGISTER) & 0xC0;
		if(value != ret)
		{
			ShowMessageAndData("SET", value, 4, PAL3, 14, 9);
			ShowMessageAndData("GOT", ret, 4, PAL3, 14, 10);
			return 0;
		}
	}
	
	return 1;
}


void CheckSCDProgramRAM()
{
	int		i = 0, banks = 4;
	
	ShowMessageAndData("Program RAM", 0x420000, 12, PAL1, 7, 7);
	if(!TestBankingProgramRAMRegisters())
	{
		ShowMessageAndData("Bank Switch FAIL", MMOD_REGISTER, 8, PAL3, 5, 8);
		
		VDP_Start();
		VDP_drawTextBG(APLAN, "Only one bank will be checked", TILE_ATTR(PAL2, 0, 0, 0), 5, 11);
		VDP_End();
		
		banks = 1;
		WaitKey(NULL);
	}
	else
	{
		VDP_Start();
		ShowMessageAndData("Bank Register OK", MMOD_REGISTER, 7, PAL2, 7, 14);
		VDP_End();
		WaitKey(NULL);
	}
	
	VDP_Start();
	VDP_clearTileMapRect(APLAN, 5, 10, 30, 28);
	VDP_End();
	
	if(banks == 4 && !TestBankingProgramRAMFast())
	{
		VDP_Start();
		VDP_drawTextBG(APLAN, "Fast Check Failed. The register", TILE_ATTR(PAL2, 0, 0, 0), 5, 10);
		VDP_drawTextBG(APLAN, "did W/R OK, but bank switch was", TILE_ATTR(PAL2, 0, 0, 0), 5, 11);
		VDP_drawTextBG(APLAN, "either not done or RAM is bad.", TILE_ATTR(PAL2, 0, 0, 0), 5, 12);
		VDP_drawTextBG(APLAN, "Try Memory Viewer at address", TILE_ATTR(PAL2, 0, 0, 0), 5, 14);
		VDP_drawTextBG(APLAN, "above and use C.", TILE_ATTR(PAL2, 0, 0, 0), 5, 15);
		VDP_End();
		if(WaitKey("'A' for check 'B' to exit") != BUTTON_A)
			return;
	}
	
	
	for(i = 0; i < banks; i++)
	{
		int good = 0;
		
		ShowMessageAndData("Bank Test", i, 2, PAL1, 13, 9);
		
		// Change Program RAM Bank
		SetSCDRegisterWORD(MMOD_REGISTER, i << 6);
		
		good = 0;
		
		good += CheckRAMWithValue(0x420000, 0x440000, 0xAAAA, 10, IS_16BIT);
		good += CheckRAMWithValue(0x420000, 0x440000, 0x5555, 12, IS_16BIT);
		good += CheckRAMWithValue(0x420000, 0x440000, 0xFFFF, 14, IS_16BIT);
		if(good == 3) CheckRAMWithValue(0x420000, 0x440000, 0x0000, 16, IS_16BIT);

		WaitKey(NULL);
		VDP_Start();
		VDP_clearTileMapRect(APLAN, 5, 10, 30, 28);
		VDP_End();
	}
}

void CheckSCDWordRAM()
{
	int good = 0;
	
	ShowMessageAndData("WORD RAM", 0x600000, 8, PAL1, 10, 8);
	
	good = 0;
	good += CheckRAMWithValue(0x600000, 0x640000, 0xAAAA, 10, IS_16BIT);
	good += CheckRAMWithValue(0x600000, 0x640000, 0x5555, 12, IS_16BIT);
	good += CheckRAMWithValue(0x600000, 0x640000, 0xFFFF, 14, IS_16BIT);
	if(good == 3) CheckRAMWithValue(0x600000, 0x640000, 0x0000, 16, IS_16BIT);

	WaitKey(NULL);
}
#endif

#define VISIBLE_HORZ	16
#define VISIBLE_VERT	28
#define	MAX_LOCATIONS	8

void MemViewer(uint32_t address)
{
	u8 			redraw = 1, docrc = 0, locpos = 1, bank = 0, pos = 0, ascii = 0;
	uint32_t	crc = 0, locations[MAX_LOCATIONS] = { 0, 0x020000, 0x200000, 0x400000, 0x420000, 0x600000, 0xA00000, 0xFF0000 };
	u16 		buttons, oldButtons = 0xffff, pressedButtons, close = 0;
	
	// Select Bank 0
	SetSCDRegisterWORD(MMOD_REGISTER, 0);
	
	for(pos = 0; pos < MAX_LOCATIONS; pos++)
	{
		if(locations[pos] == address)
		{
			locpos = pos;
			break;
		}
	}
	
	while(!close)
	{
		if(redraw)
		{
			int 	i = 0, j = 0;
			uint8_t *mem = NULL;
			char 	buffer[10];

			memset(buffer, 0, sizeof(char)*10);
			mem = (uint8_t*)address;
			
			if(docrc)
				crc = CalculateCRC(address, VISIBLE_HORZ*VISIBLE_VERT, 0);
			
			VDP_Start();
			VDP_clearTileMapRect(APLAN, 0, 0, 320 / 8, 224 / 8);
			intToHex(address, buffer, 8);
			VDP_drawTextBG(APLAN, buffer, TILE_ATTR(PAL1, 0, 0, 0), 32, 0);
			intToHex(address+448, buffer, 8);
			VDP_drawTextBG(APLAN, buffer, TILE_ATTR(PAL1, 0, 0, 0), 32, 27);
			if(docrc)
			{
				intToHex(crc, buffer, 8);
				VDP_drawTextBG(APLAN, buffer, TILE_ATTR(PAL2, 0, 0, 0), 32, 14);
			}
#ifdef SEGACD
			if(address >= 0x020000 && address < 0x040000)
#else
			if(address >= 0x420000 && address < 0x440000)
#endif
			{
				ShowMessageAndData("PB", bank, 2, PAL1, 33, 3);
				VDP_drawTextBG(APLAN, "C->bank", TILE_ATTR(PAL1, 0, 0, 0), 33, 4);
			}
			VDP_End();
			
			for(i = 0; i < VISIBLE_VERT; i++)
			{
				for(j = 0; j < VISIBLE_HORZ; j++)
				{
					if(!ascii)
						intToHex(mem[i*VISIBLE_HORZ+j], buffer, 2);
					else
					{
						uint16_t c;
						
						buffer[0] = 32;				// Space
						buffer[1] = 0;
						c = mem[i*VISIBLE_HORZ+j];
						if(c >= 32 && c <= 126)		// ASCII range
							buffer[0] = c;
					}
					VDP_Start();
					VDP_drawTextBG(APLAN, buffer, TILE_ATTR(PAL0, 0, 0, 0), j*2, i);
					VDP_End();
				}
			}
			
			redraw = 0;
		}

		buttons = JOY_readJoypad(JOY_ALL);
		pressedButtons = buttons & ~oldButtons;
		oldButtons = buttons;
		
		if(pressedButtons & BUTTON_A)
		{
			docrc = !docrc;
			redraw = 1;
		}
		
		if(pressedButtons & BUTTON_B)
		{
			locpos ++;
			if(locpos == MAX_LOCATIONS)
				locpos = 0;
			address = locations[locpos];
			redraw = 1;
		}
		
		if(pressedButtons & BUTTON_C)
		{
#ifdef SEGACD
			if(address >= 0x020000 && address < 0x040000)
#else
			if(address >= 0x420000 && address < 0x440000)
#endif
			{
				if(++bank > 3)
					bank = 0;
				SetSCDRegisterWORD(MMOD_REGISTER, bank << 6);
			}
			else
				ascii = !ascii;
			redraw = 1;
		}
		
		if(pressedButtons & BUTTON_START)
		{
			u16 type = 0;
#ifndef SEGACD	
			fmenudata resmenudata[] = { {0, "Mem View"}, {1, "Help"}, {2, "Fast Check"}, {3, "ASCII"}, {4, "exit"} };
#else
			fmenudata resmenudata[] = { {0, "Mem View"}, {1, "Help"}, {2, "CRC"}, {3, "ASCII"}, {4, "exit"} };
#endif

			type = DrawFloatMenu(1, resmenudata, 5);
			switch(type)
			{
				case 1:
					DrawHelp(HELP_MEMVIEW);	
					break;
				case 2:
#ifndef SEGACD	
					TestBankingProgramRAMFast();
#else
					docrc = !docrc;
#endif
					break;
				case 3:
					ascii = !ascii;
					break;
				case 4:
					close = 1;
					break;
				default:
					break;
			}
				
			oldButtons |= BUTTON_A;
			redraw = 1;
		}

		
		if(pressedButtons & BUTTON_LEFT)
		{
			if(address > VISIBLE_HORZ*VISIBLE_VERT)
				address -= VISIBLE_HORZ*VISIBLE_VERT;
			else
				address = 0;
			
			if(address >= 0x800000 && address < 0xA00000) // skip
				address = 0xA0FFFF;
				
			if(address >= 0xA0FFFF && address < 0xFF0000) // skip
				address = 0x7F0000;
				
			redraw = 1;
		}
		
		if(pressedButtons & BUTTON_RIGHT)
		{
			address += VISIBLE_HORZ*VISIBLE_VERT;
			
			if(address >= 0x800000 && address < 0xA00000) // skip
				address = 0xA00000;
				
			if(address >= 0xA0FFFF && address < 0xFF0000) // skip
				address = 0xFF0000;
				
			if(address >= 0xFFFFFF)
				address = 0xFFFE3F;
				
			redraw = 1;
		}
		
		if(pressedButtons & BUTTON_UP)
		{
			if(address >= 0x10000)
				address -= 0x10000;
			else
				address = 0;
			
			if(address >= 0x800000 && address < 0xA00000) // skip
				address = 0xA0FFFF;
				
			if(address >= 0xA0FFFF && address < 0xFF0000) // skip
				address = 0x7F0000;
		
			redraw = 1;
		}
		
		if(pressedButtons & BUTTON_DOWN)
		{
			address += 0x10000;
			
			if(address >= 0x800000 && address < 0xA00000) // skip
				address = 0xA00000;
				
			if(address >= 0xA0FFFF && address < 0xFF0000) // skip
				address = 0xFF0000;
				
			if(address >= 0xFFFFFF)
				address = 0xFFFE3F;

			redraw = 1;
		}
		VDP_waitVSync();
	}
}

void SegaCDMenu()
{
	u8 sel = 1;
	u16 cursel = 1, pos, reload = 1, redraw = 1;
	u16 buttons, oldButtons = 0xffff, pressedButtons;
	u16 done = 0, detectedhw = 1;

#ifndef SEGACD
	if(!DetectSCDviaExpansion())
	{
		fmenudata resmenudata[] = { {0, "SCD not detected"}, {1, "continue"}, {2, "return"} };
	
		if(DrawFloatMenu(2, resmenudata, 3) == 2)
			return;
		detectedhw = 0;
	}
	else
	{
		if(!DetectSCDBIOS(0x400000))
		{
			fmenudata resmenudata[] = { {0, "Unknown BIOS"}, {1, "continue"}, {2, "return"} };
		
			if(DrawFloatMenu(2, resmenudata, 3) == 2)
				return;
			detectedhw = 0;
		}	
	}
#endif
	while(!done)
	{
		checkHintShadow();
		if(reload)
		{
#ifndef SEGACD
			DrawMainBGwithGillian(1, 216, 72);
#else
			DrawMainBGwithGillian(1, 216, 90);
#endif
			reload = 0;
			redraw = 1;
		}

		pos = 7;
		if(redraw)
		{	
			sel = 1;
			VDP_Start();
			VDP_drawTextBG(APLAN, detectedhw ? "Sega CD Tests" : "Sega CD not detected", TILE_ATTR(PAL1, 0, 0, 0), detectedhw ? 14 : 11, 4);
			VDP_drawTextBG(APLAN, "BIOS CRC and info", TILE_ATTR(cursel == sel++ ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
			pos++;
			VDP_drawTextBG(APLAN, "Check HINT Register", TILE_ATTR(cursel == sel++ ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
#ifndef SEGACD
			VDP_drawTextBG(APLAN, "Check Flag Register", TILE_ATTR(cursel == sel++ ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
#endif
			VDP_drawTextBG(APLAN, "Check Comm Registers", TILE_ATTR(cursel == sel++ ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
			pos++;
#ifndef SEGACD
			VDP_drawTextBG(APLAN, "Program RAM Check", TILE_ATTR(cursel == sel++ ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
			VDP_drawTextBG(APLAN, "Word RAM Check", TILE_ATTR(cursel == sel++ ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
			pos++;
#endif
			VDP_drawTextBG(APLAN, "PCM RAM Check", TILE_ATTR(cursel == sel++ ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
			VDP_drawTextBG(APLAN, "Memory Viewer", TILE_ATTR(cursel == sel++ ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
			pos++;
			VDP_drawTextBG(APLAN, "Help", TILE_ATTR(cursel == sel++ ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
			VDP_drawTextBG(APLAN, "Back to Main Menu", TILE_ATTR(cursel == sel ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
			VDP_End();
			redraw = 0;
		}
		
		buttons = JOY_readJoypad(JOY_ALL);
		pressedButtons = buttons & ~oldButtons;
		oldButtons = buttons;

		if(CheckHelpAndVO(&buttons, &pressedButtons, HELP_SEGACD))
			reload = 1;

		if(pressedButtons & BUTTON_DOWN)
		{
			cursel++;
			if(cursel > sel)
				cursel = 1;
			redraw = 1;
		}

		if(pressedButtons & BUTTON_UP)
		{
			cursel--;
			if(cursel < 1)
				cursel = sel;
			redraw = 1;
		}

		if(pressedButtons & BUTTON_B || 
			pressedButtons & BUTTON_START)
		{
			done = 1;
		}
			
		if(pressedButtons & BUTTON_A)
		{
			if(cursel < sel - 2)
			{
				FadeAndCleanUp();
				DrawMainBG();
			}
			
			switch (cursel)
			{
			case 1:
				CheckSegaCDBIOSCRC();
				break;
			case 2:
				CheckHintRegister();
				break;
			case 3:
#ifndef SEGACD
				checkRegisterBYTEValues("Flag register", CFLG_REGISTER);
				break;
			case 4:		
#endif	
				CheckCtrlRegisters();
				break;
#ifndef SEGACD
			case 5:
				CheckSCDProgramRAM();
				break;
			case 6:
				CheckSCDWordRAM();
				break;
			case 7:
				PCMRAMCheck();
				break;
			case 8:
				CleanUp();
				MemViewer(0x400000);
				break;
			case 9:
				DrawHelp(HELP_SEGACD);
				break;
			case 10:
				done = 1;
				break;
#else
			case 4:
				PCMRAMCheck();
				break;
			case 5:
				CleanUp();
				MemViewer(0);
				break;
			case 6:
				DrawHelp(HELP_SEGACD);
				break;
			case 7:
				done = 1;
				break;
#endif
			}

			FadeAndCleanUp();
			buttons = JOY_readJoypad(JOY_ALL);
			pressedButtons = buttons & ~oldButtons;
			oldButtons = buttons;

			reload = 1;
		}

		VDP_waitVSync();
	}

	return;
}

void PrintPCMResults(u8 value, u16 result, u16 address)
{
	VDP_clearTileMapRect(APLAN, 0, 0, 320 / 8, 240 / 8);
	ShowMessageAndData("PCM RAM Test (Sega CD)", 0x2000, 4, PAL1, 4, 7);
	if(result == 0)
	{
		ShowMessageAndData("Memory Failed w/:", value, 2, PAL1, 9, 12);
		ShowMessageAndData("address/8bit:", address, 4, PAL1, 9, 13);
	}
	else
	{
		ShowMessageAndData("Memory OK w/:", value, 2, PAL1, 9, 12);
	}
	WaitKey(NULL);
}

u8 PCMRAMCheck()
{
	u16 result = 0, address = 0;

#ifndef SEGACD
	if(!segacd_init())
	{
		resetSegaCD();
		return 0;
	}
#endif

	result = SendSCDCommandRetVal(Op_CheckPCMRAM, 0x55, &address);
	PrintPCMResults(0x55, result, address);
	
	result = SendSCDCommandRetVal(Op_CheckPCMRAM, 0xAA, &address);
	PrintPCMResults(0xAA, result, address);
	
	result = SendSCDCommandRetVal(Op_CheckPCMRAM, 0xFF, &address);
	PrintPCMResults(0xFF, result, address);
	
	result = SendSCDCommandRetVal(Op_CheckPCMRAM, 0x00, &address);
	PrintPCMResults(0x00, result, address);
	
#ifndef SEGACD
	resetSegaCD();
#endif
	
	return 1;
}

void resetSegaCD()
{
	/*
	 * Reset the Gate Array - this specific sequence of writes is recognized by
	 * the gate array as a reset sequence, clearing the entire internal state -
	 * this is needed for the LaserActive
	 */
	write_word(0xA12002, 0xFF00);
	write_byte(0xA12001, 0x03);
	write_byte(0xA12001, 0x02);
	write_byte(0xA12001, 0x00);

    /*
     * Reset the Sub-CPU, request the bus
     */
    write_byte(0xA12001, 0x02);
    while (!(read_byte(0xA12001) & 2)) write_byte(0xA12001, 0x02); // wait on bus acknowledge
	
	// Disable vertical blank handler to generate Sub-CPU level 2 ints.
	segacd_int_enabled = 0;
}

uint8_t segacd_init()
{
	uint8_t		value = 0;
	int 		timeout = 0;
	uint8_t  	ypos = 6;
	volatile uint8_t *segacd_bios_addr;

	// Clear the screen
	VDP_clearTileMapRect(APLAN, 0, 0, 320 / 8, 240 / 8);
	
	// Detect the address of the SCD BIOS
	segacd_bios_addr = DetectSCDBIOS(0x400000);
	if (!segacd_bios_addr)
	{
		ShowMessageAndData("SCD BIOS not found", 0, 0, PAL1, 10, 10);
		WaitKey(NULL);
		return 0;
	}
	ShowMessageAndData("SCD BIOS Detected at", (uint32_t)segacd_bios_addr, 8, PAL1, 5, ypos++);
	
	// Prepare for loading the data
	resetSegaCD();
	
    /*
     * Decompress Sub-CPU BIOS to Program RAM at 0x00000
     */
	ShowMessageAndData("Decompressing Sub-CPU BIOS", 0, 0, PAL1, 5, ypos++);
    write_word(0xA12002, 0x0002); // no write-protection, bank 0, 2M mode, Word RAM assigned to Sub-CPU
    memset((void *)0x420000, 0, (uint8_t)0x20000); // clear program ram first bank - needed for the LaserActive
	Kos_Decomp(segacd_bios_addr, (uint8_t *)0x420000);

    /*
     * Copy Sub-CPU program to Program RAM at 0x06000
     */
	ShowMessageAndData("Copying Sub-CPU Program", sizeof(pcmcheck_scd), 4, PAL1, 5, ypos++);
	memcpy((void *)0x426000, pcmcheck_scd, sizeof(pcmcheck_scd));
	if (memcmp((void *)0x426000, pcmcheck_scd, sizeof(pcmcheck_scd)))
    {
		ShowMessageAndData("Failed writing Program RAM!", (uint32_t)0x420000, 8, PAL1, 5, ypos++);
		WaitKey(NULL);
		resetSegaCD();
        return 0;
    }
	
    write_byte(0xA12001, 0x01); // clear bus request, deassert reset - allow CD Sub-CPU to run
	write_byte(0xA1200E, 0x00); // clear main comm port
    while (!(read_byte(0xA12001) & 1)) 
	{
		write_byte(0xA12001, 0x01); // wait on Sub-CPU running
		write_byte(0xA1200E, 0x00); // clear main comm port
	}
	ShowMessageAndData("Sub CPU Started", 0, 0, PAL1, 5, ypos++);
	
	write_byte(0xA12010, 0x01); // clear command send (for the state machine in the SCD)
	/*
     * Set the vertical blank handler to generate Sub-CPU level 2 ints.
     * The Sub-CPU BIOS needs these in order to run.
     */
	segacd_int_enabled = 1;
    set_sr(0x2000); // enable interrupts

    /*
     * Wait for Sub-CPU program to set sub comm port indicating it is running -
     * note that unless there's something wrong with the hardware, a timeout isn't
     * needed... just loop until the Sub-CPU program responds, but 2000000 is about
     * ten times what the LaserActive needs, and the LA is the slowest unit to
     * initialize
     */
	
	timeout = 2000000;
    do
    {
		value = (u8)read_word(0xA12020);
		if(value != 0)
		{
			if (timeout-- <= 0)
			{
				ShowMessageAndData("SCD failed to start!", 0, 0, PAL1, 5, ypos+3);
				WaitKey(NULL);
				return 0;
			}
			ShowMessageAndData("Waiting for SP_Init", timeout, 3, PAL1, 5, ypos);
		}
		VDP_waitVSync();
    }while(value != 0);
	ypos ++;
	/*
     * Wait for Sub-CPU to indicate it is ready to receive commands
     */
	timeout = 500;
    do
	{
		value = (u8)read_word(0xA12020);
		if(value != 0)
		{
			if (timeout-- <= 0)
			{
				ShowMessageAndData("SCD failed to start!", 0, 0, PAL1, 5, ypos+3);
				WaitKey(NULL);
				return 0;
			}
			ShowMessageAndData("Waiting for SP_Main", timeout, 3, PAL1, 5, ypos);
		}
		VDP_waitVSync();
	}while (value != 0);
	ShowMessageAndData("SCD initialized. Press a button", 0, 0, PAL1, 5, ypos+3);
	
	WaitKey(NULL);
	VDP_clearTileMapRect(APLAN, 0, 0, 320 / 8, 240 / 8);
	return 1;
}

#ifdef SEGACD
// These are the functions called in Sega CD Mode
void SendSCDCommand(enum SCD_Command command)
{
	volatile unsigned char *segacd_comm = (void*)0xA1200E;
	
asm("SendSCDCOMM:");
	asm("tst.b	0xA1200F");
	asm("bne	SendSCDCOMM");

	*segacd_comm = 0;
asm("SendSCDWait:");
	asm("tst.b	0xA1200F");
	asm("beq	SendSCDWait");
	
	*segacd_comm = command;
asm("SendSCDWait2:");
	asm("tst.b	0xA1200F");
	asm("bne	SendSCDWait2");
}

u16 SendSCDCommandRetVal(enum SCD_Command command, u16 param, u16 *extraData)
{
	volatile unsigned char *segacd_comm = (void*)0xA1200E;
	volatile u16 *segacd_param = (void*)0xA12010;
	volatile u16 *return_val = (void*)0xA12020;
	volatile u16 *extraDataInt = (void*)0xA12022;
	u16	retval = 0;
	
asm("SendSCDCOMMwRet:");
	asm("tst.b	0xA1200F");
	asm("bne	SendSCDCOMMwRet");

	*segacd_comm = 0;
	*segacd_param = 0;
asm("SendSCDWaitwRet:");
	asm("tst.b	0xA1200F");
	asm("beq	SendSCDWaitwRet");
	
	*segacd_comm = command;
	*segacd_param = param;
asm("SendSCDWaitwRet2:");
	asm("tst.b	0xA1200F");
	asm("bne	SendSCDWaitwRet2");
	
	retval = *return_val;
	if(extraData)
		*extraData = *extraDataInt;
	return retval;
}

#else
// These are the functions called in Mode 1 (from the cartridge mode, different Addresses)
void SendSCDCommand(enum SCD_Command command)
{
	volatile u16 *segacd_comm = (void*)0xA12010;
	
asm("SendSCDCOMM:");
	asm("tst.w	0xA12020");
	asm("bne	SendSCDCOMM");

	*segacd_comm = 0;
asm("SendSCDWait:");
	asm("tst.w	0xA12020");
	asm("beq	SendSCDWait");
	
	*segacd_comm = command;
asm("SendSCDWait2:");
	asm("tst.w	0xA12020");
	asm("bne	SendSCDWait2");
}

u16 SendSCDCommandRetVal(enum SCD_Command command, u16 param, u16 *extraData)
{
	volatile u16 *segacd_comm = (void*)0xA12010;
	volatile u16 *segacd_param = (void*)0xA12012;
	volatile u16 *return_val = (void*)0xA12022;
	volatile u16 *extraDataInt = (void*)0xA12024;
	u16	retval = 0;

asm("SendSCDCOMMwRet:");
	asm("tst.w	0xA12020");
	asm("bne	SendSCDCOMMwRet");
	*segacd_comm = 0;
	*segacd_param = 0;
asm("SendSCDWaitwRet:");
	asm("tst.w	0xA12020");
	asm("beq	SendSCDWaitwRet");
	
	*segacd_comm = command;
	*segacd_param = param;
asm("SendSCDWaitwRet2:");
	asm("tst.w	0xA12020");
	asm("bne	SendSCDWaitwRet2");
	
	retval = *return_val;
	if(extraData)
		*extraData = *extraDataInt;
	return retval;
}
#endif

void write_byte(unsigned int dst, unsigned char val)
{
	unsigned char *_dst = (unsigned char *)dst;
	*_dst = val;
}

void write_word(unsigned int dst, unsigned short val)
{
	unsigned short *_dst = (unsigned short *)dst;
	*_dst = val;
}

void write_long(unsigned int dst, unsigned int val)
{
	unsigned int *_dst = (unsigned int *)dst;
	*_dst = val;
}

unsigned char read_byte(unsigned int src)
{
	volatile unsigned char *_src = (unsigned char *)src;
	return(*_src);
}
 
unsigned short read_word(unsigned int src)
{
	volatile unsigned short *_src = (unsigned short *)src;
	return(*_src);
}
 
unsigned int read_long(unsigned int src)
{
	volatile unsigned int *_src = (unsigned int *)src;
	return(*_src);
}




#include "segacdtests.h"
#include "main.h"
#include "res.h"

/*
CRC 32 based on work by Christopher Baker <https://christopherbaker.net>
*/


uint32_t _state = ~0L;
#ifndef SEGACD
u8 hIntPatchNeeded = 1;
#else
u8 hIntPatchNeeded = 0;  // Don't do it when booting from Sega CD
#endif

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

// from ChillyWilly Example of Mode1 playback
/*
 * Check for CD BIOS
 * When a cart is inserted in the MD, the CD hardware is mapped to
 * 0x400000 instead of 0x000000. So the BIOS ROM is at 0x400000, the
 * Program RAM bank is at 0x420000, and the Word RAM is at 0x600000.
 */
 
int DetectSCDBIOS(uint32_t address)
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
    return 1;
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

	CRC32_reset();

	bios = (void*)startAddress;
	for (address = 0; address < size; address ++)
	{
		uint8_t data;
		
		data = bios[address];

#ifndef SEGACD		
		// The BIOS is overlapped with a shadow of the HINT register by 
		// the ASIC in cart mode, and that alters the CRC
		if(hIntPatchNeeded && patch == 1)
		{
			if(address == 0x72)
				data = 0xFD;
			if(address == 0x73)
				data = 0x0C;
		}
		// Byteswapped ?
		if(patch == 2)
		{
			if(address == 0x73)
				data = 0xFD;
			if(address == 0x72)
				data = 0x0C;
		}
#endif
		CRC32_update(data);
	}

	checksum = CRC32_finalize();
	return checksum;
}

void ShowMessageAndData(char *message, uint32_t address, int len, int palmsg, int xpos, int ypos)
{
	int			msglen = 0;
	char		buffer[40];
	
	intToHex(address, buffer, len);
	
	msglen = strlen(message);
	VDP_Start();
	VDP_drawTextBG(APLAN, message, TILE_ATTR(palmsg, 0, 0, 0), xpos, ypos);
	VDP_drawTextBG(APLAN, " 0x", TILE_ATTR(PAL0, 0, 0, 0), xpos+msglen, ypos);
	VDP_drawTextBG(APLAN, buffer, TILE_ATTR(PAL0, 0, 0, 0), xpos+msglen+3, ypos);
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
	int		i = 0, j = 0, data[] = { 16, 16, 17, 15, 16, 48, 14, -2, 16, -4, -4, -4, -4, -2, -2, -4, -4, -12, -40, 16, 0};
	

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

void SetSCDRegion(u8 value, u32 startaddress, u32 size)
{
	u8		*ram = NULL;
	u32		address = 0;
	
	ram = (u8*)startaddress;
	for(address = 0; address < size; address++)
		ram[address] = value;
}

int ReadSCDRegion(u8 value, u32 startaddress, u32 size)
{
	u8		*ram = NULL;
	u32		address = 0;
	
	ram = (u8*)startaddress;
	for(address = 0; address < size; address++)
	{
		if(ram[address] != value)
			return address;
	}
	
	return MEMORY_OK;
}

int CheckSCDRegion(u8 value, u32 startaddress, u32 size)
{
	SetSCDRegion(value, startaddress, size);
	
	return(ReadSCDRegion(value, startaddress, size));
}

int CheckSCDRAMWithValue(char * message, u32 start, u32 end, u8 value, int pos)
{
	int memoryFail = 0;
	
	VDP_Start();
	VDP_drawTextBG(APLAN, message, TILE_ATTR(PAL1, 0, 0, 0), 12, pos);
	VDP_End();
	
	memoryFail = CheckSCDRegion(value, start, end - start);
	
	if(memoryFail != MEMORY_OK)
	{
		ShowMessageAndData("FAILED", memoryFail, 6, PAL3, 12, pos+1);
		return 0;
	}
	
	VDP_Start();
	VDP_drawTextBG(APLAN, "ALL OK", TILE_ATTR(PAL2, 0, 0, 0), 16, pos+1);
	VDP_End();
	return 1;
}

void WaitKey()
{
	u16 buttons, oldButtons = 0xffff, pressedButtons, close = 0;
	
	VDP_Start();
	VDP_drawTextBG(APLAN, "PRESS A", TILE_ATTR(PAL1, 0, 0, 0), 16, 22);
	VDP_End();

	while(!close)
	{
		buttons = JOY_readJoypad(JOY_1);
		pressedButtons = buttons & ~oldButtons;
		oldButtons = buttons;

		if(pressedButtons & BUTTON_A)
			close = 1;
		VDP_waitVSync();
	}
}


/*************** Z80 *******/


void Z80RamTest()
{
	DrawMainBG();
	
	ShowMessageAndData("Z80 RAM", 0xA00000, 15, PAL1, 7, 7);
	if(doZ80Lock)
		Z80_requestBus(1);			
	CheckSCDRAMWithValue("Setting to 0x00", 0xA00000, 0xA01FFF, 0x00, 10);
	CheckSCDRAMWithValue("Setting to 0xFF", 0xA00000, 0xA01FFF, 0xFF, 12);
	CheckSCDRAMWithValue("Setting to 0x55", 0xA00000, 0xA01FFF, 0x55, 14);
	CheckSCDRAMWithValue("Setting to 0xAA", 0xA00000, 0xA01FFF, 0xAA, 16);
	if(doZ80Lock)
		Z80_releaseBus();

	WaitKey();
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
{ 0xDD6CC972, "M2 JP 2.00C" },	// mpr-15398.bin swapped in MAME as 0x1E4344E6
{ 0x8052C7A0, "AIWA JP 2.11" },	// mpr-15768-t.bin
{ 0x50CD3D23, "LA 1.04" },		// laseractive_bios_1_04_u.bin
{ 0x3B10CF41, "LA 1.02" },		// laseractive_bios_1_02_u.bin
{ 0x474AAA44, "LA JP 1.05" },	// mega-ld 1.05 bios.bin
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

#ifndef SEGACD

typedef struct biosSwapped_data {
    uint32_t crcog;
    uint32_t crcsw;
} BIOS_SW;

// Reversed
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
{ 0x529AC15A, 0x2BF760AF }, // megacd_model1_bios_1_00_e.bin
{ 0xF18DDE5B, 0x03134289 }, // mpr-14088c.bin
{ 0x79F85384, 0x3773D5AA }, // mpr-14088h.bin
{ 0xC6D10268, 0x2461B5ED }, // mpr-15045b.bin
{ 0x4BE18FF6, 0xE0A6179B }, // 920501 Nyqvist dump, not documented
{ 0xDD6CC972, 0x1E4344E6 }, // mpr-15398.bin
{ 0x0507B590, 0xCB76F114 }, // mpr-15512.bin
{ 0x4D5CB8DA, 0x53F1757C }, // mpr-15512a.bin
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

#endif

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
	int i = 0;
	
	while(blist[i].crcsw != 0)
	{		
		if(checksum == blist[i].crcsw)
		{
			char *name = NULL;
			
			name = GetBIOSNamebyCRC(blist[i].crcog);
			if(name)
			{
				ShowMessageAndData("CD BIOS CRC32:", checksum, 8, PAL1, 6, 19);
				
				VDP_Start();
				VDP_drawTextBG(APLAN, "Byte swapped", TILE_ATTR(PAL2, 0, 0, 0), 6, 20);
				VDP_drawTextBG(APLAN, name, TILE_ATTR(PAL2, 0, 0, 0), 19, 20);
				VDP_End();
			}
			return 1;
		}
		i++;
	}
	return 0;
}
#endif

void doBIOSID(uint32_t checksum, uint32_t address)
{
	char 		*name = NULL;
	
	name = GetBIOSNamebyCRC(checksum);
	if(name)
	{
		VDP_Start();
		VDP_drawTextBG(APLAN, name, TILE_ATTR(PAL2, 0, 0, 0), 14, 20);
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
		
		// Calculate CRC with byteswapped HINT register
		VDP_Start();
		VDP_drawTextBG(APLAN, " working...", TILE_ATTR(PAL0, 0, 0, 0), 20, 19);
		VDP_End();
		
		bschecksum = CalculateCRC(address, 0x20000, 2);
		
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
		
		ShowMessageAndData("CD BIOS CRC32:", checksum, 8, PAL1, 6, 19);
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
		VDP_drawTextBG(APLAN, "BIOS not recognized", TILE_ATTR(PAL1, 0, 0, 0), 8, 20);
		VDP_End();
	}
	return;
}

void CheckSegaCDBIOSCRC(uint32_t address)
{
	uint32_t	checksum = 0;
	
	ShowMessageAndData("Sega CD BIOS Data at", address, PAL1, 8, 6, 4);
	PrintBIOSInfo(address);
	
	VDP_Start();
	VDP_drawTextBG(APLAN, "Calculating, please wait", TILE_ATTR(PAL3, 0, 0, 0), 6, 19);
	VDP_End();

#ifndef SEGACD
	// If in cart mode, patch the BIOS due to HINT
	checksum = CalculateCRC(address, 0x20000, 1);
#else
	checksum = CalculateCRC(address, 0x20000, 0);
#endif

	ShowMessageAndData("CD BIOS CRC32:", checksum, 8, PAL1, 6, 19);
	
	doBIOSID(checksum, address);

	WaitKey();
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

int checkRegisterBYTE(u32 reg, u8 value, char * message, int pos)
{
	u8 ret = 0;
	
	VDP_Start();
	VDP_drawTextBG(APLAN, message, TILE_ATTR(PAL1, 0, 0, 0), 11, pos);
	VDP_End();
	
	SetSCDRegisterBYTE(reg, value);
	ret = GetSCDRegisterBYTE(reg);
	if(ret != value)
	{
		ShowMessageAndData("FAILED", ret, 2, PAL3, 14, pos+1);
		return 0;
	}
	
	VDP_Start();
	VDP_drawTextBG(APLAN, "All OK", TILE_ATTR(PAL2, 0, 0, 0), 16, pos+1);
	VDP_End();
	return 1;
}

void checkRegisterBYTEValues(char *title, u32 reg)
{
	int good = 0;
	
	ShowMessageAndData(title, reg, 8, PAL1, 7, 8);
	
	good += checkRegisterBYTE(reg, 0x55, "Setting to 0x55", 10);
	good += checkRegisterBYTE(reg, 0xAA, "Setting to 0xAA", 12);
	good += checkRegisterBYTE(reg, 0xFF, "Setting to 0xFF", 14);
	if(good) checkRegisterBYTE(reg, 0, "Setting to 0x00", 16);
	WaitKey();
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

int checkRegisterWORD(u32 reg, u16 value, char * message, int pos)
{
	u16 ret = 0;
	
	VDP_Start();
	VDP_drawTextBG(APLAN, message, TILE_ATTR(PAL1, 0, 0, 0), 11, pos);
	VDP_End();
	
	SetSCDRegisterWORD(reg, value);
	ret = GetSCDRegisterWORD(reg);
	if(ret != value)
	{
		ShowMessageAndData("FAILED", ret, 4, PAL3, 13, pos+1);
		return 0;
	}
	
	VDP_Start();
	VDP_drawTextBG(APLAN, "All OK", TILE_ATTR(PAL2, 0, 0, 0), 16, pos+1);
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
	
	good += checkRegisterWORD(reg, 0x5555, "Setting to 0x5555", 10);
	good += checkRegisterWORD(reg, 0xAAAA, "Setting to 0xAAAA", 12);
	good += checkRegisterWORD(reg, 0xFFFF, "Setting to 0xFFFF", 14);
	if(good) checkRegisterWORD(reg, 0, "Setting to 0x0000", 16);
	WaitKey();
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
	WaitKey();
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
		WaitKey();
	}
	else
	{
		VDP_Start();
		ShowMessageAndData("Bank Register OK", MMOD_REGISTER, 7, PAL2, 7, 14);
		VDP_End();
		WaitKey();
	}
	
	VDP_Start();
	VDP_clearTileMapRect(APLAN, 5, 10, 30, 28);
	VDP_End();
	
	if(banks == 4 && !TestBankingProgramRAMFast())
	{
		VDP_Start();
		VDP_drawTextBG(APLAN, "Fast Check Failed. The register", TILE_ATTR(PAL2, 0, 0, 0), 5, 10);
		VDP_drawTextBG(APLAN, "did R/W OK, but bank switch was", TILE_ATTR(PAL2, 0, 0, 0), 5, 11);
		VDP_drawTextBG(APLAN, "either not done or RAM is bad.", TILE_ATTR(PAL2, 0, 0, 0), 5, 12);
		VDP_drawTextBG(APLAN, "Try Memory Viewer at address", TILE_ATTR(PAL2, 0, 0, 0), 5, 14);
		VDP_drawTextBG(APLAN, "above and use C.", TILE_ATTR(PAL2, 0, 0, 0), 5, 15);
		VDP_End();
		WaitKey();
		return;
	}
	
	
	for(i = 0; i < banks; i++)
	{
		int good = 0;
		
		ShowMessageAndData("Bank Test", i, 2, PAL1, 13, 9);
		
		// Change Program RAM Bank
		SetSCDRegisterWORD(MMOD_REGISTER, i << 6);
		
		good = 0;
		
		good += CheckSCDRAMWithValue("Setting to 0xAA", 0x420000, 0x440000, 0xAA, 10);
		good += CheckSCDRAMWithValue("Setting to 0x55", 0x420000, 0x440000, 0x55, 12);
		good += CheckSCDRAMWithValue("Setting to 0xFF", 0x420000, 0x440000, 0xFF, 14);
		if(good == 3) CheckSCDRAMWithValue("Setting to 0x00", 0x420000, 0x440000, 0x00, 16);

		WaitKey();
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
	good += CheckSCDRAMWithValue("Setting to 0xAA", 0x600000, 0x640000, 0xAA, 10);	
	good += CheckSCDRAMWithValue("Setting to 0x55", 0x600000, 0x640000, 0x55, 12);
	good += CheckSCDRAMWithValue("Setting to 0xFF", 0x600000, 0x640000, 0xFF, 14);
	if(good == 3) CheckSCDRAMWithValue("Setting to 0x00", 0x600000, 0x640000, 0x00, 16);

	WaitKey();
}
#endif

#define	MAX_LOCATIONS	7

void MemViewer(uint32_t address)
{
	u8 			redraw = 1, docrc = 0, locpos = 1, bank = 0, i = 0;
	uint32_t	crc = 0, locations[MAX_LOCATIONS] = { 0, 0x020000, 0x200000, 0x400000, 0x420000, 0x600000, 0xFF0000 };
	u16 		buttons, oldButtons = 0xffff, pressedButtons, close = 0;

#ifndef SEGACD	
	TestBankingProgramRAMFast();
#endif

	// Select Bank 0
	SetSCDRegisterWORD(MMOD_REGISTER, 0);
	
	for(i = 0; i < MAX_LOCATIONS; i++)
	{
		if(locations[i] == address)
		{
			locpos = i;
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

			mem = (uint8_t*)address;
			
			if(docrc)
				crc = CalculateCRC(address, 0x1C0, 0);
			
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
			if(address >= 0x020000 && address <= 0x040000)
#else
			if(address >= 0x420000 && address <= 0x440000)
#endif
			{
				ShowMessageAndData("PB", bank, 2, PAL1, 33, 3);
				VDP_drawTextBG(APLAN, "C->bank", TILE_ATTR(PAL1, 0, 0, 0), 33, 4);
			}
			VDP_End();
			
			for(i = 0; i < 28; i++)
			{
				for(j = 0; j < 16; j++)
				{
					intToHex(mem[i*16+j], buffer, 2);
					VDP_Start();
					VDP_drawTextBG(APLAN, buffer, TILE_ATTR(PAL0, 0, 0, 0), j*2, i);
					VDP_End();
				}
			}
			
			redraw = 0;
		}

		buttons = JOY_readJoypad(JOY_1);
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
			if(address >= 0x020000 && address <= 0x040000)
#else
			if(address >= 0x420000 && address <= 0x440000)
#endif
			{
				if(++bank > 3)
					bank = 0;
				SetSCDRegisterWORD(MMOD_REGISTER, bank << 6);
				redraw = 1;
			}
		}
		
		if(pressedButtons & BUTTON_START)
		{
			u16 type = 0;
			fmenudata resmenudata[] = { {0, "Mem View"}, {1, "Help"}, {2, "exit"} };

			type = DrawFloatMenu(1, resmenudata, 3);
			if(type == 1)
				DrawHelp(HELP_MEMVIEW);
			if(type == 2)
				close = 1;
				
			oldButtons |= BUTTON_A;
			redraw = 1;
		}
		
		if(pressedButtons & BUTTON_LEFT)
		{
			if(address >= 0x1C0)
				address -= 0x1C0;
			else
				address = 0;
			
			if(address >= 0x800000 && address < 0xFF0000) // skip
				address = 0x7F0000;
				
			redraw = 1;
		}
		
		if(pressedButtons & BUTTON_RIGHT)
		{
			address += 0x1C0;
			
			if(address >= 0x800000 && address < 0xFF0000) // skip
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
			
			if(address >= 0x800000 && address < 0xFF0000) // skip
				address = 0x7F0000;
		
			redraw = 1;
		}
		
		if(pressedButtons & BUTTON_DOWN)
		{
			address += 0x10000;
			
			if(address >= 0x800000 && address < 0xFF0000) // skip
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
	u16 cursel = 1, pos, reload = 1, redraw = 1;
	u16 buttons, oldButtons = 0xffff, pressedButtons;
	u16 done = 0;
#ifdef SEGACD
	int maxsel = 6;
#else
	int	maxsel = 9;
#endif

#ifndef SEGACD
	if(!DetectSCDBIOS(0x400000))
	{
		fmenudata resmenudata[] = { {0, "SCD not detected"}, {1, "continue"}, {2, "return"} };
	
		if(DrawFloatMenu(2, resmenudata, 3) == 2)
			return;
	}
#endif
	while(!done)
	{
		if(reload)
		{
			DrawMainBGwithGillian(1, 216, 72);
			reload = 0;
			redraw = 1;
		}

		pos = 7;
		if(redraw)
		{
			VDP_Start();
			VDP_drawTextBG(APLAN, "Sega CD Tests", TILE_ATTR(PAL1, 0, 0, 0), 14, 4);
			VDP_drawTextBG(APLAN, "BIOS CRC and info", TILE_ATTR(cursel == 1 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
			pos++;
			VDP_drawTextBG(APLAN, "Check HINT Register", TILE_ATTR(cursel == 2 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
#ifndef SEGACD
			VDP_drawTextBG(APLAN, "Check Flag Register", TILE_ATTR(cursel == 3 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
			VDP_drawTextBG(APLAN, "Check Comm Registers", TILE_ATTR(cursel == 4 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
#else
			VDP_drawTextBG(APLAN, "Check Communication Registers", TILE_ATTR(cursel == 3 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
#endif
			pos++;
#ifndef SEGACD
			VDP_drawTextBG(APLAN, "Program RAM Check", TILE_ATTR(cursel == 5 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
			VDP_drawTextBG(APLAN, "Word RAM Check", TILE_ATTR(cursel == 6 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
			pos++;
#endif
			VDP_drawTextBG(APLAN, "Memory Viewer", TILE_ATTR(cursel == maxsel - 2 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
			pos++;
			VDP_drawTextBG(APLAN, "Help", TILE_ATTR(cursel == maxsel -1 ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
			VDP_drawTextBG(APLAN, "Back to Main Menu", TILE_ATTR(cursel == maxsel ? PAL1 : PAL0, 0, 0, 0), 5, pos++);
			VDP_End();
			redraw = 0;
		}
		
		buttons = JOY_readJoypad(JOY_1);
		pressedButtons = buttons & ~oldButtons;
		oldButtons = buttons;

		if(CheckHelpAndVO(&buttons, &pressedButtons, HELP_SEGACD))
			reload = 1;

		if(pressedButtons & BUTTON_DOWN)
		{
			cursel++;
			if(cursel > maxsel)
				cursel = 1;
			redraw = 1;
		}

		if(pressedButtons & BUTTON_UP)
		{
			cursel--;
			if(cursel < 1)
				cursel = maxsel;
			redraw = 1;
		}

		if(pressedButtons & BUTTON_B || 
			pressedButtons & BUTTON_START)
		{
			done = 1;
		}
			
		if(pressedButtons & BUTTON_A)
		{
			if(cursel < maxsel - 2)
			{
				FadeAndCleanUp();
				DrawMainBG();
			}
			
			switch (cursel)
			{
			case 1:
#ifdef SEGACD
				CheckSegaCDBIOSCRC(0);
#else
				CheckSegaCDBIOSCRC(0x400000);
#endif
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
				CleanUp();
				MemViewer(0x400000);
				break;
			case 8:
				DrawHelp(HELP_SEGACD);
				break;
			case 9:
				done = 1;
				break;
#else
			case 4:
				CleanUp();
				MemViewer(0);
				break;
			case 5:
				DrawHelp(HELP_SEGACD);
				break;
			case 6:
				done = 1;
				break;
#endif
			}

			FadeAndCleanUp();
			buttons = JOY_readJoypad(JOY_1);
			pressedButtons = buttons & ~oldButtons;
			oldButtons = buttons;

			reload = 1;
		}

		VDP_waitVSync();
	}

	return;
}





#include "segacdtests.h"
#include "main.h"
#include "res.h"

/*
CRC 32 based on work by Christopher Baker <https://christopherbaker.net>
*/


uint32_t _state = ~0L;
u8 hIntPatchNeeded = 0;

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
		// The BIOS is overwritten by the ASIC in cart mode
		// with the HINT value, and that alters the CRC
		if(hIntPatchNeeded && patch && address == 0x72)
			data = 0xFD;
		if(hIntPatchNeeded && patch && address == 0x73)
			data = 0x0C;
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

void ShowBIOSData(uint32_t address)
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

int CheckSCDRegion(u8 value, u32 startaddress, u32 size)
{
	u8		*ram = NULL;
	u32		address = 0;
	
	ram = (u8*)startaddress;
	for(address = 0; address < size; address++)
		ram[address] = value;
	
	for(address = 0; address < size; address++)
	{
		if(ram[address] != value)
			return address;
	}
	
	return MEMORY_OK;
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

/*************** experiments **********/

/*
void write_byte(void *dst, unsigned char val)
{
    asm("movea.l 4(%sp),%a0");
    asm("move.l  8(%sp),%d0");
    asm("move.b  %d0,(%a0)");
}

void write_word(void *dst, unsigned short val)
{
    asm("movea.l 4(%sp),%a0");
    asm("move.l  8(%sp),%d0");
    asm("move.w  %d0,(%a0)");
}

unsigned char read_byte(void *src)
{
	return(*((unsigned char*)src));
}

// from ChillyWilly Example of Mode1 playback
void InitSCD()
{
	// Reset the Gate Array - this specific sequence of writes is recognized by
	// the gate array as a reset sequence, clearing the entire internal state -
	// this is needed for the LaserActive
	 
	write_word((void*)0xA12002, 0xFF00);
	write_byte((void*)0xA12001, 0x03);
	write_byte((void*)0xA12001, 0x02);
	write_byte((void*)0xA12001, 0x00);

    
    // Reset the Sub-CPU, request the bus
    write_byte((void*)0xA12001, 0x02);
    while (!(read_byte((void*)0xA12001) & 2)) write_byte((void*)0xA12001, 0x02); // wait on bus acknowledge
	
    //write_word((void*)0xA12002, 0x00FA); // no write-protection, bank 0, 2M mode, Word RAM assigned to Sub-CPU
	
	//write_byte((void*)0xA12003, 0x00); // assign word RAM to main CPU
}

void InitSCD_asm()
{
	asm("move.w	#0xff00,0xA12002");
	asm("move.b	#0x03,0xA12001");
	asm("move.b	#0x02,0xA12001");
	asm("move.b	#0x00,0xA12001");
	
	asm("lea	0xA12001,%a0");
asm("WaitSubCPU:");
	asm("move.b	#0x02,(%a0)");
	asm("move.b	(%a0),%d0");
	asm("and.b	#0x02,%d0");
	asm("beq	WaitSubCPU");
}


void InitSCD_C()
{
	u8		*scd_reg1 = (void*)0xA12001;
	u16		*scd_reg2 = (void*)0xA12002;
	
	// Reset the Gate Array - this specific sequence of writes is recognized by
	// the gate array as a reset sequence, clearing the entire internal state -
	// this is needed for the LaserActive
	
	*scd_reg2 = 0xFF00;
	*scd_reg1 = 0x03;
	*scd_reg1 = 0x02;
	*scd_reg1 = 0x00;

     // Reset the Sub-CPU, request the bus
	 
    *scd_reg1 = 0x02;
    while (!((*scd_reg1) & 2)) *scd_reg1 = 0x2; // wait on bus acknowledge	
	*scd_reg1 = 0x03;
	
	// *scd_reg2 = 0x00FA; // no write-protection, bank 0, 2M mode, Word RAM assigned to Sub-CPU
}

int DetectMegaCD()
{
    u8 		a1,a2,a3;
	u8		*scd_reg1_b = (void*)0xA12001;
	u16		*scd_regE_w = (void*)0xA1200E;
	u8		*scd_regE_b = (void*)0xA1200E;
	u8		*scd_regF_b = (void*)0xA1200F;
	
    if (((*scd_reg1_b) & 0x20))
        return 0;

    *scd_regE_w = 0x2233;
    a1 = *scd_regE_b;

    *scd_regE_b = 0x44;
    a2 = *scd_regE_b;

    *scd_regF_b = 0x55;
    a3 = *scd_regE_b;

    if (a1 == 0x22 && a2 == 0x44 && a3 == 0x55)
        return 1;

    return 0;
}
*/

/*
int DetectMegaCD()
{
    u8 a1,a2,a3;
    if ((HWREGB(0xA10001) & 0x20))
        return 0;

    HWREG(0xA1200E) = 0x2233;
    a1 = HWREGB(0xA1200E);

    HWREGB(0xA1200E) = 0x44;
    a2 = HWREGB(0xA1200E);

    HWREGB(0xA1200F) = 0x55;
    a3 = HWREGB(0xA1200E);

    if (a1 == 0x22 && a2 == 0x44 && a3 == 0x55)
        return 1;

    //if (HWREG(0x400100) == 0x5345)
    //    return 1;

    return 0;
}*/

//HWREG writes/reads a word to the given register
//HWREGB writes/reads a byte

/*
	VDP_Start();
	VDP_drawTextBG(APLAN, "Z80 RAM", TILE_ATTR(PAL1, 0, 0, 0), 14, 8);
	VDP_End();
	
	Z80_requestBus(1);			
	CheckSCDRAMWithValue("Setting to 0x00", 0xA00000, 0xA01FFF, 0x00, 10);
	CheckSCDRAMWithValue("Setting to 0xFF", 0xA00000, 0xA01FFF, 0xFF, 12);
	CheckSCDRAMWithValue("Setting to 0x55", 0xA00000, 0xA01FFF, 0x55, 14);
	CheckSCDRAMWithValue("Setting to 0xAA", 0xA00000, 0xA01FFF, 0xAA, 16);
	Z80_releaseBus();

	WaitKey();
		
*/

/*************** end experiments **********/

void WaitKey()
{
	u16 buttons, oldButtons = 0xffff, pressedButtons, close = 0;
	
	VDP_Start();
	VDP_drawTextBG(APLAN, "PRESS A", TILE_ATTR(PAL1, 0, 0, 0), 14, 22);
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

typedef struct bios_data {
    uint32_t crc;
    char *name;
} BIOSID;

const static BIOSID bioslist[] = {
{ 0xC6D10268, "M1 US 1.10" },
{ 0x529ac15a, "M1 EU 1.00" },
{ 0x3773d5aa, "M1 JP 1.0S" },
{ 0x69ed6ccd, "M1 JP 1.0G" },
{ 0x03134289, "M1 JP 1.0L" },
{ 0xdfa95ee9, "M1 JP 1.0O" },
{ 0x9d2da8f2, "M1 JP 1.01" },
{ 0x550f30bb, "M1 AS 1.0" },
{ 0x2e49d72c, "M2 US 2.11X" },
{ 0x8af65f58, "M2 US 2.00" },
{ 0x9f6f6276, "M2 US 2.00W" },
{ 0x53f1757c, "M2 EU 2.00W" },
{ 0xcb76f114, "M2 EU 2.00" },
{ 0x1e4344e6, "M2 JP 2.00C" },
{ 0x8052c7a0, "AIWA JP 2.11" },
{ 0x50cd3d23, "Laseractive 1.04" },
{ 0x3b10cf41, "Laseractive 1.02" },
{ 0x474aaa44, "Laseractive JP 1.05" },
{ 0x00eedb3a, "Laseractive JP 1.02" },
{ 0x290f8e33, "X'eye US 2.00" },
{ 0xd21fe71d, "WM JP 1.00" },
{ 0x2b19972f, "WM 2.00" },
{ 0xd48c44b5, "CDX US 2.21X" },
{ 0xaacb851e, "MMega 2.21" }, 
{ 0x73784705, "M1 US 1.10 [RF]"},
{ 0, NULL } } ; 

void doBIOSID(uint32_t checksum, uint32_t address)
{
	int i = 0;
	
	while(bioslist[i].crc != 0)
	{		
		if(checksum == bioslist[i].crc)
		{
			VDP_Start();
			VDP_drawTextBG(APLAN, bioslist[i].name, TILE_ATTR(PAL2, 0, 0, 0), 6, 20);
			VDP_End();
			return;
		}
		i++;
	}
	
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
	ShowBIOSData(address);
	
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

void CheckHintRegister()
{
	if(configHint())
		ShowMessageAndData("HINT set OK", HINT_REGISTER, 8, PAL1, 9, 12);
	else
		ShowMessageAndData("HINT set FAILED", HINT_REGISTER, 8, PAL3, 7, 12);
	WaitKey();
}

#ifndef SEGACD

int TestBankingProgramRAM()
{
	int		i;
	char 	data[4] = { '2', '4', '0', 'p' };
	u16 	*scdctrl = (u16 *)MMOD_REGISTER;
	u8		*ram = (u8*)0x420000;
	
	for(i = 0; i < 4; i++)
	{
		// Change Program RAM Bank
		*scdctrl = i << 6;
		ram[0] = data[i];
	}
	
	*scdctrl = 0;
	for(i = 0; i < 4; i++)
	{
		// Change Program RAM Bank
		*scdctrl = i << 6;
		if(ram[0] != data[i])
			return 0;
	}
	return 1;
}

void CheckSCDProgramRAM()
{
	int		i, banks = 4;
	u16 	*scdctrl = (u16 *)MMOD_REGISTER;
	
	ShowMessageAndData("Program RAM", 0x420000, 12, PAL1, 7, 7);
	if(!TestBankingProgramRAM())
	{
		ShowMessageAndData("Bank Switch FAIL", MMOD_REGISTER, 8, PAL3, 5, 8);
		
		VDP_Start();
		VDP_drawTextBG(APLAN, "Only one bank will be checked", TILE_ATTR(PAL2, 0, 0, 0), 5, 11);
		VDP_End();
		
		banks = 1;
		WaitKey();
		VDP_clearTileMapRect(APLAN, 5, 10, 30, 28);
	}
	
	for(i = 0; i < banks; i++)
	{
		int good = 0;
		
		ShowMessageAndData("Bank", i << 6, 2, PAL1, 14, 9);
		
		// Change Program RAM Bank
		*scdctrl = i << 6;
		
		good = 0;
		
		good += CheckSCDRAMWithValue("Setting to 0xAA", 0x420000, 0x440000, 0xAA, 10);
		good += CheckSCDRAMWithValue("Setting to 0x55", 0x420000, 0x440000, 0x55, 12);
		good += CheckSCDRAMWithValue("Setting to 0xFF", 0x420000, 0x440000, 0xFF, 14);
		if(good == 3) CheckSCDRAMWithValue("Setting to 0x00", 0x420000, 0x440000, 0x00, 16);

		WaitKey();
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

#define	MAX_LOCATIONS	4

void MemViewer(uint32_t address)
{
	u8 			redraw = 1, docrc = 0, locpos = 1;
	uint32_t	crc = 0, locations[MAX_LOCATIONS] = { 0x0, 0x400000, 0x420000, 0x600000 };
	u16 		buttons, oldButtons = 0xffff, pressedButtons, close = 0;
	
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
		
		if(pressedButtons & BUTTON_START)
			close = 1;
		
		if(pressedButtons & BUTTON_LEFT)
		{
			if(address >= 0x1C0)
				address -= 0x1C0;
			
			if(address >= 0x800000 && address < 0xA00000) // skip
				address = 0x7F0000;
				
			redraw = 1;
		}
		
		if(pressedButtons & BUTTON_RIGHT)
		{
			address += 0x1C0;
			
			if(address >= 0x800000 && address < 0xA00000) // skip
				address = 0xA00000;
				
			redraw = 1;
		}
		
		if(pressedButtons & BUTTON_UP)
		{
			if(address >= 0x10000)
				address -= 0x10000;
			
			if(address >= 0x800000 && address < 0xA00000) // skip
				address = 0x7F0000;
		
			redraw = 1;
		}
		
		if(pressedButtons & BUTTON_DOWN)
		{
			address += 0x10000;
			
			if(address >= 0x800000 && address < 0xA00000) // skip
				address = 0xA00000;

			redraw = 1;
		}
		VDP_waitVSync();
	}
}

void SegaCDMenu()
{
	u16 cursel = 1, pos, reload = 1, redraw = 1;
	u16 buttons, oldButtons = 0xffff, pressedButtons;
	u16 ind = 0, size = 0, done = 0;
#ifdef SEGACD
	int maxsel = 5;
#else
	int	maxsel = 8;
#endif

	while(!done)
	{
		if(reload)
		{
			VDP_Start();
			VDP_setPalette(PAL0, palette_grey);
			VDP_setPalette(PAL1, palette_green);
			VDP_setPalette(PAL2, back_pal);
			VDP_setPalette(PAL3, palette_red);

			ind = TILE_USERINDEX;
			size = sizeof(back_tiles) / 32;
			VDP_loadTileData(back_tiles, ind, size, USE_DMA);

			VDP_setMyTileMapRect(BPLAN, back_map, TILE_USERINDEX, 0, 0, 320 / 8, 224 / 8);

			VDP_End();
			reload = 0;
			redraw = 1;
		}

		pos = 8;
		if(redraw)
		{
			VDP_Start();
			VDP_drawTextBG(APLAN, "Sega CD Tests", TILE_ATTR(PAL1, 0, 0, 0), 14, 4);
			VDP_drawTextBG(APLAN, "BIOS CRC and info", TILE_ATTR(cursel == 1 ? PAL3 : PAL0, 0, 0, 0), 5, pos++);
			pos++;
			VDP_drawTextBG(APLAN, "Check HINT Register", TILE_ATTR(cursel == 2 ? PAL3 : PAL0, 0, 0, 0), 5, pos++);
#ifndef SEGACD
			VDP_drawTextBG(APLAN, "Check Flag Register", TILE_ATTR(cursel == 3 ? PAL3 : PAL0, 0, 0, 0), 5, pos++);
			VDP_drawTextBG(APLAN, "Check Communication Registers", TILE_ATTR(cursel == 4 ? PAL3 : PAL0, 0, 0, 0), 5, pos++);
#else
			VDP_drawTextBG(APLAN, "Check Communication Registers", TILE_ATTR(cursel == 3 ? PAL3 : PAL0, 0, 0, 0), 5, pos++);
#endif
			pos++;
#ifndef SEGACD
			VDP_drawTextBG(APLAN, "Program RAM Check", TILE_ATTR(cursel == 5 ? PAL3 : PAL0, 0, 0, 0), 5, pos++);
			VDP_drawTextBG(APLAN, "Word RAM Check", TILE_ATTR(cursel == 6 ? PAL3 : PAL0, 0, 0, 0), 5, pos++);
			pos++;
#endif
			VDP_drawTextBG(APLAN, "Memory Viewer", TILE_ATTR(cursel == maxsel - 1 ? PAL3 : PAL0, 0, 0, 0), 5, pos++);
			pos++;
			VDP_drawTextBG(APLAN, "Back to Main Menu", TILE_ATTR(cursel == maxsel ? PAL3 : PAL0, 0, 0, 0), 5, ++pos);
			VDP_End();
			redraw = 0;
		}
		
		buttons = JOY_readJoypad(JOY_1);
		pressedButtons = buttons & ~oldButtons;
		oldButtons = buttons;

		//if(CheckHelpAndVO(&buttons, &pressedButtons, HELP_SEGACD))
			//reload = 1;

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
			VDP_Start();
			VDP_clearTileMapRect(APLAN, 0, 0, 320 / 8, 224 / 8);
			VDP_End();
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
				FadeAndCleanUp();
				MemViewer(0x400000);
				break;
			case 8:
				done = 1;
				break;
#else
			case 4:
				FadeAndCleanUp();
				MemViewer(0);
				break;
			case 5:
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



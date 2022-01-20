/*
 * Copyright (C)2019-2021 Artemio Urbina
 *
 * This file is part of the 240p Test Suite
 * It is intended to identify all Sega CD BIOS variants
 *
 * You can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA	02111-1307	USA
 *
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

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


uint32_t CalculateCRC(uint8_t *bios, uint32_t size)
{
	uint32_t address = 0, checksum = 0;

	CRC32_reset();

	for (address = 0; address < size; address ++)
	{
		uint8_t data;
		
		data = bios[address];

		CRC32_update(data);
	}

	checksum = CRC32_finalize();
	return checksum;
}

int ByteSwap(uint8_t *bios)
{
	int pos = 0;

	while(pos < 0x20000)	
	{
		uint8_t t;

		t = bios[pos];
		bios[pos] = bios[pos+1];
		bios[pos+1] = t;
		pos += 2;
	}
}

int LoadBIOS(char *name, uint8_t *bios)
{
	FILE 	*fin = NULL;
	int		read = 0;

	memset(bios, 0, 0x20000*sizeof(uint8_t));
	fin = fopen(name, "rb");
	if(!fin)
	{
		printf("Could not open BIOS file\n");
		return 0;
	}
	read = fread(bios, sizeof(uint8_t), 0x20000, fin);
	if(fgetc(fin) != EOF)
		read = 0;
	fclose(fin);

	if(read != 0x20000)
	{
		printf("File too small, read 0x%0.8X bytes\n", read);
		return 0;
	}
	return 1;
}

int CreateFolder(char *name)
{
#if defined (WIN32)
	if(_mkdir(name) != 0)
	{
		if(errno != EEXIST)
			return 0;
	}
#else
	if(mkdir(name, 0755) != 0)
	{
		if(errno != EEXIST)
			return 0;
	}
#endif
	return 1;
}

int SaveBIOS(char *folder, uint8_t *bios, char *name)
{
	FILE 	*fout = NULL;
	int		wrote = 0;
	char	target[500];

	if(!CreateFolder(folder))
	{
		printf("Could not create folder \"%s\"\n", folder);
		return 0;
	}
	sprintf(target, "%s\\%s", folder, name);
	fout = fopen(target, "wb");
	if(!fout)
	{
		printf("Could not open BIOS file \"%s\\%s\"\n", folder, target);
		return 0;
	}
	wrote = fwrite(bios, sizeof(uint8_t), 0x20000, fout);
	fclose(fout);

	if(wrote != 0x20000)
	{
		printf("Could not write whole BIOS file, wrote 0x%0.8X \"%s\\%s\"\n", 
				wrote, folder, target);
		return 0;
	}
	return 1;
}

int DetectSCDBIOS(uint8_t *bios)
{
	uint8_t	*pos = NULL;

	pos = bios + 0x015800;
	if (memcmp(pos + 0x6D, "SEGA", 4))
	{
		pos = bios + 0x016000;
		if (memcmp(pos + 0x6D, "SEGA", 4))
		{
			// check for WonderMega/X'Eye
			if (memcmp(pos + 0x6D, "WONDER", 6))
			{
				pos = bios + 0x01AD00; // might also be 0x40D500
				// check for LaserActive
				if (memcmp(pos + 0x6D, "SEGA", 4))
					return 0;
			}
		}
	}
	return 1;
}

typedef struct bios_checksum_st {
	uint32_t	crc;
	char		*name;
} bioscrc;

int IsPresent(uint32_t	crc, bioscrc *list, int count)
{
	int i;

	for(i = 0; i < count; i++)
	{
		if(crc == list[i].crc)
			return i;
	}
	return -1;
}

int insertbios(uint32_t	crc, char *name, bioscrc *list, int *count)
{
	if(count && list && *count + 1 < 500)
	{
		list[*count].crc = crc;
		list[*count].name = name;
		(*count)++;
		return 1;
	}
	return 0;
}

int DetectBadDump(uint8_t *bios)
{
	if(bios[0x72] != 0xFD)
		return 1;
	if(bios[0x73] != 0x0C)
		return 1;
	return 0;
}

void PatchDumpForError(uint8_t *bios)
{
	bios[0x72] = 0xFF;
	bios[0x73] = 0xFF;
}

void PatchDump(uint8_t *bios)
{
	bios[0x72] = 0xFD;
	bios[0x73] = 0x0C;
}

typedef struct bios_data {
    uint32_t crc;
    char *name;
	char *mame;
} BIOSID;

const static BIOSID biosnames[] = {
{ 0xC6D10268, "M1 US 1.10", "mpr-15045b.bin" },	// 
{ 0x529AC15A, "M1 EU 1.00", "megacd_model1_bios_1_00_e.bin" },	// 
{ 0x79F85384, "M1 JP 1.0S", "mpr-14088h.bin" },   		//  swapped in MAME as 0x3773D5AA
{ 0x9BCE40B2, "M1 JP 1.0G", "epr-14088b.bin" },   		//  swapped in MAME as 0x69ED6CCD
{ 0xF18DDE5B, "M1 JP 1.0L", "mpr-14088c.bin" },   		//  swapped in MAME as 0x03134289
{ 0x2EA250C0, "M1 JP 1.0O", "epr-14088d.bin" },   		//  swapped in MAME as 0xDFA95EE9
{ 0x9D2DA8F2, "M1 JP 1.01", "epr-14088e.bin" },	// 
{ 0x4BE18FF6, "M1 JP 1.11", "mpr-14837.bin" },	// mpr-14837.bin Nyqvist dump, previously noted as SCD2, that's incorrect
{ 0x550F30BB, "M1 AS 1.0", "epr-14536h.bin" },	// 
{ 0x2E49D72C, "M2 US 2.11X", "mpr-15764-t.bin" },	// 
{ 0x8AF65F58, "M2 US 2.00", "us_scd2_930314.bin" },	// 
{ 0x9F6F6276, "M2 US 2.00W", "segacd_model2_bios_2_00w_u.bin" },	// 
{ 0x4D5CB8DA, "M2 EU 2.00W", "mpr-15512a.bin" },  		//  swapped in MAME as 0x53F1757C
{ 0x0507B590, "M2 EU 2.00", "mpr-15512.bin" },    		//  swapped in MAME as 0xCB76F114
{ 0xC1AA217F, "M2 EU 2.11X", "mpr-15811-t.bin" },   // dumped by TwistedTom,  MK-4102A-50 Sony pcb
{ 0xDD6CC972, "M2 JP 2.00C", "mpr-15398.bin" },      	//  swapped in MAME as 0x1E4344E6
{ 0x8052C7A0, "AIWA JP 2.11", "mpr-15768-t.bin" },     	// 
{ 0x50CD3D23, "LA 1.04", "laseractive_bios_1_04_u.bin" }, // 
{ 0x3B10CF41, "LA 1.02", "laseractive_bios_1_02_u.bin" }, // 
{ 0x474AAA44, "LA JP 1.05", "mega-ld 1.05 bios.bin" },  // 
{ 0x1493522C, "LA JP 1.05", "mega-ld 1.05 bios alt.bin" },  // PD6126E dump by zaxour 93.833923% identical to above
{ 0x00EEDB3A, "LA JP 1.02", "laseractive_bios_1_02_j.bin" },  // 
{ 0x290F8E33, "X'EYE US 2.00", "g304.bin" },    // 
{ 0xD21FE71D, "WM JP 1.00", "g301.bin" },   // 
{ 0x2B19972F, "WM 2.00", "wondermega_m2_bios_2_00_j.bin" },      // 
{ 0xD48C44B5, "CDX US 2.21X", "segacdx_bios_2_21_u.bin" },	// 
{ 0xAACB851E, "MMEGA 2.21", "opr-16140.bin" },   // 
{ 0, NULL } } ; 

const static BIOSID biosnamesRF[] = {
// V1
{ 0xA94CFE69, "", "us_scd1_921011_regionfree_v1.bin" },  // mpr-15045b.bin 0xC6D10268
{ 0xB03B4FF7, "", "eu_mcd1_921027_regionfree_v1.bin" },  // megacd_model1_bios_1_00_e.bin 0x529AC15A
{ 0x193CB234, "", "jp_mcd1_911228_region_free_v1.bin" },  // epr-14536h.bin 0x550F30BB
{ 0x17AF77AC, "", "us_scd2_930314_regionfree_v1.bin" },  // us_scd2_930314.bin 0x8AF65F58
{ 0x3B0FB51B, "", "JP_MegaCD2_22121992_regionfree_v1.bin" }, // mpr-15398.bin 0xDD6CC97
{ 0x2F56F336, "", "EU_MegaCD2_30031993_regionfree_v1.bin" },  // mpr-15512.bin 0x0507B590
{ 0x848685FA, "", "US_X'EYE_27121993_regionfree_v1.bin" },  // g304.bin 0x290F8E33
{ 0xD2BD281F, "", "JP_Wondermega_02061992_regionfree_v1.bin" },  // g301.bin 0xD21FE71D
{ 0x5B3A5F22, "", "US_SEGA_CDX_930907_regionfree_v1.bin" },  // segacdx_bios_2_21_u.bin 0xD48C44B5
// V2
{ 0x73784705, "", "us_scd1_921011_regfree.bin" },  // mpr-15045b.bin 0xC6D10268
{ 0xC82EE650, "", "eu_mcd1_921027_regfree.bin" },  // megacd_model1_bios_1_00_e.bin 0x529AC15A
{ 0x8E6943FA, "", "jp_mcd1_911228_regfree.bin" },  // mpr-14088h.bin 0x79F85384
{ 0x1AEE2672, "", "jp_mcd1_911217_regfree.bin" },  // epr-14088e.bin 0x9D2DA8F2
{ 0x29776266, "", "jp_mcd1_911228_pal_regfree.bin" },  // epr-14536h.bin 0x550F30BB
{ 0x6442BA81, "", "us_scd2_930621_regfree.bin" },  // mpr-15764-t.bin 0x2E49D72C
{ 0x95118134, "", "us_scd2_930314_regfree.bin" },  // us_scd2_930314.bin 0x8AF65F58
{ 0x7E86C5DA, "", "us_scd2_930601_regfree.bin" },  // segacd_model2_bios_2_00w_u.bin 0x9F6F6276
{ 0x056663AD, "", "eu_mcd2_930601_regfree.bin" },  // mpr-15512a.bin 0x4D5CB8DA
{ 0x316F3D80, "", "eu_mcd2_930330_regfree.bin" },  // mpr-15512.bin 0x0507B590
{ 0x9E140799, "", "jp_mcd2_921222_regfree.bin" },  // mpr-15398.bin 0xDD6CC972
{ 0x8F34F0EE, "", "MPR-15768-T_regfree.bin" },  // mpr-15768-t.bin 0x8052C7A0
{ 0xB72F16D7, "", "us_xeye_931227_regfree.bin" },  // g304.bin 0x290F8E33
{ 0xD650FD93, "", "jp_wmg_920206_regfree.bin" },  // g301.bin 0xD21FE71D
{ 0x3F5112F3, "", "WONDERMEGA-G303_regfree.BIN" },  // wondermega_m2_bios_2_00_j.bin 0x2B19972F
{ 0xC7EB4CC9, "", "us__cdx_930907_regfree.bin" },  // segacdx_bios_2_21_u.bin 0xD48C44B5
{ 0xC6AD0AD7, "", "eu_mmg_930916_regfree.bin" },  // opr-16140.bin 0xAACB851E
{ 0, NULL } } ; 

void PrintBIOSInfo(uint8_t *bios)
{
	char	buffer[50];
	int		i = 0, data[] = { 16, 16, 17, 15, 16, 48, 14, -2, 16, -4, -4, -4, -4, -2, -2, -4, -4, -12, -40, 16, 0};

	bios += 0x0100;

	while(data[i] != 0)
	{
		if(data[i] > 0)
		{
			memcpy(buffer, bios, sizeof(uint8_t)*data[i]);
			buffer[data[i]] = '\0';
			printf("%s", buffer);
		}
		bios+=abs(data[i]);
		i++;
	}
}

char * doBIOSID(uint32_t checksum)
{
	int i = 0;
	
	while(biosnames[i].crc != 0)
	{		
		if(checksum == biosnames[i].crc)
			return biosnames[i].mame;
		i++;
	}
	return NULL;
}

char * doBIOSID_RF(uint32_t checksum)
{
	int i = 0;
	
	while(biosnamesRF[i].crc != 0)
	{		
		if(checksum == biosnamesRF[i].crc)
			return biosnamesRF[i].mame;
		i++;
	}
	return NULL;
}

void GetBIOSString(uint8_t *bios, char *string)
{
	memcpy(string, bios+0x100, 0xA0*sizeof(uint8_t));
	string[0xA0] = '\0';
}

int main(int argc, char *argv[])
{
	bioscrc bioslist[500];
	int count = 0;
	
	printf("CRC32 Sega CD BIOS checker by Artemio Urbina 2019-2022\n\n");
	if(argc == 1)
	{
		printf("Use: CRC32 file1 file2 ... filen\n");
		return 0;
	}
	while(--argc > 0)
	{
		int detected = 0;
		uint8_t bios[0x20000];

		if(LoadBIOS(*++argv, bios))
		{
			int pos = 0;
			uint32_t 	crc;
			char *name, biosstr[0xA0];

			crc = CalculateCRC(bios, 0x20000);
			detected = DetectSCDBIOS(bios);
			if(detected)
			{
				printf("%s\n  CRC32: 0x%0.8X\n", *argv, crc);


				uint32_t reversed;

				ByteSwap(bios);
				reversed = CalculateCRC(bios, 0x20000);
				printf("  SWAPPED CRC32: 0x%0.8X\n", reversed);
				ByteSwap(bios);

			}
			else
			{
				ByteSwap(bios);
				detected = DetectSCDBIOS(bios);
				if(detected)
				{
					uint32_t badcrc;
	
					badcrc = crc;
					crc = CalculateCRC(bios, 0x20000);
					printf("%s\n  BYTE SWAPPED CRC32: 0x%0.8X\n  CRC32: 0x%0.8X\n", *argv, badcrc, crc);
				}
			}

			if(detected)
			{
				if(DetectBadDump(bios))
				{
					PatchDump(bios);
		
					crc = CalculateCRC(bios, 0x20000);
					printf("  BAD DUMP, fixed CRC32: 0x%0.8X\n", crc);
				}
				else
				{
					uint8_t bioscopy[0x20000];
					uint32_t badcrc, badReversed;
	
					memcpy(bioscopy, bios, 0x20000*sizeof(uint8_t));

					PatchDumpForError(bioscopy);
					badcrc = CalculateCRC(bioscopy, 0x20000);
					ByteSwap(bioscopy);
					badReversed = CalculateCRC(bioscopy, 0x20000);
	
					printf("  This Dump with HINT error would be CRC32: 0x%0.8X\n", badcrc);
					printf("  This Dump with HINT error and BYTE SWAPPED would be CRC32: 0x%0.8X\n", badReversed);
				}
				name = doBIOSID(crc);
				if(name)
				{
					printf("  Matched with MAMEID \"%s\"\n", name);
					if(SaveBIOS("BIOS", bios, name))
						remove(*argv);
				}
				else
				{
					name = doBIOSID_RF(crc);
					if(name)
					{
						printf("  Matched with REGIONFREEID \"%s\"\n", name);
						if(SaveBIOS("REGIONFREE", bios, name))
							remove(*argv);
					}
				}

				GetBIOSString(bios, biosstr);
				printf("  \"%s\"\n", biosstr);
	
				pos = IsPresent(crc, bioslist, count);
				if(pos == -1)
				{
					insertbios(crc, name ? name : *argv, bioslist, &count);
					if(!name)
					{
						if(SaveBIOS("NOMATCH-BIOS", bios, *argv))
							remove(*argv);
					}
				}
				else
				{
					printf("  DUPLICATE from \"%s\"\n", bioslist[pos].name);
					remove(*argv);
				}
			}
			else
				printf("== Unmatched and Unrecognized file \"%s\"\n", *argv);
		}
		else
			printf("Not a BIOS %s\n", *argv);
	}
	printf("\nTotal valid BIOS files %d\n", count);
	return(0);
}

/*
 * Copyright (C)2019-2022 Artemio Urbina
 *
 * This file is part of the 240p Test Suite
 * It is intended to identify Dreamcast BIOS variants
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

	while(pos < 0x200000)	
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

	memset(bios, 0, 0x200000*sizeof(uint8_t));
	fin = fopen(name, "rb");
	if(!fin)
	{
		printf("Could not open BIOS file\n");
		return 0;
	}
	read = fread(bios, sizeof(uint8_t), 0x200000, fin);
	if(fgetc(fin) != EOF)
		read = 0;
	fclose(fin);

	if(read != 0x200000)
	{
		printf("File \"%s\" is not 0x200000 bytes, skipping\n", name);
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

int DetectDreamcastBIOS(uint8_t *bios)
{
	uint8_t	*pos = NULL;

	pos = bios + 0x7b0;
	if (memcmp(pos, "SEGA SEGAKATANA", 15))
	{
		pos = bios + 0x7e0;
		if (memcmp(pos, "Written by eMuKei @ SEGAsysRD", 29))  // beta
			return 0;
	}
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
	wrote = fwrite(bios, sizeof(uint8_t), 0x200000, fout);
	fclose(fout);

	if(wrote != 0x200000)
	{
		printf("Could not write whole BIOS file, wrote 0x%0.8X \"%s\\%s\"\n", 
				wrote, folder, target);
		return 0;
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

typedef struct bios_data {
	int			type;
    uint32_t	crc;
    char		*name;
	char		*text;
} BIOSID;

#define	BIOS_SEGA	1
#define	BIOS_HACK	2

BIOSID biosnames[] = {
{ 	BIOS_SEGA, 
	0x5454841f, 
	"mpr-21068.ic501",
	"v1.004 (Japan)" },	// oldest known mass production version, supports Japan region only
{ 	BIOS_SEGA,
	0x2f551bc5, 
	"mpr-21871.ic501", 
	"v1.01c (World)" },
{ 	BIOS_SEGA, 
	0x89f2b1a1,
	"mpr-21931.ic501",
	"v1.01d (World)" },
{ 	BIOS_SEGA, 
	0x786168f9,
	"mpr-23588.ic501",
	"v1.022 (World)" },
{ 	BIOS_SEGA, 
	0xdcb2e86f, 
	"set5v0.976.ic507",
	"Katana Set5 v0.976 (Japan)" },
{ 	BIOS_SEGA,
	0x5702d38f,
	"set5v1.001.ic507",
	"Katana Set5 v1.001 (Japan)" },
{ 	BIOS_SEGA,
	0x2186e0e5,
	"set5v1.011.ic507",
	"Katana Set5 v1.011 (World)" },
{ 	BIOS_SEGA,
	0x52d01969,
	"set5v0.71.bin",
	"Katana Set5 Checker v0.71" },
{ 	BIOS_SEGA,
	0x485877bd,
	"set5v0.41.bin",
	"Katana Set5 Checker v0.41" },
// Dreamshell
{ 	BIOS_HACK,
	0xD1523DC3,
	"boot_loader_devkit.bios",
	"Dreamshell Devkit" },
{ 	BIOS_HACK,
	0xB3EA8EDF,
	"boot_loader_devkit_nogdrom.bios",
	"Dreamshell No GDROM" },
{ 	BIOS_HACK,
	0xDD1CF514,
	"boot_loader_retail.bios",
	"Dreamshell Retail" },
{ 	BIOS_HACK,
	0xBFA44608,
	"boot_loader_retail_nogdrom.bios",
	"Dreamshell Retail No GDROM" },
{ 	BIOS_HACK,
	0xA18BD6EA,
	"sd_loader_with.bios",
	"Dreamshell SD Loader" },
// Region Free
{ 	BIOS_HACK,
	0x65DE7BCB,
	"Link83_bios_1.0.bios",
	"v1.00 Link83 Region Free" },
{ 	BIOS_HACK,
	0x558F456E,
	"dc_bios_regionfree.bin",
	"v1.01d Region Free" }, 
{ 	BIOS_HACK,
	0xA65736C2,
	"dreamboot.bios",
	"DreamBoot v1.1.0" },
{ 	BIOS_HACK,
	0xEC7B10A1,
	"dreamboot-nogdrom.bios",
	"DreamBoot No GDROM v1.1.0" }, 
{ 	BIOS_HACK,
	0x8A0388E1,
	"dreamboot_1.0.2.bios",
	"DreamBoot v1.0.2" },
{ 	BIOS_HACK,
	0x6B8D1C5F,
	"dreamboot-nogdrom_1.0.2.bios",
	"DreamBoot No GDROM v1.0.2" }, 
{ 	BIOS_HACK,
	0x4DC1A805,
	"dreamboot_1.0.1.bios",
	"DreamBoot v1.0.1" },
{ 	BIOS_HACK,
	0x7FD6AB51,
	"dreamboot-nogdrom_1.0.1.bios",
	"DreamBoot No GDROM v1.0.1" }, 
{ 	BIOS_HACK,
	0x2991537A,
	"dreamboot_1.0.0.bios",
	"DreamBoot v1.0.0" },
{ 	BIOS_HACK,
	0x2E3FD8A9,
	"dreamboot-nogdrom_1.0.0.bios",
	"DreamBoot No GDROM v1.0.0" }, 
{ 	BIOS_HACK,
	0xDC9D84E0,
	"jc-bootROM-devkit-v1.031.bin",
	"JP Cake bootROM v1.031 devkit" },
{ 	BIOS_HACK,
	0x1249EE59,
	"jc-bootROM-retail-v1.031.bin",
	"JP Cake bootROM v1.031 retail" },
{ 	BIOS_HACK,
	0xA06F68D0,
	"jc-bootROM-devkit-v1.032.bin",
	"JP Cake bootROM v1.032 devkit" },
{ 	BIOS_HACK,
	0xF3DB5F40,
	"jc-bootROM-retail-v1.032.bin",
	"JP Cake bootROM v1.032 retail" },
// Other Hacks
{ 	BIOS_HACK,
	0x4B35A953,
	"mpr-21931-hack.ic501",
	"v1.01d (World/hack)" },
{ 	BIOS_HACK,
	0xA2564FAD,
	"dc101d_ch.bin",
	"v1.01d (Chinese hack)" },
{ 	BIOS_HACK,
	0x544C8685,
	"1_011_01_hacked.bios",
	"v1.011 Hack" },
{ 	0, 0, NULL, NULL } }; 


BIOSID *doBIOSID(uint32_t checksum)
{
	int i = 0;
	
	while(biosnames[i].crc != 0)
	{		
		if(checksum == biosnames[i].crc)
			return &biosnames[i];
		i++;
	}
	return NULL;
}

void GetBIOSString(uint8_t *bios, char *string)
{
	memcpy(string, bios+0x7b0, 0x50*sizeof(uint8_t));
	string[0x50] = '\0';
}

int main(int argc, char *argv[])
{
	bioscrc bioslist[500];
	uint8_t *bios = NULL;
	uint8_t *bioscopy = NULL;
	int count = 0;
	
	printf("CRC32 Dreamcast BIOS checker by Artemio Urbina 2022\n\n");
	if(argc == 1)
	{
		printf("Use: CRC32 file1 file2 ... filen\n");
		return 0;
	}
	
	bios = (uint8_t*)malloc(sizeof(uint8_t)*0x200000);
	if(!bios)
		return 1;
	bioscopy = (uint8_t*)malloc(sizeof(uint8_t)*0x200000);
	if(!bioscopy)
	{
		free(bios);
		return 1;
	}
	
	while(--argc > 0)
	{
		int detected = 0;

		if(LoadBIOS(*(++argv), bios))
		{
			int pos = 0;
			uint32_t 	crc;
			char biosstr[0xA0];

			if(DetectDreamcastBIOS(bios))
			{
				BIOSID *id = NULL;
				crc = CalculateCRC(bios, 0x200000);
				printf("  Searching for \"%s\" CRC 0x%X\n", *argv, crc);
				GetBIOSString(bios, biosstr);
				printf("  \"%s\"\n", biosstr);
				
				id = doBIOSID(crc);
				if(id)
				{
					printf("  Matched with %s \"%s\"\n", 
						id->type == BIOS_SEGA ? "BIOS" : "Custom BIOS",
						id->name);
					if(SaveBIOS("BIOS", bios, id->name))
						remove(*argv);
				}
				else
					printf("=== UnMatched BIOS ===\n");

		
				pos = IsPresent(crc, bioslist, count);
				if(pos == -1)
				{
					insertbios(crc, id ? id->name : *argv, bioslist, &count);
					if(!id)
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
				printf("\n");
			}
			else
				printf("== Unmatched and Unrecognized file \"%s\"\n", *argv);
		}
		else
			printf("Not a BIOS %s\n", *argv);
	}
	printf("\nTotal valid BIOS files %d\n", count);
	free(bios);
	free(bioscopy);
	return(0);
}

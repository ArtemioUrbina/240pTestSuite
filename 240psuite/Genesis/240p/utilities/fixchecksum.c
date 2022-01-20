/* 
 * 240p Test Suite
 * Copyright (C)2022 Artemio Urbina
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	FILE *fp = NULL;
	unsigned long int srcSize, checksum = 0;
	int fail = 0, i = 0, fixsize = 0;
	unsigned int value = 0, romsize = 0;
	unsigned char *src = NULL;
	
	if(argc != 2)
	{
		printf("Usage %s <source>\n", argv[0]); 
		printf("Fixes Sega Genesis ROM header checksum and size, Artemio Urbina 2022\n"); 
		return 1;
	}
	
	fp = fopen(argv[1], "rb");
	if(!fp)
	{
		printf("Could not read from file %s\n", argv[1]);
		return 1;
	}
	
	fseek(fp, 0L, SEEK_END);
	srcSize = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	if(srcSize < 0x200)
	{
		printf("ROM file %s is too small\n", argv[1]);
		return 1;
	}

	src = (char*)malloc (sizeof(char)*srcSize);
	if (!src) 
	{
		fclose(fp);
		printf("Out of memory\n");
		return -1;
	}
	
	if(fread (src, sizeof(char), srcSize, fp) != srcSize)
	{
		fclose(fp);
		printf("Error loading file\n");
		return -1;
	}

	fclose(fp);
	fp = NULL;

	romsize = src[0x1A4] << 24 | src[0x1A5] << 16 | src[0x1A6] << 8 | src[0x1A7];
	
	if(romsize != srcSize-1)
	{
		printf("Fixing size in header from 0x%08X to 0x%08X\n", romsize, srcSize-1);
		romsize = srcSize-1;
		src[0x1A4] = romsize >> 24 & 0xFF;
		src[0x1A5] = romsize >> 16 & 0xFF;
		src[0x1A6] = romsize >> 8 & 0xFF;
		src[0x1A7] = romsize & 0xFF;

		fixsize = 1;
	}
	// Calculate Checksum
	for(i = 0x200; i < srcSize; i+= 2)
	{
		value = src[i] << 8 | src[i+1];
		checksum += value;
	}
	
	checksum = checksum & 0xFFFF;

	value = src[0x18E] << 8 | src[0x18F];

	if(value == checksum)
	{
		printf("Checksum in %s was already 0x%04X\n", argv[1], checksum);
		if(!fixsize)
			return -1;
	}

	src[0x18E] = checksum >> 8 & 0xFF;
	src[0x18F] = checksum & 0xFF;

	fp = fopen(argv[1], "wb");
	if(!fp)
	{
		printf("Could not write to file %s\n", argv[1]);
		return -1;
	}
	
	if(fwrite(src, sizeof(char), srcSize, fp) != srcSize)
	{
		printf("Error writing data to file %s\n", argv[1]);
		fail = 1;;
	}
	else
		printf("Fixed Header for ROM %s\n", argv[1], checksum);
	fclose(fp);

	free(src);
	src = NULL;

	if(fail)
		return 1;
	return 0;
}

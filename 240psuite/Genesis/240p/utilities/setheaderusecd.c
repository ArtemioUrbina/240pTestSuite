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
	unsigned long int srcSize;
	int fail = 0, i = 0, setvalue = 0;
	unsigned char *src = NULL;
	
	if(argc != 3)
	{
		printf("Usage %s <source> <target>\n", argv[0]); 
		printf("Adds CD support to ROM header, Artemio Urbina 2022\n"); 
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

	src = (unsigned char*)malloc (sizeof(char)*srcSize);
	if (!src) 
	{
		fclose(fp);
		printf("Out of memory\n");
		return 1;
	}
	
	if(fread (src, sizeof(char), srcSize, fp) != srcSize)
	{
		fclose(fp);
		printf("Error loading file\n");
		return 1;
	}

	fclose(fp);
	fp = NULL;

	if(strncmp("SEGA", (char*)src + 0x100, 4) != 0)
	{
		free(src);
		printf("Not a Sega MegaDrive ROM: %s\n", argv[1]);
		return 1;
	}
	
	for(i = 0x190; i < 0x1A0; i++)
	{
		if(src[i] == 0x20)
		{
			src[i] = 'C';
			setvalue = 1;
			break;
		}
	}
	
	if(!setvalue)
	{
		printf("ROM header I/O Support was full in file: %s, aborted\n", argv[1]);
		return 1;
	}
	
	fp = fopen(argv[2], "wb");
	if(!fp)
	{
		printf("Could not write to file %s\n", argv[2]);
		return 1;
	}
	
	if(fwrite(src, sizeof(char), srcSize, fp) != srcSize)
	{
		printf("Error writing data to file %s\n", argv[2]);
		fail = 1;;
	}
	else
		printf("Added CD support to Header for ROM %s\n", argv[2]);
	fclose(fp);

	free(src);
	src = NULL;

	if(fail)
		return 1;
	return 0;
}

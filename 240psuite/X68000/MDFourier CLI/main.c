/* 
 * 240p Test Suite
 * Copyright (C)2021 Artemio Urbina
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
 
#include <dos.h>
#include <iocs.h>
#include <stdio.h>
#include <stdlib.h>
#include "video.h"
#include "key.h"
#include "crc.h"
#include "mdfourier.h"

#define ADPCM_SIZE	39061
#define ADPCM_CRC32	0x8E5FFD51

// Enter supervisor mode, so we can access vblank regiter
void initX68000()
{
	_dos_super(0);
}

u8 *loadADPCM(char *filename, u16 *size)
{
	FILE *file = NULL;
	u8 *file_buffer = NULL;
	u16 file_size = 0;
	
	if(!size)
		return NULL;
	*size = 0;
	
	file = fopen(filename, "r");
	if(!file)
		return NULL;
	
	fseek(file , 0, SEEK_END);
	file_size = ftell(file);	
	rewind(file);
	
	if(!file_size)
	{		
		fclose(file);
		return NULL;
	}
	
	// truncate the file, useful if transferred via XMODEM
	if(file_size > ADPCM_SIZE)
		file_size = ADPCM_SIZE;

	file_buffer = (u8*)malloc(sizeof(u8)*file_size);
	if(!file_buffer)
	{		
		fclose(file);
		return NULL;
	}
	
	if(fread(file_buffer, sizeof(u8), file_size, file) != file_size) 
	{		
		fclose(file);
		return NULL;
	}
	
	fclose(file);
	
	*size = file_size;
	return file_buffer;
}

int main(void)
{
	int 			input = 0;
	unsigned char	*adpcm_sweep = NULL;
	unsigned short	adpcm_size = 0;
	DWORD			crc32;
	
	initX68000();
	
	printf("MDFourier for X68000 v 0.3 -- http://junkerhq.net/MDFourier\n");
	printf("  Artemio Urbina 2021-2022\n\n");
	
	printf("Loading ADPCM samples...\n");
	adpcm_sweep = loadADPCM("sweep.pcm", &adpcm_size);
	if(!adpcm_sweep)
	{
		printf("Could not load ADPCM samples from 'sweep.pcm' file, aborting\n");
		return 0;
	}
	
	printf("Verifying ADPCM samples...\n");
	crc32 = crc32buf((char*)adpcm_sweep, adpcm_size);
	if(crc32 != ADPCM_CRC32)
	{
		printf("ADPCM samples did not match the expected CRC32: 0x%X != 0x%X. Aborting\n", (unsigned int)crc32, ADPCM_CRC32);
		free(adpcm_sweep);
		adpcm_sweep = NULL;
		adpcm_size = 0;
		return 0;
	}

	printf("Press SPACE or a joystick button when ready to record MDFourier\n");
	printf("Press ESC to just measure ADPCM DMA playback duration\n");
	
	do
	{
		input = read_input();
		wait_vblank();
	}
	while(!input);
	
	if(input == 1)
	{
		printf("Starting MDFourier test tones (ESC aborts)\n");
		ExecuteMDF(20, adpcm_sweep, adpcm_size);
	}
	if(input == -1)
	{
		printf("Starting MDFourier ADPCM DMA Playback test\n");
		ExecuteADPCMOnly(adpcm_sweep, adpcm_size);
	}
	
	if(adpcm_sweep)
	{
		free(adpcm_sweep);
		adpcm_sweep = NULL;
		adpcm_size = 0;
	}

	return 0;
}

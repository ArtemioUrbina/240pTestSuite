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

#include "types.h"

/* from inside x68000 book page 298 trasncribed and adapted by Artemio Urbina*/

struct DMAREG {
	unsigned char csr;
	unsigned char cer;
	unsigned short sparel;
	unsigned char dcr;
	unsigned char ocr;
	unsigned char scr;
	unsigned char ccr;
	unsigned short spare2; 
	unsigned short mtc;
	unsigned char *mar;
	unsigned long spare3;
	unsigned char *dar;
	unsigned short spare4;
	unsigned short btc;
	unsigned char *bar;
	unsigned long spare5;
	unsigned char spare6; 
	unsigned char niv;
	unsigned char spare7;
	unsigned char eiv;
	unsigned char spare8;
	unsigned char mfc;
	unsigned short spare9; 
	unsigned char spare10;
	unsigned char cpr;
	unsigned short spare11; 
	unsigned char spare12;
	unsigned char dfc;
	unsigned long spare13;
	unsigned short spare14;
	unsigned char spare15;
	unsigned char bfc;
	unsigned long spare16;
	unsigned char sparel7;
	unsigned char gcr;
};

/* 00: Left and right ON
   01: Right only 
   10: Left only 
   11: Left and right OFF */

#define ADPCM_STEREO	0x00
#define ADPCM_RIGHT		0x01
#define ADPCM_LEFT		0x02
#define ADPCM_NONE		0x03

void adpcm_outsel(unsigned char sel);

#define ADPCM_3_9KHZ_07_8KHZ		0x00
#define ADPCM_5_2KHZ_10_4KHZ		0x01
#define ADPCM_7_8KHZ_15_6KHZ		0x02

void adpcm_sample(unsigned char rate);

#define ADPCM_8MHZ	0x00
#define ADPCM_4MHZ	0x01
void adpcm_clksel(unsigned char sel);

void adpcm_stop();
void adpcm_start();
void adpcm_dma_setup(unsigned char *adpcm_source, unsigned short adpcm_size);
void Prepare_ADPCM_DMA(unsigned char *adpcm_source, unsigned short adpcm_size);
void adpcm_dma_start();
void adpcm_dma_wait_complete();
void adpcm_dma_clear_flag();


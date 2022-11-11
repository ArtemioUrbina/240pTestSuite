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
 
#include <stdio.h>
#include "ym2151.h"
#include "MSM6258.h"

void adpcm_outsel(unsigned char sel)
{
	vu8 *ppi_cwr;

	ppi_cwr = (u8*)0xe9a005;
	*ppi_cwr = (0 << 1) | ((sel >> 1) & 1); /* Left */
	*ppi_cwr = (1 << 1) | (sel & 1); /* Right */
}

/* 10 -> 1/512  (7.8k/4Mhz 15.6k/8Mhz) */
/* 01 -> 1/768  (5.2k/4Mhz 10.4k/8Mhz) */
/* 00 -> 1/1024 (3.9k/4Mhz 7.8k/8Mhz) */
void adpcm_sample(unsigned char rate)
{
	vu8 *ppi_cwr;

	ppi_cwr = (u8*)0xe9a005;
	*ppi_cwr = (rate << 2) & 0x0C;
}

/* OPM 0x1B sets clk, 0 -> 8Mhz 1 -> 4Mhz) */
void adpcm_clksel(unsigned char sel)
{
	YM2151_writeReg(0x1b, (sel & 1) << 7);
}

void adpcm_stop()
{
	vu8 *adpcm_command;

	adpcm_command  = (u8*)0xe92001;
	*adpcm_command = 0x1;
}

void adpcm_start()
{
	vu8 *adpcm_command;

	adpcm_command  = (u8*)0xe92001;
	*adpcm_command = 0x2;
}

void adpcm_dma_setup(unsigned char *adpcm_source, unsigned short adpcm_size)
{
	volatile struct DMAREG *dma;
	vu8 *adpcm_data;

	dma = (struct DMAREG *)0xe840c0;
	adpcm_data = (u8*)0xe92003;

	dma->dcr = 0x80;
	dma->ocr = 0x32;
	dma->scr = 0x04;
	dma->ccr = 0x00;
	dma->cpr = 0x08;
	dma->mfc = 0x05;
	dma->dfc = 0x05;

	dma->mtc = adpcm_size;
	dma->mar = adpcm_source;
	dma->dar = (unsigned char *)adpcm_data;
}

void Prepare_ADPCM_DMA(unsigned char *adpcm_source, unsigned short adpcm_size)
{
	adpcm_stop();
	adpcm_clksel(ADPCM_8MHZ); /* ADPCM Clock 8Mhz */
	adpcm_outsel(ADPCM_STEREO); /* Panpot Control Stereo*/
	adpcm_sample(ADPCM_7_8KHZ_15_6KHZ); /* Sampling rate 15.6k */
	adpcm_dma_clear_flag();

	adpcm_dma_setup(adpcm_source, adpcm_size);
}

void adpcm_dma_start()
{
	volatile struct DMAREG *dma;

	dma = (struct DMAREG *)0xe840c0;
	dma->ccr |= 0x80;
}

void adpcm_dma_wait_complete()
{
	vu8						*adpcm_status;
	volatile struct DMAREG 	*dma;

	dma = (struct DMAREG *)0xe840c0;
	adpcm_status = (u8*)0xe92001;

	while(!(dma->csr & 0x90) && ! (*adpcm_status & 0x80));
}

void adpcm_dma_clear_flag()
{
	volatile struct DMAREG *dma;

	dma = (struct DMAREG *)0xe840c0;
	dma->csr = 0xff;
}

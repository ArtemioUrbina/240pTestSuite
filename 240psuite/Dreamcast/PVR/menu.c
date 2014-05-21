/* 
 * 240p Test Suite
 * Copyright (C)2011 Artemio Urbina
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA	02111-1307	USA
 */

#include <kos.h>
#include <kmg/kmg.h>
#include <kos/img.h>
#include <zlib/zlib.h>
#include <assert.h>

#include "image.h"
#include "vmodes.h"
#include "menu.h"

//#define BENCHMARK 

uint DrawMenu = 0;
extern char *HelpData;
ImagePtr fbtexture = NULL;

#define FB_TEX_H	1024
#define FB_TEX_V	512
#define FB_TEX_BYTES	sizeof(uint16)

void InitTextureFB()
{	
	if(fbtexture)
		return;

	fbtexture = (ImagePtr)malloc(sizeof(struct image_st));	
	if(!fbtexture)
	{
		fprintf(stderr, "Could not malloc image struct for FB\n");
		return;
	}
	fbtexture->tex = pvr_mem_malloc(FB_TEX_H*FB_TEX_V*FB_TEX_BYTES); //Min  size for 640x480
	if(!fbtexture->tex)
	{
		free(fbtexture);
		fbtexture = NULL;
		fprintf(stderr, "Could not pvr mem malloc image struct for FB\n");
		return;
	}

	fbtexture->r = 1.0f;
	fbtexture->g = 1.0f;
	fbtexture->b = 1.0f;
    
	fbtexture->tw = FB_TEX_H;
	fbtexture->th = FB_TEX_V;
	fbtexture->x = 0;
	fbtexture->y = 0;
	fbtexture->u1 = 0.0f;
	fbtexture->v1 = 0.0f;
	fbtexture->u2 = 1.0f;
	fbtexture->v2 = 1.0f;
	fbtexture->layer = 0.5f;
	fbtexture->scale = 0;
	fbtexture->alpha = 1.0;
	fbtexture->w = fbtexture->tw;
	fbtexture->h = fbtexture->th;
	fbtexture->RefCount = 1;
	fbtexture->FH = 0;
	fbtexture->FV = 0;
	fbtexture->copyOf = NULL;
	fbtexture->texFormat = PVR_TXRFMT_ARGB1555;
}

void FreeTextureFB()
{
	if(fbtexture)
		FreeImage(&fbtexture);
}

#include "help.h"

#define rotr(value, shift) \
    (value >> shift) | (value << (16 - shift))

void ShowMenu()
{
	int 	i, numpix, w, tw, th;
	uint16  *buffer, npixel;
	uint32  save;
	uint32  pixel;  
	uint16  r, g, b;
#ifdef BENCHMARK
	uint32 start, end;
	char	msg[100];
		
	timer_ms_gettime(NULL, &start);
#endif
	InitTextureFB();

	numpix = vid_mode->width * vid_mode->height;
	w = vid_mode->width;
	tw = FB_TEX_H;
	th = FB_TEX_V;
	if(w == 320)
	{
		tw /= 2;
		th /= 2;
	}

	buffer = (uint16 *)malloc(FB_TEX_H*FB_TEX_V*FB_TEX_BYTES); 
	if(buffer == NULL)
	{
		dbglog(DBG_ERROR, "FB copy can't allocate memory\n");
		FreeTextureFB();
		return;
	}

	if(vid_mode->pm != PM_RGB565)
	{
		dbglog(DBG_ERROR, "FB copy: video mode not 565\n");
		free(buffer);
		FreeTextureFB();
		return;
	}

#ifdef BENCHMARK
	timer_ms_gettime(NULL, &end);
	sprintf(msg, "FB buffer init took %lu ms\n", (unsigned int)end - start);
	dbglog(DBG_KDEBUG, msg);
	timer_ms_gettime(NULL, &start);
#endif
	save = irq_disable();
	for(i = 0; i < numpix; i++)
	{
		uint x = 0, y = 0;

		pixel = vram_s[i];

		r = (((pixel >> 11) & 0x1f) << 0);
		g = (((pixel >>  5) & 0x3f) >> 1);
		b = (((pixel >>  0) & 0x1f) << 0);

		npixel = r << 2 | rotr(g, 3) | b << 8 | 0x80;

		x = i % w;
		y = i / w;
		y *= tw;
		buffer[y+x] = ((npixel << 8) & 0xff00) | ((npixel >> 8) & 0x00ff);
	}
	irq_restore(save);

#ifdef BENCHMARK
	timer_ms_gettime(NULL, &end);
	sprintf(msg, "FB conversion took %lu ms\n", (unsigned int)end - start);
	dbglog(DBG_KDEBUG, msg);
	timer_ms_gettime(NULL, &start);
#endif
	pvr_txr_load_ex (buffer, fbtexture->tex, tw, th, PVR_TXRLOAD_16BPP|PVR_TXRLOAD_SQ);
	fbtexture->tw = tw;
	fbtexture->th = th;
	fbtexture->w = fbtexture->tw;
	fbtexture->h = fbtexture->th;

#ifdef BENCHMARK
	timer_ms_gettime(NULL, &end);
	sprintf(msg, "FB texture upload took %lu ms\n", (unsigned int)end - start);
	dbglog(DBG_KDEBUG, msg);
#endif

	free(buffer);
	buffer = NULL;

	fbtexture->alpha = 0.75f;

	 DrawHelpWindow(HelpData, fbtexture);

	FreeTextureFB();
}

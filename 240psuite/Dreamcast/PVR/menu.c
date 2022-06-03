/* 
 * 240p Test Suite
 * Copyright (C)2011-2022 Artemio Urbina
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
#include <stdlib.h>

#include "image.h"
#include "vmodes.h"
#include "menu.h"
#include "controller.h"
#include "font.h"
#include "help.h"
#include "vmu.h"
#include "vmufs.h"
#include "hardware.h"
	
uint8		EndProgram = 0;
uint8		refreshVMU = 0;
uint		DrawMenu = 0;
extern char	*HelpData;
ImagePtr	fbtexture = NULL;
uint16		*fbtextureBuffer = NULL;

struct settings_st settings = DEFAULT_OPTIONS;
struct settings_st default_settings = DEFAULT_OPTIONS;

void FreeTextureFB()
{
	if(fbtexture)
		FreeImage(&fbtexture);

	if(EndProgram)
	{
		if(fbtextureBuffer)
		{
			free(fbtextureBuffer);
			fbtextureBuffer = NULL;
		}
	}
}

void InitTextureFB()
{	
	if(!fbtexture)
	{
		fbtexture = (ImagePtr)malloc(sizeof(struct image_st));	
		if(!fbtexture)
		{
			dbglog(DBG_ERROR, "Could not malloc image struct for FB\n");
			return;
		}
		memset(fbtexture, 0, sizeof(struct image_st));
		fbtexture->tex = NULL;
	}

	if(!fbtexture->tex)
	{
		fbtexture->tex = pvr_mem_malloc(FB_TEX_H*FB_TEX_V*FB_TEX_BYTES); //Min	size for 640x480
		if(!fbtexture->tex)
		{
			dbglog(DBG_ERROR, "Could not pvr mem malloc image struct for FB\n");
			FreeTextureFB();
			return;
		}
	}

	if(!fbtextureBuffer)
	{
		fbtextureBuffer = (uint16 *)malloc(FB_TEX_H*FB_TEX_V*FB_TEX_BYTES);
		if(fbtextureBuffer == NULL)
		{
			FreeTextureFB();
			dbglog(DBG_CRITICAL, "FB copy can't allocate memory\n");
			return;
		}
		memset(fbtextureBuffer, 0, FB_TEX_H*FB_TEX_V*FB_TEX_BYTES);
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
	fbtexture->FH = 0;
	fbtexture->FV = 0;
	fbtexture->texFormat = PVR_TXRFMT_ARGB1555;
	IgnoreOffset(fbtexture);
	
	InsertImage(fbtexture, "FB");
}

uint8 ReloadFBTexture()
{
	if(fbtextureBuffer && fbtexture)
	{
		if(!fbtexture->tex)
		{
			fbtexture->tex = pvr_mem_malloc(FB_TEX_H*FB_TEX_V*FB_TEX_BYTES);
			if(!fbtexture->tex)
			{
				dbglog(DBG_CRITICAL, "Could not malloc FB to VRAM\n");
				return 0;
			}
			memset(fbtexture->tex, 0, FB_TEX_H*FB_TEX_V*FB_TEX_BYTES);
		}

		pvr_txr_load_ex (fbtextureBuffer, fbtexture->tex,
			fbtexture->tw, fbtexture->th, PVR_TXRLOAD_16BPP|PVR_TXRLOAD_SQ);
		return 1;
	}
	return 0;
}

int CopyFBToBG()
{
	int 	i, numpix, w, tw, th;
	uint16	npixel;
	uint32	save;
	uint32	pixel;	
	uint16	*fbcopy = NULL;
	uint16	r, g, b;
#ifdef BENCHMARK
	uint64	start, end, mstart;
	char	msg[100];
		
	start = timer_ms_gettime64();
	mstart = start;
#endif

	if(!fbtexture)
		return 0;

	numpix = vid_mode->width * vid_mode->height;
	w = vid_mode->width;
	tw = FB_TEX_H;
	th = FB_TEX_V;
	if(w == 320)
	{
		tw /= 2;
		if(dH < 256)
			th /= 2;
	}

	if(vid_mode->pm != PM_RGB565)
	{
		dbglog(DBG_CRITICAL, "FB copy: video mode not 565\n");
		return 0;
	}

	fbcopy = (uint16*)malloc(sizeof(uint16)*numpix);
	if(!fbcopy)
	{
		dbglog(DBG_CRITICAL, "FB copy: not enough memory\n");
		return 0;
	}
	
	memset(fbtextureBuffer, 0, FB_TEX_H*FB_TEX_V*FB_TEX_BYTES);
#ifdef BENCHMARK
	end = timer_ms_gettime64();
	sprintf(msg, "FB buffer init took %"PRIu64" ms\n", end - start);
	dbglog(DBG_INFO, msg);
	start = timer_ms_gettime64();
#endif

	save = irq_disable();
	memcpy(fbcopy, vram_s, sizeof(uint16)*numpix);
	irq_restore(save);

#ifdef BENCHMARK
	end = timer_ms_gettime64();
	sprintf(msg, "FB buffer memcpy took %"PRIu64" ms\n", end - start);
	dbglog(DBG_INFO, msg);
	start = timer_ms_gettime64();
#endif

	for(i = 0; i < numpix; i++)
	{
		uint x = 0, y = 0;

		pixel = fbcopy[i];

		r = (((pixel >> 11) & 0x1f) << 0);
		g = (((pixel >>  5) & 0x3f) >> 1);
		b = (((pixel >>  0) & 0x1f) << 0);

		npixel = r << 2 | rotr(g, 3) | b << 8 | 0x80;

		x = i % w;
		y = i / w;
		y *= tw;
		fbtextureBuffer[y+x] = ((npixel << 8) & 0xff00) | ((npixel >> 8) & 0x00ff);
	}
		
	free(fbcopy);
	fbcopy = NULL;

#ifdef BENCHMARK
	end = timer_ms_gettime64();
	sprintf(msg, "FB conversion took %"PRIu64" ms\n", end - start);
	dbglog(DBG_INFO, msg);
	start = timer_ms_gettime64();
#endif

	pvr_txr_load_ex (fbtextureBuffer, fbtexture->tex, tw, th,
		PVR_TXRLOAD_16BPP|PVR_TXRLOAD_SQ);
	fbtexture->tw = tw;
	fbtexture->th = th;
	fbtexture->w = fbtexture->tw;
	fbtexture->h = fbtexture->th;

#ifdef BENCHMARK
	end = timer_ms_gettime64();
	sprintf(msg, "FB texture upload took %"PRIu64" ms\n", end - start);
	dbglog(DBG_INFO, msg);
	
	sprintf(msg, "FB texture entire process took %"PRIu64" ms\n", end - mstart);
	dbglog(DBG_INFO, msg);
#endif

	fbtexture->r = 0.75f;
	fbtexture->g = 0.75f;
	fbtexture->b = 0.75f;
	
	return 1;
}

void ShowMenu(char *filename)
{
	DrawMenu = 1;
	HelpData = filename;
}

void ShowHelpWindow(char *Data)
{
	if(!settings.IgnoreFrameBuffer)
	{
		InitTextureFB();
		CopyFBToBG();
	}
	HelpWindow(Data, fbtexture);
	FreeTextureFB();
}

void DrawShowMenu()
{
	ImagePtr	back;
	int			done = 0;
	int			sel = 1, oldsel = 0, joycnt = 0;
	char		*vmuopt[6] =
				{
					"Help",
					"Video",
					"Options",
					"Credits",
					"Close",
					"Exit"
				};

	if(!settings.IgnoreFrameBuffer)
	{
		InitTextureFB();
		CopyFBToBG();
	}

	back = LoadKMG("/rd/FloatMenu.kmg.gz", 0);
	if(!back)
	{
		FreeTextureFB();
		return;
	}	

	back->x = (dW - MENUSIZE_W) / 2;
	back->y = (dH - MENUSIZE_H) / 2;
	back->alpha = 0.75f;

	updateVMU("   MENU", "", 1);
	while(!done && !EndProgram)
	{
		float			r = 1.0f;
		float			g = 1.0f;
		float			b = 1.0f;
		int   			c = 1;
		uint16			x = back->x + 20;
		uint16			y = back->y + 10;
		uint16			pressed = 0;
		controller		*st;
				
		StartScene();
		
		if(fbtexture)
			DrawImage(fbtexture);
		DrawImage(back);

		DrawStringS(x-10, y, 0.0f, 1.0f, 0.0f, VERSION_NUMBER); y += 3*fh; 		
		
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b,
				"Help"); y += fh; c++;				
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b,
				"Video"); y += fh; c++;		
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b,
				"Options"); y += fh; c++;		
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b,
				"Credits"); y += fh; c++;		
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b,
				"Close Menu"); y += 2* fh; c++;			
		DrawStringS(x, y, r-0.2, sel == c ? 0 : g,	sel == c ? 0 : b,
				"Exit 240p Suite"); y += 3* fh; 		
		
		EndScene();		
		
		st = ReadController(0, &pressed);

		if ( pressed & CONT_DPAD_UP )
		{
			sel --;
			if(sel < 1)
				sel = c;
		}
		
		if ( pressed & CONT_DPAD_DOWN )
		{
			sel ++;
			if(sel > c)
				sel = 1;
		}
		
		JoystickMenuMove(st, &sel, c, &joycnt);
			
		if (pressed & CONT_B || pressed & CONT_START) 		
			done = 1;	
	
		if (pressed & CONT_A)
		{
			switch(sel)
			{			
					case 1:
						HelpWindow(HelpData, fbtexture);
						break;					
					case 2:		
						SelectVideoMode(fbtexture);
						back->x = (dW - MENUSIZE_W) / 2;
						back->y = (dH - MENUSIZE_H) / 2;
						break;
					case 3:		
						ChangeOptions(fbtexture);
						break;
					case 4:
						DrawCredits(fbtexture);
						break;
					case 5:
						done = 1;
						break;
					case 6:
						EndProgram = 1;
						done = 1;
						break;
					default:
						break;
			}
			updateVMU("   MENU", "", 1);
		}		

		if(oldsel != sel)
		{
			updateVMU("   MENU", vmuopt[sel-1],1);
			oldsel = sel;
		}
	}
	
	FreeTextureFB();
	FreeImage(&back);
	HelpData = GENERALHELP;

	updateVMU("        ", "", 1);
	refreshVMU = 1;

	return;
}


void ChangeOptions(ImagePtr screen)
{	
	int 		sel = 1, close = 0, broadcast, joycnt = 0;	
	int			saved = -1, loaded = -1, timer = 0, hint = 0;
	ImagePtr	back;
	char		error[256];
	
	back = LoadKMG("/rd/help.kmg.gz", 0);
	if(!back)
		return;

	back->alpha = 0.75f;

	broadcast = flashrom_get_region_broadcast();
	updateVMU("  Options", "", 1);
	srand((int)(time(0) ^ getpid()));
	while(!close && !EndProgram) 
	{		
		float			r = 1.0f;
		float			g = 1.0f;
		float			b = 1.0f;
		uint8			c = 1;
		uint16			x = 50;
		uint16			y = 25+fh;
		uint16			OptPos = 160;
		uint16			pressed = 0;
		char			intensity[80];
		int				changedPVR = 0;
		controller		*st;
				
		StartScene();

		if(screen)
			DrawImage(screen);
		DrawImage(back);

		DrawStringS(x + 70, 42, 0.0f, 1.0f, 0.0f, "General Options"); y += 2*fh; 

		// option 1, Deflicker
		DrawStringS(x + OptPos, y, r, sel == c ? 0 : g, sel == c ? 0 : b,
			settings.Deflicker == 1 ? "ON" : "OFF"); 
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b,
			"PVR Deflickering Filter:"); y += fh; c++;
		
		// option 2, Dithering	
		DrawStringS(x + OptPos, y, r, sel == c ? 0 : g, sel == c ? 0 : b,
			settings.Dithering == 1 ? "ON" : "OFF"); 
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b,
			"PVR Texture Dithering:"); y += fh; c++;
		
		// option 3, UseKOSDefaults	
		DrawStringS(x + OptPos, y, r, sel == c ? 0 : g, sel == c ? 0 : b,
			settings.UseKOSDefaults == 1 ? "ON" : "OFF"); 
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b,
			"Use KOS video Defaults:"); y += fh; c++;
			
		// option 4, ChangeVideoRegisters	
		DrawStringS(x + OptPos, y, r, sel == c ? 0 : g, sel == c ? 0 : b,
			settings.ChangeVideoRegs == 0 ? "ON" : "OFF"); 
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b,
			"Use KOS Register Defaults:"); y += fh; c++;

		// option 5,  Enable PAL
		if(broadcast != FLASHROM_BROADCAST_NTSC)
		{
			DrawStringS(x + OptPos, y, r, sel == c ? 0 : g, sel == c ? 0 : b,
				settings.EnablePAL == 1 ? "ON" : "OFF"); 
			DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b,
				"Enable PAL modes:"); y += fh; c++;
		}
		else
		{
			DrawStringS(x + OptPos, y, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f,
				settings.EnablePAL	== 1 ? "ON" : "OFF");
			DrawStringS(x, y, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f,
				"Enable PAL modes:"); y += fh; c++;
		}

		// option 6,  Enable PAL BG
		if(broadcast != FLASHROM_BROADCAST_NTSC && settings.EnablePAL)
		{
			DrawStringS(x + OptPos, y, r, sel == c ? 0 : g, sel == c ? 0 : b,
				settings.EnablePALBG == 1 ? "ON" : "OFF"); 
			DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b,
				"Enable PAL Background:"); y += fh; c++;			
		}
		else
		{
			DrawStringS(x + OptPos, y, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f,
				settings.EnablePALBG  == 1 ? "ON" : "OFF");
			DrawStringS(x, y, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f,
				"Enable PAL Background:"); y += fh; c++;			
		}

		// option 7,  PAL Background
		if(broadcast != FLASHROM_BROADCAST_NTSC && settings.EnablePAL)
		{
			char BorderColor[100];

			sprintf(BorderColor, "[%0.1f,%0.1f,%0.1f]",
				(double)settings.PalBackR, (double)settings.PalBackG, (double)settings.PalBackB); 
			DrawStringS(x + OptPos, y, r, sel == c ? 0 : g, sel == c ? 0 : b,
				BorderColor); 
			DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b,
				"Change PAL Background:"); y += fh; c++;
		}
		else
		{
			char BorderColor[100];

			sprintf(BorderColor, "[%0.1f,%0.1f,%0.1f]",
				(double)settings.PalBackR, (double)settings.PalBackG, (double)settings.PalBackB); 
			DrawStringS(x + OptPos, y, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f,
				BorderColor);
			DrawStringS(x, y, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f,
				"Change PAL Background:"); y += fh; c++;			
		}

		// option 8,  PAL Start
		if(broadcast != FLASHROM_BROADCAST_NTSC && settings.EnablePAL)
		{
			DrawStringS(x + OptPos, y, r, sel == c ? 0 : g, sel == c ? 0 : b,
				GetPalStartText());
			DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b,
				"PAL starting line:"); y += fh; c++;			
		}
		else
		{
			DrawStringS(x + OptPos, y, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f,
				GetPalStartText());
			DrawStringS(x, y, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f,
				"PAL starting line:"); y += fh; c++;			
		}

		// option 9, Scanline intensity
		sprintf(intensity, "%0.0f%%", GetScanlineIntensity());
		if(vmode == VIDEO_480P_SL)
		{
			DrawStringS(x + OptPos, y, r, sel == c ? 0 : g, sel == c ? 0 : b, intensity); 
			DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Scanline intensity:"); y += fh; c++;			
			
			// option 8, Scanline even/odd
			DrawStringS(x + OptPos, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, ScanlinesEven() ? "EVEN" : "ODD");
			DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Scanlines"); y += fh; c++;	
		}				
		else
		{
			DrawStringS(x + OptPos, y, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f,
				intensity);
			DrawStringS(x, y, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f,
				"Scanline intensity:"); y += fh; c++;			
			
			// option 10, Scanline even/odd
			DrawStringS(x + OptPos, y, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f,
				ScanlinesEven() ? "EVEN" : "ODD"); 					
			DrawStringS(x, y, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f,
				"Scanlines"); y += fh; c++;	
		}
		
		//Option 11, Save to VMU
		if(VMUPresent())
		{
			char *msg = NULL;

			switch(saved)
			{
				case 1:
					msg = "#GOptions saved to VMU#G";
					break;
				case -1:
					msg = "Save Options to VMU";
					break;
				case 0:
					msg = error;
					break;
				default:
					msg = "#RUndefined error#R";
					dbglog(DBG_ERROR, "ERROR: VMU save returned unexpected value %d: %s\n", saved, error);
					break;
			}

			DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, msg); y += fh; c++;
		}
		else
		{
			DrawStringS(x, y, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f,
					"Save Options to VMU"); y += fh; c++;
			saved = -1;
		}
		
		// Option 12, Load Options from VMU
		if(VMUPresent())
		{
			char *msg = NULL;
			
			switch(loaded)
			{
				case 1:
					msg = "#GOptions loaded from VMU#G";
					break;
				case -1:
					msg = "Load Options from VMU";
					break;
				case 0:
					msg = error;
					break;
				default:
					msg = "#RUndefined error#R";
					dbglog(DBG_ERROR, "ERROR: VMU load returned unexpected value %d: %s\n", loaded, error);
					break;
			}

			DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, msg); y += fh; c++;
		}
		else
		{
			DrawStringS(x, y, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f,
					"Load Options from VMU"); y += fh; c++;
			loaded = -1;
		}
		
		// Option 13, Disable FrameBuffer copies
		DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Use FrameBuffer copy:");
		DrawStringS(x + OptPos, y, r, sel == c ? 0 : g, sel == c ? 0 : b,
			settings.IgnoreFrameBuffer == 0 ? "ON" : "OFF");  y += fh; c++;		
		
		// Option 14, Reset to default options
		DrawStringS(x, y, r-0.2, sel == c ? 0 : g, sel == c ? 0 : b, "Reset all options to defaults"); y += fh; c++;
		
		// Option 15, Exit
		DrawStringS(x, y, r-0.2, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Main Menu"); 		
		
		// Comments on options
		r = g = b = 0.8;
		if(vmode == VIDEO_480P_SL && sel == 9)	
			DrawStringS(x-15, y + fh, r, g, b, "Adjust with L and R triggers"); 										
		if(broadcast == FLASHROM_BROADCAST_NTSC && (sel > 4 && sel < 9))
			DrawStringS(x-15, y + fh, r, g, b,
				"Only PAL FlashROMs can output PAL correctly"); 
		if(vmode != VIDEO_480P_SL && (sel == 9 || sel == 10))
			DrawStringS(x-15, y + fh, r, g, b,
				"Scanlines only in scaled 480p mode via D-SUB (VGA)");
		if(sel == 11 && hint == 1)
			DrawStringS(x-15, y + fh, r, g, b,
				"Hold L or R while saving for hidden eye-catchers!");
		DrawStringS(x+60, 200, r, g, b, "Press START for help");
		if(rand() % 1000 == 47)
			hint = !hint;
		EndScene();
		
		// Clean load/save messages
		if(timer)
		{
			timer--;
			if(!timer)
			{
				loaded = -1;
				saved = -1;
			}
		}

		st = ReadController(0, &pressed);
		
		if ( pressed & CONT_DPAD_UP )
		{
			sel --;
			if(sel < 1)
				sel = c;
		}
		
		if ( pressed & CONT_DPAD_DOWN )
		{
			sel ++;
			if(sel > c)
				sel = 1;
		}
		
		if(st && st->joyy != 0)
		{
			if(++joycnt > 5)
			{
				if(st && st->joyy > 0)
					sel ++;
				if(st && st->joyy < 0)
					sel --;
	
				if(sel < 1)
					sel = c;
				if(sel > c)
					sel = 1;
				joycnt = 0;
			}
		}
		else
			joycnt = 0;
		

		if ( pressed & CONT_RTRIGGER && sel == 9)
		{
			if(vmode == VIDEO_480P_SL)
				RaiseScanlineIntensity();
		}
		
		if ( pressed & CONT_LTRIGGER && sel == 9)
		{
			if(vmode == VIDEO_480P_SL)
				LowerScanlineIntensity();
		}

		if ( pressed & CONT_B ) 		
			close = 1;	
	
		if ( pressed & CONT_START ) 		
			HelpWindow(OPTIONSHELP, screen);
	
		if (pressed & CONT_A)
		{
			switch(sel)
			{		
					case 1:
						settings.Deflicker = !settings.Deflicker;
						ChangeResolution(vmode);
						break;
					case 2:
						settings.Dithering = !settings.Dithering;
						ChangeResolution(vmode);
						break;
					case 3:
						settings.UseKOSDefaults = !settings.UseKOSDefaults;
						ChangeResolution(vmode);
						break;
					case 4:
						settings.ChangeVideoRegs = !settings.ChangeVideoRegs;
						ChangeResolution(vmode);
						break;
					case 5:
						// NTSC consoles output a corrupt PAL signal
						if(broadcast != FLASHROM_BROADCAST_NTSC)
							settings.EnablePAL = !settings.EnablePAL;
						break;
					case 6:
						// NTSC consoles output a corrupt PAL signal
						if(settings.EnablePAL)
						{
							settings.EnablePALBG = !settings.EnablePALBG;
							changedPVR = 1;
						}
						break;
					case 7:
						if(settings.EnablePAL)
						{
							ChangePALBackgroundColor(screen);
							changedPVR = 1;
						}
						break;
					case 8:
						// NTSC consoles output a corrupt PAL signal
						if(settings.EnablePAL)
						{
							Set576iLine23Option(settings.PALStart+1);
							if(IsPAL)	
								ChangeResolution(vmode);
						}
						break;
					case 9:
						// Dealt with above
						break;
					case 10:
						if(vmode == VIDEO_480P_SL)
							ToggleScanlineEvenOdd();
						break;
					case 11:
						if(VMUPresent() && saved != 1)
						{
							int eyecatcher = 0;

							if ( st && st->buttons & CONT_RTRIGGER )
								eyecatcher = 1;
							if ( st && st->buttons & CONT_LTRIGGER )
								eyecatcher = 2;
							saved = WriteVMUSave(eyecatcher, error);
							loaded = -1;
							timer = 200;
						}
						break;
					case 12:		
						if(VMUPresent())
						{
							loaded = LoadVMUSave(error);
							ChangeResolution(vmode);
							saved = -1;
							timer = 200;
						}
						break;
					case 13:
						settings.IgnoreFrameBuffer = !settings.IgnoreFrameBuffer;
						break;
					case 14:
						{
							settings = default_settings;
							if(broadcast != FLASHROM_BROADCAST_NTSC)
								settings.EnablePAL = 1;
							loaded = saved = -1;
							ChangeResolution(vmode);
						}
						break;
					case 15:
						if ( st && st->buttons & CONT_RTRIGGER )
						{
							settings.drawborder = !settings.drawborder;
							changedPVR = 1;
						}
						else
							close = 1;
						break;
			}
		}

		if(changedPVR)
		{
			if(!settings.drawborder) // Draw back video signal limits?
				vid_border_color(0, 0, 0);
			else
				vid_border_color(255, 255, 255);
		
			if(!settings.EnablePALBG)
				pvr_set_bg_color(0.0f, 0.0f, 0.0f);
			else
				pvr_set_bg_color(settings.PalBackR, settings.PalBackG, settings.PalBackB);
		}
	}
	
	FreeImage(&back);

	return;
}

void ChangePALBackgroundColor(ImagePtr title)
{
	int 			sel = 1, close = 0;
	ImagePtr		back, block, blackblock;
		
	back = LoadKMG("/rd/help.kmg.gz", 0);
	if(!back)
		return;

	block = LoadKMG("/rd/white.kmg.gz", 0);
	if(!block)
		return;

	blackblock = LoadKMG("/rd/black.kmg.gz", 0);
	if(!blackblock)
		return;

	block->x = 160;
	block->y = 92;
	block->w = 20;
	block->h = 20;
	block->layer = 5.0;

	blackblock->x = block->x - 1;
	blackblock->y = block->y - 1;
	blackblock->w = block->w + 2;
	blackblock->h = block->h + 2;
	blackblock->r = 0;
	blackblock->g = 0;
	blackblock->b = 0;
	blackblock->layer = 4.0;

	back->alpha = 0.7;

	while(!close && !EndProgram) 
	{
		float	r = 1.0;
		float	g = 1.0;
		float	b = 1.0;
		uint8	c = 1;
		uint16	x = 80;
		uint16	y = 70;
		uint16	pressed = 0;
		char	color[80];
		controller		*st;

		block->r = settings.PalBackR;
		block->g = settings.PalBackG;
		block->b = settings.PalBackB;

		StartScene();

		DrawImage(title);
		DrawImage(back);
		DrawImage(blackblock);
		DrawImage(block);

		DrawStringS(x - 20, y, 0.0, 1.0, 0.0, "PAL background color"); y += 2*fh;

		sprintf(color, "Red:    %0.2f", (double)settings.PalBackR);
		DrawStringS(x, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, color); y += fh; c++;
		sprintf(color, "Green:  %0.2f", (double)settings.PalBackG);
		DrawStringS(x, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, color); y += fh; c++;
		sprintf(color, "Blue:   %0.2f", (double)settings.PalBackB);
		DrawStringS(x, y, r, sel == c ? 0 : g,  sel == c ? 0 : b, color); y += fh; c++;
		DrawStringS(x, y + fh, r-0.2, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Options Menu");

		DrawStringS(x-40, y + 6*fh, r-0.4, g-0.4, b-0.4, "The background color is used to fill the screen");
		DrawStringS(x-40, y + 7*fh, r-0.4, g-0.4, b-0.4, "to the selected PAL resolution when needed");

		if(sel != c)
			DrawStringS(x-40, y + 4*fh, r, g, b, "Adjust with L and R triggers or Left & Right");

		EndScene();

		st = ReadController(0, &pressed);

		if ( pressed & CONT_DPAD_UP  )
		{
			sel --;
			if(sel < 1)
				sel = c;
		}


		if ( pressed & CONT_DPAD_DOWN  )
		{
			sel ++;
			if(sel > c)
				sel = 1;
		}

		if ( pressed & CONT_B )
			close = 1;

		if (pressed & CONT_A)
		{
			switch(sel)
			{
				case 4:
					close = 1;
					break;
			}
		}

		if(st)
		{
			if ((st->buttons & CONT_LTRIGGER || pressed & CONT_DPAD_LEFT ) && sel == 1)
			{
				if(settings.PalBackR - 0.01 >= 0)
					settings.PalBackR -= 0.01;
			}
		
			if ((st->buttons & CONT_RTRIGGER || pressed & CONT_DPAD_RIGHT ) && sel == 1)
			{
				if(settings.PalBackR + 0.01 <= 1.0)
					settings.PalBackR += 0.01;
			}
		
			if ((st->buttons & CONT_LTRIGGER || pressed & CONT_DPAD_LEFT) && sel == 2)
			{
				if(settings.PalBackG - 0.01 >= 0)
					settings.PalBackG -= 0.01;
			}
		
			if ((st->buttons & CONT_RTRIGGER || pressed & CONT_DPAD_RIGHT) && sel == 2)
			{
				if(settings.PalBackG + 0.01 <= 1.0)
					settings.PalBackG += 0.01;
			}

			if ((st->buttons & CONT_LTRIGGER || pressed & CONT_DPAD_LEFT) && sel == 3)
			{
				if(settings.PalBackB - 0.01 >= 0)
					settings.PalBackB -= 0.01;
			}
		
			if ((st->buttons & CONT_RTRIGGER || pressed & CONT_DPAD_RIGHT) && sel == 3)
			{
				if(settings.PalBackB + 0.01 <= 1.0)
					settings.PalBackB += 0.01;
			}
		}
	}
	FreeImage(&back);
	FreeImage(&block);
	FreeImage(&blackblock);
}

void SelectVideoMode(ImagePtr screen)
{
	int 		sel = 1, close = 0, oldsel = 0, joycnt = 0, broadcast;		
	ImagePtr	back;
	char		*vmuopt[9] =
				{
					"240p",
					"480i/LD",
					"480i/FS",
					"288p",
					"576i/LD",
					"576i/FS",
					"480p/LD",
					"480p/FS",
					"Close"
				};
	int			vmodepos[8] =  // take in Video Mode, outputs menu position
				{
					0, //VIDEO_240P
					3, //VIDEO_288P
					1, //VIDEO_480I_A240
					4, //VIDEO_576I_A264
					6, //VIDEO_480P_SL
					2, //VIDEO_480I
					5, //VIDEO_576I
					7, //VIDEO_480P
				};
	controller	*st = NULL;
	
	back = LoadKMG("/rd/help.kmg.gz", 0);
	if(!back)
		return;
		
	back->alpha = 0.75f;

	broadcast = flashrom_get_region_broadcast();
	sel = vmodepos[vmode] + 1;
	updateVMU("Video Mode", "", 1);
	while(!close && !EndProgram) 
	{		
		float	r = 1.0f;
		float	g = 1.0f;
		float	b = 1.0f;
		uint8	c = 1;
		uint16	x = 80;
		uint16	y = 60;
		uint16	pressed = 0;

		StartScene();

		if(screen)
			DrawImage(screen);
		DrawImage(back);

		DrawStringS(x - 20, y, 0.0f, 1.0f, 0.0f, "Please select the desired video mode"); y += 2*fh; 
		
		DrawStringS(x - 10, y + (vmodepos[vmode] * fh)+((vmodepos[vmode] >= 3) ? fh/2 : 0) +
			((vmodepos[vmode] >= 6) ? fh/2 - 1: 0), 0.0f, 1.0f, 0.0f, ">"); 
		
		if(vcable != CT_VGA)
		{
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "240p"); y += fh; c++;
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b,
				"480i scaled 240p assets (NTSC)"); y += fh; c++;
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b,
				"480i mixed 480p/240p assets (1:1/NTSC)"); y += fh; c++;
		}
		else
		{
			DrawStringS(x, y, sel != c ? 0.5f : 0.7f, sel != c ? 0.5f : 0.7f, sel != c ? 0.5f : 0.7f,
				"240p"); y += fh; c++;
			DrawStringS(x, y, sel != c ? 0.5f : 0.7f, sel != c ? 0.5f : 0.7f, sel != c ? 0.5f : 0.7f,
				"480i scaled 240p assets (NTSC)"); y += fh; c++;
			DrawStringS(x, y, sel != c ? 0.5f : 0.7f, sel != c ? 0.5f : 0.7f, sel != c ? 0.5f : 0.7f,
				"480i mixed 480p/240p assets (1:1/NTSC)"); y += fh; c++;
		}

		y += fh/2;
		if(vcable != CT_VGA && settings.EnablePAL && broadcast != FLASHROM_BROADCAST_NTSC)
		{
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b,
				"288p mixed 240p/264p assets"); y += fh; c++;
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b,
				"576i scaled 240p assets (PAL)"); y += fh; c++;
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b,
				"576i mixed 480p/240p assets (1:1/PAL)"); y += fh; c++;
		}
		else
		{
			DrawStringS(x, y, sel != c ? 0.5f : 0.7f, sel != c ? 0.5f : 0.7f, sel != c ? 0.5f : 0.7f,
				"288p with 264p assets"); y += fh; c++;
			DrawStringS(x, y, sel != c ? 0.5f : 0.7f, sel != c ? 0.5f : 0.7f, sel != c ? 0.5f : 0.7f,
				"576i scaled 240p assets (PAL)"); y += fh; c++;
			DrawStringS(x, y, sel != c ? 0.5f : 0.7f, sel != c ? 0.5f : 0.7f, sel != c ? 0.5f : 0.7f,
				"576i mixed 480p/240p assets (1:1/PAL)"); y += fh; c++;
		}

		y += fh/2;
		if(vcable == CT_VGA)
		{
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b,
				"480p scaled 240p assets & scanlines"); y += fh; c++;
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b,
				"480p mixed 480p/240p assets (1:1)"); y += fh; c++;			
		}
		else
		{
			DrawStringS(x, y, sel != c ? 0.5f : 0.7f, sel != c ? 0.5f : 0.7f, sel != c ? 0.5f : 0.7f,
				"480p scaled 240p assets & scanlines"); y += fh; c++;
			DrawStringS(x, y, sel != c ? 0.5f : 0.7f, sel != c ? 0.5f : 0.7f, sel != c ? 0.5f : 0.7f,
				"480p mixed 480p/240p assets (1:1)"); y += fh; c++;			
		}	
			
		DrawStringS(x, y + fh, r-0.2, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Main Menu"); 		
				
		r = g = b = 0.8;

		if(vcable == CT_COMPOSITE && broadcast != FLASHROM_BROADCAST_NTSC && (sel >= 0 && sel <= 3))
			DrawStringS(x-40, y + 3* fh, r, g, b,
				"PAL60 colors might decode incorrectly"); 
		if(broadcast == FLASHROM_BROADCAST_NTSC && (sel >= 4 && sel <= 6))
			DrawStringS(x-40, y + 3*fh, r, g, b,
				"Only PAL FlashROMs can output PAL correctly"); 

		if(vcable != CT_VGA && ((sel == 7) || (sel == 8)))
			DrawStringS(x-40, y + 3*fh, r, g, b,
				"480p is only available though D-SUB (VGA)"); 

		DrawStringS(x+40, 200, r, g, b, "Press START for help");
		EndScene();

		st = ReadController(0, &pressed);

		if ( pressed & CONT_DPAD_UP )
		{
			sel --;
			if(sel < 1)
				sel = c;
		}
		
		if ( pressed & CONT_DPAD_DOWN )
		{
			sel ++;
			if(sel > c)
				sel = 1;	
		}
		
		JoystickMenuMove(st, &sel, c, &joycnt);

		if ( pressed & CONT_B ) 
			close = 1;	

		if ( pressed & CONT_START ) 
			HelpWindow(VIDEOHELP, screen);

		if (pressed & CONT_A)
		{
			switch(sel)
			{
					case 1:
						if(vcable != CT_VGA)
							ChangeResolution(VIDEO_240P);
						break;
					case 2:
						if(vcable != CT_VGA)
							ChangeResolution(VIDEO_480I_A240);
						break;
					case 3:
						if(vcable != CT_VGA)
							ChangeResolution(VIDEO_480I);
						break;
					case 4:
						if(vcable != CT_VGA && settings.EnablePAL &&
							broadcast != FLASHROM_BROADCAST_NTSC)
							ChangeResolution(VIDEO_288P);
						break;
					case 5:
						if(vcable != CT_VGA && settings.EnablePAL &&
							broadcast != FLASHROM_BROADCAST_NTSC)
							ChangeResolution(VIDEO_576I_A264);
						break;
					case 6:
						if(vcable != CT_VGA && settings.EnablePAL &&
							broadcast != FLASHROM_BROADCAST_NTSC)
							ChangeResolution(VIDEO_576I);
						break;
					case 7:
						if(vcable == CT_VGA)
							ChangeResolution(VIDEO_480P_SL);
						break;					
					case 8:
						if(vcable == CT_VGA)
							ChangeResolution(VIDEO_480P);
						break;		
					case 9:
						close = 1;
						break;
					default:
						break;
			}
		}		
		if(oldsel != sel)
		{
			updateVMU("Video Mode", vmuopt[sel-1], 1);
			oldsel = sel;
		}
	}
	FreeImage(&back);

	return;
}

void DrawNish()
{
	uint16 			done  = 0, held = 0;
	controller		*st;
	ImagePtr		nish;

	nish = LoadKMG("/rd/nish.kmg.gz", 0);
	if(!nish)
		return;

	updateVMU("Nishka", "MEOW", 1);
	while(!done)
	{
		done = 1;

		StartScene();
		DrawImage(nish);
		EndScene();

		st = ReadController(0, &held);

		if(st && st->buttons & CONT_RTRIGGER)
			done = 0;
	}
	FreeImage(&nish);
}

void DrawCreditsOnFB()
{
	if(!settings.IgnoreFrameBuffer)
	{
		InitTextureFB();
		CopyFBToBG();
	}
	DrawCredits(fbtexture);
	FreeTextureFB();
}

void DrawCredits(ImagePtr back)
{
	int 		done = 0;
	uint16		pressed, counter = 1;
	char 		data[50];
	controller	*st;
	float		r, b, g;

	updateVMU("	Credits", "", 1);
	if(back)
	{
		r = back->r;
		g = back->b;
		b = back->g;

		back->r = 0.4;
		back->g = 0.4;
		back->b = 0.4;
	}

	while(!done && !EndProgram) 
	{
		int x = 20, y = 10;

		StartScene();
		if(back)
			DrawImage(back);

		DrawStringS(x, y, 0.0, 1.0, 0.0, "Code and Patterns:"); y += fh; 
		DrawStringS(x+5, y, 1.0, 1.0, 1.0, data); y += fh; 

		if(counter == 1)
			sprintf(data, "Artemio Urbina");
		if(counter == 60*4)
			sprintf(data, "@Artemio (twitter)");
		if(counter == 60*8)
			counter = 0;

		y += fh;
		DrawStringS(x+20, y, 0.0, 1.0, 0.0, "SDK:"); 
		DrawStringS(x+170, y, 0.0, 1.0, 0.0, "SDK Assistance:"); y += fh; 
		DrawStringS(x+25, y, 1.0, 1.0, 1.0, "KallistiOS");
		DrawStringS(x+175, y, 1.0, 1.0, 1.0, "BlueCrab"); y += fh; 
		
		y += fh;
		DrawStringS(x, y, 0.0, 1.0, 0.0, "Monoscope Pattern:"); y += fh; 
		DrawStringS(x+5, y, 1.0, 1.0, 1.0, "Keith Raney"); y += fh; 		
		DrawStringS(x, y, 0.0, 1.0, 0.0, "Donna Art:"); y += fh; 
		DrawStringS(x+5, y, 1.0, 1.0, 1.0, "Jose Salot (@pepe_salot)"); y += fh; 
		
		y += fh;
		DrawStringS(x, y, 0.0, 1.0, 0.0, "Menu Pixel Art:"); y += fh; 
		DrawStringS(x+5, y, 1.0, 1.0, 1.0, "Asher"); y += fh; 		
		DrawStringS(x, y, 0.0, 1.0, 0.0, "Advisor:"); y += fh; 
		DrawStringS(x+5, y, 1.0, 1.0, 1.0, "Fudoh"); y += fh; 
		DrawStringS(x, y, 0.0, 1.0, 0.0, "Collaboration:"); y += fh; 
		DrawStringS(x+5, y, 1.0, 1.0, 1.0, "shmups regulars"); y += fh; 
		y += fh;
		DrawStringS(x, y, 0.0, 1.0, 0.0, "Info on using this suite:"); y += fh; 
		DrawStringS(x+5, y, 1.0, 1.0, 1.0, "http://junkerhq.net/240p/"); y += fh; 

		y += fh;
		DrawStringS(x+20, y, 0.0, .75, .75, "This program is free software and open source.");  y += fh;
		DrawStringS(x+20, y, 0.0, .75, .75, "Source code is available under GPL.");  y += fh;
#ifndef NO_FFTW
		DrawStringS(x+20, y, 0.0, 0.75, 0.75, "Includes the #GFastest Fourier Transform in the West#G"); y += fh;
		DrawStringS(x+20, y, 0.0, 0.75, 0.75, "http://www.fftw.org/"); y += fh;
#endif

		y = 10;
		DrawStringS(200, y, 1.0, 1.0, 1.0, VERSION_NUMBER); y += fh; 
		DrawStringS(200, y, 1.0, 1.0, 1.0, VERSION_DATE); y += fh; 
		y += fh*16;
		DrawStringS(200, y, 0.0, 0.75, 0.75, "Dedicated to Elisa"); 

		EndScene();
		counter ++;

		st = ReadController(0, &pressed);
		if(st)
		{
			if (pressed & CONT_B)
				done = 1;

			if (pressed & CONT_RTRIGGER)
			{
				DrawNish();
				updateVMU("	Credits", "", 1);
			}

		}
	}

	if(back)
	{
		back->r = r;
		back->g = g;
		back->b = b;
	}
}

void DrawIntro()
{
	uint32			counter, frames = 30;
	float			delta = 0.0f;
	ImagePtr		black = NULL, back = NULL;

	back = LoadKMG("/rd/black.kmg.gz", 1);
	if(!back)
		return;
		
	black = LoadKMG("/rd/black.kmg.gz", 1);
	if(!black)
		return;

	black->alpha = 1.0f;
	delta = 1.0f / frames;
	black->w = dW;
	black->h = dH;
	black->layer = 5.0f;
	updateVMU(" KORDAMP", "", 1);
	for(counter = 0; counter < frames*2; counter ++)
	{
		black->alpha -= delta;
		if(black->alpha < 0.0f)
			black->alpha = 0.0f;

		if(counter == frames)
			delta *= -1;

		StartScene();
		DrawImage(back);
		DrawStringS(120, 115, 1.0, 1.0, 1.0, "KORDAMP PRESENTS");
		DrawImage(black);

		EndScene();
	}
	FreeImage(&black);
	FreeImage(&back);
}

int SelectMenu(char *title, fmenudata *menu_data, int num_options, int selected_option)
{
	int 		sel = selected_option, close = 0, value = MENU_CANCEL, joycnt = 0;		
	ImagePtr	Back = NULL, black = NULL;
	
	black = LoadKMG("/rd/black.kmg.gz", 1);
	if(!black)
		return MENU_CANCEL;
		
	Back = LoadKMG("/rd/FloatMenu.kmg.gz", 0);
	if(Back)
	{
		Back->x = (dW - MENUSIZE_W) / 2;
		Back->y = (dH - MENUSIZE_H) / 2;
		Back->alpha = 0.75f;
	}

	while(!close) 
	{		
		float		r = 1.0f;
		float		g = 1.0f;
		float		b = 1.0f;
		int			c = 1, i = 0;				    					   
		float		x = Back ? Back->x + 20.0f : 100.0f;
		float		y = Back ? Back->y + 10.0f : 60.0f;
        uint16		pressed = 0;
		controller	*st;
				
		StartScene();
		
		if(black)
			DrawImage(black); 
		if(Back)        
			DrawImage(Back);       

		DrawStringS(x, y, 0.0f, 1.0f, 0.0f, title); y += 3*fh; 		
		
		for(i = 0; i < num_options; i++)
		{
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, menu_data[i].option_text);
			y += fh; c++;		
		}
        
		y += 2* fh;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Close Menu");		
								
		EndScene();		
		
		st = ReadController(0, &pressed);
		
		if ( pressed & CONT_DPAD_UP )
	    {
		    sel --;
		    if(sel < 1)
			    sel = c;		
	    }
	    
	    if ( pressed & CONT_DPAD_DOWN )
	    {
		    sel ++;
		    if(sel > c)
			    sel = 1;	
	    }		

		JoystickMenuMove(st, &sel, c, &joycnt);	
			
		if (pressed & CONT_B || pressed & CONT_START) 		
		{
			close = 1;	
			value = MENU_CANCEL;
		}
	
		if (pressed & CONT_A)
		{     
			close = 1;

			if(sel == c)
				value = MENU_CANCEL;
			else
				value = menu_data[sel - 1].option_value;
		}		
	}
	
	if(Back)
		FreeImage(&Back);
	if(black)
		FreeImage(&black);

	refreshVMU = 1;
	return value;
}

void DrawMessage(char *msg)
{
	int 		done = 0;
	uint16		pressed;
	ImagePtr	back = NULL;
	
	back = LoadKMG("/rd/black.kmg.gz", 1);
	while(!done && !EndProgram) 
	{
		StartScene();
		if(back)
			DrawImage(back);
		DrawStringS(20, 60, 0.0f, 1.0f, 0.0f, msg); 
		EndScene();
	
		ReadController(0, &pressed);
		if(pressed & CONT_A || pressed & CONT_B)
			done =	1;
	}
	if(back)
		FreeImage(&back);
}

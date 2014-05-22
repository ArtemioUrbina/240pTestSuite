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
#include "controller.h"
#include "font.h"
#include "help.h"

//#define BENCHMARK 

#define MENUSIZE_W 116
#define MENUSIZE_H 123

uint8 EndProgram = 0;
uint DrawMenu = 0;
extern char *HelpData;
ImagePtr fbtexture = NULL;
uint16  *fbtextureBuffer = NULL;

#define FB_TEX_H	1024
#define FB_TEX_V	512
#define FB_TEX_BYTES	sizeof(uint16)

#define rotr(value, shift) \
    (value >> shift) | (value << (16 - shift))

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
			fprintf(stderr, "Could not malloc image struct for FB\n");
			return;
		}
		fbtexture->tex = NULL;
	}

	if(!fbtexture->tex)
	{
		fbtexture->tex = pvr_mem_malloc(FB_TEX_H*FB_TEX_V*FB_TEX_BYTES); //Min  size for 640x480
		if(!fbtexture->tex)
		{
			FreeTextureFB();
			fprintf(stderr, "Could not pvr mem malloc image struct for FB\n");
			return;
		}
	}

	if(!fbtextureBuffer)
	{
		fbtextureBuffer = (uint16 *)malloc(FB_TEX_H*FB_TEX_V*FB_TEX_BYTES); 
		if(fbtextureBuffer == NULL)
		{
			FreeTextureFB();
			dbglog(DBG_ERROR, "FB copy can't allocate memory\n");
			return;
		}
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
				fprintf(stderr, "Could not malloc FB to VRAM\n");
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

void CopyFBToBG()
{
	int 	i, numpix, w, tw, th;
	uint16  npixel;
	uint32  save;
	uint32  pixel;  
	uint16  r, g, b;
#ifdef BENCHMARK
	uint32 start, end;
	char	msg[100];
		
	timer_ms_gettime(NULL, &start);
#endif

	if(!fbtexture)
		return;

	numpix = vid_mode->width * vid_mode->height;
	w = vid_mode->width;
	tw = FB_TEX_H;
	th = FB_TEX_V;
	if(w == 320)
	{
		tw /= 2;
		th /= 2;
	}

	if(vid_mode->pm != PM_RGB565)
	{
		dbglog(DBG_ERROR, "FB copy: video mode not 565\n");
		FreeTextureFB();
		return;
	}

	memset(fbtextureBuffer, 0, FB_TEX_H*FB_TEX_V*FB_TEX_BYTES);
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
		fbtextureBuffer[y+x] = ((npixel << 8) & 0xff00) | ((npixel >> 8) & 0x00ff);
	}
	irq_restore(save);

#ifdef BENCHMARK
	timer_ms_gettime(NULL, &end);
	sprintf(msg, "FB conversion took %lu ms\n", (unsigned int)end - start);
	dbglog(DBG_KDEBUG, msg);
	timer_ms_gettime(NULL, &start);
#endif
	pvr_txr_load_ex (fbtextureBuffer, fbtexture->tex, tw, th, PVR_TXRLOAD_16BPP|PVR_TXRLOAD_SQ);
	fbtexture->tw = tw;
	fbtexture->th = th;
	fbtexture->w = fbtexture->tw;
	fbtexture->h = fbtexture->th;

#ifdef BENCHMARK
	timer_ms_gettime(NULL, &end);
	sprintf(msg, "FB texture upload took %lu ms\n", (unsigned int)end - start);
	dbglog(DBG_KDEBUG, msg);
#endif

	fbtexture->alpha = 0.75f;
}

void ShowMenu(char *filename)
{
	DrawMenu = 1;
	HelpData = filename;
}

void DrawShowMenu()
{
	ImagePtr	back;
	int		done = 0;
	int		sel = 1;

	InitTextureFB();
	CopyFBToBG();

	back = LoadKMG("/rd/FloatMenu.kmg.gz", 1);
	if(!back)
	{
		FreeTextureFB();
		return;
	}	

	back->x = (dW - MENUSIZE_W) / 2;
        back->y = (dH - MENUSIZE_H) / 2;
	back->alpha = 0.75f;

	while(!done && !EndProgram)
	{
		float   r = 1.0f;
		float   g = 1.0f;
		float   b = 1.0f;
		int   	c = 1;				    					   
		uint16	x = back->x + 20;
		uint16  y = back->y + 10;
        	uint16	pressed = 0;
				
		StartScene();
		
		if(fbtexture)
			DrawImage(fbtexture);
		DrawImage(back);

		DrawStringS(x, y, 0.0f, 1.0f, 0.0f, VERSION_NUMBER); y += 3*fh; 		
		
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Help"); y += fh; c++;				
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Video"); y += fh; c++;		
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Options"); y += fh; c++;		
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Close Menu"); y += 2* fh; c++;			
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "Exit 240p Suite"); y += 3* fh; 		
		
		EndScene();		
		
		ReadController(0, &pressed);

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
						SelectVideoMode();
						break;
					case 3:		
						ChangeOptions();						
						break;
					case 4:
						done = 1;
						break;
					case 5:
						EndProgram = 1;
						done = 1;
						break;
					default:
						break;
			} 			            										
		}		
	}
	
	FreeTextureFB();
	FreeImage(&back);
	HelpData = GENERALHELP;

	return;
}


void ChangeOptions()
{	
	int 		sel = 1, close = 0;	
	ImagePtr	back;
	
	back = LoadKMG("/rd/help.kmg.gz", 1);
	if(!back)
		return;
		
	back->alpha = 0.75f;
		
	while(!close && !EndProgram) 
	{		
		float	r = 1.0f;
		float	g = 1.0f;
		float	b = 1.0f;
		uint8	c = 1;				    					   
		uint16	x = 80;
		uint16	y = 70;
		uint16	OptPos = 140;
        	uint16	pressed = 0;
		char	intensity[80];
				
		StartScene();
		        
		if(fbtexture)
			DrawImage(fbtexture);
		DrawImage(back);

		DrawStringS(x - 20, y, 0.0f, 1.0f, 0.0f, "General Options"); y += 2*fh; 

		// option 1, Scanline intensity
		sprintf(intensity, "%f%%", GetScanlineIntensity());
		if(vmode == FAKE_640_SL)
		{
			DrawStringS(x + OptPos, y, r, sel == c ? 0 : g, sel == c ? 0 : b, intensity); 
			DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Scanline intensity:"); y += fh; c++;			
			
			// option 2, Scanline even/odd
			DrawStringS(x + OptPos, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, ScanlinesEven() ? "EVEN" : "ODD"); 					
			DrawStringS(x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, "Scanlines"); y += fh; c++;	
		}				
		else
		{
			DrawStringS(x + OptPos, y, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, intensity);
			DrawStringS(x, y, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, "Scanline intensity:"); y += fh; c++;			
			
			// option 2, Scanline even/odd
			DrawStringS(x + OptPos, y, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, ScanlinesEven() ? "EVEN" : "ODD"); 					
			DrawStringS(x, y, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, sel == c ? 0.5f : 0.7f, "Scanlines"); y += fh; c++;	
		}
		
		DrawStringS(x, y + 2* fh, r, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Main Menu"); 		
				
		if(vmode == FAKE_640_SL && sel == 1)	
			DrawStringS(x-40, y + 4*fh, r, g, b, "Adjust with L and R triggers"); 										
		if(vmode != FAKE_640_SL && (sel == 1 || sel == 2))
			DrawStringS(x-40, y + 4*fh, r, g, b, "Scanlines are only available in\n480 Line Doubled mode"); 						
			
		EndScene();		
        
		ReadController(0, &pressed);

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

		if ( pressed & CONT_RTRIGGER && sel == 1)
	    	{
			if(vmode == FAKE_640_SL)
				RaiseScanlineIntensity();
	    	}
	    
	    	if ( pressed & CONT_LTRIGGER && sel == 1)
	    	{
			if(vmode == FAKE_640_SL)
				LowerScanlineIntensity();
	    	}			
			
		if ( pressed & CONT_B ) 		
			close = 1;	
	
		if (pressed & CONT_A)
		{     
			switch(sel)
			{			
					case 2:
						if(vmode == FAKE_640_SL)
							ToggleScanlineEvenOdd();
						break;
					case 3:
						close = 1;
						break;
					default:
						break;
			} 			            										
		}		
	}
	
	FreeImage(&back);

	return;
}

void SelectVideoMode()
{
	int 		sel = 1, close = 0;		
	ImagePtr	back;
	
	
	back = LoadKMG("/rd/help.kmg.gz", 1);
	if(!back)
		return;
		
	back->alpha = 0.75f;
	while(!close && !EndProgram) 
	{		
		float	r = 1.0f;
		float   g = 1.0f;
		float   b = 1.0f;
		uint8	c = 1;				    					   
		uint16	x = 80;
		uint16	y = 80;
        	uint16	pressed = 0;
		uint8	changed = 0;
				
		StartScene();
		        
		if(fbtexture)
			DrawImage(fbtexture);
		DrawImage(back);        

		DrawStringS(x - 20, y, 0.0f, 1.0f, 0.0f, "Please select the desired video mode"); y += 2*fh; 
		
		DrawStringS(x - 10, y + (vmode * fh), 0.0f, 1.0f, 0.0f, ">"); 
		
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "240p"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "480i scaled 240p assets (NTSC)"); y += fh; c++;
		DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "480i mixed 480p/240p assets (1:1/NTSC)"); y += fh; c++;
		if(vcable == CT_VGA)
		{
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "480p scaled 240p assets & scanlines"); y += fh; c++;
			DrawStringS(x, y, r, sel == c ? 0 : g,	sel == c ? 0 : b, "480p mixed 480p/240p assets (1:1)"); y += fh; c++;			
		}
		else
		{
			DrawStringS(x, y, sel != c ? 0.5f : 0.7f, sel != c ? 0.5f : 0.7f, sel != c ? 0.5f : 0.7f, "480p scaled 240p assets & scanlines"); y += fh; c++;
			DrawStringS(x, y, sel != c ? 0.5f : 0.7f, sel != c ? 0.5f : 0.7f, sel != c ? 0.5f : 0.7f, "480p mixed 480p/240p assets (1:1)"); y += fh; c++;			
		}	
			
		DrawStringS(x, y + fh, r, sel == c ? 0 : g, sel == c ? 0 : b, "Back to Main Menu"); 		
				
		if(vcable != CT_VGA && ((sel == 4) || (sel == 5)))
			DrawStringS(x-40, y + 4*fh, r, g, b, "480p is only available though D-SUB (VGA)"); 						
		EndScene();		
        
		ReadController(0, &pressed);

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
			
		if ( pressed & CONT_B ) 		
			close = 1;	
	
		if (pressed & CONT_A)
		{     
			switch(sel)
			{			
					case 1:
						ReleaseTextures();
						ChangeResolution(NATIVE_320);
						changed = 1;
						break;
					case 2:
						ReleaseTextures();
						ChangeResolution(FAKE_640);
						changed = 1;
						break;
					case 3:
						ReleaseTextures();
						ChangeResolution(NATIVE_640);
						changed = 1;
						break;
					case 4:
						if(vcable == CT_VGA)
						{
							ReleaseTextures();
							ChangeResolution(FAKE_640_SL);
							changed = 1;
						}
						break;					
					case 5:
						if(vcable == CT_VGA)
						{
							ReleaseTextures();
							ChangeResolution(NATIVE_640_FS);
							changed = 1;
						}
						break;		
					case 6:
						close = 1;
						break;
					default:
						break;
			} 			            										
		}		

		if(changed)
		{
			RefreshLoadedImages();
			changed = 0;
		}
	}
	FreeImage(&back);

	return;
}

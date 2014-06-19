/* 
 * 240p Test Suite
 * Copyright (C)2014 Artemio Urbina (Wii GX)
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
 
/*---------------------------------------------------------------------------------

	Image Functions

---------------------------------------------------------------------------------*/

#include <stdio.h>
#include "image.h"
#include "menu.h"
#include "options.h"

static u8 gp_fifo[GX_FIFO_MINSIZE] ATTRIBUTE_ALIGN(32);

TPLFile backsTPL;	
u8 *full_textures_tpl = NULL;
uLong full_textures_tpl_size = TEXTURE_FSIZE;
// We use zlib to compress textures at compile time
// this needs a program called cfile (source under the tools folder)
// and it must be placed in the devkitPPC bin folder when compiled		

u8 InitGX()
{	
    memset(&gp_fifo, 0, sizeof(gp_fifo));    
    GX_Init(&gp_fifo, sizeof(gp_fifo));    
	
    // Load Textures	
	full_textures_tpl = (u8 *)memalign(32, full_textures_tpl_size);
	if(!full_textures_tpl)
		return 0;
	
	if(uncompress(full_textures_tpl, &full_textures_tpl_size, textures_tpl, textures_tpl_size) != Z_OK)	
		return 0 ;	
	
	TPL_OpenTPLFromMemory(&backsTPL, (void *)full_textures_tpl, full_textures_tpl_size);			
	return 1;
}

void EndGX()
{
	if(full_textures_tpl)
	{
		free(full_textures_tpl);
		full_textures_tpl = NULL;
	}
}

void SetupGX()
{
	f32 yscale;
	u32 xfbHeight;	
    u16 xfbWidth;
	u32 width = 0;
	u32 height = 0;
	Mtx44 perspective;
	GXColor background = {0, 0, 0, 0xff};	
	
	GX_SetCopyClear(background, 0x00ffffff);
 
    GX_SetViewport(0.0F, 0.0F, rmode->fbWidth, rmode->efbHeight, 0.0F, 1.0F);	
	yscale = GX_GetYScaleFactor(rmode->efbHeight,rmode->xfbHeight);
	xfbHeight = GX_SetDispCopyYScale(yscale);
    xfbWidth = VIDEO_PadFramebufferWidth(rmode->fbWidth);  
	GX_SetScissor(0,0,rmode->fbWidth,rmode->efbHeight);
		
	GX_SetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
		
	GX_SetDispCopySrc(0,0,rmode->fbWidth,rmode->efbHeight);
	GX_SetDispCopyDst(xfbWidth, xfbHeight);
	GX_SetCopyFilter(rmode->aa, rmode->sample_pattern, Options.FlickerFilter, rmode->vfilter);
	GX_SetFieldMode(rmode->field_rendering,((rmode->viHeight==2*rmode->xfbHeight)?GX_ENABLE:GX_DISABLE));	

	GX_SetCullMode(GX_CULL_NONE);
			
	GX_SetDispCopyGamma(GX_GM_1_0);
		
	GX_SetNumChans(1);
	GX_SetNumTexGens(1);
		
	GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
	GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
		
	GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);	
		
	height = rmode->efbHeight;
	width = (vmode == VIDEO_240P || vmode == VIDEO_288P) ? 320 : xfbWidth;
	
	guOrtho(perspective,0,height,0,width,0,300);    
	GX_LoadProjectionMtx(perspective, GX_ORTHOGRAPHIC);
	GX_SetDither(GX_FALSE);
}

inline void StartScene()
{
	Mtx GXmodelView2D;		
	
	GX_SetViewport(0,0,rmode->fbWidth,rmode->efbHeight,0,1);
	GX_InvVtxCache();

	GX_ClearVtxDesc();
	GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);	
	GX_SetVtxDesc (GX_VA_CLR0, GX_DIRECT);
	GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT);
		
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

	guMtxIdentity(GXmodelView2D);
	guMtxTransApply (GXmodelView2D, GXmodelView2D, 0.0F, 0.0F, -5.0F);
	GX_LoadPosMtxImm(GXmodelView2D,GX_PNMTX0);
	
	if(Options.EnablePALBG && IsPAL)
	{
		ImagePtr back = NULL;
		
		back = LoadImage(WHITEIMG, 1);
		if(back)
		{
			back->r = Options.PalBackR; 
			back->g = Options.PalBackG;
			back->b = Options.PalBackB;
			
			DrawImage(back);
			FreeImage(&back);
		}
	}
}

inline void EndScene()
{
	DrawScanlines();	
			
	GX_SetZMode(GX_DISABLE, GX_LEQUAL, GX_FALSE);
	GX_SetBlendMode(GX_BM_BLEND,GX_BL_SRCALPHA,GX_BL_INVSRCALPHA,GX_LO_CLEAR);
	GX_SetAlphaUpdate(GX_TRUE);
	GX_SetColorUpdate(GX_TRUE);
		
	if(!DrawMenu)
	{		
		GX_CopyDisp(frameBuffer[IsPAL][ActiveFB], GX_TRUE); 		
	  
		VIDEO_Flush();                      
		VIDEO_WaitVSync();

		ActiveFB ^= 1;   
		VIDEO_SetNextFramebuffer(frameBuffer[IsPAL][ActiveFB]);   		
	}
	else
	{
		DrawMenu = 0;
		ShowMenu();
		
		VIDEO_Flush();                      
		VIDEO_WaitVSync();
	}
}

void IgnoreOffset(ImagePtr image)
{
	if(image)
		image->IgnoreOffsetY = 1;
}

ImagePtr LoadImage(int Texture, int maptoscreen)
{		
    u32 fmt = 0;
	u16 t_width = 0, t_height = 0;
	ImagePtr image;
	
	image = (ImagePtr)malloc(sizeof(struct image_st));
	if(!image)
	{
		fprintf(stderr, "Could not malloc image struct %d\n", Texture);
		return(NULL);
	}
		
	TPL_GetTexture(&backsTPL, Texture, &image->tex);	
	
	// me 
	// GX_InitTexObjLOD(&image->tex, GX_NEAR, GX_NEAR, 0.0, 10.0, 0.0, GX_FALSE, GX_FALSE, GX_ANISO_1);
	GX_InitTexObjLOD(&image->tex, Options.BilinearFiler, GX_NEAR, 0.0, 10.0, 0.0, GX_FALSE, GX_FALSE, GX_ANISO_1);
	TPL_GetTextureInfo(&backsTPL, Texture, &fmt, &t_width, &t_height);	

	image->r = 0xff;
	image->g = 0xff;
	image->b = 0xff;

	image->tw = t_width;
	image->th = t_height;
	image->x = 0;
	image->y = 0;
	image->u1 = 0.0f;
	image->v1 = 0.0f;
	image->u2 = 1.0f;
	image->v2 = 1.0f;
	image->layer = 1.0f;
	image->scale = 1;
	image->alpha = 0xff;
	image->w = image->tw;
	image->h = image->th;
	image->RefCount = 1;
	image->FH = 0;
	image->FV = 0;
	image->copyOf = NULL;
	image->cFB = NULL;
	image->IgnoreOffsetY = 0;	
	
	if(maptoscreen)
	{
		if(image->w < dW && image->w != 8)
			CalculateUV(0, 0, 320, IsPAL ? 264 : 240, image);
		else
			CalculateUV(0, 0, dW, dH, image);		
			
		IgnoreOffset(image);
	}	

	return image;
}

ImagePtr CloneImage(ImagePtr source, int maptoscreen)
{	
	ImagePtr image;
	
	if(!source)
		return NULL;
	if(source->cFB)
		return NULL;
		
	image = (ImagePtr)malloc(sizeof(struct image_st));	
	if(!image)
	{
		fprintf(stderr, "Could not malloc image struct during copy\n");
		return(NULL);
	}
	image->tex = source->tex;
	image->r = source->r;
	image->g = source->g;
	image->b = source->b;

	image->tw = source->tw;
	image->th = source->th;
	image->x = source->x;
	image->y = source->y;
	image->u1 = source->u1;
	image->v1 = source->v1;
	image->u2 = source->u2;
	image->v2 = source->v2;
	image->layer = source->layer;
	image->scale = source->scale;
	image->alpha = source->alpha;
	image->w = image->tw;
	image->h = image->th;
	image->RefCount = 0;
	image->FH = image->FH;
	image->FV = image->FV;
	image->copyOf = source;
	image->cFB = NULL;
	image->IgnoreOffsetY = source->IgnoreOffsetY;
	source->RefCount ++;	
	if(maptoscreen)
	{
		if(image->w < dW && image->w != 8)
			CalculateUV(0, 0, 320, IsPAL ? 264 : 240, image);
		else
			CalculateUV(0, 0, dW, dH, image);
	}

	return image;
}

ImagePtr CopyFrameBufferToImage()
{	
	int fbsize = 0;
	ImagePtr image;	
	u8 *cfb = NULL;	
	u32	width, height;
			
	width = rmode->fbWidth;
	height = rmode->efbHeight;
	
	fbsize = GX_GetTexBufferSize(width, height, GX_TF_RGBA8, GX_TRUE, 0);
	cfb = (u8 *)memalign(32, fbsize);
    if (!cfb)
		return NULL;
		
    GX_DrawDone();
    GX_SetCopyFilter(GX_FALSE, NULL, GX_FALSE, NULL);
    GX_SetTexCopySrc(0, 0, width, height);
	GX_SetTexCopyDst(width, height, GX_TF_RGBA8, GX_FALSE);
	GX_CopyTex(cfb, GX_TRUE);
	GX_PixModeSync();	
	DCFlushRange(cfb, fbsize);
	
	SetupGX();
	
	ActiveFB ^= 1;    		
	  
	VIDEO_Flush();                      
	VIDEO_WaitVSync();

	VIDEO_SetNextFramebuffer(frameBuffer[IsPAL][ActiveFB]);   			
	
	image = (ImagePtr)malloc(sizeof(struct image_st));
	if(!image)
	{
		free(cfb);
		fprintf(stderr, "\nCould not malloc image struct FB\n");
		return(NULL);
	}	
	
	image->cFB = cfb;
	GX_InitTexObj(&image->tex, image->cFB, width, height, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
	GX_InitTexObjLOD(&image->tex, GX_NEAR, GX_NEAR, 0.0, 10.0, 0.0, GX_FALSE, GX_FALSE, GX_ANISO_1);				

	image->r = 0xff;
	image->g = 0xff;
	image->b = 0xff;

	if(vmode == VIDEO_240P || vmode == VIDEO_288P)
		width = 320;
		
	image->tw = width;
	image->th = height;
	image->x = 0;
	image->y = 0;
	image->u1 = 0.0f;
	image->v1 = 0.0f;
	image->u2 = 1.0f;
	image->v2 = 1.0f;
	image->layer = 1.0f;
	image->scale = 0;
	image->alpha = 0xff;
	image->w = image->tw;
	image->h = image->th;
	image->RefCount = 1;
	image->FH = 0;
	image->FV = 0;
	image->copyOf = NULL;	
	IgnoreOffset(image);	
			
	return image;
}


void FreeImage(ImagePtr *image)
{	
	if(*image)
	{
		u16 *ref;

		if((*image)->copyOf)		
			ref = &((*image)->copyOf->RefCount);
		else
			ref = &((*image)->RefCount);
			
		(*ref)--;
		if(!(*ref) && !(*image)->copyOf)
		{
			if((*image)->cFB)
			{
				free((*image)->cFB);
				(*image)->cFB = NULL;
			}
			//pvr_mem_free((*image)->tex); We don't delete images from TPL
			free(*image);
			*image = NULL;
		}
	}
}

void CalculateUV(float posx, float posy, float width, float height, ImagePtr image)
{
	if(!image)
		return;
	
	// These just keep the numbers clean
	if(posx > image->tw)
		posx = posx - image->tw;
	if(posx < -1*image->tw)
		posx = posx + image->tw;

	if(posy > image->th)
		posy = posy - image->th;
	if(posy < -1*image->th)
		posy = posy + image->th;

	image->w = width;
	image->h = height;
	image->u1 = posx/image->tw;
	image->v1 = posy/image->th;
	image->u2 = (posx + width)/image->tw;
	image->v2 = (posy + height)/image->th;
}

void FlipH(ImagePtr image, u16 flip)
{
	if(!image)
		return;

	if((!image->FH && flip) || (image->FH && !flip))
	{
		float u1;
	
		u1 = image->u1;
		image->u1 = image->u2;	
		image->u2 = u1;	

		image->FH = !image->FH;
	}
}

void FlipV(ImagePtr image, u16 flip)
{
	if(!image)
		return;

	if((!image->FV && flip) || (image->FV && !flip))
	{
		float v1;
	
		v1 = image->v1;
		image->v1 = image->v2;	
		image->v2 = v1;	
		
		image->FV = !image->FV;
	}
}

void FlipHV(ImagePtr image, u16 flip)
{
	FlipH(image, flip);
	FlipV(image, flip);
}

void DrawImage(ImagePtr image)
{ 	
	float x, y, w, h; 

	
	if(!image)
		return;
	
	x = image->x;
	y = image->y;
	w = image->w;
	h = image->h;	
	
	// Center display vertically in PAL modes, since images are mostly NTSC
	if(!image->IgnoreOffsetY)
		y+= offsetY;
		
	if(image->scale && (vmode == VIDEO_480I_A240 || 
						vmode == VIDEO_480P_SL || 
						vmode == VIDEO_576I_A264))
	{
		x *= 2;
		y *= 2;
		w *= 2;
		h *= 2;
	}		
		
	GX_LoadTexObj(&image->tex, GX_TEXMAP0);	
		
	GX_Begin(GX_QUADS, GX_VTXFMT0, 4);		// Draw A Quad
		GX_Position2f32(x, y);				// Top Left
		GX_Color4u8(image->r, image->g, image->b, image->alpha);
		GX_TexCoord2f32(image->u1, image->v1);				
		
		GX_Position2f32(x+w, y);			// Top Right
		GX_Color4u8(image->r, image->g, image->b, image->alpha);
		GX_TexCoord2f32(image->u2, image->v1);		
		
		GX_Position2f32(x+w,y+h);			// Bottom Right
		GX_Color4u8(image->r, image->g, image->b, image->alpha);
		GX_TexCoord2f32(image->u2, image->v2);
		
		GX_Position2f32(x,y+h);				// Bottom Left
		GX_Color4u8(image->r, image->g, image->b, image->alpha);
		GX_TexCoord2f32(image->u1, image->v2);		
	GX_End();		
	GX_DrawDone();							// Done Drawing The Quad 
}


#define SCANSTEP 0x03
ImagePtr   scanlines = NULL;

void LoadScanlines()
{
	if(!scanlines)
	{
		scanlines = LoadImage(SCANLINESIMG, 0);
		if(!scanlines)
			return;
		scanlines->layer = 5.0;
		scanlines->alpha = 0xaa;
		scanlines->scale = 0;
		CalculateUV(0, 0, 640, 480, scanlines);
	}
}

void ToggleScanlineEvenOdd()
{
	if(scanlines)
	{
		if(scanlines->y == 0)
			scanlines->y = -1;
		else
			scanlines->y = 0;
	}
}

void RaiseScanlineIntensity()
{
	if(scanlines)
	{		
		if((int)scanlines->alpha + SCANSTEP > 0xff)
			scanlines->alpha = 0xff;
		else
			scanlines->alpha += SCANSTEP;
	}
}

void LowerScanlineIntensity()
{
	if(scanlines)
	{		
		if((int)scanlines->alpha - SCANSTEP < 0x00)
			scanlines->alpha = 0x00;
		else
			scanlines->alpha -= SCANSTEP;
	}
}

int ScanlinesEven()
{
	if(scanlines)
		return(scanlines->y == 0);
	return 0;
}

u8 GetScanlineIntensity()
{
	if(scanlines)
		return((u8)scanlines->alpha*100/0xff);
	else
		return 0;
}

u8 GetRawScanlineValue()
{
	if(scanlines)
		return((u8)scanlines->alpha);
	else
		return 0;
}

inline void DrawScanlines()
{
	if(vmode == VIDEO_480P_SL && scanlines)
		DrawImage(scanlines);
}

inline void ReleaseScanlines()
{
	FreeImage(&scanlines);
}

void SetRawScanlineIntensity(u8 intensity)
{
	if(scanlines)
	{		
		scanlines->alpha = intensity;
	}
}

void SetScanlinesEvenOrOdd(u8 Even)
{
	if(scanlines)
	{
		if(!Even)
			scanlines->y = -1;
		else
			scanlines->y = 0;
	}
}

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
#include "options.h"
#include "myTPL.h"

#ifdef DEBUG_MEM
#include "font.h"
#include <limits.h>

#define DEBUG_NUM	15

char *debug[DEBUG_NUM];
int debugnum = 0;

void debugstr(char *data)
{
	int len = 0;
	char *ndata = NULL;
	
	if(!data)
		return;
			
	len = strlen(data);
	ndata = malloc(sizeof(char)*len+1);
	if(!ndata)
		return;
	
	strcpy(ndata, data);
	if(debugnum < DEBUG_NUM)
	{
		debug[debugnum] = ndata;
		debugnum ++;
	}
	else
	{
		int i = 0;
			
		free(debug[0]);
		debug[0] = NULL;
		
		for(i = 0; i < debugnum-1; i ++)
			debug[i] = debug[i + 1];
		
		debug[debugnum-1] = ndata;
	}
}

void debugdata()
{	
	int i = 0;
	
	for(i = 0; i < debugnum; i++)
		if(debug[i]) DrawStringB(200, 20+i*fh, 0x00, 0xff, 0x00, debug[i]);   	
}
#endif

static u8 gp_fifo[GX_FIFO_MINSIZE] ATTRIBUTE_ALIGN(32);

TPLFile backsTPL;
u8 TPL_Loaded = 0;
u8 TextureMemoryMalloc = 0;
u8 *full_textures_tpl = NULL;
uLong full_textures_tpl_size = TEXTURE_FSIZE;
// We use zlib to compress textures at compile time
// this needs a program called cfile (source under the tools folder)
// and the compiled version for your OS must be placed in the tools folder

u8 LoadTextures()
{
	// Load Textures	
	if(!TextureMemoryMalloc && !full_textures_tpl)
	{
		full_textures_tpl = (u8 *)memalign(32, full_textures_tpl_size);
		if(!full_textures_tpl)
			return 0;
		TextureMemoryMalloc = 1;
	}
	
	if(uncompress(full_textures_tpl, &full_textures_tpl_size, textures_tpl, textures_tpl_size) != Z_OK)	
	{
		free(full_textures_tpl);
		full_textures_tpl = NULL;
		return 0;	
	}
	
	if(TPL_OpenTPLFromMemory(&backsTPL, (void *)full_textures_tpl, full_textures_tpl_size) != 1)
		return 0;
	
	TPL_Loaded = 1;
	return(1);
}

void CloseTextures()
{
	if(TPL_Loaded)
		TPL_CloseTPLFile(&backsTPL);
	TPL_Loaded = 0;
}

void ReleaseTextures()
{
	if(full_textures_tpl)
	{
		CloseTextures();
		free(full_textures_tpl);
		full_textures_tpl = NULL;
	}
}

u8 InitGX()
{	
	memset(&gp_fifo, 0, sizeof(gp_fifo));	 
	GX_Init(&gp_fifo, sizeof(gp_fifo));    
	
	return LoadTextures();
}

void EndGX()
{
	ReleaseTextures();
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

void StartScene()
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

void StartSceneMtx(Mtx *GXmodelView2D)
{
	GX_SetViewport(0,0,rmode->fbWidth,rmode->efbHeight,0,1);
	GX_InvVtxCache();

	GX_ClearVtxDesc();
	GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);	
	GX_SetVtxDesc (GX_VA_CLR0, GX_DIRECT);
	GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT);
		
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

	guMtxIdentity(*GXmodelView2D);
	guMtxTransApply (*GXmodelView2D, *GXmodelView2D, 0.0F, 0.0F, -5.0F);
	GX_LoadPosMtxImm(*GXmodelView2D,GX_PNMTX0);
	
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

void EndScene()
{
			
	GX_SetZMode(GX_DISABLE, GX_LEQUAL, GX_FALSE);
	GX_SetBlendMode(GX_BM_BLEND,GX_BL_SRCALPHA,GX_BL_INVSRCALPHA,GX_LO_CLEAR);
	GX_SetAlphaUpdate(GX_TRUE);
	GX_SetColorUpdate(GX_TRUE);
		
	GX_CopyDisp(frameBuffer[IsPAL][ActiveFB], GX_TRUE); 		
  
	VIDEO_Flush();						
	VIDEO_WaitVSync();

	ActiveFB ^= 1;	 
	VIDEO_SetNextFramebuffer(frameBuffer[IsPAL][ActiveFB]);   		
}

void IgnoreOffset(ImagePtr image)
{
	if(image)
		image->IgnoreOffsetY = 1;
}

ImagePtr LoadImageMemCpy(int Texture, int maptoscreen)
{		
	u16 t_width = 0, t_height = 0;
	void *texture = NULL;
	ImagePtr image;
	
	if(!TPL_Loaded)
		return NULL;

	image = (ImagePtr)malloc(sizeof(struct image_st));
	if(!image)
	{
		fprintf(stderr, "Could not malloc image struct %d\n", Texture);
		return(NULL);
	}
	memset(image, 0, sizeof(struct image_st));
		
	TPL_GetTextureMEMCopy(&backsTPL, Texture, &image->tex, &texture);	
	
	//GX_InitTexObjLOD(&image->tex, Options.BilinearFiler, GX_NEAR, 0.0, 10.0, 0.0, GX_FALSE, GX_FALSE, GX_ANISO_1);
	//GX_InitTexObjLOD(&image->tex, GX_NEAR, GX_NEAR, 0.0, 10.0, 0.0, GX_FALSE, GX_FALSE, GX_ANISO_1);
	TPL_GetTextureInfo(&backsTPL, Texture, NULL, &t_width, &t_height);

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
	image->FH = 0;
	image->FV = 0;
	image->cFB = NULL;
	image->memCpyTexture = texture;
	image->IgnoreOffsetY = 0;	
	
	if(maptoscreen)
		MapToScreen(image);

	return image;
}

ImagePtr LoadImage(int Texture, int maptoscreen)
{		
	u16 t_width = 0, t_height = 0;
	ImagePtr image;
	
	if(!TPL_Loaded)
		return NULL;
	
	image = (ImagePtr)malloc(sizeof(struct image_st));
	if(!image)
	{
		fprintf(stderr, "Could not malloc image struct %d\n", Texture);
		return(NULL);
	}
	memset(image, 0, sizeof(struct image_st));
		
	TPL_GetTexture(&backsTPL, Texture, &image->tex);	
	
	//GX_InitTexObjLOD(&image->tex, Options.BilinearFiler, GX_NEAR, 0.0, 10.0, 0.0, GX_FALSE, GX_FALSE, GX_ANISO_1);
	//GX_InitTexObjLOD(&image->tex, GX_NEAR, GX_NEAR, 0.0, 10.0, 0.0, GX_FALSE, GX_FALSE, GX_ANISO_1);
	TPL_GetTextureInfo(&backsTPL, Texture, NULL, &t_width, &t_height);

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
	image->FH = 0;
	image->FV = 0;
	image->cFB = NULL;
	image->memCpyTexture = NULL;
	image->IgnoreOffsetY = 0;	
	
	if(maptoscreen)
		MapToScreen(image);

	return image;
}

void MapToScreen(ImagePtr image)
{
	if(!image)
		return;
		
	if(image->w < dW && image->w != 8)
		CalculateUV(0, 0, 320, IsPAL ? 264 : 240, image);
	else
		CalculateUV(0, 0, dW, dH, image);		
		
	IgnoreOffset(image);
}

ImagePtr CopyFrameBufferToImage()
{	
	int fbsize = 0;
	ImagePtr image;	
	u8 *cfb = NULL;	
	u32	width, height;
			
	width = rmode->fbWidth;
	height = rmode->efbHeight;
	fbsize = GX_GetTexBufferSize(width, height, GX_TF_RGBA8, GX_FALSE, 0);
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
	memset(image, 0, sizeof(struct image_st));
	
	image->cFB = cfb;
	GX_InitTexObj(&image->tex, image->cFB, width, height, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
	GX_InitTexObjFilterMode(&image->tex, GX_NEAR, GX_NEAR);

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
	image->FH = 0;
	image->FV = 0;
	image->memCpyTexture = NULL;
	IgnoreOffset(image);	
			
	return image;
}


void FreeImage(ImagePtr *image)
{	
	if(!(*image))
		return;

	if(*image)
	{
		if((*image)->cFB)
		{
			free((*image)->cFB);
			(*image)->cFB = NULL;
		}
		if((*image)->memCpyTexture)
		{
			free((*image)->memCpyTexture);
			(*image)->memCpyTexture = NULL;
		}

		free(*image);
		*image = NULL;
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

void DrawImageRotate(ImagePtr image, float angle, Mtx *m)
{ 	
	float 		x, y, w, h; 	
	guVector 	axis;

	
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

	axis.x = 0;
	axis.y = 0;
	axis.z = 1.0f;
	guMtxRotAxisDeg(*m, &axis, angle);
	guMtxTransApply(*m, *m, x + (w/2), y + (h/2), 0.0f);
	GX_LoadPosMtxImm(*m, GX_PNMTX0);	
		
	GX_LoadTexObj(&image->tex, GX_TEXMAP0);	
		
	GX_Begin(GX_QUADS, GX_VTXFMT0, 4);		// Draw A Quad
		GX_Position2f32(-(w/2), -(h/2));				// Top Left
		GX_Color4u8(image->r, image->g, image->b, image->alpha);
		GX_TexCoord2f32(image->u1, image->v1);				
		
		GX_Position2f32(w/2, -(h/2));			// Top Right
		GX_Color4u8(image->r, image->g, image->b, image->alpha);
		GX_TexCoord2f32(image->u2, image->v1);		
		
		GX_Position2f32(w/2, h/2);			// Bottom Right
		GX_Color4u8(image->r, image->g, image->b, image->alpha);
		GX_TexCoord2f32(image->u2, image->v2);
		
		GX_Position2f32(-(w/2), h/2);				// Bottom Left
		GX_Color4u8(image->r, image->g, image->b, image->alpha);
		GX_TexCoord2f32(image->u1, image->v2);		
	GX_End();		
	GX_DrawDone();							// Done Drawing The Quad 
}

void SetTextureColor(ImagePtr tex, u8 r, u8 g, u8 b)
{
	tex->r = r;
	tex->g = g;
	tex->b = b;
}


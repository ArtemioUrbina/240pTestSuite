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
#ifndef IMAGES_H
#define IMAGES_H

#include <string.h>
#include <malloc.h>
#include <zlib.h>
#include <ogc/tpl.h>

#include "textures_tpl.h"
#include "textures.h"
#include "comp_textures.h"

#include "video.h"

typedef struct image_st * ImagePtr;

struct image_st{
		GXTexObj tex;
		float   x;
		float   y;
		float   layer;    
		float	u1;
		float	v1;
		float	u2;
		float	v2;
		float  	w;	
		float  	h;
		float  	tw;
		float  	th; 
		u8  	r;	
		u8  	g;   
		u8  	b;   
		u8		alpha;
		u32 	scale;
		u16 	FH;
		u16 	FV;

		u16     	RefCount;
		ImagePtr  	copyOf;
		u8			*cFB;
};

/***********************/
/*    Base Functions   */
/***********************/

u8 InitGX();
void SetupGX();
void StartScene();
void EndScene();
void EndSceneScreenShot();
void EndGX();

/************************/
/*    Image Functions   */
/************************/

ImagePtr LoadImage(int texture, int maptoscreen);
ImagePtr CloneImage(ImagePtr source, int maptoscreen);
ImagePtr CopyFrameBufferToImage();
void FreeImage(ImagePtr *image);
void CalculateUV(float posx, float posy, float width, float height, ImagePtr image);
void FlipH(ImagePtr image, u16 flip);
void FlipV(ImagePtr image, u16 flip);
void FlipHV(ImagePtr image, u16 flip);
void DrawImage(ImagePtr image);

/************************/
/*    Scanline Functions   */
/************************/

extern ImagePtr scanlines;

void LoadScanlines();
inline void ReleaseScanlines();
inline void DrawScanlines();
u8 GetScanlineIntensity();
int ScanlinesEven();
void LowerScanlineIntensity();
void RaiseScanlineIntensity();
void ToggleScanlineEvenOdd();

u8 GetRawScanlineValue();
void SetRawScanlineIntensity(u8 intensity);
void SetScanlinesEvenOrOdd(u8 Even);


#endif
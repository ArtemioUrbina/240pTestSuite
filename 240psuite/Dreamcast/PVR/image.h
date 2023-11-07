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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef IMAGES_H
#define IMAGES_H

#ifdef USE_PNG
#include <png/png.h>
#endif
	
typedef struct image_st * ImagePtr;

struct image_st{
		pvr_ptr_t tex;
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
		float  	r;	
		float  	g;   
		float  	b;   
		float   alpha;
		uint32  scale;
		uint16  FH;
		uint16  FV;
		uint8	IgnoreOffsetY;
		uint32	texFormat;
		
		uint8	use_direct_color;
		uint16	a_direct;
		uint16	r_direct;
		uint16	g_direct;
		uint16	b_direct;
};

#define MAX_IMAGES	50
#define PATH_LEN	150

#define PACK_ARGB8888(a,r,g,b) ( ((a & 0xFF) << 24) | ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF) )

//states
#define MEM_RELEASED	0
#define MEM_LOADED		1
#define MEM_TEXRELEASED	2
#define MEM_ERROR		3

typedef struct image_mem_st ImageMem;

struct image_mem_st{
	ImagePtr	image;
	char		name[PATH_LEN];
	uint8		state;
};

extern ImageMem Images[MAX_IMAGES];

void InitImages();
void CleanImages();
void RefreshLoadedImages();
void InsertImage(ImagePtr image, char *name);
void ReleaseTextures();
void IgnoreOffset(ImagePtr image);
void UseDirectColor(ImagePtr image, uint16 a, uint16 r, uint16 g, uint16 b);

ImagePtr LoadIMG(const char *filename, int maptoscreen);
uint8 ReLoadIMG(ImagePtr image, const char *filename);
void FreeImage(ImagePtr *image);
uint8 FreeImageData(ImagePtr *image);
void CalculateUV(float posx, float posy, float width, float height, ImagePtr image);
void FlipH(ImagePtr image, uint16 flip);
void FlipV(ImagePtr image, uint16 flip);
void FlipHV(ImagePtr image, uint16 flip);
void DrawImage(ImagePtr image);
void DrawImageRotate(ImagePtr image, float angle);
void StartScene();
void EndScene();

typedef struct dpallette_st {
	uint32_t	numcolors;
	uint32_t	*colors;
} pallette;

int load_palette(const char *fn, pallette *pal);
void release_palette(pallette *pal);
void set_palette(pallette *pal);
void clear_palette();

#endif

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

#include <png/png.h>

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

		uint16    RefCount;
		ImagePtr  copyOf;
};

#define MAX_IMAGES	50
#define PATH_LEN	150

//states
#define MEM_RELEASED 	0
#define MEM_LOADED	1
#define MEM_TEXRELEASED	2

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

ImagePtr LoadKMG(const char *filename, int maptoscreen);
uint8 ReLoadKMG(ImagePtr image, const char *filename);
ImagePtr CloneImage(ImagePtr source, int maptoscreen);
void FreeImage(ImagePtr *image);
uint8 FreeImageData(ImagePtr *image);
void CalculateUV(float posx, float posy, float width, float height, ImagePtr image);
void FlipH(ImagePtr image, uint16 flip);
void FlipV(ImagePtr image, uint16 flip);
void FlipHV(ImagePtr image, uint16 flip);
void DrawImage(ImagePtr image);
inline void StartScene();
inline void EndScene();

#endif

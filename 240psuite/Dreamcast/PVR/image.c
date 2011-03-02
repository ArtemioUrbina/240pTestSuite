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

#include "image.h"
#include "vmodes.h"

//#define BENCHMARK 

ImagePtr LoadImage(const char *filename, int maptoscreen)
{
	uint32	w, h;
	ImagePtr image;
#ifdef BENCHMARK
	uint32 start, end;
	char msg[100];
		
	timer_ms_gettime(NULL, &start);	
#endif
	image = (ImagePtr)malloc(sizeof(struct image_st));
	if(!image)
	{
		fprintf(stderr, "Could not malloc image struct %s\n", filename);
		return(NULL);
	}
	if(png_load_texture(filename, &image->tex, PNG_FULL_ALPHA, &w, &h) == -1)
	{
		free(image);
		fprintf(stderr, "Could not load %s\n", filename);
		return(NULL);
	} 		
#ifdef BENCHMARK
	timer_ms_gettime(NULL, &end);
	sprintf(msg, "PNG %s took %lu ms\n", filename, (unsigned int)end - start);
	dbglog(DBG_KDEBUG, msg);	
#endif

	image->r = 1.0f;
	image->g = 1.0f;
	image->b = 1.0f;

	image->tw = w;
	image->th = h;
	image->x = 0;
	image->y = 0;
	image->u1 = 0.0f;
	image->v1 = 0.0f;
	image->u2 = 1.0f;
	image->v2 = 1.0f;
	image->layer = 1.0f;
	image->scale = 1;
	image->alpha = 1.0;
	image->w = image->tw;
	image->h = image->th;
	image->RefCount = 1;
	image->FH = 0;
	image->FV = 0;
	image->copyOf = NULL;
	if(maptoscreen)
	{
		if(image->w < dW && image->w != 8)
			CalculateUV(0, 0, 320, 240, image);
		else
			CalculateUV(0, 0, dW, dH, image);
	}

	return image;
}

ImagePtr LoadKMG(const char *filename, int maptoscreen)
{	
	ImagePtr image;
	kos_img_t img;
#ifdef BENCHMARK
	uint32 start, end;
	char	msg[100];
		
	timer_ms_gettime(NULL, &start);
#endif
	image = (ImagePtr)malloc(sizeof(struct image_st));
	if(!image)
	{
		fprintf(stderr, "Could not malloc image struct %s\n", filename);
		return(NULL);
	}
	
	if(kmg_to_img(filename, &img) != 0)
	{
		free(image);
		fprintf(stderr, "Could not load %s\n", filename);
		return(NULL);
	}

	image->tex = pvr_mem_malloc(img.byte_count);
	if(!image->tex)
	{
		free(image);
		fprintf(stderr, "Could not load %s to VRAM\n", filename);
		return(NULL);
	}

	pvr_txr_load_kimg(&img, image->tex, 0);
	kos_img_free(&img, 0);	

#ifdef BENCHMARK
	timer_ms_gettime(NULL, &end);
	sprintf(msg, "KMG %s took %lu ms\n", filename, (unsigned int)end - start);
	dbglog(DBG_KDEBUG, msg);
#endif

	image->r = 1.0f;
	image->g = 1.0f;
	image->b = 1.0f;

	image->tw = img.w;
	image->th = img.h;
	image->x = 0;
	image->y = 0;
	image->u1 = 0.0f;
	image->v1 = 0.0f;
	image->u2 = 1.0f;
	image->v2 = 1.0f;
	image->layer = 1.0f;
	image->scale = 1;
	image->alpha = 1.0;
	image->w = image->tw;
	image->h = image->th;
	image->RefCount = 1;
	image->FH = 0;
	image->FV = 0;
	image->copyOf = NULL;
	if(maptoscreen)
	{
		if(image->w < dW && image->w != 8)
			CalculateUV(0, 0, 320, 240, image);
		else
			CalculateUV(0, 0, dW, dH, image);
	}

	return image;
}

ImagePtr CloneImage(ImagePtr source, int maptoscreen)
{	
	ImagePtr image;
	
	if(!source)
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
	source->RefCount ++;
	if(maptoscreen)
	{
		if(image->w < dW && image->w != 8)
			CalculateUV(0, 0, 320, 240, image);
		else
			CalculateUV(0, 0, dW, dH, image);
	}

	return image;
}

void FreeImage(ImagePtr *image)
{	
	if(*image)
	{
		uint16 *ref;

		if((*image)->copyOf)		
			ref = &((*image)->copyOf->RefCount);
		else
			ref = &((*image)->RefCount);
			
		(*ref)--;
		if(!(*ref) && !(*image)->copyOf)
		{
			pvr_mem_free((*image)->tex);
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

	image->w = width;
	image->h = height;
	image->u1 = posx/image->tw;
	image->v1 = posy/image->th;
	image->u2 = (posx + width)/image->tw;
	image->v2 = (posy + height)/image->th;
}

void FlipH(ImagePtr image, uint16 flip)
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

void FlipV(ImagePtr image, uint16 flip)
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

void FlipHV(ImagePtr image, uint16 flip)
{
	FlipH(image, flip);
	FlipV(image, flip);
}

void DrawImage(ImagePtr image)
{ 	
	float x, y, w, h; 

	pvr_poly_cxt_t cxt;
	pvr_poly_hdr_t hdr;
	pvr_vertex_t vert;
	
	if(!image)
		return;

	x = image->x;
	y = image->y;
	w = image->w;
	h = image->h;
	
	if(image->scale && (vmode == FAKE_640 || vmode == FAKE_640_SL))
	{
		x *= 2;
		y *= 2;
		w *= 2;
		h *= 2;
	}
		
	pvr_poly_cxt_txr(&cxt, PVR_LIST_TR_POLY, PVR_TXRFMT_ARGB1555, image->tw, image->th, image->tex, PVR_FILTER_NONE);
	pvr_poly_compile(&hdr, &cxt);
	pvr_prim(&hdr, sizeof(hdr));

	vert.argb = PVR_PACK_COLOR(image->alpha, image->r, image->g, image->b);		
	vert.oargb = 0;
	vert.flags = PVR_CMD_VERTEX;
	
	vert.x = x;
	vert.y = y;
	vert.z = image->layer;
	vert.u = image->u1;
	vert.v = image->v1;
	pvr_prim(&vert, sizeof(vert));
	
	vert.x = x + w;
	vert.y = y;
	vert.z = image->layer;
	vert.u = image->u2;
	vert.v = image->v1;
	pvr_prim(&vert, sizeof(vert));
	
	vert.x = x;
	vert.y = y + h;
	vert.z = image->layer;
	vert.u = image->u1;
	vert.v = image->v2;
	pvr_prim(&vert, sizeof(vert));
	
	vert.x = x + w;
	vert.y = y + h;
	vert.z = image->layer;
	vert.u = image->u2;
	vert.v = image->v2;
	vert.flags = PVR_CMD_VERTEX_EOL;
	pvr_prim(&vert, sizeof(vert));
}


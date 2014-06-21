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
#include "help.h"

extern uint16 DrawMenu;
ImageMem Images[MAX_IMAGES];
uint8 UsedImages = 0;

//#define BENCHMARK 

void InitImages()
{
	uint8	i = 0;
	for(i = 0; i < MAX_IMAGES; i++)
	{
		Images[i].image = NULL;
		Images[i].state = MEM_RELEASED;
	}
	UsedImages = 0;
}

void CleanImages()
{
	uint8	i = 0;
	for(i = 0; i < MAX_IMAGES; i++)
	{
		if(Images[i].state != MEM_RELEASED)
		{
			dbglog(DBG_ERROR, "Found unreleased image %s\n", Images[i].name);
			FreeImage(&Images[i].image);
			Images[i].state = MEM_RELEASED;
			Images[i].name[0] = '\0';
			if(UsedImages)
				UsedImages --;
			else
				dbglog(DBG_ERROR, "Invalid used image index\n");
		}	
	}
}

void RefreshLoadedImages()
{
	uint8	i = 0;

	for(i = 0; i < MAX_IMAGES; i++)
	{
		if(Images[i].state == MEM_TEXRELEASED)
		{
			if(Images[i].name[0] == 'F' && Images[i].name[1] == 'B')
			{
				if(!ReloadFBTexture())
					dbglog(DBG_ERROR, "Could not reload FrameBuffer\n");
				else
					Images[i].state = MEM_LOADED;
			}
			else
			{
				if(ReLoadKMG(Images[i].image, Images[i].name))
					Images[i].state = MEM_LOADED;
			}
		}	
	}
}

void ReleaseTextures()
{
	uint8	i = 0;

	for(i = 0; i < MAX_IMAGES; i++)
	{
		if(Images[i].state == MEM_LOADED)
		{
			if(!FreeImageData(&Images[i].image))
				dbglog(DBG_ERROR, "Could not free image: %s\n", Images[i].name);
			else
				Images[i].state = MEM_TEXRELEASED;
		}	
	}
}

void InsertImage(ImagePtr image, char *name)
{
	if(UsedImages < MAX_IMAGES)
	{
		uint8 i, placed = 0;

		for(i = 0; i < MAX_IMAGES && !placed; i++)
		{
			if(Images[i].state == MEM_RELEASED)
			{
				Images[i].image = image;
				Images[i].state = MEM_LOADED;
				strncpy(Images[i].name, name, PATH_LEN);
				UsedImages ++;
				placed = 1;
			}
		}
		if(!placed)
			dbglog(DBG_ERROR, "Image array full (NO FREE)\n");
	}
	else
		dbglog(DBG_ERROR, "Image array full\n");
}

void ReleaseImage(ImagePtr image)
{
	uint8   i = 0, deleted = 0;

        for(i = 0; i < MAX_IMAGES && !deleted; i++)
        {
		if(Images[i].image == image)
		{
			Images[i].image = NULL;
			Images[i].state = MEM_RELEASED;
                        Images[i].name[0] = '\0';
			if(UsedImages)
				UsedImages --;
			else
				dbglog(DBG_ERROR, "Invalid used image index\n");
			deleted = 1;
		}
	}

	if(!deleted)
		dbglog(DBG_ERROR, "Image not found for deletion\n");
}

int gkmg_to_img(const char * fn, kos_img_t * rv) {	
	kmg_header_t	hdr;
	int		dep;	
	//int length = 0;
	gzFile f;		

	assert( rv != NULL );

	/* Open the file */
	//length = zlib_getlength((char *)fn);	
	f = gzopen(fn, "r");
	if (!f) {		
		dbglog(DBG_ERROR, "gkmg_to_img: can't open file '%s'\n", fn);
		return -1;
	}  
		
	/* Read the header */
	if (gzread(f, &hdr, sizeof(hdr)) != sizeof(hdr)) {
		gzclose(f);
		dbglog(DBG_ERROR, "gkmg_to_img: can't read header from file '%s'\n", fn);
		return -2;
	}

	/* Verify a few things */
	if (hdr.magic != KMG_MAGIC || hdr.version != KMG_VERSION ||
		hdr.platform != KMG_PLAT_DC)
	{
		gzclose(f);
		dbglog(DBG_ERROR, "gkmg_to_img: file '%s' is incompatible:\n"
			"   magic %08lx version %d platform %d\n",
			fn, hdr.magic, (int)hdr.version, (int)hdr.platform);
		return -3;
	}
	
	/* Setup the kimg struct */
	rv->w = hdr.width;
	rv->h = hdr.height;

	dep = 0;
	if (hdr.format & KMG_DCFMT_VQ)
		dep |= PVR_TXRLOAD_FMT_VQ;
	if (hdr.format & KMG_DCFMT_TWIDDLED)
		dep |= PVR_TXRLOAD_FMT_TWIDDLED;

	switch (hdr.format & KMG_DCFMT_MASK) {
	case KMG_DCFMT_RGB565:
		rv->fmt = KOS_IMG_FMT(KOS_IMG_FMT_RGB565, dep);
		break;

	case KMG_DCFMT_ARGB4444:
		rv->fmt = KOS_IMG_FMT(KOS_IMG_FMT_ARGB4444, dep);
		break;

	case KMG_DCFMT_ARGB1555:
		rv->fmt = KOS_IMG_FMT(KOS_IMG_FMT_ARGB1555, dep);
		break;

	case KMG_DCFMT_YUV422:
		rv->fmt = KOS_IMG_FMT(KOS_IMG_FMT_YUV422, dep);
		break;

	case KMG_DCFMT_BUMP:
		/* XXX */
		rv->fmt = KOS_IMG_FMT(KOS_IMG_FMT_RGB565, dep);
		break;

	case KMG_DCFMT_4BPP_PAL:
	case KMG_DCFMT_8BPP_PAL:
	default:
		assert_msg( 0, "currently-unsupported KMG pixel format" );
		gzclose(f);
		if(rv->data)
			free(rv->data);
		return -5;
	}
	
	rv->byte_count = hdr.byte_count;

	/* And load the rest  */
	
	rv->data = malloc(hdr.byte_count);
	if (!rv->data) {
		dbglog(DBG_ERROR, "gkmg_to_img: can't malloc(%d) while loading '%s'\n",
			(int)hdr.byte_count, fn);
		gzclose(f);
		return -4;
	}
	if (gzread(f, rv->data, rv->byte_count) != rv->byte_count) {
		dbglog(DBG_ERROR, "gkmg_to_img: can't read %d bytes while loading '%s'\n",
			(int)hdr.byte_count, fn);
		gzclose(f);
		free(rv->data);
		return -6;
	}

	/* Ok, all done */
	gzclose(f);

	/* If the byte count is not a multiple of 32, bump it up as well.
		 This is for DMA/SQ usage. */
	rv->byte_count = (rv->byte_count + 31) & ~31;
	
	return 0;
}

void IgnoreOffset(ImagePtr image)
{
	if(image)
		image->IgnoreOffsetY = 1;
}

ImagePtr LoadKMG(const char *filename, int maptoscreen)
{	
	int load, len;
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
	
	len = strlen(filename);
	if(filename[len - 2] == 'g' && filename[len - 1] == 'z')
		load = gkmg_to_img(filename, &img);
	else
		load = kmg_to_img(filename, &img);
	if(load != 0)
	{
		free(image);
		fprintf(stderr, "Could not load %s\n", filename);
		return(NULL);
	}

	image->tex = pvr_mem_malloc(img.byte_count);
	if(!image->tex)
	{
		free(image);
		kos_img_free(&img, 0);	
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
	image->FH = 0;
	image->FV = 0;
	image->texFormat = PVR_TXRFMT_ARGB1555;
	image->IgnoreOffsetY = 0;

	if(maptoscreen)
	{
		if(image->w < dW && image->w != 8)
			CalculateUV(0, 0, 320, IsPAL ? 264 : 240, image);
		else
			CalculateUV(0, 0, dW, dH, image);

		IgnoreOffset(image);
	}

	InsertImage(image, (char*)filename);

	return image;
}

uint8 ReLoadKMG(ImagePtr image, const char *filename)
{	
	int load, len;
	kos_img_t img;
#ifdef BENCHMARK
	uint32 start, end;
	char	msg[100];
		
	timer_ms_gettime(NULL, &start);
#endif
	if(!image)
	{
		fprintf(stderr, "Invalid image pointer for reload %s\n", filename);
		return 0;
	}
	
	if(image->tex)
	{
		fprintf(stderr, "Found unreleased texture while reloading %s\n", filename);
		return 0;
	}

	len = strlen(filename);
	if(filename[len - 2] == 'g' && filename[len - 1] == 'z')
		load = gkmg_to_img(filename, &img);
	else
		load = kmg_to_img(filename, &img);
	if(load != 0)
	{
		fprintf(stderr, "Could not load %s\n", filename);
		return 0;
	}

	image->tex = pvr_mem_malloc(img.byte_count);
	if(!image->tex)
	{
		fprintf(stderr, "Could not load %s to VRAM\n", filename);
		kos_img_free(&img, 0);	
		return 0;
	}

	pvr_txr_load_kimg(&img, image->tex, 0);
	kos_img_free(&img, 0);	

#ifdef BENCHMARK
	timer_ms_gettime(NULL, &end);
	sprintf(msg, "KMG %s took %lu ms\n", filename, (unsigned int)end - start);
	dbglog(DBG_KDEBUG, msg);
#endif
	return 1;
}

void FreeImage(ImagePtr *image)
{	
	if(*image)
	{
		ReleaseImage(*image);
		FreeImageData(image);
		free(*image);
		*image = NULL;
	}
}

uint8 FreeImageData(ImagePtr *image)
{	
	if(*image)
	{
		if((*image)->tex)
		{
			pvr_mem_free((*image)->tex);
			(*image)->tex = NULL;
			return 1;
		}
	}
	return 0;
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
	
	if(!image || !image->tex)
		return;

	x = image->x;
	y = image->y;
	w = image->w;
	h = image->h;
	
	// Center display vertically in PAL modes, since images are mostly NTSC
	if(!image->IgnoreOffsetY)
		y+= offsetY;

	if(image->scale && (vmode == VIDEO_480P_SL
			|| vmode == VIDEO_480I_A240
			|| vmode == VIDEO_576I_A264))
	{
		x *= 2;
		y *= 2;
		w *= 2;
		h *= 2;
	}
		
	pvr_poly_cxt_txr(&cxt, PVR_LIST_TR_POLY, image->texFormat, image->tw, image->th, image->tex, PVR_FILTER_NONE);
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

inline void StartScene()
{
	pvr_scene_begin();
	pvr_list_begin(PVR_LIST_TR_POLY);

	/*
	if(IsPAL)
        {
                ImagePtr back = NULL;

		back = LoadKMG("/rd/white.kmg.gz", 1);
                if(back)
                {
                        back->r = 0.4f;
                        back->g = 0.4f;
                        back->b = 0.4f;

                        DrawImage(back);
                        FreeImage(&back);
                }
        }
	*/
}

inline void EndScene()
{
	DrawScanlines();
	pvr_list_finish();				
	pvr_scene_finish();
	pvr_wait_ready();

	//PVRStats("EndFrame");
	if(DrawMenu)
	{
		DrawMenu = 0;
		DrawShowMenu();
	}
}

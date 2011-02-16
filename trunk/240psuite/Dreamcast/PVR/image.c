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

#include <kos.h>
#include "image.h"
#include "vmodes.h"

ImagePtr LoadImage(const char *filename)
{
    ImagePtr image;
    
    image = (ImagePtr)malloc(sizeof(struct image_st));
    //  PNG_MASK_ALPHA PNG_NO_ALPHA PNG_FULL_ALPHA
    if(png_load_texture(filename, &image->tex, PNG_MASK_ALPHA, &image->tw, &image->th) == -1)
    {
        free(image);
        fprintf(stderr, "Could not load %s\n", filename);
        return(NULL);
    }   

    image->x = 0;
    image->y = 0;
    image->layer = 1;
    image->scale = 1;
    image->alpha = 1.0;
    image->w = image->tw;
    image->h = image->th;

    return image;
}

void FreeImage(ImagePtr *image)
{
    if(*image)
    {
    	pvr_mem_free((*image)->tex);
    	free(*image);
    	*image = NULL;
    }
}

void DrawImage(ImagePtr image)
{    
    pvr_poly_cxt_t cxt;
    pvr_poly_hdr_t hdr;
    pvr_vertex_t vert;
    
    if(!image)
        return;

    uint32 x, y, w, h;

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
    	
    // PVR_TXRFMT_ARGB1555 PVR_TXRFMT_RGB565 PVR_TXRFMT_ARGB4444		
    pvr_poly_cxt_txr(&cxt, PVR_LIST_TR_POLY, PVR_TXRFMT_ARGB1555, image->tw, image->th, image->tex, PVR_FILTER_NONE);
    pvr_poly_compile(&hdr, &cxt);
    pvr_prim(&hdr, sizeof(hdr));

    vert.argb = PVR_PACK_COLOR(image->alpha, 1.0f, 1.0f, 1.0f);    
    vert.oargb = 0;
    vert.flags = PVR_CMD_VERTEX;
    
    vert.x = x;
    vert.y = y;
    vert.z = image->layer;
    vert.u = 0.0;
    vert.v = 0.0;
    pvr_prim(&vert, sizeof(vert));
    
    vert.x = x + w;
    vert.y = y;
    vert.z = image->layer;
    vert.u = 1.0;
    vert.v = 0.0;
    pvr_prim(&vert, sizeof(vert));
    
    vert.x = x;
    vert.y = y + h;
    vert.z = image->layer;
    vert.u = 0.0;
    vert.v = 1.0;
    pvr_prim(&vert, sizeof(vert));
    
    vert.x = x + w;
    vert.y = y + h;
    vert.z = image->layer;
    vert.u = 1.0;
    vert.v = 1.0;
    vert.flags = PVR_CMD_VERTEX_EOL;
    pvr_prim(&vert, sizeof(vert));
}


/* 
 * 240p Test Suite
 * Copyright (C)2011 Artemio Urbina
 *
 * This file is part of the 240p Test Suite
 *
 * To Eleven is free software; you can redistribute it and/or modify
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
#include "font.h"
#include "image.h"
#include "vmodes.h"

plx_font_t *font = NULL;
plx_fcxt_t *font_cxt = NULL;

float f_size = 7.5f;

float fw = 7.0f;
float fh = 9.0f;

ImagePtr   black_t = NULL;

void LoadFont()
{
    if(!font)
    {
	    font = plx_font_load("/rd/font.txf");
	    font_cxt = plx_fcxt_create(font, PVR_LIST_TR_POLY);
    }

    if(!black_t)
	    black_t = LoadImage("/rd/black.png");
}

void ReleaseFont()
{
	if(font_cxt)
	{
		plx_fcxt_destroy(font_cxt);
		font_cxt = NULL;
	}
	if(font)
	{
		plx_font_destroy(font);
		font = NULL;
	}
	if(black_t)
	{
		FreeImage(&black_t);
		black_t = NULL;
	}
}

void DrawString(float x, float y, float r, float g, float b, char *str) 
{
	point_t pos;

	pos.x = x;
	pos.y = y;
	pos.z = 3.0f;

	plx_fcxt_begin(font_cxt);

	if(vmode == FAKE_640 || vmode == FAKE_640_SL)
	{
		x *= 2;
		y *= 2;
		plx_fcxt_setsize(font_cxt, f_size*2);
	}
	else
		plx_fcxt_setsize(font_cxt, f_size);

	plx_fcxt_setpos_pnt(font_cxt, &pos);
	plx_fcxt_setcolor4f(font_cxt, 1.0f, r, g, b);
	plx_fcxt_draw(font_cxt, str);

	plx_fcxt_end(font_cxt);
}

void DrawStringS(float x, float y, float r, float g, float b, char *str) 
{
	int c = 1;
	point_t pos;

	plx_fcxt_begin(font_cxt);

	if(vmode == FAKE_640 || vmode == FAKE_640_SL)
	{
		x *= 2;
		y *= 2;
		c *= 2;
		plx_fcxt_setsize(font_cxt, f_size*2);
	}
	else
		plx_fcxt_setsize(font_cxt, f_size);

	pos.x = x+c;
	pos.y = y+c;
	pos.z = 2.0f;
	plx_fcxt_setpos_pnt(font_cxt, &pos);
	plx_fcxt_setcolor4f(font_cxt, 0.8f, 0, 0, 0);
	plx_fcxt_draw(font_cxt, str);

	pos.x = x;
	pos.y = y;
	pos.z = 3.0f;
	plx_fcxt_setpos_pnt(font_cxt, &pos);
	plx_fcxt_setcolor4f(font_cxt, 1.0f, r, g, b);
	plx_fcxt_draw(font_cxt, str);

	plx_fcxt_end(font_cxt);
}

void DrawStringB(float x, float y, float r, float g, float b, char *str) 
{
	float left, up, right, down, c = 1;
	pvr_poly_cxt_t cxt;
	pvr_poly_hdr_t hdr;
	pvr_vertex_t vert;
	point_t pos;


	plx_fcxt_begin(font_cxt);

	if(vmode == FAKE_640 || vmode == FAKE_640_SL)
	{
		x *= 2;
		y *= 2;
		c *= 2;
		plx_fcxt_setsize(font_cxt, f_size*2);
	}
	else
		plx_fcxt_setsize(font_cxt, f_size);

	pos.x = x;
	pos.y = y;
	pos.z = 3.0f;

	plx_fcxt_setpos_pnt(font_cxt, &pos);
	plx_fcxt_setcolor4f(font_cxt, 1.0f, r, g, b);

	plx_fcxt_str_metrics(font_cxt, str, &left, &up, &right, &down);
	plx_fcxt_draw(font_cxt, str);

	plx_fcxt_end(font_cxt);

	if(!black_t)
		return;

	pvr_poly_cxt_txr(&cxt, PVR_LIST_TR_POLY, PVR_TXRFMT_ARGB1555,
		      black_t->tw, black_t->th, black_t->tex, PVR_FILTER_NONE);
	pvr_poly_compile(&hdr, &cxt);
	pvr_prim(&hdr, sizeof(hdr));

	vert.argb = PVR_PACK_COLOR(black_t->alpha, 1.0f, 1.0f, 1.0f);
	vert.oargb = 0;
	vert.flags = PVR_CMD_VERTEX;

	left = x - c;
	right += x + c;
	up = y - fh * c;
	down += y + c;

	vert.x = left;
	vert.y = up;
	vert.z = 2.0f;
	vert.u = 0.0;
	vert.v = 0.0;
	pvr_prim(&vert, sizeof(vert));

	vert.x = right;
	vert.y = up;
	vert.z = 2.0f;
	vert.u = 1.0;
	vert.v = 0.0;
	pvr_prim(&vert, sizeof(vert));

	vert.x = left;
	vert.y = down;
	vert.z = 2.0f;
	vert.u = 0.0;
	vert.v = 1.0;
	pvr_prim(&vert, sizeof(vert));

	vert.x = right;
	vert.y = down;
	vert.z = 2.0f;
	vert.u = 1.0;
	vert.v = 1.0;
	vert.flags = PVR_CMD_VERTEX_EOL;
	pvr_prim(&vert, sizeof(vert));
}


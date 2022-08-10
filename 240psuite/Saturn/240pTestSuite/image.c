/* 
 * 240p Test Suite for Nintendo 64
 * Copyright (C)2018 Artemio Urbina
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

#include <yaul.h>
#include <tga.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "fs.h"
#include "image.h"
#include "font.h"

/*
int nbg_data[4] = { SCRN_NBG0,
					SCRN_NBG1,
					SCRN_NBG2,
					SCRN_NBG3 };

int nbg_char_data[4] = { VRAM_CTL_CYCP_CHPNDR_NBG0,
						 VRAM_CTL_CYCP_CHPNDR_NBG1,
						 VRAM_CTL_CYCP_CHPNDR_NBG2,
						 VRAM_CTL_CYCP_CHPNDR_NBG3 };*/

bool DisplayImage(const char *filename, int nbg, int width, int height, int piority, bool transparent)
{	
	
	/*struct scrn_bitmap_format nbg_format;
	struct vram_ctl *vram_ctl;
	void *fh;
	int ret;
	uint8_t *ptr;
	tga_t tga;
	uint32_t amount;*/
	
	//using only NBG0 for now
	if(nbg < S_NBG0 || nbg > S_NBG0)
		return false;

	if(!filename)
		return false;

	_svin_background_set_no_filelist(filename);
	/*
	nbg_format.sbf_scroll_screen = nbg_data[nbg];
	nbg_format.sbf_cc_count = SCRN_CCC_RGB_32768;
	nbg_format.sbf_bitmap_size.width = width;
	nbg_format.sbf_bitmap_size.height = height;
	nbg_format.sbf_bitmap_pattern = VRAM_ADDR_4MBIT(0, 0x00000);
	
	vdp2_scrn_bitmap_format_set(&nbg_format);
	vdp2_scrn_priority_set(nbg_data[nbg], piority); 
	
	vram_ctl = vdp2_vram_control_get();
	vram_ctl->vram_cycp.pt[0].t7 = nbg_char_data[nbg];
	vram_ctl->vram_cycp.pt[0].t6 = nbg_char_data[nbg];
	vram_ctl->vram_cycp.pt[0].t5 = nbg_char_data[nbg];
	vram_ctl->vram_cycp.pt[0].t4 = nbg_char_data[nbg];
	vram_ctl->vram_cycp.pt[0].t3 = VRAM_CTL_CYCP_NO_ACCESS;
	vram_ctl->vram_cycp.pt[0].t2 = VRAM_CTL_CYCP_NO_ACCESS;
	vram_ctl->vram_cycp.pt[0].t1 = VRAM_CTL_CYCP_NO_ACCESS;
	vram_ctl->vram_cycp.pt[0].t0 = VRAM_CTL_CYCP_NO_ACCESS;
	
	vram_ctl->vram_cycp.pt[1].t7 = nbg_char_data[nbg];
	vram_ctl->vram_cycp.pt[1].t6 = nbg_char_data[nbg];
	vram_ctl->vram_cycp.pt[1].t5 = nbg_char_data[nbg];
	vram_ctl->vram_cycp.pt[1].t4 = nbg_char_data[nbg];
	vram_ctl->vram_cycp.pt[1].t3 = VRAM_CTL_CYCP_NO_ACCESS;
	vram_ctl->vram_cycp.pt[1].t2 = VRAM_CTL_CYCP_NO_ACCESS;
	vram_ctl->vram_cycp.pt[1].t1 = VRAM_CTL_CYCP_NO_ACCESS;
	vram_ctl->vram_cycp.pt[1].t0 = VRAM_CTL_CYCP_NO_ACCESS;
	vdp2_vram_control_set(vram_ctl);
	
	fh = fs_open(path);
	if(!fh)
	{
		DrawString("Could not open file\n", 120, 20, 1);
		return false;
	}	
	if(!fs_load(fh, (uint8_t *)0x00200000))
	{
		DrawString("Could not load file\n", 120, 20, 1);
		return false;
	}
	
	tga.tga_options.msb = false;
	tga.tga_options.transparent_pixel = COLOR_RGB888_TO_RGB555(200, 0, 200);
	
	ptr = (uint8_t *)0x00200000;
	ret = tga_read(&tga, ptr);
	if(ret != TGA_FILE_OK)
	{
		DrawString("tga_read FAILED\n", 120, 20, 1);
		return false;
	}

	amount = tga_image_decode(&tga, (void *)VRAM_ADDR_4MBIT(0, 0x00000));
	if(amount <= 0)
	{
		DrawString("tga_image_decode FAIL\n", 120, 20, 1);
		return false;
	}
	
	vdp2_scrn_display_set(nbg_data[nbg], transparent);
	vdp2_tvmd_display_set();
*/
	//TODO size and count vram
	return true;
}


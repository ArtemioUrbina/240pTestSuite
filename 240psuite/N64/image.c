/* 
 * 240p Test Suite for Nintendo 64
 * Copyright (C)2024 Artemio Urbina
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
 
 #include "image.h"
 #include "video.h"
 
void rdpqStart() {
 	rdpq_attach(__disp, NULL);
	rdpq_set_mode_copy(true);
 }
 
void rdpqEnd() {
	rdpq_detach_wait();
 }
 
void rdpqDrawImage(sprite_t* tiles, float x, float y) {			
	if(tiles)
		rdpq_sprite_blit(tiles, x, y, NULL);
 }
 
void rdpqClearScreen() {
	rdpq_set_mode_fill(RGBA32(0, 0, 0, 0xff));
	rdpq_fill_rectangle(0, 0, dW, dH);
	rdpq_set_mode_copy(true);
 }
 
void freeImage(sprite_t **image) {
	if(*image) {
		free(*image);
		*image = NULL;
	}
}

/* Frame Buffer for menu */
surface_t *__screen_buffer = NULL;

bool copyFrameBuffer() {	
	if(!__disp)
		return false;
	
	freeFrameBuffer();
	__screen_buffer = (surface_t *)malloc(sizeof(surface_t));
	if(!__screen_buffer)
		return false;
		
	*__screen_buffer = surface_alloc(surface_get_format(__disp), __disp->width, __disp->height);
	
	rdpq_attach(__screen_buffer, NULL);
	rdpq_set_mode_copy(false);
	rdpq_tex_blit(__disp, 0, 0, NULL);
	rdpq_detach_wait();
	
	return true;
}

void freeFrameBuffer() {
	if(__screen_buffer) {
		surface_free(__screen_buffer);
		free(__screen_buffer);
		__screen_buffer = NULL;
	}
}

void displayFrameBuffer() {
	if(!__screen_buffer || !__disp)
		return;

	rdpq_attach(__disp, NULL);
	rdpq_set_mode_copy(false);
	rdpq_tex_blit(__screen_buffer, 0, 0, NULL);
	rdpq_detach_wait();
}

void darkenBuffer(int amount) {
    if(!__screen_buffer)
		return;

	if(surface_get_format(__screen_buffer) != FMT_RGBA16)
		return;

	uint16_t *screen = (uint16_t*)__screen_buffer->buffer;
	int len = TEX_FORMAT_PIX2BYTES(surface_get_format(__screen_buffer), __screen_buffer->width * __screen_buffer->height)/2;
    for(int i = 0; i < len; i++) {
		color_t color;
			
		color = color_from_packed16(screen[i]);
		
		if(color.r > amount)
			color.r -= amount;
		else
			color.r = 0;
		if(color.g > amount)
			color.g -= amount;
		else
			color.g = 0;
		if(color.b > amount)
			color.b -= amount;
		else
			color.b = 0;
	
		screen[i] = graphics_convert_color(color);
	}
}
 

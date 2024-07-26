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

#define PAL4_SIZE	16
#define PAL8_SIZE	256

bool clearScreen = false;
unsigned int currFB = 0;

void rdpqSetDrawMode() {
	if(vMode == SUITE_640x480) {
		rdpq_set_mode_standard();
		rdpq_mode_alphacompare(1);
		rdpq_mode_filter(FILTER_POINT);
	}
	else
		rdpq_set_mode_copy(true);
}
 
void rdpqStart() {
	if(clearScreen) {
		rdpq_attach_clear(__disp, NULL);
		if(currFB == current_buffers) {
			clearScreen = false;
			currFB = 0;
		}
		else
			currFB ++;
	}
	else
		rdpq_attach(__disp, NULL);
	
	rdpqSetDrawMode();
}
 
void rdpqEnd() {
	rdpq_detach_wait();
}
 
void rdpqDrawImage(image* data) {
#ifdef DEBUG_BENCHMARK
	assertf(data, "rdpqDrawImage() received NULL image");
	assertf(data->tiles, "rdpqDrawImage() received NULL tiles");
#else
	if(!data) 				return;
	if(!data->tiles)		return;
#endif

	if(vMode == SUITE_640x480 && data->scale) {
		if(data->center) {
			data->x = (dW/2 - data->tiles->width)/2;
			data->y = (dH/2 - data->tiles->height)/2;
		}
		rdpq_sprite_blit(data->tiles, 2*data->x, 2*data->y, &(rdpq_blitparms_t) {
			.scale_x = 2.0f, .scale_y = 2.0f
			});
	}
	else {
		if(data->center) {
			data->x = (dW - data->tiles->width)/2;
			data->y = (dH - data->tiles->height)/2;
		}
		rdpq_sprite_blit(data->tiles, data->x, data->y, NULL);
	}
}

void rdpqDrawImageXY(image* data, int x, int y) {
#ifdef DEBUG_BENCHMARK
	assertf(data, "rdpqDrawImage() received NULL image");
	assertf(data->tiles, "rdpqDrawImage() received NULL tiles");
#else
	if(!data) 				return;
	if(!data->tiles)		return;
#endif
	
	if(vMode == SUITE_640x480 && data->scale) {
		rdpq_sprite_blit(data->tiles, 2*x, 2*y, &(rdpq_blitparms_t) {
			.scale_x = 2.0f, .scale_y = 2.0f
			});
	}
	else
		rdpq_sprite_blit(data->tiles, x, y, NULL);
}
 
void rdpqClearScreen() {
	rdpq_set_mode_fill(RGBA32(0, 0, 0, 0xff));
	rdpq_fill_rectangle(0, 0, dW, dH);
	
	rdpqSetDrawMode();
}

/* Image stuct */

image *loadImage(char *name) {
	image *data = NULL;
	
	data = (image*)malloc(sizeof(image));
	if(!data)
		return NULL;
	data->tiles = sprite_load(name);
	if(!data->tiles) {
		free(data);
		return(NULL);
	}
	data->x = 0;
	data->y = 0;
	data->center = false;
	data->scale = true;
	
	data->palette = NULL;
	data->origPalette = NULL;
	data->palSize = 0;
	data->fadeSteps = 0;
	
	/* prepare palette if it exists */
	tex_format_t texFormat = sprite_get_format(data->tiles);
	
	if(texFormat == FMT_CI4)
		data->palSize = PAL4_SIZE;
	if(texFormat == FMT_CI8)
		data->palSize = PAL8_SIZE;

	if(data->palSize) {
		data->palette = sprite_get_palette(data->tiles);
#ifdef DEBUG_BENCHMARK
		assertf(data->palette != NULL, 
			"loadImage() No palette in indexed image");
#endif
	}
	
	return(data);
}
 
void freeImage(image **data) {
	if(*data) {
		if((*data)->tiles) {
			sprite_free((*data)->tiles);
			(*data)->tiles = NULL;
		}
		if((*data)->origPalette) {
			free((*data)->origPalette);
			(*data)->origPalette = NULL;
		}
		free(*data);
		*data = NULL;
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
	if(!__screen_buffer->buffer) {
		free(__screen_buffer);
		__screen_buffer = NULL;
		return false;
	}
	
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

void fadePaletteStep(uint16_t *colorRaw, unsigned int fadeSteps) {
	color_t color;
			
	color = color_from_packed16(*colorRaw);
    color.r = (color.r > 0) ? (color.r - color.r/fadeSteps) : 0;
    color.g = (color.g > 0) ? (color.g - color.g/fadeSteps) : 0;
    color.b = (color.b > 0) ? (color.b - color.b/fadeSteps) : 0;
	*colorRaw = color_to_packed16(color);
}

/* Fade paletted images */

void setPaletteFX(image *data) {
	if(!data->palette)
		return;
	
	if(data->origPalette) {
		free(data->origPalette);
		data->origPalette = NULL;
	}
	
	if(data->palette) {
		data->origPalette = (uint16_t *)malloc(sizeof(uint16_t)*data->palSize);
		if(data->origPalette)
			memcpy(data->origPalette, data->palette, sizeof(uint16_t)*data->palSize);
	}
}

void resetPalette(image *data) {
	if(data->palette && data->origPalette) {
		memcpy(data->palette, data->origPalette, sizeof(uint16_t)*data->palSize);
		data_cache_hit_writeback(data->palette, sizeof(uint16_t)*data->palSize);
	}
}

void fadeInit(image *data, unsigned int steps) {
#ifdef DEBUG_BENCHMARK
	assertf(data, "fadeInit() NULL pointer received");
	assertf(data->palette, "fadeInit() Image with no palette in fade");
	assertf(steps != 0, "fadeInit() steps is zero");
#else
	if(!data || !data->palette || steps == 0)
		return;
#endif

	resetPalette(data);
	data->fadeSteps = steps;
}

void fadeImageStep(image *data) {
#ifdef DEBUG_BENCHMARK
	assertf(data, "fadeImageStep() NULL pointer received");
	assertf(data->palette, "fadeImageStep() Image with no palette for dade");
	assertf(data->fadeSteps != 0, "fadeImageStep() fadeStep is zero");
#else
	if(!data || !data->palette || data->fadeSteps == 0)
		return;
#endif

	for(unsigned int c = 0; c < data->palSize; c++)
		fadePaletteStep(&data->palette[c], data->fadeSteps);
	
	// Invalidate the cache
	data_cache_hit_writeback_invalidate(data->palette, sizeof(uint16_t)*data->palSize);
}

#ifdef DEBUG_BENCHMARK
#include "font.h"

void printPalette(image *data, int x, int y) {
	for(unsigned int c = 0; c < data->palSize; c++) {
		char str[64];
		color_t color = color_from_packed16(data->palette[c]);
	
		sprintf(str, "%X %X %X", color.r, color.g, color.b);
		drawStringS(x, y, 0x00, 0xff, 0x00, str);
		y += fh;
	}
}
#endif
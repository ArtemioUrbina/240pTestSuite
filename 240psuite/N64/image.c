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
 #include "controller.h"

#define PAL4_SIZE	16
// We can't know the size (mksprite.c:1163) our highest one is 32,
// and 64 causes a buffer overflow in our fade code
#define PAL8_SIZE	32

int clearScreen = 0;
unsigned int currFB = 0;

// Upscale frame buffer
surface_t *__real_disp = NULL;
surface_t *__upscale_fb = NULL;
int upscaleFrame = 0;
int menuIgnoreUpscale = 0;

void setClearScreen() {
	clearScreen = 1;
}

void rdpqSetDrawMode() {
	rdpq_set_mode_copy(1);
}
 
void rdpqStart() {
	if(clearScreen) {
		rdpq_attach_clear(__disp, NULL);

		if(upscaleFrame) {
			rdpq_attach_clear(__upscale_fb, NULL);
			rdpq_detach_wait();
		}

		if(currFB == current_buffers) {
			clearScreen = 0;
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
	if(upscaleFrame) {
		/* Set a target for software drawing functions */
		__real_disp = __disp;
		__disp = __upscale_fb;
	}

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

	if(isVMode480() && !data->scale)
		upscaleFrame = 0;
	
	if(upscaleFrame)
		rdpq_attach(__upscale_fb, NULL);
	
	if(data->center) {
		const surface_t *output = rdpq_get_attached();
		
		if(output) {
			data->x = (output->width - data->tiles->width)/2;
			data->y = (output->height - data->tiles->height)/2;
		}
	}

	if(!data->flipH && !data->flipH) {
		rdpq_sprite_blit(data->tiles, data->x, data->y, NULL);
	}
	else {
		rdpq_set_mode_standard();
		rdpq_mode_alphacompare(1);
		rdpq_sprite_blit(data->tiles, data->x, data->y, &(rdpq_blitparms_t) {
			.flip_x = data->flipH, .flip_y = data->flipV
			});
		rdpqSetDrawMode();
	}
	
	if(upscaleFrame)
		rdpq_detach();
}

void rdpqDrawImageXY(image* data, int x, int y) {
#ifdef DEBUG_BENCHMARK
	assertf(data, "rdpqDrawImage() received NULL image");
	assertf(data->tiles, "rdpqDrawImage() received NULL tiles");
#else
	if(!data) 				return;
	if(!data->tiles)		return;
#endif
	
	if(isVMode480() && !data->scale)
		upscaleFrame = 0;
		
	if(upscaleFrame)
		rdpq_attach(__upscale_fb, NULL);
	
	if(!data->flipH && !data->flipH) {
		rdpq_sprite_blit(data->tiles, x, y, NULL);
	}
	else {
		rdpq_set_mode_standard();
		rdpq_mode_alphacompare(1);
		rdpq_sprite_blit(data->tiles, x, y, &(rdpq_blitparms_t) {
			.flip_x = data->flipH, .flip_y = data->flipV
			});
		rdpqSetDrawMode();
	}
	
	if(upscaleFrame)
		rdpq_detach();
}

void rdpqFillWithImage(image* data) {
#ifdef DEBUG_BENCHMARK
	assertf(__disp, "rdpqFillWithImage() __disp was NULL");
	assertf(data, "rdpqFillWithImage() received NULL image");
	assertf(data->tiles, "rdpqFillWithImage() received NULL tiles");
	assertf(data->palette, "rdpqFillWithImage() image with no palette");
#else
	if(!__disp)				return;
	if(!data) 				return;
	if(!data->tiles)		return;
	if(!data->palette)		return;
#endif
	unsigned int width = __disp->width, height = __disp->height;
	surface_t tiles_surf = sprite_get_pixels(data->tiles);

	if(upscaleFrame) {
		rdpq_attach(__upscale_fb, NULL);
		width = __upscale_fb->width;
		height = __upscale_fb->height;
	}

	rdpq_clear(RGBA32(0xf8, 0xf8, 0xf8, 0xff));
	
	rdpq_mode_push();
	
	rdpq_mode_tlut(TLUT_RGBA16);
	rdpq_tex_upload_tlut(data->palette, 0, data->palSize);
	rdpq_tex_upload(TILE0, &tiles_surf, &(rdpq_texparms_t) 
		{ .s.repeats = REPEAT_INFINITE, .t.repeats = REPEAT_INFINITE });
    rdpq_texture_rectangle(TILE0, 0, 0, width, height, 0, 0);
	
	rdpq_mode_pop();
	
	if(upscaleFrame)
		rdpq_detach();
}

void rdpqClearScreen() {
	if(upscaleFrame)
		rdpq_attach(__upscale_fb, NULL);
	rdpq_clear(RGBA32(0, 0, 0, 0xff));
	if(upscaleFrame)
		rdpq_detach();
}

void rdpqClearScreenWhite() {
	if(upscaleFrame)
		rdpq_attach(__upscale_fb, NULL);
	rdpq_clear(RGBA32(IRE_100, IRE_100, IRE_100, 0xff));
	if(upscaleFrame)
		rdpq_detach();
}

/* Image stuct */

image *loadImage(char *name) {
	image *data = NULL;

#ifndef DEBUG_BENCHMARK	
	int fh = dfs_open(name+5);   // remove "rom:/"
	if(fh < DFS_ESUCCESS)
		return NULL;
	dfs_close(fh);
#endif

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
	data->center = 0;
	data->scale = 1;
	data->flipH = 0;
	data->flipV = 0;
	
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
	if(!data || !*data)
		return;
	
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

/* Frame Buffer for menu */

surface_t *__menu_fb = NULL;

int copyMenuFB() {	
#ifdef DEBUG_BENCHMARK
	assertf(__disp != NULL, "copyFrameBuffer(): __disp was NULL");
	assertf(__menu_fb == NULL, "copyFrameBuffer(): __menu_fb was not NULL");
#else
	if(!__disp)
		return 0;
	freeMenuFB();
#endif

	__menu_fb = (surface_t *)malloc(sizeof(surface_t));
	if(!__menu_fb)
		return 0;
	memset(__menu_fb, 0, sizeof(surface_t));
		
	*__menu_fb = surface_alloc(surface_get_format(__disp), __disp->width, __disp->height);
	if(!__menu_fb->buffer) {
		free(__menu_fb);
		__menu_fb = NULL;
		return 0;
	}
	
	rdpq_attach(__menu_fb, NULL);
	rdpq_set_mode_copy(0);
	rdpq_tex_blit(__disp, 0, 0, NULL);
	rdpq_detach();

	if(!upscaleFrame)
		menuIgnoreUpscale = 1;

	return 1;
}

void freeMenuFB() {
	if(__menu_fb) {
		surface_free(__menu_fb);
		free(__menu_fb);
		__menu_fb = NULL;
	}

	if(menuIgnoreUpscale)
		menuIgnoreUpscale = 0;
}

void drawMenuFB() {
	if(!__menu_fb || !__disp)
		return;

	rdpq_attach(upscaleFrame ? __upscale_fb : __disp, NULL);
	rdpq_set_mode_copy(0);
	rdpq_tex_blit(__menu_fb, 0, 0, NULL);
	rdpq_detach();
}

void darkenMenuFB(int amount) {
	if(!__menu_fb)
		return;

	if(surface_get_format(__menu_fb) != FMT_RGBA16)
		return;
	
	uint16_t *pixels = (uint16_t*)__menu_fb->buffer;
	unsigned int len = TEX_FORMAT_PIX2BYTES(surface_get_format(__menu_fb), __menu_fb->width * __menu_fb->height)/2;

	for(unsigned int i = 0; i < len; i++) {
		color_t color;
			
		color = color_from_packed16(pixels[i]);
		
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
	
		pixels[i] = graphics_convert_color(color);
	}
}

/* Frame Buffer for upscaling */

void rdpqUpscalePrepareFB() {
	if(!menuIgnoreUpscale && isVMode480()) {
		createUpscaleFB();
		upscaleFrame = 1;
	}
}

int createUpscaleFB() {
	if(__upscale_fb || !__disp)
		return 0;
	
	__upscale_fb = (surface_t *)malloc(sizeof(surface_t));
	if(!__upscale_fb)
		return 0;
	memset(__upscale_fb, 0, sizeof(surface_t));

	*__upscale_fb = surface_alloc(surface_get_format(__disp), __disp->width/2, __disp->height/2);
	if(!__upscale_fb->buffer) {
		free(__upscale_fb);
		__upscale_fb = NULL;
		return 0;
	}
	
	return 1;
}

void freeUpscaleFB() {
	if(__upscale_fb) {
		surface_free(__upscale_fb);
		free(__upscale_fb);
		__upscale_fb = NULL;
	}
	
	if(__real_disp) {
		__disp = __real_disp;
		__real_disp = NULL;
	}
}

void executeUpscaleFB() {
	if(!upscaleFrame) {
		if(__disp) {
			// Set dW and dH to the real values for next frame
			dW = __disp->width;
			dH = __disp->height;
		}
		return;
	}

	if(!__upscale_fb || !__disp)
		return;
	
	if(__real_disp) {
		__disp = __real_disp;
		__real_disp = NULL;
	}
	
	rdpq_attach(__disp, NULL);
	rdpq_set_mode_standard();
	rdpq_mode_alphacompare(0);
	rdpq_mode_filter(FILTER_POINT);
	rdpq_tex_blit(__upscale_fb, 0, 0, &(rdpq_blitparms_t) {
			.scale_x = 2.0f, .scale_y = 2.0f
			});
	rdpq_detach();
	
	// Set dW and dH to the real values for next frame
	dW = __upscale_fb->width;
	dH = __upscale_fb->height;
	
	upscaleFrame = 0;
}

/* Fade paletted images */

void fadePaletteStep(uint16_t *colorRaw, unsigned int fadeSteps) {
	color_t color;
	
	color = color_from_packed16(*colorRaw);
	color.r = (color.r > 0) ? (color.r - color.r/fadeSteps) : 0;
	color.g = (color.g > 0) ? (color.g - color.g/fadeSteps) : 0;
	color.b = (color.b > 0) ? (color.b - color.b/fadeSteps) : 0;
	*colorRaw = color_to_packed16(color);
}

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

inline void updatePalette(image *data) {
	if(!data->palette) return;
	// Invalidate the cache
	data_cache_hit_writeback_invalidate(data->palette, sizeof(uint16_t)*data->palSize);
}

void swapPaletteColors(image *data, unsigned int color1, unsigned int color2) {
	if(!data->palette) return;
	
	uint16_t tmpColor = data->palette[color1];
	
	data->palette[color1] = data->palette[color2];
	data->palette[color2] = tmpColor;
	updatePalette(data);
}

void resetPalette(image *data) {
	if(data->palette && data->origPalette) {
		memcpy(data->palette, data->origPalette, sizeof(uint16_t)*data->palSize);
		updatePalette(data);
	}
}

void swapPalette100to75(image *data) {
	if(!data || !data->palette)
		return;
	
	for(unsigned int c = 0; c < data->palSize; c++) {
		color_t color = color_from_packed16(data->palette[c]);
		if(color.r >= IRE_100)
			color.r = IRE_075;
		if(color.g >= IRE_100)
			color.g = IRE_075;
		if(color.b >= IRE_100)
			color.b = IRE_075;
		data->palette[c] = color_to_packed16(color);
	}
	updatePalette(data);
}

void swapPalette75to100(image *data) {
	if(!data || !data->palette)
		return;
	
	for(unsigned int c = 0; c < data->palSize; c++) {
		color_t color = color_from_packed16(data->palette[c]);
		if(color.r == IRE_075)
			color.r = IRE_100;
		if(color.g == IRE_075)
			color.g = IRE_100;
		if(color.b == IRE_075)
			color.b = IRE_100;
		data->palette[c] = color_to_packed16(color);
	}
	updatePalette(data);
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
	
	updatePalette(data);
}

#define FADE_STEPS	20
#define FADE_HOLD	10

void drawSplash(char *name, int delay) {
	joypad_buttons_t keys;
	image *logo = NULL;
	
	logo = loadImage(name);
	if(!logo)
		return;

	logo->center = true;
	if(dH > 240)
		logo->scale = 0;
	
	while(delay) {
		getDisplay();
		
		rdpqStart();
		rdpqDrawImage(logo);
		rdpqEnd();
		
		waitVsync();
		
		joypad_poll();
		keys = controllerButtonsDown();
		
		delay --;
		if(keys.a || keys.b || keys.start)
			delay = 0;
	}
	
	delay = FADE_STEPS;
	fadeInit(logo, FADE_STEPS);
	while(delay) {
		getDisplay();
		
		rdpqStart();
		rdpqDrawImage(logo);
		rdpqEnd();
		
		fadeImageStep(logo);
		
		waitVsync();
		
		delay --;
	}
	
	setClearScreen();
	delay = FADE_HOLD;
	while(delay) {
		getDisplay();
		
		rdpqStart();
		rdpqEnd();
		
		waitVsync();
		
		delay --;
	}
	
	freeImage(&logo);
}


/* Big Numbers */
#define NUMBER_COLORS 	4

image *bigNumbers[NUMBER_COLORS] = { NULL, NULL, NULL, NULL };

void loadNumbers() {
	if(bigNumbers[NUMBER_WHITE])
		return;
	
	for(unsigned int num = 0; num < NUMBER_COLORS; num++) {
		bigNumbers[num] = loadImage("rom:/numbers.sprite");
	}
	
	bigNumbers[NUMBER_WHITE]->palette[3] = graphics_make_color(IRE_100, IRE_100, IRE_100, 0xff);
	updatePalette(bigNumbers[NUMBER_WHITE]);
	bigNumbers[NUMBER_BLACK]->palette[3] = graphics_make_color(0, 0, 0, 0xff);
	updatePalette(bigNumbers[NUMBER_BLACK]);
	bigNumbers[NUMBER_RED]->palette[3] = graphics_make_color(IRE_100, 0, 0, 0xff);
	updatePalette(bigNumbers[NUMBER_RED]);
	bigNumbers[NUMBER_BLUE]->palette[3] = graphics_make_color(0, 0, IRE_100, 0xff);
	updatePalette(bigNumbers[NUMBER_BLUE]);
}

void releaseNumbers() {
	for(unsigned int num = 0; num < NUMBER_COLORS; num++) {
		freeImage(&bigNumbers[num]);
	}
}

void drawDigit(uint16_t x, uint16_t y, uint16_t color, uint16_t digit) {
	if(!bigNumbers[NUMBER_WHITE])
		return;
	
	if(isVMode480() && !bigNumbers[color]->scale)
		upscaleFrame = 0;
		
	if(upscaleFrame)
		rdpq_attach(__upscale_fb, NULL);
	
	rdpq_sprite_blit(bigNumbers[color]->tiles, x, y, &(rdpq_blitparms_t) {
		.s0 = digit*24, .t0 = 0,
		.width = 24, .height = 40
		});
	
	if(upscaleFrame)
		rdpq_detach();
}

void drawNumber(uint16_t x, uint16_t y, uint16_t color, uint16_t number) {
	char  string[10], *str;

	str = string;

	sprintf(str, "%u", number);
	while (*str) {
		drawDigit(x, y, color, *str++);
		x += 24;
	}
}

void drawBlackBox(int x, int y, int width, int height) {
	graphics_draw_box(upscaleFrame ? __upscale_fb : __disp, 
		x, y, width, height, graphics_make_color(0, 0, 0, 0xff));		
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

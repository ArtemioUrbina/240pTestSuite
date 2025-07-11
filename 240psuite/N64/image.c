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
 
 #include "font.h"
 #include "image.h"
 #include "video.h"
 #include "controller.h"

#define PAL4_SIZE	16
// We can't know the size (mksprite.c:1163) our highest one is 32,
// and 64 causes a buffer overflow in our fade code
#define PAL8_SIZE	256

int clearScreen = 0;
unsigned int currFB = 0;

// Upscale frame buffer
surface_t *__real_disp = NULL;
surface_t *__upscale_fb = NULL;
int upscaleFrame = 0;
int menuIgnoreUpscale = 0;

#define LFU_YES	1
#define LFU_NO	0
#define LFU_UNK	-1
int lastFrameUpscaled = LFU_UNK;

void setClearScreen() {
	clearScreen = 1;
}

void frameEnableUpscaler(int enable) {
	upscaleFrame = enable;
}

void rdpqSetDrawMode(int type) {
	if(current_bitdepth == DEPTH_32_BPP)
		rdpq_set_mode_standard();
	else {
		rdpq_set_mode_copy(type);
		rdpq_mode_antialias(current_rdp_aa_filter);
	}
}
 
void rdpqStart() {
	assertf(__disp != NULL, "rdpqStart(): with NULL _disp");
	
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
	
	rdpqSetDrawMode(1);
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
			if(!data->rotate) {
				data->x = (output->width - data->tiles->width)/2;
				data->y = (output->height - data->tiles->height)/2;
			}
			else {
				data->x = output->width/2;
				data->y = output->height/2;
			}
		}
	}

	if(!data->flipH && !data->flipH && !data->rotate) {
		rdpq_sprite_blit(data->tiles, data->x, data->y, NULL);
	}
	else if(data->rotate) {
		rdpq_set_mode_standard();
		rdpq_mode_alphacompare(1);
		rdpq_sprite_blit(data->tiles, data->x, data->y, &(rdpq_blitparms_t) {
			.cx = data->tiles->width / 2,
			.cy = data->tiles->height / 2,
			.theta = data->rotAngle*M_PI/180
			});
		rdpqSetDrawMode(1);
	} else {
		rdpq_set_mode_standard();
		rdpq_mode_alphacompare(1);
		rdpq_sprite_blit(data->tiles, data->x, data->y, &(rdpq_blitparms_t) {
			.flip_x = data->flipH, .flip_y = data->flipV
			});
		rdpqSetDrawMode(1);
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
		rdpqSetDrawMode(1);
	}
	
	if(upscaleFrame)
		rdpq_detach();
}

void rdpqFillWithImage(image* data) {
	rdpqFillWithImageXY(data, 0, 0);
}

void rdpqFillWithImageXY(image* data, float x, float y) {
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

	if(sprite_fits_tmem(data->tiles)) {
		surface_t tiles_surf = sprite_get_pixels(data->tiles);

		if(upscaleFrame)
			rdpq_attach(__upscale_fb, NULL);

		rdpq_clear(RGBA32(IRE_100, IRE_100, IRE_100, IRE_100));
		
		rdpq_mode_push();
		
		rdpq_mode_tlut(TLUT_RGBA16);
		rdpq_tex_upload_tlut(data->palette, 0, data->palSize);
		rdpq_tex_upload(TILE0, &tiles_surf, &(rdpq_texparms_t) 
			{ 	.s.repeats = REPEAT_INFINITE, .t.repeats = REPEAT_INFINITE, 
				.s.mirror = MIRROR_NONE, .t.mirror = MIRROR_NONE, 
				.s.translate = x, .t.translate = y });
		rdpq_texture_rectangle(TILE0, 0, 0, getDispWidth(), getDispHeight(), 0, 0);
		
		rdpq_mode_pop();
		
		if(upscaleFrame)
			rdpq_detach();
	}
	else {
		for(unsigned int y = 0; y < getDispHeight(); y += data->tiles->height) {
			for(unsigned int x = 0; x < getDispWidth(); x += data->tiles->width)
				rdpqDrawImageXY(data, x, y);
		}
	}
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

void rdpqDrawRectangle(int tx, int ty, int bx, int by, int r, int g, int b) {
	if(upscaleFrame)
		rdpq_attach(__upscale_fb, NULL);
	
	rdpq_mode_push();
	
	rdpq_set_mode_fill(RGBA32(r, g, b, 0xff));
	rdpq_fill_rectangle(tx, ty, bx, by);
	
	rdpq_mode_pop();
	
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
	data->rotate = 0;
	data->rotAngle = 00.f;
	
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
	if(!__disp)
		return 0;
	freeMenuFB();
	
	// ATM don't allow it to happen when in 32bpp since it draws
	// incorerctly with rdpq_tex_blit
	if(current_bitdepth == DEPTH_32_BPP)
		return 0;
	
	// only allow 480i in 8MB expanded systems
	// reason is: although it works, it ends up leading to memory
	// fragmentation even if allocated from boot since constant
	// resolution changes deallocate and reallocate libDragon's
	// framebuffers (safe buffer)
	
	if(getDispHeight() > 288 && (get_memory_size() / 0x100000) < 8)
		return 0;

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
	rdpqSetDrawMode(0);
	rdpq_tex_blit(__disp, 0, 0, NULL);
	rdpq_detach();

	if(isVMode480() && !upscaleFrame)
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
	surface_t *target = NULL;
	
	if(!__menu_fb || !__disp)
		return;
	
	target = upscaleFrame ? __upscale_fb : __disp;
	
	rdpq_attach(target, NULL);
	rdpqSetDrawMode(0);
	rdpq_clear(RGBA32(0, 0, 0, 0xff));
	// check if we are using a FB from a different resolution and adjust
	if(target->height == __menu_fb->height && target->width > __menu_fb->width)
		rdpq_tex_blit(__menu_fb, (target->width - __menu_fb->width)/2, 0, NULL);
	else
		rdpq_tex_blit(__menu_fb, 0, 0, NULL);
	rdpq_detach();
}

void darkenMenuFB(int amount) {
	if(!__menu_fb)
		return;
	
	if(surface_get_format(__menu_fb) == FMT_RGBA16) {
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
	
	if(surface_get_format(__menu_fb) == FMT_RGBA32) {
		uint32_t *pixels = (uint32_t *)__menu_fb->buffer;
		unsigned int len = TEX_FORMAT_PIX2BYTES(surface_get_format(__menu_fb), __menu_fb->width * __menu_fb->height)/4;

		for(unsigned int i = 0; i < len; i++) {
			color_t color;
				
			color = color_from_packed32(pixels[i]);

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
}

int hasMenuFB() {
	if(!__menu_fb)
		return 0;
	return 1;
}

/* Frame Buffer for upscaling */

void rdpqUpscalePrepareFB() {
	if(!menuIgnoreUpscale && isVMode480()) {
		createUpscaleFB();
		upscaleFrame = 1;
		lastFrameUpscaled = LFU_UNK;
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
		lastFrameUpscaled = LFU_NO;
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
	
	upscaleFrame = 0;
	lastFrameUpscaled = LFU_YES;
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

void updatePalette(image *data) {
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

void fadePaletteStep(uint16_t *colorRaw, unsigned int fadeSteps) {
	color_t color;
	
	color = color_from_packed16(*colorRaw);
	color.r = (color.r > 0) ? (color.r - color.r/fadeSteps) : 0;
	color.g = (color.g > 0) ? (color.g - color.g/fadeSteps) : 0;
	color.b = (color.b > 0) ? (color.b - color.b/fadeSteps) : 0;
	*colorRaw = color_to_packed16(color);
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

int getDispWidth() {
	if(lastFrameUpscaled != LFU_YES &&
		(!upscaleFrame || menuIgnoreUpscale))
		return getHardWidth();
	
	if(__upscale_fb)
		return(__upscale_fb->width);

	return 0;
}

int getDispHeight() {
	if(lastFrameUpscaled != LFU_YES &&
		(!upscaleFrame || menuIgnoreUpscale))
		return getHardHeight();
	
	if(__upscale_fb)
		return(__upscale_fb->height);
	
	return 0;
}

#define FADE_STEPS	20
#define FADE_HOLD	10

// We force an external palette size because the API doens't give us one
// if unknown, to add a new image, add verbose in the Makefile for
// mksprite and check the colors

int drawSplash(char *name, int delay, int paleteSize) {
	int cancel = 0;
	joypad_buttons_t keys;
	image *logo = NULL;
	
	logo = loadImage(name);
	if(!logo)
		return 0;

	logo->center = true;
	if(getDispHeight() > 288 && logo->tiles->height > 240)
		logo->scale = 0;
	logo->palSize = paleteSize;
	
	if(!delay)
		cancel = 1;
	while(delay) {
		getDisplay();
		
		rdpqStart();
		rdpqDrawImage(logo);
		rdpqEnd();
		
		waitVsync();
		
		joypad_poll();
		keys = controllerButtonsDown();
		
		delay --;
		if(keys.a || keys.b || keys.start) {
			delay = 0;
			cancel = 1;
		}
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
	return cancel;
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

void printPalette(image *data, int x, int y) {
	for(unsigned int c = 0; c < data->palSize; c++) {
		char str[64];
		color_t color = color_from_packed16(data->palette[c]);
	
		sprintf(str, "%X %X %X", color.r, color.g, color.b);
		drawStringS(x, y, 0x00, 0xff, 0x00, str);
		y += fh;
	}
}


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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <SDL/SDL.h>

#include "SDL_dreamcast.h"
#include "SDL_mixer.h"
#include "SDL_gfxPrimitives.h"

#include "SDL_gfx_fnt.h"
#include "SDL_gfx_5x7_fnt.h"

// Dreamcast max displayable res using SDL is 320x234

//#define SERIAL
typedef SDL_Surface * bitmap;

extern 	uint8 romdisk[];
int	W = 320, H = 240;
int	fontW = 5, fontH = 7;

#define NATIVE_320	0
#define FAKE_640	1
#define NATIVE_640	2
#define FAKE_640_SL	3

int vmode 	= NATIVE_320;
int vcable	= CT_RGB;
SDL_Surface 	*buff320 = NULL;

KOS_INIT_ROMDISK(romdisk);
KOS_INIT_FLAGS(INIT_DEFAULT);

inline static void DStringS(SDL_Surface *screen, char *str, int x, int y, int r, int g, int b, int a);
inline static void DStringB(SDL_Surface *screen, char *str, int x, int y, int r, int g, int b, int bgcolor);

inline static bitmap LoadImage(char *image_path);
inline static bitmap LoadImageWAlpha(char *image_path, Uint32 R, Uint32 G, Uint32 B);
inline static int DrawImage(SDL_Surface *surface, bitmap image, int x_pos, int y_pos );
inline static void FreeImage(bitmap *image);
inline static void ScaleScreen(SDL_Surface *screen);
inline static void ClearScreen(SDL_Surface *screen);

void DrawColorBars(SDL_Surface *screen);
void Draw601ColorBars(SDL_Surface *screen);
void DrawGrid(SDL_Surface *screen);
void DrawLinearity(SDL_Surface *screen);
void DropShadowTest(SDL_Surface *screen);
void LagTest(SDL_Surface *screen);
void DrawStripes(SDL_Surface *screen);
void DrawCheckBoard(SDL_Surface *screen);
void ChangeResolution(SDL_Surface **screen, int *W, int *H, Uint32 *video_flags, Uint32 *video_bits);
void DrawCredits(SDL_Surface *screen);

int main ( int argc, char *argv[] )
{
	int		done = 0, sel = 1, redraw = 1;
	Uint16  	oldbuttons = 0xffff, pressed, joycnt = 0;
	SDL_Surface 	*screen;
	Uint32 		video_flags = SDL_HWSURFACE | SDL_HWPALETTE | SDL_DOUBLEBUF | SDL_FULLSCREEN;
	Uint32   	video_bits = 16;
    	bitmap          title = NULL;

	SDL_DC_ShowAskHz(SDL_TRUE);
    	SDL_DC_VerticalWait(SDL_TRUE);
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		fprintf(stderr,
			"Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	screen = SDL_SetVideoMode(W, H, video_bits, video_flags);
	if (screen == NULL)
	{
		fprintf(stderr, "Couldn't set %dx%dx%d video mode: %s\n",
					W, H, video_bits, SDL_GetError());
		exit(1);
	}

	if(Mix_OpenAudio(44100, AUDIO_U8, 2, 4096) != 0)
		fprintf(stderr, "Unable to initialize audio: %s\n", Mix_GetError());

	vcable = vid_check_cable();

    	SDL_ShowCursor(SDL_DISABLE);

	fontW = 5;
	fontH = 7;
 	gfxPrimitivesSetFont(&SDL_gfx_font_5x7_fnt, fontW, fontH);

    	title = LoadImage("/rd/title.bmp");	

 	while (!done)
	{
		int c = 1; // option count

        	if(redraw)
        	{
        		char    res[20];
			int 	r, g, b, x = 40, y = 55, fh = fontH + 2;

			r = 0xFF;
			g = 0xFF;
			b = 0xFF;

            		if(!DrawImage(screen, title, 0, 0 ))
				ClearScreen(screen);

			DStringS(screen, "Color Bars",
				x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, 0xFF); c++; y += fh;
			DStringS(screen, "Color Bars with Gray Scale",
				x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, 0xFF); c++; y += fh;
			DStringS(screen, "Grid",
				x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, 0xFF); c++; y += fh;
			DStringS(screen, "Linearity",
				x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, 0xFF); c++; y += fh;
			DStringS(screen, "Drop Shadow Test",
				x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, 0xFF); c++; y += fh;
			DStringS(screen, "Lag Test",
				x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, 0xFF); c++; y += fh;
			DStringS(screen, "Horizontal Stripes",
				x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, 0xFF); c++; y += fh;
			DStringS(screen, "Checkerboard",
				x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, 0xFF); c++; y += fh;
			switch(vmode)
			{
				case NATIVE_320:
				case NATIVE_640:
        				sprintf(res, "Resolution: %dx%d", W, H);
					break;
				case FAKE_640:
        				sprintf(res, "Resolution: Fake %dx%d", W, H);
					break;
				case FAKE_640_SL:
        				sprintf(res, "Resolution: Fake %dx%d scanlines", W, H);
					break;
			}
			DStringS(screen, res,
				x, y, r, sel == c ? 0 : g, sel == c ? 0 : b, 0xFF); c++; y += fh;
			DStringS(screen, "Credits",
				x, y+fh, r, sel == c ? 0 : g, sel == c ? 0 : b, 0xFF); 


			switch(vcable)
			{

				case CT_RGB:
					DStringS(screen, "RGB Cable", 230, 215, 0x0, 0xFF, 0xFF, 0xFF);
					break;
				case CT_VGA:
					DStringS(screen, "VGA Cable", 230, 215, 0x0, 0xFF, 0xFF, 0xFF);
					break;
				case CT_COMPOSITE:
					DStringS(screen, "Composite Cable", 230, 215, 0x0, 0xFF, 0xFF, 0xFF);
					break;
			}

            		redraw = 0;
        	}
		else
			c = 10;

		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
		{
			pressed = st->buttons & ~oldbuttons;
			oldbuttons = st->buttons;
#ifdef SERIAL
			if (st->buttons & CONT_START && st->buttons & CONT_B)
				done =  1;
#endif

			if (pressed & CONT_DPAD_UP)
			{
				sel --;
				if(sel < 1)
					sel = c;
				redraw = 1;
			}

			if (pressed & CONT_DPAD_DOWN)
			{
				sel ++;
				if(sel > c)
					sel = 1;
				redraw = 1;
			}

			if(st->joyy != 0)
			{
				if(++joycnt > 5)
				{
					if(st->joyy > 0)
						sel ++;
					if(st->joyy < 0)
						sel --;
	
					if(sel < 1)
						sel = c;
					if(sel > c)
						sel = 1;
					redraw = 1;
					joycnt = 0;
				}
			}
			else
				joycnt = 0;

			if (pressed & CONT_A)
			{
				switch(sel)
				{
					case 1:
						DrawColorBars(screen);
						break;
					case 2:
						Draw601ColorBars(screen);
						break;
					case 3:
						DrawGrid(screen);
						break;
					case 4:
						DrawLinearity(screen);
						break;
					case 5:
						DropShadowTest(screen);
						break;
					case 6:
						LagTest(screen);
						break;
					case 7:
						DrawStripes(screen);
						break;
					case 8:
						DrawCheckBoard(screen);
						break;
					case 9:
						ChangeResolution(&screen, &W, &H, &video_flags, &video_bits);
						break;
					case 10:
						DrawCredits(screen);
						break;

				}              
				redraw = 1;
			}
		}
		MAPLE_FOREACH_END()

		ScaleScreen(screen);
	}

	if(buff320)
	{
		SDL_FreeSurface(buff320);
		buff320 = NULL;
	}

	Mix_CloseAudio();
    	FreeImage(&title);
	exit(0);

	return(0);
}

/****************************** Auxiliary functions ******************************/

inline static void DStringS(SDL_Surface *screen, char *str, int x, int y, int r, int g, int b, int a)
{
 	stringRGBA(screen, x+1, y+1, str, 0x00, 0x00, 0x00, a);
 	stringRGBA(screen, x, y, str, r, g, b, a);
}

inline static void DStringB(SDL_Surface *screen, char *str, int x, int y, int r, int g, int b, int bgcolor )
{
	int		len = 0;
	
	len = strlen(str) * fontW + 4;
	SDL_Rect rcDest = { x, y, len, fontH + 3 };
 	SDL_FillRect(screen, &rcDest, bgcolor);
 	stringRGBA(screen, x + 2, y + 2, str, r, g, b, 0xFF);
}

inline static bitmap LoadImage(char *image_path)
{
	SDL_Surface *image = NULL, *opt = NULL;
	
	image = SDL_LoadBMP(image_path);
	if (!image)
	{
		fprintf (stderr, "Could not load: %s\n",  image_path );
		return NULL;
	}

    	/* Convert the image to the video format (maps colors) */
	opt = SDL_DisplayFormat(image);
	SDL_FreeSurface(image);
	if (opt == NULL)
	{
		fprintf (stderr, "Could not convert: %s\n",  image_path );
		return NULL;
	}

    	return opt;
}

inline static bitmap LoadImageWAlpha(char *image_path, Uint32 R, Uint32 G, Uint32 B)
{
    	SDL_Surface *image = NULL, *opt = NULL;
	
	image = SDL_LoadBMP(image_path);
	if (!image)
	{
		fprintf (stderr, "Could not load: %s\n",  image_path );
		return NULL;
	}

    	SDL_SetColorKey(image, SDL_SRCCOLORKEY, SDL_MapRGB(image->format, R, G, B)); 
    	/* Convert the image to the video format (maps colors) */
	opt = SDL_DisplayFormat(image);
	SDL_FreeSurface(image);
	if (opt == NULL)
	{
		fprintf (stderr, "Could not convert: %s\n",  image_path );
		return NULL;
	}

    	return opt;
}

inline static int DrawImage(SDL_Surface *surface, bitmap image, int x_pos, int y_pos )
{
    	if(!image)
        	return 0;

	SDL_Rect rcDest = { x_pos, y_pos, 0, 0 };
	SDL_BlitSurface ( image, NULL, surface, &rcDest );
	return 1;
}

inline static void FreeImage(bitmap *image)
{
    	if(*image)
    	{
        	SDL_FreeSurface (*image);
        	*image = NULL;
    	}
}

inline static void LineDoubler(SDL_Surface *target, SDL_Surface *source)
{
	int x, y;
    	void *prevline = NULL, *nextline = NULL;

	if (SDL_MUSTLOCK(target)) SDL_LockSurface(target);
	if (SDL_MUSTLOCK(source)) SDL_LockSurface(source);

	for(y = 0; y < source->h; y++) 
    	{
        	prevline = (void *)(target->pixels + 2560*y);
        	nextline = (void *)(target->pixels + 2560*y + 1280);
		for(x = 0; x < source->w; x++) 
		{
    			void *pos = (void *)(target->pixels + 2560*y + 4*x);
    			Uint16 src_p = *(Uint16 *)(source->pixels + 640*y + 2*x);

			*(Uint16 *)(pos) = src_p;
			*(Uint16 *)(pos + 2) = src_p;
		}
		if(vmode == FAKE_640)
        		memcpy(nextline, prevline, 1280);
    	}

	if (SDL_MUSTLOCK(target)) SDL_UnlockSurface(target);
	if (SDL_MUSTLOCK(source)) SDL_UnlockSurface(source);
}

inline static void ScaleScreen(SDL_Surface *screen)
{
	if(vmode == FAKE_640 || vmode == FAKE_640_SL)
	{
		SDL_Surface *rscreen = SDL_GetVideoSurface();
		LineDoubler(rscreen, screen);
 		SDL_Flip(rscreen);
	}
	else
 		SDL_Flip(screen);
	//vid_waitvbl();
}

/*
	Uint32  start, end;

	start = SDL_GetTicks();
	while(start == SDL_GetTicks());

	end = SDL_GetTicks() - 1;
	printf("%i ms\n", end-start);
*/

inline static void ClearScreen(SDL_Surface *screen)
{
        memset(screen->pixels, 0, screen->pitch*screen->h);
	//SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
}

/**************************** End Auxiliary functions *****************************/

void DrawColorBars(SDL_Surface *screen)
{
	int done = 0;
    	bitmap  wall;
    
    	wall = LoadImage("/rd/color.bmp");
    	if(!wall)
        	return;

	if(!DrawImage(screen, wall, 0, 0 ))
		return;

	while(!done)
	{
		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
			
		if (st->buttons & CONT_START)
			done =  1;

		MAPLE_FOREACH_END()
		ScaleScreen(screen);
	}
    	FreeImage(&wall);

	return;
}

void Draw601ColorBars(SDL_Surface *screen)
{
	int done = 0;
    	bitmap  wall;
    
    	wall = LoadImage("/rd/601701cb.bmp");
    	if(!wall)
        	return;

	if(!DrawImage(screen, wall, 0, 0 ))
		return;

	while(!done)
	{
		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
			
		if (st->buttons & CONT_START)
			done =  1;

		MAPLE_FOREACH_END()
		ScaleScreen(screen);
	}
    	FreeImage(&wall);

	return;
}

void DrawGrid(SDL_Surface *screen)
{
	int done = 0;
    	bitmap  wall;
    
    	wall = LoadImage("/rd/grid.bmp");
    	if(!wall)
        	return;

	if(!DrawImage(screen, wall, 0, 0 ))
		return;    

	while(!done)
	{
		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
			
		if (st->buttons & CONT_START)
			done =  1;

		MAPLE_FOREACH_END()
		ScaleScreen(screen);
	}
    	FreeImage(&wall);

	return;
}


void DrawLinearity(SDL_Surface *screen)
{
	int done = 0, redraw = 1, gridpattern = 1, showgrid = 1;
    	Uint16  oldbuttons = 0xffff, pressed;
    	bitmap  circles, grid, gridd;
    
    	circles = LoadImageWAlpha("/rd/circles.bmp", 0, 0, 0);
    	if(!circles)
        	return;

    	grid = LoadImage("/rd/circles_grid.bmp");
    	if(!grid)
        	return;

    	gridd = LoadImage("/rd/circles_griddot.bmp");
    	if(!gridd)
        	return;
    
	while(!done)
	{
		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
	    	pressed = st->buttons & ~oldbuttons;
		oldbuttons = st->buttons;
			
		if (pressed & CONT_A)
		{
			showgrid = !showgrid;
			redraw = 1;
		}

		if (pressed & CONT_B)
		{
			if(gridpattern == 1)
				gridpattern = 2;
			else
				gridpattern = 1;
			if(showgrid)
				redraw = 1;
		}

		if (pressed & CONT_START)
			done =  1;

		MAPLE_FOREACH_END()

		if(redraw)
		{            
            		ClearScreen(screen);
			if(showgrid)
			{
				if(gridpattern == 1)
				{
					if(!DrawImage(screen, grid, 0, 0 ))
						return;    
				}
				else
				{
					if(!DrawImage(screen, gridd, 0, 0 ))
						return;    
				}
			}

			if(!DrawImage(screen, circles, 0, 0 ))
				return;    

			redraw = 0;
		}

		ScaleScreen(screen);
	}
    	FreeImage(&grid);
    	FreeImage(&gridd);
    	FreeImage(&circles);
}

void DropShadowTest(SDL_Surface *screen)
{
    	char msg[50];
	int done = 0, frame = 1, invert = 0, text = 0;
    	Uint16  oldbuttons = 0xffff, pressed, changeback = 0;
    	int x = 0, y = 0, lx = 0, ly = 0, back = 0, redraw = 0;
    	bitmap wall, shadow, stripespos, checkpos, bgimage;

    	wall = LoadImage("/rd/motoko.bmp");
    	if(!wall)
        	return;

    	checkpos = LoadImage("/rd/checkpos.bmp");
    	if(!checkpos)
        	return;

    	stripespos = LoadImage("/rd/stripespos.bmp");
    	if(!stripespos)
        	return;

    	shadow = LoadImageWAlpha("/rd/shadow.bmp", 255, 0, 255 );
    	if(!shadow)
        	return;

	bgimage = wall;
	
	redraw = 1;
	while(!done)
	{
		if(!redraw)
		{
			SDL_Rect rect = { lx, ly, 32, 32 };
			SDL_BlitSurface (bgimage, &rect, screen, &rect);
		}
		else
		{
        		if(!DrawImage(screen, bgimage, 0, 0))
		    		return;
			redraw = 0;
		}

        	if(text)
        	{
			if(text == 1)
			{
				SDL_Rect trect = { 200, 10, 110, 12 };
				SDL_BlitSurface (bgimage, &trect, screen, &trect);
			}
			else
            			DStringB(screen, msg, 200, 10, 0x00, 0xFF, 0x00, 0x00 );
            		text --;
        	}
        
        	if(frame == invert)
        	{
            		if(!DrawImage(screen, shadow, x, y))
		        	return;
            		frame = !frame;
			lx = x;
			ly = y;
        	}
        	else
            		frame = !frame;

		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
	    	pressed = st->buttons & ~oldbuttons;
		oldbuttons = st->buttons;
			
        	if (st->buttons & CONT_DPAD_UP)
		{
			y --;
			if(y < 0)
				y = 0;
		}

		if (st->buttons & CONT_DPAD_DOWN)
		{
			y ++;
			if(y > 234 - 32)
				y = 234 - 32;
		}

        	if (st->buttons & CONT_DPAD_LEFT)
		{
			x --;
			if(x < 0)
				x = 0;
		}

		if (st->buttons & CONT_DPAD_RIGHT)
		{
			x ++;
			if(x > 320 - 32)
				x = 320 - 32;
		}

		// Joystick
		if(st->joyx != 0)
		{
			x += st->joyx/10;
			if(x > 320 - 32)
				x = 320 - 32;
			if(x < 0)
				x = 0;
		}

		if(st->joyy != 0)
		{
			y += st->joyy/10;
			if(y > 234 - 32)
				y = 234 - 32;
			if(y < 0)
				y = 0;
		}

		if (pressed & CONT_START)
			done =  1;

        	if (pressed & CONT_A)
		{
			if(back > 0)
				back --;
			else
				back = 2;
			redraw = 1;
			changeback = 1;
		}

        	if (pressed & CONT_B)
		{
			if(back < 2)
				back ++;
			else
				back = 0;
			redraw = 1;
			changeback = 1;
		}

        	if (st->buttons & CONT_Y && st->buttons & CONT_X)
		{
			if(back < 2)
				back ++;
			else
				back = 0;
			changeback = 1;
		}

		if(changeback)
		{
			switch(back)
			{
				case 0:
					bgimage = wall;
					break;
				case 1:
					bgimage = checkpos;
					break;
				case 2:
					bgimage = stripespos;
					break;
			}
			changeback = 0;
		}

        	if (pressed & CONT_X)
		{
            		invert = !invert;           
            		if(invert)
                		sprintf(msg, "Shadow on odd frames ");
            		else
                		sprintf(msg, "Shadow on even frames");
                
            		text = 60;
        	}

		MAPLE_FOREACH_END()
		ScaleScreen(screen);
	}
    	FreeImage(&wall);
    	FreeImage(&checkpos);
    	FreeImage(&stripespos);
    	FreeImage(&shadow);

	return;
}

void LagTest(SDL_Surface *screen)
{
	int 		clicks[10], done = 0, view = 0, speed = 1, change = 1, erase = 0;
	int		x, y, x2, y2, channel = -1, drawoffset = 0, audio = 1, pos = 0;
    	Uint16  	oldbuttons = 0xffff, pressed;
    	bitmap  	sprite, spriteneg;
	SDL_Rect 	rect = { 0, 0, 32, 32 };
	Mix_Chunk 	*sound = NULL;
    
    	sprite = LoadImageWAlpha("/rd/lag-per.bmp", 0, 0, 0);
    	if(!sprite)
		return;

    	spriteneg = LoadImage("/rd/lag-full.bmp");
    	if(!spriteneg)
		return;

	 
	sound = Mix_LoadWAV("/rd/beep.wav");
	if(!sound)
		fprintf(stderr, "Unable to load WAV file: %s", Mix_GetError());

	x = 144;
	y = 60;
	x2 = 108;
	y2 = 96;


    	ClearScreen(screen);
	DStringB(screen, "Press the \"A\" button when the sprite is aligned.", 40, 190, 0xFF, 0xFF, 0xFF, SDL_MapRGB(screen->format, 0, 0, 0));
	DStringB(screen, "A negative value means you pressed \"A\" before   ", 40, 200, 0xFF, 0xFF, 0xFF, SDL_MapRGB(screen->format, 0, 0, 0));
	DStringB(screen, "they intersected. \"B\" button toggles horz/vert.", 40, 210, 0xFF, 0xFF, 0xFF, SDL_MapRGB(screen->format, 0, 0, 0));

	while(!done)
	{
		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
        	pressed = st->buttons & ~oldbuttons;
		oldbuttons = st->buttons;
			
        	if(pressed & CONT_A)
		{
			if(change)
			{
				clicks[pos] = (y - 96) * speed;
				drawoffset = 1;
				change  = 0;
			}
		}
        
        	if(pressed & CONT_B)
		{
			view ++;
			if(view > 2)
				view = 0;
		}
        
		if(pressed & CONT_Y)
			audio = !audio;

		if(pressed & CONT_START)
			done =  1;

		MAPLE_FOREACH_END()

		// Erase previous circles
		if(erase || view == 0 || view == 2)
		{
			rect.x = x;
			rect.y = y;
			SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 0, 0, 0));
		}

		if(erase || view == 1 || view == 2)
		{
			rect.x = x2;
			rect.y = y2;
			SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 0, 0, 0));
		}

		if(erase)
			erase = 0;

		if(y > 132)
		{
			speed = -1;
			change = 1;
		}

		if(y < 60)
		{
			speed = 1;
			change = 1;
		}

		y += speed;
		x2 += speed;

		if(y == 96)
		{
                	if(!DrawImage(screen, spriteneg, 144, 96 ))
				return;
		}
		else
		{
                	if(!DrawImage(screen, sprite, 144, 96 ))
				return;
		}

		if(view == 0 || view == 2)
		{
                	if(!DrawImage(screen, sprite, x, y))
				return;
			erase = 1;
		}

		if(view == 1 || view == 2)
		{
                	if(!DrawImage(screen, sprite, x2, y2))
				return;
			erase = 1;
		}

		if(drawoffset)
		{
			int px, py;
			char msg[50];

			px = 10;
			py = pos * (fontH + 3) + 20;

			sprintf(msg, "Offset %0.2d:", pos + 1);
			DStringB(screen, msg, px, py, 0xFF, 0xFF, 0xFF, SDL_MapRGB(screen->format, 0, 0, 0));

			px += (strlen(msg) + 1) * fontW;
			sprintf(msg, "%2d  ", clicks[pos]);
			if(clicks[pos] >= 0)
			{
				if(clicks[pos] == 0)
					DStringB(screen, msg, px, py, 0x00, 0xFF, 0x00, SDL_MapRGB(screen->format, 0, 0, 0));
				else
					DStringB(screen, msg, px, py, 0xFF, 0xFF, 0xFF, SDL_MapRGB(screen->format, 0, 0, 0));
			}
			else
				DStringB(screen, msg, px, py, 0xFF, 0x0, 0x0, SDL_MapRGB(screen->format, 0, 0, 0));

			if(clicks[pos] >= 0)
				pos++;

			if(pos > 9)
				done = 1;

			drawoffset = 0;
		}

		ScaleScreen(screen);

		if(y == 96 && audio)
		{
			channel = Mix_PlayChannel(-1, sound, 0);
			if(channel == -1) 
				fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
		}
	}


	if(channel != -1)
		while(Mix_Playing(channel) != 0)

	if(sound)
		Mix_FreeChunk(sound);

	if(pos > 9)
	{
		int	i = 0, total = 0;
		double  res = 0;
    		bitmap  wall;
		char 	msg[20];
    
		done = 0;
    		wall = LoadImage("/rd/back.bmp");
    		if(!wall)
        		return;
    		if(!DrawImage(screen, wall, 0, 0 ))
        		return;

		for(i = 0; i < 10; i++)
		{
			int px, py;

			px = 80;
			py = i * (fontH + 3) + 60;

			sprintf(msg, "%2d", clicks[i]);
			if(clicks[i] >= 0)
			{
				if(clicks[i] == 0)
					DStringS(screen, msg, px, py, 0x00, 0xFF, 0x00, 0xFF);
				else
					DStringS(screen, msg, px, py, 0xFF, 0xFF, 0xFF, 0xFF);
			}
			else
				DStringS(screen, msg, px, py, 0xFF, 0x0, 0x0, 0xFF);

			if(clicks[i] >= 0)
				total += clicks[i];

		}

		res = (double)total / 10.0;
		sprintf(msg, "%d/10 = %0.2f milliseconds", total, res);
		DStringS(screen, "+", 70, 100, 0xFF, 0x0, 0x0, 0xFF);
		DStringS(screen, "----", 65, 60 + (fontH+3)*10, 0xFF, 0x0, 0x0, 0xFF);
		DStringS(screen, msg, 70, 60 + (fontH+3)*11, 0xFF, 0xFF, 0xFF, 0xFF);

		if(total < 5)
			DStringS(screen, "EXCELLENT REFLEXES!", 200, 120, 0x0, 0xFF, 0x0, 0xFF);
		if(total == 0)
			DStringS(screen, "INCREDIBLE REFLEXES!!", 200, 120, 0x0, 0xFF, 0x0, 0xFF);

		while(!done)
		{
			MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
				
			if (st->buttons & CONT_START)
				done =  1;
	
			MAPLE_FOREACH_END()
			ScaleScreen(screen);
		}
	}

    	FreeImage(&sprite);
    	FreeImage(&spriteneg);
}

void DrawStripes(SDL_Surface *screen)
{
    	int     done = 0, field = 0, alternate = 0,
	    	frame = 0, dframe = 0, invert = 0;
    	Uint16  oldbuttons = 0xffff, pressed;
    	bitmap  stripespos, stripesneg;
    
    	stripespos = LoadImage("/rd/stripespos.bmp");
    	if(!stripespos)
        	return;

    	stripesneg = LoadImage("/rd/stripesneg.bmp");
    	if(!stripesneg)
        	return;

    	if(!DrawImage(screen, stripespos, 0, 0 ))
        	return;

	while(!done)
	{        
        	if(alternate || invert)
        	{
            		if(field == 1)
            		{
                		if(!DrawImage(screen, stripespos, 0, 0 ))
    		        		return;
                		field = 0;
            		}
            		else
            		{
                		if(!DrawImage(screen, stripesneg, 0, 0 ))
    		        		return;
                		field = 1;
            		}
            		invert = 0;
        	}        

        	if(dframe)
        	{
            		char msg[20];
	
            		sprintf(msg, "Frame: %0.2d", frame);
            		DStringB(screen, msg, 250, 10, 0x00, 0xFF, 0x00, 0x00 );
            		frame ++;
            		if(frame > 59)
                		frame = 0;
        	}

		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
        	pressed = st->buttons & ~oldbuttons;
		oldbuttons = st->buttons;
			
        	if (pressed & CONT_A)
			alternate = !alternate;
        
        	if (pressed & CONT_B && !alternate)
            		invert = 1;       

        	if (pressed & CONT_X)
        	{
            		dframe = !dframe;
	    		frame = 0;            
	    		if(!dframe)
	    		{
				field = !field;
				invert = 1;
	    		}
        	}

		if (pressed & CONT_START)
			done =  1;

		MAPLE_FOREACH_END()

		ScaleScreen(screen);
	}
    	FreeImage(&stripespos);
    	FreeImage(&stripesneg);
}

void DrawCheckBoard(SDL_Surface *screen)
{
    	int     done = 0, field = 0, alternate = 0, frame = 0, dframe = 0, invert = 0;
    	Uint16  oldbuttons = 0xffff, pressed;
    	bitmap  checkpos, checkneg;
    
    	checkpos = LoadImage("/rd/checkpos.bmp");
    	if(!checkpos)
        	return;

    	checkneg = LoadImage("/rd/checkneg.bmp");
    	if(!checkneg)
        	return;

    	if(!DrawImage(screen, checkpos, 0, 0 ))
        	return;
	while(!done)
	{        
        	if(alternate || invert)
        	{
            		if(field == 1)
            		{
                		if(!DrawImage(screen, checkpos, 0, 0 ))
    		        		return;
                		field = 0;
            		}
            		else
            		{
                		if(!DrawImage(screen, checkneg, 0, 0 ))
    		        		return;
                		field = 1;
            		}
	    		invert = 0;
        	}

        	if(dframe)
        	{
            		char msg[20];
	
            		sprintf(msg, "Frame: %0.2d", frame);
            		DStringB(screen, msg, 250, 10, 0x00, 0xFF, 0x00, 0x00 );
            		frame ++;
            		if(frame > 59)
                		frame = 0;
        	}

		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
        	pressed = st->buttons & ~oldbuttons;
		oldbuttons = st->buttons;
			
        	if (pressed & CONT_A)
			alternate = !alternate;
        
        	if (pressed & CONT_B && !alternate)
			invert = 1;

        	if (pressed & CONT_X)
        	{
            		dframe = !dframe;
	    		frame = 0;            
	    		if(!dframe)
	    		{
				field = !field;
				invert = 1;
	    		}
        	}

		if (pressed & CONT_START)
			done =  1;

		MAPLE_FOREACH_END()
		ScaleScreen(screen);
	}
    	FreeImage(&checkpos);
    	FreeImage(&checkneg);
}

void ChangeResolution(SDL_Surface **screen, int *W, int *H, Uint32 *video_flags, Uint32 *video_bits)
{
	int lastw;

    	ClearScreen(SDL_GetVideoSurface());
	SDL_Flip(SDL_GetVideoSurface());

	lastw = *W;

	vmode ++;

	if(vmode > FAKE_640_SL)
		vmode = NATIVE_320;

	if(vmode > NATIVE_640)
	{
		if(vcable != CT_VGA)
			vmode = NATIVE_320;
	}

	switch(vmode)
	{
		case NATIVE_320:
			*W = 320;
			*H = 240;
			break;
		case FAKE_640:
			*W = 640;	
			*H = 480;
			break;
		case NATIVE_640:
			*W = 640;
			*H = 480;
			break;
		case FAKE_640_SL:
			*W = 640;
			*H = 480;
			break;
	}

	if(lastw != *W)
	{
		*screen = SDL_SetVideoMode(*W, *H, *video_bits, *video_flags);
		if (*screen == NULL)
		{
			fprintf(stderr, "Couldn't set %dx%dx%d video mode: %s\n",
					*W, *H, *video_bits, SDL_GetError());
			exit(1);
		}
	}

	*screen = SDL_GetVideoSurface();

	if(vmode == FAKE_640 || vmode == FAKE_640_SL)
	{
        	SDL_Surface * tmp;
		if(buff320)
		{
			SDL_FreeSurface(buff320);
			buff320 = NULL;
		}
		tmp = SDL_CreateRGBSurface(SDL_HWSURFACE, 320, 240, 16, 0, 0, 0, 0);
        	/* Convert the buffer to the video format (maps colors) */
	    	buff320 = SDL_DisplayFormat(tmp);
	    	SDL_FreeSurface(tmp);
	    	if (buff320 == NULL)	 
		    fprintf (stderr, "Could not convert 320 buffer\n" );		    
	    
		*screen = buff320;
	}

}

void DrawCredits(SDL_Surface *screen)
{
	int done = 0, x = 40, y = 50;
    	bitmap  wall;
    
    	wall = LoadImage("/rd/back.bmp");
    	if(!wall)
        	return;

	if(!DrawImage(screen, wall, 0, 0 ))
		return;

	DStringS(screen, "Code and Patterns:", x, y, 0x0, 0xFF,  0x0, 0xFF); y += 10;
	DStringS(screen, "Artemio Urbina", x+5, y, 0xFF, 0xFF, 0xFF, 0xFF); y += 10;
	DStringS(screen, "Advisor:", x, y, 0x0, 0xFF, 0x0, 0xFF);y += 10;
	DStringS(screen, "Tobias W. Reich", x+5, y, 0xFF, 0xFF, 0xFF, 0xFF);y += 10;
	DStringS(screen, "Menu Pixel Art:", x, y, 0x0, 0xFF, 0x0, 0xFF);y += 10;
	DStringS(screen, "Asher", x+5, y, 0xFF, 0xFF, 0xFF, 0xFF);y += 10;
	DStringS(screen, "SDK", x, y, 0x0, 0xFF, 0x0, 0xFF);y += 10;
	DStringS(screen, "KallistiOS + SDL", x+5, y, 0xFF, 0xFF, 0xFF, 0xFF);y += 10;
	DStringS(screen, "Toolchain built with:", x, y, 0x0, 0xFF, 0x0, 0xFF);y += 10;
	DStringS(screen, "https://github.com/losinggeneration/buildcross", x+5, y, 0xFF, 0xFF, 0xFF, 0xFF);y += 10;
	DStringS(screen, "Info on using this suite:", x, y, 0x0, 0xFF, 0x0, 0xFF);y += 10;
	DStringS(screen, "http://junkerhq.net/xrgb", x+5, y, 0xFF, 0xFF, 0xFF, 0xFF);

	while(!done)
	{
		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, st)
			
		if (st->buttons & CONT_START)
			done =  1;

		MAPLE_FOREACH_END()
		ScaleScreen(screen);
	}
    	FreeImage(&wall);
}


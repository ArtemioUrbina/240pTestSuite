/* 
 * 240p Test Suite
 * Copyright (C)2014 Artemio Urbina (PC Engine/TurboGrafx-16)
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
 
/*
            DEFINES
*/

#define MAIN_BACKGROUND "graphics/mainbg.pcx"
#define MAIN_BACKMAP 	"graphics/mainbg.fmp"

#define MAIN_BG_512		"graphics/back512.pcx"
#define MAIN_MAP_512 	"graphics/back512.fmp"

#define FS_MAP 			"graphics/fullscreen.fmp"
#define CHECK_TILES 	"graphics/check.pcx"

#define PLUGE_MAP 		"graphics/pluge.fmp"
#define PLUGE_TILES 	"graphics/pluge.pcx"

#define COLOR_MAP 		"graphics/color_BAT.bin"
#define COLOR_TILES 	"graphics/color_DATA.bin"
#define COLOR_PAL 		"graphics/color_PAL.bin"

#define GRID320_240_MAP "graphics/grid320_240.fmp"
#define GRID320_224_MAP "graphics/grid320_224.fmp"
#define GRID256_240_MAP	"graphics/grid256_240.fmp"
#define GRID256_224_MAP	"graphics/grid256_224.fmp"
#define GRID512_240_MAP	"graphics/grid512_240.fmp"
#define GRID512_224_MAP	"graphics/grid512_224.fmp"
#define GRID_TILES 		"graphics/grid.pcx"


#define FONT            "graphics/font.pcx"


/*
            GRAPHICS
*/

#incchr(my_font, FONT, 32, 3);

#incbin(MB_map, MAIN_BACKMAP);
#incchr_ex(MB_bg, MAIN_BACKGROUND, 0, 0, 20, 7, 0);
#incpal(MB_pal, MAIN_BACKGROUND, 0, 1);

#incbin(MB512_map, MAIN_MAP_512);
#incchr_ex(MB512_bg, MAIN_BG_512, 0, 0, 20, 10, 0);
#incpal(MB512_pal, MAIN_BG_512, 0, 1);

/*
			TEST 
*/

#incbin(fs_map, FS_MAP);
#incchr_ex(check_bg, CHECK_TILES, 0, 0, 1, 1, 0);
#incpal(check_pal, CHECK_TILES, 0, 1);

#incchr_ex(hstripes_bg, CHECK_TILES, 8, 0, 1, 1, 0);
#incchr_ex(vstripes_bg, CHECK_TILES, 0, 8, 1, 1, 0);
#incchr_ex(white_bg, CHECK_TILES, 8, 8, 1, 1, 0);

/*
			PATTERNS
*/

#incbin(pluge_map, PLUGE_MAP);
#incchr_ex(pluge_bg, PLUGE_TILES, 0, 0, 20, 3, 0);
#incpal(pluge_pal, PLUGE_TILES, 0, 1);

#incchr_ex(grid_bg, GRID_TILES, 0, 0, 2, 4, 0);
#incpal(grid_pal, GRID_TILES, 0, 1);

#incbin(grid320_240_map, GRID320_240_MAP);
#incbin(grid320_224_map, GRID320_224_MAP);

#incbin(grid256_224_map, GRID256_224_MAP);
#incbin(	, GRID256_240_MAP);

#incbin(grid512_224_map, GRID512_224_MAP);
#incbin(grid512_240_map, GRID512_240_MAP);

#incbin(color_map, COLOR_MAP);
#incbin(color_bg, COLOR_TILES);
#incbin(color_pal, COLOR_PAL);
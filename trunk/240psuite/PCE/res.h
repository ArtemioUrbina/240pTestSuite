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

#define FS_MAP 			"graphics/fullscreen.fmp"
#define CHECK_TILES 	"graphics/check.pcx"

#define GRID320_240_MAP "graphics/grid320_240.fmp"
#define GRID320_224_MAP "graphics/grid320_224.fmp"
#define GRID256_240_MAP	"graphics/grid256_240.fmp"
#define GRID256_224_MAP	"graphics/grid256_224.fmp"
#define GRID512_240_MAP	"graphics/grid512_240.fmp"
#define GRID512_224_MAP	"graphics/grid512_224.fmp"
#define GRID_TILES 		"graphics/grid.pcx"

/*
            GRAPHICS
*/

#incbin(MB_map, MAIN_BACKMAP);
#incchr_ex(MB_bg, MAIN_BACKGROUND, 0, 0, 20, 7, 0);
#incpal(MB_pal, MAIN_BACKGROUND, 0, 1);

/*
			TEST 
*/

#incbin(fs_map, FS_MAP);
#incchr_ex(check_bg, CHECK_TILES, 0, 0, 1, 1, 0);
#incpal(check_pal, CHECK_TILES, 0, 1);

/*
			PATTERNS
*/

#incchr_ex(grid_bg, GRID_TILES, 0, 0, 2, 4, 0);
#incpal(grid_pal, GRID_TILES, 0, 1);

#incbin(grid320_240_map, GRID320_240_MAP);
#incbin(grid320_224_map, GRID320_224_MAP);

#incbin(grid256_224_map, GRID256_224_MAP);
#incbin(grid256_240_map, GRID256_240_MAP);

#incbin(grid512_224_map, GRID512_224_MAP);
#incbin(grid512_240_map, GRID512_240_MAP);

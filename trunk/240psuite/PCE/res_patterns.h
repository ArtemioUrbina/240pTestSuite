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

#ifdef CDROM1
/*
            GRAPHICS
*/

#incchr(my_font, "graphics/font.pcx", 32, 3);

#incbin(fs_map, "graphics/fullscreen.fmp");
#incchr_ex(check_bg, "graphics/check.pcx", 0, 0, 1, 1, 0);
#incpal(check_pal, "graphics/check.pcx", 0, 1);

#incchr_ex(hstripes_bg, "graphics/check.pcx", 8, 0, 1, 1, 0);
#incchr_ex(vstripes_bg, "graphics/check.pcx", 0, 8, 1, 1, 0);
#incchr_ex(white_bg, "graphics/check.pcx", 8, 8, 1, 1, 0);

#incchr_ex(cgrid_bg, "graphics/circles_grid.pcx", 0, 0, 1, 1, 0);

#include "graphics.h"

#endif


/*
			PATTERNS
*/

#ifndef CDROM1

#incbin(pluge_map, "graphics/pluge.fmp");
#incchr_ex(pluge_bg, "graphics/pluge.pcx", 0, 0, 2, 23, 0);

#incchr_ex(grid_bg, "graphics/grid.pcx", 0, 0, 2, 4, 0);

#incbin(grid320_240_map, "graphics/grid320_240.fmp");
#incbin(grid320_224_map, "graphics/grid320_224.fmp");

#incbin(grid256_224_map, "graphics/grid256_224.fmp");
#incbin(grid256_240_map	, "graphics/grid256_240.fmp");

#incbin(grid512_224_map, "graphics/grid512_224.fmp");
#incbin(grid512_240_map, "graphics/grid512_240.fmp");

#incbin(color_map, "graphics/color_BAT.bin");
#incbin(color_bg, "graphics/color_DATA.bin");

#incbin(gray_map, "graphics/gray.fmp");
#incchr_ex(gray_bg, "graphics/gray.pcx", 0, 0, 4, 2, 0);

#incbin(cb601_map, "graphics/cb601.fmp");
#incchr_ex(cb601_bg, "graphics/cb601.pcx", 0, 0, 2, 34, 0);

#incbin(colorbleed_map, "graphics/colorbleed.fmp");
#incchr_ex(colorbleed_bg, "graphics/colorbleed.pcx", 0, 0, 2, 3, 0);
#incchr_ex(colorbleedchk_bg, "graphics/colorbleedcheck.pcx", 0, 0, 2, 3, 0);

#incbin(SMPTE75_map, "graphics/SMPTE75.fmp");
#incchr_ex(SMPTE75_bg, "graphics/SMPTE75.pcx", 0, 0, 2, 27, 0);

#incbin(linearity240_map, "graphics/linearity240.fmp");
#incchr_ex(linearity240_bg, "graphics/linearity240.pcx", 0, 0, 4, 49, 0);

#incbin(linearity224_map, "graphics/linearity224.fmp");
#incchr_ex(linearity224_bg, "graphics/linearity224.pcx", 0, 0, 2, 71, 0);

#incbin(linearity256_map, "graphics/linearity256.fmp");
#incchr_ex(linearity256_bg, "graphics/linearity256.pcx", 0, 0, 4, 41, 0);

#incbin(sharpness_map, "graphics/sharpness.fmp");
#incchr_ex(sharpness_bg, "graphics/sharpness.pcx", 0, 0, 4, 51, 0);

#incchr_ex(ire100_bg, "graphics/100IRE.pcx", 0, 0, 2, 2, 0);
#endif

#incbin(ire100_map, "graphics/100IRE.fmp");


/*
		Palettes
*/
#incpal(grid_pal, "graphics/grid.pcx", 0, 1);
#incbin(color_pal, "graphics/color_PAL.bin");
#incpal(gray_pal, "graphics/gray.pcx", 0, 1);
#incpal(cb601_pal, "graphics/cb601.pcx", 0, 1);
#incpal(colorbleed_pal, "graphics/colorbleed.pcx", 0, 1);
#incpal(SMPTE75_pal, "graphics/SMPTE75.pcx", 0, 1);
#incpal(SMPTE100_pal, "graphics/SMPTE100.pcx", 0, 1);
#incpal(linearity240_pal, "graphics/linearity240.pcx", 0, 1);
#incpal(linearity224_pal, "graphics/linearity224.pcx", 0, 1);
#incpal(linearity256_pal, "graphics/linearity256.pcx", 0, 1);
#incpal(pluge_pal, "graphics/pluge.pcx", 0, 1);
#incpal(sharpness_pal, "graphics/sharpness.pcx", 0, 1);

#ifdef CDROM1
#incpal(MB_pal, "graphics/mainbg.pcx", 0, 1);
#incpal(MB512_pal, "graphics/back512.pcx", 0, 1);
#incpal(SD_pal, "graphics/SD.pcx", 0, 1);
#endif




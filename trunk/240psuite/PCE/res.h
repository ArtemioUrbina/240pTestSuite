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
            GRAPHICS
*/

#incchr(my_font, "graphics/font.pcx", 32, 3);

#incbin(MB_map, "graphics/mainbg.fmp");
#incchr_ex(MB_bg, "graphics/mainbg.pcx", 0, 0, 20, 7, 0);
#incpal(MB_pal, "graphics/mainbg.pcx", 0, 1);

#incbin(MB512_map, "graphics/back512.fmp");
#incchr_ex(MB512_bg, "graphics/back512.pcx", 0, 0, 4, 36, 0);
#incpal(MB512_pal, "graphics/back512.pcx", 0, 1);

#incbin(motoko_map, "graphics/motoko_BAT.bin");
#incbin(motoko_bg, "graphics/motoko_DATA.bin");
#incbin(motoko_pal, "graphics/motoko_PAL.bin");

#incbin(n_map, "graphics/N_BAT.bin");
#incbin(n_bg, "graphics/N_DATA.bin");
#incbin(n_pal, "graphics/N_PAL.bin");

/*
			TEST 
*/

#incbin(fs_map, "graphics/fullscreen.fmp");
#incchr_ex(check_bg, "graphics/check.pcx", 0, 0, 1, 1, 0);
#incpal(check_pal, "graphics/check.pcx", 0, 1);

#incchr_ex(hstripes_bg, "graphics/check.pcx", 8, 0, 1, 1, 0);
#incchr_ex(vstripes_bg, "graphics/check.pcx", 0, 8, 1, 1, 0);
#incchr_ex(white_bg, "graphics/check.pcx", 8, 8, 1, 1, 0);

/*
			PATTERNS
*/

#incbin(pluge_map, "graphics/pluge.fmp");
#incchr_ex(pluge_bg, "graphics/pluge.pcx", 0, 0, 2, 23, 0);
#incpal(pluge_pal, "graphics/pluge.pcx", 0, 1);

#incchr_ex(grid_bg, "graphics/grid.pcx", 0, 0, 2, 4, 0);
#incpal(grid_pal, "graphics/grid.pcx", 0, 1);

#incbin(grid320_240_map, "graphics/grid320_240.fmp");
#incbin(grid320_224_map, "graphics/grid320_224.fmp");

#incbin(grid256_224_map, "graphics/grid256_224.fmp");
#incbin(grid256_240_map	, "graphics/grid256_240.fmp");

#incbin(grid512_224_map, "graphics/grid512_224.fmp");
#incbin(grid512_240_map, "graphics/grid512_240.fmp");

#incbin(color_map, "graphics/color_BAT.bin");
#incbin(color_bg, "graphics/color_DATA.bin");
#incbin(color_pal, "graphics/color_PAL.bin");

#incbin(gray_map, "graphics/gray.fmp");
#incchr_ex(gray_bg, "graphics/gray.pcx", 0, 0, 4, 2, 0);
#incpal(gray_pal, "graphics/gray.pcx", 0, 1);

#incbin(cb601_map, "graphics/cb601.fmp");
#incchr_ex(cb601_bg, "graphics/cb601.pcx", 0, 0, 2, 34, 0);
#incpal(cb601_pal, "graphics/cb601.pcx", 0, 1);

#incbin(colorbleed_map, "graphics/colorbleed.fmp");
#incchr_ex(colorbleed_bg, "graphics/colorbleed.pcx", 0, 0, 2, 3, 0);
#incchr_ex(colorbleedchk_bg, "graphics/colorbleedcheck.pcx", 0, 0, 2, 3, 0);
#incpal(colorbleed_pal, "graphics/colorbleed.pcx", 0, 1);

#incbin(SMPTE75_map, "graphics/SMPTE75.fmp");
#incchr_ex(SMPTE75_bg, "graphics/SMPTE75.pcx", 0, 0, 2, 27, 0);
#incpal(SMPTE75_pal, "graphics/SMPTE75.pcx", 0, 1);
#incpal(SMPTE100_pal, "graphics/SMPTE100.pcx", 0, 1);

#incbin(linearity240_map, "graphics/linearity240.fmp");
#incchr_ex(linearity240_bg, "graphics/linearity240.pcx", 0, 0, 4, 49, 0);
#incpal(linearity240_pal, "graphics/linearity240.pcx", 0, 1);

#incbin(linearity224_map, "graphics/linearity224.fmp");
#incchr_ex(linearity224_bg, "graphics/linearity224.pcx", 0, 0, 2, 71, 0);
#incpal(linearity224_pal, "graphics/linearity224.pcx", 0, 1);

#incbin(sharpness_map, "graphics/sharpness.fmp");
#incchr_ex(sharpness_bg, "graphics/sharpness.pcx", 0, 0, 4, 51, 0);
#incpal(sharpness_pal, "graphics/sharpness.pcx", 0, 1);

/*
		SPRITES
*/

#incspr(SD_sp, "graphics/SD.pcx", 0, 0, 4, 7);
#incpal(SD_pal, "graphics/SD.pcx", 0, 1);
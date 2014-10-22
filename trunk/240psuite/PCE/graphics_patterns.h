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
 
 
 This version of the suite is compiled with HuC from https://github.com/uli/huc
 
 */
 
#ifdef CDROM
extern char MB_map[];
extern int MB_bg[];
extern int MB_pal[];

extern int SD_sp[];
extern int SD_pal[];
 
extern char fs_map[];
extern int white_bg[];
extern int check_pal[];
#endif

extern char pluge_map[];
extern int pluge_bg[];
extern int pluge_pal[];

extern char color_map[];
extern int color_bg[];
extern int color_pal[];

extern char colorbleed_map[];
extern int colorbleed_bg[];
extern int colorbleedchk_bg[];
extern int colorbleed_pal[];

extern char SMPTE75_map[];
extern int SMPTE75_bg[];
extern int SMPTE75_pal[];
extern int SMPTE100_pal[];

extern char cb601_map[];
extern int cb601_bg[];
extern int cb601_pal[];

extern int grid_bg[];
extern int grid_pal[];

extern char grid256_224_map[];
extern char grid256_240_map[];
extern char grid320_224_map[];
extern char grid320_240_map[];
extern char grid512_224_map[];
extern char grid512_240_map[];

extern char linearity240_map[];
extern int linearity240_bg[];
extern int linearity240_pal[];

extern char linearity224_map[];
extern int linearity224_bg[];
extern int linearity224_pal[];

extern char linearity256_map[];
extern int linearity256_bg[];
extern int linearity256_pal[];

extern char sharpness_map[];
extern int sharpness_bg[];
extern int sharpness_pal[];

extern char gray_map[];
extern int gray_bg[];
extern int gray_pal[];
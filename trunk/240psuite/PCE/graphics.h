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
 
 #ifdef CDROM1
 #include "gdata.h"
 #endif

#ifndef CDROM1
 
extern char MB_map[];
extern int MB_bg[];

extern int SD_sp[];
extern int shadow_sp[];
extern int striped_sp[];

extern int LED_sp[];

extern char MB512_map[];
extern int MB512_bg[];

extern char fs_map[];
extern int white_bg[];

extern int hstripes_bg[];
extern int vstripes_bg[];

extern int check_bg[];

extern char motoko_map[];
extern int motoko_bg[];

extern int palm_sp[];

extern char sonic_map[];
extern int sonic_bg[];

extern char lagback_map[];
extern int lagback_bg[];

extern char cgrid_bg[];


#endif

extern int MB512_pal[];
extern int check_pal[];
extern int motoko_pal[];
extern int SD_pal[];
extern int palm_pal[];
extern int sonic_pal[];
extern int lagback_pal[];
extern int MB_pal[];
extern int n_pal[];



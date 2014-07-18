/* 
 * 240p Test Suite
 * Copyright (C)2014 Artemio Urbina (SNES)
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
 
.include "hdr.asm"

/* has only 40 bytes left */
.section ".rodata1" superfree

back_tiles:
.incbin "back.pic"
back_tiles_end:

gillian_tiles:
.incbin "gillian.pic"
gillian_tiles_end:

grid_tiles:
.incbin "grid.pic"
grid_tiles_end:

motoko_tiles1:
.incbin "motoko.pic" read $6000
motoko_tiles1_end:

.ends

/* Has CC0 bytes free */
.section ".rodata2" superfree

back_map:
.incbin "back.map"
back_map_end:

back_pal:
.incbin "back.pal"
back_pal_end:

grid_map:
.incbin "grid.map"
grid_map_end:

grid240_map:
.incbin "grid240.map"
grid240_map_end:

grid_pal:
.incbin "grid.pal"
grid_pal_end:

gillian_pal:
.incbin "gillian.pal"
gillian_pal_end:

motoko_map:
.incbin "motoko.map"
motoko_map_end:

motoko_pal:
.incbin "motoko.pal"
motoko_pal_end:

SMPTECB75_map:
.incbin "SMPTECB75.map"
SMPTECB75_map_end:

SMPTECB75_pal:
.incbin "SMPTECB75.pal"
SMPTECB75_pal_end:

SMPTECB100_pal:
.incbin "SMPTECB100.pal"
SMPTECB100_pal_end:

color_map:
.incbin "color.map"
color_map_end:

color_pal:
.incbin "color.pal"
color_pal_end:

circles_map:
.incbin "circles.map"
circles_map_end:

circles_pal:
.incbin "circles.pal"
circlesr_pal_end:

pluge_map:
.incbin "pluge.map"
pluge_map_end:

pluge_pal:
.incbin "pluge.pal"
pluge_pal_end:

grayramp_map:
.incbin "grayramp.map"
grayramp_map_end:

grayramp_pal:
.incbin "grayramp.pal"
grayramp_pal_end:

colorbleed_map:
.incbin "colorbleed.map"
colorbleed_map_end:

colorbleed_pal:
.incbin "colorbleed.pal"
colorbleed_pal_end:

cb601701_map:
.incbin "601701cb.map"
cb601701_map_end:

cb601701_pal:
.incbin "601701cb.pal"
cb601701_pal_end:

sharpness_map:
.incbin "sharpness.map"
sharpness_map_end:

sharpness_pal:
.incbin "sharpness.pal"
sharpness_pal_end:
 
fullscreen_map:
.incbin "fullscreen.map"
fullscreen_map_end:

sprites_pal:
.incbin "sprites.pal"
sprites_pal_end:

numbers_pal:
.incbin "numbers.pal"
numbers_pal_end:

lagtest_pal:
.incbin "lagtest.pal"
lagtest_pal_end:

sonicback_pal:
.incbin "sonicback.pal"
sonicback_pal_end:

sonicfloor_pal:
.incbin "sonicfloor.pal"
sonicfloor_pal_end:

.ends

.section ".rodata3" superfree

font:
.incbin "font.pic"

motoko_tiles2:
.incbin "motoko.pic" skip $6000
motoko_tiles2_end:

IRE100_map:
.incbin "100IRE.map"
IRE100_map_end:

sonicback_map:
.incbin "sonicback.map"
sonicback_map_end:

sonicfloor_map:
.incbin "sonicfloor.map"
sonicfloor_map_end:	

nishka_map:
.incbin "n_m.s"
nishka_map_end:

LEDsprites_pal:
.incbin "LEDsprites.pal"
LEDsprites_pal_end:

striped_tiles:
.incbin "striped.pic"
striped_tiles_end:

striped_pal:
.incbin "striped.pal"
striped_pal_end:

lagspr_tiles:
.incbin "lag-spr.pic"
lagspr_tiles_end:

lagspr_pal:
.incbin "lag-spr.pal"
lagspr_pal_end:

.ends

.section ".rodata4" superfree

fontback:
.incbin "fontback.pic"
fontback_end:

SMPTECB75_tiles:
.incbin "SMPTECB75.pic"
SMPTECB75_tiles_end:

color_tiles:
.incbin "color.pic"
color_tiles_end:

circles_tiles:
.incbin "circles.pic"
circles_tiles_end:

pluge_tiles:
.incbin "pluge.pic"
pluge_tiles_end:

grayramp_tiles:
.incbin "grayramp.pic"
grayramp_tiles_end:

colorbleed_tiles:
.incbin "colorbleed.pic"
colorbleed_tiles_end:

colorbleedchk_tiles:
.incbin "colorbleedchk.pic"
colorbleedchk_tiles_end:

cb601701_tiles:
.incbin "601701cb.pic"
cb601701_tiles_end:

sharpness_tiles:
.incbin "sharpness.pic"
sharpness_tiles_end:

fullscreen_tiles:
.incbin "fullscreen.pic"
fullscreen_tiles_end:

fullscreen256_tiles:
.incbin "fullscreen256.pic"
fullscreen256_tiles_end:

hstripes_tiles:
.incbin "hstripes.pic"
hstripes_tiles_end:

vstripes_tiles:
.incbin "vstripes.pic"
vstripes_tiles_end:

check_tiles:
.incbin "check.pic"
check_tiles_end:

sprites_tiles:
.incbin "sprites.pic"
sprites_tiles_end:

IRE100_tiles:
.incbin "100IRE.pic"
IRE100_tiles_end:

lagtest_map:
.incbin "lagtest.map"
lagtest_map_end:

nishka_pal:
.incbin "n_p.s"
nishka_pal_end:

circlesgrid_tiles:
.incbin "circles_grid.pic"
circlesgrid_tiles_end:

.ends

.section ".rodata5" superfree

numbers_tiles:
.incbin "numbers.pic"
numbers_tiles_end:

lagtest_tiles:
.incbin "lagtest.pic"
lagtest_tiles_end:

sonicfloor_tiles:
.incbin "sonicfloor.pic"
sonicfloor_tiles_end:

color_grid_map:
.incbin "color_grid.map"
color_grid_map_end:

.ends

.section ".rodata6" superfree

sonicback_tiles:
.incbin "sonicback.pic"
sonicback_tiles_end:

LEDsprites_tiles:
.incbin "LEDsprites.pic"
LEDsprites_tiles_end:

.ends

.section ".rodata7" superfree

nishka_tiles:
.incbin "n_t.s"
nishka_tiles_end:

.ends

.section ".rodata8" superfree

color_grid_tiles:
.incbin "color_grid.pic"
color_grid_tiles_end:

.ends
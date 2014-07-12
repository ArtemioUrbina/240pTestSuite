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

.ends

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

.ends

.section ".rodata3" superfree

font:
.incbin "font.pic"

motoko_tiles1:
.incbin "motoko.pic" read $6000
motoko_tiles1_end:

.ends

.section ".rodata4" superfree

motoko_tiles2:
.incbin "motoko.pic" skip $6000 read $7040
motoko_tiles2_end:

.ends

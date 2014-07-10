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

gillian_map:
.incbin "gillian.map"
gillian_map_end:

gillian_pal:
.incbin "gillian.pal"
gillian_pal_end:

.ends

.section ".rodata3" superfree

font:
.incbin "font.pic"

.ends

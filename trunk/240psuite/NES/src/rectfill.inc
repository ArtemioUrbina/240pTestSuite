;
; Nametable rectangle fill engine
; Copyright 2015 Damian Yerrick
;
; Copying and distribution of this file, with or without
; modification, are permitted in any medium without royalty provided
; the copyright notice and this notice are preserved in all source
; code copies.  This file is offered as-is, without any warranty.
;
.ifndef RECTFILL_INC
RECTFILL_INC = 1

.global rf_curnametable, rf_curpattable, rf_tilenum
.global rf_draw_rects, rf_draw_attrs, rf_push_attrs, rf_draw_labels
.global rf_load_tiles, rf_color_lineImgBuf, rf_copy8tiles

; Macros for 

.macro rf_rect in_left, in_top, in_right, in_bottom, in_tile, in_flags
.local rectx, recty, rectw, recth
rectx = ((in_left) + 4) >> 3
recty = ((in_top) + 4) >> 3
rectw = (((in_right) + 4) >> 3) - rectx
recth = (((in_bottom) + 4) >> 3) - recty
  .assert 0 <= rectx && rectx < 32, error, "left side out of bounds"
  .assert 0 <= recty && recty < 30, error, "top out of bounds"
  .assert 0 < rectw && rectx + rectw <= 32, error, "left side out of bounds"
  .assert 0 < recth && recth + recty <= 30, error, "top out of bounds"
  .dbyt (rectw << 10) | (recty << 5) | rectx
  .byte in_tile
  .byte (recth << 3) | (in_flags)
.endmacro

RF_INCR   = $04
RF_ROWXOR = $02
RF_COLXOR = $01

.macro rf_attr in_left, in_top, in_right, in_bottom, in_attr
.local rectx, recty, rectw, recth
rectx = ((in_left) + 8) >> 4
recty = ((in_top) + 8) >> 4
rectw = (((in_right) + 8) >> 4) - rectx
recth = (((in_bottom) + 8) >> 4) - recty
  .assert 0 <= rectx && rectx < 16, error, "left side out of bounds"
  .assert 0 <= recty && recty < 15, error, "top out of bounds"
  .assert 0 < rectw && rectx + rectw <= 16, error, "left side out of bounds"
  .assert 0 < recth && recth + recty <= 15, error, "top out of bounds"
  .byte recth
  .byte (rectw << 3) | (in_attr)
  .byte ((recty << 4) & $E0) | (rectx << 1) | (recty & $01)
.endmacro

;;
; Sets the X, Y, and color of the following nul-terminated string.
.macro rf_label in_left, in_top, in_fgc, in_bgc
.local txtx, txty, fgc, bgc
txty = ((in_top) + 4) >> 3
  .assert 0 <= txty && txty < 30, error, "top out of bounds"
  .byte in_bgc * 5 ^ in_fgc, txty, in_left
.endmacro

.endif

;
; Include files for NES port of 240p test suite
; Copyright 2015 Damian Yerrick
;
; This program is free software; you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation; either version 2 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License along
; with this program; if not, write to the Free Software Foundation, Inc.,
; 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
;

.ifndef GLOBAL_INC
.define GLOBAL_INC

; init.s
.global reset_handler

; main.s
.global OAM
.global main, nmi_handler, irq_handler
.globalzp cur_keys, new_keys, das_keys, das_timer
.globalzp oam_used, nmis, tvSystem

; pads.s
.global read_pads, autorepeat

; ppuclear.s
.global ppu_clear_nt, ppu_clear_oam, ppu_screen_on

; helpscreen.s
.global helpscreen, load_sb53_ay, load_sb53, help_reload
.global txtlinebuf, vram_copydstlo, vram_copydsthi

; unpb53.s
.global unpb53_some, unpb53_xtiles, unpb53_ay_xtiles, PB53_outbuf
.globalzp ciSrc, ciDst, ciBufStart, ciBufEnd

; vwf_draw.s
.global vwfPutTile, vwfPuts, vwfPuts0
.global vwfGlyphWidth, vwfStrWidth, vwfStrWidth0
.global clearLineImg, lineImgBuf, invertTiles
.global copyLineImg_helpscreen

; paldetect.s
.global getTVSystem

; stills.s
.global do_linearity, do_sharpness, do_ire, do_smpte, do_601bars
.global do_pluge, do_gcbars, do_cpsgrid, do_gray_ramp, do_color_bleed
.global do_full_stripes, do_solid_color
.global test_state

; stopwatch.s
.global do_stopwatch

; backlight.s
.global do_backlight

; scrolling.s
.global do_grid_scroll

; soundtest.s
.global do_sound_test

; overscan.s
.global do_overscan

; megaton.s
.global do_manual_lag

; semixparent.s
.global do_drop_shadow_sprite

; bcd.s
.global bcd8bit
bcd_highdigits = $0000

; random.s
.globalzp CRCLO, CRCHI
.global rand_crc, crc16_update

.endif

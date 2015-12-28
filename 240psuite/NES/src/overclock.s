;
; CPU clock speed test for 240p test suite
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

.include "nes.inc"
.include "global.inc"
.include "rectfill.inc"
.importzp helpsect_cpu_clock_speed

.segment "RODATA"
nine_y = 2
s0_x = 16  ; to line up roughly under when 9 sprites bit turns on
s0_y = nine_y + 24

overclock_rects:
  rf_rect   0,  0,256,240,$00, 0
  rf_rect 128,104,192,152,$80, RF_INCR  ; text area
  .byte 0

  rf_attr   0,  0,256,240, 0
  .byte 0

  rf_label  64,104, 2, 0
  .byte "Cycles/line:",0
  rf_label  64,112, 2, 0
  .byte "Lines/frame:",0
  rf_label  64,120, 2, 0
  .byte "NMI scanline:",0
  rf_label  64,128, 2, 0
  .byte "TV system:",0
  rf_label  64,136, 2, 0
  .byte "Frame rate:",0
  rf_label  64,144, 2, 0
  .byte "CPU speed:",0
  .byte 0

tl_pb53:
  .byte $84
  .byte %00111111,$FF,$00,$80

tv_system_names_lo: .lobytes ntsc_name, pal_name
tv_system_names_hi: .hibytes ntsc_name, pal_name
ntsc_name: .byte "NTSC (60 Hz)",0
pal_name:  .byte "PAL (50 Hz)",0

NTSC_HSYNC_CENTI = 1574580  ; NTSC hsync in cHz
NTSC_HSYNC_4096  = 64494    ; 4096 * NTSC hsync in kHz
PAL_HSYNC_CENTI  = 1560218  ; PAL hsync in cHz
PAL_HSYNC_4096   = 63907    ; 4096 * PAL hsync in kHz

hsync_centi_lo:   .lobytes   NTSC_HSYNC_CENTI, PAL_HSYNC_CENTI
hsync_centi_hi:   .hibytes   NTSC_HSYNC_CENTI, PAL_HSYNC_CENTI
hsync_centi_bank: .bankbytes NTSC_HSYNC_CENTI, PAL_HSYNC_CENTI
hsync_4096_lo: .hibytes NTSC_HSYNC_4096, PAL_HSYNC_4096
hsync_4096_hi: .hibytes NTSC_HSYNC_4096, PAL_HSYNC_4096

frame_time      = test_state + 0
time_192_lines  = test_state + 2
time_s0_nmi     = test_state + 4

; Intermediate results
cyc_10_lines    = test_state + 6
lines_per_field = test_state + 8
nmi_line        = test_state + 10
oc_tvSystem     = test_state + 12
; Printing
prpos           = vram_copydstlo
string_buffer   = test_state + 13

.segment "CODE"

.proc do_overclock
  lda #VBLANK_NMI
  sta PPUCTRL
  sta help_reload
  sta vram_copydsthi
  asl a
  sta rf_curpattable
  sta PPUMASK
  jsr rf_load_yrgb_palette

  ; Load static tiles
  lda #$00
  sta PPUADDR
  sta PPUADDR
  ldy #<tl_pb53
  lda #>tl_pb53
  ldx #2
  stx rf_tilenum
  jsr unpb53_ay_xtiles

  ldx #$20
  stx rf_curnametable
  ldy #<overclock_rects
  lda #>overclock_rects
  jsr rf_draw_rects_attrs_ay
  inc ciSrc
  bne :+
    inc ciSrc+1
  :
  jsr rf_draw_labels
  jsr s0_setup
  
  lda #$00
  tay
  ldx #$08
  jsr ppu_clear_nt  ; blank the text window

loop:
  lda #0
  sta new_keys
  jsr run_tests_once
  jsr display_results

  lda new_keys+0
  and #KEY_START
  beq not_help
    ldx #helpsect_cpu_clock_speed
    lda #KEY_A|KEY_START|KEY_B|KEY_LEFT|KEY_RIGHT
    jsr helpscreen
    jmp do_overclock
  not_help:

  lda new_keys+0
  and #KEY_B
  bne done
  jmp loop
done:
  rts
.endproc

.proc oc_present
  lda nmis
:
  cmp nmis
  beq :-

  lda vram_copydsthi
  bmi :+
    jsr rf_copy8tiles
  :
  ldx #0
  lda #>OAM
  stx OAMADDR
  sta OAM_DMA
  ldy #0
  lda #VBLANK_NMI|BG_0000
  sec
  jsr ppu_screen_on

  ; OR together all new keypresses during lag frames
  lda new_keys+0
  pha
  jsr read_pads
  pla
  ora new_keys+0
  sta new_keys+0
  rts
.endproc

.proc display_results
  ; If a frame is longer than 1.5 * 192 lines, it's PAL
  lda time_192_lines+1
  lsr a
  sta $01
  lda time_192_lines
  ror a
  adc time_192_lines
  sta $00
  lda $01
  adc time_192_lines+1
  sta $01
  lda frame_time
  cmp $00
  lda frame_time+1
  sbc $01
  lda #0
  rol a
  sta tvSystem

  ; time_192_lines is 1/12 the cycles in 192 lines
  ; 5/8 of this is the number of cycles in 10 lines
  lda time_192_lines+1
  sta cyc_10_lines+1
  lda time_192_lines
  asl a
  rol cyc_10_lines+1
  asl a
  rol cyc_10_lines+1
  adc time_192_lines
  sta cyc_10_lines

  ; cyc_10_lines now holds cycles in 80 lines, so divide by 8
  lda cyc_10_lines+1
  adc time_192_lines+1
  lsr a
  ror cyc_10_lines
  lsr a
  ror cyc_10_lines
  lsr a
  ror cyc_10_lines
  sta cyc_10_lines+1
  bcc :+
  inc cyc_10_lines
  bne :+
  inc cyc_10_lines+1
:

  ; lines per field = (384 * frame_time / time_192_lines + 1) / 2
  ; 128 * frame_time
  ldy frame_time+1
  ldx frame_time
  jsr iters_to_scanlines
  stx lines_per_field
  sty lines_per_field+1

  ; lines s0 to nmi = (384 * time_s0_nmi / time_192_lines + 1) / 2
  ; 128 * frame_time
  ldy time_s0_nmi+1
  ldx time_s0_nmi
  jsr iters_to_scanlines
  clc
  txa
  adc #s0_y * 8
  sta nmi_line
  tya
  adc #0
  sta nmi_line+1

  ; Now we're ready to start printing things.  Results start at $0800
  lda #$08
  sta prpos+1
  lda #$00
  sta prpos+0
  
  lda cyc_10_lines+1
  ldy cyc_10_lines
  ldx #1
  jsr write_16bit_number

  lda lines_per_field+1
  ldy lines_per_field
  ldx #0
  jsr write_16bit_number

  lda nmi_line+1
  ldy nmi_line
  ldx #0
  jsr write_16bit_number

  ldx tvSystem
  lda tv_system_names_hi,x
  sta 1
  lda tv_system_names_lo,x
  sta 0
  jsr write_result_string

  ldx tvSystem
  lda lines_per_field+1
  sta fac2hi
  lsr a
  sta prodmlo
  lda lines_per_field
  sta fac2lo
  ror a
  sta prodlo
  lda hsync_centi_lo,x
  adc prodlo
  sta prodlo
  lda hsync_centi_hi,x
  adc prodmlo
  sta prodmlo
  lda #0
  sta prodhi
  adc hsync_centi_bank,x
  sta prodmhi
  jsr div32x16
  lda prodmlo
  ldy prodlo
  ldx #2
  jsr write_16bit_number

  lda time_192_lines
  sta fac1lo
  lda time_192_lines+1
  sta fac1hi
  ldx tvSystem
  lda hsync_4096_lo,x
  sta fac2lo
  lda hsync_4096_hi,x
  sta fac2hi
  jsr mul16x16
  asl prodmlo
  lda #0
  adc prodmhi
  tay
  lda prodhi
  adc #0
  ldx #3
  jsr write_16bit_number

  rts
.endproc


;;
; @param Y:X number of iterations (high)
; @param Y:X number of scanlines
.proc iters_to_scanlines
  tya
  lsr a
  sta prodmhi
  txa
  ror a
  sta prodmlo
  lda #0
  sta prodhi
  ror a
  sta prodlo
  ; + 256 * frame_time
  txa
  adc prodmlo
  sta prodmlo
  tya
  adc prodmhi
  sta prodmhi
  lda time_192_lines+1
  sta fac2hi
  lda time_192_lines
  sta fac2lo
  jsr div32x16
  lsr prodmlo
  lda prodlo
  ror a
  adc #0
  tax
  lda prodmlo
  adc #0
  tay
  rts
.endproc

;;
; Writes a 16-bit number to a line of result.
; @param AY value
; @param X number of decimal places
.proc write_16bit_number
  stx string_buffer
  sty bcdNum+0
  sta bcdNum+1
  lda #0
  sta bcdNum+2
.endproc
;;
; Writes a number to a line of result.
; @param bcdNum number
; @param string_buffer[0] number of decimal places
.proc write_result_number
  jsr bcdConvert24
  ldx string_buffer
  lda #>string_buffer
  ldy #<string_buffer
  jsr bcdWrite
  txa
  tay

  ; Right-pad with spaces (to erase previous number) and NUL-terminate
  lda #' '
:
  sta (0),y
  iny
  cpy #10
  bcc :-
  lda #0
  sta (0),y
.endproc
;;
; Writes the NUL-terminated string at (0) to a line of result.
.proc write_result_string
  jsr clearLineImg
  ldx #0
  jsr vwfPuts0
  lda #%0011
  jsr rf_color_lineImgBuf
  
  jsr oc_present
  lda prpos
  tax
  clc
  adc #128
  sta prpos
  bcc :+
  inc prpos+1
:
  rts
.endproc

; PPU tests ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.segment "CODE"

;;
; Sets up a screen with 9 sprites near the top and sprite 0
; near the bottom
.proc s0_setup
s0_ppuaddr = $2000 + s0_y * 32 + s0_x
s0_tile = $01
nine_tile = $00

  lda #0
  sta PPUMASK
  sta OAM+2
  lda #s0_x << 3
  sta OAM+3
  lda #>s0_ppuaddr
  sta PPUADDR
  lda #<s0_ppuaddr
  sta PPUADDR
  lda #8 * s0_y - 1
  sta OAM+0
  lda #s0_tile
  sta OAM+1
  sta PPUDATA
  ldx #4
clrloop:
  lda #8 * nine_y
  sta OAM,x
  lda #0
  sta OAM+2,x
  sta OAM+3,x
  lda #nine_tile
  sta OAM+1,x
  inx
  inx
  inx
  inx
  bne clrloop
  rts
.endproc

.proc run_tests_once
  jsr oc_present

  ; Measure frame period in 12 cycle units
  jsr s0_rise_to_rise
  stx frame_time+0
  sty frame_time+1
  ; frame_time is 

  ; Measure period of 192 scanlines in 12 cycle units
  ; (or scanline period in 1/16 cycle units)
  jsr sov_to_s0
  stx time_192_lines+0
  sty time_192_lines+1
  
  jsr now_to_nmi
  stx time_s0_nmi+0
  sty time_s0_nmi+1

  ; If frame_time > time_192_lines + time_192_lines / 2
  ; then there are more than 288 lines, that is, PAL
  rts
.endproc


.align 64
;;
; Counts how long it takes from a sprite 0 rise to another fall
; and rise.  Used to determine cycles per frame.
; @return number of cycles divided by 12; X: low byte; Y: high byte
.proc s0_rise_to_rise
  ldx #0
  ldy #0
loop1:
  bit PPUSTATUS
  bvs loop1
loop2:
  bit PPUSTATUS
  bvc loop2
loop3:
  inx
  bne :+
  iny
:
  bit PPUSTATUS
  bvs loop3
loop4:
  inx
  bne :+
  iny
:
  bit PPUSTATUS
  bvc loop4
  rts
.endproc

;;
; Counts how long it takes from 9 sprites rise to sprite 0 rise.
; Used to determine cycles per scanline.
; @return number of cycles divided by 12; X: low byte; Y: high byte
.proc sov_to_s0
  ldx #0
  ldy #0
  lda #1<<5
loop1:
  bit PPUSTATUS
  bne loop1
loop2:
  bit PPUSTATUS
  beq loop2
loop3:
  inx
  bne :+
  iny
:
  bit PPUSTATUS
  bvc loop3
  rts
.endproc

.proc now_to_nmi
  ldx #5  ; account for s0 storage overhead
  ldy #0
  lda nmis
loop:
  inx
  bne :+
    iny
  :
  cmp a:nmis
  beq loop
  .assert >*=>loop, error, "now_to_nmi crosses page"
  rts
.endproc

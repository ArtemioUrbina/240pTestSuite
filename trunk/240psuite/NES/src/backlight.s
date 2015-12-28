;
; Backlight test for 240p test suite
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
.importzp helpsect_backlight_zone_test

.segment "RODATA"
backlight_sprites:  .incbin "obj/nes/backlight_sprites.chr.pb53"

.segment "CODE"
.proc do_backlight
sprite_x = test_state+0
sprite_y = test_state+1
sprite_size = test_state+2
sprite_hide = test_state+3
  lda #$FC
  sta sprite_size
  lda #128
  sta sprite_x
  sta sprite_y
  asl a
  sta sprite_hide

restart:
  lda #0
  sta OAM+2
  sta PPUMASK
  lda #$1F
  sta PPUADDR
  lda #$C0
  sta PPUADDR
  lda #>(backlight_sprites+2)
  ldy #<(backlight_sprites+2)
  ldx #4
  jsr unpb53_ay_xtiles
  ldx #4
  jsr ppu_clear_oam
loop:
  lda sprite_x
  sta OAM+3
  lda sprite_y
  ora sprite_hide
  sta OAM+0
  lda sprite_size
  sta OAM+1
  lda nmis
:
  cmp nmis
  beq :-
  lda #OBJ_ON
  sta PPUMASK
  lda #0
  sta OAMADDR
  lda #>OAM
  sta OAM_DMA
  lda #VBLANK_NMI|OBJ_1000
  sta PPUCTRL

  jsr read_pads
  lda new_keys+0
  and #KEY_START
  beq not_help
    lda #KEY_B|KEY_A|KEY_START|KEY_LEFT|KEY_RIGHT
    ldx #helpsect_backlight_zone_test
    jsr helpscreen
    jmp restart
  not_help:

  lda new_keys+0
  and #KEY_A
  beq not_toggle_hide
    lda #$F0
    eor sprite_hide
    sta sprite_hide
  not_toggle_hide:

  lda new_keys+0
  and #KEY_SELECT
  beq not_toggle_size
    inc sprite_size
    bne not_toggle_size
    lda #$FC
    sta sprite_size
  not_toggle_size:

  lda cur_keys+0
  and #KEY_RIGHT
  beq not_right
    inc sprite_x
    bne not_right
    dec sprite_x
  not_right:

  lda cur_keys+0
  and #KEY_DOWN
  beq not_down
    lda sprite_y
    cmp #238
    bcs not_down
    inc sprite_y
  not_down:

  lda cur_keys+0
  and #KEY_LEFT
  beq not_left
    lda sprite_x
    beq not_left
    dec sprite_x
  not_left:

  lda cur_keys+0
  and #KEY_UP
  beq not_up
    lda sprite_y
    beq not_up
    dec sprite_y
  not_up:

  lda new_keys+0
  and #KEY_B
  bne done
  jmp loop
done:
  rts
.endproc

;
; Help screen for 240p test suite
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
.importzp helpsect_linearity, helpsect_sharpness, helpsect_ire
.importzp helpsect_smpte_color_bars, helpsect_601_color_bars
.importzp helpsect_pluge, helpsect_grid, helpsect_gradient_color_bars
.importzp helpsect_gray_ramp, helpsect_color_bleed
.importzp helpsect_full_screen_stripes, helpsect_solid_color_screen

; sb53-based stills ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.segment "BSS"
test_state: .res 24

.segment "CODE"

.proc do_linearity
  lda #VBLANK_NMI
  sta test_state
  sta help_reload
  sta PPUCTRL
  asl a
  sta PPUMASK

:
  lda #linearity_bank
  sta :-+1

  ; Load picture without grid
  ldx tvSystem
  beq :+
    ldx #4
  :
  ldy linearity_pics+0,x
  lda linearity_pics+1,x
  ldx #$00
  jsr load_sb53_ay

  ; Load picture with grid
  ldx tvSystem
  beq :+
    ldx #4
  :
  ldy linearity_pics+2,x
  lda linearity_pics+3,x
  ldx #$14
  jsr load_sb53_ay

loop:
  lda nmis
:
  cmp nmis
  beq :-
  ldx #0
  ldy #0
  lda test_state
  clc
  jsr ppu_screen_on

  jsr read_pads
  lda new_keys
  and #KEY_A
  beq not_toggle_grid
    lda #1|BG_1000
    eor test_state
    sta test_state
  not_toggle_grid:
  lda new_keys
  and #KEY_START
  beq not_help
    lda #KEY_B|KEY_A|KEY_START|KEY_LEFT|KEY_RIGHT
    ldx #helpsect_linearity
    jsr helpscreen
    jmp do_linearity
  not_help:
  lda new_keys
  and #KEY_B
  beq loop
  rts
.endproc

.proc do_sharpness
  lda #VBLANK_NMI
  sta help_reload
  sta PPUCTRL
  asl a
  sta PPUMASK

:
  lda #linearity_bank
  sta :-+1
  ldy #<sharpness_bg
  lda #>sharpness_bg
  ldx #$00
  jsr load_sb53_ay

loop:
  lda nmis
:
  cmp nmis
  beq :-
  ldx #0
  ldy #0
  lda #VBLANK_NMI
  clc
  jsr ppu_screen_on

  jsr read_pads
  lda new_keys+0
  and #KEY_START
  beq not_help
    lda #KEY_B|KEY_A|KEY_START|KEY_LEFT|KEY_RIGHT
    ldx #helpsect_sharpness
    jsr helpscreen
    jmp do_sharpness
  not_help:
  lda new_keys
  and #KEY_B
  beq loop
  rts
.endproc

.segment "BANK01"
linearity_bank = 1
linearity_pics:
  .addr linearity_ntsc, linearity_grid_ntsc
  .addr linearity_pal, linearity_grid_pal
linearity_ntsc:      .incbin "obj/nes/linearity_ntscgray.sb53"
linearity_grid_ntsc: .incbin "obj/nes/linearity_grid_ntscgray.sb53"
linearity_pal:       .incbin "obj/nes/linearity_palgray.sb53"
linearity_grid_pal:  .incbin "obj/nes/linearity_grid_palgray.sb53"
sharpness_bg:        .incbin "obj/nes/sharpnessgray.sb53"

; IRE ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.segment "RODATA"

ire_rects:
  rf_rect   0,  0,256,240,$00, 0  ; Clear screen to black
  rf_rect  64, 64,192,176,$0C, 0  ; Color 3: inside
  rf_rect 160,192,224,200,$F8, RF_INCR  ; text area
  .byte $00
ire_attrs:
  rf_attr  0,  0,256, 240, 0
  .byte $00
.if 0
ire_texts:
  rf_label 112,112, 0, 3
  .byte "IRE LAND",0
  .byte $00
.endif

ire_msgs:
  .addr ire_msg0, ire_msg1, ire_msg2, ire_msg3, ire_msg4
  .addr ire_msg5, ire_msg6
ire_msg0:  .byte "0 on 0",0
ire_msg1:  .byte "40 on 0",0
ire_msg2:  .byte "70 on 0",0
ire_msg3:  .byte "100 on 0",0
ire_msg4:  .byte "100 on 40",0
ire_msg5:  .byte "100 on 70",0
ire_msg6:  .byte "100 on 100",0

irelevel_bg: .byte $0F,$0F,$0F
irelevel_fg: .byte $0F,$00,$10,$20,$20,$20,$20

.segment "CODE"
.proc do_ire
ire_level = test_state+0
need_reload = test_state+1
disappear_time = test_state+2

  lda #3
  sta ire_level
restart:
  jsr rf_load_tiles
  lda #$20
  sta need_reload
  sta rf_curnametable
  sta rf_tilenum
  lda #$00
  sta rf_curpattable
  ldy #<ire_rects
  lda #>ire_rects
  jsr rf_draw_rects_attrs_ay

loop:
  lda need_reload
  beq not_reloading
    sty $FF
    lda #0
    sta need_reload
    lda #120
    sta disappear_time
    jsr clearLineImg
    lda ire_level
    asl a
    tax
    lda ire_msgs+1,x
    ldy ire_msgs,x
    ldx #8
    jsr vwfPuts
    lda #%0111
    jsr rf_color_lineImgBuf
    lda #$0F
    sta vram_copydsthi
    lda #$80
    sta vram_copydstlo
  not_reloading:

  lda nmis
:
  cmp nmis
  beq :-

  ; Update palette
  lda #$3F
  sta PPUADDR
  lda #$00
  sta PPUADDR
  ldx ire_level
  ldy irelevel_bg,x
  sty PPUDATA
  lda disappear_time
  beq :+
    ldy #$02
  :
  sty PPUDATA
  beq :+
    ldy #$38
    dec disappear_time
  :
  sty PPUDATA
  ldy irelevel_fg,x
  sty PPUDATA

  ; Copy tiles if needed
  lda vram_copydsthi
  bmi :+
    jsr rf_copy8tiles
    lda #$80
    sta vram_copydsthi
  :
    
  ; And turn the display on
  ldx #0
  ldy #0
  lda #VBLANK_NMI|BG_0000
  clc
  jsr ppu_screen_on
  jsr read_pads

  lda new_keys+0
  and #KEY_START
  beq not_help
    lda #KEY_B|KEY_A|KEY_START|KEY_LEFT|KEY_RIGHT
    ldx #helpsect_ire
    jsr helpscreen
    jmp restart
  not_help:

  lda new_keys+0
  and #KEY_RIGHT
  beq not_increase
  lda ire_level
  cmp #6
  bcs not_increase
    inc ire_level
    inc need_reload
  not_increase:

  lda new_keys+0
  and #KEY_LEFT
  beq not_decrease
  lda ire_level
  beq not_decrease
    dec ire_level
    inc need_reload
  not_decrease:

  lda new_keys+0
  and #KEY_B
  bne done
  jmp loop

done:
  rts
.endproc

; SMPTE color bars ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.segment "RODATA"
smpte_rects:
  rf_rect   0,  0, 32,160,$04, 0  ; bar 1
  rf_rect  32,  0, 40,160,$10, 0  ; bar 1-2
  rf_rect  40,  0, 72,160,$08, 0  ; bar 2 (allegedly not so yellow)
  rf_rect  72,  0, 80,160,$11, 0  ; bar 2-3
  rf_rect  80,  0,112,160,$0c, 0  ; bar 3
  rf_rect 112,  0,144,160,$04, 0  ; bar 4
  rf_rect 144,  0,176,160,$04, 0  ; bar 5
  rf_rect 176,  0,184,160,$10, 0  ; bar 5-6
  rf_rect 184,  0,216,160,$08, 0  ; bar 6
  rf_rect 216,  0,224,160,$11, 0  ; bar 6-7
  rf_rect 224,  0,256,160,$0c, 0  ; bar 7
  rf_rect   0,160,256,240,$00, 0  ; black
  rf_rect   0,160, 32,176,$0c, 0  ; bar 7
  rf_rect  32,160, 40,176,$18, 0  ; bar 7-
  rf_rect  72,160, 80,176,$19, 0  ; bar -5
  rf_rect  80,160,112,176,$04, 0  ; bar 5
  rf_rect 144,160,176,176,$0c, 0  ; bar 3
  rf_rect 176,160,184,176,$18, 0  ; bar 3-
  rf_rect 216,160,224,176,$19, 0  ; bar -1
  rf_rect 224,160,256,176,$04, 0  ; bar 1
  rf_rect   0,176, 48,240,$04, 0  ; i
  rf_rect  48,176, 96,240,$08, 0  ; white
  rf_rect  96,176,144,240,$0C, 0  ; q
  rf_rect 184,176,200,240,$08, 0  ; 0d
  .byte $00
  rf_attr   0,  0,256,240, 0
  rf_attr 112,  0,144,160, 3  ; green bar
  rf_attr 144,  0,256,160, 1  ; bars 5-7
  rf_attr   0,160,112,176, 1  ; bars 7-5
  rf_attr   0,176,144,240, 2  ; i, y, q
  rf_attr 160,176,256,240, 3  ; 0d
  .byte $00

_601_rects:
  rf_rect   0,  0,256,240,$04, 0  ; bar 1 and bg

  rf_rect  32, 48, 40, 96,$10, 0  ; bar 1-2
  rf_rect  40, 48, 72, 96,$08, 0  ; bar 2 (allegedly not so yellow)
  rf_rect  72, 48, 80, 96,$11, 0  ; bar 2-3
  rf_rect  80, 48,112, 96,$0c, 0  ; bar 3
  rf_rect 112, 48,144, 96,$04, 0  ; bar 4
  rf_rect 144, 48,176, 96,$04, 0  ; bar 5
  rf_rect 176, 48,184, 96,$10, 0  ; bar 5-6
  rf_rect 184, 48,216, 96,$08, 0  ; bar 6
  rf_rect 216, 48,224, 96,$11, 0  ; bar 6-7
  rf_rect 224, 48,256, 96,$0c, 0  ; bar 7
  rf_rect   0,144, 32,192,$0c, 0  ; bar 7
  rf_rect  32,144, 40,192,$12, 0  ; bar 7-6
  rf_rect  40,144, 72,192,$08, 0  ; bar 6
  rf_rect  72,144, 80,192,$13, 0  ; bar 6-5
  rf_rect  80,144,112,192,$04, 0  ; bar 5
  rf_rect 112,144,144,192,$04, 0  ; bar 4
  rf_rect 144,144,176,192,$0c, 0  ; bar 3
  rf_rect 176,144,184,192,$12, 0  ; bar 3-2
  rf_rect 184,144,216,192,$08, 0  ; bar 2
  rf_rect 216,144,224,192,$13, 0  ; bar 2-1
  .byte $00
  rf_attr   0,  0,256,240, 0
  rf_attr 112, 48,144, 96, 3  ; green bar
  rf_attr 144, 48,256, 96, 1  ; bars 5-7
  rf_attr   0,144,112,192, 1  ; bars 7-5
  rf_attr 112,144,144,192, 3  ; green bar
  .byte $00

smpte_palettes:
  .byte $0f,$10,$28,$2c, $0f,$14,$16,$02, $0f,$01,$20,$04, $0f,$1a,$0d,$00
  .byte $0f,$20,$38,$2c, $0f,$14,$16,$12, $0f,$01,$20,$04, $0f,$2a,$0d,$00

smpte_types:
  .addr smpte_rects
  .byte helpsect_smpte_color_bars
  .addr _601_rects
  .byte helpsect_601_color_bars

.segment "CODE"

.proc do_smpte
  ldx #0
  bpl do_bars
.endproc

.proc do_601bars
  ldx #3
  ; fall through
.endproc
.proc do_bars
smpte_level = test_state+0
smpte_type  = test_state+1

  lda #0
  sta smpte_level
  stx smpte_type
restart:
  jsr rf_load_tiles
  lda #$20
  sta rf_curnametable
  lda #$00
  sta rf_curpattable
  ldx smpte_type
  ldy smpte_types+0,x
  lda smpte_types+1,x
  jsr rf_draw_rects_attrs_ay

loop:
  lda nmis
:
  cmp nmis
  beq :-

  ; Update palette
  lda #$3F
  sta PPUADDR
  lda #$00
  sta PPUADDR
  lda smpte_level
  and #$10
  tax
  ldy #16
  :
    lda smpte_palettes,x
    sta PPUDATA
    inx
    dey
    bne :-

  ; And turn the display on
  ldx #0
  ldy #0
  lda #VBLANK_NMI|BG_0000
  clc
  jsr ppu_screen_on
  
  ; Update sound
  lda smpte_level
  and #$01
  beq :+
    lda #$88
  :
  sta $4008
  lda #55  ; FIXME: change this for PAL NES and Dendy
  sta $400A
  lda #0
  sta $400B

  jsr read_pads
  lda new_keys+0
  and #KEY_START
  beq not_help
    lda #KEY_B|KEY_A|KEY_START|KEY_LEFT|KEY_RIGHT
    ldy smpte_type
    ldx smpte_types+2,y
    jsr helpscreen
    jmp restart
  not_help:

  lda new_keys+0
  and #KEY_A
  beq not_increase
    lda #16
    eor smpte_level
    sta smpte_level
  not_increase:

  lda new_keys+0
  and #KEY_SELECT
  beq not_beep
    lda #1
    eor smpte_level
    sta smpte_level
  not_beep:

  lda new_keys+0
  and #KEY_B
  bne done
  jmp loop

done:
  rts
.endproc

; PLUGE ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.segment "RODATA"
pluge_rects:
  rf_rect   0,  0,256,240,$00, 0
  rf_rect  16, 48, 32,192,$0A, 0  ; lowest color above black
  rf_rect 224, 48,240,192,$0A, 0
  rf_rect  48, 48, 64,192,$04, 0  ; below black
  rf_rect 192, 48,208,192,$04, 0
  rf_rect  80, 48,176, 96,$0C, 0  ; gray boxes in center
  rf_rect  80, 96,176,144,$08, 0
  rf_rect  80,144,176,192,$04, 0
  .byte $00
  rf_attr   0,  0,256,240, 0
  rf_attr  80, 48,176,192, 1
  .byte $00
pluge_palette:
  .byte $0F,$0D,$04,$0A, $0F,$00,$10,$20

.segment "CODE"
.proc do_pluge
  jsr rf_load_tiles
  lda #$20
  sta rf_curnametable
  lda #$00
  sta rf_curpattable
  ldy #<pluge_rects
  lda #>pluge_rects
  jsr rf_draw_rects_attrs_ay

loop:
  lda nmis
:
  cmp nmis
  beq :-

  ; Update palette
  lda #$3F
  sta PPUADDR
  ldx #$00
  stx PPUADDR
  :
    lda pluge_palette,x
    sta PPUDATA
    inx
    cpx #8
    bcc :-

  ; And turn the display on
  ldx #0
  ldy #0
  lda #VBLANK_NMI|BG_0000
  clc
  jsr ppu_screen_on

  jsr read_pads
  lda new_keys+0
  and #KEY_START
  beq not_help
    lda #KEY_B|KEY_A|KEY_START|KEY_LEFT|KEY_RIGHT
    ldx #helpsect_pluge
    jsr helpscreen
    jmp do_pluge
  not_help:

  lda new_keys+0
  and #KEY_B
  beq loop
  rts
.endproc

; GRADIENT COLOR BARS ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.segment "RODATA"

gcbars_grid:
  rf_rect   0,  0,256,240,$16, RF_ROWXOR|RF_COLXOR
gcbars_nogrid:
  rf_rect  80, 32,112, 64,$04, 0  ; red 0
  rf_rect 112, 32,144, 64,$08, 0  ; red 1
  rf_rect 144, 32,176, 64,$0C, 0  ; red 2
  rf_rect  80, 80,112,112,$04, 0  ; green 0
  rf_rect 112, 80,144,112,$08, 0  ; green 1
  rf_rect 144, 80,176,112,$0C, 0  ; green 2
  rf_rect  80,128,112,160,$04, 0  ; blue 0
  rf_rect 112,128,144,160,$08, 0  ; blue 1
  rf_rect 144,128,176,160,$0C, 0  ; blue 2
  rf_rect  80,176,112,208,$04, 0  ; white 0
  rf_rect 112,176,144,208,$08, 0  ; white 1
  rf_rect 144,176,208,208,$0C, 0  ; white 2-3
  .byte $00
  rf_attr   0,  0,256,240, 0
  rf_attr  80, 32,176, 64, 1  ; red
  rf_attr  80, 80,176,112, 2  ; green
  rf_attr  80,128,176,160, 3  ; blue
  .byte $00
  rf_label  80, 24, 3, 0
  .byte "0",0
  rf_label 112, 24, 3, 0
  .byte "1",0
  rf_label 144, 24, 3, 0
  .byte "2",0
  rf_label 176, 24, 3, 0
  .byte "3",0
  rf_label  48, 40, 3, 0
  .byte "Red",0
  rf_label  48, 88, 3, 0
  .byte "Green",0
  rf_label  48,136, 3, 0
  .byte "Blue",0
  rf_label  48,184, 3, 0
  .byte "White",0
  .byte $00

.segment "CODE"

.proc do_gcbars
  lda #VBLANK_NMI|BG_0000|OBJ_8X16
  sta test_state+0
restart:
  jsr rf_load_tiles
  jsr rf_load_yrgb_palette

  ; On $2400, draw a CPS-2 grid and no labels
  ldx #$24
  stx rf_curnametable
  ldy #<gcbars_grid
  lda #>gcbars_grid
  jsr rf_draw_rects_attrs_ay

  ldx #$20
  stx rf_curnametable
  stx rf_tilenum
  lda #$00
  sta rf_curpattable
  tay
  jsr ppu_clear_nt
  ldy #<gcbars_nogrid
  lda #>gcbars_nogrid
  jsr rf_draw_rects_attrs_ay
  inc ciSrc
  bne :+
    inc ciSrc+1
  :
  jsr rf_draw_labels

  ; Set up sprite pattable
  lda #$10
  sta PPUADDR
  ldy #$00
  sty PPUADDR
  dey
  ldx #32
  sprchrloop:
    sty PPUDATA
    dex
    bne sprchrloop
  
  ; The "pale" ($3x) colors are drawn as 24 sprites.

sprite_y = $00
sprite_attr = $02
sprite_x = $03
  lda #31
  sta sprite_y
  ldx #0
  sprboxloop:
    lda #176
    sprcolloop:
      sta sprite_x
      lda sprite_y
      sta OAM+0,x
      clc
      adc #16
      sta OAM+4,x
      lda #$01
      sta OAM+1,x
      sta OAM+5,x
      lda sprite_attr
      sta OAM+2,x
      sta OAM+6,x
      lda sprite_x
      sta OAM+3,x
      sta OAM+7,x
      txa
      clc
      adc #8
      tax
      lda sprite_x
      clc
      adc #8
      cmp #208
      bcc sprcolloop
    inc sprite_attr
    lda sprite_y
    adc #48-1  ; the carry adds an additional 1
    sta sprite_y
    cmp #160
    bcc sprboxloop

  ; And hide the rest of sprites      
  jsr ppu_clear_oam

loop:
  lda nmis
:
  cmp nmis
  beq :-
  
  ldx #0
  stx OAMADDR
  lda #>OAM
  sta OAM_DMA
  ldy #0
  lda test_state+0
  sec
  jsr ppu_screen_on
  jsr read_pads

  lda new_keys+0
  and #KEY_START
  beq not_help
    lda #KEY_B|KEY_A|KEY_START|KEY_LEFT|KEY_RIGHT
    ldx #helpsect_gradient_color_bars
    jsr helpscreen
    jmp restart
  not_help:

  lda new_keys+0
  and #KEY_A
  beq not_toggle_screen
    lda #$01
    eor test_state+0
    sta test_state+0
  not_toggle_screen:

  lda new_keys+0
  and #KEY_B
  beq loop
  rts
.endproc

; CPS-2 STYLE GRID ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.segment "RODATA"
cpsgrid_224p_rects:
  rf_rect   0,  0,256,224,$16, RF_ROWXOR|RF_COLXOR
  rf_rect   0,224,256,240,$00, 0
  .byte $00
  rf_attr   0,  0,256,240, 0
  rf_attr  16, 16,240,208, 1
  .byte $00
cpsgrid_240p_rects:
  rf_rect   0,  0,256,240,$16, RF_ROWXOR|RF_COLXOR
  .byte $00
  rf_attr   0,  0,256,240, 0
  rf_attr  16, 16,240,224, 1
  .byte $00

.segment "CODE"
.proc do_cpsgrid
  lda #VBLANK_NMI|BG_0000|OBJ_8X16
  sta test_state+0
restart:
  jsr rf_load_tiles
  jsr rf_load_yrgb_palette

  ; On $2400, draw a CPS-2 grid and no labels
  ldx #$20
  stx rf_curnametable
  ldy #<cpsgrid_224p_rects
  lda #>cpsgrid_224p_rects
  jsr rf_draw_rects_attrs_ay
  ldx #$24
  stx rf_curnametable
  ldy #<cpsgrid_240p_rects
  lda #>cpsgrid_240p_rects
  jsr rf_draw_rects_attrs_ay
  
loop:
  lda nmis
:
  cmp nmis
  beq :-

  ldx #0  
  ldy #0
  lda test_state+0
  lsr a
  bcs :+
    ldy #240-8
  :
  rol a  ; restores test_state bit 0 and clears carry
  jsr ppu_screen_on
  jsr read_pads

  lda new_keys+0
  and #KEY_START
  beq not_help
    lda #KEY_B|KEY_A|KEY_START|KEY_LEFT|KEY_RIGHT
    ldx #helpsect_grid
    jsr helpscreen
    jmp restart
  not_help:

  lda new_keys+0
  and #KEY_A
  beq not_toggle_screen
    lda #$01
    eor test_state+0
    sta test_state+0
  not_toggle_screen:

  lda new_keys+0
  and #KEY_B
  beq loop
  rts
.endproc

; GRAY RAMP ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.segment "RODATA"
gray_ramp_rects:
  rf_rect  32, 24, 48,120,$01, 0
  rf_rect  48, 24, 64,120,$02, 0
  rf_rect  64, 24, 80,120,$03, 0
  rf_rect  80, 24, 96,120,$04, 0
  rf_rect  96, 24,112,120,$05, 0
  rf_rect 112, 24,128,120,$06, 0
  rf_rect 128, 24,144,120,$07, 0
  rf_rect 144, 24,160,120,$08, 0
  rf_rect 160, 24,176,120,$09, 0
  rf_rect 176, 24,192,120,$0a, 0
  rf_rect 192, 24,208,120,$0b, 0
  rf_rect 208, 24,224,120,$0c, 0
  rf_rect  32,120, 48,216,$0c, 0
  rf_rect  48,120, 64,216,$0b, 0
  rf_rect  64,120, 80,216,$0a, 0
  rf_rect  80,120, 96,216,$09, 0
  rf_rect  96,120,112,216,$08, 0
  rf_rect 112,120,128,216,$07, 0
  rf_rect 128,120,144,216,$06, 0
  rf_rect 144,120,160,216,$05, 0
  rf_rect 160,120,176,216,$04, 0
  rf_rect 176,120,192,216,$03, 0
  rf_rect 192,120,208,216,$02, 0
  rf_rect 208,120,224,216,$01, 0
  .byte $00

.segment "CODE"

.proc do_gray_ramp
  jsr rf_load_tiles
  jsr rf_load_yrgb_palette
  ldx #$20
  stx rf_curnametable
  lda #$00
  tay
  jsr ppu_clear_nt
  lda #<gray_ramp_rects
  sta ciSrc
  lda #>gray_ramp_rects
  sta ciSrc+1
  jsr rf_draw_rects

loop:
  lda nmis
:
  cmp nmis
  beq :-
  ldx #0
  ldy #0
  lda #VBLANK_NMI
  clc
  jsr ppu_screen_on

  jsr read_pads
  lda new_keys+0
  and #KEY_START
  beq not_help
    lda #KEY_B|KEY_A|KEY_START|KEY_LEFT|KEY_RIGHT
    ldx #helpsect_gray_ramp
    jsr helpscreen
    jmp do_gray_ramp
  not_help:
  lda new_keys
  and #KEY_B
  beq loop
  rts
.endproc

; COLOR BLEED ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.segment "RODATA"
bleedtile_top:    .byte $00, $55, $55
bleedtile_bottom: .byte $FF, $55, $AA

bleed_rects:
  rf_rect   0,  0,256,240,$00, 0
  rf_rect  16, 32,240, 64,$01, 0
  rf_rect  16, 80,240,112,$01, 0
  rf_rect  16,128,240,160,$01, 0
  rf_rect  16,176,240,208,$01, 0
  .byte $00
  rf_attr   0,  0,256,240, 0
  rf_attr  16, 32,240, 64, 1
  rf_attr  16, 80,240,112, 2
  rf_attr  16,128,240,160, 3
  .byte $00
fullstripes_rects:
  rf_rect   0,  0,256,240,$01, 0
  .byte $00
  rf_attr   0,  0,256,240, 0
  .byte $00
bleed_palette:
  .byte $0F,$0F,$0F,$20, $0F,$0F,$0F,$16, $0F,$0F,$0F,$1A, $0F,$0F,$0F,$12
  .byte $0F,$0F,$0F,$20
frame_label:
  .byte "Frame",0

bleed_types:
  .addr bleed_rects
  .byte helpsect_color_bleed
  .addr fullstripes_rects
  .byte helpsect_full_screen_stripes

.segment "CODE"
;;
; Sets tile 0 to solid color 0 and tile 1 to the color bleed tile
; in colors 2 and 3.
; @param X Pattern select: 0 horizontal; 1 vertical; 2 checkerboard
; @param A invert: $FF or $00
; @param Y Frame counter
.proc prepare_color_bleed_tiles

  ; Draw the frame counter
  pha
  txa
  pha
  jsr clearLineImg
  tya
  jsr bcd8bit
  ora #'0'
  ldx #56
  jsr vwfPutTile
  lda bcd_highdigits
  ora #'0'
  ldx #51
  jsr vwfPutTile
  lda #>frame_label
  ldy #<frame_label
  ldx #20
  jsr vwfPuts

  pla
  tax
  pla
  sta $00

  ; Draw the tile in question into x=8-15
  ldy #8
  l2:
    lda bleedtile_top,x
    eor $00
    sta lineImgBuf,y
    iny
    lda bleedtile_bottom,x
    eor $00
    sta lineImgBuf,y
    iny
    cpy #16
    bcc l2
  lda #%00000110
  jsr rf_color_lineImgBuf

  ; Finally, blank the first tile to color 0
  lda #$00
  sta vram_copydstlo
  sta vram_copydsthi
  ldy #15
  l3:
    sta lineImgBuf,y
    dey
    bpl l3
  
  rts 
.endproc

.proc do_full_stripes
  ldx #3
  bpl do_generic_color_bleed
.endproc

.proc do_color_bleed
  ldx #0
  ; fall through
.endproc
.proc do_generic_color_bleed
tile_shape  = test_state+0
xor_value   = test_state+1
frame_count = test_state+2
bg_type     = test_state+3
  stx bg_type
  ldx #0
  stx tile_shape
  stx xor_value
  stx frame_count

restart:
  lda nmis
:
  cmp nmis
  beq :-
  lda #$3F
  sta PPUADDR
  ldy #$00
  sty PPUADDR
  palloop:
    lda bleed_palette,y
    sta PPUDATA
    iny
    cpy #20
    bcc palloop

  ldx #$20
  stx rf_curnametable
  lda #$80
  sta PPUCTRL
  sta help_reload
  asl a
  sta PPUMASK
  lda bg_type
  and #$7F
  tax
  ldy bleed_types+0,x
  lda bleed_types+1,x
  jsr rf_draw_rects_attrs_ay
  
  ; Set up the frame counter sprites
  ldx #0
  ldy #2
  objloop:
    lda #207
    sta OAM+0,x
    tya
    sta OAM+1,x
    asl a
    asl a
    asl a
    sta OAM+3,x
    lda #0
    sta OAM+2,x
    txa
    adc #4
    tax
    iny
    cpy #8
    bcc objloop
  jsr ppu_clear_oam 

loop:
  ldx tile_shape
  lda xor_value
  ldy frame_count
  jsr prepare_color_bleed_tiles
  lda nmis
:
  cmp nmis
  beq :-
  jsr rf_copy8tiles
  ldx #0
  stx OAMADDR
  lda #>OAM
  sta OAM_DMA
  ldy #0
  lda bg_type
  asl a  ; bg_type D7 controls sprite on/off
  lda #VBLANK_NMI|BG_0000|OBJ_0000
  jsr ppu_screen_on

  jsr read_pads
  lda das_keys
  and #KEY_UP|KEY_DOWN|KEY_LEFT|KEY_RIGHT
  sta das_keys
  lda das_timer
  cmp #4
  bcs :+
    lda #1
    sta das_timer
  :
  ldx #0
  jsr autorepeat
  inc frame_count
  lda frame_count
  cmp #60
  bcc :+
    lda #0
    sta frame_count
  :

  lda new_keys+0
  and #KEY_START
  beq not_help
    lda bg_type
    and #$7F
    tay
    ldx bleed_types+2,y
    lda #KEY_B|KEY_A|KEY_START|KEY_LEFT|KEY_RIGHT
    jsr helpscreen
    jmp restart
  not_help:

  lda new_keys+0
  and #KEY_UP|KEY_DOWN|KEY_LEFT|KEY_RIGHT
  beq not_flip
    lda #$FF
    eor xor_value
    sta xor_value
  not_flip:

  lda new_keys+0
  and #KEY_SELECT
  beq not_toggle_counter
    lda #$80
    eor bg_type
    sta bg_type
  not_toggle_counter:

  lda new_keys+0
  and #KEY_A
  beq not_switch
    inc tile_shape
    lda tile_shape
    cmp #3
    bcc not_switch
      lda #0
      sta tile_shape
  not_switch:

  lda new_keys
  and #KEY_B
  bne done
  jmp loop
done:
  rts
.endproc

; SOLID COLOR SCREEN ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.segment "RODATA"

; cur_color=0-2: use one of these
; cur_color=3: use white_color
; cur_color=4: use black_color
solid_color_rgb: .byte $16,$1A,$12

solid_color_rects:
  rf_rect   0,  0,256,240,$00, 0
  rf_rect 160, 32,224, 40,$20, RF_INCR
  .byte $00
  rf_attr   0,  0,256,240, 0
  .byte $00
color_msg:  .byte "Color:",0

.segment "CODE"

.proc do_solid_color
cur_color = test_state+0
color_box_open = test_state+1
white_color = test_state+2
black_color = test_state+3
cur_bg_color = lineImgBuf+128
text_dark_color = lineImgBuf+129
text_light_color = lineImgBuf+130

  lda #3
  sta cur_color
  lda #0
  sta color_box_open
  lda #$20
  sta white_color
  lda #$0F
  sta black_color
restart:
  jsr rf_load_tiles
  ldx #$20
  stx rf_curnametable
  ldy #<solid_color_rects
  lda #>solid_color_rects
  jsr rf_draw_rects_attrs_ay

loop:
  ; Prepare color display
  jsr clearLineImg
  lda white_color
  lsr a
  lsr a
  lsr a
  lsr a
  ldx #44
  jsr vwfPutNibble
  lda white_color
  and #$0F
  ldx #50
  jsr vwfPutNibble
  ldy #<color_msg
  lda #>color_msg
  ldx #8
  jsr vwfPuts

  lda #%1001
  jsr rf_color_lineImgBuf
  lda #$02
  sta vram_copydsthi
  lda #$00
  sta vram_copydstlo
  
  ; Choose palette display
  ; 0-2: RGB, no box
  ; 3: white, optional box
  ; 4: black, no box
  ldx cur_color
  cpx #3
  beq load_palette_white
  bcs load_palette_black
  lda solid_color_rgb,x
  bcc have_bg_color_no_text
load_palette_white:
  lda white_color
  ldx color_box_open
  beq have_bg_color_no_text
  ldx #$02
  stx text_dark_color
  ldx #$38
  stx text_light_color
  bne have_bg_color
load_palette_black:
  lda black_color
have_bg_color_no_text:
  sta text_dark_color
  sta text_light_color
have_bg_color:
  sta cur_bg_color

  lda nmis
:
  cmp nmis
  beq :-
  lda #$3F
  sta PPUADDR
  lda #$00
  sta PPUADDR
  lda cur_bg_color
  sta PPUDATA 
  sta PPUDATA
  lda text_dark_color
  sta PPUDATA
  lda text_light_color
  sta PPUDATA
  jsr rf_copy8tiles
  ldx #0
  ldy #0
  lda #VBLANK_NMI|BG_0000
  jsr ppu_screen_on

  jsr read_pads
  lda new_keys+0
  and #KEY_START
  beq not_help
    lda #KEY_B|KEY_A|KEY_START|KEY_LEFT|KEY_RIGHT
    ldx #helpsect_solid_color_screen
    jsr helpscreen
    jmp restart
  not_help:

  lda new_keys+0
  and #KEY_RIGHT
  beq not_right
  lda color_box_open
  beq next_color
    lda white_color
    and #$0F
    clc
    adc #1
    cmp #$0D
    bcc :+
      lda #$00
    :
    eor white_color
    and #$0F
    eor white_color
    sta white_color
    jmp not_right
  next_color:
    inc cur_color
    lda cur_color
    cmp #5
    bcc not_right
      lda #0
      sta cur_color
  not_right:

  lda new_keys+0
  and #KEY_LEFT
  beq not_left
  lda color_box_open
  beq prev_color
    lda white_color
    and #$0F
    clc
    adc #<-1
    bpl :+
      lda #$0C
    :
    eor white_color
    and #$0F
    eor white_color
    sta white_color
    jmp not_left
  prev_color:
    dec cur_color
    bpl not_left
      lda #4
      sta cur_color
  not_left:

  lda new_keys+0
  and #KEY_UP
  beq not_up
  lda color_box_open
  beq not_up
    lda white_color
    clc
    adc #16
    cmp #$40
    bcc :+
      lda #$30
    :
    sta white_color
  not_up:

  lda new_keys+0
  and #KEY_DOWN
  beq not_down
  lda color_box_open
  beq not_down
    lda white_color
    sec
    sbc #16
    bcs :+
      lda #$00
    :
    sta white_color
  not_down:

  lda new_keys+0
  and #KEY_A
  beq notA
  lda cur_color
  cmp #3
  bcc notA
  bne A_toggle_black
    lda #$01
    eor color_box_open
    sta color_box_open
    bcs notA
  A_toggle_black:
    lda #$0D ^ $0F
    eor black_color
    sta black_color
  notA:

  lda new_keys+0
  and #KEY_B
  bne done
  jmp loop
done:
  rts
.endproc

.proc vwfPutNibble
  cmp #10
  bcc :+
    adc #'A'-'9'-2
  :
  adc #'0'
  jmp vwfPutTile
.endproc

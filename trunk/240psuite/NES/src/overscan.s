;
; Overscan test for 240p test suite
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
.importzp helpsect_overscan

.segment "RODATA"
overscan_rects:
  rf_rect   0,  0,256,240,$00, 0
  rf_rect  88,112,136,120,$80, RF_INCR  ; [Bottom ]Overscan:
  rf_rect 152,112,184,120,$86, RF_INCR  ; pixels (
  rf_rect  88,120,136,128,$80, RF_INCR  ; [Left ]Overscan:
  rf_rect 152,120,184,128,$86, RF_INCR  ; pixels (
  rf_rect  88,128,136,136,$80, RF_INCR  ; [Right ]Overscan:
  rf_rect 152,128,184,136,$86, RF_INCR  ; pixels (
  rf_rect 200,112,208,136,$8A, 0        ; ) ) )
  rf_rect 136,104,154,136,$F0, RF_INCR  ; pixel counts: $F0-$F7
  rf_rect 184,104,200,136,$F8, RF_INCR  ; percentages: $F8-$FF
  .byte 0

  rf_attr   0,  0,256,240, 0
  .byte 0

  ; These start at $80 and are replicated using rects above
  rf_label  89,104, 1, 0
  .byte "Overscan:",0  ; $80-$85
  rf_label 152,104, 1, 0
  .byte "pixels (",0   ; $86-$89
  rf_label 201,104, 1, 0
  .byte ")",0   ; $8A

  ; These aren't replicated
  rf_label  70,104, 1, 0
  .byte "Top",0
  rf_label  56,112, 1, 0
  .byte "Bottom",0
  rf_label  67,120, 1, 0
  .byte "Left",0
  rf_label  64,128, 1, 0
  .byte "Right",0
  .byte 0

; BCD-encoded permill amounts
; To regenerate the next two lines in python3:
; print("  .byte", ",".join('$%02d' % (min(99, i * 1000 // 240)) for i in range(25)))
; print("  .byte", ",".join('$%02d' % (i * 1000 // 256) for i in range(25)))
vert_pctages:
  .byte $00,$04,$08,$12,$16,$20,$25,$29,$33,$37,$41,$45,$50,$54,$58,$62,$66,$70,$75,$79,$83,$87,$91,$95,$99
horz_pctages:
  .byte $00,$03,$07,$11,$15,$19,$23,$27,$31,$35,$39,$42,$46,$50,$54,$58,$62,$66,$70,$74,$78,$82,$85,$89,$93

;                    top  bot left right
arrow_xadd1:  .byte  124, 124,   0,<-15
arrow_yadd1:  .byte  <-1,<-32, 115, 115
arrow_xadd2:  .byte    0,   0,   8,   8
arrow_yadd2:  .byte    8,   8,   0,   0
arrow_xmask:  .byte    0,   0, $FF, $FF
arrow_ymask:  .byte  $FF, $FF,   0,   0
arrow_negate: .byte    0, $FF,   0, $FF

overscan_pb53:
  .incbin "obj/nes/overscan.chr.pb53"

.segment "CODE"

amt_top      = test_state+0
amt_bottom   = test_state+1
amt_left     = test_state+2
amt_right    = test_state+3
change_dir   = test_state+4
upd_progress = test_state+5

.proc do_overscan
  ldx #5
  lda #0
:
  sta test_state,x
  dex
  bpl :-
restart:
  lda #VBLANK_NMI
  sta PPUCTRL
  sta help_reload
  asl a
  sta PPUMASK

  ; load the palette
  ldy #$3F
  sty PPUADDR
  lda #$00
  sta PPUADDR
  sta PPUDATA
  ldx #$20
  stx PPUDATA
  ldy #$3F
  sty PPUADDR
  lda #$11
  sta PPUADDR
  stx PPUDATA
  lda #$10
  sta PPUDATA

  lda #$00
  sta PPUADDR
  sta PPUADDR
  sta rf_curpattable
  ldy #<(overscan_pb53 + 2)
  lda #>(overscan_pb53 + 2)
  ldx overscan_pb53 + 1
  jsr unpb53_ay_xtiles
  ldx #$20
  stx rf_curnametable
  ldx #$80
  stx rf_tilenum
  ldy #<overscan_rects
  lda #>overscan_rects
  jsr rf_draw_rects_attrs_ay
  inc ciSrc
  bne :+
    inc ciSrc+1
  :
  jsr rf_draw_labels

  lda #3
  jsr overscan_prepare_side_a
  jsr overscan_copy4cols
  lda #2
  jsr overscan_prepare_side_a
  jsr overscan_copy4cols
  lda #VBLANK_NMI
  sta PPUCTRL
  lda #1
  jsr overscan_prepare_side_a
  jsr rf_copy8tiles
  lda #0
  jsr overscan_prepare_side_a
  jsr rf_copy8tiles
  jsr overscan_prepare_pxcounts
  jsr rf_copy8tiles
  jsr overscan_prepare_pctages
  jsr rf_copy8tiles

  ; Sprite map:
  ; 0-1: left
  ; 2-5: corner overlaps
  ldx #6*4
  jsr ppu_clear_oam
  
  ; TO DO:
  ; Fix corners

loop:
  jsr overscan_prepare_sprites

  ; Find something to update
  ldy upd_progress
  bne upd_not0
    lda change_dir
    jsr overscan_prepare_side_a
    inc upd_progress
    bne upd_done
  upd_not0:
  dey
  bne upd_not1
    jsr overscan_prepare_pxcounts
    inc upd_progress
    bne upd_done
  upd_not1:
  dey
  bne upd_done
    jsr overscan_prepare_pctages
    inc upd_progress
  upd_done:

  lda nmis
:
  cmp nmis
  beq :-

  ; Upload OAM first because some capture cards capture the start of
  ; vblank and can see the palette update rainbow
  lda #$00
  sta OAMADDR
  lda #>OAM
  sta OAM_DMA

  ; Set palette for arrows based on A
  lda #$3F
  sta PPUADDR
  lda #$12
  sta PPUADDR
  lda #$10
  bit cur_keys+0
  bpl :+
    lda #$26
  :
  sta PPUDATA
  bit vram_copydsthi
  bmi isnocopy
  bvs iscolcopy
    jsr rf_copy8tiles
    jmp iscopydone
  iscolcopy:
    jsr overscan_copy4cols
  iscopydone:
    lda #$FF
    sta vram_copydsthi
  isnocopy:

  ldx #0
  ldy #0
  lda #VBLANK_NMI|BG_0000|OBJ_0000
  sec
  jsr ppu_screen_on

  jsr read_pads
  ldx #0
  lda das_keys
  and #KEY_UP|KEY_DOWN|KEY_LEFT|KEY_RIGHT
  sta das_keys
  jsr autorepeat
  
  lda new_keys+0
  and #KEY_START
  beq not_help
    lda #KEY_B|KEY_A|KEY_START|KEY_LEFT|KEY_RIGHT
    ldx #helpsect_overscan
    jsr helpscreen
    jmp restart
  not_help:
  
  lda new_keys+0
  and #KEY_UP|KEY_DOWN|KEY_LEFT|KEY_RIGHT
  beq not_move
    ldy #4
    whichbtnloop:
      dey
      lsr a
      bcc whichbtnloop
    bit cur_keys+0
    bmi try_adjusting
      sty change_dir
      bpl not_move
    try_adjusting:
      tya
      eor change_dir
      cmp #2
      bcs not_move
      ; 1: increase; 0: decrease
      asl a
      clc
      adc #$FF
      clc
      ldx change_dir
      adc test_state,x
      bpl :+
        lda #0
      :
      cmp #24
      bcc :+
        lda #24
      :
      sta test_state,x
      lda #0
      sta upd_progress
  not_move:

  lda new_keys+0
  and #KEY_B
  bne done
  jmp loop
done:
  rts
.endproc

.proc overscan_prepare_sprites

  ; First draw the arrow sprites
  ldy change_dir

  ; X coordinate
  lda test_state,y
  eor arrow_negate,y
  and arrow_xmask,y
  clc
  adc arrow_xadd1,y
  sta OAM+3
  clc
  adc arrow_xadd2,y
  sta OAM+7

  ; Y coordinate
  lda test_state,y
  eor arrow_negate,y
  and arrow_ymask,y
  clc
  adc arrow_yadd1,y
  sta OAM+0
  clc
  adc arrow_yadd2,y
  sta OAM+4

  ; Tile number
  tya
  lsr a
  and #$01
  ora #$20
  sta OAM+1
  sta OAM+5

  ; Attribute
  lda #0
  sta OAM+2
  lda #$C0
  sta OAM+6
  
  ; FIXME: For now there are no corner sprites.  Later on:
  ; At the corners of the background, horizontal edges override
  ; vertical. Use sprites to extend the vertical edges.
  
  ; Corner sprites Y
  lda amt_top
  sec 
  sbc #1
  sta OAM+8
  sta OAM+12
  lda #231
  sec 
  sbc amt_bottom
  sta OAM+16
  sta OAM+20
  
  ; Corner sprites tile number
  lda amt_left
  and #$07
  ora #$10
  sta OAM+9
  sta OAM+17
  lda amt_right
  and #$07
  ora #$10
  sta OAM+13
  sta OAM+21
  
  ; Corner sprites attribute
  lda #$00
  sta OAM+10
  sta OAM+18
  lda #$40
  sta OAM+14
  sta OAM+22

  ; Corner sprites X
  lda amt_left
  and #$F8
  sta OAM+11
  sta OAM+19
  lda amt_right
  eor #$FF
  and #$F8
  sta OAM+15
  sta OAM+23

  rts
.endproc

; Drawing the numbers in the middle ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.proc overscan_prepare_pxcounts
table_index = $02
base_x = $03
  jsr clearLineImg
  lda #$0F
  sta vram_copydsthi
  ldx #$00
  stx vram_copydstlo
  stx base_x
  loop:
    stx table_index
    lda test_state,x
    jsr bcd8bit
    ora #'0'
    tay
    lda base_x
    ora #8
    tax
    tya
    jsr vwfPutTile
    lda bcd_highdigits
    beq less_than_ten
      lda base_x
      ora #3
      tax
      lda bcd_highdigits
      ora #'0'
      jsr vwfPutTile
    less_than_ten:
    lda base_x
    clc
    adc #16
    sta base_x
    ldx table_index
    inx
    cpx #4
    bcc loop
  lda #%0001
  jmp rf_color_lineImgBuf
.endproc

.proc overscan_prepare_pctages
table_index = $02
base_x = $03

  jsr clearLineImg
  
  ldx amt_top
  lda vert_pctages,x
  sta $0C
  ldx amt_bottom
  lda vert_pctages,x
  sta $0D
  ldx amt_left
  lda horz_pctages,x
  sta $0E
  ldx amt_right
  lda horz_pctages,x
  sta $0F

  lda #$0F
  sta vram_copydsthi
  ldx #$80
  stx vram_copydstlo
  ldx #$00
  stx base_x
  loop:
    stx table_index
    lda $0C,x
    lsr a
    lsr a
    lsr a
    lsr a
    ora #'0'
    sta lineImgBuf+96
    lda #'.'
    sta lineImgBuf+97
    lda $0C,x
    and #$0F
    ora #'0'
    sta lineImgBuf+98
    lda #'%'
    sta lineImgBuf+99
    ldx base_x
    txa
    clc
    adc #16
    sta base_x
    ldy #<(lineImgBuf+96)
    lda #>(lineImgBuf+96)
    jsr vwfPuts
    ldx table_index
    inx
    cpx #4
    bcc loop
  lda #%0001
  jmp rf_color_lineImgBuf
.endproc

; Drawing the edges ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.proc overscan_prepare_bulk
amt = $00
tilebase = $01
eortype = $02
  sta amt
  stx tilebase
  sty eortype

  ; Draw the long lines.  The caller fixes up the corner.
  ldx #0
  setverts:
    txa
    lsr a
    lsr a
    eor eortype
    sec
    adc amt
    bpl :+
      lda #0
    :
    cmp #8
    bcc :+
      lda #8
    :
    eor tilebase
    sta lineImgBuf,x
    txa
    clc
    adc #32
    tax
    bpl setverts

  ; Now extend these values to the whole row or column
  ldx #0
  rowloop:
    lda lineImgBuf,x
    inx
    ldy #31
    :
      sta lineImgBuf,x
      inx
      dey
      bne :-
    cpx #128
    bcc rowloop
  rts
.endproc

.proc leading_white_whole_tiles
  lsr a
  lsr a
  lsr a
  beq notlwholetiles
  tax
  dex
  lda #$08
  leadwholeloop:
    .repeat 4, I
      sta lineImgBuf+32*I,x
    .endrepeat
    dex
    bpl leadwholeloop
  notlwholetiles:
  rts
.endproc


.proc trailing_white_whole_tiles
  lsr a
  lsr a
  lsr a
  beq notlwholetiles
  eor #$1F
  tax
  inx
  lda #$08
  leadwholeloop:
    .repeat 4, I
      sta lineImgBuf+32*I,x
    .endrepeat
    inx
    cpx #32
    bcc leadwholeloop
  notlwholetiles:
  rts
.endproc

;;
; @param A distance along short end;  bit 7 set to subtract 32 instead
; @param Y distance along long end
; @param X tile xor
.proc draw_white_partial_tiles
toptile      = $00
startcol     = $01
coldirection = $02

  ; Calculate distance in tiles from short end
  and #$98
  asl a
  php
  asl a
  sta startcol
  plp
  lda #32
  bcc :+
    lda #<-32
  :
  sta coldirection

  tya
  and #$07
  beq notopedgefix
  stx toptile
  ora toptile
  sta toptile
  tya
  lsr a
  lsr a
  lsr a
  ora startcol
  tax
  clc
  tledgefix:
    lda toptile
    sta lineImgBuf,x
    txa
    clc
    adc coldirection
    tax
    bpl tledgefix
  notopedgefix:
  rts
.endproc

.proc overscan_prepare_top

  lda amt_top
  ldx #$00
  ldy #$FF
  jsr overscan_prepare_bulk

  lda amt_left
  jsr leading_white_whole_tiles
  lda amt_top
  clc
  adc #7
  ldy amt_left
  ldx #$10
  jsr draw_white_partial_tiles

  lda amt_right
  jsr trailing_white_whole_tiles
  lda #0
  sec
  sbc amt_right
  tay
  lda amt_top
  clc
  adc #7
  ldx #$18
  jsr draw_white_partial_tiles


  lda #$20
  sta vram_copydsthi
  lda #$00
  sta vram_copydstlo
  rts
.endproc

.proc overscan_prepare_bottom
  lda #7
  sec
  sbc amt_bottom
  ldx #$08
  ldy #$18
  jsr overscan_prepare_bulk

  lda amt_left
  jsr leading_white_whole_tiles
  lda #152
  sec
  sbc amt_bottom
  ldy amt_left
  ldx #$10
  jsr draw_white_partial_tiles

  lda amt_right
  jsr trailing_white_whole_tiles
  lda #0
  sec
  sbc amt_right
  tay
  lda #152
  sec
  sbc amt_bottom
  ldx #$18
  jsr draw_white_partial_tiles

toptile = $00
bottomtile = $01
toppos = $02
bottompos = $03

  ; TO DO: Draw top and bottom

  lda #$23
  sta vram_copydsthi
  lda #$40
  sta vram_copydstlo
  rts
.endproc

.proc overscan_prepare_left
  lda amt_left
  ldx #$10
  ldy #$FF
  jsr overscan_prepare_bulk
 
  lda amt_top
  jsr leading_white_whole_tiles
  lda amt_left
  ldy amt_top
  ldx #$00
  jsr draw_white_partial_tiles

  lda amt_bottom
  clc
  adc #16
  jsr trailing_white_whole_tiles
  lda #240
  sec
  sbc amt_bottom
  tay
  lda amt_left
  ldx #$08
  jsr draw_white_partial_tiles

  lda #$60
  sta vram_copydsthi
  lda #$00
  sta vram_copydstlo
  rts
.endproc

.proc overscan_prepare_right
  lda #7
  sec
  sbc amt_right
  ldx #$18
  ldy #$18
  jsr overscan_prepare_bulk

  lda amt_top
  jsr leading_white_whole_tiles
  lda #159
  sec
  sbc amt_right
  ldy amt_top
  ldx #$00
  jsr draw_white_partial_tiles

  lda amt_bottom
  clc
  adc #16
  jsr trailing_white_whole_tiles
  lda #240
  sec
  sbc amt_bottom
  tay
  lda #159
  sec
  sbc amt_right
  ldx #$08
  jsr draw_white_partial_tiles

toptile = $00
bottomtile = $01
toppos = $02
bottompos = $03

  ; TO DO: Draw top and bottom

  lda #$60
  sta vram_copydsthi
  lda #$1C
  sta vram_copydstlo
  rts
.endproc

.proc overscan_prepare_side_a
  asl a
  tax
  lda sideprocs+1,x
  pha
  lda sideprocs,x
  pha
  rts
.pushseg
.segment "RODATA"
sideprocs:
  .addr overscan_prepare_top-1  ; TODO: top
  .addr overscan_prepare_bottom-1  ; TODO: bottom
  .addr overscan_prepare_left-1
  .addr overscan_prepare_right-1
.popseg
.endproc

.proc overscan_copy4cols
  clc
  ldx #VBLANK_NMI|VRAM_DOWN
  stx PPUCTRL
  ldx #0
  colloop:
    lda vram_copydsthi
    sta PPUADDR
    lda vram_copydstlo
    sta PPUADDR
    inc vram_copydstlo
    ldy #5
    partloop:
      .repeat 6, I
        lda lineImgBuf+I,x
        sta PPUDATA
      .endrepeat
      txa
      adc #6
      tax
      dey
      bne partloop
    inx
    inx
    bpl colloop
  rts
.endproc



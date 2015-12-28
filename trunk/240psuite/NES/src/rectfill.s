;
; Nametable rectangle fill engine
; Copyright 2015 Damian Yerrick
;
; Copying and distribution of this file, with or without
; modification, are permitted in any medium without royalty provided
; the copyright notice and this notice are preserved in all source
; code copies.  This file is offered as-is, without any warranty.
;
.include "nes.inc"
.include "global.inc"
.include "rectfill.inc"

plane0and = $04
plane1and = $05
plane0xor = $06
plane1xor = $07

.segment "BSS"
rf_curpattable:   .res 1
rf_curnametable:  .res 1
rf_tilenum:       .res 1

.segment "CODE"
.proc rf_load_tiles
  lda #$80
  sta help_reload
  sta PPUCTRL
  asl a
  sta PPUMASK
  sta PPUADDR
  sta PPUADDR
:
  lda #stdtiles_bank
  sta :-+1
  ldx stdtiles_pb53 + 1
  lda #>(stdtiles_pb53 + 2)
  ldy #<(stdtiles_pb53 + 2)
  jmp unpb53_ay_xtiles
.endproc

.segment "CODE"

; Filled rectangles are described by start address, width, height,
; tile number, and row alternation.  They can be used to draw solid
; rectangles, draw 16x16 pixel grids, or leave space for VWF to be
; filled in later.  The list is terminated by width 0.
;
; byte 0   byte 1   byte 2   byte 3
; 76543210 76543210 76543210 76543210
; WWWWWWYY YYYXXXXX TTTTTTTT HHHHHIRC
; 
; X: left side X (8px)
; Y: top Y (8px)
; W: width (8px)
; H: height (8px)
; T: tile number
; I: Add 1 to tile number after each tile
; R: XOR tile number by 8 after each row
; C: XOR tile number by 1 after each tile

.proc rf_draw_rects
dstaddrlo   = $00
dstaddrhi   = $01
width       = $02
height      = $03
tilenumber  = $04
colxor      = $05
rowxor      = $06
colinc      = $07

  ; Unpack parameters for next rectfill
  ldy #0
  sty colxor
  sty rowxor
  sty colinc
  lda (ciSrc),y
  lsr a
  lsr a
  bne notDone  ; Zero width means rectfills are done
    rts
  notDone:
  sta width
  lda (ciSrc),y
  and #%00000011
  ora rf_curnametable
  sta dstaddrhi
  iny
  lda (ciSrc),y
  sta dstaddrlo
  iny
  lda (ciSrc),y
  sta tilenumber
  iny
  lda (ciSrc),y
  lsr a
  rol colxor
  lsr a
  ldx #0
  bcc :+
    ldx #8
  :
  stx rowxor
  lsr a
  rol colinc
  sta height
  
  ; Move pointer to next rectfill
  ; clc
  lda ciSrc
  adc #4
  sta ciSrc
  bcc :+
    inc ciSrc+1
  :

  ; If the width is odd, an additional colxor should be done
  ; after each row as well
  lda width
  and colxor
  ora rowxor
  sta rowxor

  rowloop:
    lda dstaddrhi
    sta PPUADDR
    lda dstaddrlo
    sta PPUADDR
    clc
    adc #32
    sta dstaddrlo
    bcc :+
      inc dstaddrhi
    :
    lda tilenumber
    ldx width
    tileloop:
      sta PPUDATA
      clc
      adc colinc
      eor colxor
      dex
      bne tileloop
    eor rowxor
    sta tilenumber
    dec height
    bne rowloop
  jmp rf_draw_rects
.endproc

; Attribute fills are used to set color areas.  The list is
; terminated by height being 0.
;
; byte 0   byte 1   byte 2
; 0000HHHH WWWWW0AA YYYXXXxy
;
; X: left side X (32px)
; x: Shift right by 16px
; Y: top Y (32px)
; y: Shift down by 16px
; W: width (16px)
; H: height (16px)
; A: attribute value

attrbuf = $0100

.proc rf_draw_attrs
dstaddr   = $00
rowmask   = $01
widleft   = $02
attrvalue = $03
oddtop    = $04
oddleft   = $05
width     = $06
height    = $07
bmask     = $08

  ; Unpack parameters for next attribute fill
  ldy #0
  lda (ciSrc),y
  and #$0F
  bne notDone
    rts
  notDone:
  sta height
  iny
  lda (ciSrc),y
  lsr a
  lsr a
  lsr a
  sta width
  lda (ciSrc),y
  and #$03
  tax
  lda times85,x
  sta attrvalue
  iny
  lda (ciSrc),y
  ldx #$00
  lsr a
  bcc :+
    ldx #$0F
    inc height
  :
  stx oddtop
  ldx #$00
  lsr a
  bcc :+
    ldx #$33
    inc width
  :
  stx oddleft
  sta dstaddr

  ; Move pointer to next attribute fill
  clc
  lda ciSrc
  adc #3
  sta ciSrc
  bcc :+
    inc ciSrc+1
  :

  ; Now take it one pair of rows at a time
  rowloop:
    ldx dstaddr
    lda width
    sta widleft
    lda height
    cmp #2
    lda oddtop
    bcs :+
      ora #$F0
    :
    sta rowmask
    ora oddleft
    sta bmask
    byteloop:
      lda widleft
      cmp #2
      bcs :+
        lda bmask
        ora #$CC
        sta bmask
      :
      lda attrbuf,x
      eor attrvalue
      and bmask
      eor attrvalue
      sta attrbuf,x
      inx
      lda rowmask
      sta bmask
      lda widleft
      sec
      sbc #2
      sta widleft
      bcc rowdone
      bne byteloop
    rowdone:
    lda #0
    sta oddtop
    lda #8
    clc
    adc dstaddr
    sta dstaddr
    lda height
    sec
    sbc #2
    sta height
    bcc filldone
    bne rowloop
  filldone:

  jmp rf_draw_attrs
.endproc 


.proc rf_draw_rects_attrs_ay
  sty ciSrc
  sta ciSrc+1
.endproc
.proc load_rects_and_attrs
  jsr rf_draw_rects
  inc ciSrc
  bne :+
    inc ciSrc+1
  :
  jsr rf_draw_attrs
  ; fall through
.endproc
.proc rf_push_attrs
  lda rf_curnametable
  ora #$03
  sta PPUADDR
  lda #$C0
  sta PPUADDR
  ldx #0
:
  lda attrbuf,x
  sta PPUDATA
  inx
  cpx #64
  bcc :-
  rts
.endproc

; Lines of text are described by color, top left corner, and text
; content.  The list is terminated by foreground and background
; colors both being 0.
;
; byte 0   byte 1   byte 2   byte 3                 byte L+3
; 0000BBAA 000YYYYY XXXXXXXX ext-ASCII encoded text 00000000
;
; X: left side X (1px)
; Y: Y (8px)
; A: Color AND
; B: Color XOR
; The XOR value is the background color, and the AND value is
; the foreground color XOR the background color.

.proc rf_draw_labels
txtptrlo     = $00
txtptrhi     = $01
width        = $02  ; number of tiles
xcoord       = $0C
ycoord       = $0D
txtcolors    = $0E

  ; Unpack parameters for text drawing
  ldy #$00
  lda (ciSrc),y
  and #$0F
  bne notDone
    rts
  notDone:
  sta txtcolors
  jsr clearLineImg
  ldy #1
  lda (ciSrc),y
  sta ycoord
  iny
  lda (ciSrc),y
  sta xcoord

  ; Draw text and measure its width in tiles
  and #$07
  tax
  clc
  lda ciSrc
  adc #3
  tay
  lda ciSrc+1
  adc #0
  jsr vwfPuts  ; X = width, AY = pointer to nul terminator

  ; Move pointer to next line of text
  sta ciSrc+1
  iny
  bne :+
    inc ciSrc+1
  :
  sty ciSrc+0

  ; Translate pixel width to tile width
  txa
  clc
  adc #7
  lsr a
  lsr a
  lsr a
  sta width

  ; Calculate destination address in nametable
  lda ycoord
  lsr a
  ror xcoord
  lsr a
  ror xcoord
  lsr a
  ror xcoord
  ora rf_curnametable
  sta PPUADDR
  lda xcoord
  sta PPUADDR

  ; Write tiles to nametable
  ldx rf_tilenum
  txa
  ldy width
:
  stx PPUDATA
  inx
  dey
  bne :-
  stx rf_tilenum

  ; Calculate destination address in pattern table
  asl a
  rol a
  rol a
  rol a
  sta txtptrlo  ; 4:3210_765
  rol a
  and #$0F
  ora rf_curpattable
  sta PPUADDR
  lda txtptrlo
  and #$F0
  sta PPUADDR

  ; Calculate AND/XOR masks for each plane
  lda txtcolors
  jsr rf_calcandxormasks

  ; now copy the line image in the appropriate colors
  ldx #0
  copytilesloop:
    lda plane0and
    ldy plane0xor
    jsr copy1tileplane
    txa
    sec
    sbc #8
    tax
    lda plane1and
    ldy plane1xor
    jsr copy1tileplane
    dec width
    bne copytilesloop
  jmp rf_draw_labels

copy1tileplane:
curplaneand = $08
curplanexor = $09
  sta curplaneand
  sty curplanexor
  ldy #8
  copyplaneloop:
    lda lineImgBuf,x
    and curplaneand
    eor curplanexor
    sta PPUDATA
    inx
    dey
    bne copyplaneloop
  rts
.endproc

.proc rf_calcandxormasks
  ldx #0
  colorcalcloop:
    ldy #0
    lsr a
    bcc :+
      dey
    :
    sty plane0and,x
    inx
    cpx #4
    bcc colorcalcloop
  rts
.endproc


;;
; Copies 128 bytes from lineImgBuf to VRAM starting at vram_copydst.
; Does not modify Y or CPU memory.
.proc rf_copy8tiles
  lda vram_copydsthi
  sta PPUADDR
  lda vram_copydstlo
  sta PPUADDR
  clc
  ldx #0
  :
    .repeat 8, I
      lda lineImgBuf+I,x
      sta PPUDATA
    .endrepeat
    txa
    adc #8
    tax
    bpl :-
  rts
.endproc

;;
; Adds foreground and background color to the VWF tiles for
; a dynamic text area.
; It overwrites memory that the VWF engine already uses.
; In particular, it preserves the VWF text pointer ($00-$01).
; @param A bits 3-2: bgcolor; bits 1-0: fgcolor
.proc rf_color_lineImgBuf
curplaneand = $08
curplanexor = $09
srcptr = $0A
  jsr rf_calcandxormasks
  .assert >(lineImgBuf + 64) = >lineImgBuf, error, "page-crossing lineImgBuf not supported"
  lda #<(lineImgBuf+64)
  sta srcptr
  lda #>(lineImgBuf+64)
  sta srcptr+1
  ldy #63
  tileloop:
    ldx plane1and
    lda plane1xor
    jsr colorize_one_plane
    clc
    tya
    adc #8
    tay
    sec
    lda srcptr
    sbc #8
    sta srcptr
    ldx plane0and
    lda plane0xor
    jsr colorize_one_plane
    cpy #$FF
    bne tileloop
  rts

colorize_one_plane:
  sta curplanexor
  stx curplaneand
  ldx #8
  byteloop:
    lda lineImgBuf,y
    and curplaneand
    eor curplanexor
    sta (srcptr),y
    dey
    dex
    bne byteloop
  rts
.endproc

.proc rf_load_yrgb_palette
  ; Load palette
  lda nmis
:
  cmp nmis
  beq :-
  lda #$3F
  sta PPUADDR
  ldy #$00
  sty PPUADDR
  palloop:
    lda gcbars_palette,y
    sta PPUDATA
    iny
    cpy #28
    bcc palloop
  rts
.endproc
.segment "RODATA"
times85:
  .byte $00, $55, $AA, $FF
gcbars_palette:
  .byte $0F,$00,$10,$20, $0F,$06,$16,$26, $0F,$0A,$1A,$2A, $0F,$02,$12,$22
  .byte $0F,$FF,$FF,$36, $0F,$FF,$FF,$3A, $0F,$FF,$FF,$32

.segment "BANK01"
stdtiles_bank = 1
stdtiles_pb53:       .incbin "obj/nes/stdtiles.chr.pb53"


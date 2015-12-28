; NES variable width font drawing library
; Copyright 2006-2015 Damian Yerrick
;
; Copying and distribution of this file, with or without
; modification, are permitted in any medium without royalty provided
; the copyright notice and this notice are preserved in all source
; code copies.  This file is offered as-is, without any warranty.

; Change history:
; 2006-03: vwfPutTile rewritten by "Blargg" (Shay Green)
; and then adapted by Damian Yerrick to match old semantics
; 2010-06: DY skipped completely transparent pattern bytes
; 2011-11: DY added string length measuring
; 2012-01: DY added support for inverse video
; 2015-02: vwfPutTile rewritten by DY again; no more Blargg code

.include "nes.inc"
.export vwfPutTile, vwfPuts, vwfPuts0
.export vwfGlyphWidth, vwfStrWidth, vwfStrWidth0
.export clearLineImg, lineImgBuf, invertTiles
.export copyLineImg_helpscreen
.exportzp lineImgBufLen
.import vwfChrData, vwfChrWidths

lineImgBuf = $0100
lineImgBufLen = 128
FONT_HT = 8

srcStr = $00
horzPos = $04
shiftedByte = $05
tileAddr = $06
shiftContinuation = $08
leftMask = $0A
rightMask = $0B

.segment "CODE"
;;
; Clears the line image buffer.
; Does not modify Y or zero page.
.proc clearLineImg
  ldx #lineImgBufLen/4-1
  lda #0
:
  .repeat 4, I
    sta lineImgBuf+lineImgBufLen/4*I,x
  .endrepeat
  dex
  bpl :-
  rts
.endproc

.macro getTileAddr
  sec
  sbc #' '
  ; Find source address
  asl a     ; 7 6543 210-
  adc #$80  ; 6 -543 2107
  rol a     ; - 5432 1076
  asl a     ; 5 4321 076-
  tay
  and #%00000111
  adc #>vwfChrData
  sta tileAddr+1
  tya
  and #%11111000
  sta tileAddr
.endmacro

.res 32  ; 1 or 32 to adjust shiftslide

;;
; Puts a 1-bit tile to position X in the line image buffer.
; In:   A = tile number
;       X = destination X position
; Trash: AXY, $05-$0B
.proc vwfPutTile
  getTileAddr

  ; Construct fast shifter
  txa
  and #%00000111
  tay
  lda leftMasks,y
  sta leftMask
  eor #$FF
  sta rightMask
  lda shiftContinuations,y
  sta shiftContinuation
  lda #>shiftslide
  sta shiftContinuation+1

  ; Process scanlines from the bottom up
  txa
  .if ::FONT_HT = 8
    ora #8-1
  .elseif ::FONT_HT = 16
    asl a
    ora #16-1
  .else
    .assert 0, error, "font size must be 8 or 16"
  .endif
  tax
  ldy #FONT_HT - 1
chrbyteloop:
  lda (tileAddr),y
  beq isBlankByte
  jmp (shiftContinuation)
shiftslide:
  rol a
  rol a
  rol a
  rol a
  rol a
  rol a
  rol a
  sta shiftedByte
  and rightMask
  ora lineImgBuf+FONT_HT,x
  sta lineImgBuf+FONT_HT,x
  lda shiftedByte
  rol a
  and leftMask
dontshift:
  ora lineImgBuf,x
  sta lineImgBuf,x
isBlankByte:
  dex
  dey
  bpl chrbyteloop
  rts

  .assert >shiftslide = >dontshift, error, "shiftslide crosses page boundary"

.pushseg
.segment "RODATA"
leftMasks:
  .repeat 8, I
    .byte $FF >> I
  .endrepeat
shiftContinuations:
  .byte <dontshift
  .repeat 7, I
    .byte <(shiftslide+I)
  .endrepeat
.popseg
.endproc

;;
; Calculates the width in pixels of a string.
; @param AAYY: string address, stored to $00-$01
; @return total pen-advance in A; strlen in Y; carry set if overflowed
; Trash: $02
.proc vwfStrWidth
str = $00
  sty str
  sta str+1
.endproc
;;
; Same as vwfStrWidth.
; @param $00-$01: string address
.proc vwfStrWidth0
str = vwfStrWidth::str
width = $02
  ldy #0
  sty width
loop:
  lda (str),y
  beq bail
  tax
  lda vwfChrWidths-32,x
  clc
  adc width
  sta width
  bcs bail
  iny
  bne loop
bail:
  lda width
  rts
.endproc

;;
; Finds both the pen-advance of a glyph and the
; columns actually occupied by opaque pixels.
; in: A = character number (32-127)
; out: A = columns containing a bit; X: pen-advance in pixels
.proc vwfGlyphWidth
  tay
  ldx vwfChrWidths-32,y
  getTileAddr
  ldy #7
  lda #0
:
  ora (tileAddr),y
  dey
  bpl :-
  rts
.endproc

;;
; Writes a string to position X, terminated by $00-$1F byte.
; In:   AAYY = string base address, stored to $00-$01
;       X = destination X position
; Out:  X = ending X position
;       $00-$01 = END of string
;       AAYY = End of string, plus one if greater than 0
; Trash: $04-$0B
.proc vwfPuts
  sty srcStr
  sta srcStr+1
.endproc
.proc vwfPuts0
  stx horzPos
loop:
  ldy #0
  lda (srcStr),y
  beq done0
  cmp #32
  bcc doneNewline
  beq isSpace
  ldx horzPos
  jsr vwfPutTile
  ldy #0
isSpace:
  lda (srcStr),y
  inc srcStr
  bne :+
  inc srcStr+1
:
  tax
  lda vwfChrWidths-32,x
  clc
  adc horzPos
  sta horzPos
  cmp #lineImgBufLen
  bcc loop

doneNewline:
  lda #1
done0:
  clc
  adc srcStr
  tay
  lda #0
  adc srcStr+1
  ldx horzPos 
  rts
.endproc

;;
; Inverts the first A tiles in lineImgBuf.
.proc invertTiles
  asl a
  asl a
  asl a
  .if ::FONT_HT = 16
    asl a
  .endif
  tax
  dex
invertloop:
  lda #$FF
  eor lineImgBuf,x
  sta lineImgBuf,x
  dex
  bne invertloop
  lda #$FF
  eor lineImgBuf
  sta lineImgBuf
  rts
.endproc

;;
; Copies a rendered line of text to CHR RAM.
; in:  AAYY = destination address ($0000-$1F00)
; (usually Y = $00 or $08)
; trash: $00
.proc copyLineImg_helpscreen
  sta PPUADDR
  sty PPUADDR
  ldx #0
  clc
partloop:
  .repeat 2, plane
    .repeat 2, pair
      .repeat 8, scanline  
        lda lineImgBuf+8*plane+16*pair+scanline,x
        sta PPUDATA
      .endrepeat
    .endrepeat
  .endrepeat
  txa
  adc #32
  tax
  cpx #lineImgBufLen
  bcs done
  jmp partloop
done:
  rts
.endproc



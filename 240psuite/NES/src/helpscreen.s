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
.import helptitles_hi, helptitles_lo
.import helppages_hi, helppages_lo, help_cumul_pages
.importzp HELP_NUM_PAGES, HELP_NUM_SECTS, HELP_BANK

.segment "BSS"
help_cur_seg:   .res 1
help_cur_page:  .res 1
help_cur_line:  .res 1
help_reload:    .res 1
help_ok_keys:   .res 1
prev_nonblank:  .res 1
cur_nonblank:   .res 1
help_cursor_y:  .res 1
cursor_dirty:   .res 1
vram_copydstlo: .res 1
vram_copydsthi: .res 1
pagenumberbuf:  .res 32

.segment "CODE"

;;
; @param A the keys that are OK to use
;   usually this includes KEY_LEFT|KEY_RIGHT
;   for menu selection, use KEY_UP|KEY_DOWN|KEY_A|KEY_START
;   for going back, use KEY_B
; @param X the segment number
; @param help_cur_page the page to start on.
;   If not within the segment, goes to the segment's first page.
; @return A: number of page within segment; Y: cursor Y position
.proc helpscreen
  sta help_ok_keys
  stx help_cur_seg
  lda help_reload
  bpl :+
    jsr helpscreen_load
    lda #0
    sta prev_nonblank
    sta help_reload
  :

  ; If not within this document, move to the first page
  ldx help_cur_seg
  lda help_cur_page
  cmp help_cumul_pages,x
  bcc movetofirstpage
  cmp help_cumul_pages+1,x
  bcc nomovepage
movetofirstpage:
  lda help_cumul_pages,x
  sta help_cur_page
  lda #0
  sta help_cursor_y
nomovepage:
  lda #0
  sta help_cur_line
  lda #2
  sta cursor_dirty
  sta cur_nonblank

loop:
  jsr help_prepare_line
  lda nmis
:
  cmp nmis
  beq :-

  ldx #0
  stx OAMADDR
  lda #>OAM
  sta OAM_DMA

  lda vram_copydsthi
  bmi nocopytxt
  ldy vram_copydstlo
  jsr copyLineImg_helpscreen
  lda #$FF
  sta vram_copydsthi
  bpl vramdone
nocopytxt:
  lda cursor_dirty
  beq vramdone
  jsr help_draw_cursor
  lda #0
  sta cursor_dirty
vramdone:

  ldx #0
  ldy #8
  lda #VBLANK_NMI|BG_0000|OBJ_8X16
  sec
  jsr ppu_screen_on
  
  jsr read_pads
  ldx #0
  jsr autorepeat
  lda das_keys+0
  and #KEY_UP|KEY_DOWN
  sta das_keys+0
  lda new_keys+0
  and help_ok_keys
  sta new_keys+0

  and #KEY_RIGHT
  beq notNextPage
  ldx help_cur_seg
  lda help_cumul_pages+1,x
  sec
  sbc help_cur_page
  bcc notNextPage
  cmp #2
  bcc notNextPage
  inc help_cur_page
  lda #0
  sta help_cur_line  ; 0 in current line triggers new page
notNextPage:

  lda new_keys+0
  and #KEY_LEFT
  beq notPrevPage
  ldx help_cur_seg
  lda help_cumul_pages,x
  cmp help_cur_page
  bcs notPrevPage
  dec help_cur_page
  lda #0
  sta help_cur_line
notPrevPage:

  lda new_keys+0
  and #KEY_UP
  beq notCursorUp
  lda help_cursor_y
  beq notCursorUp
  sta cursor_dirty
  dec help_cursor_y
notCursorUp:

  lda new_keys+0
  and #KEY_DOWN
  beq notCursorDown
  lda prev_nonblank
  sec
  sbc help_cursor_y
  bcc notCursorDown
  cmp #4
  bcc notCursorDown
  sta cursor_dirty
  inc help_cursor_y
notCursorDown:

  ; B: Exit  
  lda new_keys+0
  and #KEY_B
  bne done

  ; A: Exit if has drawn up to the cursor Y
  lda new_keys+0
  and #KEY_A|KEY_START
  beq notPressA
  lda help_cursor_y
  clc
  adc #2
  cmp help_cur_line
  bcc done
  
notPressA:

  jmp loop
done:
  ldx help_cur_seg
  lda help_cur_page
  sec
  sbc help_cumul_pages,x
  ldy help_cursor_y
  rts
.endproc

.proc help_prepare_line
  jsr clearLineImg
  lda help_cur_line
  bne not_title_line

  ; Title line: Set ciSrc to the start of the page, then
  ; draw the section's title
  ldx help_cur_page
  lda helppages_hi,x
  sta ciSrc+1
  lda helppages_lo,x
  sta ciSrc+0
  ldx help_cur_seg
  lda helptitles_hi,x
  ldy helptitles_lo,x
have_ay:
  ldx #0
  jsr vwfPuts
finish_line:
  lda help_cur_line
  inc help_cur_line
  clc
  adc #$0500>>7
  lsr a
  sta vram_copydsthi
  lda #0
  ror a
  sta vram_copydstlo
  lda #16
  jmp invertTiles

not_title_line:
  cmp #1
  bne not_pagenum_line

  lda help_ok_keys
  and #KEY_B
  beq no_draw_b_exit
    lda #>b_exit_msg
    ldy #<b_exit_msg
    ldx #96
    jsr vwfPuts
  no_draw_b_exit:

  lda help_ok_keys
  and #KEY_DOWN
  beq no_draw_updowna 
    lda #>updowna_msg
    ldy #<updowna_msg
    ldx #38
    jsr vwfPuts
  no_draw_updowna:

  ldy #0
pagenum_template_loop:
  lda pagenum_template,y
  sta pagenumberbuf,y
  beq pagenum_template_done
  iny
  bne pagenum_template_loop
pagenum_template_done:
  
  ldx help_cur_seg
  lda help_cur_page
  sec
  sbc help_cumul_pages,x
  clc
  adc #'1'  ; Currently do not allow more than 9 pages per document
  sta pagenumberbuf+2
  lda help_cumul_pages+1,x
  sec
  sbc help_cumul_pages,x
  clc
  adc #'0'
  sta pagenumberbuf+4
  lda #2
  sta cur_nonblank
  lda #>pagenumberbuf
  ldy #<pagenumberbuf
  jmp have_ay

not_pagenum_line:
  cmp #22
  bcs page_done

  ; Treat leading tab characters as 16 pixels each
  ldx #0
  ldy #0
tabloop:
  lda (ciSrc),y
  beq is_null_line
  cmp #9
  bne tabdone
  txa
  adc #16-1  ; -1 because carry is always set after CMP BNE
  tax
  inc ciSrc
  bne tabloop
  inc ciSrc+1
  bne tabloop
tabdone:

  ; Mark this line as having something on it
  ; prev_nonblank is how many lines are actually not blank
  ; cur_nonblank is how many lines will be not blank after this
  ; page completes
  ldy help_cur_line
  iny
  sty cur_nonblank
  cpy prev_nonblank
  bcc :+
    sty prev_nonblank
  :
  
  ; And draw the text at the horizontal position calculated above
  lda ciSrc+1
  ldy ciSrc
  jsr vwfPuts
  sta ciSrc+1
  sty ciSrc
  jmp finish_line
is_null_line:
  ldy cur_nonblank
  dey
  dey
  dey
  cpy help_cursor_y
  bcs :+
    sty cursor_dirty
    sty help_cursor_y
  :

  lda help_cur_line
  cmp prev_nonblank
  bcs page_done
;  lda #'n'
;  ldx #16
;  jsr vwfPutTile
  jmp finish_line
page_done:
  lda cur_nonblank
  sta prev_nonblank
  lda #$FF
  sta help_cur_line
  rts
.endproc

.proc help_draw_cursor
  lda #$20
  sta PPUADDR
  lda #$CE
  sta PPUADDR
  lda #VBLANK_NMI|VRAM_DOWN
  sta PPUCTRL
  ldy help_cursor_y
  iny
  lda help_ok_keys
  and #KEY_DOWN
  bne :+
    ldy #0
  :
  clc
  lda #$60
cursorloop:
  tax
  dey
  bne :+
    ldx #$4F
  :
  stx PPUDATA
  adc #8
  bcc cursorloop
  rts
.endproc

.segment "RODATA"
hello_msg:  .byte "Hello from VWF",0

; Help screen background ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.segment "CODE"
.proc helpscreen_load

hbset:
  lda #HELP_BANK
  sta hbset+1

  ldy #<gus_bg
  lda #>gus_bg
  ldx #$00  ; bit 4: pattern table; bits 3-2: nametable
  jsr load_sb53_ay

  ; Load palette for sprite and VWF text
  lda #$3F
  sta PPUADDR
  ldy #$09
  sty PPUADDR
palloop:
  lda helpscreen_palette-$09,y
  iny
  sta PPUDATA
  cpy #20
  bcc palloop
  
  ; Load character sprite tiles
  lda #$10
  sta PPUADDR
  lda #$00
  sta PPUADDR
  lda #>(gus_sprite+2)
  ldy #<(gus_sprite+2)
  ldx gus_sprite+1
  jsr unpb53_ay_xtiles

  ; Load arrow tiles
  lda #$04
  sta PPUADDR
  lda #$E0
  sta PPUADDR
  lda #>cursor_pb53
  ldy #<cursor_pb53
  ldx #2
  jsr unpb53_ay_xtiles

  ; Load tilemap for VWF
dstlo   = $00
dsthi   = $01
tilenum = $02

  ldx #$50
  lda #$20  ; $20CE: Body text (16x20 tiles)
  sta dsthi
  ldy #$CE
  sty dstlo
  ldy #$8E  ; $208E: Page title (16x1 tiles)
  jsr setup_one_vwf_line
  lda #$23  ; $236E: Page number (16x1 tiles)
  ldy #$6E
  jsr setup_one_vwf_line
  
vwfmap_rowloop:
  lda dsthi
  ldy dstlo
  jsr setup_one_vwf_line
  lda dstlo
  clc
  adc #32
  sta dstlo
  bcc :+
    inc dsthi
  :
  cpx #0
  bne vwfmap_rowloop

  ; Clear out the tiles in the VWF text area ($0500-$0FFF)
  lda #$FF
  tay
  ldx #$05
  jsr ppu_clear_nt
  ldx #$08
  jsr ppu_clear_nt
  ldx #$0C
  jsr ppu_clear_nt

  ; Prepare Gus sprite
strip_y = $00
strip_height = $01

  ldx #0
  ldy #0
sprstriploop:
  lda gus_sprite_strips+0,y
  cmp #$FF
  bcs sprstripdone
  sta strip_y
  lda gus_sprite_strips+1,y
  sta tilenum
  lda gus_sprite_strips+4,y
  sta strip_height
sprstripentryloop:
  lda strip_y
  sta OAM,x
  inx
  clc
  adc #16
  sta strip_y
  lda tilenum
  inc tilenum
  inc tilenum
  sta OAM,x
  inx
  lda gus_sprite_strips+2,y
  sta OAM,x
  inx
  lda gus_sprite_strips+3,y
  sta OAM,x
  inx
  dec strip_height
  bne sprstripentryloop
  tya
  clc
  adc #5
  tay
  bcc sprstriploop
sprstripdone:
  jsr ppu_clear_oam
  
  rts

setup_one_vwf_line:
  sta PPUADDR
  sty PPUADDR
  ldy #4
vwfmap_tileloop:
  txa
  inx
  sta PPUDATA
  stx PPUDATA
  sta PPUDATA
  stx PPUDATA
  inx
  dey
  bne vwfmap_tileloop
  rts
.endproc

;;
; Loads an SB53 file at address AAYY to the pattern table and
; nametable.
; X bit 4: pattern table base ($0000 or $1000)
; X bits 3-2: nametable base ($2000, $2400, $2800, or $2C00)
.proc load_sb53_ay
  sty ciSrc
  sta ciSrc+1
.endproc
.proc load_sb53

  ; Seek to the start of the pattern table
  lda #VBLANK_NMI
  ldy #0
  sta PPUCTRL
  sty PPUMASK
  txa
  pha
  and #$10
  sta PPUADDR
  sty PPUADDR

  ; Read the number of used tiles and decompress that many tiles
  lda (ciSrc),y
  inc ciSrc
  bne :+
    inc ciSrc+1
  :
  tax
  jsr unpb53_xtiles

  ; Decompress the nametable
  pla
  and #$0C
  ora #$20
  sta PPUADDR
  lda #$00
  sta PPUADDR
  ldx #1024/16
  jsr unpb53_xtiles

  ; Load the palette
  lda nmis
:
  cmp nmis
  beq :-
  lda #$3F
  sta PPUADDR
  ldy #$00
  sty PPUADDR
palloop:
  lda (ciSrc),y
  iny
  sta PPUDATA
  cpy #16
  bcc palloop
  rts
.endproc

.segment "BANK00"
gus_bg:
  .incbin "obj/nes/gus_bg.sb53"
gus_sprite:
  .incbin "obj/nes/gus_sprite.chr.pb53"
cursor_pb53:
  .byte $87
  .byte $40,$FF,$9F,$87,$81,$87,$9F,$FF,$82

gus_sprite_strips:
  .byte 115,$21,$40,32, 3
  .byte 103,$27,$40,40, 5
  .byte  55,$11,$40,48, 8
  .byte  55,$01,$40,56, 8
  .byte  55,$01,$00,64, 8
  .byte  55,$11,$00,72, 8
  .byte 103,$27,$00,80, 5
  .byte 115,$21,$00,88, 3
  .byte $FF
helpscreen_palette:
  .byte     $20,$0F,$20,$0F,$0F,$20,$20
  .byte $0F,$02,$27,$20
pagenum_template:  .byte 134," 1/1 ",135,0
updowna_msg:       .byte 132,133,"A: Select",0
b_exit_msg:        .byte "B: Exit",0




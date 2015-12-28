.include "nes.inc"
.include "global.inc"
.include "rectfill.inc"
.importzp helpsect_manual_lag_test

; Megaton is the manual lag test.

CENTERPOS = 128
LEFTWALL = 128-36
RIGHTWALL = 128+36
MAX_TESTS = 10
; Normally sides are 72 apart. When randomize is on, the side is
; temporarily moved (rand()&0x0F)-8 pixels to the right of its
; normal position.
num_tests = test_state + MAX_TESTS
mt_dirty  = test_state + 16
mt_cursor_y = test_state + 17
reticlepos = test_state + 18
reticletarget = test_state + 19
reticledir = test_state + 20  ; 1: horizontal; 2: vertical; 3: both
lastgrade = test_state + 21  ; 0-5: marv-miss; >5: no grade
lastrawlag = test_state + 22  ; in signed magnitude (7 set: early)
enableflags = test_state + 23

EN_RANDOM = $80
EN_BEEP = $40

MT_DIRTY_CBOXCOLOR = $01
MT_DIRTY_RAWLAG = $02
MT_DIRTY_LAG_HISTORY = $04



; 00-17: Reticle tiles
; 18-44: Static labels
; 45-5B: Reserved for more labels
; 5C-7F: Grade labels
; 80-87: Last raw lag
; 88-9B: Raw lag values
GRADE_NAMES_TILE_BASE = $5C
TILES_PER_GRADE_NAME = 6
RAWLAG_TILE_BASE = $80
LAG_HISTORY_TILE_BASE = $88
TILES_PER_LAG_HISTORY = $02

.segment "BANK01"
megatontilesbank = 1
megatontiles:  .incbin "obj/nes/megatontiles.chr.pb53"

megaton_rects:
  rf_rect   0,  0,256,240,$00, 0
  rf_rect 112, 96,144,128,$08, RF_INCR  ; The receptor
  rf_rect  96,136,160,144,RAWLAG_TILE_BASE, RF_INCR
  rf_rect  32, 40, 32 + 8 * MAX_TESTS * TILES_PER_LAG_HISTORY, 48,LAG_HISTORY_TILE_BASE, RF_INCR
  .byte 0
  rf_attr   0,  0,256,240, 0
  .byte 0
  ; rf_label stuff goes here
  rf_label  16,184, 3, 0
  .byte "Press A when reticles align",0
  rf_label  32,192, 3, 0
  .byte "Direction:",0
  rf_label 100,192, 3, 0
  .byte "horizontal",0
  rf_label 164,192, 3, 0
  .byte "vertical",0
  rf_label  32,200, 3, 0
  .byte "Randomize:",0
  rf_label 100,200, 3, 0
  .byte "on",0
  rf_label  32,208, 3, 0
  .byte "Audio:",0
  rf_label 100,208, 3, 0
  .byte "on",0
  .byte 0

grade_names:
  .byte "Marvelous!",0
  .byte "Perfect!",0
  .byte "Great!",0
  .byte "Good",0
  .byte "Boo",0
  .byte "Miss",0
exact_msg:  .byte "exact",0
late_msg:   .byte "late",0
early_msg:  .byte "early",0

.segment "RODATA"

; Let's flash the reticle based on DDR colors
; The actual window widths on DDR Extreme are 2, 4, 11, 17, 27
; And these are in half frames on each side because it reads pads
; twice per frame.
; We don't have that luxury, so we just round times up.
window_widths:  .byte 1, 2, 6, 9, 14, 255
NUM_GRADES = * - window_widths
grade_colors:  .byte $20,$38,$2A,$21,$24,$16,$0F

reticle_y:     .byte <-33,<-33,<-33,<-33,<-25,<-25,<-17,<-17, <-9, <-9, <-9, <-9
reticle_tile:  .byte  $02, $03, $03, $02, $01, $01, $01, $01, $02, $03, $03, $02
reticle_attr:  .byte  $00, $00, $40, $40, $00, $40, $80, $C0, $80, $80, $C0, $C0
reticle_x:     .byte <-16, <-8,   0,   8,<-16,   8,<-16,   8,<-16, <-8,   0,   8

.segment "CODE"

.proc do_manual_lag
  lda #0
  sta num_tests
  sta mt_cursor_y
  lda #1
  sta reticledir
  lda #EN_RANDOM
  sta enableflags

.if 0
  ldy #MAX_TESTS-1
:
  tya
  asl a
  asl a
  sta test_state,y
  dey
  bpl :-
  jmp megaton_show_results
.endif

restart:
  lda #MT_DIRTY_CBOXCOLOR
  sta mt_dirty
  lda nmis
  sta CRCLO
  eor #1
  sta CRCHI
:
  lda #megatontilesbank
  sta :-+1
  lda #VBLANK_NMI
  sta lastgrade
  sta help_reload

  sta PPUCTRL
  asl a
  sta PPUMASK
  sta PPUADDR
  sta PPUADDR
  sta rf_curpattable

  lda #LEFTWALL
  sta reticlepos
  lda #RIGHTWALL
  sta reticletarget

  ldy #<(megatontiles + 2)
  lda #>(megatontiles + 2)
  ldx megatontiles + 1
  jsr unpb53_ay_xtiles
  jsr rf_load_yrgb_palette
  lda #$3F
  sta PPUADDR
  lda #$13
  sta PPUADDR
  lda #$20
  sta PPUDATA
  sta rf_curnametable
  lda #$18
  sta rf_tilenum
  ldy #<megaton_rects
  lda #>megaton_rects
  jsr rf_draw_rects_attrs_ay
  inc ciSrc
  bne :+
    inc ciSrc+1
  :
  jsr rf_draw_labels

  ; Load grade labels
  lda #<grade_names
  sta $00
  lda #>grade_names
  sta $01
  lda #GRADE_NAMES_TILE_BASE
  gradenameloop:
    sta rf_tilenum
    
    ; Centering
    jsr clearLineImg
    jsr vwfStrWidth0
    lsr a
    eor #$FF
    sec
    adc #4 * TILES_PER_GRADE_NAME
    tax
    jsr vwfPuts0
    inc $00
    bne :+
      inc $01
    :
    lda #%0011
    jsr rf_color_lineImgBuf
    lda rf_tilenum
    asl a
    rol a
    rol a
    rol a
    pha
    and #$F0
    sta vram_copydstlo
    pla
    rol a
    and #$0F
    sta vram_copydsthi
    jsr rf_copy8tiles
    lda rf_tilenum
    clc
    adc #TILES_PER_GRADE_NAME
    cmp #GRADE_NAMES_TILE_BASE + TILES_PER_GRADE_NAME * NUM_GRADES
    bcc gradenameloop
  lda #$FF
  sta vram_copydsthi
  
  ; Clear current lag and lag history tiles
  ldx #$08
  lda #$00
  tay
  jsr ppu_clear_nt
  jsr draw_lag_history_so_far

  ; Set up audio output
  lda #$0F
  sta SNDCHN
  lda #8
  sta $4001  ; disable sweep
  lda #$B0
  sta $4000  ; volume=0
  lda #111
  sta $4002  ; 1 kHz
  lda #$00
  sta $4003

  ; Now everything is set up

loop:
  ldx #0
  stx oam_used

  jsr move_reticle

  ; Draw reticle
  lda reticledir
  and #1
  beq :+
    ldx reticlepos
    ldy #128
    jsr draw_one_reticle
  :
  lda reticledir
  and #2
  beq :+
    ldx #128
    ldy reticlepos
    jsr draw_one_reticle
  :
  jsr mt_draw_cursor
  ldx oam_used
  jsr ppu_clear_oam

  ; Prepare background updates if there's not already one prepared
  lda vram_copydsthi
  bpl prepdone
  lda mt_dirty
  lsr a
  lsr a
  bcc nopreprawlag
    jsr mt_prepare_rawlag
    jmp prepdone
  nopreprawlag:
  lsr a
  bcc prepdone
    lda num_tests
    jsr mt_prepare_lag_history
  prepdone:

  lda nmis
:
  cmp nmis
  beq :-

  ; Update the first thing that's dirty  
  lda mt_dirty
  lsr a
  bcc nocboxupdate
    jsr update_checkboxes
    jmp vramupdatedone
  nocboxupdate:
  lda vram_copydsthi
  bmi vramupdatedone
    jsr rf_copy8tiles
    lda #$FF
    sta vram_copydsthi
  vramupdatedone:

  ldx #0
  stx OAMADDR
  lda #>OAM
  sta OAM_DMA
  ldy #0
  lda #VBLANK_NMI|OBJ_0000|BG_0000
  sec
  jsr ppu_screen_on

  lda #$B0
  ldx reticlepos
  cpx #128
  bne no_beep_this_frame
  bit enableflags
  bvc no_beep_this_frame
  lda #$BC
no_beep_this_frame:
  sta $4000
  
  jsr read_pads

  ; If A is pressed for the first time this pass, grade it
  lda new_keys
  bpl notA
  lda lastgrade
  cmp #NUM_GRADES
  bcc notA
    jsr grade_press  
  notA:

  ; If Start is pressed, pause and show help screen
  lda new_keys
  and #KEY_START
  beq not_help
    ldx #$B0  ; turn off beeper
    stx $4000
    ldx #helpsect_manual_lag_test
    lda #KEY_A|KEY_START|KEY_B|KEY_LEFT|KEY_RIGHT
    jsr helpscreen
    jmp restart
  not_help:
  
  lda new_keys
  jsr handle_cursor_move

  ldy num_tests
  cpy #MAX_TESTS
  bcs megaton_show_results
  lda new_keys
  and #KEY_B
  bne done
  jmp loop
done:
  lda #$B0  ; silence beeper
  sta $4000
  rts
.endproc

.segment "BANK01"

megaton_result_rects:
  rf_rect   0,  0,256,240,$00, 0
  rf_rect  64, 48,64+8*TILES_PER_LAG_HISTORY, 48+8*MAX_TESTS, LAG_HISTORY_TILE_BASE, RF_INCR
  rf_rect  64, 64 + 8 * MAX_TESTS,112,72 + 8 * MAX_TESTS, RAWLAG_TILE_BASE, RF_INCR
  .byte $00
  rf_attr   0,  0,256,240, 0
  .byte 0
  ; rf_label stuff goes here
  rf_label  32,32, 3, 0
  .byte "Measured lag:",0
  rf_label  32,56 + 8 * MAX_TESTS, 3, 0
  .byte "Average lag:",0
  rf_label  32,200, 3, 0
  .byte "Press B to close",0
  .byte 0
  
msg_frames: .byte " frames"

.segment "CODE"

.proc megaton_show_results
:
  lda #megatontilesbank
  sta :-+1
  lda #$B0  ; silence beeper
  sta $4000
  lda #VBLANK_NMI
  sta PPUCTRL
  sta help_reload
  asl a
  sta PPUMASK
  sta rf_curpattable
  sta PPUADDR
  sta PPUADDR
  ldx #$20
  stx rf_curnametable
  stx rf_tilenum
  :
    sta PPUDATA
    dex
    bne :-

  jsr draw_lag_history_so_far

  ; Sum
lagtotal = $0C
lagtotal100 = $0D
  lda #0
  sta lagtotal100
  ldy #MAX_TESTS-1
  clc
  lagsumloop:
    adc test_state,y
    bcc addwrap1
      sbc #200
      inc lagtotal100
      inc lagtotal100
    addwrap1:
    cmp #100
    bcc addwrap0
      sbc #100
      inc lagtotal100
      bcs addwrap1
    addwrap0:
    dey
    bpl lagsumloop
  sta lagtotal
  sta $FF

  ; Write sum
  ldy #0
  lda lagtotal100
  beq lessthan100frames
    jsr bcd8bit
    tax
    lda bcd_highdigits
    beq :+
      ora #'0'
      sta txtlinebuf,y
      iny
    :
    txa
    ora #'0'
    sta txtlinebuf,y
    iny
  lessthan100frames:
  lda lagtotal
  jsr bcd8bit
  ora #'0'
  sta txtlinebuf+2,y
  lda bcd_highdigits
  eor #'0'
  sta txtlinebuf,y
  lda #'.'
  sta txtlinebuf+1,y
  iny
  iny
  iny
  ldx #0
  suffixloop:
    lda msg_frames,x
    sta txtlinebuf,y
    beq suffixdone
    inx
    iny
    bpl suffixloop
  suffixdone:

  jsr clearLineImg
  ldy #<txtlinebuf
  lda #>txtlinebuf
  ldx #0
  jsr vwfPuts
  lda #%0011
  jsr rf_color_lineImgBuf
  lda #>(RAWLAG_TILE_BASE * 16)
  sta vram_copydsthi
  lda #<(RAWLAG_TILE_BASE * 16)
  sta vram_copydstlo
  jsr rf_copy8tiles
  ldy #<megaton_result_rects
  lda #>megaton_result_rects
  jsr rf_draw_rects_attrs_ay
  inc ciSrc
  bne :+
    inc ciSrc+1
  :
  jsr rf_draw_labels


loop:
  lda nmis
:
  cmp nmis
  beq :-
  ldx #0
  ldy #0
  lda #VBLANK_NMI|BG_0000
  clc
  jsr ppu_screen_on
  jsr read_pads
  lda new_keys+0
  and #KEY_B|KEY_START
  beq loop
  rts
.endproc

; movement ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.proc move_reticle
  ; Move reticle
  lda reticletarget
  cmp reticlepos
  lda #0
  adc #$FF
  sec
  rol a
  clc
  adc reticlepos
  sta reticlepos
  cmp reticletarget
  bne not_turn_around
    lda #$FF
    sta lastgrade
    lda #MT_DIRTY_CBOXCOLOR
    ora mt_dirty
    sta mt_dirty
    lda #RIGHTWALL
    bit reticlepos
    bpl :+
      lda #LEFTWALL
    :
    sta reticletarget
    bit enableflags
    bpl :+
      jsr rand_crc
      and #$0F
      clc
      adc #<-8
      clc
      adc reticletarget
      sta reticletarget
    :
  not_turn_around:

  rts
.endproc

.proc handle_cursor_move
  lsr a
  bcc not_increase
    ldy mt_cursor_y
    bne enable_toggles
    inc reticledir
    lda reticledir
    cmp #4
    bcc :+
      lda #1
      sta reticledir
    :
  request_checkbox_update:
    lda #MT_DIRTY_CBOXCOLOR
    ora mt_dirty
    sta mt_dirty
    rts
  not_increase:

  lsr a
  bcc not_decrease
    ldy mt_cursor_y
    bne enable_toggles
    dec reticledir
    bne :+
      lda #3
      sta reticledir
    :
    jmp request_checkbox_update
  enable_toggles:
    dey
    bne :+
      lda #EN_RANDOM
      bmi have_toggle_id
    :
    lda #EN_BEEP
    have_toggle_id:
    eor enableflags
    sta enableflags
    jmp request_checkbox_update
  not_decrease:

  lsr a
  bcc not_next_row
    lda mt_cursor_y
    cmp #2
    bcs :+
      inc mt_cursor_y
    :
    jmp request_checkbox_update
  not_next_row:

  lsr a
  bcc done
    lda mt_cursor_y
    beq :+
      dec mt_cursor_y
    :
    jmp request_checkbox_update
  done:
  rts
.endproc

;;
; Grades an A button press.
.proc grade_press
absdist = $0C

  lda reticlepos
  eor #$80
  bpl notNeg
    eor #$FF
    sec
    adc #1
  notNeg:
  sta absdist
  
  ; Translate absolute time difference to a color index and name
  ; based on DDR rules.  Linear search for the first window width
  ; wider than the absolute time difference.
  ldy #0
  findgrade:
    cmp window_widths,y
    bcc foundgrade
    iny
    cpy #NUM_GRADES-1
    bcc findgrade
  foundgrade:
  sty lastgrade

  ; If absdist is nonzero, bit 7 of reticlepos XOR reticletarget
  ; is true only if early.
  lda absdist
  beq :+
    lda reticlepos
    eor reticletarget
    and #$80
    ora absdist
  :
  sta lastrawlag

  ; Record only on-time and late presses
  bmi skip_recording_early
    ldy num_tests
    sta test_state,y
    inc num_tests
    lda #MT_DIRTY_LAG_HISTORY
    ora mt_dirty
    sta mt_dirty
  skip_recording_early:
  
  lda #MT_DIRTY_CBOXCOLOR|MT_DIRTY_RAWLAG
  ora mt_dirty
  sta mt_dirty
  rts
.endproc

; Background updates ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;
; Updates the grade color and the checkboxes
.proc update_checkboxes
  lda mt_dirty
  and #<~MT_DIRTY_CBOXCOLOR
  sta mt_dirty

  ; 1. Grade color
  ldy lastgrade
  cpy #6
  bcc :+
    ldy #6
  :
  lda #$3F
  sta PPUADDR
  lda #$01
  sta PPUADDR
  lda grade_colors,y
  sta PPUDATA

  ; $230B: horizontal display enabled
  ; $2313: vertical display enabled
  ; $232B: randomize enabled
  ; $233B: audio enabled
  ldy #$23
  lda reticledir
  sta $00
  sty PPUADDR
  lsr $00
  lda #$0B
  sta PPUADDR
  lda #2
  rol a
  sta PPUDATA
  sty PPUADDR
  lsr $00
  lda #$13
  sta PPUADDR
  lda #2
  rol a
  sta PPUDATA

  lda enableflags
  sta $00
  sty PPUADDR
  asl $00
  lda #$2B
  sta PPUADDR
  lda #2
  rol a
  sta PPUDATA
  sty PPUADDR
  asl $00
  lda #$4B
  sta PPUADDR
  lda #2
  rol a
  sta PPUDATA

  lda #$22
  sta PPUADDR
  lda #$0D
  sta PPUADDR
  ldy #6
  lda lastgrade
  cmp #NUM_GRADES
  bcs nogradetext
  asl a
  adc lastgrade
  asl a
  adc #GRADE_NAMES_TILE_BASE
  :
    sta PPUDATA
    adc #1
    dey
    bne :-
  jmp gradedone
  nogradetext:
  lda #0
  :
    sta PPUDATA
    dey
    bne :-
  gradedone:
  rts
.endproc

.proc mt_prepare_rawlag
  jsr clearLineImg
  lda lastrawlag
  beq is_exact
  and #$7F
  jsr bcd8bit
  ora #'0'
  ldx #16
  jsr vwfPutTile
  lda bcd_highdigits
  beq less_than_ten
    ora #'0'
    ldx #11
    jsr vwfPutTile
  less_than_ten:  
  ldy #<late_msg
  lda #>late_msg
  ldx #26
  bit lastrawlag
  bpl have_text
  ldy #<early_msg
  lda #>early_msg
  bne have_text
is_exact:  
  ldy #<exact_msg
  lda #>exact_msg
  ldx #20
have_text:
  jsr vwfPuts
  lda #<~MT_DIRTY_RAWLAG
  and mt_dirty
  sta mt_dirty
  lda #<(RAWLAG_TILE_BASE << 4)
  sta vram_copydstlo
  lda #>(RAWLAG_TILE_BASE << 4)
  sta vram_copydsthi
  lda #%0011
  jmp rf_color_lineImgBuf
.endproc

.proc mt_prepare_lag_history
entry_to_draw = $0C
x_position = $0D

  sec
  sbc #8/TILES_PER_LAG_HISTORY
  bcs :+
    lda #0
  :
  sta entry_to_draw

  ; Calculate destination address in VRAM
  clc
  adc #LAG_HISTORY_TILE_BASE/TILES_PER_LAG_HISTORY
  sta vram_copydsthi
  lda #0
  .repeat 3
    lsr vram_copydsthi
    ror a
  .endrepeat
  sta vram_copydstlo

  jsr clearLineImg
  lda #<~MT_DIRTY_LAG_HISTORY
  and mt_dirty
  sta mt_dirty

  ; Actually draw each entry
  lda #0
  entryloop:
    sta x_position
    ldy entry_to_draw
    cpy num_tests
    bcs no_more_entries
    inc entry_to_draw
    lda test_state,y
    jsr bcd8bit
    ora #'0'
    tay
    lda #5
    ora x_position
    tax
    tya
    jsr vwfPutTile
    lda bcd_highdigits
    beq less_than_ten
      ora #'0'
      ldx x_position
      jsr vwfPutTile
    less_than_ten:
    lda x_position
    clc
    adc #8 * TILES_PER_LAG_HISTORY
    bpl entryloop
  no_more_entries:

  lda #%0011
  jmp rf_color_lineImgBuf
  rts
.endproc

.proc draw_lag_history_so_far
tests_drawn = $0E

  lda #8/TILES_PER_LAG_HISTORY
  drawlagsloop:
    sta tests_drawn
    jsr mt_prepare_lag_history
    jsr rf_copy8tiles
    lda tests_drawn
    cmp #MAX_TESTS
    bcs drawlagsdone
    adc #8/TILES_PER_LAG_HISTORY
    cmp #MAX_TESTS
    bcc drawlagsloop
    lda #MAX_TESTS
    bcs drawlagsloop
  drawlagsdone:
  rts
.endproc

; Sprite drawing ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.proc draw_one_reticle
basex = 0
basey = 1
  stx basex
  sty basey
  ldx oam_used
  ldy #11
objloop:
  lda reticle_y,y
  clc
  adc basey
  sta OAM,x
  inx
  lda reticle_tile,y
  sta OAM,x
  inx
  lda reticle_attr,y
  sta OAM,x
  inx
  lda reticle_x,y
  clc
  adc basex
  sta OAM,x
  inx
  dey
  bpl objloop
  stx oam_used
  rts
.endproc

.proc mt_draw_cursor
  lda mt_cursor_y
  asl a
  asl a
  asl a
  adc #191
  ldx oam_used
  sta OAM,x
  inx
  lda #6
  sta OAM,x
  inx
  lda #0
  sta OAM,x
  inx
  lda #16
  sta OAM,x
  inx
  stx oam_used
  rts
.endproc

.include "nes.inc"
.include "global.inc"
.include "rectfill.inc"

.importzp helpsect_stopwatch

sw_hours = test_state+0
sw_minutes = test_state+1
sw_seconds = test_state+2
sw_frames = test_state+3
sw_framescolor = test_state+4

; bit 7: update
; bit 6: count
; 00: lap+stop, 40: lap+run; 80: stop; c0: run
sw_running = test_state+5

.segment "BANK01"
stopwatchgfx_bank = 1
stopwatch_bg:    .incbin "obj/nes/lag_clock_face.sb53"
fizzter_digits:  .incbin "obj/nes/fizzter_digits.chr.pb53"
stopwatch_balls: .incbin "obj/nes/lag_clock_balls.chr.pb53"
stopwatch_labels:
  rf_label  56, 40, 2, 0
  .byte "hours",0
  rf_label  96, 40, 2, 0
  .byte "minutes",0
  rf_label 136, 40, 2, 0
  .byte "seconds",0
  rf_label 176, 40, 2, 0
  .byte "frames",0
  .byte 0
stopwatch_ball_x:
  .byte 116,142,159,159,142,116, 90, 73, 73, 90
stopwatch_ball_y:
  .byte  87, 97,123,155,181,191,181,155,123, 97

COLON_DOT = $BC
LAP_INDICATOR_LEFT = $BD
LAP_INDICATOR_RIGHT = $BE
lapIndicatorAddr = lineImgBuf+96+24

.segment "CODE"
.proc do_stopwatch
  ldx #4
  lda #$80
  sta sw_running
  asl a
  :
    sta sw_hours,x
    dex
    bpl :-
restart:
  lda #VBLANK_NMI
  sta help_reload
  sta PPUCTRL
  ldx #$00
  stx PPUMASK
:
  lda #stopwatchgfx_bank
  sta :-+1
  
  ; Tiles $00-$7F (approximately): the clock face
  ldy #<stopwatch_bg
  lda #>stopwatch_bg
  jsr load_sb53_ay
  ; load sprite palette
  bit PPUDATA
  lda #$26
  sta PPUDATA
  lda #$16
  sta PPUDATA

  ; BG tiles $80-$BB: background digits (6 tiles each)
  ; $BC: colon dot
  lda #$08
  sta PPUADDR
  lda #$00
  sta PPUADDR
  ldy #<(fizzter_digits + 2)
  lda #>(fizzter_digits + 2)
  ldx fizzter_digits + 1
  jsr unpb53_ay_xtiles

  ; $C0-$CF we reserve for labels
  lda #$00
  sta rf_curpattable
  lda #$20
  sta rf_curnametable
  lda #$C0
  sta rf_tilenum
  lda #<stopwatch_labels
  sta ciSrc
  lda #>stopwatch_labels
  sta ciSrc+1
  jsr rf_draw_labels
  
  ; OBJ tiles $00-$59: sprite dot overlays (9 tiles each)
  ; $5A: pointer dot

  lda #$10
  sta PPUADDR
  lda #$00
  sta PPUADDR
  ldy #<(stopwatch_balls + 2)
  lda #>(stopwatch_balls + 2)
  ldx stopwatch_balls + 1
  jsr unpb53_ay_xtiles

  ; Uses sprites 0-9
  ldx #10
  jsr ppu_clear_oam
  
  ; Clear background buffer. After ppu_clear_oam, X=0
  txa
:
  sta lineImgBuf,x
  inx
  bpl :-
  jsr sw_prepare

loop:
  ; counting logic
  bit sw_running
  bvc not_counting
    ldy #0
    inc sw_frames
    lda sw_frames
    cmp #60
    bcc not_counting
    sty sw_frames
    inc sw_seconds
    lda sw_seconds
    cmp #60
    bcc not_counting
    sty sw_seconds
    inc sw_minutes
    lda sw_minutes
    cmp #60
    bcc not_counting
    sty sw_minutes
    inc sw_hours
    lda sw_hours
    cmp #100
    bcc not_counting
    sty sw_hours
  not_counting:

  bit sw_running
  bpl not_updating
    jsr sw_prepare
    jmp update_done
  not_updating:
    lda #LAP_INDICATOR_LEFT
    sta lapIndicatorAddr
    lda #LAP_INDICATOR_RIGHT
    sta lapIndicatorAddr+1
  update_done:
  
  lda nmis
:
  cmp nmis
  beq :-
  jsr rf_copy8tiles
  ldx #0
  stx OAMADDR
  lda #>OAM
  sta OAM_DMA

  ; Update the palette
  lda #$3F
  sta PPUADDR
  lda #$0D
  sta PPUADDR
  lda sw_framescolor
  sta PPUDATA
  eor #$30
  sta PPUDATA
  ldy #0
  lda #VBLANK_NMI|BG_0000|OBJ_1000
  sec
  jsr ppu_screen_on
  
  jsr read_pads
  lda new_keys+0
  and #KEY_START
  beq not_help
    lda #KEY_B|KEY_A|KEY_START|KEY_LEFT|KEY_RIGHT
    ldx #helpsect_stopwatch
    jsr helpscreen
    jmp restart
  not_help:
  
  lda new_keys+0
  and #KEY_A
  beq notStartStop
    lda #$40
    eor sw_running
    sta sw_running
  notStartStop:
  
  lda new_keys+0
  and #KEY_SELECT
  beq notLapReset
    ; If stopped and not lap, reset.  Otherwise toggle lap.
    lda sw_running
    cmp #$80
    bne pressedLap

    ; clear to 0
    asl a
    ldx #3
    :
      sta sw_hours,x
      dex
      bpl :-
    bmi notLapReset
  pressedLap:
    eor #$80
    sta sw_running
  notLapReset:

  lda new_keys+0
  and #KEY_B
  bne done
  jmp loop
done:
  rts
.endproc

.proc sw_prepare
  lda #$20
  sta vram_copydsthi
  lda #$C0
  sta vram_copydstlo

  lda sw_hours
  ldx #7
  jsr sw_draw_digits
  lda sw_minutes
  ldx #12
  jsr sw_draw_digits
  lda sw_seconds
  ldx #17
  jsr sw_draw_digits
  lda sw_frames
  ldx #22
  jsr sw_draw_digits

  ; blue or red? choose based on bit 0 of frames count
  lda sw_frames
  and #$01
  asl a
  asl a
  ora #$22
  sta sw_framescolor

  ; Erase lap indicator
  lda #0
  sta lapIndicatorAddr
  sta lapIndicatorAddr+1

  ; Move frames hand
  lda sw_frames
  jsr bcd8bit
objtiley = $00
objtilenum = $01
objtilex = $03

  tax
  sta objtilenum
  asl a
  asl a
  asl a
  adc objtilenum
  sta objtilenum
  lda stopwatch_ball_x,x
  sta objtilex
  lda stopwatch_ball_y,x
  sta objtiley
  ldx #$00
  framehandcolloop:
    clc
    lda objtiley
    sta OAM+0,x
    adc #8
    sta OAM+4,x
    adc #8
    sta OAM+8,x
    lda objtilenum
    inc objtilenum
    sta OAM+1,x
    adc #3
    sta OAM+5,x
    adc #3
    sta OAM+9,x
    lda #0
    sta OAM+2,x
    sta OAM+6,x
    sta OAM+10,x
    lda objtilex
    sta OAM+3,x
    sta OAM+7,x
    sta OAM+11,x
    adc #8
    sta objtilex
    txa
    adc #12
    tax
    cpx #36
    bcc framehandcolloop
  lda objtiley
  lsr a
  clc
  adc #78
  sta OAM+36
  lda #90
  sta OAM+37
  lda #0
  sta OAM+38
  lda objtilex
  lsr a
  clc
  adc #54
  sta OAM+39

  ; Finally draw colon dots
  lda #COLON_DOT
  ldx #32+11
  jsr onesetofcolondots
  ldx #64+11
onesetofcolondots:
  .repeat 3, I
    sta lineImgBuf+5*I,x
  .endrepeat
  rts  
.endproc


;;
; Draw digits of number in A (0-99) at horizontal position X
; in the nametable buffer.
.proc sw_draw_digits
  jsr bcd8bit
  pha
  lda bcd_highdigits
  jsr onedig
  pla
onedig:
  sta bcd_highdigits
  asl a
  adc bcd_highdigits
  asl a
  ora #$80
  jsr onedigcol
onedigcol:
  .repeat 3, I
    sta lineImgBuf+32*I,x
    adc #1
  .endrepeat
  inx
  rts
.endproc

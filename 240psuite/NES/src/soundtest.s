.include "nes.inc"
.include "global.inc"
.importzp helpsect_sound_test_frequency, helpsect_sound_test

tonefreqs_lo:
  .repeat 9, I
    .byte <((7 << I) - 1)
  .endrepeat
tonefreqs_hi:
  .repeat 9, I
    .byte >((7 << I) - 1)
  .endrepeat

.segment "CODE"

.proc do_sound_test
  ldx #helpsect_sound_test_frequency
  lda #KEY_A|KEY_B|KEY_START|KEY_UP|KEY_DOWN|KEY_LEFT|KEY_RIGHT
  jsr helpscreen
  lda new_keys+0
  and #KEY_A
  beq not_beep
    cpy #9
    bcc istribeep
    jsr pulse_beep
    jmp do_sound_test
  istribeep:
    jsr beep_octave_y
    jmp do_sound_test
  not_beep:
  lda new_keys+0
  and #KEY_START
  beq not_help
    ldx #helpsect_sound_test
    lda #KEY_A|KEY_B|KEY_START|KEY_LEFT|KEY_RIGHT
    jsr helpscreen
    jmp do_sound_test
  not_help:
  lda new_keys+0
  and #KEY_B
  bne done
  jmp do_sound_test
done:
  rts
.endproc

.proc beep_octave_y
  ; Demo of tone (for SMPTE bars and tone)
  ; frequency on NTSC NES is very nearly 1000 Hz
  lda #$88
  sta $4008

  ; TO DO: Y tables for PAL NES and Dendy
  lda tonefreqs_lo,y
  sta $400A
  lda tonefreqs_hi,y
  sta $400B
  ldx #20
delayloop:
  lda nmis
:
  cmp nmis
  beq :-
  lda #LIGHTGRAY|BG_ON|OBJ_ON|TINT_R
  sta PPUMASK
  dex
  bne delayloop
  lda #$00
  sta $4008
  lda #$FF
  sta $400B
  rts
.endproc

.proc pulse_beep
  ; Demo of tone (for SMPTE bars and tone)
  ; frequency on NTSC NES is very nearly 1000 Hz
  lda #$B8
  sta $4000
  lda #$08
  sta $4001
  lda tonefreqs_lo+4
  sta $4002
  lda tonefreqs_hi+4
  sta $4003
  ldx #20
delayloop:
  lda nmis
:
  cmp nmis
  beq :-
  lda #LIGHTGRAY|BG_ON|OBJ_ON|TINT_R
  sta PPUMASK
  dex
  bne delayloop
  lda #$B0
  sta $4000
  rts
.endproc

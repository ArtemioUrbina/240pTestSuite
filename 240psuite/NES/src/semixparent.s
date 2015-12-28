.include "nes.inc"
.include "global.inc"
.include "rectfill.inc"
.importzp helpsect_drop_shadow_sprite

.proc do_drop_shadow_sprite
restart:
:
  lda #<.bank(gus_portrait_sb53)
  sta :-+1
  lda #VBLANK_NMI
  sta PPUCTRL
  sta help_reload
  asl a
  sta PPUMASK
  
  ldy #<gus_portrait_sb53
  lda #>gus_portrait_sb53
  ldx #$00
  jsr load_sb53_ay

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
  and #KEY_START
  beq not_help
    ldx #helpsect_drop_shadow_sprite
    lda #KEY_A|KEY_B|KEY_START|KEY_LEFT|KEY_RIGHT
    jsr helpscreen
    jmp restart
  not_help:

  lda new_keys+0
  and #KEY_B
  bne done
  jmp loop
done:
  rts
.endproc

.segment "BANK02"
.byte "anal sex?"
gus_portrait_sb53:  .incbin "obj/nes/gus_portrait.sb53"


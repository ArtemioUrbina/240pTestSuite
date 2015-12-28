.include "nes.inc"
.include "global.inc"
.include "rectfill.inc"
.importzp helpsect_grid_scroll_test
GRID_SCROLL_TILE = $0D

.segment "CODE"
.proc do_grid_scroll
direction = test_state+0
speed     = test_state+1
running   = test_state+2
arelease  = test_state+3
xpos      = test_state+4
ypos      = test_state+5
  lda #KEY_RIGHT
  sta direction
  sta speed
  sta running
restart:
  jsr rf_load_tiles
  jsr rf_load_yrgb_palette
  lda #GRID_SCROLL_TILE
  ldy #$00
  sty arelease
  ldx #$20
  jsr ppu_clear_nt
  ldx #$2C
  jsr ppu_clear_nt
loop:

  lda running
  beq not_running
  lda direction
  sta 0

  lsr 0
  bcc not_running_right
    lda xpos
    clc
    adc speed
    sta xpos
    jmp not_running
  not_running_right:
  lsr 0
  bcc not_running_left
    lda xpos
    sbc speed
    sta xpos
    jmp not_running
  not_running_left:
  lsr 0
  bcc not_running_down
    lda ypos
    clc
    adc speed
    sta ypos
    jmp not_running
  not_running_down:
  lsr 0
  bcc not_running_up
    lda ypos
    sbc speed
    sta ypos
    jmp not_running
  not_running_up:
  not_running:

  lda nmis
:
  cmp nmis
  beq :-
  lda xpos
  and #$07
  tax
  lda ypos
  and #$07
  tay
  lda #VBLANK_NMI|BG_0000
  clc
  jsr ppu_screen_on
  
  jsr read_pads
  lda new_keys+0
  and #KEY_START
  beq not_help
    lda #KEY_B|KEY_A|KEY_START|KEY_LEFT|KEY_RIGHT
    ldx #helpsect_grid_scroll_test
    jsr helpscreen
    jmp restart
  not_help:

  lda new_keys+0
  and #KEY_A
  ora arelease
  sta arelease

  ; Release A: Toggle running
  lda cur_keys+0
  eor #$FF
  and arelease
  beq :+
    lda #1
    eor running
    sta running
    lda #0
    sta arelease
  :

  lda cur_keys+0
  and #KEY_A
  beq not_dir_change
    ; Hold A and press a direction to set the scroll direction
    lda new_keys+0
    and #KEY_UP|KEY_DOWN|KEY_LEFT|KEY_RIGHT
    beq dir_speed_change_done
    sta direction
    lda #0
    sta arelease
    beq dir_speed_change_done
  not_dir_change:
  lda new_keys+0
  and #KEY_UP
  beq notIncSpeed
    lda speed
    cmp #4
    bcs notIncSpeed
    inc speed
    bcc dir_speed_change_done
  notIncSpeed:
  lda new_keys+0
  and #KEY_DOWN
  beq notDecSpeed
    lda speed
    cmp #2
    bcc notDecSpeed
    dec speed
    bcc dir_speed_change_done
  notDecSpeed:

  dir_speed_change_done:
  
  lda new_keys+0
  and #KEY_B
  bne done
  jmp loop
done:
  rts

.endproc

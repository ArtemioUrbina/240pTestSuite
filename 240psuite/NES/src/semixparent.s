.include "nes.inc"
.include "global.inc"
.include "rectfill.inc"
.importzp helpsect_shadow_sprite

sprect_y = $00
sprect_tilenum = $01
sprect_attr = $02
sprect_x = $03
sprect_w = $04
sprect_h = $05
sprect_tileadd = $06

.proc do_drop_shadow_sprite
sprite_x = test_state + 0
sprite_y = test_state + 1
shadow_frame = test_state + 2  ; 0: even or horiz; 1: odd or vert; 2: diag
sprite_shape = test_state + 3  ; 0: shadow_reticle; 1: Hepsie
shadow_type = test_state + 4  ; 0: flicker; 1: striped
bgtype = test_state + 5
release_keys = test_state + 6
sprite_facing = test_state + 7
disappear_time = test_state + 8

  lda #112
  sta sprite_x
  sta sprite_y
  lda #0
  sta shadow_frame
  sta sprite_shape
  sta shadow_type
  sta bgtype
  sta sprite_facing
  sta disappear_time

restart:
  lda #VBLANK_NMI
  sta PPUCTRL
  asl a
  sta PPUMASK
  ldx #$10
  stx PPUADDR
  sta PPUADDR

  ; Load reticle graphics
:
  lda #<.bank(shadow_reticle_pb53)
  sta :-+1
  ldy #<(shadow_reticle_pb53+2)
  lda #>(shadow_reticle_pb53+2)
  ; ldx #16  ; already loaded from PPUADDR above
  jsr unpb53_with_shadows
  ldy #<(hepsie_pb53+2)
  lda #>(hepsie_pb53+2)
  ldx hepsie_pb53+1
  jsr unpb53_with_shadows

  ; Load names of shadow type/frame combos
  lda #<frame_names
  sta $00
  lda #>frame_names
  sta $01
  lda #$00
  sta vram_copydstlo
  lda #$1D
  sta vram_copydsthi
  loadnamesloop:
    jsr clearLineImg
    ldx #4
    jsr vwfPuts0
    sty $00
    sta $01
    lda #%0000110
    jsr rf_color_lineImgBuf
    jsr rf_copy8tiles
    lda vram_copydstlo
    clc
    adc #128
    bcc :+
      inc vram_copydsthi
    :
    sta vram_copydstlo
    ldy #0
    lda (0),y
    bne loadnamesloop

change_bg:
  lda #$FF
  sta OAM+0  ; hide sprite 0 in screens that don't use it
  lda bgtype
  jsr shadow_load_bg
  lda #$3F
  sta PPUADDR
  lda #$11
  sta PPUADDR
  ldx #0
  stx release_keys
:
  lda hepsie_palette,x
  sta PPUDATA
  inx
  cpx #hepsie_palette_len
  bcc :-
  
loop:
  ldx #4
  stx oam_used

  ; Draw Hepsie in front of shadow sprite if Hepsie is showing
  lda sprite_facing
  sta sprect_attr
  lda #0
  sta sprect_tilenum
  lda #4
  sta sprect_tileadd
  sta sprect_w
  lda sprite_y
  sta sprect_y
  lda sprite_x
  sta sprect_x

  lda sprite_shape
  beq not_hepsie_front
    lda #$40
    sta sprect_tilenum
    lda #3
    sta sprect_w
    lda #2
    sta sprect_h
    jsr draw_shadow_spriterect
    inc sprect_attr
    lda sprite_x
    sta sprect_x
    lda #2
    sta sprect_h
    jsr draw_shadow_spriterect
    inc sprect_attr

    lda sprite_y
    clc
    adc #8
    sta sprect_y
    lda sprite_x
    clc
    adc #8
    sta sprect_x
    lda #$40
    sta sprect_tilenum
  not_hepsie_front:

  ; Draw the shadow frame
  ldx shadow_type
  lda shadow_frame
  cmp max_shadow_frame_for_type,x
  bcc :+
    lda #0
    sta shadow_frame
  :

  lda nmis
  eor shadow_frame
  and #$01
  ora shadow_type
  beq no_shadow
    lda #4
    sta sprect_h
    lda shadow_type
    beq :+
      lda shadow_frame
      sec
      adc sprect_tilenum
      sta sprect_tilenum
    :
    jsr draw_shadow_spriterect
  no_shadow:
  
  ; If 
  lda disappear_time
  beq no_name
    dec disappear_time
    lda #1
    sta sprect_h
    lsr a
    sta sprect_attr
    lda #8
    sta sprect_w
    lda shadow_type
    asl a
    clc
    adc shadow_frame
    asl a
    asl a
    asl a
    adc #$D0
    sta sprect_tilenum
    lda #1
    sta sprect_tileadd
    lda #160
    sta sprect_x
    lda sprite_y
    cmp #112
    lda #240-32
    bcc :+
      lda #24
    :
    sta sprect_y
    jsr draw_shadow_spriterect
  no_name:

  ldx oam_used
  jsr ppu_clear_oam

  lda nmis
:
  cmp nmis
  beq :-
  ldx #0
  lda #>OAM
  stx OAMADDR
  sta OAM_DMA
  ldy #0
  lda #VBLANK_NMI|BG_0000|OBJ_1000
  sec
  jsr ppu_screen_on
  jsr read_pads

  ; For Green Hill Zone only, scroll the background to 4 times
  ; the sprite's displacement
  lda bgtype
  cmp #1
  bne :+
    lda sprite_x
    asl a
    sta hill_zone_xlo
    lda #0
    rol a
    asl hill_zone_xlo
    rol a
    sta hill_zone_xhi
    jsr hill_zone_do_raster
  :

  lda new_keys+0
  and #KEY_START
  beq not_help
    ldx #helpsect_shadow_sprite
    lda #KEY_A|KEY_B|KEY_START|KEY_LEFT|KEY_RIGHT
    jsr helpscreen
    jmp restart
  not_help:

  lda new_keys+0
  and #KEY_SELECT
  beq not_select
    lda #1
    eor sprite_shape
    sta sprite_shape
  not_select:

  lda cur_keys+0
  eor #$FF
  and release_keys
  bpl not_release_A
    ; A: change odd or even frames or stripe direction
    inc shadow_frame
    lda #120
    sta disappear_time
  not_release_A:

  lda release_keys
  ora new_keys+0
  and cur_keys+0
  sta release_keys

  lda cur_keys+0
  bpl not_holding_A
    lda new_keys+0
    lsr a
    bcc not_A_right
      ; A+Right: Next BG
      ldx bgtype
      inx
      cpx #4
      bcc have_new_bgtype
      ldx #0
      bcs have_new_bgtype
    not_A_right:
    lsr a
    bcc not_A_left
      ; A+Left: Previous BG
      ldx bgtype
      dex
      bpl have_new_bgtype
      ldx #3
    have_new_bgtype:
      stx bgtype
      jmp change_bg
    not_A_left:
    lsr a
    lsr a
    bcc not_A_up
      ; A+Up: Switch shadow type
      lda #$01
      eor shadow_type
      sta shadow_type
      lda #120
      sta disappear_time
    invalidate_A_release:
      lda #0
      sta release_keys
    not_A_up:
    jmp dpad_done
  not_holding_A:
    lda cur_keys+0
    and #KEY_RIGHT
    beq not_right
      lda sprite_facing
      and #<~$40
      sta sprite_facing
      lda sprite_x
      cmp #256-32
      bcs not_right
      inc sprite_x
    not_right:
    lda cur_keys+0
    and #KEY_LEFT
    beq not_left
      lda sprite_facing
      ora #$40
      sta sprite_facing
      lda sprite_x
      beq not_left
      dec sprite_x
    not_left:
    lda cur_keys+0
    and #KEY_DOWN
    beq not_down
      lda sprite_y
      cmp #240-32
      bcs not_down
      inc sprite_y
    not_down:
    lda cur_keys+0
    and #KEY_UP
    beq not_up
      lda sprite_y
      beq not_up
      dec sprite_y
    not_up:
  dpad_done:

  lda new_keys+0
  and #KEY_B
  bne done
  jmp loop
done:
  rts
.endproc

.proc shadow_load_bg
  asl a
  tax
  lda bgloadprocs+1,x
  pha
  lda bgloadprocs,x
  pha
  lda #VBLANK_NMI
  sta PPUCTRL
  sta help_reload
  asl a
  sta PPUMASK
  rts

.pushseg
.segment "RODATA"
bgloadprocs:
  .addr load_gus-1, hill_zone_load-1
  .addr load_fullscreenhorz-1, load_fullscreendiag-1
.popseg

load_gus:
:
  lda #<.bank(gus_portrait_sb53)
  sta :-+1
  ldy #<gus_portrait_sb53
  lda #>gus_portrait_sb53
  ldx #$00
  jmp load_sb53_ay

load_fullscreenhorz:
  lda #$00
  beq have_fullscreentilepat
load_fullscreendiag:
  lda #$55
have_fullscreentilepat:
  ldx #$00
  stx PPUADDR
  stx PPUADDR
  ldx #16
:
  sta PPUDATA
  eor #$FF
  dex
  bne :-

  lda #$3F
  sta PPUADDR
  lda #$00
  sta PPUADDR
  ldy #$0F
  sty PPUDATA
  sty PPUDATA
  sty PPUDATA
  ldx #$20
  stx PPUDATA
  tay
  jmp ppu_clear_nt
.endproc

.proc unpb53_with_shadows
  sty ciSrc
  sta ciSrc+1
tiles_left = 2

  stx tiles_left
  ldx #0
  stx ciBufStart
  ldx #16
  stx ciBufEnd
loop:
  jsr unpb53_some
  
  ldx #7
  ldy #1
shadowcalcloop:
  lda #0
  sta PB53_outbuf+24,x
  sta PB53_outbuf+40,x
  sta PB53_outbuf+56,x
  lda PB53_outbuf+0,x
  ora PB53_outbuf+8,x
  
  ; Compute vertical stripes
  pha
  and #$AA
  sta PB53_outbuf+32,x
  pla

  ; Compute horizontal stripes
  dey
  bne :+
    ldy #2
    lda #0
  :
  sta PB53_outbuf+16,x
  
  ; Compute diagonal stripes
  eor PB53_outbuf+32,x
  sta PB53_outbuf+48,x
  dex
  bpl shadowcalcloop

  ; Now copy the tiles to VRAM
  ldx #0
copyloop:
  lda PB53_outbuf,x
  sta $2007
  inx
  cpx #64
  bcc copyloop
  dec tiles_left
  bne loop
  rts
.endproc


.proc draw_shadow_spriterect
curx = $07
xadd = $08

  lda #8
  bit sprect_attr
  bvc :+
    lda sprect_w
    asl a
    asl a
    asl a
    adc sprect_x
    sec
    sbc #8
    sta sprect_x
    lda #<-8
  :
  sta xadd
  ldx oam_used
rowloop:
  lda sprect_x
  sta curx
  ldy sprect_w
tileloop:
  lda sprect_y
  sta OAM,x
  inx
  lda sprect_tilenum
  sta OAM,x
  inx
  clc
  adc sprect_tileadd
  sta sprect_tilenum
  lda sprect_attr
  sta OAM,x
  inx
  lda curx
  sta OAM,x
  inx
  clc
  adc xadd
  sta curx
  dey
  bne tileloop
  lda sprect_y
  clc
  adc #8
  sta sprect_y
  dec sprect_h
  bne rowloop
  stx oam_used
  rts
.endproc

.segment "RODATA"
max_shadow_frame_for_type:  .byte 2, 3

.segment "RODATA"
gus_portrait_sb53:  .incbin "obj/nes/gus_portrait.sb53"

.segment "BANK01"
shadow_reticle_pb53:.incbin "obj/nes/shadow_reticle.chr.pb53"
hepsie_pb53:        .incbin "obj/nes/hepsie.chr.pb53"
hepsie_palette:
  .byte $0F,$1A,$27,$FF,$0F,$14,$27,$FF,$0F,$0F,$0F
hepsie_palette_len = * - hepsie_palette
frame_names:
  .byte "odd frames",10
  .byte "even frames",10
  .byte "horiz. stripes",10
  .byte "vert. stripes",10
  .byte "diag. stripes",0

;
; Dispatch for 240p test suite
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
.importzp helpsect_240p_test_suite_menu, helpsect_about, helpsect_credits

OAM = $0200

.segment "ZEROPAGE"
nmis:          .res 1
oam_used:      .res 1  ; starts at 0
cur_keys:      .res 2
new_keys:      .res 2
das_keys:      .res 2
das_timer:     .res 2
tvSystem:      .res 1

.segment "CODE"
;;
; This NMI handler is good enough for a simple "has NMI occurred?"
; vblank-detect loop.  But sometimes there are things that you always
; want to happen every frame, even if the game logic takes far longer
; than usual.  These might include music or a scroll split.  In these
; cases, you'll need to put more logic into the NMI handler.
.proc nmi_handler
  inc nmis
  rti
.endproc

; A null IRQ handler that just does RTI is useful to add breakpoints
; that survive a recompile.  Set your debugging emulator to trap on
; reads of $FFFE, and then you can BRK $00 whenever you need to add
; a breakpoint.
;
; But sometimes you'll want a non-null IRQ handler.
; On NROM, the IRQ handler is mostly used for the DMC IRQ, which was
; designed for gapless playback of sampled sounds but can also be
; (ab)used as a crude timer for a scroll split (e.g. status bar).
.proc irq_handler
  rti
.endproc

.proc main
  lda #VBLANK_NMI
  sta PPUCTRL
  sta help_reload
  lda #$0F
  sta SNDCHN
  jsr getTVSystem
  sta tvSystem
  jsr do_credits
;  jsr do_manual_lag

forever:
  ldx #helpsect_240p_test_suite_menu
  lda #KEY_A|KEY_START|KEY_UP|KEY_DOWN|KEY_LEFT|KEY_RIGHT
  jsr helpscreen
  
  ; At this point, page A line Y
  cmp #1
  tya
  bcc :+
    adc #11-1
  :
  jsr main_dispatch
  jmp forever
.endproc

.proc main_dispatch
  asl a
  tax
  lda routines+1,x
  pha
  lda routines,x
  pha
  rts  
.pushseg
.segment "RODATA"
routines:
  .addr do_pluge-1
  .addr do_gcbars-1
  .addr do_smpte-1
  .addr do_601bars-1
  .addr do_color_bleed-1
  .addr do_cpsgrid-1
  .addr do_linearity-1
  .addr do_gray_ramp-1
  .addr do_solid_color-1
  .addr do_ire-1
  .addr do_sharpness-1

  .addr do_overscan-1
  .addr beep-1
  .addr beep-1
  .addr do_stopwatch-1
  .addr do_manual_lag-1
  .addr beep-1
  .addr do_grid_scroll-1
  .addr do_full_stripes-1
  .addr do_backlight-1
  .addr do_sound_test-1
  .addr do_about-1
  .addr do_credits-1
.popseg
.endproc

.proc beep
  ; Demo of tone (for SMPTE bars and tone)
  ; frequency on NTSC NES is very nearly 1000 Hz
  lda #$88
  sta $4008
  lda #55  ; FIXME: change this for PAL NES and Dendy
  sta $400A
  lda #0
  sta $400B
  ldx #20
delayloop:
  lda nmis
:
  cmp nmis
  beq :-
  dex
  bne delayloop
  lda #$00
  sta $4008
  sta $400B
  rts
.endproc

.proc do_credits
  ldx #helpsect_credits
  lda #KEY_LEFT|KEY_RIGHT|KEY_B|KEY_A|KEY_START
  jmp helpscreen
.endproc

.proc do_about
  ldx #helpsect_about
  lda #KEY_LEFT|KEY_RIGHT|KEY_B|KEY_A|KEY_START
  jmp helpscreen
.endproc

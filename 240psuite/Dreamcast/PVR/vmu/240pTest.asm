;******************************************************************************
; Main ASM file
;******************************************************************************
; Controller test for 240p test Suite using the Perspective drawing library.
; Artemio Urbina for the 240p Test Suite 2022
;
; Uses libperspective by Kresna Susila for drawing: 
;           https://slum.online/dreamcast/
;
; Assemble with waterbear by wtetzner:
;           https://github.com/wtetzner/waterbear
;******************************************************************************
        .include        "./lib/sfr.i"   ; VMU Special Function Register Mapping

;******************************************************************************
; Global Constants & External Libraries
;******************************************************************************
; Values to use during the program for areas of the screen to toggle
button_up_pos       equ 25
button_down_pos     equ 121
button_left_pos     equ 72
button_right_pos    equ 74
button_a_pos        equ 99
button_b_pos        equ 101
button_mode_pos     equ 53
button_sleep_pos    equ 51

bit_up_pos          equ 0
bit_down_pos        equ 1
bit_left_pos        equ 2
bit_right_pos       equ 3
bit_a_pos           equ 4
bit_b_pos           equ 5
bit_mode_pos        equ 6
bit_sleep_pos       equ 7

;Timeout in cycles for exit in Control Test since we disabled sleep, 30 ~1sec
Hold_B_Timeout      equ $30

;******************************************************************************
; Variables
;******************************************************************************
; Global variables
p3_pressed              =       $4                    ; 1 byte
p3_last_input           =       $5                    ; 1 byte
toggle_location         =       $6                    ; 1 byte
b_held                  =       $7                    ; 1 byte
highlights              =       $8                    ; 1 byte

; Title screen  variables
title_anim_counter      =       $9                    ; 1 byte
title_address           =       $10                   ; 2 bytes
title_frame_counter     =       $12                   ; 1 byte

; Timeout for auto-sleep
auto_sleep_timeout      =       $13                   ; 1 byte
auto_sleep_timeout_sub  =       $14                   ; 1 byte

;------------------------------------------------------------------------------
;******************************************************************************
; Interrupt Vectors
;******************************************************************************
        .org    $00             ; Main entry point
        jmpf    Start

        .org    $03
        reti

        .org    $0b
        reti

        .org    $13             ; Timer0L
        reti

        .org    $1b
        jmpf    t1int           ; Initialise timer 1

        .org    $23             ; Timer0H
        reti

        .org    $2b
        reti

        .org    $33
        reti

        .org    $3b
        reti

        .org    $43
        reti

        .org    $4b             ; Clear Port 3 Interrupts
        clr1    p3int, 0
        clr1    p3int, 1
        reti

        .org    $130
t1int:
        push    ie
        clr1    ie,7
        not1    ext,0
        jmpf    t1int
        pop     ie
        reti

        .org    $1f0
goodbye:
        not1    ext,0
        jmpf    goodbye         ; leave game mode



;******************************************************************************
; Game Header
;******************************************************************************
        .org   $200
        .text 16 "VMU Control Test"
        .text 32 "240p Test Suite by Artemio      "
        .string 16 "240p Test Suite"

;******************************************************************************
; Icon Header
;******************************************************************************
        .org    $240
        .include icon "img/VMUTest.gif" speed=10

;******************************************************************************
; Main Entry Point
;******************************************************************************
        .org    $680
Start:
        clr1    ie,7

        mov     #$a1,ocr
        mov     #$09,mcr
        mov     #$80,vccr
        clr1    p3int,0
        clr1    p1,7
        mov     #$ff,p3

        set1    ie,7

        ;----------------------------------------------------------------------
        ; Our Main Loop
        ;----------------------------------------------------------------------        
Main_Loop:

        ; Initialise the p3_last_input
        mov     #%11111111, p3_last_input

        ; Initialise some variables
        mov     #26, title_address
        mov     #0, title_anim_counter
        mov     #0, title_frame_counter
        call    Reset_auto_sleep

.title_Loop
        clr1    ocr, 5
        mov     #<title_LUT, trl
        mov     #>title_LUT, trh
        ld      title_anim_counter
        rol
        ldc
        st      title_address
        ld      title_anim_counter
        rol
        inc     acc
        ldc
        st      title_address+1

        ; Draw the frame
        P_Draw_Background title_address
        P_Blit_Screen

        inc     title_frame_counter
        ld      title_frame_counter
        sub     #10
        bnz     .poll_loop
        mov     #0, title_frame_counter

        ; Check to see if we need to reset the frame
        inc     title_anim_counter
        ld      title_anim_counter
        sub     #26
        bnz     .poll_loop
        mov     #0, title_anim_counter
.poll_loop:
        set1    ocr, 5

        call    Check_auto_sleep
        ; If we are plugged into a Dreamcast, abort
        call    Check_DC_plugged
        ; If battery is low, abort
        call    Check_low_Battery

        ; Poll input
        call    Get_Input
        
        mov     #Button_A, acc
        call    Check_Button_Pressed
        bz      .title_Loop
        call    Controller_Test
        jmp     Main_Loop


%macro  Check_Input %bit_pos, %toggle_pos, %next
        ;----------------------------------------------------------------------
        ; Checks the bit mask for a button and toggles if it is held
        ;----------------------------------------------------------------------
        ; Accepts:      bit mask, y*6+x toggle pos, next label to jump to
        ; Destroys:     ACC
        ;
        ;----------------------------------------------------------------------

        ld      p3                                ; loads controls
        bp      acc, %bit_pos, .not_%             ; checks if button pressed
        bp      highlights, %bit_pos, %next       ; if pressed check if drawn
        set1    highlights, %bit_pos              ; if not, set the flag  
        jmp     .toggle_%                         ; jump to toggle it
.not_%:
        bn      highlights, %bit_pos, %next       ; button was not pressed
        clr1    highlights, %bit_pos              ; check if drawn pressed
.toggle_%:
        mov     #%toggle_pos, acc                 ; prepare the position
        call    P_Invert_block                    ; toggle it
%end

Controller_Test:
        ;----------------------------------------------------------------------
        ; Our Main Controller Test
        ;----------------------------------------------------------------------

        mov     #%00000000, highlights
        mov     #Hold_B_Timeout, b_held
        call    Reset_auto_sleep

        ; Draw the background
        P_Draw_Background_Constant controller_back

.control_loop:
        P_Blit_Screen

        ; If we are plugged into a Dreamcast, abort
        call    Check_DC_plugged
        ; If battery is low, abort
        call    Check_low_Battery
        ; Check if no input has been received for a while and sleep
        call    Check_auto_sleep

.check_a:
        Check_Input bit_a_pos, button_a_pos, .check_b

.check_b:
        Check_Input bit_b_pos, button_b_pos, .check_up

.check_up:
        Check_Input bit_up_pos, button_up_pos, .check_down

.check_down:
        Check_Input bit_down_pos, button_down_pos, .check_left

.check_left:
        Check_Input bit_left_pos, button_left_pos, .check_right

.check_right:
        Check_Input bit_right_pos, button_right_pos, .check_mode

.check_mode:
        Check_Input bit_mode_pos, button_mode_pos, .check_sleep

.check_sleep:
        Check_Input bit_sleep_pos, button_sleep_pos, .check_b_held

.check_b_held;
        ; Check if B has been held to exit
        ld      p3
        bp      acc, bit_b_pos, .reset_b
        dbnz    b_held, .loop_back
        ret
.reset_b
        mov     #Hold_B_Timeout, b_held
.loop_back:
        jmp     .control_loop

P_Invert_block:
        ;----------------------------------------------------------------------
        ; Invert a block
        ;----------------------------------------------------------------------
        ; Accepts:      ACC (postion  (y * 6) + x)
        ; Destroys:     ACC, C, VRMAD1, VRMAD2, VSEL
        ;
        ; Inverts the screen colours
        ;----------------------------------------------------------------------

        st      toggle_location
        clr1    ocr, 5
        ; Prepare the frame buffer address
        mov     #P_WRAM_BANK, vrmad2
        mov     #P_WRAM_ADDR, vrmad1
        mov     #%00000000, vsel

        mov     #2, c

        ld      vrmad1
        add     toggle_location
        st      vrmad1

.loop
        ld      vtrbf
        xor     #%01111110
        st      vtrbf
        ld      vrmad1
        add     #6
        st      vrmad1
        ld      vtrbf
        xor     #%01111110
        st      vtrbf
        ld      vrmad1
        add     #6
        st      vrmad1
        ld      vtrbf
        xor     #%01111110
        st      vtrbf
        ld      vrmad1
        add     #6
        st      vrmad1
        ld      vtrbf
        xor     #%01111110
        st      vtrbf
        ld      vrmad1
        add     #6
        st      vrmad1
        dbnz    c, .loop
        set1    ocr, 5
        call    Reset_auto_sleep
        ret


Reset_auto_sleep:
        mov     #0, auto_sleep_timeout
        mov     #0, auto_sleep_timeout_sub
        ret
        
        ;----------------------------------------------------------------------
        ; Auto sleeps in about X seconds of innactivity
        ;   title screen:     about 35 seconds
        ;   controller test:  about 26 seconds
        ;----------------------------------------------------------------------

Check_auto_sleep:
        inc     auto_sleep_timeout
        ld      auto_sleep_timeout
        sub     #$60
        bnz     .return_autosleep

        mov     #0, auto_sleep_timeout
        inc     auto_sleep_timeout_sub
        ld      auto_sleep_timeout_sub
        sub     #$10
        bnz     .return_autosleep

        ; Code from libkcommon
.sleep:
        mov     #0, t1cnt               ; Disable audio
        set1    pcon, 0                 ; Activate HALT mode (saves power)
        bn      p3, 7, .sleep           ; wait until Sleep Key is released
        mov     #0,vccr                 ; turn off LCD
.sleepmore:
        set1    pcon,0                  ; activate HALT mode (saves power)
        bp      p7, 0, quit             ; Docked?
        bp      p3, 7, .sleepmore       ; no Sleep Key pressed yet
        mov     #$80, vccr              ; turn on LCD again
.waitsleepup:
        set1    pcon,0                  ; activate HALT modus (saves power)
        bn      p3,7,.waitsleepup
        call    Reset_auto_sleep        ; Resets auto sleep timers after waking up
.return_autosleep:
        ret
quit:
        jmpf    goodbye

        ;----------------------------------------------------------------------
        ; Checks if connected to the Dreamcast mid test
        ;----------------------------------------------------------------------
Check_DC_plugged:
        bp      p7, 0, quit              ; Quit, if Dreamcast Connection  
        ret

        ;----------------------------------------------------------------------
        ; Checks if battery is low
        ;----------------------------------------------------------------------
Check_low_Battery:
        bn      p7, 1, quit              ; Quit if battery is low
        ret

        .include        "./lib/libperspective.asm"
        .include        "./lib/libkcommon.asm"       
        .include        "./img/controller_back.asm"
        .include        "./img/title.asm"

;******************************************************************************
; End of File
;******************************************************************************
        .cnop   0,$200          ; pad to an even number of blocks

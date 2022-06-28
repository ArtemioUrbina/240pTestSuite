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

;Timeout in cycles for exit in Control Test since we disabled sleep, 30 ~1sec
Hold_B_TimeSND_on   equ $2
Hold_B_TimeSND_off  equ $ff

;******************************************************************************
; Variables
;******************************************************************************
; Global variables
p3_pressed              =       $4                    ; 1 byte
p3_last_input           =       $5                    ; 1 byte
toggle_location         =       $6                    ; 1 byte
b_held                  =       $7                    ; 1 byte
highlights              =       $8                    ; 1 byte

; Title screen variables
title_anim_counter      =       $9                    ; 1 byte
title_address           =       $A                    ; 2 bytes
title_frame_counter     =       $C                    ; 1 byte

; Timeout for auto-sleep
auto_sleep_timeout      =       $D                    ; 1 byte
auto_sleep_timeout_sub  =       $E                    ; 1 byte

; Sleep screen variables
sleep_anim_counter      =       $F                    ; 1 byte
sleep_address           =       $10                   ; 2 bytes
sleep_frame_counter     =       $12                   ; 1 byte
refresh_screen          =       $13                   ; 1 byte

; Sound variables
sound_start_lr          =       $14                   ; 1 byte
sound_start_lc          =       $15                   ; 1 byte
sound_toggle_lc         =       $16                   ; 1 byte
sound_running           =       $17                   ; 1 byte
sound_sync_frames       =       $18                   ; 1 byte

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
        .text   16 "VMU Ctr/Snd Test"
        .text   32 "240p Test Suite by Artemio      "
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

        mov     #%10100001,ocr
        mov     #%00001001,mcr
        mov     #%10000000,vccr
        clr1    p3int,0
        clr1    p1,7
        mov     #%11111111,p3

        set1    ie,7

;******************************************************************************
; Our Main Loop
;******************************************************************************
Main_Loop:

        ; Initialise the p3_last_input
        mov     #%11111111, p3_last_input

        ; Initialise some variables
        mov     #26, title_address
        mov     #0, title_anim_counter
        mov     #0, title_frame_counter
        mov     #0, refresh_screen
        call    Reset_auto_sleep

.title_Loop
        ld      title_frame_counter          ; check if we really need refresh
        bnz     .skip_refresh

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

        set1    ocr, 5

.skip_refresh:
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
        call    Check_auto_sleep
        ; If we are plugged into a Dreamcast, abort
        call    Check_DC_plugged
        ; If battery is low, abort
        call    Check_low_Battery

        ; Poll input
        call    Get_Input
        
        mov     #Button_Right, acc
        call    Check_Button_Pressed
        bnz     .call_control_test
        mov     #Button_Left, acc
        call    Check_Button_Pressed
        bz      .title_loop
        call    Frequency_Sweep
        jmp     Main_Loop
.call_control_test:
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

;******************************************************************************
; Our Main Controller Test
;******************************************************************************
Controller_Test:
        mov     #%00000000, highlights
        mov     #Hold_B_Timeout, b_held
        mov     #$ff, toggle_location
        mov     #0, refresh_screen
        call    Reset_auto_sleep

        ; Draw the background
        P_Draw_Background_Constant controller_back

.control_loop:
        ld      toggle_location
        bz      .skip_refresh
        P_Blit_Screen

.skip_refresh:
        mov     #0, toggle_location         ; Check if toggle happened

        ; If we are plugged into a Dreamcast, abort
        call    Check_DC_plugged

        ; If battery is low, abort
        call    Check_low_Battery

        ; Check if no input has been received for a while and sleep
        call    Check_auto_sleep
        ld      refresh_screen
        bnz     Controller_Test

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

.check_b_held:
        ; Check if B has been held to exit
        ld      p3
        bp      acc, bit_b_pos, .reset_b
        dbnz    b_held, .loop_back
        ret
.reset_b:
        mov     #Hold_B_Timeout, b_held
.loop_back:
        jmp     .control_loop

P_Invert_block:
        ;----------------------------------------------------------------------
        ; Invert a block of pixels for Controller Test
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

;******************************************************************************
; Our Frequency Sweep Test
;******************************************************************************
Frequency_sweep:
        ; Initialise some variables
        mov     #Hold_B_TimeSND_off, b_held
        mov     #0, refresh_screen
        call    Reset_auto_sleep
        call    sndinit

        ; Draw the background
        P_Draw_Background_Constant sound_back
        P_Blit_Screen

.sweep_loop:
        ; If we are plugged into a Dreamcast, abort
        call    Check_DC_plugged

        ; If battery is low, abort
        call    Check_low_Battery

        ; Check if no input has been received for a while and sleep
        ld      sound_running
        bp      acc, 0, .check_frq_advance  ; playing back, no auto-sleep/input
        call    Check_auto_sleep
        ld      refresh_screen
        bnz     Frequency_sweep             ; reset test if auto-sleep happened

        ; Poll input
        call    Get_Input
        
        ; Check if we start the sequence
        mov     #Button_A, acc
        call    Check_Button_Pressed
        bz      .check_b_held

        ; start playback
        mov     #Hold_B_TimeSND_on, b_held     ; load Interrupt time out
        set1    PCON,0                  ; wait for the base interrupt 0.5 secs
        call    sndstart
        jmp     .wait_interrupt

.check_frq_advance:
        ld      sound_running
        bn      acc, 0, .check_b_held   ; not running, ignore and continue

        call    sndadvfreq              ; change frequency
.wait_interrupt:
        set1    PCON,0                  ; wait for the base interrupt 0.5 secs

.check_b_held:
        ; Check if B has been held to exit
        ld      p3
        bp      acc, bit_b_pos, .reset_b
        dbnz    b_held, .check_frq_advance
        call    sndstop
        ret

.reset_b:
        ld      sound_running
        bp      acc, 0, .reset_while_playing
        mov     #Hold_B_TimeSND_off, b_held
        jmp     .sweep_loop

.reset_while_playing
        mov     #Hold_B_TimeSND_on, b_held
        jmp     .sweep_loop
        
        ;----------------------------------------------------------------------
        ; Sound Initialization for Playback
        ;----------------------------------------------------------------------
sndinit:
        clr1    P1, 7
        clr1    sound_running, 0
        ret

        ;----------------------------------------------------------------------
        ; Starts the sequence and sets the flags
        ;----------------------------------------------------------------------
sndstart:
        set1    sound_running, 0

        ; start the cycle and open with sync tones and silence
        call    sync_tones
        call    wait_silence
                
        mov     #$e0,sound_start_lr     ; $e0  (lowest is $00)
        mov     #$f0,sound_start_lc     ; $f0  (lowest is $80)
        mov     #1, sound_toggle_lc

        ld      sound_start_lr
        st      T1LR
        ld      sound_start_lc
        st      T1LC

        mov     #$D0, T1CNT    ; Set ELDT1C, T1LRUN and T1HRUN to “1”.
        ret

        ;----------------------------------------------------------------------
        ; Advances to next frequency, and resets counters if finished
        ;----------------------------------------------------------------------
sndadvfreq:
        ; do the sweep for lr
        ld      sound_start_lr
        inc     acc
        st      sound_start_lr
        sub     #$ff                        ; e0 to fe, so we finish at ff
        bnz     .continue_t1lc

        ; stop the cycle and close with silence and sync tones
        call    wait_silence
        call    sync_tones

        call    sndstop
        ret
        
.continue_t1lc:
        ; check if we run the same process for lc
        ld      sound_toggle_lc
        sub     #2
        bz      .reset_and_increment_lc
        ld      sound_toggle_lc
        inc     acc
        st      sound_toggle_lc
        jmp     .load_regs

.reset_and_increment_lc:
        mov     #1, sound_toggle_lc  
    
        ; run the same process for lc
        ld      sound_start_lc
        inc     acc
        st      sound_start_lc

.load_regs
        ; now load both to the timer registers
        ld      sound_start_lr
        st      T1LR
        ld      sound_start_lc
        st      T1LC
        ret

sync_tones:
        mov     #10, sound_sync_frames
        mov     #$fe,T1LR               ; Highest note that runs on a real VMU
        mov     #$ff,T1LC               ; about 2730hz

.loop_sync:
        ; stop playback
        mov     #$D0, T1CNT             ; Set ELDT1C, T1LRUN and T1HRUN to “1”.
        set1    PCON,0                  ; wait for the base interrupt 0.5 secs

        mov     #0, T1CNT
        set1    PCON,0                  ; wait for the base interrupt 0.5 secs

        dbnz    sound_sync_frames, .loop_sync
        ret

wait_silence:
        mov     #4, sound_sync_frames
        ; stop playback
        mov     #0, T1CNT               ; Set ELDT1C, T1LRUN and T1HRUN to “1”.
.loop_silence:
        set1    PCON,0                  ; wait for the base interrupt 0.5 secs
        dbnz    sound_sync_frames, .loop_silence
        ret

sndstop:
        mov     #0, T1CNT
        clr1    sound_running, 0
        ret

;******************************************************************************
; Auto-sleep with timout to save battery and add cat animation by ferigne
;******************************************************************************
        ;----------------------------------------------------------------------
        ; restarts auto-sleep counters
        ;----------------------------------------------------------------------
Reset_auto_sleep:
        mov     #0, auto_sleep_timeout
        mov     #0, auto_sleep_timeout_sub
        ret
        
        ;----------------------------------------------------------------------
        ; Auto sleeps in about 20 seconds of innactivity
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
        call    sleep_animate
.return_autosleep:
        ret

        ;----------------------------------------------------------------------
        ; Internal Check for auto-sleep
        ;----------------------------------------------------------------------
Check_auto_sleep_int:
        inc     auto_sleep_timeout
        ld      auto_sleep_timeout
        sub     #$30
        bnz     .return_autosleep_int

        mov     #0, auto_sleep_timeout
        inc     auto_sleep_timeout_sub
        ld      auto_sleep_timeout_sub
        sub     #$10
        bnz     .return_autosleep_int
        call    execute_sleep
        mov     #1, refresh_screen
.return_autosleep_int:
        ret

        ;----------------------------------------------------------------------
        ; Pre-sleep animation
        ;----------------------------------------------------------------------
sleep_animate:
        ; Initialise some variables
        call    Reset_auto_sleep
        mov     #0, sleep_address
        mov     #0, sleep_anim_counter
        mov     #0, sleep_frame_counter
        mov     #0, refresh_screen

.sleep_loop
        ld      sleep_frame_counter          ; check if we really need refresh
        bnz     .skip_refresh

        clr1    ocr, 5
        mov     #<sleep_LUT, trl
        mov     #>sleep_LUT, trh
        ld      sleep_anim_counter
        rol
        ldc
        st      sleep_address
        ld      sleep_anim_counter
        rol
        inc     acc
        ldc
        st      sleep_address+1

        ; Draw the frame
        P_Draw_Background sleep_address
        P_Blit_Screen

        set1    ocr, 5

.skip_refresh:
        inc     sleep_frame_counter
        ld      sleep_frame_counter
        sub     #25
        bnz     .poll_loop
        mov     #0, sleep_frame_counter

        ; Check to see if we need to reset the frame
        inc     sleep_anim_counter
        ld      sleep_anim_counter
        sub     #12
        bnz     .poll_loop
        mov     #0, sleep_anim_counter

.poll_loop:
        call    Check_auto_sleep_int
        ld      refresh_screen
        bz      .not_from_wakeup
        call    Reset_auto_sleep
        ret

.not_from_wakeup:
        ; If we are plugged into a Dreamcast, abort
        call    Check_DC_plugged
        ; If battery is low, abort
        call    Check_low_Battery

        ; Poll input
        ld      p3
        xor     #%11111111     
        bz      .sleep_loop
        call    Reset_auto_sleep
        mov     #1, refresh_screen
        ret

execute_sleep:
        P_Fill_Screen P_WHITE
        P_Blit_Screen

        ; Code from libkcommon
.sleep_wait:
        call    sndstop                 ; Disable audio
        set1    pcon, 0                 ; Activate HALT mode (saves power)
        bn      p3, 7, .sleep_wait      ; wait until Sleep Key is released
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

;******************************************************************************
; Includes: Graphics and libraries
;******************************************************************************

        .include        "./lib/libperspective.asm"
        .include        "./lib/libkcommon.asm"       
        .include        "./img/controller_back.asm"
        .include        "./img/sound_back.asm"
        .include        "./img/title.asm"
        .include        "./img/sleep.asm"

;******************************************************************************
; End of File
;******************************************************************************
        .cnop   0,$200          ; pad to an even number of blocks

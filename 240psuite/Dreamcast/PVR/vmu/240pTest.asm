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
Button_Up_pos       equ 25
Button_Down_pos     equ 121
Button_Left_pos     equ 72
Button_Right_pos    equ 74
Button_A_pos        equ 99
Button_B_pos        equ 101
Button_Sleep_pos    equ 51
Button_Mode_pos     equ 53

;Timeout in cycles for exit in Control Test since we disabled sleep
Hold_B_Timeout      equ $60

;******************************************************************************
; Variables
;******************************************************************************
; Global variables
p3_pressed              =       $4                    ; 1 byte
p3_last_input           =       $5                    ; 1 byte
toggle_location         =       $00                   ; 1 byte
b_held                  =       Hold_B_Timeout        ; 1 byte


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
        clr1 ie,7
        mov #$a1,ocr
        mov #$09,mcr
        mov #$80,vccr
        clr1 p3int,0
        clr1 p1,7
        mov #$ff,p3

        set1 ie,7

        ; Initialise the p3_last_input
        mov     #%11111111, p3_last_input

Main_Loop:
        call    Controller_Test
        jmpf    Main_Loop

Controller_Test:
        ;----------------------------------------------------------------------
        ; Our Main Controller Test
        ;----------------------------------------------------------------------
        
        ; Draw the background
        P_Draw_Background_Constant controller_back

.control_loop:
        P_Blit_Screen

        set1    ocr, 5

        ; If we are plugged into a Dreamcast, abort
        call    Check_DC_plugged
        ; Poll input
        call    Get_Input_Test

        mov     #Button_A, acc
        call    Check_Button_Pressed
        bz      .check_b
        mov     #Button_A_pos, acc
        jmp     .do_toggle

.check_b:
        mov     #Button_B, acc
        call    Check_Button_Pressed
        bz      .check_up
        mov     #Button_B_pos, acc
        jmp     .do_toggle

.check_up:
        mov     #Button_Up, acc
        call    Check_Button_Pressed
        bz      .check_down
        mov     #Button_Up_pos, acc
        jmp     .do_toggle

.check_down:
        mov     #Button_Down, acc
        call    Check_Button_Pressed
        bz      .check_left
        mov     #Button_Down_pos, acc
        jmp     .do_toggle

.check_left:
        mov     #Button_Left, acc
        call    Check_Button_Pressed
        bz      .check_right
        mov     #Button_Left_pos, acc
        jmp     .do_toggle

.check_right:
        mov     #Button_Right, acc
        call    Check_Button_Pressed
        bz      .check_sleep
        mov     #Button_Right_pos, acc
        jmp     .do_toggle

.check_sleep:
        mov     #Button_Sleep, acc
        call    Check_Button_Pressed
        bz      .check_mode
        mov     #Button_Sleep_pos, acc
        jmp     .do_toggle

.check_mode:
        mov     #Button_Mode, acc
        call    Check_Button_Pressed
        bz      .jump_back
        mov     #Button_Mode_pos, acc
        jmp     .do_toggle

.do_toggle:
        st      toggle_location     
        call    P_Invert_block

.jump_back
        ld      p3
        and     #Button_B
        bnz     .reset_b
        dbnz    b_held, .control_loop
        jmpf    goodbye

.reset_b
        mov     #Hold_B_Timeout, b_held
        jmp     .control_loop

P_Invert_block:
        ;----------------------------------------------------------------------
        ; Invert a block
        ;----------------------------------------------------------------------
        ; Accepts:      toggle_location (postion  (y * 6) + x)
        ; Destroys:     ACC, C, VRMAD1, VRMAD2, VSEL
        ;
        ; Inverts the screen colours
        ;----------------------------------------------------------------------
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
        ret

Get_Input_Test:
        ;----------------------------------------------------------------------
        ; Get input from P3, compare input to previous P3 input to get buttons
        ; pressed this cycle, doesn't check for sleep or mode
        ;----------------------------------------------------------------------
        ld      p3_last_input
        st      c
        ld      p3
        st      p3_last_input
        xor     #%11111111
        and     c
        xor     #%11111111
        st      p3_pressed
        ret

Check_DC_plugged:
        ;----------------------------------------------------------------------
        ; Checks if connected to the Dreamcast mid test
        ;----------------------------------------------------------------------
        ld      P7
        and     #%00000001
        bnz     .plugged_dreamcast
        ret
.plugged_dreamcast
        jmpf    goodbye     


        .include        "./lib/libperspective.asm"
        .include        "./lib/libkcommon.asm"       
        .include        "./img/controller_back.asm"

;******************************************************************************
; End of File
;******************************************************************************
        .cnop   0,$200          ; pad to an even number of blocks

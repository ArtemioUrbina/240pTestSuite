;******************************************************************************
; libkcommon
;******************************************************************************
; Some common functions I use all the time
;******************************************************************************
; Nicer names for button masks
Button_Up               equ     %00000001
Button_Down             equ     %00000010
Button_Left             equ     %00000100
Button_Right            equ     %00001000
Button_A                equ     %00010000
Button_B                equ     %00100000
Button_Mode             equ     %01000000
Button_Sleep            equ     %10000000

Get_Input:
        ;----------------------------------------------------------------------
        ; Get input from P3, compare input to previous P3 input to get buttons
        ; pressed this cycle.
        ;----------------------------------------------------------------------
        ld      p3_last_input
        st      c
        ld      p3
        bn      acc, 6, .quit
        bn      acc, 7, .sleep
        st      p3_last_input
        xor     #%11111111
        and     c
        xor     #%11111111
        st      p3_pressed
        ret
.quit
        jmpf    goodbye
.sleep
        mov     #0, t1cnt               ; Disable audio
        set1    pcon, 0                 ; Activate HALT mode (saves power)
        bn      p3, 7, .sleep           ; wait until Sleep Key is released
        mov     #0,vccr                 ; turn off LCD
.sleepmore
        set1    pcon,0                  ; activate HALT mode (saves power)
        bp      p7, 0, .quit            ; Docked?
        bp      p3, 7, .sleepmore       ; no Sleep Key pressed yet
        mov     #$80, vccr              ; turn on LCD again
.waitsleepup:
        set1    pcon,0                  ; activate HALT modus (saves power)
        bn      p3,7,.waitsleepup
        ret

Check_Button_Pressed:
        ;----------------------------------------------------------------------
        ; Check if a button is pressed, not held, this cycle
        ;----------------------------------------------------------------------
        ; acc = button to check
        ;----------------------------------------------------------------------
        st      c
        ld      p3_pressed
        xor     #%11111111
        and     c
        ret

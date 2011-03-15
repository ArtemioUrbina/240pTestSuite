align macro
	cnop 0,\1
	endm
	

; ========================================================================================
;  Security Sector 
; ========================================================================================

SecurityCode:	; Uncomment the correct one for your target region

		incbin "security\usa.bin"

		; All Security sectors differ in size, and code executes from security end
		; in order to allow region codes to be edited without effecting data and
		; code offsets, an absolute jump to the actual IP code is used.

		bra IP_Start			

		; This padding ensures that region patching after compile time will be,
		; successful, as all security sectors are smaller than 0x600 bytes.

		align $600

; =======================================================================================
;  Initial Program, Sets up base hardware and bootstraps the main program.
;  This code ONLY works as an IPL (Intial Program loader), is generic, and has nothing 
;  to do with the game itself, and as such, should be left as-is.
;  Warning: Must end before $1000 on the CD filesystem (End of second sector)
; =======================================================================================		
IP_Start:
		bset	#1, $A12003		; Give WordRAM to Sub CPU
@initloop:		
		tst.b	$A1200F			; Has sub CPU finished init?
		bne	@initloop		; if not, branch

		move.b	#01,d0			; Set command to $01 (Load file)
		bsr	SubCommand		; Execute command
		
		move.b	#02,d0			; Set command to $02 (Request WordRAM)
		bsr	SubCommand		; Excecute command
		
		jmp	$200000			; Jump to WordRAM



; =======================================================================================
; SubCommand
;  Sends a command do the Sub CPU and waits for completion
;  Params: d0 = command id
; =======================================================================================
		
SubCommand:
		tst.b	$A1200F		; is sub cpu free?
		bne	SubCommand	; if not, wait for it to finish corrent operation

		move.b	#00, $A1200E	; Clear Command
@waitresponse1:
		tst.b	$A1200F		; sub cpu ready?
		beq	@waitresponse1	; if not, branch
	
		move.b	d0, $A1200E	; Send the command
@waitresponse2:
		tst.b	$A1200F		; Is sub CPU done?
		bne	@waitresponse2	; if not, branch

; =======================================================================================
; ASyncSubCommand
;  Sends a command do the Sub CPU but does not wait for completion
;  Params: d0 = command id
; =======================================================================================
AsyncSubCommand:
		tst.b	$A1200F		; is sub cpu free?
		bne	SubCommand	; if not, wait for it to finish corrent operation

		move.b	#00, $A1200E	; Clear Command
@waitready:
		tst.b	$A1200F		; sub cpu ready?
		beq	@waitready	; if not, branch
	
		move.b	d0, $A1200E	; Send the command
		rts
; =======================================================================================
;  Sega CD Boot loader by Luke Usher/SoullessSentinel (c) 2010
;  Luke current version at: https://github.com/LukeUsher/ProjectCD
;  BSD 2-Clause License by Luke (assigned after this fork)
;  Used with permission. This code is not released under the GPL.
;
;  Modified by Artemio Urbina for MDFourier and the 240p test Suite
;  All PCM and RAM routine made for the 240p Test Suite
;  Now checks if files exist and fails, and has return values in
;  some scenarios.
; =======================================================================================
align macro
	cnop 0,\1
	endm
	
; =======================================================================================
;  Sega CD Sub Program
;  This is the OS that runs on the Sega CD CPU, and handles all disc activity, and all
;  access to the Sega CD specific hardware
; =======================================================================================
		include "_inc/cdbios.asm"
		org $6000

; Push and pop macros
push macro code
	movem.l	\code,-(a7)
	endm

pop macro code
	movem.l	(a7)+,\code
	endm 
	
; Sub cpu PCM SOUND DRIVER EQUATES
ENVdat		equ	$FF0001
PANdat		equ	$FF0003
FDLdat		equ	$FF0005
FDHdat		equ	$FF0007
LSLdat		equ	$FF0009
LSHdat		equ	$FF000B
STdat		equ	$FF000D
CTRLdat		equ	$FF000F
ONOFFdat	equ	$FF0011
WAVEdat		equ	$FF2001

; clock cycle delay for PCM ops
PCMREGDELAY	equ	5
		
; =======================================================================================
;  Sub Module Header
; =======================================================================================
SPHeader:
ModuleName:			dc.b 'MAIN-SUBCPU',0
ModuleVersion:		dc.w 0, 0
NextModule:			dc.l 0
ModuleSize:			dc.l 0
ModuleStartAddr:	dc.l JumpTable-SPHeader
ModuleWorkRAM:		dc.l 0
JumpTable:			
					dc.w SP_Init-JumpTable
					dc.w SP_Main-JumpTable
					dc.w SP_IRQ-JumpTable
					dc.w 0


; =======================================================================================
;  SP_Init
;  Called once on intial boot (VBlank not enabled yet)
; =======================================================================================
SP_Init:
		andi.b	#$FA, $FF8003		; Set WordRAM to 2M Mode
		move.w	#'I', $FF8020		; Set Status to Init
		rts							; Return to BIOS (which will then call SP_Main)					; Return to BIOS (which will then call SP_Main)

; =======================================================================================
;  SP_Main
;  Main routine, this is the core of the SP Operating System
;  WARNING: Does NOT check validitiy of input, invalid function calls WILL crash the cpu
; =======================================================================================
SP_Main:
		bsr		InitPCM			; Initialize Ricoh RF5C164 (PCM) Chip
		move.w	#0, $FF8020		; Send value to main CPU
		move.w	#0, $FF8022		; Clean return value 1, needed for Laseractive. Crashes regular sp.asm
		move.w	#0, $FF8024		; Clean return value 2

@main_loop:
		tst.w	$FF8010			; Check command
		bne		@main_loop		; If NOT clear, loop 
		move.w	#1, $FF8020		; Else, set status to ready
@loop:				
		tst.w	$FF8010			; Check command
		beq		@loop			; If none issued, loop

		moveq	#0, d0			; Clear d0
		move.w	$FF8010, d0		; Store command to d0
		move.w	$FF8012, d1		; Store param to d1
		moveq	#0, d6			; Clear d6 for return value 1
		moveq	#0, d7			; Clear d7 for return value 2
		add.w	d0, d0			; Calculate Offset (Double)
		add.w	d0, d0			; Calculate Offset (Double again)
		jsr		OpTable(pc,d0)	; Execute function from jumptable
		move.w	#0, $FF8020		; Mark as done
		move.w	d6, $FF8022		; copy return value 1 if any
		move.w	d7, $FF8024		; copy return value 2 if any
		bra		@main_loop		; Loop

; =======================================================================================
;  OpTable
;  Jumptable of all functions provided by the SP Operating System
;  These must be within the range of the bra.w, as each entry is limited to 4 bytes
;  4 bytes = bra opcode (2 bytes) and a relative offset as a word.
; =======================================================================================		
OpTable:
		bra.w	Op_Null			    ; Null Operation
		bra.w	Op_InitISOFS		; Init ISO9660 filesystem
		bra.w	Op_GetWordRAM		; Give WordRAM to Main CPU
		bra.w	Op_InitCD		    ; Init
		bra.w	Op_SeekCDMDF	    ; Seek CD-DA Track 2 for MDFourier
		bra.w	Op_UnPauseCD	    ; Play (unpause) CD-DA Track
		bra.w	Op_StopCD		    ; Stop CD-DA Track
		bra.w	Op_RemovePauseLimit	; Removes the pause limit for MDF wait 
		bra.w	Op_ResetPauseLimit	; Resets the pause limit after MDF 
		bra.w	Op_PlayCDMDF	    ; Play CD-DA Track 2 for the MDFourier
		bra.w	Op_PlayCD240	    ; Play CD-DA Track 3 for the Suite
		bra.w	Op_PlayCDDA	    	; Play CD-DA Track from d1
		bra.w	Op_PlayPCM			; Play Full PCM Memory
		bra.w	Op_StopPCM			; Stop PCM Playback
		bra.w	Op_SetPCMLeft		; Set PCM for Left Channel
		bra.w	Op_SetPCMRight		; Set PCM for Right Channel
		bra.w	Op_SetPCMCenter		; Set PCM for Both Channels
		bra.w	Op_IncremetPCMFreq	; Increment the internal value by 1
		bra.w	Op_DecrementPCMFreq	; Decrement the internal value by 1
		bra.w	Op_TestPCM			; play PCM with custom Frequencies
		bra.w	Op_SetSamplesSweep	; Normal samples for sweep 32552hz
		bra.w	Op_SetSamplesTest	; Sound Test PCM 
		bra.w	Op_SetSamplesTest2	; Sound Test PCM 2
		bra.w	Op_SetSampSin32000	; Use 32000hz 1khz sample
		bra.w	Op_SetSampSin32552	; Use 32552hz 1khz sample
		bra.w	Op_SetSampSin32604	; Use 32604hz 1khz sample
		bra.w	Op_CheckPCMRAM		; Write and Check to the full PCM RAM with value in FF8012
		bra.w	Op_LoadPCMRAM		; Use PCM sample data to fill PCM RAM
		bra.w	Op_CDTracks			; Query Number of Tracks
		bra.w	Op_DriveVersion		; Query Drive Version
		bra.w	Op_DummyTest		; Dummy Test Command

Op_Null:
		rts
		
Op_InitISOFS:
		rts
		
Op_GetWordRAM:	
		bset	#0, $FF8003		; Give WordRAM to Main CPU
		rts

Op_InitCD:
		BIOS_MSCSTOP
		bsr		wait_BIOS
		lea		DriveInit_Params(pc),a0
		BIOS_DRVINIT
		bsr		wait_BIOS
		BIOS_CDCSTOP
		bsr		wait_BIOS
		BIOS_CDCSTAT
		bsr		wait_BIOS
        rts
	
Op_PlayCDMDF:
		rts
		
Op_PlayCD240:
		rts
		
Op_PlayCDDA:
		move.w	d1,d3
		jmp PlayCDDA
		
Op_UnPauseCD:
		rts

Op_RemovePauseLimit:
		rts
		
Op_ResetPauseLimit:
		rts
	
Op_SeekCDMDF:
		rts

Op_StopCD:
		BIOS_MSCSTOP
		bsr		wait_BIOS
		rts

Op_PlayPCM:
		move.b  #$08, FDHdat			; ReSet H byte of sample rate (real approx 32552KHz)
		bsr		PCMWait

		move.b  #$00, FDLdat			; ReSet L byte of sample rate (real approx 32552KHz)
		bsr		PCMWait
		
		move.b	#$FF, ENVdat			; Set ENV to FF - full volume
		bsr		PCMWait
			
		move.b	#$00, STdat				; Set start address to $0000
		bsr		PCMWait

		move.b	#$FE, ONOFFdat			; Set channel 1 to on
		bsr		PCMWait
		rts
		
Op_StopPCM:			
		move.b	#$FF, ONOFFdat			; Set all audio channels to off
		bsr		PCMWait
		
		move.b	#$00, ENVdat			; Set ENV to 00 - Muted
		bsr		PCMWait
		rts

Op_IncremetPCMFreq:		
		rts
		
Op_DecrementPCMFreq:		
		rts
		
Op_TestPCM:		
		rts
		
Op_SetPCMLeft:
		move.b	#$0F, PANdat			; Set full Left
		bsr		PCMWait
		rts
		
Op_SetPCMRight:
		move.b	#$F0, PANdat			; Set full Right
		bsr		PCMWait
		rts
		
Op_SetPCMCenter:
		move.b	#$FF, PANdat			; Set full L/R channels
		bsr		PCMWait
		rts
		
Op_SetSamplesSweep:
		rts

Op_SetSampSin32000:
		rts
		
Op_SetSampSin32552:
		rts

Op_SetSampSin32604:
		rts
		
Op_SetSamplesTest:
		rts
		
Op_SetSamplesTest2:
		rts

Op_CheckPCMRAM:
		move.b	#$FF, ONOFFdat			; Set all audio channels to off
		bsr		PCMWait
		move.b	#$40, CTRLdat			; Turn off PCM RAM Access
		bsr		PCMWait
		
		move.b	d1, d0					; Set RAM check value
		bsr		PCMValueStore			; Write all banks with value from d0
		bsr		PCMValueCompare			; Compare all banks with value from d0
		
		move.b	#$C0, CTRLdat			; Turn on PCM RAM Access
		bsr		PCMWait
		rts
		
Op_DummyTest:
		move.w	#$1,d6			; return OK
		move.w	#$E715,d7		; return magic number as parameter
		rts
		
; =======================================================================================
;  Op_CDTracks
;  Calls CDBSTAT for number of tracks
; =======================================================================================
Op_CDTracks:
		BIOS_CDBSTAT
		; byte 16 is 1st track 
		; byte 17 is last track 
		move.b  17(a0),d7		; copy last track
		cmp.b	#$ff,d7
		bne		@returnTracks	; continue if we got tracks

		move.w	#0,d7			; return 0 tracks if no disc was found
		move.w  #0,d6
		rts
		
@returnTracks:
		move.w  #1,d6
		rts
		
; =======================================================================================
;  Op_DriveVersion
;  Calls CDBSTAT for Drive Version
; =======================================================================================
Op_DriveVersion:
		BIOS_CDBSTAT
		; byte 17 is last track, if 0xff we could not load TOC so drive is wrong
		; byte 18 is Drive Version
		move.b  17(a0),d7		; copy last track
		cmp.b	#$ff,d7
		bne		@returnDrive	; continue if we got tracks
		
		move.w	#0,d7			; return 0 tracks if no disc was found
		move.w  #0,d6
		rts
		
@returnDrive:
		move.b  18(a0),d7		; copy drive version
		move.w	#1, d6			; return 1
		rts

; =======================================================================================
;  PlayCDDA: Plays a CD-DA track
;  Input: d3.b - Track number
; =======================================================================================

PlayCDDA:
		BIOS_CDCSTOP
		bsr		wait_BIOS
		BIOS_MSCSTOP
		bsr		wait_BIOS
		
		lea		CDTrack(pc),a0
		move.w	d3,(a0)
		BIOS_MSCPLAY1
		bsr		wait_BIOS
		rts

; =======================================================================================
;  wait_BIOS: Waits for the BIOS to get ready for commands
; =======================================================================================
		
wait_BIOS:
		BIOS_CDBCHK						; Check BIOS status
		bcs.s	wait_BIOS				; If not ready, branch
		rts

; =======================================================================================
;  InitPCM: Gets the Ricoh RF5C164 PCM chip ready for playback
; =======================================================================================
		
InitPCM:
		move.b	#$FF, ONOFFdat			; Set all audio channels to off
		bsr		PCMWait
	
		move.b	#$C0, CTRLdat			; Select channel 1
		bsr		PCMWait

		move.b	#$FF, PANdat			; Set full L/R channels
		bsr		PCMWait

		move.b	#$00, ENVdat			; Set ENV to 00 - silent
		bsr		PCMWait

		move.b	#$00, STdat				; Set start address to $0000
		bsr		PCMWait

		move.b  #$ff, LSHdat			; Set loop high address to $FF
		bsr		PCMWait

		move.b  #$f0, LSLdat			; Set loop low address to $F0 where FF is at for loop
		bsr		PCMWait

		move.b  #$08, FDHdat			; Set H byte of sample rate (real approx 32552KHz, even if docs say 32604)
		bsr		PCMWait

		move.b  #$00, FDLdat			; Set L byte of sample rate (real approx 32552KHz, even if docs say 32604)
		bsr		PCMWait
		rts

; =======================================================================================
;  PCMWait, waits PCMREGDELAY cycles for PCM to be ready for commands 
; =======================================================================================

PCMWait:
		move.l	d0, -(a7)
		move.w  #PCMREGDELAY, d0

@PCMWaitLoop:
		dbra	d0, @PCMWaitLoop
		move.l	(a7)+, d0
		rts


; =======================================================================================
;  WritePCMBank subroutine, copies PCM data form RAM to PCM RAM 
;  Input: d0.w - bytes to copy
;         d1.b - Target Bank
;         a0.l - source for PCM data
; =======================================================================================

WritePCMBank:
		push	a1				; Store used registers
		or.b	#$80, d1		; 1000b - bit 4 = 1 = play on bit 3 = 0 = select bank
		move.b	d1, CTRLdat		; Select bank to write into
		bsr		PCMWait

		move.l	#WAVEdat, a1	; Get pointer to start of PCM bank

		sub.l	#1, d0			; Adjust for dbra

@WritePCMLoop:
		move.b	(a0)+, (a1)
		add.l	#2, a1

		dbra	d0, @WritePCMLoop
		pop		a1				; Restore used registers
		rts
		
; =======================================================================================
;  PCMValueStore subroutine, writes whole PCM RAM to be a specific value
;  Input: d0.b - PCM Value to write
; =======================================================================================		

PCMValueStore:
		push	d1/d2/d3		; Store used registers
		move.b	#0,d2			; D2 - start at first bank

@PCMValueStoreLoop:		
		move.l	#$1000,d3		; we copy in bank increments
		move.b	d2,d1			; D1 - current bank
		bsr		WritePCMValueBank
		add.b	#1,d2			; D2 - Bank count
		cmp.w	#$10,d2			; have we finished copy in the number of banks (64kb)
		BNE		@PCMValueStoreLoop
		
		pop		d1/d2/d3		; Restore used registers
		rts

; =======================================================================================
;  WritePCMValueBank subroutine, writes value to PCM RAM
;  Input: d0.b - PCM Value to write
;         d3.w - Bank Size
;  Out:   d6.b - return value, 0 fail 1 ok
;         d7.w - If fail, address with difference ($0-$FFFF)
; =======================================================================================	

WritePCMValueBank:
		push	a1				; Store used registers
		move.b	d1, CTRLdat		; Select bank to write into
		bsr		PCMWait

		move.l	#WAVEdat, a1	; Get pointer to start of PCM bank
		sub.l	#1, d3			; Adjust for dbra

@WritePCMValueBankLoop:
		move.b	d0, (a1)
		add.l	#2, a1

		dbra	d3, @WritePCMValueBankLoop
		pop		a1				; Restore used registers
		rts

; =======================================================================================
;  PCMValueCompare subroutine, checks whole PCM RAM to be a specific value
;  Input: d0.b - PCM Value to check
;  Out:   d6.b - return value, 0 fail 1 ok
;         d7.w - If fail, address with difference ($0-$FFFF)
; =======================================================================================			

PCMValueCompare:
		push	d1/d2/d3		; Store used registers
		move.b	#0,d2			; D2 - start at first bank

@PCMValueCompareLoop:		
		move.l	#$1000,d3		; we copy in bank increments (4096 bytes)
		move.b	d2,d1			; D1 - current bank
		bsr		ComparePCMValueBank
		
		cmp.b	#1,d4
		BNE		@PCMValueCompareFail
		add.b	#1,d2			; D2 - Bank count
		cmp.w	#$10,d2			; have we finished copy in the number of banks (64kb)
		BNE		@PCMValueCompareLoop
		
		pop		d1/d2/d3		; Restore used registers
		move.w	#$1,d6			; return OK
		move.w	#$E715,d7		; return address of error (out of range)
		rts
		
@PCMValueCompareFail:
		move.w	#0,d6			; return fail
		move.w	d5,d7			; return Fail address
		pop		d1/d2/d3		; Restore used registers
		rts


; =======================================================================================
;  ComparePCMValueBank subroutine
;  Input: d0.b - PCM Value to check
;         d1.b - Bank to check
;         d3.w - Bank Size
;  Out:   d4.b - return value, 0 fail 1 ok
;         d5.w - If fail, address with difference ($0-$FFFF)
; =======================================================================================	

ComparePCMValueBank:
		push	d2/d6/d7/a1		; Store used registers
		move.b	d1, CTRLdat		; Select bank to write into
		bsr		PCMWait

		move.l	#WAVEdat, a1	; Get pointer to start of PCM bank
		move.w	d3, d6			; copy bank size to d7
		sub.l	#1, d3			; Adjust for dbra
		move.w	#0, d7			; Byte counter in current block
		
@ComparePCMValueBankLoop:
		move.b	(a1),d2
		cmp.b	d0, d2
		BNE.s	@ComparePCMValueError
		add.l	#2, a1
		add.w	#1, d7

		dbra	d3, @ComparePCMValueBankLoop
		
		move.w	#1, d4			; return 1, all ok
		pop		d2/d6/d7/a1		; Restore used registers
		rts

@ComparePCMValueError
		mulu.w	d6, d1			; calculate bank*size
		move.l	d7, d5			; copy current byte offset into d5
		add.l	d1, d5			; store complete address in d5 (bank*size+addr)
		move.w	#0, d4			; return 0, failed
		pop		d2/d6/d7/a1		; Restore used registers
		rts

; =======================================================================================
;  SP_IRQ
;  Interrupt Request, Level 2 Interrupt routine
; =======================================================================================
SP_IRQ:
		rts

; =======================================================================================
;  Op_LoadPCMRAM subroutine, loads PCM samples to RAM
;  Out:   d6.b - 1 success, 0 fail (file not found or bigger than 64KB)
; =======================================================================================

Op_LoadPCMRAM:
		push	d0/d1/d2/d3/a2/a3	; Store used registers

        move.w #2048,d2           	; number to words for cycle to fill 64kb
		move.l #$20000,a2           ; store the destination address in a2

@copy_m_loop:
		move.l #PCMSmplData, a3     ; store the source address in a3
        move.w #15,d1               ; size of source sample data
        
@copy_b_loop:
        move.w	(a3)+,(a2)+         ; copy one byte from source to destination
        dbf d1, @copy_b_loop        ; decrement d1 and repeat the loop if d1 is not zero
		
        dbf d2, @copy_m_loop    	; fill the 64kb

		; prepare for WritePCMBank
		move.l	#$20000, a2		; A2 - PCM Address
		move.b	#0, d2			; D2 - start at first bank
		move.b	#$10, d3		; Set banks for 64kb

@LoadPCMLoop:		
		move.l	#$1000,d0		; we copy in bank increments
		move.l	a2,a0			; A0 - PCM source Address
		move.b	d2,d1			; D1 - current bank
		bsr		WritePCMBank
		add.b	#1,d2			; D2 - Bank
		add.l	#$1000,a2		; Increment address by 1000h, a bank size
		cmp.b	d3,d2			; have we finished copy in the number of sectors?
		BNE		@LoadPCMLoop
		
		pop		d0/d1/d2/d3/a2/a3	; Restore used registers
		move.w	#1, d6			; return 1 for loaded
		rts
		
@failLoadPCM
		pop		d0/d1/d2/d3/a2/a3	; Restore used registers
		move.w	#0, d6			; return 0 for failed
		rts

	
; =======================================================================================
;  Data
; =======================================================================================

DriveInit_Params:
		dc.w	$01FF				; first track, last track (all)
		even				    	; just in case

;  PCMSampleData, 16 words		
PCMSmplData:
		dc.w	$8018, $3046, $5969, $747C
		dc.w	$7E7C, $7469, $5946, $3018
		dc.w	$8098, $B0C6, $D9E9, $F4FC
		dc.w	$FEFC, $F4E9, $D9C6, $B098
		align 2

CDTrack:
		dc.w	0
		align 2

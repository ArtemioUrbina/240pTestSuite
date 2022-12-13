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
;  This is the OS that runs on the Segs CD CPU, and handles all disc activity, and all
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

; CD Check status
ErrInvalid	equ	$0A
ErrTrayOpen	equ	$0B
ErrNoCD		equ	$0C

; clock cycle delay for PCM ops
PCMREGDELAY	equ	5

; CD Tracks
MDFCDTrack	equ	2
MscCDTrack	equ	3
		
; =======================================================================================
;  Sub Module Header
; =======================================================================================
ModuleName:			dc.b 'MAIN-SUBCPU',0
ModuleVersion:		dc.w 0, 0
NextModule:			dc.l 0
ModuleSize:			dc.l 0
ModuleStartAddr:	dc.l $20
ModuleWorkRAM:		dc.l 0
JumpTable:	
					dc.w SP_Init-JumpTable
					dc.w SP_Main-JumpTable
					dc.w SP_IRQ-JumpTable
					dc.w 0


; =======================================================================================
;  SP_Init
;  Called once on intial boot, as soon as it is loaded by the Sega CD. 
; =======================================================================================
SP_Init:
		andi.b	#$FA, $FF8003		; Set WordRAM to 2M Mode
		bsr		Init9660			; Init ISO filesystem
		clr.b	$FF800F				; Clear status flag
		rts							; Return to BIOS (which will then call SP_Main)

DriveInit_Params:
		dc.w	$01FF				; first track, last track (all)
		even				    	; just in case

; =======================================================================================
;  SP_Main
;  Main routine, this is the core of the SP Operating System
;  WARNING: Does NOT check validitiy of input, invalid function calls WILL crash the cpu
; =======================================================================================
SP_Main:
		tst.b	$FF800E			; Check command
		bne		SP_Main			; If NOT clear, loop 
		move.b	#1, $FF800F		; Else, set status to ready
@loop:				
		tst.b	$FF800E			; Check command
		beq		@loop			; If none issued, loop

		moveq	#0, d0			; Clear d0
		move.b	$FF800E, d0		; Store command to d0
		move.w	$FF8010, d1		; Store param to d1
		moveq	#0, d6			; Clear d6 for return value 1
		moveq	#0, d7			; Clear d7 for return value 2
		add.w	d0, d0			; Calculate Offset (Double)
		add.w	d0, d0			; Calculate Offset (Double again)
		jsr		OpTable(pc,d0)	; Execute function from jumptable
		move.b	#0, $FF800F		; Mark as done
		move.w	d6, $FF8020		; copy return value 1 if any
		move.w	d7, $FF8022		; copy return value 2 if any
		bra		SP_Main			; Loop

; =======================================================================================
;  OpTable
;  Jumptable of all functions provided by the SP Operating System
;  These must be within the range of the bra.w, as each entry is limited to 4 bytes
;  4 bytes = bra opcode (2 bytes) and a relative offset as a word.
; =======================================================================================		
OpTable:
		bra.w	Op_Null			    ; Null Operation
		bra.w	Op_LoadBootFile		; Load File (from ISO9660 filesystem)
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
		bra.w	Op_CheckPCMRAM		; Write and Check to the full PCM RAM with value in FF8010
		bra.w	Op_LoadPCMRAM		; Not used in Sega CD version
		bra.w	Op_CDTracks			; Query Number of Tracks
		bra.w	Op_DriveVersion		; Query Drive Version
		bra.w	Op_GetCDTrackType	; Query CD Track Type
		bra.w	Op_CheckCDReady		; Check if Dive and CD are ready
		bra.w	Op_CheckTrayClosed	; Return 1 on tray closed 0 on open
		bra.w	Op_TrayOpen			; Open the Tray
		bra.w	Op_DummyTest		; Dummy reply test

Op_Null:
		rts
		
Op_LoadBootFile:
		cmp.b	#1, bootloaded		; check if already loaded
		beq		@bootloaded			; load only once if already loaded due to race condition
		lea		@bootfile(pc),a0	; Name pointer
		bsr		FindFile		    ; Find File returns params in the right format for ReadCD
		move.l	#$80000, a0			; Set destination to WordRAM
		bsr		ReadCD				; load file
		move.b	#1, bootloaded		; set flag that we loaded the main
@bootloaded:
		rts
		
@bootfile:
		dc.b	'M_INIT.PRG',0
		align 2

Op_GetWordRAM:	
		bset	#0, $FF8003		; Give WordRAM to Main CPU
		rts

Op_InitCD:
		lea		DriveInit_Params(pc),a0
		BIOS_DRVINIT
		bsr		wait_BIOS
		BIOS_CDCSTOP
		bsr		wait_BIOS
		BIOS_CDCSTAT
		bsr		wait_BIOS
		bsr		InitPCM
        rts
	
Op_PlayCDMDF:
		move.w	#MDFCDTrack,d3
		jmp PlayCDDA
		
Op_PlayCD240:
		move.w	#MscCDTrack,d3
		jmp PlayCDDA
		
Op_PlayCDDA:
		move.w	d1,d3
		jmp PlayCDDA
		
Op_UnPauseCD:
		BIOS_MSCPAUSEOFF
		bsr		wait_BIOS
		rts

Op_RemovePauseLimit:
		move.w	#$ffff,d1				;Removes CD Pause timeout
		BIOS_CDBPAUSE
		bsr		wait_BIOS
		rts
		
Op_ResetPauseLimit:
		move.w	#$4000,d1				;Sets the CD Pause timeout to $4000 value after stop
		BIOS_CDBPAUSE
		bsr		wait_BIOS
		rts
	
Op_SeekCDMDF:
		lea		CDTrack(pc),a0
		move.w	#MDFCDTrack,d1
		move.w	d1,(a0)
		BIOS_MSCSEEK1
		bsr		wait_BIOS
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
		add.w	#$1,PCMTestFreq	
		rts
		
Op_DecrementPCMFreq:		
		sub.w	#$1,PCMTestFreq	
		rts
		
Op_TestPCM:		
		move.w	PCMTestFreq,d0
		
		move.b	d0, FDLdat				; Set L byte of sample rate
		bsr		PCMWait

		lsr.w	#8, d0					; Get high order byte
		move.b	d0, FDHdat				; Set H byte of sample rate 
		bsr		PCMWait
	
		bsr		Op_PlayPCM
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
		lea		pcmsweepfile(pc),a0	; Sweep 1-16khz
		bsr		LoadPCMFile
		rts

Op_SetSampSin32000:
		lea		pcmssin32000(pc),a0	; 1khz sine wave at 32000hz
		bsr		LoadPCMFile
		rts
		
Op_SetSampSin32552:
		lea		pcmsincomm(pc),a0	; 1khz sine wave at 32552hz
		bsr		LoadPCMFile
		rts

Op_SetSampSin32604:
		lea		pcmsinsega(pc),a0	; 1khz sine wave at 32604hz
		bsr		LoadPCMFile
		rts
		
Op_SetSamplesTest:
		lea		pcmtest(pc),a0	; Never Give Up!
		bsr		LoadPCMFile
		rts
		
Op_SetSamplesTest2:
		lea		pcmtest2(pc),a0	; Fourier Art
		bsr		LoadPCMFile
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

Op_TrayOpen:
		BIOS_DRVOPEN
		rts
		
; =======================================================================================
;  Op_CheckTrayClosed
;  Calls CDBSTAT to check if tray is closed
; =======================================================================================

Op_CheckTrayClosed:
		BIOS_CDBSTAT
		move.w  0(a0),d1		; copy status
		btst.l  #14,d1			; Is Tray Open?
		bne		@trayOpen		; Tray was open...
		btst.l  #12,d1			; Does it have a disc?
		bne		@noCD			; No disc present, abort
		btst.l  #15,d1			; is it ready?
		bne		Op_CheckTrayClosed
		
		move.w  #1,d6			; return tray closed
		rts
		
@trayOpen:
		move.w	#ErrTrayOpen,d7	; Tray Open
		move.w  #0,d6			; return fail
		rts
		
@noCD:
		move.w	#ErrNoCD,d7		; No CD
		move.w  #0,d6			; return fail

; =======================================================================================
;  Op_CheckCDReady
;  Calls CDBSTAT to check if tray is closed, CD is present and TOC is read
; =======================================================================================

Op_CheckCDReady:
		BIOS_CDBSTAT
		move.w  0(a0),d1		; check status
		btst.l  #14,d1			; Is Tray Open?
		bne		@trayOpen		; Tray was open...
		btst.l  #12,d1			; Does it have a disc?
		bne		@noCD			; No disc present, abort
		btst.l  #15,d1			; is it ready?
		bne		Op_CheckCDReady
		
@readingTOC:
		BIOS_CDBSTAT
		move.w  0(a0),d1		; check status
		btst.l  #14,d1			; Is Tray Open?
		bne		@trayOpen		; Tray was open...
		btst.l  #12,d1			; Does it have a disc?
		bne		@noCD			; No disc present, abort
		btst.l  #13,d1			; is it reading TOC?
		bne		@readingTOC
		
		move.w  #1,d6			; All good, return success
		rts
		
@trayOpen:
		move.w	#ErrTrayOpen,d7	; Tray Open
		move.w  #0,d6			; return fail
		rts
		
@noCD:
		move.w	#ErrNoCD,d7		; No CD
		move.w  #0,d6			; return fail
		rts
	
; =======================================================================================
;  Op_CDTracks
;  Calls CDBSTAT for number of tracks
; =======================================================================================
Op_CDTracks:
		bsr		Op_CheckCDReady
		cmp.w	#0, d6
		bne		@checkTracks	
		rts						; return fail from Op_CheckCDReady
		
@checkTracks:
		; byte 16 is 1st track 
		; byte 17 is last track 
		move.b  17(a0),d7		; copy last track
		cmp.b	#$ff,d7			; Compare to check value
		beq		@trakListErr	; Continue if we got tracks
		
		move.w  #1,d6			; return success and tracks in d7
		rts

@trakListErr:		
		move.w	#ErrInvalid,d7	; Track list not read
		move.w  #0,d6			; emulation? return fail
		rts
		
; =======================================================================================
;  Op_DriveVersion
;  Calls CDBSTAT for Drive Version
; =======================================================================================
Op_DriveVersion:
		bsr		Op_CDTracks
		cmp.w	#0,d6
		bne		@returnDrive	
		rts		; return same errors as Op_CDTracks
		
@returnDrive:
		move.b  18(a0),d7		; copy drive version
		move.w	#1, d6			; return 1
		rts

; =======================================================================================
;  Op_GetCDTrackType: Gets CD track Type
;  Input: d3.b - Track number
;  Output: d6.w  fail or sucess
;  Output: d7.w  raw track type
; =======================================================================================
Op_GetCDTrackType:
		BIOS_CDBTOCREAD
		bsr		wait_BIOS
		; d0 BCD Time, ignored
		; d1 Track Type, return raw
		move.w	#1,d6	
		move.w	d1,d7
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
;  LoadPCMFile subroutine, loads a PCM file from CD to the PCM RAM
;  Input: a0.l - PCM Filename in CD-ROM
;  Out:   d6.b - 1 success, 0 fail (file not found or bigger than 64KB)
; =======================================================================================

LoadPCMFile:
		push	d0/d1/d2/d3/a2	; Store used registers
		bsr		FindFile		; Find File returns params in the right format for ReadCD
		move.l	#$20000, a0		; Set destination to Program RAM
		bsr		ReadCD

		cmp		#0,d6			; Check if Read CD found the file
		beq		@failLoadPCM	; abort if not loaded
		move.l	d1, d3			; d1.l has the size from above in sectors (2048 bytes)
		asr.l	#1, d3			; convert sectors to banks (4096 bytes)
		
		cmp		#$10, d3		; Check is smaller than 64kb
		bgt		@failLoadPCM	; Abort if bigger
		move.l	#$20000, a2		; A2 - PCM Address
		move.b	#0, d2			; D2 - start at first bank

@LoadPCMLoop:		
		move.l	#$1000,d0		; we copy in bank increments
		move.l	a2,a0			; A0 - PCM source Address
		move.b	d2,d1			; D1 - current bank
		bsr		WritePCMBank
		add.b	#1,d2			; D2 - Bank
		add.l	#$1000,a2		; Increment address by 1000h, a bank size
		cmp.b	d3,d2			; have we finished copy in the number of sectors?
		BNE		@LoadPCMLoop
		
		pop		d0/d1/d2/d3/a2	; Restore used registers
		; our samples already have the FF terminators for loop
		move.w	#1, d6			; return 1 for loaded
		rts
		
@failLoadPCM
		pop		d0/d1/d2/d3/a2	; Restore used registers
		move.w	#0, d6			; return 0 for failed
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
		move.l	#$1000,d3		; we copy in bank increments
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
;  Op_LoadPCMRAM subroutine, Only available form cartridge
;  Out:   d6.b - 1 success, 0 fail 
; =======================================================================================

Op_LoadPCMRAM:
		move.w	#0, d6			; return 0 for failed
		rts
	
; =======================================================================================
;  SP_IRQ
;  Interrupt Request, Level 2 Interrupt routine
; =======================================================================================
SP_IRQ:
		rts

; =======================================================================================
;  ReadCD
;  Input: d0.l	start sector
;	  d1.l	sectors to read
;	  a0.l	destination
; =======================================================================================
ReadCD:
		push	d0-d7/a0-a6			; Store all registers
		
		cmp.l	#0,d0				; check if we have a valid size
		beq		@readcdfail			; abort if no size
		lea		BiosPacket(pc), a5	; Load bios packet
		move.l	d0, (a5)			; Write start sector to packet
		move.l	d1, 4(a5)			; Write size to packet
		move.l	a0, 8(a5)			; Write destination to packet
		movea.l	a5, a0				; Put packet to a0 (for BIOS)
		BIOS_CDCSTOP				; Stop CDC
		BIOS_ROMREADN				; Begin data read
@waitSTAT:
		BIOS_CDCSTAT				; Check CDC status
		bcs		@waitSTAT			; If not ready, branch
@waitREAD:
		BIOS_CDCREAD				; Read data
		bcc		@waitREAD			; If not done, branch
@WaitTransfer:
		movea.l	8(a5), a0			; Get destination address
		lea		12(a5), a1			; Get header
		BIOS_CDCTRN					; Transfer sector
		bcc		@waitTransfer		; If not done, branch
		BIOS_CDCACK					; Acknowledge transfer
		addq.l	#1, (a5)			; Increment starting sector
		addi.l	#$0800, 8(a5)		; Increment destination address
		subq.l	#1, 4(a5)			; Decrement sectors left
		bne		@waitSTAT			; If not finished, branch
		pop		d0-d7/a0-a6			; Restore all registers
		move.l	#1,d6				; return 1 when loaded
		rts							; Return

@readcdfail
		move.l	#0,d6				; return 0 when failed
		pop		d0-d7/a0-a6			; Restore all registers
		rts
	
; =======================================================================================
;  ISO9660 Driver
; =======================================================================================
Init9660:
		push	d0-d7/a0-a6		; Store all registers

		; Load Volume VolumeDescriptor

		move.l	#$10, d0			; Start Sector
		move.l  #2, d1				; Size in sector
		lea.l	SectorBuffer, a0	; Destination
		bsr		ReadCD				; Read Data

		; Load Root Directory

		lea.l	SectorBuffer,a0	; Get pointer to sector buffer
		lea.l	156(a0),a1		; Get root directory record
		
		move.b	6(a1),d0		; Get first part of Sector address
		lsl.l	#8,d0			; bitshift
		move.b	7(a1),d0		; Get next part of sector address
		lsl.l	#8,d0			; bitshift
		move.b	8(a1),d0		; get next part of sector address
		lsl.l	#8,d0			; bitshift
		move.b	9(a1),d0		; get final part of sector address.
								; d0 now contains start sector address
						
		move.l	#$20, d1		; Size ($20 Sectors)
		bsr	ReadCD
		
		pop	d0-d7/a0-a6			; Restore all registers		
		rts
		
; =======================================================================================
;  Find File (ISO9660)
;  Input:  a0.l - Pointer to filename
;  Output: d0.l - Start sector
;	       d1.l - Filesize
; =======================================================================================
FindFile:
		push	a1/a2/a5/a6		; Store used registers
		lea.l	SectorBuffer,a1	; Get sector buffer
		move.l	#$10000,a5		; set a limit for search
@ReadFilenameStart:
		movea.l	a0,a6			; Store filename pointer
		move.b	(a6)+,d0		; Read character from filename
@findFirstChar:
		movea.l	a1,a2			; Store Sector buffer pointer
		cmp.b	(a1)+,d0		; Compare with first letter of filename and increment
		bne.b	@findFirstChar	; If not matched, branch
		cmp.l	a5,a1			; compare offset with search limit
		bge.b	@filenotfound	; abort of limit reached
@checkChars:
		move.b	(a6)+,d0		; Read next charactor of filename and increment
		beq		@getInfo		; If all characters were matched, branch			
		cmp.b	(a1)+,d0		; Else, check next character
		bne.b	@ReadFilenameStart	; If not matched, find next file
		bra.b	@checkChars		; else, check next character

@filenotfound:
		move.l	#0,d0			; return 0 when failed
		move.l	#0,d1			; return 0 when failed
		pop	a1/a2/a5/a6			; Restore used registers	
		rts
	
@getInfo:
		sub.l	#33,a2			; Move to beginning of directory entry
		move.b	6(a2),d0		; Get first part of Sector address
		lsl.l	#8,d0			; bitshift
		move.b	7(a2),d0		; Get next part of sector address
		lsl.l	#8,d0			; bitshift
		move.b	8(a2),d0		; get next part of sector address
		lsl.l	#8,d0			; bitshift
		move.b	9(a2),d0		; get final part of sector address.
								; d0 now contains start sector address

		move.b	14(a2),d1		; Same as above, but for FileSize
		lsl.l	#8,d1
		move.b	15(a2),d1
		lsl.l	#8,d1
		move.b	16(a2),d1
		lsl.l	#8,d1
		move.b	17(a2),d1
						
		lsr.l	#8,d1			; Bitshift filesize (to get sector count)
		lsr.l	#3,d1			; if file is not aligned to 2048 bytes, will discard
	
		pop	a1/a2/a5/a6			; Restore used registers	
		rts
	
; =======================================================================================
;  Data
; =======================================================================================

PCMTestFreq:
		dc.w	$0800
		
pcmsweepfile:
		dc.b	'SWEEP.PCM',0
		align 2
		
pcmssin32000:
		dc.b	'SIN32000.PCM',0
		align 2
		
pcmsincomm:
		dc.b	'SIN32552.PCM',0
		align 2
		
pcmsinsega:
		dc.b	'SIN32604.PCM',0
		align 2
		
pcmtest:
		dc.b	'PCMTEST.PCM',0
		align 2

pcmtest2:
		dc.b	'PCMTESZ.PCM',0
		align 2
		
bootloaded:
		dc.b	0
		align 2
		
CDTrack:
		dc.w	0
		align 2
		
; =======================================================================================
;  BiosPacket
;  Stores paramaters for BIOS calls
; =======================================================================================

BiosPacket:
	dc.l	0, 0, 0, 0, 0
Header:
	ds.l	32

; =======================================================================================
; ANY CODE PAST THIS POINT WILL BE OVERWRITTEN AT RUNTIME AND IS USED AS A READ BUFFER
; Any and all additions to this Sub-Program must be added BEFORE the definition of this 
; memory area
; =======================================================================================		
SectorBuffer:
		dc.l 0

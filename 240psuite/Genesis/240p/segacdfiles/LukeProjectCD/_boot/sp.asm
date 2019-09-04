; =======================================================================================
;  Sega CD Boot loader by Luke Usher/SoullessSentinel (c) 2010
;  Used with permission. This code is not released under the GPL.
;  Modified by Artemio Urbina for MDFourier and the 240p test Suite
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

; clock cycle delay for PCM ops
PCMREGDELAY	equ	5
		
; =======================================================================================
;  Sub Module Header
; =======================================================================================
ModuleName:	dc.b 'MAIN-SUBCPU',0
ModuleVersion:	dc.w 0, 0
NextModule:	dc.l 0
ModuleSize:	dc.l 0
ModuleStartAddr:dc.l $20
ModuleWorkRAM:	dc.l 0
JumpTable:	dc.w SP_Init-JumpTable
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
		dc.b	1, $FF				; first track, last track (all)
		even				    	; just in case
Buffer:
		dc.l	2
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
		add.w	d0, d0			; Calculate Offset (Double)
		add.w	d0, d0			; Calculate Offset (Double again)
		jsr		OpTable(pc,d0)	; Execute function from jumptable
		move.b	#0, $FF800F		; Mark as done
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
		bra.w	Op_PlayCDMDF	    ; Play CD-DA Track 2 for the MDFourier Debug
		bra.w	Op_PlayCD240	    ; Play CD-DA Track 3 for the Suite
		bra.w	Op_PlayPCM			; Play Full PCM Memory
		bra.w	Op_StopPCM			; Stop PCM Playback
		bra.w	Op_SetPCMLeft		; Set PCM for Left Channel
		bra.w	Op_SetPCMRight		; Set PCM for Right Channel
		bra.w	Op_SetPCMCenter		; Set PCM for Both Channels
		bra.w	Op_IncremetPCMFreq	; Increment the internal value by 1
		bra.w	Op_DecrementPCMFreq	; Decrement the internal value by 1
		bra.w	Op_TestPCM			; Test PCM Frequencies
		bra.w	Op_SetSamplesSweep	; Normal samples for sweep 32552hz
		bra.w	Op_SetSamplesTest	; Sound Test PCM 
		bra.w	Op_SetSamplesTest2	; Sound Test PCM 2
		bra.w	Op_SetSampSin32000	; Use 32000hz 1khz sample
		bra.w	Op_SetSampSin32552	; Use 32552hz 1khz sample
		bra.w	Op_SetSampSin32604	; Use 32604hz 1khz sample

Op_Null:
		rts
		
Op_LoadBootFile:
		lea		@bootfile(pc),a0	; Name pointer
		bsr		FindFile		    ; Find File returns params in the right format for ReadCD
		move.l	#$80000, a0			; Set destination to WordRAM
		bsr		ReadCD
		rts
		
@bootfile:
		dc.b	'M_INIT.PRG',0
		align 2

Op_GetWordRAM:	
		bset	#0, $FF8003		; Give WordRAM to Main CPU
		rts

Op_InitCD:
		moveq	#0,d0
		lea		DriveInit_Params(pc),a0
		BIOS_DRVINIT
		BIOS_CDCSTOP
		BIOS_CDCSTAT
		bsr		InitPCM
        rts
	
Op_PlayCDMDF:
		move.w	#2,d1
		jmp PlayCDDA
		
Op_PlayCD240:
		move.w	#3,d1
		jmp PlayCDDA
		
PlayCDDA:
		lea		(Buffer).l,a0
		move.w	d1,(a0)
		BIOS_MSCPLAY1
		rts
		
Op_UnPauseCD:
		BIOS_MSCPAUSEOFF
		rts
		
Op_SeekCDMDF:
        move.w	#$ffff,d1    			;Removes CD Pause timeout, reset at stop
        BIOS_CDBPAUSE

		move.w	#2,d1
		lea		(Buffer).l,a0
		move.w	d1,(a0)
		BIOS_MSCSEEK1
		rts

Op_StopCD:
		BIOS_MSCSTOP

        move.w	#$1194,d1    			;Sets the CD Pause timeout to the minimum value after stop
        BIOS_CDBPAUSE
		rts

Op_PlayPCM:
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
		
		move.b  #$08, FDHdat			; Set H byte of sample rate 16khz at 0x07DD
	    bsr		PCMWait
    
	    move.b  #$00, FDLdat			; Set L byte of sample rate
	    bsr		PCMWait
		rts

PCMTestFreq:
		dc.w	$0800
		
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

InitPCM:
	    move.b	#$FF, ONOFFdat			; Set all audio channels to off
	    bsr		PCMWait
	
	    move.b	#$C0, CTRLdat			; Select channel 0
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
    
	    move.b  #$08, FDHdat			; Set H byte of sample rate 16khz at 0x08DD (Docs are wrong, check MDFourier site)
	    bsr		PCMWait
    
	    move.b  #$00, FDLdat			; Set L byte of sample rate
	    bsr		PCMWait
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

PCMWait:
	    move.l	d0, -(a7)
	    move.w  #PCMREGDELAY, d0

PCMWaitLoop:
	    dbra	d0, PCMWaitLoop
	    move.l	(a7)+, d0
	    rts

pcmsweepfile:
		dc.b	'SWEEP.PCM',0
		align 2
		
Op_SetSamplesSweep:
		lea		pcmsweepfile(pc),a0	; Sweep 1-16khz
		bsr		LoadPCMFile
		rts

pcmssin32000:
		dc.b	'SIN32000.PCM',0
		align 2
		
Op_SetSampSin32000:
		lea		pcmssin32000(pc),a0	; 1khz sine wave at 32000hz
		bsr		LoadPCMFile
		rts
		
pcmsincomm:
		dc.b	'SIN32552.PCM',0
		align 2
		
Op_SetSampSin32552:
		lea		pcmsincomm(pc),a0	; 1khz sine wave at 32552hz
		bsr		LoadPCMFile
		rts
		
pcmsinsega:
		dc.b	'SIN32604.PCM',0
		align 2

Op_SetSampSin32604:
		lea		pcmsinsega(pc),a0	; 1khz sine wave at 32604hz
		bsr		LoadPCMFile
		rts
		
pcmtest:
		dc.b	'PCMTEST.PCM',0
		align 2
		
Op_SetSamplesTest:
		lea		pcmtest(pc),a0	; Never Give Up!
		bsr		LoadPCMFile
		rts
		
pcmtest2:
		dc.b	'PCMTESZ.PCM',0
		align 2
		
Op_SetSamplesTest2:
		lea		pcmtest2(pc),a0	; Fourier Art
		bsr		LoadPCMFile
		rts

LoadPCMFile:
		bsr		FindFile		; Find File returns params in the right format for ReadCD
		move.l	#$20000, a0		; Set destination to Program RAM
		bsr		ReadCD
				
; d1.l has the size from above in sectors (2048)
; but this code is (still) hardcoded for 64kb

		move.l	#$20000, a2		; A2 - PCM Address
		move.b	#0,d2			; D2 - start at first bank
LoadPCMLoop:		
		move.l	#$1000,d0		; we copy in bank size increments
		move.l	a2,a0			; A0 - PCM Address
		move.b	d2,d1			; D1 - current bank
		bsr		WritePCM
		add.b	#1,d2			; D1 - Bank
		add.l	#$1000,a2		; Increment address by 1000h
		cmp.w   #$10,d2
		BNE     LoadPCMLoop
		; our samples already have the FF terminators for loop
		rts


; D0 - Size
; D1 - Bank
; A0 - PCM Address
WritePCM:
		or.b	#$80, d1		; 1000b - bit 4 = 1 = play on bit 3 = 0 = select bank
		move.b	d1, CTRLdat		; Select bank to write into
		bsr		PCMWait

		move.l	#WAVEdat, a1	; Get pointer to start of PCM bank

		sub.l	#1, d0			; Adjust for dbra

@WritePCMLoop:
		move.b	(a0)+, (a1)
		add.l	#2, a1

		dbra	d0, @WritePCMLoop
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
		rts							; Return

		
; =======================================================================================
;  BiosPacket
;  Stores paramaters for BIOS calls
; =======================================================================================

BiosPacket:
	dc.l	0, 0, 0, 0, 0
Header:
	ds.l	32
	
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
;	   d1.l - Filesize
; =======================================================================================
FindFile:
		push	a1/a2/a6		; Store used registers
		lea.l	SectorBuffer,a1	; Get sector buffer
@ReadFilenameStart:
		movea.l	a0,a6			; Store filename pointer
		move.b	(a6)+,d0		; Read character from filename
@findFirstChar:
		movea.l	a1,a2			; Store Sector buffer pointer
		cmp.b	(a1)+,d0		; Compare with first letter of filename and increment
		bne.b	@findFirstChar		; If not matched, branch
@checkChars:
		move.b	(a6)+,d0		; Read next charactor of filename and increment
		beq		@getInfo		; If all characters were matched, branch			
		cmp.b	(a1)+,d0		; Else, check next character
		bne.b	@ReadFilenameStart	; If not matched, find next file
		bra.b	@checkChars		; else, check next character
	
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
		lsr.l	#3,d1
	
		pop	a1/a2/a6			; Restore used registers	
		rts

; =======================================================================================
; ANY CODE PAST THIS POINT WILL BE OVERWRITTEN AT RUNTIME AND IS USED AS A READ BUFFER
; Any and all additions to this Sub-Program must be added BEFORE the definition of this 
; memory area
; =======================================================================================		
SectorBuffer:
		dc.l 0

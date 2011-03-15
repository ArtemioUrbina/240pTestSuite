; =======================================================================================
;  Sega CD Boot loader by Luke Usher/SoullessSentinel (c) 2010
;  Used with permission. This code is not released under the GPL.
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
		bsr	Init9660		; Init ISO filesystem
		clr.b	$FF800F			; Clear status flag
		rts				; Return to BIOS (which will then call SP_Main)

; =======================================================================================
;  SP_Main
;  Main routine, this is the core of the SP Operating System
;  WARNING: Does NOT check validitiy of input, invalid function calls WILL crash the cpu
; =======================================================================================
SP_Main:
		tst.b	$FF800E			; Check command
		bne	SP_Main			; If NOT clear, loop 
		move.b	#1, $FF800F		; Else, set status to ready
@loop:				
		tst.b	$FF800E			; Check command
		beq	@loop			; If none issued, loop

		moveq	#0, d0			; Clear d0
		move.b	$FF800E, d0		; Store command to d0
		add.w	d0, d0			; Calculate Offset (Double)
		add.w	d0, d0			; Calculate Offset (Double again)
		jsr	OpTable(pc,d0)		; Execute function from jumptable
		move.b	#0, $FF800F		; Mark as done
		bra	SP_Main			; Loop

; =======================================================================================
;  OpTable
;  Jumptable of all functions provided by the SP Operating System
;  These must be within the range of the bra.w, as each entry is limited to 4 bytes
;  4 bytes = bra opcode (2 bytes) and a relative offset as a word.
; =======================================================================================		
OpTable:
		bra.w	Op_Null			; Null Operation
		bra.w	Op_LoadBootFile		; Load File (from ISO9660 filesystem)
		bra.w	Op_GetWordRAM		; Give WordRAM to Main CPU

Op_Null:
		rts
		
Op_LoadBootFile:
		lea	@bootfile(pc),a0	; Name pointer
		bsr	FindFile		; Find File returns params in the right format for ReadCD
		move.l	#$80000, a0		; Set destionation to WordRAM
		bsr	ReadCD
		rts
@bootfile:
		dc.b	'M_INIT.PRG',0
		align 2
		
Op_GetWordRAM:
		bset	#0, $FF8003		; Give WordRAM to Main CPU
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
		push	d0-d7/a0-a6		; Store all registers
		lea	BiosPacket(pc), a5	; Load bios packet
		move.l	d0, (a5)		; Write start sector to packet
		move.l	d1, 4(a5)		; Write size to packet
		move.l	a0, 8(a5)		; Write destination to packet
		movea.l	a5, a0			; Put packet to a0 (for BIOS)
		BIOS_CDCSTOP			; Stop CDC
		BIOS_ROMREADN			; Begin data read
@waitSTAT:
		BIOS_CDCSTAT			; Check CDC status
		bcs	@waitSTAT		; If not ready, branch
@waitREAD:
		BIOS_CDCREAD			; Read data
		bcc	@waitREAD		; If not done, branch
@WaitTransfer:
		movea.l	8(a5), a0		; Get destination address
		lea	12(a5), a1		; Get header
		BIOS_CDCTRN			; Transfer sector
		bcc	@waitTransfer		; If not done, branch
		BIOS_CDCACK			; Acknowledge transfer
		addq.l	#1, (a5)		; Increment starting sector
		addi.l	#$0800, 8(a5)		; Increment destination address
		subq.l	#1, 4(a5)		; Decrement sectors left
		bne	@waitSTAT		; If not finished, branch
		pop	d0-d7/a0-a6		; Restore all registers
		rts				; Return

		
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

		move.l	#$10, d0		; Start Sector
		move.l  #2, d1			; Size in sector
		lea.l	SectorBuffer, a0	; Destination
		bsr	ReadCD			; Read Data

						; Load Root Directory

		lea.l	SectorBuffer,a0		; Get pointer to sector buffer
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
		
		pop	d0-d7/a0-a6		; Restore all registers		
		rts
		
; =======================================================================================
;  Find File (ISO9660)
;  Input:  a0.l - Pointer to filename
;  Output: d0.l - Start sector
;	   d1.l - Filesize
; =======================================================================================
FindFile:
		push	a1/a2/a6		; Store used registers
		lea.l	SectorBuffer,a1		; Get sector buffer
@ReadFilenameStart:
		movea.l	a0,a6			; Store filename pointer
		move.b	(a6)+,d0		; Read character from filename
@findFirstChar:
		movea.l	a1,a2			; Store Sector buffer pointer
		cmp.b	(a1)+,d0		; Compare with first letter of filename and increment
		bne.b	@findFirstChar		; If not matched, branch
@checkChars:
		move.b	(a6)+,d0		; Read next charactor of filename and increment
		beq	@getInfo		; If all characters were matched, branch			
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
	
		pop	a1/a2/a6		; Restore used registers	
		rts

; =======================================================================================
; ANY CODE PAST THIS POINT WILL BE OVERWRITTEN AT RUNTIME AND IS USED AS A READ BUFFER
; Any and all additions to this Sub-Program must be added BEFORE the definition of this 
; memory area
; =======================================================================================		
SectorBuffer:
		dc.l 0

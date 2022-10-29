; 240p Test Suite Sound Driver
; Based on freem ADPCM example from: http://ajworld.net/neogeodev/
; Modifications made by Artemio Urbina for the Suite and MDFourier
;==============================================================================;
; defines
	include "sounddef.inc"
	include "sysmacro.inc"
	include "soundcmd.inc" ; shared include with 68K

; Defines for RST usage, in case the locations change later.
writeDEportA	= $18
writeDEportB	= $20
waitYamaha		= $28
;==============================================================================;
	org $0000
Start:
	di
	jp		EntryPoint
;==============================================================================;
	org $0018
	jp		jmp_writeDEportA
;==============================================================================;
	org $0020
	jp		jmp_writeDEportB
;==============================================================================;
	org $0028
; Keep checking the busy flag in Status 0 until it's clear.
checkLoop:
	in		a,(4)			; read Status 0 (busy flag in bit 7)
	add		a
	jr		C,checkLoop
	ret
;==============================================================================;
	org $0038
	di						; re-enabled at end of IRQ routine
	jp		IRQ
;==============================================================================;

;==============================================================================;
	org $0066
; NMI
; Inter-processor communications.

; In this driver, the NMI gets the command from the 68K and interprets it.

NMI:
	; save registers
	push	af
	push	bc
	push	de
	push	hl

	in		a,(0)			; Acknowledge NMI, get command from 68K via Port 0
	ld		(curCommand),a	; update curCommand
	ld		b,a				; load value into b for comparisons

	; "Commands $01 and $03 are always expected to be implemented as they
	; are used by the BIOSes for initialization purposes." - NeoGeoDev Wiki
	cp		1				; Command 1 (Slot Switch)
	jp		Z,cmdSwitchSlot
	cp		3				; Command 3 (Soft Reset)
	jp		Z,cmdSoftReset
	or		a				; check if Command is 0
	jp		Z,endNMI		; exit if Command 0

	call	HandleCommand

	xor		a				; clear a for now.
	out		(0xC),a			; Reply to 68K with something.
	out		(0),a			; Write to port 0 (Clear sound code)

endNMI:
	; restore registers
	pop		hl
	pop		de
	pop		bc
	pop		af
	retn

;==============================================================================;
; IRQ (called from $0038)
; Handle an interrupt request.

IRQ:
; save registers
	push	af
	push	bc
	push	de
	push	hl
	push	ix
	push	iy

	; update internal Status 1 register
	in		a, (6)
	ld		(intStatus1),a

	; --check status of ADPCM channels--

	; for ADPCM-B, looping is handled automatically (via repeat flag).
	; we don't need to check anything. ADPCM-A, on the other hand...

	; Check if any ADPCM-A channel has finished
	; only stop/loop the sample if we've reached the end of it.
	ld		a,(intStatus1)
	bit		0,a
	jp		Z,.IRQ_checkPCMA1

	; stop sample on ADPCM-A channel 1
	ld		de,0x0081
	rst 	writeDEportB

.IRQ_checkPCMA1:
	ld		a,(intStatus1)
	bit		1,a
	jp		Z,.IRQ_checkPCMA2

	; stop sample on ADPCM-A channel 2
	ld		de,0x0082
	rst 	writeDEportB

.IRQ_checkPCMA2:
	ld		a,(intStatus1)
	bit		2,a
	jp		Z,.IRQ_checkPCMA3

	; stop sample on ADPCM-A channel 3
	ld		de,0x0084
	rst 	writeDEportB

.IRQ_checkPCMA3:
	ld		a,(intStatus1)
	bit		3,a
	jp		Z,.IRQ_checkPCMA4

	; stop sample on ADPCM-A channel 4
	ld		de,0x0088
	rst 	writeDEportB

.IRQ_checkPCMA4:
	ld		a,(intStatus1)
	bit		4,a
	jp		Z,.IRQ_checkPCMA5

	; stop sample on ADPCM-A channel 5
	ld		de,0x0090
	rst 	writeDEportB

.IRQ_checkPCMA5:
	ld		a,(intStatus1)
	bit		5,a
	jp		Z,.IRQ_afterPCMA

	; stop sample on ADPCM-A channel 6
	ld		de,0x00A0
	rst 	writeDEportB

.IRQ_afterPCMA:
	; update internal Status 0 register
	in		a, (4)
	ld		(intStatus0),a

	; check timers if needed here

.IRQ_end:
	; restore registers
	pop		iy
	pop		ix
	pop		hl
	pop		de
	pop		bc
	pop		af

	ei

	; return
	ret

;==============================================================================;
; EntryPoint
; The entry point for the sound driver.

EntryPoint:
	ld		sp,0xFFFC		; Set stack pointer ($FFFD-$FFFE is used for other purposes)
	im		1				; Set Interrupt Mode 1 (IRQ at $38)
	xor		a				; make value in A = 0

	; Clear RAM at $F800-$FFFF
	ld		(0xF800),a		; set $F800 = 0
	ld		hl,0xF800		; 00 value is at $F800
	ld		de,0xF801		; write sequence begins at $F801
	ld		bc,0x7FF		; end at $FFFF
	ldir					; clear out memory

	; Initialize variables
	ld		(loopB),a

	; Initialize SSG frequencies
	ld		(SSG_FreqFine),a
	ld		(SSG_FreqCoarse),a

	; Next APCM-A channel
	ld		(PCMnextChannel),a
	ld		(PCMcurrChannel),a

	; Silence/Stop SSG, FM, and ADPCM
	call	ssg_Stop
	call	fm_Stop

	; silence ADPCM-A, ADPCM-B
	;call	pcma_Stop
	call	pcmb_Stop

	;-------------------------------------------;
	; write 1 to port $C0
	ld		a,1
	out		(0xC0),a

	; continue setting up the hardware, etc.

	ld		de,0x2730		; Reset Timer flags, Disable Timer IRQs
	rst 	writeDEportA	; write to ports 4 and 5
	ld		de,0x1001		; Reset ADPCM-B
	rst 	writeDEportA	; write to ports 4 and 5
	ld		de,0x1C00		; Unmask ADPCM-A and B flag controls
	rst 	writeDEportA	; write to ports 4 and 5

	; Initialize more variables
	call	SetDefaultBanks	; Set default program banks

	; Set ADPCM-A shared volume
	ld		de,0x013F		; Set ADPCM-A volume to Maximum
	rst 	writeDEportB	; write to ports 6 and 7

	; set ADPCM-B volume
	ld		a,0xFF
	ld		(intPCMB_Volume),a		; ADPCM-B volume max
	ld		de,0x1BFF		; Set ADPCM-B volume to Maximum
	rst 	writeDEportA	; write to ports 4 and 5

	; set ADPCM-B output on Left and Right channels
	ld		a,0xC0
	ld		(intPCMB_LeftRight),a	; ADPCM-B left and right channels
	ld		de,0x11C0		; Set ADPCM-B Left/Right output
	rst 	writeDEportA	; write to ports 4 and 5

	; set default ADPCM-B sample rate
	ld		a,0xB5
	ld		(intPCMB_SampRateL),a	; Delta-N Sampling Rate (L)
	ld		de,0x19B5		; Set ADPCM-B Sampling Rate LSB
	rst 	writeDEportA	; write to ports 4 and 5

	ld		a,0x65
	ld		(intPCMB_SampRateH),a	; Delta-N Sampling Rate (H)
	ld		de,0x1A65		; Set ADPCM-B Sampling Rate LSB
	rst 	writeDEportA	; write to ports 4 and 5

	; Enable NMIs so we can start receiving commands
	ld		a,1
	out		(8),a			; Write to Port 8 (Enable NMI)
	ei

;------------------------------------------------------------------------------;
; MainLoop
; Nothing here; everything is handled via NMI and IRQ

MainLoop:
	jp		MainLoop

;==============================================================================;
; jmp_writeDEportA
; Writes data (from registers de) to port A (4 and 5, YM2610 A1 line=0)

jmp_writeDEportA:
	push	af
	ld		a,d
	out		(4),a			; write to port 4 (address 1)
	rst		waitYamaha		; wait for busy flag
	ld		a,e
	out		(5),a			; write to port 5 (data 1)
	rst		waitYamaha		; wait for busy flag
	pop		af
	ret

;------------------------------------------------------------------------------;
; jmp_writeDEportB
; Writes data (from registers de) to port B (6 and 7, YM2610 A1 line=1)

jmp_writeDEportB:
	push	af
	ld		a,d
	out		(6),a			; write to port 6 (address 2)
	rst		waitYamaha		; wait for busy flag
	ld		a,e
	out		(7),a			; write to port 7 (data 2)
	rst		waitYamaha		; wait for busy flag
	pop		af
	ret

;==============================================================================;
; cmdSwitchSlot
; Performs setup work for Command $01 (Slot Change).

cmdSwitchSlot:
	di
	xor		a
	out		(0xC),a			; write 0 to port 0xC (Respond to 68K)
	out		(0),a			; write to port 0 (Clear sound code)
	ld		sp,0xFFFC		; set stack pointer

	; call Switch Command
	ld		hl,executeSwitchSlot
	push	hl
	retn

;==============================================================================;
; cmdSoftReset
; Performs setup work for Command $03 (Soft Reset).

cmdSoftReset:
	di
	xor		a
	out		(0xC),a			; write 0 to port 0xC (Respond to 68K)
	out		(0),a			; write to port 0 (Clear sound code)
	ld		sp,0xFFFC		; set stack pointer

	; call Soft Reset Routine
	ld		hl,executeSoftReset
	push	hl
	retn

;==============================================================================;
; SetDefaultBanks
; Sets the default program banks. This setup treats the M1 ROM as linear space.

SetDefaultBanks:
	SetBank 0x1E,8			; Set $F000-$F7FF bank to bank $1E (30 *  2K)
	SetBank 0xE,9			; Set $E000-$EFFF bank to bank $0E (14 *  4K)
	SetBank 6,0xA			; Set $C000-$DFFF bank to bank $06 ( 6 *  8K)
	SetBank 2,0xB			; Set $8000-$BFFF bank to bank $02 ( 2 * 16K)
	ret

;==============================================================================;
; fm_Stop
; Stops all FM channels.

; "...if you're accessing the same address multiple times, you may write the
; address first and procceed to write the data register multiple times."
; - translated from YM2610 Application Manual, Section 9

fm_Stop:
	di
	push	af
	ld		a,0x28			; Slot and Key On/Off
	out		(4),a			; write to port 4 (address 1)
	rst		waitYamaha		; Write delay 1 (17 cycles)
	;---------------------------------------------------;
	ld		a,0x01			; FM Channel 1
	out		(5),a			; write to port 5 (data 1)
	rst		waitYamaha		; Write delay 2 (83 cycles)
	;---------------------------------------------------;
	ld		a,0x02			; FM Channel 2
	out		(5),a			; write to port 5 (data 1)
	rst		waitYamaha		; Write delay 2 (83 cycles)
	;---------------------------------------------------;
	ld		a,0x05			; FM Channel 3
	out		(5),a			; write to port 5 (data 1)
	rst		waitYamaha		; Write delay 2 (83 cycles)
	;---------------------------------------------------;
	ld		a,0x06			; FM Channel 4
	out		(5),a			; write to port 5 (data 1)
	rst		waitYamaha		; Write delay 2 (83 cycles)
	pop		af
	ret

;==============================================================================;
; ssg_Stop
; Silences and stops all SSG channels.

ssg_Stop:
	di
	ld		de,0x0800		; SSG Channel A Volume/Mode
	rst 	writeDEportA	; write to ports 4 and 5
	;-------------------------------------------------;
	ld		de,0x0900		; SSG Channel B Volume/Mode
	rst 	writeDEportA	; write to ports 4 and 5
	;-------------------------------------------------;
	ld		de,0x0A00		; SSG Channel C Volume/Mode
	rst 	writeDEportA	; write to ports 4 and 5
	;-------------------------------------------------;
	; Disable all SSG channels
	ld		de,0x073F		; Disable all SSG channels (Tone and Noise)
	rst 	writeDEportA	; write to ports 4 and 5
	ret

;==============================================================================;
; pcma_Stop
; Stops all ADPCM-A channels.

pcma_Stop:
	di
	ld		de,0x009F			; $009F		Dump all ADPCM-A channels (stop sound)
	rst 	writeDEportB

	ret

;==============================================================================;
; pcmb_Stop
; Stops the ADPCM-B channel.

pcmb_Stop:
	di
	ld		de,0x1001			; $1001		Force stop synthesis
	rst 	writeDEportA

	dec		e					; $1000		Stop ADPCM-B output
	rst 	writeDEportA
	ret

;==============================================================================;
; HandleCommand
; Handles any command that isn't already dealt with separately (e.g. $01, $03).

HandleCommand:
	ld		a,(curCommand)	; get current command
	ld		b,a				; save backup of command

	; commands that change ADPCM-B rate.
	; The check is handled in a very simple manner for this program.
	jp		M,command_ChangeRate

    cp		SOUNDCMD_SSGRampinit
	jp		Z,command_SSGrampInit

	cp		SOUNDCMD_SSGRampcycle
	jp		Z,command_SSGrampCycle

	cp		SOUNDCMD_SSGPulseStart
	jp		Z,command_SSG_pulseStart

	cp		SOUNDCMD_SSGPulseStop
	jp		Z,command_SSGpulseStop

    ; command $02: play Jingle ADPCM-A (channel 1)
	cp		SOUNDCMD_PlayJingleA
	jp		Z,command_PlayJingleA

    ; command play Coin ADPCM-A (channel 1)
	cp		SOUNDCMD_PlayCoinA
	jp		Z,command_PlayCoinA

    ; command play left ADPCM-A (channel 1)
	cp		SOUNDCMD_PlayLeft
	jp		Z,command_PlayLeftA

    ; command play right ADPCM-A (channel 1)
	cp		SOUNDCMD_PlayRight
	jp		Z,command_PlayRightA

    ; command play center ADPCM-A (channel 1)
	cp		SOUNDCMD_PlayCenter
	jp		Z,command_PlayCenterA

	; command to play ADPCM-B Center
	cp		SOUNDCMD_ADPCMB_Center
	jp		Z,command_PlayPCMBCenter

	; command to play ADPCM-B Left
	cp		SOUNDCMD_ADPCMB_Left
	jp		Z,command_PlayPCMBLeft

	; command to play ADPCM-B Right
	cp		SOUNDCMD_ADPCMB_Right
	jp		Z,command_PlayPCMBRight

	; command to stop ADPCM-A
	cp		SOUNDCMD_StopADPCMA
	jp		Z,pcma_Stop

	; command to stop ADPCM-B
	cp		SOUNDCMD_StopADPCMB
	jp		Z,pcmb_Stop

	; command loop ADPCM-B
	cp		SOUNDCMD_LoopB
	jp		Z,command_LoopPCMB

	; command no loop ADPCM-B
	cp		SOUNDCMD_NoLoopB
	jp		Z,command_NoLoopPCMB

	; if it's not one of the above, then do nothing.
	ret

;==============================================================================;
; system command holding cell.

;------------------------------------------------------------------------------;
; executeSwitchSlot
; Handles a slot switch.

executeSwitchSlot:
	di
	xor		a
	out		(0xC),a			; Write 0 to port 0xC (Reply to 68K)
	out		(0),a			; Reset sound code

	call	SetDefaultBanks	; initialize banks to default config

	; (FM) turn off Left/Right, AM Sense and PM Sense
	ld		de,0xB500		; $B500: turn off for channels 1/3
	rst 	writeDEportA
	rst 	writeDEportB
	ld		de,0xB600		; $B600: turn off for channels 2/4
	rst 	writeDEportA
	rst 	writeDEportB

	; (ADPCM-A, ADPCM-B) Reset ADPCM channels
	ld		de,0x00BF		; $00BF: ADPCM-A Dump=1, all channels=1
	rst 	writeDEportB
	ld		de,0x1001		; $1001: ADPCM-B Reset=1
	rst 	writeDEportA

	; (ADPCM-A, ADPCM-B) Poke ADPCM channel flags (write 1, then 0)
	ld		de,0x1CBF		; $1CBF: Reset flags for ADPCM-A 1-6 and ADPCM-B
	rst 	writeDEportA
	ld		de,0x1C00		; $1C00: Enable flags for ADPCM-A 1-6 and ADPCM-B
	rst 	writeDEportA

	; silence FM channels
	ld		de,0x2801		; FM channel 1 (1/4)
	rst 	writeDEportA
	ld		de,0x2802		; FM channel 2 (2/4)
	rst 	writeDEportA
	ld		de,0x2805		; FM channel 5 (3/4)
	rst 	writeDEportA
	ld		de,0x2806		; FM channel 6 (4/4)
	rst 	writeDEportA

	; silence SSG channels
	ld		de,0x800		;SSG Channel A
	rst 	writeDEportA
	ld		de,0x900		;SSG Channel B
	rst 	writeDEportA
	ld		de,0xA00		;SSG Channel C
	rst 	writeDEportA

	; set up infinite loop in RAM
	ld		hl,0xFFFD
	ld		(hl),0xC3		; Set 0xFFFD = 0xC3 ($C3 is opcode for "jp")
	ld		(0xFFFE),hl		; Set 0xFFFE = 0xFFFD (making "jp $FFFD")
	ld		a,1
	out		(0xC),a			; Write 1 to port 0xC (Reply to 68K)
	jp		0xFFFD			; jump to infinite loop in RAM

;------------------------------------------------------------------------------;
; executeSoftReset
; Handles a soft reset.

executeSoftReset:
	di
	ld		a,0
	out		(0xC),a			; Write to port 0xC (Reply to 68K)
	out		(0),a			; Reset sound code
	ld		sp,0xFFFF
	jp		Start			; Go back to the top.

;==============================================================================;
; command_PlayJingleA (command $02)
; Play Jingle sample on the first ADPCM-A channel.

command_PlayJingleA:
    ld      a,0
    ld      b,0xc0			; center, both channels
    call    PlayPCMA
    ret

;------------------------------------------------------------------------------;
; SOUNDCMD_PlayCoinA
; Play coin sample on the first ADPCM-A channel.

command_PlayCoinA:
    ld      a,1
    ld      b,0xc0			; center, both channels
    call    PlayPCMA
    ret

;------------------------------------------------------------------------------;
; SOUNDCMD_PlayLeftA
; Play coin sample on the first ADPCM-A channel.

command_PlayLeftA:
    ld      a,2
    ld      b,0x80			; left channel
    call    PlayPCMA
    ret

;------------------------------------------------------------------------------;
; SOUNDCMD_PlayRightA
; Play coin sample on the first ADPCM-A channel.

command_PlayRightA:
    ld      a,3
    ld      b,0x40			; right channel
    call    PlayPCMA
    ret

;------------------------------------------------------------------------------;
; SOUNDCMD_PlayCenterA
; Play coin sample on the first ADPCM-A channel.

command_PlayCenterA:
    ld      a,4
    ld      b,0xc0      ; center, both channels
    call    PlayPCMA
    ret

;------------------------------------------------------------------------------;
; PlayPCMA
; Play sample number 'a' on the next ADPCM-A channel.
; L/R balance in 'b'

PlayPCMA:
	di
    ; load sample using 'a'
	call	loadSample			; load sample addres from a to ix from table

	; check for next available channel
	ld		a,(PCMnextChannel)
	ld		(PCMcurrChannel),a
	inc		a
	cp		6
	jr		nz,.storeNextChannel
	xor		a					; zero 'a'

.storeNextChannel:
	ld		(PCMnextChannel),a

	; volume/output at 1F
    ; set l/r panning with 'b'
	ld		a,(PCMcurrChannel)
	add		0x08				; Base register for Volume and Pan
    ld      d,a					; register for current channel
    ld      a,0x1F
    or      b					; left right panning in b
    ld      e,a
	rst 	writeDEportB

	; start addr (register $10+Channel,$18+Channel)
	; samples_PCMA word 1
	ld		a,(PCMcurrChannel)
	add		0x10				; Base register for start Addr byte 1
	ld		d,a
	ld		e,(ix)
	rst 	writeDEportB

	ld		a,(PCMcurrChannel)
	add		0x18				; Base register for start Addr byte 2
	ld		d,a
	ld		e,(ix+1)
	rst 	writeDEportB

	; end addr (register $20+Channel,$28+Channel)
	; samples_PCMA word 2
	ld		a,(PCMcurrChannel)
	add		0x20				; Base register for end Addr byte 1
	ld		d,a
	ld		e,(ix+2)
	rst 	writeDEportB

	ld		a,(PCMcurrChannel)
	add		0x28				; Base register for end Addr byte 2
	ld		d,a
	ld		e,(ix+3)
	rst 	writeDEportB

	; store in 'b' the current bit mask for channel
	ld		b,0x01				; set base bitmask
	ld		a,(PCMcurrChannel)	
	cp		0					; check if channel is zero
	jr		z,.reset_enable		; if so, skip to execution

	; create bitmask for channel
.loopshifts
	sla		b
	dec		a
	jr		nz,.loopshifts

	; reset and enable using bitmask from 'b'
.reset_enable:
	ld		a,b					; copy bitmask
	ld		d,0x1C
	ld		e,a
	rst 	writeDEportA
	ld		de,0x1C00
	rst 	writeDEportA

	; play sound on selected channel using mask in a
	ld		d,0x00
	ld		e,a
	rst 	writeDEportB

	ret

;------------------------------------------------------------------------------;
; helper to load sample entry from the table
; sample entry is in 'a'

loadSample:
	; multiply 'a' (sample entry) by 4
	ld		l,a	 
	ld		h,0
	add 	hl,hl
	add 	hl,hl
	ld		de,samples_PCMA ; add the sample table base addr
	add 	hl, de
	push	hl				; use the stack to make the copy
	pop 	ix

	ret

;------------------------------------------------------------------------------;
; command_PlayPCMBCenter 
; Set both channels
; then ADPCM-B sample.

command_PlayPCMBCenter:
	ld		b,0xC0
	call	command_PlayPCMB
	ret

;------------------------------------------------------------------------------;
; command_PlayPCMBLeft
; Set left channel
; then ADPCM-B sample.

command_PlayPCMBLeft:
	ld		b,0x80
	call	command_PlayPCMB
	ret

;------------------------------------------------------------------------------;
; command_PlayPCMBRight
; Set right channel
; then ADPCM-B sample.

command_PlayPCMBRight:
	ld		b,0x40
	call	command_PlayPCMB
	ret

;------------------------------------------------------------------------------;
; command_PlayPCMBRight
; Play an ADPCM-B sample, takes panning from 'b'

command_PlayPCMB:
	di

	; don't bother trying to play back ADPCM-B on Neo-Geo CD; it won't work.
	ifnd TARGET_CD

	; reset and enable
	ld		de,0x1C80
	rst 	writeDEportA
	ld		de,0x1C00
	rst 	writeDEportA
	ld		de,0x1000
	rst 	writeDEportA

	; --left/right ($11)--
	ld		d,0x11
	ld		e,b				; panning from 'b'
	rst 	writeDEportA

	; --volume ($1B)--
	ld		de,0x1BFF
	rst 	writeDEportA

	; --start addr ($12,$13)--
	; samples_PCMB word 1 (low first, then high)
	ld		ix,samples_PCMB
	ld		d,0x12
	ld		a,(ix)
	ld		e,a
	rst 	writeDEportA
	ld		d,0x13
	ld		a,(ix+1)
	ld		e,a
	rst 	writeDEportA

	; --end addr ($14,$15)--
	; samples_PCMB word 2 (low first, then high)
	ld		d,0x14
	ld		a,(ix+2)
	ld		e,a
	rst 	writeDEportA
	ld		d,0x15
	ld		a,(ix+3)
	ld		e,a
	rst 	writeDEportA

	; --delta-n ($19,$1A)--
	ld		d,0x19
	ld		a,(intPCMB_SampRateL)
	ld		e,a
	rst 	writeDEportA
	ld		d,0x1A
	ld		a,(intPCMB_SampRateH)
	ld		e,a
	rst 	writeDEportA

	ld		b,0x80				; set start bit
	call	setADPCMBLoop
	endif

	ret

;------------------------------------------------------------------------------;
; Sets looping register values for ADPCM-B sample.
; takes start bit form 'b'

setADPCMBLoop:
	; --main control ($10)--
	; %SxxLxxxR (S=start, L=loop, R=Reset)
	; repeat/loop flag = loopB<<4
	ld		a,(loopB)
	sla		a
	sla		a
	sla		a
	sla		a
	or		b
	ld		d,0x10
	ld		e,a
	rst 	writeDEportA

	ret

;------------------------------------------------------------------------------;
; command_LoopPCMB 
; Sets looping for ADPCM-B sample.

command_LoopPCMB:
	ld		a,1
	ld		(loopB),a
	ld		b,0
	call	setADPCMBLoop
	ret

;------------------------------------------------------------------------------;
; command_NoLoopPCMB 
; Disables looping for ADPCM-B sample.

command_NoLoopPCMB:
	xor		a
	ld		(loopB),a
	ld		b,0
	call	setADPCMBLoop
	ret

;------------------------------------------------------------------------------;
; Sample Rates in this demo:
; 11025 | delta-n: 0x32DA (13018) | rate: 1.00x | $80 | (native rate (encoded))
; 16538 | delta-n: 0x4C49 (19529) | rate: ~1.5x | $81 |
; 22050 | delta-n: 0x65B5 (26037) | rate: 2.00x | $82 |
; 27563 | delta-n: 0x7F23 (32547) | rate: ~2.5x | $83 |
; 33075 | delta-n: 0x9890 (39056) | rate: 3.00x | $84 |
; 38588 | delta-n: 0xB1FE (45566) | rate: ~3.5x | $85 |
; 44100 | delta-n: 0xCB6B (52075) | rate: 4.00x | $86 | (original sample's rate)
; 55125 | delta-n: 0xFE45 (65093) | rate: 5.00x | $87 |

; low byte (port 0x19)
tbl_RatesPCMB_L:
	db 0xDA
	db 0x49
	db 0xB5
	db 0x23
	db 0x90
	db 0xFE
	db 0x6B
	db 0x45

; high byte (port 0x1A)
tbl_RatesPCMB_H:
	db 0x32
	db 0x4C
	db 0x65
	db 0x7F
	db 0x98
	db 0xB1
	db 0xCB
	db 0xFE

; command_ChangeRate (commands $80-$87)
; Changes rate of ADPCM-B sample.

command_ChangeRate:
	; mask command number w/0x07 to get new rate index
	and		0x07
	; index into tbl_RatesPCMB_L and tbl_RatesPCMB_H
	ld		c,a
	xor		a
	ld		b,a

	; set intPCMB_SampRateL
	ld		hl,tbl_RatesPCMB_L
	add		hl,bc
	ld		a,(hl)
	ld		(intPCMB_SampRateL),a

	; set intPCMB_SampRateH
	ld		hl,tbl_RatesPCMB_H
	add		hl,bc
	ld		a,(hl)
	ld		(intPCMB_SampRateH),a

	; change sample rate as it's playing, too, just in case
	ld		d,0x19
	ld		a,(intPCMB_SampRateL)
	ld		e,a
	rst 	writeDEportA
	ld		d,0x1A
	ld		a,(intPCMB_SampRateH)
	ld		e,a
	rst 	writeDEportA

	ret

; initialize variables for a ramp sweep
; and stop andy SSG sound
command_SSGrampInit:
	call	ssg_Stop

	ld		a,0
	ld		(SSG_FreqFine),a
	ld		(SSG_FreqCoarse),a
	ret

command_SSGrampCycle:
	ld		d,0x00
	ld		a,(SSG_FreqFine)
	ld		e,a
	rst 	writeDEportA

	ld		d,0x01			;SSG Channel A Coarse Tune
	ld		a,(SSG_FreqCoarse)
	ld		e,a		
	rst 	writeDEportA

	ld		de,0x073E		;SSG Mixing, Only channel A
	rst 	writeDEportA

	ld		de,0x080F		;SSG Channel A Volume
	rst 	writeDEportA

	; Advance to next frequency
	ld		a,(SSG_FreqFine)
	inc 	a
	ld		(SSG_FreqFine),a
	cp		0x00
	jp		nz,.finish_ssg

	; Advance to next Coarse frequency
	ld		a,(SSG_FreqCoarse)
	inc 	a
	cp		0x10
	jp		nz,.update_coarse_ssg
	ld		a,0
.update_coarse_ssg:
	ld		(SSG_FreqCoarse),a

.finish_ssg:
	ret

command_SSG_pulseStart:
	ld		de,0x0011		; 8khz (8294hz)
	rst 	writeDEportA

	ld		de,0x0100		; Coarse to zero
	rst 	writeDEportA

	ld		de,0x073E		;SSG Mixing, Only channel A
	rst 	writeDEportA

	ld		de,0x080F		;SSG Channel A Volume
	rst 	writeDEportA

	ret

command_SSGpulseStop:
	ld		de,0x073F		;SSG Mixing, Silence A
	rst 	writeDEportA

	ld		de,0x0800		;SSG Channel A Volume
	rst 	writeDEportA

	ret

;==============================================================================;
; samples
	ifd TARGET_CD
		include "samples_cd.inc"
	else
		include "samples_cart.inc"
	endif

;==============================================================================;
; Z80 ram
	include "soundram.inc"

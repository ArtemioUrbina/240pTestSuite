; 240p Test Suite Sound Driver
; Based on freem ADPCM example from: http://ajworld.net/neogeodev/
; Modifications made by Artemio Urbina for the 240p Test Suite and MDFourier
;==============================================================================;
; defines
	include "sysmacro.inc"
	include "soundcmd.inc" ; shared include with 68K

; Defines for RST usage, in case the locations change later.
writeDEportA	= $08
writeDEportB	= $18
waitYamaha		= $28
;------------------------------------------------------------------------------;
	org $0000
Start:
	di
	jp		EntryPoint
;==============================================================================;
;								  RST
;==============================================================================;

;--------------------------------------------------------------------------;
	org $0008
;--------------------------------------------------------------------------;
; writeDEportA
;--------------------------------------------------------------------------;
; Writes data (from registers de) to port A (4 and 5, YM2610 A1 line=0)
	push	af
	ld		a,d
	out		(4),a			; write to port 4 (address 1)
	rst		waitYamaha		; wait for busy flag
	ld		a,e
	out		(5),a			; write to port 5 (data 1)
	rst		waitYamaha		; wait for busy flag
	pop		af
	ret

;--------------------------------------------------------------------------;
	org $0018
;--------------------------------------------------------------------------;
; writeDEportB
;--------------------------------------------------------------------------;
; Writes data (from registers de) to port B (6 and 7, YM2610 A1 line=1)
	push	af
	ld		a,d
	out		(6),a			; write to port 6 (address 2)
	rst		waitYamaha		; wait for busy flag
	ld		a,e
	out		(7),a			; write to port 7 (data 2)
	rst		waitYamaha		; wait for busy flag
	pop		af
	ret

;--------------------------------------------------------------------------;
	org $0028
;--------------------------------------------------------------------------;
; waitYamaha if Busy
;--------------------------------------------------------------------------;
; Keep checking the busy flag in Status 0 until it's clear.
checkLoop:
	in		a,(4)			; read Status 0 (busy flag in bit 7)
	add		a
	jr		C,checkLoop
	ret

;==============================================================================;
	org $0038
;==============================================================================;
; IRQ 
;==============================================================================;
; Handle an interrupt request from the YM2610 Timer
; unused for nowk
IRQ:
	reti

;==============================================================================;
;							  NMI
;==============================================================================;
	org $0066
;--------------------------------------------------------------------------;
; NMI
;--------------------------------------------------------------------------;
; Inter-processor communications.
; In this driver, the NMI gets the command from the 68K and interprets it.

NMI:
	di						; NMI di

	exx
	ex		af,af'

	in		a,(0)			; Acknowledge NMI, get command from 68K via Port 0
	ld		b,a				; make a copy
	ld		a,0xff			; set processing code
	out		(0xC),a			; Write to port 0 (Processing sound code)
	ld		a,b				; set a for comparisons
	ld		(curCommand),a	; update curCommand

	or		a				; check if Command is 0
	jp		Z,endNMI		; exit if Command 0

	call	HandleCommand
	
	ld		a,(curCommand)
	set		7,a				; set bit 7 for reply, this is OK reply
	out		(0xC),a			; Reply to 68K
	xor		a				; clear a for response.
	out		(0),a			; Write to port 0 (Clear sound code)

endNMI:
	ex		af,af'
	exx

	ei						; end NMI di
	retn


;==============================================================================;
; EntryPoint
;==============================================================================;
; The entry point for the sound driver.

EntryPoint:
	ld		sp,0xFFFC		; Set stack pointer
	im		1				; Set Interrupt Mode 1 (IRQ at $38)
	xor		a				; make value in A = 0

	; Clear RAM at $F800-$FFFF
	ld		(0xF800),a		; set $F800 = 0
	ld		hl,0xF800		; 00 value is at $F800
	ld		de,0xF801		; write sequence begins at $F801
	ld		bc,0x7FF		; end at $FFFF
	ldir					; clear out memory

	;-------------------------------------------;
	; Initialize variables
	; although they are 00 from above...

	; Initialize SSG frequencies
	ld		(SSG_FreqCoarse),a
	ld		(SSG_FreqFine),a

	; APCM-A 
	ld		(PCMnextChannel),a
	ld		(PCMcurrChannel),a

	; ADPCM-B
	ld		(intPCMB_currSample),a
	ld		(intPCMB_Loop),a

	; Silence/Stop SSG, FM, and ADPCM
	call	ssg_Stop
	call	fm_Stop

	; silence ADPCM-A, ADPCM-B
	;call	command_PCMAStop	; Commented so that the coin plays
	call	command_PCMBStop

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

	;-------------------------------------------;
	; Set ADPCM-A shared volume
	ld		de,0x013F		; Set ADPCM-A volume to Maximum
	rst 	writeDEportB	; write to ports 6 and 7

	;-------------------------------------------;
	; set ADPCM-B volume
	ld		a,0xFF			
	ld		(intPCMB_Volume),a		
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

	;-------------------------------------------;
	; Enable NMIs so we can start receiving commands
	ld		a,1
	out		(8),a			; Write to Port 8 (Enable NMI)
	ei						; was set at Startup

;==============================================================================;
; MainLoop
;==============================================================================;
; Nothing here; everything is handled via NMI and IRQ

MainLoop:
	jp		MainLoop

;------------------------------------------------------------------------------;
; helper from entry SetDefaultBanks
; Sets the default program banks. This setup treats the M1 ROM as linear space.

SetDefaultBanks:
	SetBank 0x1E,8			; Set $F000-$F7FF bank to bank $1E (30 *  2K)
	SetBank 0xE,9			; Set $E000-$EFFF bank to bank $0E (14 *  4K)
	SetBank 6,0xA			; Set $C000-$DFFF bank to bank $06 ( 6 *  8K)
	SetBank 2,0xB			; Set $8000-$BFFF bank to bank $02 ( 2 * 16K)
	ret

;==============================================================================;
; HandleCommand
;==============================================================================;
; Handles any command that isn't already dealt with separately (e.g. $01, $03).

HandleCommand:
	ld		a,(curCommand)	; get current command

	; commands that change ADPCM-B rate.
	; The check is handled in a very simple manner for this program.
	jp		M,command_ChangeRate

	; execute the rest via jump table, will crash if command > 0x6F
	; is called, but it has flat execution time
	ld		h,0
	ld		l,a
	add		hl,hl
	ld		de,CommandTbl
	add		hl,de

	ld		a,(hl)
	inc		hl
	ld		h,(hl)
	ld		l,a
	jp		(hl)

;==============================================================================;
; COMMANDS
;==============================================================================;

;------------------------------------------------------------------------------;
; command_SwitchSlot
;------------------------------------------------------------------------------;
; Performs setup work for Command $01 (Slot Change).

command_SwitchSlot:
	xor		a
	out		(0xC),a			; write 0 to port 0xC (Respond to 68K)
	out		(0),a			; write to port 0 (Clear sound code)
	ld		sp,0xFFFC		; set stack pointer

	; call Switch Command
	ld		hl,executeSwitchSlot
	push	hl
	retn

;------------------------------------------------------------------------------;
; command_SoftReset
;------------------------------------------------------------------------------;
; Performs setup work for Command $03 (Soft Reset).

command_SoftReset:
	xor		a
	out		(0xC),a			; write 0 to port 0xC (Respond to 68K)
	out		(0),a			; write to port 0 (Clear sound code)
	ld		sp,0xFFFC		; set stack pointer

	; call Soft Reset Routine
	ld		hl,executeSoftReset
	push	hl
	retn

;------------------------------------------------------------------------------;
; executeSwitchSlot
; Handles a slot switch.

executeSwitchSlot:
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
	ld		a,0
	out		(0xC),a			; Write to port 0xC (Reply to 68K)
	out		(0),a			; Reset sound code
	ld		sp,0xFFFF
	jp		Start			; Go back to the top.

;------------------------------------------------------------------------------;
; command_RAMTest
;------------------------------------------------------------------------------;
; RAM Test handler

command_RAMTest:
	call	execute_RAMTest
	jr		nc,command_null			; if no error, return normally

	; if a RAM error was found...
	pop		hl						; remove return from stack, yeah I know...
	ld		a,(curCommand)
	; Reply to 68K with command as it was sent, this indicates an error
	out		(0xC),a
	xor		a						; clear a for now.
	out		(0),a					; Write to port 0 (Clear sound code)
	jp		endNMI					; finish NMI

;------------------------------------------------------------------------------;
; command_null
;------------------------------------------------------------------------------;
; place holder for unexistant commands, and return for RAM test

command_null:
	ret

;==============================================================================;
; FM COMMANDS
;==============================================================================;

;------------------------------------------------------------------------------;
; fm_Stop
;------------------------------------------------------------------------------;
; Stops all FM channels.

; "...if you're accessing the same address multiple times, you may write the
; address first and procceed to write the data register multiple times."
; - translated from YM2610 Application Manual, Section 9

fm_Stop:
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
; SSG COMMANDS
;==============================================================================;

; a value of 0x0003 (coarse/fine) is ~63300khz
; 0x0006 is ~25780khz
; 0x0007 is ~21090khz
; 0x0001 and 0x0002 can't be measured witha 192khz smapling rate
; lowest frequency is around 30hz 0x0fff
; All these values were measured on an NTSC AES via headphone out

;------------------------------------------------------------------------------;
; command_SSGrampInit using channel A
;------------------------------------------------------------------------------;
; initialize variables for a ramp sweep
; and stop any SSG sound

command_SSGrampInit:
	call	ssg_Stop

	xor		a
	ld		(SSG_FreqCoarse),a
	ld		(SSG_FreqFine),a

	ret

;------------------------------------------------------------------------------;
; command_SSGrampCycle
;------------------------------------------------------------------------------;
; Does a cycle in the Ramp sweep for SSG using channel A

command_SSGrampCycle:
	ld		d,0x01			;SSG Channel A Coarse Tune
	ld		a,(SSG_FreqCoarse)
	ld		e,a				; coarse freq
	rst 	writeDEportA

	ld		d,0x00			;SSG Channel A Fine Tune
	ld		a,(SSG_FreqFine)
	ld		e,a				; fine freq
	rst 	writeDEportA

	ld		de,0x080F		;SSG Channel A Volume
	rst 	writeDEportA

	; Advance to next frequency
	ld		a,(SSG_FreqFine)
	inc 	a
	ld		(SSG_FreqFine),a
	cp		0x00
	jr		nz,.finish_ssg

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

;------------------------------------------------------------------------------;
; command_SSG_pulseStart using Channel A
;------------------------------------------------------------------------------;
; Starts an 8390hz tone in Channel A, intended for sync pulses linked to vsync
; in MDFourier

command_SSG_pulseStart:
	ld		de,0x0100		; Coarse to zero
	rst 	writeDEportA

	ld		de,0x0010		; ~8khz (8390hz)
	rst 	writeDEportA

	ld		de,0x080F		; SSG Channel A Volume
	rst 	writeDEportA

	ret

;------------------------------------------------------------------------------;
; command_SSG_1khzStart using Channel A
;------------------------------------------------------------------------------;
; Starts an 999.2hz tone in Channel A, intended for confirmation beeps

command_SSG_1khzStart:
	ld		de,0x0100		; Coarse to zero
	rst 	writeDEportA

	ld		de,0x007F		; ~1khz (999.2hz)
	rst 	writeDEportA

	ld		de,0x080F		; SSG Channel A Volume
	rst 	writeDEportA

	ret

;------------------------------------------------------------------------------;
; command_SSG_260hzStart using Channel A
;------------------------------------------------------------------------------;
; Starts an 260.1hz tone in Channel A, intended for missed key presses in a test

command_SSG_260hzStart:
	ld		de,0x0101		; Coarse to 1
	rst 	writeDEportA

	ld		de,0x00e5		; ~ 260hz (260.1hz)
	rst 	writeDEportA

	ld		de,0x080F		; SSG Channel A Volume
	rst 	writeDEportA

	ret

;------------------------------------------------------------------------------;
; command_SSGpulseStop using Channel A
;------------------------------------------------------------------------------;
; Stops any of the above in Channel A

command_SSGpulseStop:
	ld		de,0x0800		; SSG Channel A Volume
	rst 	writeDEportA

	ret

;------------------------------------------------------------------------------;
; ssg_Stop (All channels)
;------------------------------------------------------------------------------;
; Silences and stops all SSG channels.

ssg_Stop:
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
	ld		de,0x071C		; SSG Mixing, Tone A,B, Noise C
	rst 	writeDEportA	; write to ports 4 and 5

	ret


;==============================================================================;
; ADPCM-A COMMANDS
;==============================================================================;

;------------------------------------------------------------------------------;
; command_PlayJingleA (command $02)
;------------------------------------------------------------------------------;
; Play Jingle sample on the first ADPCM-A channel.

command_PlayJingleA:
	ld		a,0				; sample 0
	ld		b,0xc0			; center, both channels
	call	PlayPCMA
	ret

;------------------------------------------------------------------------------;
; SOUNDCMD_PlayCoinA
;------------------------------------------------------------------------------;
; Play coin sample on the first ADPCM-A channel.

command_PlayCoinA:
	ld		a,1				; sample 1
	ld		b,0xc0			; center, both channels
	call	PlayPCMA
	ret

;------------------------------------------------------------------------------;
; SOUNDCMD_PlayLeftA
;------------------------------------------------------------------------------;
; Play coin sample on the first ADPCM-A channel.

command_PlayLeftA:
	ld		a,2				; sample 2
	ld		b,0x80			; left channel
	call	PlayPCMA
	ret

;------------------------------------------------------------------------------;
; SOUNDCMD_PlayRightA
;------------------------------------------------------------------------------;
; Play coin sample on the first ADPCM-A channel.

command_PlayRightA:
	ld		a,3				; sample 3
	ld		b,0x40			; right channel
	call	PlayPCMA
	ret

;------------------------------------------------------------------------------;
; SOUNDCMD_PlayCenterA
;------------------------------------------------------------------------------;
; Play coin sample on the first ADPCM-A channel.

command_PlayCenterA:
	ld		a,4				; sample 4
	ld		b,0xc0			; center, both channels
	call	PlayPCMA
	ret

;------------------------------------------------------------------------------;
; PlayPCMA
;------------------------------------------------------------------------------;
; Play sample number 'a' on the next ADPCM-A channel.
; L/R balance in 'b'

PlayPCMA:
	; load sample using 'a'
	call	loadSampleADPCMA	; load sample addres from a to ix from table

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
	ld		d,a					; register for current channel
	ld		a,0x1F
	or		b					; left right panning in b
	ld		e,a
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
; helper to load ADPCM-A sample entry from the table
;------------------------------------------------------------------------------;
; sample entry is in 'a'

loadSampleADPCMA:
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
; command_PCMAStop
;------------------------------------------------------------------------------;
; Stops all ADPCM-A channels.

command_PCMAStop:
	ld		de,0x00BF			; $00BF	Dump all ADPCM-A channels (stop sound)
	rst 	writeDEportB
	ret

;==============================================================================;
; ADPCM-B COMMANDS
;==============================================================================;

;------------------------------------------------------------------------------;
; command_setADPCMB_Sample0 
;------------------------------------------------------------------------------;
; Load sample 0 to ADPCM-B RAM id
; then ADPCM-B sample.

command_setADPCMB_Sample0:
	ld		a,0
	ld		(intPCMB_currSample),a
	ret

;------------------------------------------------------------------------------;
; command_setADPCMB_Sample1
;------------------------------------------------------------------------------;
; Load sample 1 to ADPCM-B RAM id
; then ADPCM-B sample.

command_setADPCMB_Sample1:
	ld		a,1
	ld		(intPCMB_currSample),a
	ret

;------------------------------------------------------------------------------;
; command_PlayPCMBCenter 
;------------------------------------------------------------------------------;
; Set both channels
; then ADPCM-B sample.

command_PlayPCMBCenter:
	ld		a,0xC0
	ld		(intPCMB_LeftRight),a
	call	play_PCMB_RAM_SR
	ret

;------------------------------------------------------------------------------;
; command_PlayPCMBLeft
;------------------------------------------------------------------------------;
; Set left channel
; then ADPCM-B sample.

command_PlayPCMBLeft:
	ld		a,0x80
	ld		(intPCMB_LeftRight),a
	call	play_PCMB_RAM_SR
	ret

;------------------------------------------------------------------------------;
; command_PlayPCMBRight
;------------------------------------------------------------------------------;
; Set right channel
; then ADPCM-B sample.

command_PlayPCMBRight:
	ld		a,0x40
	ld		(intPCMB_LeftRight),a
	call	play_PCMB_RAM_SR
	ret

;------------------------------------------------------------------------------;
; play_PCMB_Native_SR
;------------------------------------------------------------------------------;
; Plays ADPCM-B at native (table defined) SR

play_PCMB_Native_SR:
	call	loadPCMB
	call	loadRateAPCMB
	call	sendPlayPCMB
	ret

;------------------------------------------------------------------------------;
; play_PCMB_RAM_SR
;------------------------------------------------------------------------------;
; Plays ADPCM-B at RAM defined SR (intPCMB_SampRateL and intPCMB_SampRateH)
; Hard coded to sample 0 for now

play_PCMB_RAM_SR:
	call	loadPCMB
	call	loadCustomSamplerate
	call	sendPlayPCMB
	ret

;------------------------------------------------------------------------------;
; PlayPCMB 
;------------------------------------------------------------------------------;
; Play an ADPCM-B sample degined in 'a', takes panning from intPCMB_LeftRight

loadPCMB:
	call	loadSampleAPCMB
	call	command_PCMBStop

	; --left/right ($11)--
	ld		d,0x11
	ld		a,(intPCMB_LeftRight)
	ld		e,a				
	rst 	writeDEportA

	; --volume ($1B)--
	ld		de,0x1BFF
	rst 	writeDEportA

	; --start addr ($12,$13)--
	; samples_PCMB word 1 (low first, then high)
	;ld		ix,samples_PCMB
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

	ret

;------------------------------------------------------------------------------;
; Load ADPCM-B sample entry from the table
;------------------------------------------------------------------------------;
; sample entry is in 'a'

loadSampleAPCMB:
	; Comment just these parts for NGCD so that execution times are identical
	ifnd TARGET_CD

	; multiply 'a' (sample entry) by 4
	ld		a,(intPCMB_currSample)
	ld		l,a	 
	ld		h,0
	add 	hl,hl
	add 	hl,hl
	ld		de,samples_PCMB ; add the sample table base addr
	add 	hl, de
	push	hl				; use the stack to make the copy
	pop 	ix

	endif
	ret

;------------------------------------------------------------------------------;
; helper to load ADPCM-B Sample Rate entry from the table
;------------------------------------------------------------------------------;
; sample entry is in 'a'

loadRateAPCMB:
	; Comment just these parts for NGCD so that execution times are identical
	ifnd TARGET_CD

	; multiply 'a' (sample entry) by 4
	ld		l,a	 
	ld		h,0
	add 	hl,hl
	add 	hl,hl
	ld		de,rates_PCMB	; add the sample table base addr
	add 	hl, de
	push	hl				; use the stack to make the copy
	pop 	ix

	endif
	ret

;------------------------------------------------------------------------------;
; loadCustomSamplerate
;------------------------------------------------------------------------------;
; Sends the RAM defined Sample rat eto the YM2610

loadCustomSamplerate:
	; --delta-n ($19,$1A)--
	ld		d,0x19
	ld		a,(intPCMB_SampRateL)
	ld		e,a
	rst 	writeDEportA
	ld		d,0x1A
	ld		a,(intPCMB_SampRateH)
	ld		e,a
	rst 	writeDEportA

	ret

;------------------------------------------------------------------------------;
; sendPlayPCMB 
;------------------------------------------------------------------------------;
; Sends playback and loop flags to YM2610

sendPlayPCMB:
	; --main control ($10)--
	; %SxxLxxxR (S=start, L=loop, R=Reset)
	; repeat/loop flag = loopB<<4
	ld		a,(intPCMB_Loop)
	sla		a
	sla		a
	sla		a
	sla		a
	or		0x80
	ld		d,0x10
	ld		e,a
	rst 	writeDEportA

	ret

;------------------------------------------------------------------------------;
; command_LoopPCMB 
;------------------------------------------------------------------------------;
; Sets looping for ADPCM-B sample.

command_LoopPCMB:
	ld		a,1
	ld		(intPCMB_Loop),a
	call	command_PCMBStop
	ret

;------------------------------------------------------------------------------;
; command_NoLoopPCMB 
;------------------------------------------------------------------------------;
; Disables looping for ADPCM-B sample.

command_NoLoopPCMB:
	xor		a
	ld		(intPCMB_Loop),a
	call	command_PCMBStop
	ret

;------------------------------------------------------------------------------;
; Sample Rates used in ADPCM-B
;------------------------------------------------------------------------------;
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

;------------------------------------------------------------------------------;
; command_ChangeRate (commands $80-$87)
;------------------------------------------------------------------------------;
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

;------------------------------------------------------------------------------;
; command_PCMBStop
;------------------------------------------------------------------------------;
; Stops the ADPCM-B channel.

command_PCMBStop:
	ld		de,0x1001			; $1001		Force stop synthesis
	rst 	writeDEportA

	dec		e					; $1000		Stop ADPCM-B output
	rst 	writeDEportA
	ret

;==============================================================================;
; Z80 RAM Test
;==============================================================================;

; RAM is at $F800-$FFFF
; but we need the stack with the current code
; so we test from the end of our RAM variables to the SP
; the rest of the RAM was tested by use, we wouldn't be
; running this code without it working...
; I know...

execute_RAMTest:
	; Fill with 0 and test
	xor 	a
	call	.fillcmp
	ret 	c			; exit if error

	; fill with ff and test
	ld		a,0xff
	call	.fillcmp
	ret 	c			; exit if error

	; fill with aa and test
	ld		a,0xaa
	call	.fillcmp
	ret 	c			; exit if error

	; fill with 55 and test
	ld		a,0x55
	call	.fillcmp
	ret 	c			; exit if error

	; walking bit test
.walking:
	; Figure out the unused RAM size, SP - usedRAM
	ld		hl,0			; prepare to duplicate SP
	add		hl,sp			; copy SP
	ld		bc,usedRAM		; prepare for the substraction
	and		a				; erase carry
	sbc		hl,bc			; substract sp-usedRAM
	ld		b,h				; copy size to bc
	ld		c,l
	ld		hl,usedRAM		; get started at usedRAM

.wlklp1:
	ld		a,0x80		; 10000000

.wlklp2:
	ld		(hl),a		; store in RAM
	cp		(hl)		; read it back
	scf 				; set carry in case of error
	ret 	nz			; return if error
	rrca				; rotate one bit to the right
	cp		0x80		
	jr		nz,.wlklp2	; compare to initial pattern for a loop
	ld		(hl),0		; clear
	inc 	hl
	dec 	bc			; decrement and test 16 bit counter
	ld		a,b
	or		c
	jr		nz,.wlklp1	; continue until done
	ret 				; no errors ("or c" clears carry)

	; Fill memory and test
	; a  = test value
	; hl = base address
	; bc = size of area in bytes
	; if no errors carry is 0
	; else
	;	carry is 1
	;	hl = Address of error
	;	we will return just success or fail to the Neo Geo 68k
.fillcmp:
	ld		(usedRAM),a		; set usedRAM = value
	
	ld		de,usedRAM		; prepare for block copy
	inc		de

	; Figure out the unused RAM size, SP - usedRAM
	ld		hl,0			; prepare to duplicate SP
	add		hl,sp			; copy SP
	ld		bc,usedRAM		; prepare for the substraction
	and		a				; erase carry
	sbc		hl,bc			; substract sp-usedRAM
	ld		b,h				; copy size to bc
	ld		c,l
	dec		bc				; decrement it by one, since we start at usedRAM + 1

	ld		hl,usedRAM		; 00 value is at usedRAM, our source
	ldir					; Fill out memory

	; compare a filled memory block with value
.compare:
	; Figure out the unused RAM size, SP - usedRAM
	ld		hl,0			; prepare to duplicate SP
	add		hl,sp			; copy SP
	ld		bc,usedRAM		; prepare for the substraction
	and		a				; erase carry
	sbc		hl,bc			; substract sp-usedRAM
	ld		b,h				; copy size to bc
	ld		c,l
	dec		bc				; decrement it by one, since we start at usedRAM + 1
	ld		hl,usedRAM		; 00 value is at usedRAM, our source

.cmploop:
	cpi
	jr		nz,.cmp_err		; jump if not equal
	jp		pe,.cmploop		; continue

	; no errors, clear carry
	or		a				; clear carry, no errors
	ret						; return from .fillcmp

	; error, set carry
.cmp_err:
	scf						; set carry, error
	ret						; report with error

CommandTbl:
	dw		command_null				; 0x00
	dw		command_SwitchSlot			; 0x01
	dw		command_PlayJingleA			; 0x02
	dw		command_SoftReset			; 0x03
	dw		command_null				; 0x04
	dw		command_null				; 0x05
	dw		command_null				; 0x06
	dw		command_null				; 0x07
	dw		command_null				; 0x08
	dw		command_null				; 0x09
	dw		command_null				; 0x0A
	dw		command_null				; 0x0B
	dw		command_null				; 0x0C
	dw		command_null				; 0x0D
	dw		command_null				; 0x0E
	dw		command_null				; 0x0F

	dw		command_RAMTest				; 0x10
	dw		command_null				; 0x11
	dw		command_null				; 0x12
	dw		command_null				; 0x13
	dw		command_null				; 0x14
	dw		command_null				; 0x15
	dw		command_null				; 0x16
	dw		command_null				; 0x17
	dw		command_null				; 0x18
	dw		command_null				; 0x19
	dw		command_null				; 0x1A
	dw		command_null				; 0x1B
	dw		command_null				; 0x1C
	dw		command_null				; 0x1D
	dw		command_null				; 0x1E
	dw		command_null				; 0x1F		last BIOS reserved

    dw		command_PlayCoinA			; 0x20
    dw		command_PlayLeftA			; 0x21
    dw		command_PlayRightA			; 0x22
    dw		command_PlayCenterA			; 0x23
	dw		command_null				; 0x24
	dw		command_null				; 0x25
	dw		command_null				; 0x26
	dw		command_null				; 0x27
	dw		command_null				; 0x28
	dw		command_null				; 0x29
	dw		command_null				; 0x2A
	dw		command_null				; 0x2B
	dw		command_null				; 0x2C
	dw		command_null				; 0x2D
	dw		command_null				; 0x2E
	dw		command_PCMAStop			; 0x2F

    dw		command_PlayPCMBLeft		; 0x30
	dw		command_PlayPCMBCenter		; 0x31
    dw		command_PlayPCMBRight		; 0x32
    dw		command_LoopPCMB			; 0x33
    dw		command_NoLoopPCMB			; 0x34
    dw		command_setADPCMB_Sample0	; 0x35
	dw		command_setADPCMB_Sample1	; 0x36
	dw		command_null				; 0x37
	dw		command_null				; 0x38
	dw		command_null				; 0x39
	dw		command_null				; 0x3A
	dw		command_null				; 0x3B
	dw		command_null				; 0x3C
	dw		command_null				; 0x3D
	dw		command_null				; 0x3E
	dw		command_PCMBStop			; 0x3F

	dw		command_SSGrampInit			; 0x40
    dw		command_SSGrampCycle		; 0x41
    dw		command_SSG_pulseStart		; 0x42
    dw		command_SSG_1khzStart		; 0x43
    dw		command_SSG_260hzStart		; 0x44
	dw		command_null				; 0x45
	dw		command_null				; 0x46
	dw		command_null				; 0x47
	dw		command_null				; 0x48
	dw		command_null				; 0x49
	dw		command_null				; 0x4A
	dw		command_null				; 0x4B
	dw		command_null				; 0x4C
	dw		command_null				; 0x4D
	dw		command_null				; 0x4E
	dw		command_SSGpulseStop		; 0x4f

	dw		command_null				; 0x50
	dw		command_null				; 0x51
	dw		command_null				; 0x52
	dw		command_null				; 0x53
	dw		command_null				; 0x54
	dw		command_null				; 0x55
	dw		command_null				; 0x56
	dw		command_null				; 0x57
	dw		command_null				; 0x58
	dw		command_null				; 0x59
	dw		command_null				; 0x5A
	dw		command_null				; 0x5B
	dw		command_null				; 0x5C
	dw		command_null				; 0x5D
	dw		command_null				; 0x5E
	dw		command_null				; 0x5F

	dw		command_null				; 0x60
	dw		command_null				; 0x61
	dw		command_null				; 0x62
	dw		command_null				; 0x63
	dw		command_null				; 0x64
	dw		command_null				; 0x65
	dw		command_null				; 0x66
	dw		command_null				; 0x67
	dw		command_null				; 0x68
	dw		command_null				; 0x69
	dw		command_null				; 0x6A
	dw		command_null				; 0x6B
	dw		command_null				; 0x6C
	dw		command_null				; 0x6D
	dw		command_null				; 0x6E
	dw		command_null				; 0x6F
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
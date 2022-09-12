	* ++====================================================================++
	* || common_crt0_cd.s - C Run Time Startup Code for Neo Geo CD			||
	* ++--------------------------------------------------------------------++
	* || $Id: common_crt0_cd.s,v 1.5 2001/07/13 14:46:31 fma Exp $			||
	* ++--------------------------------------------------------------------++
	* || This is the startup code needed by programs compiled with GCC		||
	* ++--------------------------------------------------------------------||
	* || BGM: The Cranberries - Saving Grace								||
	* ++====================================================================++
	
_ZERO_DIVIDE	=	0x00c00426
_CHK_CMD	=	0x00c00426
_TRAPV_CMD	=	0x00c00426
_NPC_1010	=	0x00c00426
_NPC_1111	=	0x00c00426
_IRQ4	=	0x00c00426
_IRQ5	=	0x00c00426
_IRQ6	=	0x00c00426
_IRQ7	=	0x00c00426
	
********************** Exported Symbols **********************
	.globl	_start
	.globl	atexit

********************** Imported Symbols **********************
	.globl	__do_global_ctors
	.globl	__do_global_dtors
	.globl	main
	.globl	memset
	.globl	__bss_start	
	.globl	_end
	.globl	_sbrk_base1
	.globl	_sbrk_base2

********************** Program Start *************************

	.org	0x0000
	
	.long	0x0010f300	|;reset stack ptr
	.long	0x00c00402	/*;reset ptr*/
	.long	0x00c00408	/*;bus error*/
	.long	0x00c0040e	/*;address error*/

	.long	0x00c00414	/*;illegal instruction*/
	.long	_ZERO_DIVIDE	/*;division by 0*/
	.long	_CHK_CMD	/*;CHK command*/
	.long	_TRAPV_CMD	/*;TRAPV command*/

	.long	0x00c0041a	/*;illegal privilege*/
	.long	0x00c00420	/*;trace exception handling*/
	.long	_NPC_1010	/*;no package command (1010)*/
	.long	_NPC_1111	/*;no package command (1111)*/

	.long	0x00c00426, 0x00c00426, 0x00c00426	/*;unused*/
	.long	0x00c0042c	/*;uninitialized interrupt*/

	|;.long	0x00c00426,	0x00c00426,	0x00c00426,	0x00c00426	/*;unused*/
	|;.long	0x00c00426,	0x00c00426,	0x00c00426,	0x00c00426	/*;unused*/
	|;.long	0x00c00432	/*;virtual interrupt*/
	.long	0xC00522, 0xC00528, 0xC0052E, 0xC00534
	.long	0xC0053A, 0xC004F2, 0xC004EC, 0xC004E6
	.long	0xC004E0
	
	|; 0x64
	.long	_IRQ1, _IRQ2, _IRQ3, _IRQ4, _IRQ5, _IRQ6, _IRQ7	/*;4-7 unused*/
	.long	TRAP00,	TRAP01,	TRAP02,	TRAP03
	.long	TRAP04,	TRAP05,	TRAP06,	TRAP07
	.long	TRAP08,	TRAP09,	TRAP10,	TRAP11
	.long	TRAP12,	TRAP13,	TRAP14,	TRAP15
	.long	0xC00426, 0xC00426, 0xC00426, 0xC00426
	.long	0xC00426, 0xC00426, 0xC00426, 0xC00426
	.long	0xC00426, 0xC00426, 0xC00426, 0xC00426
	.long	0xC00426, 0xC00426, 0xC00426, 0xC00426
	
	.org	0x100
	.ascii	"NEO-GEO"
	.byte	_CDDA_FLAG
	.word	_NGH
	.long	_PROGRAM_SIZE
	.long	_WRK_BCKP_AREA
	.word	_WRK_BCKP_AREA_SIZE
	.byte	_EYE_CATCHER
	.byte	_EYE_CATCHER_TILES

	.long	JPconfig
	.long	NAconfig
	.long	EUconfig
	
	jmp		_ENTRY_USER
	jmp		_ENTRY_PLAYER_START
	jmp		_ENTRY_DEMO_END
	jmp		_ENTRY_COIN_SOUND

	.word	0xffff
	.long	0xffffffff
	.long	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
	.long	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
	.long	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
	.long	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff

|;* Standard IRQ1 handler
_irq1_handler:
	move.w	#2, 0x3C000C
	rte
|;* Standard IRQ3 handler
_irq3_handler:
	move.w	#1, 0x3C000C
_dummy_exc_handler:
	rte
|;* Dummy atexit (does nothing for now)
atexit:
	moveq	#0, d0
_dummy_config_handler:
	rts

|;* Entry point of our program
_start:
	|;* Setup stack pointer and 'system' pointer
	|;lea		0x10F300, a7
	|;lea		0x108000, a5

	|;* Reset watchdog
	move.b	d0, 0x300001
	
	moveq	#0, d0
	move.l	d0, TInextTable
	move.l	d0, VBL_callBack
	move.l	d0, VBL_skipCallBack
	
	|;* Enable manual reset (A+B+C+START or SELECT)
	bclr	#7, 0x10FD80

	|;* Flush interrupts
	move.b	#7, 0x3C000C

	|;* Enable interrupts
	move.w	#0x2000, sr
	
	|;* Stop CDDA
	move.w	#0x0200, d0
	jsr		0xc0056a

	|;* Initialize base pointers for malloc
	move.l	#0x120000, _sbrk_base2
	move.l	#_end, d0
	add.l	#32, d0
	and.l	#0xFFFFFF0, d0
	move.l	d0, _sbrk_base1
	
	|;* Initialize BSS section
	move.l	#_end, d0
	sub.l	#__bss_start, d0
	move.l	d0, -(a7)
	clr.l	-(a7)
	pea		__bss_start
	jbsr	memset	
	
	|;hotfix
	move.b #0x80, 0x10fd80
	
	|;* Jump to main
	jbsr	main

	|;* Call global destructors
	jbsr	__do_global_dtors

	|;* For CD systems, return to CD Player
	jmp		0xC0055E

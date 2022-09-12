********************* Vector Definitions *********************
_IRQ1	=	DAT_TIfunc
_IRQ2	= DAT_vblankTI
_IRQ3	=	_irq3_handler
_ENTRY_USER	= _start
_ENTRY_PLAYER_START	= _dummy_config_handler
_ENTRY_DEMO_END	= _dummy_config_handler |; MVS game switch
_ENTRY_COIN_SOUND	= _dummy_config_handler |; coin sound

DUMMY			= _dummy_exc_handler
TRAP00			= _dummy_exc_handler
TRAP01			= _dummy_exc_handler
TRAP02			= _dummy_exc_handler
TRAP03			= _dummy_exc_handler
TRAP04			= _dummy_exc_handler
TRAP05			= _dummy_exc_handler
TRAP06			= _dummy_exc_handler
TRAP07			= _dummy_exc_handler
TRAP08			= _dummy_exc_handler
TRAP09			= _dummy_exc_handler
TRAP10			= _dummy_exc_handler
TRAP11			= _dummy_exc_handler
TRAP12			= _dummy_exc_handler
TRAP13			= _dummy_exc_handler
TRAP14			= _dummy_exc_handler
TRAP15			= _dummy_exc_handler

************************ Definitions *************************
_CDDA_FLAG	= 0
_NGH	=	0x7777
_PROGRAM_SIZE	=	0x00100000
_WRK_BCKP_AREA = bkp_data
_WRK_BCKP_AREA_SIZE = 0x0100	|;256bytes
_EYE_CATCHER = 0x02	/*;eye catcher (0-common 1-custom 2-off)*/
_EYE_CATCHER_TILES = 0x01 /*;eye catcher start tiles (upper bits, 0x01 => 0x0100)*/

	.include	"common_crt0_cd.s"

* Names MUST be 16 characters long
*           <---------------->
JPconfig:
	.ascii	"HELLO WORLD JP  "
	.word	0xffff, 0xffff
	.byte		0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

NAconfig:
	.ascii	"HELLO WORLD US  "
	.word	0xffff, 0xffff
	.byte		0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	
EUconfig:
	.ascii	"HELLO WORLD EU  "
	.word	0xffff, 0xffff
	.byte		0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00


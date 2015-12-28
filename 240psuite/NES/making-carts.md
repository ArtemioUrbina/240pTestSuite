The following instructions for making a cartridge of 240p test suite
for NES are based on instructions in [Ice Man's post] on NESdev BBS.

The suite uses the [UNROM board] (iNES mapper 2) with horizontal
nametable arrangement, called "vertical mirroring" by the
emulation community. You can pull the circuit board out of any
[game using UNROM] and replace the mask ROM with a 27C512, 29F512,
or equivalent 64Kx8-bit or bigger 5 V UV EPROM or flash memory.
Or you can buy a [discrete repro board] from RetroStage and
populate it with the necessary discrete parts and key CIC.

NES ROM images contain a 16-byte header, starting with `4E 45 53 1A`.
This describes the game's intended circuit board to an emulator.  But
because you have an actual circuit board, you'll want to chop off the
first 16 bytes in a hex editor, resulting in a binary image 65,536
bytes in size.  And if you're using a memory larger than a 27C512,
you'll need to double up the ROM to fill it.  The `nestobin.py`
program in the `tools` folder automates this, or if you don't have
Python installed, you can use FamiROM, linked in Ice Man's post.

If using a donor board, you'll need a "line" head screwdriver
(sometimes called "GameBit") to open the cartridge shell.
First desolder the program ROM on the right side of the board.
This is a 28-pin mask ROM, marked "PRGROM" or "U1 PRG" on the
PCB.  DO NOT REMOVE the skinny chips (LS161, LS32, and CIC)
or the CHR RAM.  Clean the holes.

After you've burned the binary to your EPROM using an EPROM
programmer, solder it into the PCB.  The pinout for RetroStage
reproduction boards should match that of an EPROM.  But if you
use a Nintendo donor board, you'll need to rearrange a few pins
slightly using short pieces of wire.

For 28-pin EPROMs (27c512):
1. Bend up pin 22
2. Solder pin 22 to GND (/OE)

For 32-pin EPROMs (27c040):
1. Bend up pin 1, 2, 24, 31 and 32
2. Solder pin 2 to hole 22 (A16)
3. Solder pin 24 to GND (/OE)
4. Solder pin 31 to hole 28 (+5V)
5. Solder pin 32 to hole 28 (+5V)

When using a 32 pin EPROM, make sure the capacitor on the right side
of the PRG ROM is removed and soldered in from the bottom of the
board.  Otherwise the EPROM will not fit in properly, and the cart
won't close.  Take care to solder it back in with correct polarity.

Finally, set the nametable arrangement to horizontal.  On Nintendo
donor boards, make sure H is closed with solder and V is open.
The majority of UNROM games have H closed, but a few have V closed,
so you'll need to resolder the pads.  RetroStage boards use the
"mirroring" convention, so close VERT and leave HORIZ open.

[Ice Man's post]: http://forums.nesdev.com/viewtopic.php?p=159747#p159747
[UNROM board]: http://bootgod.dyndns.org:7777/pcb.php?PcbID=425+426+427+428+429+430+432+433+434
[game using UNROM]: [http://bootgod.dyndns.org:7777/search.php?keywords=unrom&kwtype=pcb&group=groupid]
[discrete repro board]: http://www.retrostage.net/nes_discretes.htm
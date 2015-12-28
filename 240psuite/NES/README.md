240p Test Suite
===============

The [240p test suite] is a homebrew software suite for video game
consoles developed to help in the evaluation of upscalers, upscan
converters and line doublers.

It has tests designed with the processing of 240p signals in mind,
although when possible it includes other video modes and specific
tests for them.  These have been tested with video processors on
real hardware and a variety of displays, including CRTs and Arcade
monitors via RGB.

As a secondary target, the suite aims to provide tools for
calibrating colors, black and white levels for specific console
outputs and setups. 

This is free software, with full source code available under the GPL.

[240p test suite]: http://junkerhq.net/xrgb/index.php/240p_test_suite

Usage
-----
Load 240pee.nes onto a [PowerPak] and run it.

You can also burn it to an NES cartridge with an UNROM board.

1. Chop off the first 16 bytes, which contain the iNES header.
2. Write the remaining 65536 bytes to a 27C512, 29F512, or
   equivalent 64Kx8-bit 5 V EPROM or flash memory.
3. Open the Game Pak's case with a GameBit screwdriver.
4. Desolder the existing PRG ROM from the board.  Don't desolder
   the short skinny chips (mapper and CIC) or the CHR RAM.
5. Make sure the UNROM board's H pad has a solder blob and the V pad
   is not covered.  The suite uses horizontal nametable arrangement,
   called "vertical mirroring" by the emulation community.
6. Compare the memory's pinout to the pinout for the mask ROM on
   the UNROM board to see which pins need to be lifted and connected
   through patch wires to a different hole.
7. Solder the memory in place, and add necessary patch wires.

Once the test is running, the credits will appear.  You can navigate
the menu with the Control Pad and the A and B Buttons.  There are
two pages of tests, the first with mostly still images and the second
with more interactive tests.  In each test, you can press B to exit
or Start to display instructions.  The test instructions are also
available in "helppages.txt" in the suite's source code.

Versions
--------
The Nintendo Entertainment System (NES) can output 240 picture
lines in a progressive "double struck" mode.  It does not support
interlaced video, and its 52-color palette is closer to HSV than RGB.

The 240p test suite for Nintendo Entertainment System was developed
in 6502 assembly language using [ca65], with image conversion tools
written in Python 3 and [Pillow] (Python Imaging Library).  It was
tested on authentic NTSC and PAL NES consoles using a [PowerPak].

Since version 0.05, the suite for NES compensates for differences
between NTSC and PAL NES.  It should work on a Dendy famiclone,
though this hasn't been tested yet.

There are as of November 2015 six other platforms that run the suite:

* It was first developed in C for the Sega Genesis using the SGDK,
  using 320x224p pixel resolution.
* The Genesis version is also available as a disc image for Sega CD.
* The Sega Dreamcast version was coded using KallistiOS and runs on
  any MIL-CD compatible Dreamcast console.
* The Super NES version was coded using PVSnesLib.  It runs mostly
  in 256x224p, with a few tests in the less common 356x239p mode.
* The GameCube version supports 240p, 480i and 480p modes in NTSC
  and their PAL counterparts on any homebrew-capable GameCube.
* The Wii version is identical to the GameCube version.  It runs on
  a Wii console with the Homebrew Channel installed.

[ca65]: https://cc65.github.io/cc65/
[Pillow]: https://pillow.readthedocs.org/
[PowerPak]: http://www.retrousb.com/product_info.php?cPath=24&products_id=34

Contributors
------------
* Concept: Artemio Urbina [@Artemio]
* Code: Damian Yerrick [@PinoBatch]
* Main menu graphics: Damian Yerrick
* Portrait in Shadow sprite test: darryl.revok
* Hill zone background: mikejmoffitt
* Extra patterns and collaboration: Konsolkongen & [shmups] regulars

If you're interested in contributing to the NES port, you're invited
to post in its [development thread].  We seek experts in the following:

* Testing on PAL NES
* Testing on Dendy
* Testing with high-quality displays and scalers
* Measuring the actual IRE levels of NES video with an oscilloscope
* Putting 27C512 onto an UNROM-compatible board

[@Artemio]: https://twitter.com/Artemio
[@PinoBatch]: https://twitter.com/PinoBatch
[shmups]: http://shmups.system11.org/
[development thread]: http://forums.nesdev.com/viewtopic.php?t=13394

Copyright 2011-2014 Artemio Urbina  
Copyright 2015 Damian Yerrick

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

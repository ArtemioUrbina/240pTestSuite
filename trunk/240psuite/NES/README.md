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
Full instructions are in `making-carts.md`.

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

Since version 0.05, it compensates for differences between NTSC and
PAL NES.  It should work on PAL famiclones such as Dendy, though
this hasn't been tested yet.

There are as of late 2015 five other platforms that run the suite:

* It was first developed in C for the Sega Genesis using the SGDK,
  using 320x224p pixel resolution.  It comes as a cartridge image
  or as a disc image for Sega CD.
* The TurboGrafx-16 version was made with HuC.  It supports widths
  256, 320, and 512, and heights 224 and 239.  It comes as a HuCard
  image for Turbo EverDrive, a disc image for CD-ROM2 (TurboGrafx-CD
  with System Card 1/2), and a disc image for Super CD-ROM2
  (TurboDuo or Super System Card) that loads completely into RAM.
* The Super NES version was coded using PVSnesLib.  It runs mostly
  in 256x224p, with a few tests in the less common 256x239p mode.
* The Sega Dreamcast version was made with KallistiOS and runs on
  any MIL-CD compatible Dreamcast console.  It supports 240i, 480i,
  480p, 288p, and 576i resolutions, cables and crystals permitting.
* The Nintendo GameCube version was made with devkitPPC and runs on
  any homebrew-capable GameCube or Wii console.  It supports the same
  resolutions as the Dreamcast version.  It comes as a disc image for
  a modded GameCube, a DOL file for SD Media Launcher on GameCube,
  and a DOL file for Homebrew Channel on Wii.

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
to post in its [development thread].  We seek experts with authentic
NES and clone consoles, both NTSC and PAL, who can run the suite on  
high-quality displays and scalers.

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

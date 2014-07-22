240p Test Suite
===============================================================================
For full dcoumentation and source code, please visit:
http://junkerhq.net/xrgb/index.php/240p_test_suite
===============================================================================

The 240p test suite is a homebrew software suite for video game consoles 
developed to help in the evaluation of upscalers, upscan converters and 
line doublers.

It has tests designed with the processing of 240p signals in mind, although
when possible it includes other video modes and specific tests for them. 
These have been tested with video processors on real hardware and a variety
of displays, including CRTs and Arcade monitors via RGB.

As a secondary target, the suite aims to provide tools for calibrating colors,
black and white levels for specific console outputs and setups. 

This is free software, with full source code available under the GPL.

Versions
===============================================================================

There are currently six platforms that run the suite, all have been developed
and tested on real hardware, The software should run in NTSC mode by default.

The Sega Genesis version was developed using the SGDK, it was the first version
of the 240p Test Suite and is fully coded in C. In order to run it, we 
recommend the Tototek Flashcart or the newer Everdrive.

The Genesis resolution is 320x224 and is being used fully, in NTSC (60hz).

The Sega CD version has the exact same specs as teh Genesis one, but you only
need a CD-R to run it.

The Sega Dreamcast version includes more options, but the core functionality 
for 240p is the same. No special hardware or modifications are needed, it will
run on any MIL-CD compatible Dreamcast after burning the CDI image 
(discjuggler format). It was coded using KallistiOS and some functionality from
parallax (font support).

The SNES version of the suite runs in full 256x224p, and has some tests and 
patterns for the 356x239p mode.

The Wii and Gamecube versions of the suite need a console able to run homebrew.
and support 240p, 480i and 480p modes in NTSC, as well as their counterparts
in PAL.


Contributors
===============================================================================
    * Code: Artemio Urbina
    * Main menu graphics: Asher
    * SGDK assistance: Stephane
    * KallistiOS SDK assistance: BlueCrab
    * PVSnesLib: Alekmaul
    * Advisor: Fudoh 
    * Extra patterns and collaboration: Konsolkongen & shmups regulars

===============================================================================

For full documentation and source code, please visit:
http://junkerhq.net/240p/

===============================================================================
2011-2014 Artemio Urbina

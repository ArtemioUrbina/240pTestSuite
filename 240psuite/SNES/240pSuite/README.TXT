240p Test Suite
===============================================================================
For full documentation and source code, please visit:
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

There are currently several platforms that run the suite, all have been developed
and tested on real hardware. The software should run in NTSC mode by default.

Sega Genesis/Mega Drive:
------------------------
It was developed using the SGDK, it was the first version
of the 240p Test Suite and is fully coded in C. In order to run it, we 
recommend a Mega Everdrive flash cartridge, or the older Tototek one. The 
Genesis resolution is 320x224 and is being used fully, in NTSC (60hz).

Sega CD/Mega CD:
----------------
This version has the exact same specs as the Genesis one, but you only
need a CD-R to run it. The program is fully loaded to the system RAM after
startup.

PC Engine/Turbo Grafx-16:
-------------------------
This version of the suite requires a flash cart, such as the Turbo Everdrive.
It has support for 224p and 240p, as well as the 3 horizontal resolutions:
256, 320 and 512. It must be noted that most games in 240p really use only 239
lines, so the suite also supports that video mode. It was developed using HuC
and ASM.

PCE Duo/Turbo Duo/CD-ROM2/Super CDROM2:
---------------------------------------
Both versions have the exact same features as the PC Engine one, however
there are two variants. The CD-ROM2 version works with Systems that have
a System Cars 1.0 or 2.0/2.1, including the American TurboGrafx-16. The
Super CD-ROM2 version uses System Card 3.0 or the Arcade Card. The benefit
of this SCD version is that it loads only once at startup, and everything
is ran from RAM afterwards.

Super Nintendo/Super Famicom:
-----------------------------
This version of the suite runs in full 256x224p, and has some tests and 
patterns for the 356x239p mode. It requires a flash cart, such as the
Super Everdrive.


Sega Dreamcast:
---------------
This version includes more options, but the core functionality for 240p is
the same. No special hardware or modifications are needed, it will
run on any MIL-CD compatible Dreamcast after burning the CDI image 
(discjuggler format). It was coded using KallistiOS and some functionality from
parallax (font support).

Wii:
----
This version of the suite needs a console able to run homebrew. It supports
240p, 480i and 480p modes in NTSC, as well as their counterparts in PAL. Can
be ran from the SD Card.

Gamecube:
---------
This version has the same features as the Wii version, can be ran from CD or
an SD card media launcher.

NES/Famicom:
---------
The Nintendo Entertainment System (NES) can output 240 picture
lines in a progressive "double struck" mode.  It does not support
interlaced video, and its 52-color palette is closer to HSV than RGB.
It was developed by Damian Yerrick.

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
2011-2016 Artemio Urbina

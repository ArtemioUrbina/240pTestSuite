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

Versions
===============================================================================

There are currently three platforms that run the suite, both have been developed
and tested on real hardware, The software should run in NTSC mode by default 
(hasn't been tested in PAL hardware, but should work as expected).

The Sega Genesis version was developed using the SGDK, it was the first version
of the 240p Test Suite and is fully coded in C. In order to run it, we 
recommend the Tototek Flashcart or the newer Everdrive.

The Genesis resolution is 320x224 and is being used fully, in NTSC (60hz).
[edit] Sega Dreamcast

The Sega CD version has the exact same specs as teh Genesis one, but you only
need a CD-R to run it.

The Sega Dreamcast version includes more options, but the core functionality 
for 240p is the same. No special hardware or modifications are needed, it will
run on any MIL-CD compatible Dreamcast after burning the CDI image 
(discjuggler format). It was coded using KallistiOS and some functionality from
parallax (font support).

This version has a menu option for changing the current resolution, with the 
following values:

    * 240p: Standard 320x240 at 60hz is used, with all graphic data in that 
        resolution.
    * Fake 480i: This is 640x480i at 60hz, but using all the resources at 
        320x240 using internal line doubling. Useful for testing for this
        case which is present in many Dreamcast and PS2 games. Also known 
        as "Fake hi-res"
    * 480i: It is 640x480i at 60hz as well, however graphics are unscaled and 
        shown only on the upper left corner of the screen (the first 320x240 
        pixels). Since there is a 1 to 1 pixel ration as in 240p, tests can be
        performed for 480i de-interlacers and scalers.
    * 480p with scanlines: Only available when using a VGA Box. This is 
        internally line-doubled and displayed. In the case of previous 
        resolutions, they are also internally line doubled to 480p, but 
        respecting each mode variations. 

The software is compatible with composite, s-video, RGB and VGA cables.

There is a previous SDL version that runs on 320x234 mode, but the small
framework was rewritten to use the PVR chip on the Dreamcast directly with
a great advantage in speed for frame alternating patterns. However the SDL
version is kept - but no longer maintained - for documentation purposes along
with its source code.


Contributors
===============================================================================
    * Code: Artemio Urbina
    * Main menu graphics: Asher
    * SGDK assistance: Stephane
    * KallistiOS SDK assistance: BlueCrab
    * Advisor: Fudoh 
    * Extra patterns and collaboration: Konsolkongen & shmups regulars


===============================================================================

For full documentation and source code, please visit:
http://junkerhq.net/240p/

===============================================================================
2011-2014 Artemio Urbina

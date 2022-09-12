# 240p Test Suite for the Neo Geo

Clone into the projects folder of the NeoDev toolchain folder. The NeoDev Toolchain should be located at C:\NeoDev

## Quick Setup

If you're using VSCode, open `ng240ptest.code-workspace` and run the `Build CHAR ROM` and `Build Fix ROM`. Then run the default `Run Build Task`. The default task will run a clean, make, copyroms, and launch the build in MAME.

If you're not using VSCode, use the following below.

1. Build the CROM and SROM. Needs to be done every time contents in the gfx folder are changed.
`make char`
`make fix`
<br>
2. Build the PROM.
`make`
<br>
3. Copy ROMs to MAME included in the toolchain.
`make copyroms`
<br>
4. Run in MAME.
`cd C:\NeoDev\mame`
`mame neogeo 240ptestng -nofilter`
<br>

## TODO

1. Update toolchain to latest version of gcc.
2. Create workflow for building proper ROMs. Currently ROMs are "dev" ROMs without padding and proper names.
3. Incorporate tools for building the .neo ROM for use with flashcart.
4. Incorporate tools for building the sound MROM and VROM.

## Resources
[Neo Geo Development Wiki](https://wiki.neogeodev.org/index.php?title=Main_Page) - Neo Geo Bible<br>
[68000 ASM Programming for the Neo Geo](https://www.chibiakumas.com/68000/neogeo.php) - Another excellent source of information<br>
[NeoBitz SDK](http://sebastianmihai.com/neogeo-neo-thunder.html) - SDK used for Neo Geo.<br>
[DATlib Video Library](https://www.yaronet.com/topics/189390-datlib-update-03) - Library for graphics. Must be used with NeoBitz SDK.<br>
[Neo Geo Sprite Viewer](https://neospriteviewer.mattgreer.dev/) - View the sprite tiles of a Neo Geo game<br>
[NGFX GraphicsEditor](https://blastar.citavia.de/index.php?controller=post&action=view&id_post=37) - Interesting tool for graphics editing for use with the Neo Geo<br>
[NGFX ColorFadeCalculator](https://blastar.citavia.de/index.php?controller=post&action=view&id_post=34) - Another interesting tool for calculating gradients. Might be useful for color bars pattern<br>
[NeoBuilder Homebrew GUI](https://bidouillouzzz.blogspot.com/2022/02/neobuilder-homebrew-gui.html#more) - Used for converting ROMs to .neo for use with the Terraonion NeoSD<br>
[Matt's Neo Stuff - Neo Geo Dev Tools](https://neogeo.mattgreer.dev/tools) - Great collection of Neo Geo Dev Tools<br>
[NGFX Soundbuilder](https://blastar.citavia.de/index.php?controller=blog&action=view&category=ngfx-toolchain) - Potential tool for MDF<br>
[Freem's Neo Geo Dev Page](http://www.ajworld.net/neogeodev/) - Useful information<br>
[Freem's - My Ideal Toolset](http://www.ajworld.net/neogeodev/ideal_tools.html) - Insight<br>
[ngdevkit-examples](https://github.com/dciabrin/ngdevkit-examples) - Collection of Neo Geo programs that can be built with ngdevkit<br>

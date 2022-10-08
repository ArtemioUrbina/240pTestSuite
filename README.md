# 240p Test Suite for the Neo Geo

Clone into the projects folder of the NeoDev toolchain folder. The NeoDev Toolchain should be located at C:\NeoDev

## Quick Setup

If you're using VSCode, open `ng240ptest.code-workspace` and run the `Build CHAR ROM` and `Build Fix ROM`. Then run the default `Run Build Task`. The default task will run a clean, make, copyroms, and launch the build in MAME.

If you're not using VSCode, use the following below.

1. Build the CROM and SROM. Needs to be done every time contents in the gfx folder are changed.
`make char`
`make fix`
2. Build the PROM.
`make`
3. Copy ROMs to MAME included in the toolchain.
`make copyroms`
4. Run in MAME.
`cd C:\NeoDev\mame`
`mame neogeo 240ptestng -nofilter -keepaspect -skip_gameinfo`

## MAME Drivers
The following is for the unmodified ROMs. (Checksums not correct)
```
	<software name="240ptestng">
	<description>240p Test Suite - Neo Geo</description>
	<year>2022</year>
	<publisher>Artemio / Dasutin</publisher>
	<sharedfeat name="release" value="MVS,AES" />
	<sharedfeat name="compatibility" value="MVS,AES" />
	<part name="cart" interface="neo_cart">
		<dataarea name="maincpu" width="16" endianness="big" size="0x400000">
			<rom loadflag="load16_word" name="dev_p1.rom" offset="0x000000" size="0x100000" crc="bdda2c6e" sha1="6a94dee2d22feb07ea68a90ce67d5cac1b17b9c9" />
		</dataarea>
		<dataarea name="fixed" size="0x040000">
			<rom offset="0x000000" size="0x020000" name="fix.bin" crc="0e6a7c73" sha1="31b1194524dcc80ec4d63bac088b6fb4909f496c" />
		</dataarea>
		<dataarea name="audiocpu" size="0x040000">
			<rom  offset="0x000000" size="0x040000" name="m1.rom" crc="da4878cf" sha1="ce13d18a4c5d01974df8542c67c4df00dbc6e7c1" />
		</dataarea>
		<dataarea name="ymsnd" size="0x100000">
			<rom  offset="0x000000" size="0x100000" name="v1.rom" crc="149a5c2f" sha1="d52eac230f7aaa1d70cbb8d50a2513f180c65e4d" />
		</dataarea>
		<dataarea name="sprites" size="0x2800000">
			<rom loadflag="load16_word" name="char.bin" offset="0x000000" size="0x800000" crc="a9bdc000" sha1="93b0dfcd2121ddf6ea1fe99514a176d76e4b0c98" />
		</dataarea>
	</part>
	</software>
```
The following is for the modified ROMs for use with cartridges (Checksums not correct)
```
	<software name="240ptestng">
	<description>240p Test Suite - Neo Geo</description>
	<year>2022</year>
	<publisher>Dasutin / Artemio</publisher>
	<sharedfeat name="release" value="MVS,AES" />
	<sharedfeat name="compatibility" value="MVS,AES" />
	<part name="cart" interface="neo_cart">
		<dataarea name="maincpu" width="16" endianness="big" size="0x100000">
			<rom loadflag="load16_word_swap" name="2501-p1.p1" offset="0x0000000" size="0x100000" crc="bdda2c6e" sha1="6a94dee2d22feb07ea68a90ce67d5cac1b17b9c9" />
		</dataarea>
		<dataarea name="fixed" size="0x040000">
			<rom offset="0x000000" size="0x040000" name="2501-s1.s1" crc="0e6a7c73" sha1="31b1194524dcc80ec4d63bac088b6fb4909f496c" />
		</dataarea>
		<dataarea name="audiocpu" size="0x020000">
			<rom  offset="0x000000" size="0x020000" name="2501-m1.m1" crc="da4878cf" sha1="ce13d18a4c5d01974df8542c67c4df00dbc6e7c1" />
		</dataarea>
		<dataarea name="ymsnd" size="0x100000">
			<rom  offset="0x000000" size="0x100000" name="2501-v1.v1" crc="149a5c2f" sha1="d52eac230f7aaa1d70cbb8d50a2513f180c65e4d" />
		</dataarea>
		<dataarea name="sprites" size="0x200000">
			<rom loadflag="load16_byte" name="2501-c1.c1" offset="0x000000" size="0x100000" crc="a9bdc000" sha1="93b0dfcd2121ddf6ea1fe99514a176d76e4b0c98" />
			<rom loadflag="load16_byte" name="2501-c2.c2" offset="0x000001" size="0x100000" crc="a9bdc000" sha1="93b0dfcd2121ddf6ea1fe99514a176d76e4b0c98" />
		</dataarea>
	</part>
	</software>
```
For the Neo Geo CD (Copy the CHD image)
```
	<software name="240ptestngcd">
		<description>240p Test Suite Neo Geo CD</description>
		<year>2022</year>
		<publisher>Dasutin/Artemio</publisher>
		<info name="alt_title" value="240pTestSuite" />
		<info name="serial" value="NGCD-2501 (USA)" />
		<info name="release" value="20200904 (USA)" />
		<part name="cdrom" interface="neocd_cdrom">
			<diskarea name="cdrom">
				<disk name="240pTestSuite" sha1="8cefd6a76b45bac8a1fdae0691ca33e737aa41d7"/>
			</diskarea>
		</part>
	</software>
```

## TODO

1. Incorporate tools for building the sound MROM and VROM.

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

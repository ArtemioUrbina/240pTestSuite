#!/usr/bin/make -f
#
# Makefile for 240p test suite
# Copyright 2011-2015 Damian Yerrick
#
# Copying and distribution of this file, with or without
# modification, are permitted in any medium without royalty
# provided the copyright notice and this notice are preserved.
# This file is offered as-is, without any warranty.
#

# These are used in the title of the NES program and the zip file.
title = 240pee
version = 0.06

# Space-separated list of assembly language files that make up the
# PRG ROM.  If it gets too long for one line, you can add a backslash
# (the \ character) at the end of the line and continue on the next.
objlist = vwf7 init paldetect vwf_draw unrom \
  main helpscreen helppages.hlp stills stopwatch backlight \
  scrolling soundtest overscan megaton semixparent overclock \
  pads ppuclear unpb53 bcd random rectfill muldiv

AS65 = ca65
LD65 = ld65
CFLAGS65 = 
objdir = obj/nes
srcdir = src
imgdir = tilesets

#EMU := "/C/Program Files/Nintendulator/Nintendulator.exe"
EMU := fceux
# other options for EMU are start (Windows) or gnome-open (GNOME)

# Windows needs .exe suffixed to the names of executables; UNIX does
# not.  COMSPEC will be set to the name of the shell on Windows and
# not defined on UNIX.
ifdef COMSPEC
DOTEXE=.exe
else
DOTEXE=
endif

.PHONY: runalt dist zip clean

run: $(title).nes
	$(EMU) $<

# Rule to create or update the distribution zipfile by adding all
# files listed in zip.in.  Use changes to the compiled program and
# README as a heuristic for when something was changed.
dist: zip
zip: $(title)-$(version).zip
$(title)-$(version).zip: zip.in $(title).nes README.md CHANGES.txt $(objdir)/index.txt
	zip -9 -u $@ -@ < $<

$(objdir)/index.txt: makefile
	echo "Files produced by build tools go here, but caulk goes where?" > $@

clean:
	-rm $(objdir)/*.o $(objdir)/*.s $(objdir)/*.chr

# Rules for PRG ROM

objlisto = $(foreach o,$(objlist),$(objdir)/$(o).o)

map.txt $(title).nes: unrom512kbit.cfg $(objlisto)
	$(LD65) -o $(title).nes -m map.txt -C $^

$(objdir)/%.o: $(srcdir)/%.s $(srcdir)/nes.inc $(srcdir)/global.inc
	$(AS65) $(CFLAGS65) $< -o $@

$(objdir)/%.o: $(objdir)/%.s
	$(AS65) $(CFLAGS65) $< -o $@

# Files that depend on included files (.include, .incbin)

$(objdir)/helpscreen.o: \
  $(objdir)/gus_bg.sb53 $(objdir)/gus_sprite.chr.pb53
$(objdir)/stills.o: \
  $(objdir)/linearity_ntscgray.sb53 $(objdir)/linearity_palgray.sb53 \
  $(objdir)/linearity_grid_ntscgray.sb53 $(objdir)/linearity_grid_palgray.sb53 \
  $(objdir)/sharpnessgray.sb53 $(srcdir)/rectfill.inc
$(objdir)/rectfill.o: \
  $(objdir)/stdtiles.chr.pb53 $(srcdir)/rectfill.inc
$(objdir)/stopwatch.o: \
  $(objdir)/lag_clock_face.sb53 $(objdir)/fizzter_digits.chr.pb53 \
  $(objdir)/lag_clock_balls.chr.pb53 $(srcdir)/rectfill.inc
$(objdir)/backlight.o: \
  $(objdir)/backlight_sprites.chr.pb53
$(objdir)/scrolling.o: \
  $(srcdir)/rectfill.inc $(objdir)/greenhillzone.sb53
$(objdir)/overscan.o: \
  $(objdir)/overscan.chr.pb53 $(srcdir)/rectfill.inc
$(objdir)/megaton.o: \
  $(objdir)/megatontiles.chr.pb53 $(srcdir)/rectfill.inc
$(objdir)/semixparent.o: \
  $(objdir)/gus_portrait.sb53 $(srcdir)/rectfill.inc \
  $(objdir)/shadow_reticle.chr.pb53 $(objdir)/hepsie.chr.pb53
$(objdir)/overclock.o: \
  $(srcdir)/rectfill.inc

# Generic rules for CHR

$(objdir)/%.chr: $(imgdir)/%.png
	tools/pilbmp2nes.py --planes "0;1" $< $@
$(objdir)/%16.chr: $(imgdir)/%.png
	tools/pilbmp2nes.py -H 16 --planes "0;1" $< $@
$(objdir)/%gray.sav: $(imgdir)/%.png
	tools/savtool.py --palette 0f0010200f1616160f1616160f161616 $< $@
$(objdir)/%.sb53: $(objdir)/%.sav
	tools/sav2sb53.py $< $@
$(objdir)/%.pb53: $(objdir)/%
	tools/pb53.py $< $@

# Specific rules for CHR

$(objdir)/gus_sprite.chr: $(imgdir)/gus_sprite.png
	tools/pilbmp2nes.py -H 128 --planes "0;1" $< $@
$(objdir)/fizzter_digits.chr: $(imgdir)/fizzter_digits.png
	tools/pilbmp2nes.py -H 24 --planes "0;1" $< $@
$(objdir)/gus_bg.sav: $(imgdir)/gus_bg.png
	tools/savtool.py --palette 0f0820180f1a202a0f060f160f0f1424 $< $@
$(objdir)/lag_clock_face.sav: $(imgdir)/lag_clock_face.png
	tools/savtool.py --palette 20100f24202424242021112420261624 $< $@
$(objdir)/vwf7.s: $(imgdir)/vwf7.png
	tools/vwfbuild.py $< $@
$(objdir)/gus_portrait.sav: $(imgdir)/gus_portrait.png
	tools/savtool.py --palette 0F2012370F2028100F2027370F122810 $< $@
$(objdir)/greenhillzone.sb53: $(imgdir)/greenhillzone.png
	tools/widesb53.py $< 2208322022081A29220818282208181A $@

# Help me!
$(objdir)/%.hlp.s: $(srcdir)/%.txt
	tools/paginate_help.py $< > $@


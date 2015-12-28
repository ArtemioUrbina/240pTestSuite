#!/usr/bin/env python3
from __future__ import with_statement, division, print_function
import array, os, sys
from binascii import a2b_hex
from PIL import Image, ImageChops
import chnutils, pilbmp2nes, savtool
from pb53 import pb53

def bmptowidesb53(infilename, palette, outfilename):
    im = Image.open(infilename)
    if im.size[0] != 512:
        raise ValueError("Image width is %d pixels (expected 512)"
                         % im.size[0])
    if im.size[1] != 240:
        raise ValueError("Image height is %d pixels (expected 240)"
                         % im.size[0])

    # Quantize picture to palette
    palette = b''.join(palette[0:1] + palette[i + 1:i + 4]
                       for i in range(0, 16, 4))
    palettes = [[tuple(savtool.bisqpal[r]) for r in palette[i:i + 4]]
                 for i in range(0, 16, 4)]
    imf, attrs = savtool.colorround(im, palettes)

    # Convert to unique tiles    
    chrdata = pilbmp2nes.pilbmp2chr(imf, 8, 8)
    chrdata, linear_namdata = chnutils.dedupe_chr(chrdata)
    print("%d distinct tiles" % len(chrdata))

    # Split into separate 32x32 nametables
    nametables = [[linear_namdata[i:i + 32]
                   for i in range(x, len(linear_namdata), im.size[0] // 8)]
                  for x in range(0, im.size[0] // 8, 32)]
    nametables = [bytes(b for row in nt for b in row) for nt in nametables]

    # Pack attributes into bytes
    if len(attrs) % 2:
        attrs.append([0] * len(attrs[0]))
    attrs = [[lc | (rc << 2) for lc, rc in zip(row[0::2], row[1::2])]
             for row in attrs]
    attrs = [[tc | (bc << 4) for (tc, bc) in zip(t, b)]
             for (t, b) in zip(attrs[0::2], attrs[1::2])]
    # Split into separate 32x32 nametables
    attrs = [bytes(b for row in attrs for b in row[i:i + 8])
             for i in range(0, len(attrs[0]), 8)]
    print([len(x) for x in attrs])

    outdata = [
        bytearray([len(chrdata) & 0xFF]),
        pb53(b''.join(chrdata), copyprev=False)[0]
    ]
    outdata.extend(pb53(nt + at, copyprev=False)[0]
                   for nt, at in zip(nametables, attrs))
    outdata.append(palette)
    with open(outfilename, 'wb') as outfp:
        outfp.writelines(outdata)

usagemsg = """usage: %s INFILE PALETTE OUTFILE

INFILE -- path to indexed PNG file
PALETTE -- NES palette string of 32 hex digits
OUTFILE -- path to output c512 file
"""

def main(argv=None):
    argv = argv or sys.argv
    if len(argv) != 4:
        progname = os.path.basename(argv[0])
        if len(argv) > 1 and argv[1] == '--help':
            print(usagemsg % progname)
            return
        print("%s: wrong number of arguments; try %s --help"
              % (progname, progname), file=sys.stderr)
        sys.exit(1)
    infilename = argv[1]
    palette = bytes.fromhex(argv[2])
    outfilename = argv[3]
    bmptowidesb53(infilename, palette, outfilename)

if __name__=='__main__':
    main()
##    main(["widesb53.py", "../tilesets/greenhillzone.png",
##          "2232201622081A292208181622161616", "../obj/nes/greenhillzone.sb53"])

#!/usr/bin/env python3
#
# UNROM to BNROM converter for 240p test suite
# Copyright 2015 Damian Yerrick
#
# Copying and distribution of this file, with or without
# modification, are permitted in any medium without royalty
# provided the copyright notice and this notice are preserved.
# This file is offered as-is, without any warranty.
#
import sys, os

usageMsg = """Usage: %s INFILE OUTFILE
Converts an NES UNROM image that uses only banks 0 and 1
(at $8000) and bank 3 (at $C000) to BNROM.

Options:
    --help, -h: show this message
    --version: Show version and copyright notices
"""
versionMsg = """UNROM to BNROM converter from 240p test suite 0.09
Copyright 2015 Damian Yerrick.  This is free software; see the source for
details.  There is ABSOLUTELY NO WARRANTY to the extent permitted by law.  
"""

def main(argv=None):
    argv = argv or sys.argv
    progname = os.path.basename(argv[0])
    if len(argv) > 1 and argv[1] in ('-h', '/?', '-?', '--help'):
        print(usageMsg % (progname,))
        return
    if len(argv) > 1 and argv[1] == 'version' in ('-h', '/?', '-?', '--help'):
        print(versionMsg)
        return
    if len(argv) != 3:
        print("%s: wrong number of filenames; try %s --help"
              % (progname, progname), file=sys.stderr)
        sys.exit(1)

    infilename, outfilename = argv[1:3]

    with open(infilename, 'rb') as infp:
        header = bytearray(infp.read(16))
        banks = [infp.read(16384) for i in range(4)]
    if header[4] != 4 or header[5] != 0:
        print("%s: expected 64K PRG and 0K CHR but got %dK PRG and %dK CHR"
              % (infilename, header[4] * 16, header[5] * 8), file=sys.stderr)
        sys.exit(1)

    # Change mapper to BNROM
    header[6] = (header[6] & 0x0F) | 0x20  # lower nibble
    header[7] = (header[7] & 0x0F) | 0x20  # upper nibble

    # Bank 2 is empty, so rearrange banks into order 0, 3, 1, 3
    # so that the fixed bank
    out = [header, banks[0], banks[3], banks[1], banks[3]]
    with open(outfilename, 'wb') as outfp:
        outfp.writelines(out)

if __name__=='__main__':
    main()

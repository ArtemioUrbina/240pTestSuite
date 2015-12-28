#!/usr/bin/env python3
"""
iNES to raw binary tool for python 2.7 and 3.x

Copyright 2015 Damian Yerrick
Copying and distribution of this file, with or without
modification, are permitted in any medium without royalty
provided the copyright notice and this notice are preserved.
This file is offered as-is, without any warranty.
"""
from __future__ import with_statement, division, print_function
import sys, os

eprom_names = {
    8192: '27C64',
    16384: '27C128',
    32768: '27C256',
    65536: '27C512',
    131072: '27C010',
    262144: '27C020',
    524288: '27C040'
}

default_filename = "../240pee.nes"
usage_msg = """usage: %s [FILE1.nes [FILE2.nes ...]]
Converts an NES program in iNES format to raw PRG and CHR binary files,
doubling the data to fill the entire PRG ROM.
If no ROM filename given, converts %s"""

def doubleup(data, nameprefix, ext):
    if not data:
        return
    maxsize = max(eprom_names)
    while len(data) <= maxsize:
        try:
            eprom_name = eprom_names[len(data)]
        except KeyError:
            print("no EPROM name for %d bytes" % len(prgrom), file=sys.stderr)
        filename = '%s-%s.%s' % (nameprefix, eprom_name, ext)
        with open(filename, 'wb') as outfp:
            outfp.write(data)
        data = data + data

def main(argv=None):
    argv = argv or sys.argv
    if len(argv) < 2:
        filenames = [default_filename]
    elif argv[1] in ['-h', '-?', '--help', '/?']:
        progname = os.path.basename(argv[0])
        print(usage_msg % (progname, default_filename))
        return
    else:
        filenames = argv[1:]

    for infilename in filenames:
        with open(infilename, "rb") as infp:
            romdata = infp.read()
        header = bytearray(romdata[:16])
        prgsize = header[4] * 16384
        chrsize = header[5] * 8192

        pfx, oldext = os.path.splitext(infilename)
        prgrom = romdata[16:16 + prgsize]
        doubleup(prgrom, pfx, "prg")
        chrrom = romdata[16 + prgsize:16 + prgsize + chrsize]
        doubleup(chrrom, pfx, "chr")

if __name__=='__main__':
    main()

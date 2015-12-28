#!/usr/bin/env python3
import json, sys, re
from vwfbuild import ca65_bytearray

MAXPAGELEN = 20

nonalnumRE = re.compile("[^0-9a-zA-Z]+")

decode_240ptxt_table = {
    128: '\u00A9',  # copyright
    129: '\U0001F426',  # bird
    132: '\u2191',  # up
    133: '\u2193',  # down
    134: '\u2190',  # left
    135: '\u2192',  # right
}

def decode_240ptxt(blo, errors='strict'):
    out, firsterr, lasterr = [], None, 0
    for i, bvalue in enumerate(blo):
        # values 0-127 are same as ascii
        if bvalue < 128:
            out.append(chr(bvalue))
            continue
        try:
            out.append(decode_240ptxt_table[bvalue])
        except KeyError:
            lasterr = i
            if firsterr is not None: firsterr = i
            if errors != 'ignore': out.append('\uFFFD')
    if firsterr is not None and errors == 'strict':
        raise UnicodeDecodeError(encoding='240ptxt',
                                 reason='no code point for 0x%02x'
                                        % blo[firsterr],
                                 object=blo, start=firsterr, end=lasterr + 1)
    return ''.join(out)

encode_240ptxt_table = dict((v, k) for k, v in decode_240ptxt_table.items())

def encode_240ptxt(s, errors='strict'):
    out, firsterr, lasterr = bytearray(), None, 0
    for i, ch in enumerate(s):
        # values 0-127 are same as ascii
        if ch < '\u0080':
            out.append(ord(ch))
            continue
        try:
            out.append(encode_240ptxt_table[ch])
        except KeyError:
            lasterr = i
            if firsterr is not None: firsterr = i
            if errors != 'ignore': out.append('?')
    if firsterr is not None and errors == 'strict':
        raise UnicodeEncodeError(encoding='240ptxt',
                                 reason='no encoding for U+%04x'
                                        % ord(s[firsterr]),
                                 object=blo, start=firsterr, end=lasterr + 1)
    return bytes(out)

def lines_to_docs(lines):
    docs = []
    secttitles = {}
    for linenum, line in enumerate(lines):
        if line.startswith('==') and line.endswith('=='):
            # New section
            secttitle = line.strip('=').strip()
            normtitle = nonalnumRE.sub('_', secttitle.lower()).strip('_')
            try:
                oldsection = secttitles[normtitle]
            except KeyError:
                pass
            else:
                oldsecttitle, oldlinenum = oldsection
                raise ValueError("%s:%d: %s was already defined on line %d")
            secttitles[normtitle] = (secttitle, linenum)
            docs.append((secttitle, normtitle, [[]]))
            continue
        docpages = docs[-1][-1] if docs else None
        doclastpage = docpages[-1] if docpages else None
        
        if line == '':
            # Blank line; append only if following a nonblank line
            if doclastpage and doclastpage[-1]:
                doclastpage.append('')
            continue
        if doclastpage is None:
            raise IndexError("%s:%d: nonblank line with no open document"
                             % (filename, linenum + 1))
        if line.startswith('----') and line.rstrip('-') == '':
            # Page break
            if doclastpage:
                docpages.append([])
            continue
        if len(doclastpage) >= MAXPAGELEN:
            raise IndexError("%s:%d: exceeds page length of %d lines"
                             % (filename, linenum + 1, MAXPAGELEN))
        doclastpage.append(line)

    for doc in docs:
        pages = doc[-1]

        # Remove trailing blank lines
        for page in pages:
            while page and not page[-1]: del page[-1]

        # Remove blank pages
        for i in range(len(pages) - 1, -1, -1):
            if not pages[i]: del pages[i]

    # Remove blank docs entirely
    for i in range(len(docs) - 1, -1, -1):
        if not docs[i][-1]:
            del docs[i]

    return docs

def ca65_escape_bytes(blo):
    """Encode an iterable of ints in 0-255, mostly ASCII, for ca65 .byte statement"""
    runs = []
    for c in blo:
        if 32 <= c <= 126 and c != 34:
            if runs and isinstance(runs[-1], bytearray):
                runs[-1].append(c)
            else:
                runs.append(bytearray([c]))
        else:
            runs.append(c)
    return ','.join('"%s"' % r.decode('ascii')
                    if isinstance(r, bytearray)
                    else '%d' % r
                    for r in runs)

def render_help(docs):
    lines = ["""
; Help data generated with paginate_help.py - do not edit
.export helptitles_hi, helptitles_lo
.export helppages_hi, helppages_lo, help_cumul_pages
.exportzp HELP_NUM_PAGES, HELP_NUM_SECTS, HELP_BANK

.segment "BANK00"
HELP_BANK = 0
"""]
    lines.extend('.exportzp helpsect_%s' % (doc[1])
                 for i, doc in enumerate(docs))
    lines.extend('helpsect_%s = %d' % (doc[1], i)
                 for i, doc in enumerate(docs))
    lines.append('helptitles_hi:')
    lines.extend('  .byte >helptitle_%s' % doc[1] for doc in docs)
    lines.append('helptitles_lo:')
    lines.extend('  .byte <helptitle_%s' % doc[1] for doc in docs)
    lines.extend('helptitle_%s: .byte %s,0'
                 % (doc[1], ca65_escape_bytes(encode_240ptxt(doc[0])))
                 for doc in docs)

    cumul_pages = [0]
    pagenum = 0
    for doc in docs:
        for page in doc[-1]:
            lines.append("helppage_%03d:" % pagenum)
            page = [bytearray(encode_240ptxt(line)) for line in page]
            for i in range(len(page) - 1):
                page[i].append(10)  # newline
            page[-1].append(0)
            lines.extend("  .byte %s" % ca65_escape_bytes(line)
                         for line in page)
            pagenum += 1
        assert pagenum == cumul_pages[-1] + len(doc[-1])
        cumul_pages.append(pagenum)
    lines.append('help_cumul_pages:')
    lines.append(ca65_bytearray(cumul_pages))
    lines.append('HELP_NUM_PAGES = %d' % cumul_pages[-1])
    lines.append('HELP_NUM_SECTS = %d' % len(docs))
    lines.append('helppages_hi:')
    lines.extend('  .byte >helppage_%03d' % i for i in range(cumul_pages[-1]))
    lines.append('helppages_lo:')
    lines.extend('  .byte <helppage_%03d' % i for i in range(cumul_pages[-1]))

    return "\n".join(lines)

def main(argv=None):
    argv = argv or sys.argv
    if len(argv) > 1 and argv[1] == '--help':
        print("usage: paginate_help.py INFILE.txt")
        return
    if len(argv) != 2:
        print("paginate_help.py: wrong number of arguments; try paginate_help.py --help")
        sys.exit(1)

    filename = argv[1]
    with open(filename, 'r') as infp:
        lines = [line.rstrip() for line in infp]
    docs = lines_to_docs(lines)
##    print(json.dumps(docs, indent=2))
    print(render_help(docs))

if __name__=='__main__':
##    main(['paginate_help.py', '../src/helppages.txt'])
    main()

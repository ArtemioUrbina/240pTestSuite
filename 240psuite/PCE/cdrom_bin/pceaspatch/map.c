#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "defs.h"
#include "externs.h"
#include "protos.h"

/* locals */
static unsigned char buffer[512];


/* ----
 * pce_load_map()
 * ----
 */
extern char globalname[];

int
pce_load_map(char *fname, int mode)
{
	FILE *fp;
	unsigned char header[16];
	int fsize;
	int size;
	int nb;
	int cnt;
	int i, j;
	FILE *out;
	int pos = 0;

	/* init */
	cnt = 0;

	/* open the file */
	if ((fp = open_file(fname, "rb")) == NULL) {
		fatal_error("Can not open file!");
		return (1);
	}

	/* check FMP header */
	fread(header, 1, 12, fp);
	fsize = (header[4] << 24) +
			(header[5] << 16) +
			(header[6] <<  8) +
			 header[7] - 4;

	if (memcmp(header, "FORM", 4) || memcmp(&header[8], "FMAP", 4)) {
		/* incorrect header */
		if (mode)
			fatal_error("Invalid FMP format!");
		fclose(fp);
		return(mode);
	}

	/* define label */
	labldef(loccnt, 1);

	/* output */
	if (pass == LAST_PASS)
		loadlc(loccnt, 0);

	/* get pcx file name */
	strcpy(globalname, fname+12);
	pos = strlen(globalname) - 4;
	globalname[pos++] = '_';
	globalname[pos++] = 'm';
	globalname[pos++] = 'a';
	globalname[pos++] = 'p';
	globalname[pos++] = '.';
	globalname[pos++] = 'b';
	globalname[pos++] = 'i';
	globalname[pos++] = 'n';
	globalname[pos++] = '\0';

	out = fopen(globalname, "wb");
	if(!out)
	{
		printf("could not open output file %s\n", globalname);
		return 0;
	}
	/* browse chunks */
	while (fsize > 0) {
		fread(header, 1, 8, fp);
		size = (header[4] << 24) +
			   (header[5] << 16) +
			   (header[6] <<  8) +
			    header[7];
		fsize -= 8;
		fsize -= size;

		/* BODY chunk */
		if (memcmp(header, "BODY", 4) == 0) {
			/* add size */
			cnt += (size >> 1);

			if (pass == LAST_PASS) {
				/* read chunk */
				while (size) {
					/* read a block */
					nb = (size > 512) ? 512 : size;
					fread(buffer, 1, nb, fp);
					size -= nb;
					nb >>= 1;

					/* convert word -> byte */
					for (i = 0, j = 0; i < nb; i++, j += 2)
						buffer[i] = ((buffer[j] + (buffer[j+1] << 8)) >> 5) - 1;

					/* output buffer */
					putbuffer(buffer, nb);
					if(out)
						fwrite(buffer, 1, nb, out);
				}
			}
			else
			{
				putbuffer(NULL, size >> 1);
				if(out)
				{
					int i = 0;
					for(i = 0; i < size >> 1; i++)
						fputc(0, out);
				}
			}

			/* ok */
			break;
		}
		else {
			if (fsize > 0)
			{
				/* unsupported chunk */
				fseek(fp, size, SEEK_CUR);
			}
		}

		if(fsize < 0)
			break;
	}

	/* close file */
	fclose(fp);
	if(out)
		fclose(out);

	/* size */
	if (lablptr) {
		lablptr->data_type = P_INCBIN;
		lablptr->data_size = cnt;
	}
	else {
		if (lastlabl) {
			if (lastlabl->data_type == P_INCBIN)
				lastlabl->data_size += cnt;
		}
	}

	/* output line */
	if (pass == LAST_PASS)
		println();

	/* ok */
	return (1);
}




//The FMP file format
//The first 12 bytes are as follows:
//4bytes ASCII = 'FORM'
//long int = size of file less header (which is filesize-8)
//4bytes ASCII = 'FMAP'
//NOTE:  The chunk size long ints like the one above are stored in
//'Motorola' format, NOT Intel.  You will have to byteswap to get the
//correct value, ie:  Bytes 1,2,3,4 need to become 4,3,2,1.
//
//The chunks in the file follow on one after the other, and consist of
//an 8byte header, and the information specific to that chunk.  See
//how the playback source reads in the information.  The chunks can be
//in any order, and some chunks may not be used in a particular file.
//
//Chunk header:
//4bytes ASCII = ChunkID (example: 'MPHD')
//long int = size of chunk data less header
//
//These are the chunks as of V1.1:
//ATHR - Up to 4 ASCII strings of author information, separated by 0 values,
//       always an even size.
//MPHD - Map header, see struct in mappy.c
//EDHD - Editor information, see struct in mappy.c
//CMAP - Colour palette for 8bit maps, red byte, green byte,
//       blue byte for however many colours are needed (so usually 256*3 bytes).
//BKDT - Block data. Contains BLKSTR structures for however many block
//       structures were made.
//ANDT - Animation data. Contains ANISTR structures for however many
//       animation structures were made, and also animation data.
//BGFX - The raw graphics in whatever format the map is in.
//       Examples: 8bit: mapwidth*mapheight bytes per block,
//       in forward format *numblocks 16bit: mapwidth*mapheight*2
//       bytes per block, each word contains 5 bits red, 6 bits green,
//       5 bits blue.
//BODY - An array of short ints containing positive offsets into BKDT,
//       and negative offsets into ANDT.
//LYR? - Where ? is an ASCII number form 1 to 7.
//       These are the same size and format as BODY, and allow object
//       layers to be used.


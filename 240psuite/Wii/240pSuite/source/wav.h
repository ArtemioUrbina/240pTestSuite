/* 
 * 240p Test Suite
 * Copyright (C)2023 Artemio Urbina (Wii GX)
 *
 * This file is part of the 240p Test Suite
 *
 * The 240p Test Suite is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The 240p Test Suite is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 240p Test Suite; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
 
 
/********************************************************/
/* WAV data structures */

typedef struct	RIFF_HEADER
{
	/* RIFF Chunk Descriptor */
	uint8_t 		RIFF[4];		/* RIFF Header Magic header */
	uint32_t		ChunkSize;		/* RIFF Chunk Size */
	uint8_t 		WAVE[4];		/* WAVE Header */
} riff_hdr;

typedef struct	SYB_CHUNK
{
	/* sub-chunk */
	uint8_t 		chunkID[4]; 	/* Chunk ID */
	uint32_t		Size;			/* Size of the SubChunk */
} sub_chunk;

#define	WAVE_FORMAT_PCM			0x0001
#define	WAVE_FORMAT_IEEE_FLOAT	0x0003
#define	WAVE_FORMAT_ALAW		0x0006
#define	WAVE_FORMAT_MULAW		0x0007
#define WAVE_FORMAT_EXTENSIBLE	0xFFFE

#define	FMT_TYPE_1	16
#define	FMT_TYPE_2	18
#define	FMT_TYPE_3	40

#define	FMT_TYPE_1_SIZE	0
#define	FMT_TYPE_2_SIZE	2
#define	FMT_TYPE_3_SIZE	24

#define	FMT_EXTRA_SIZE 24

typedef struct	FMT_HEADER
{
	/* "fmt" sub-chunk*/
	uint8_t 		fmt[4]; 		/* FMT header */
	uint32_t		Subchunk1Size;	/* Size of the fmt chunk */
	uint16_t		AudioFormat;	/* Audio format 1=PCM,6=mulaw,7=alaw,	  257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM */
	uint16_t		NumOfChan;		/* Number of channels 1=Mono 2=Sterio */
	uint32_t		SamplesPerSec;	/* Sampling Frequency in Hz */
	uint32_t		bytesPerSec;	/* bytes per second */
	uint16_t		blockAlign; 	/* 2=16-bit mono, 4=16-bit stereo */
	uint16_t		bitsPerSample;	/* Number of bits per sample */
} fmt_hdr;

typedef struct	FMT_HEADER_EXT1
{
	/* "fmt" sub-chunk */
	uint16_t		extSize;		/* Size of the extension: 18 */
} fmt_hdr_ext1;

typedef struct	FMT_HEADER_EXT2
{
	/* "fmt" sub-chunk */
	uint16_t		extSize;		/* Size of the extension: 22 */
	uint16_t		wValidBitsPerSample; /* at most 8*M */
	uint32_t		dwChannelMask;	/* Speaker position mask */
	/*	GUID (16 bytes, first two bytes are the data format code) */
	uint16_t		formatCode; /* formatcode */
	uint8_t			SubFormat[14];
} fmt_hdr_ext2;

typedef struct	DATA_HEADER
{
	/* "data" sub-chunk */
	uint8_t 		DataID[4]; /* "data"  string */
	uint32_t		DataSize;	/* Sampled data length */
} data_hdr;

typedef struct	FACT_CK
{
	/* "fact" sub-chunk for WAVE_FORMAT_EXTENSIBLE */
	uint8_t 		DataID[4]; /* "fact"  string */
	uint32_t		DataSize;	/* Chunk size: minimum 4 */
	uint32_t		dwSampleLength;  /* Number of samples (per channel) */
} fact_ck;

typedef struct	WAV_HEADER
{
	riff_hdr	riff;
	fmt_hdr		fmt;
	data_hdr	data;
} wav_hdr;


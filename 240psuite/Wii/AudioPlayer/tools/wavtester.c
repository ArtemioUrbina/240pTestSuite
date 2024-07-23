#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
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
	/*  GUID (16 bytes, first two bytes are the data format code) */
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

/********************************************************/

void CloseFS()
{
}

int InitFS()
{
	return 1;
}

#define ulong unsigned long

void printbinary(unsigned long long n, char *b, unsigned int size)
{
    char *p = b + size;
    *--p = '\0';
    for (; p-- > b;  n >>= 1) {
        *p = '0' + (char)(n & 1);
    }
}

void DrawData(char *title, int data)
{
	char 			msg[33];
	printbinary(data, msg, 33);
	printf("%s: %d (%s)\n", title, data, msg);
}

uint32_t EndianCorrect32bits(uint32_t num)
{
	uint32_t swapped = 0;
	
	/*
	swapped = ((num>>24)&0xff) | // move byte 3 to byte 0
              ((num<<8)&0xff0000) | // move byte 1 to byte 2
              ((num>>8)&0xff00) | // move byte 2 to byte 1
              ((num<<24)&0xff000000); // byte 0 to byte 3
	*/
	swapped = num;
	return swapped;
}

void cleanup(FILE *file, int *errType, int val)
{
	fclose(file);
	CloseFS();
	*errType = val;
}

#define HEADER_BUFFER 4096

int copyFromBuffer(void *dest, uint8_t *buffer, size_t size, int *buffpos, FILE *file, int *errType, int errCode)
{
	DrawData("err", errCode);
	DrawData("start buffer", *buffpos);
	if(*buffpos + size > HEADER_BUFFER)
	{
		cleanup(file, errType, errCode);
		free(buffer);
		return 0;
	}
	
	memcpy(dest, buffer+*buffpos, size);
	*buffpos += size;
	DrawData("end buffer", *buffpos);
	return 1;
}

uint8_t *LoadPCMToBuffer(char *filename, ulong *size, int *errType)
{
	FILE *file = NULL;
	ulong file_size = 0;
	uint8_t *file_buffer = NULL, *hbuffer = NULL;
	riff_hdr riff;
	fmt_hdr fmt;
	fmt_hdr_ext1 fmtExtra;
	fmt_hdr_ext2 fmtExtra2;
	data_hdr data;
	int found = 0, buffpos = 0;
	
	*errType = 0;
	
	if(!InitFS())
		return NULL;

	file = fopen(filename, "rb");
	if(!file)
	{
		CloseFS();
		*errType = 1;
		return NULL;
	}
	
	hbuffer = (uint8_t*)malloc(sizeof(uint8_t)*HEADER_BUFFER);
	if(!hbuffer)
	{
		cleanup(file, errType, 2);
		return NULL;
	}
	
	if(fread(hbuffer, sizeof(uint8_t), HEADER_BUFFER, file) != sizeof(uint8_t)*HEADER_BUFFER)
	{
		cleanup(file, errType, 3);
		return NULL;
	}
	
	rewind(file);
	
	if(!copyFromBuffer(&riff, hbuffer, sizeof(riff_hdr), &buffpos, file, errType, 4))
		return NULL;	
	
	fmt.Subchunk1Size = EndianCorrect32bits(fmt.Subchunk1Size);
	if(strncmp((char*)riff.RIFF, "RIFF", 4) != 0)
	{
		cleanup(file, errType, 5);
		return NULL;
	}
	
	if(strncmp((char*)riff.WAVE, "WAVE", 4) != 0)
	{
		cleanup(file, errType, 6);
		return NULL;
	}
	
	// look for fmt chunk
	found = 0;
	do
	{
		sub_chunk	schunk;

		if(!copyFromBuffer(&schunk, hbuffer, sizeof(sub_chunk), &buffpos, file, errType, 7))
			return NULL;
		
		schunk.Size = EndianCorrect32bits(schunk.Size);
		if(strncmp((char*)schunk.chunkID, "fmt", 3) != 0)
		{
			buffpos += schunk.Size*sizeof(uint8_t);
		}
		else
		{
			buffpos -= sizeof(sub_chunk);
			found = 1;
		}
	}while(!found);
	
	if(!copyFromBuffer(&fmt, hbuffer, sizeof(fmt_hdr), &buffpos, file, errType, 8))
		return NULL;
		
	fmt.bytesPerSec = EndianCorrect32bits(fmt.bytesPerSec);
	fmt.SamplesPerSec = EndianCorrect32bits(fmt.SamplesPerSec);
	fmt.Subchunk1Size = EndianCorrect32bits(fmt.Subchunk1Size);
	
	if(fmt.NumOfChan != 2)
	{
		cleanup(file, errType, 9);
		return NULL;
	}
	
	if(fmt.AudioFormat != WAVE_FORMAT_PCM)
	{
		cleanup(file, errType, 10);
		return NULL;
	}
	
	if(fmt.bitsPerSample != 16)
	{
		cleanup(file, errType, 11);
		return NULL;
	}
	
	if(fmt.SamplesPerSec != 48000)
	{
		cleanup(file, errType, 12);
		return NULL;
	}
	
	switch(fmt.Subchunk1Size)
	{
		case FMT_TYPE_1:
			break;
		case FMT_TYPE_2:
			if(!copyFromBuffer(&fmtExtra, hbuffer, sizeof(fmt_hdr_ext1), &buffpos, file, errType, 13))
				return NULL;
			break;
		case FMT_TYPE_3:
			if(!copyFromBuffer(&fmtExtra2, hbuffer, sizeof(fmt_hdr_ext2), &buffpos, file, errType, 14))
				return NULL;
			break;
		default:
			if(fmt.Subchunk1Size + 8 > sizeof(fmt_hdr))  // Add the fmt and chunksize length: 8 bytes
				buffpos += fmt.Subchunk1Size + 8 - sizeof(fmt_hdr);
			break;
	}
	
	// look for data chunk
	found = 0;
	do
	{
		sub_chunk	schunk;

		if(!copyFromBuffer(&schunk, hbuffer, sizeof(sub_chunk), &buffpos, file, errType, 15))
			return NULL;
		schunk.Size = EndianCorrect32bits(schunk.Size);
		if(strncmp((char*)schunk.chunkID, "data", 4) != 0)
		{
			buffpos += schunk.Size*sizeof(uint8_t);
		}
		else
		{
			buffpos -= sizeof(sub_chunk);
			found = 1;
		}

		// Read Total Fact if available
		if(fmt.AudioFormat == WAVE_FORMAT_EXTENSIBLE)
		{			
			cleanup(file, errType, 16);
			return NULL;
		}
	}while(!found);
	
	if(!copyFromBuffer(&data, hbuffer, sizeof(data_hdr), &buffpos, file, errType, 17))
		return NULL;
	
	data.DataSize = EndianCorrect32bits(data.DataSize);
	if(data.DataSize == 0)
	{
		cleanup(file, errType, 18);
		return NULL;
	}
	
	// advance file to position
	if(fread(hbuffer, sizeof(uint8_t), buffpos, file) != sizeof(uint8_t)*buffpos)
	{
		free(hbuffer);
		cleanup(file, errType, 19);
		return NULL;
	}
	
	free(hbuffer);
	hbuffer= NULL;

	file_size = data.DataSize;
	
	file_buffer = (uint8_t*)malloc(sizeof(uint8_t)*file_size);
	if(!file_buffer)
	{		
		cleanup(file, errType, 20);
		return NULL;
	}
	
	if(fread(file_buffer, sizeof(uint8_t), file_size, file) != file_size) 
	{		
		cleanup(file, errType, 21);
		return NULL;
	}
	
	fclose(file);
	CloseFS();
	
	*size = file_size;
	return file_buffer;
}

uint8_t *LoadFileToBuffer(char *filename, ulong *size)
{
	FILE *file = NULL;
	ulong file_size = 0;
	uint8_t *file_buffer = NULL;
	
	if(!InitFS())
		return NULL;

	file = fopen(filename, "r");
	if(!file)
	{
		CloseFS();
		return NULL;
	}
	fseek(file , 0, SEEK_END);
	file_size = ftell(file);	
	rewind(file);
	
	if(!file_size)
	{		
		fclose(file);
		CloseFS();
		return NULL;
	}
	
	file_buffer = (uint8_t*)malloc(sizeof(uint8_t)*file_size);
	if(!file_buffer)
	{		
		fclose(file);
		CloseFS();
		return NULL;
	}
	
	if(fread(file_buffer, sizeof(uint8_t), file_size, file) != file_size) 
	{		
		fclose(file);
		CloseFS();
		return NULL;
	}
	
	fclose(file);
	CloseFS();
	
	*size = file_size;
	return file_buffer;
}

int main() 
{
	int errType = 0;
	ulong size = 0;
	char errCode[3];
	uint8_t *data = NULL;
	
	data = LoadPCMToBuffer("a.wav", &size, &errType);
	if(!data)
	{
		errCode[0] = errType / 10 + '0';
		errCode[1] = errType % 10 + '0';
		errCode[2] = '\0';

		printf("Err: %d\n", errType);
		printf(errCode);
	}
	else
	{
		free(data);
		data = NULL;
		printf("Loaded!");
	}
	return 0;
}

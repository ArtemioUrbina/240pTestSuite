#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned long SumBytes(char *buffer, unsigned long len)
{
	unsigned long	sum = 0, i = 0;

	for(i = 0; i < len; i++)
		sum += (unsigned char)buffer[i];
	return sum;
}

unsigned long MirrorSum(char *buffer, unsigned long len, unsigned long target)
{
	unsigned long	pow2 = 1;

	if(!len)
		return 0;

	while(pow2 * 2 <= len)
		pow2 *= 2;

	if(pow2 == len)
		return SumBytes(buffer, len) * (target / len);

	return SumBytes(buffer, pow2) + MirrorSum(buffer + pow2, len - pow2, target - pow2);
}

int main(int argc, char **argv)
{
	FILE			*sfile, *nfile;
	char 			*buffer = NULL;
	long 			size = 0;
	unsigned long	pow2 = 1, kb = 0, sum = 0;
	unsigned char	country = 0x02, romsize = 0;

	if(argc < 3)
	{
		fprintf(stderr, "Please specify an input and output sfc file\n");
		return 1;
	}

	if(argc > 3)
		country = (unsigned char)strtoul(argv[3], NULL, 16);

	sfile = fopen(argv[1], "rb");
	if(!sfile)
	{
		fprintf(stderr, "Could not open source ROM %s\n", argv[1]);
		return 1;
	}

	fseek(sfile, 0L, SEEK_END);
	size = ftell(sfile);
	if(size < 0x8000)
	{
		fclose(sfile);
		fprintf(stderr, "Not a valid ROM file %s\n", argv[1]);
		return 1;
	}

	buffer = (char*)malloc(sizeof(char)*size);
	if(!buffer)
	{
		fclose(sfile);
		fprintf(stderr, "Could not load %s ROM file to RAM\n", argv[1]);
		return 1;
	}
	fseek(sfile, 0L, SEEK_SET);
	memset(buffer, 0x0, sizeof(char)*size);
	if(fread(buffer, sizeof(char), size, sfile) != (size_t)size)
	{
		fclose(sfile);
		free(buffer);
		fprintf(stderr, "Could not load %s ROM file to RAM\n", argv[1]);
		return 1;
	}
	fclose(sfile);

	// ROM size, rounded up to the next power of two (768KB -> 0x0A)
	kb = (size + 1023) / 1024;
	while((1UL << romsize) < kb)
		romsize++;
	buffer[0x7fd7] = romsize;

	// Region
	buffer[0x7fd9] = country;

	// Checksum, mirror non power of two ROMs as the hardware sees them
	buffer[0x7fdc] = (char)0xff;
	buffer[0x7fdd] = (char)0xff;
	buffer[0x7fde] = 0x00;
	buffer[0x7fdf] = 0x00;

	while(pow2 < (unsigned long)size)
		pow2 *= 2;
	sum = MirrorSum(buffer, size, pow2) & 0xffff;

	buffer[0x7fdc] = (char)(~sum & 0xff);
	buffer[0x7fdd] = (char)((~sum >> 8) & 0xff);
	buffer[0x7fde] = (char)(sum & 0xff);
	buffer[0x7fdf] = (char)(sum >> 8);

	nfile = fopen(argv[2], "wb");
	if(!nfile)
	{
		free(buffer);
		fprintf(stderr, "Could not open target ROM %s\n", argv[2]);
		return 1;
	}

	fwrite(buffer, sizeof(char), size, nfile);
	free(buffer);
	fclose(nfile);

	printf("%s: %ldKB ROM size 0x%02X region 0x%02X checksum 0x%04lX\n", 
		argv[2], size / 1024, romsize, country, sum);
	return 0;
}

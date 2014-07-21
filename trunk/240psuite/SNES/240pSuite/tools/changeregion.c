#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	FILE    *sfile, *nfile;
	char 	*buffer = NULL;
	long 	size = 0;

	if(argc < 3)
    {
        fprintf(stderr, "Please specify an inpuit and output sfc file\n");
        return 0;
    }

    sfile = fopen(argv[1], "r");
    if(!sfile)
    {
        fprintf(stderr, "Could not open source ROM %s\n", argv[1]);
        return 0;
    }

	fseek(sfile, 0L, SEEK_END);
	size = ftell(sfile)+1;
	buffer = (char*)malloc(sizeof(char)*size);
	if(!buffer)
	{
		fclose(sfile);
		fprintf(stderr, "Could not load %s ROM file to RAM\n", argv[1]);
		return 0;
	}
	fseek(sfile, 0L, SEEK_SET);
	memset(buffer, 0x0, sizeof(char)*size);
	fread(buffer, sizeof(char), size-1, sfile);
	fclose(sfile);

	if(size < 0x7fd9)
	{
		fprintf(stderr, "Not a valid ROM file %s\n", argv[1]);
		return 0;
	}
	
	nfile = fopen(argv[2], "w");
    if(!nfile)
    {
		fclose(sfile);
        fprintf(stderr, "Could not open target ROM %s\n", argv[2]);
        return 0;
    }

	if(buffer[0x7fd9] != 0x02)
		buffer[0x7fd9] = 0x02;

	fwrite(buffer, sizeof(char), size - 1, nfile);
	free(buffer);
	fclose(nfile);
	return 0;
}
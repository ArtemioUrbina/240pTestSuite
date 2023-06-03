#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	FILE	*crom1 = NULL, *crom2 = NULL, *nfile = NULL;
	char 	fnameb[100], *buffer1 = NULL, *buffer2 = NULL;
	long 	size1 = 0, size2 = 0, pos = 0;

	if(argc < 2)
	{
		fprintf(stderr, "Please specify input base names\n");
        fprintf(stderr, "Ex. %s 2501 for 2501-c1.c1 and 2501-c2.c2\n", argv[0]);
		return 0;
	}

    sprintf(fnameb, "%s-c1.c1", argv[1]);
	crom1 = fopen(fnameb, "rb");
	if(!crom1)
	{
		fprintf(stderr, "Could not open source CROM %s\n", fnameb);
		return 0;
	}

    sprintf(fnameb, "%s-c2.c2", argv[1]);
	crom2 = fopen(fnameb, "rb");
	if(!crom2)
	{
		fprintf(stderr, "Could not open source CROM %s\n", fnameb);
		return 0;
	}

	fseek(crom1, 0L, SEEK_END);
	size1 = ftell(crom1);
	
	fseek(crom2, 0L, SEEK_END);
    size2 = ftell(crom2);

	if(size1 != size2)
	{
		fprintf(stderr, "CROM files have different sizes %ld vs %ld\n", size1, size2);
        fclose(crom1);
        fclose(crom2);
		return 0;
	}
	
    fseek(crom1, 0L, SEEK_SET);
    buffer1 = (char*)malloc(sizeof(char)*(size1+1));
	if(!buffer1)
	{
		fprintf(stderr, "Could not load ROM file 1 to RAM\n");
		return 0;
	}
	memset(buffer1, 0x0, sizeof(char)*(size1+1));
	fread(buffer1, sizeof(char), size1, crom1);
    fclose(crom1);

    fseek(crom2, 0L, SEEK_SET);
    buffer2 = (char*)malloc(sizeof(char)*(size2+1));
	if(!buffer2)
	{
		fprintf(stderr, "Could not load ROM file 2 to RAM\n");
		return 0;
	}
    memset(buffer2, 0x0, sizeof(char)*(size2+1));
	fread(buffer2, sizeof(char), size2, crom2);
    fclose(crom2);

	nfile = fopen("crom0", "wb");
	if(!nfile)
	{
        free(buffer1);
        free(buffer2);
		fprintf(stderr, "Could not open target ROM file crom0\n");
		return 0;
	}

    for(pos = 0; pos < size1; pos+=2)
    {
        fwrite(buffer1+pos, sizeof(char), 2, nfile);
        fwrite(buffer2+pos, sizeof(char), 2, nfile);
    }

	fclose(nfile);
    free(buffer1);
    free(buffer2);

    if(argc > 2 && argv[2][0] == 'f')
    {
        FILE *fpga = NULL;

        /* Create FPGA file as instructed for multi cart */
	    fpga = fopen("fpga", "wa");
	    if(!fpga)
	    {
		    fprintf(stderr, "Could not create fpga file\n");
            return 0;
	    }
        fprintf(fpga, "10");
        fclose(fpga);
    }
	return 0;
}
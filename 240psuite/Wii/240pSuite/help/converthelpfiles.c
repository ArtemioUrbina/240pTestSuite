#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINESPERPAGE	16

int main(int argc, char **argv)
{
    int     c = 0, d = 0;
    int	    i = 0, lines = 0, linecount = 0, currpage = 0;
	long 	size = 0;
	FILE 	*fp = NULL;
	char 	*buffer = NULL;
    char    ***pages;
    int     *npages;
    char    **realpages = NULL;
    int     realnpages = 0;
    
    FILE    *nfile, *hfile;

    nfile = fopen("helpcode.c", "w");
    if(!nfile)
    {
        fprintf(stderr, "Could not open results  file\n");
        return 0;
    }

    hfile = fopen("helpcode.h", "w");
    if(!hfile)
    {
        fprintf(stderr, "Could not open results  file\n");
        return 0;
    }

    pages = &realpages;
    npages = &realnpages;

    if(argc < 2)
    {
        fprintf(stderr, "Please specify a file for %s\n", argv[0]);
        return 0;
    }

    for(c = 1; c < argc; c++)
    {
        fp = fopen(argv[c], "r");
        if(!fp)
        {
            fprintf(stderr, "Failed to open %s\n", argv[0]);
            return 0;
        }
        fseek(fp, 0L, SEEK_END);
	    size = ftell(fp)+1;
	    buffer = (char*)malloc(sizeof(char)*size);
	    if(!buffer)
	    {
		    fclose(fp);
		    fprintf(stderr, "Could not load %s help file to RAM\n", argv[c]);
		    return 0;
	    }
	    fseek(fp, 0L, SEEK_SET);
	    memset(buffer, 0x0, sizeof(char)*size);
	    fread(buffer, sizeof(char), size-1, fp);
	    fclose(fp);
    
	    for(i = 0; i < size; i++)
		    if(buffer[i] == '\n')
			    lines ++;
    
	    *npages = (lines / LINESPERPAGE);
	    if(lines % LINESPERPAGE && lines > LINESPERPAGE)
		    (*npages) ++;
	    if(*npages < 1)
		    *npages = 1;
    
	    *pages = (char**)malloc(sizeof(char*)*(*npages));
	    if(!*pages)
	    {
		    fprintf(stderr, "Could not malloc array of pages %s\n", argv[c]);
		    free(buffer);
		    buffer = NULL;
		    return 0;
	    }
    
	    *pages[0] = buffer;
	    currpage = 1;
	    if(*npages > 1)
	    {
		    for(i = 0; i < size; i++)
		    {
			    if(buffer[i] == '\n')
			    {
				    linecount ++;
				    if(linecount == LINESPERPAGE)
				    {
					    linecount = 0;
					    buffer[i] = 0x0;
					    if(currpage < *npages)
						    (*pages)[currpage++] = buffer+i+1;
				    }
			    }
		    }
	    }  

        /*    char    **realpages = NULL;
    int     realnpages = 0;
        */
        /* here */
        argv[c][strlen(argv[c]) - 4] = '_';
        
        fprintf(nfile, "char *%s[] = { \n", argv[c]);
        fprintf(hfile, "extern char *%s[];\n", argv[c]);

        for(d = 0; d < realnpages; d ++)
        {
            int pos = 0;
                       
            fprintf(nfile, "\"", argv[c], d + 1);
            while(realpages[d][pos] != '\0')
            {
                if(realpages[d][pos] == '\n')
                    fprintf(nfile, "\\n");
                else {
                    if(realpages[d][pos] == '"')
                        fprintf(nfile, "\\");
                    fprintf(nfile, "%c", realpages[d][pos]);
                }     
                pos ++;           
            }            
            fprintf(nfile, "\"");

            if(d != realnpages - 1)
                fprintf(nfile, ",\n");
            else
                fprintf(nfile, ",\nNULL\n};\n");
        }

        free(buffer);
        buffer = NULL;
        if(realpages)
		    free(realpages);
        realpages = NULL;
        realnpages = 0;	            
        i = lines = linecount = currpage = 0;

    
    }
    fclose(nfile);
    fclose(hfile);
    return 0;
}
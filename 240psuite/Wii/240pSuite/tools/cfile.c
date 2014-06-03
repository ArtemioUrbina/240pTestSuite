/* 
 * 240p Test Suite
 * Copyright (C)2014 Artemio Urbina (Wii GX)
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

/*
Compress the texture.tpl file to improve load times
and reduce executable size. This file must be compiled
and placed in the devkitppc/bin folder.
*/
int main(int argc, char *argv[])
{    
    FILE *fp = NULL, *fout = NULL, *fhead = NULL;
    unsigned long int srcSize, cmpSize;
    char *src = NULL, *comp = NULL;
    
    if(argc != 4)
    {
        printf("Usage %s <source> <target> <header>\n", argv[0]); 
        return -1;
    }
    
    printf("Compressing %s to %s\n", argv[1], argv[2]); 
    fp = fopen(argv[1], "rb");
    if(!fp)
    {
        printf("Could not read from file %s\n", argv[1]);        
        return -1;
    }
    
    fseek(fp, 0L, SEEK_END);
    srcSize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    cmpSize = srcSize * 2;
    src = (char*)malloc (sizeof(char)*srcSize);
    if (!src) 
    {
        fclose(fp);        
        printf("Out of memory\n");
        return -1;
    }
    
    if(fread (src, sizeof(char), srcSize, fp) != srcSize)
    {
        fclose(fp);        
        printf("Error reading file\n");
        return -1;
    }

    fclose(fp);
    fp = NULL;

    comp = (char*)malloc (sizeof(char)*cmpSize);
    if (!comp) 
    {
        free(src);
        printf("Out of memory\n");
        return -1;
    }    

    if(compress(comp, &cmpSize, src, srcSize) != Z_OK)
    {   
        free(src);     
        free(comp);                
        return -1;
    }

    free(src);
    src = NULL;

    fout = fopen(argv[2], "wb");
    if(!fout)
    {    
        free(comp);        
        printf("Could not write to file %s\n", argv[2]);
        return -1;
    }
    
    if(fwrite(comp, sizeof(char), cmpSize, fout) != cmpSize)
        printf("Error writing data to file %s\n", argv[2]);
    else
        printf("Compressed data written to file %s\n", argv[2]);
    fclose(fout);  

    fhead = fopen(argv[3], "w");
    if(!fhead)
    {            
        printf("Could not write to header file %s\n", argv[3]);
        return -1;
    } 
    fprintf(fhead, "#define\tTEXTURE_CSIZE\t\t%ul\n", cmpSize);
    fprintf(fhead, "#define\tTEXTURE_FSIZE\t\t%ul\n", srcSize);
    fclose(fhead);

    printf("Header data written to file %s\n", argv[3]);
    return 0;
}

/* 
 * 240p Test Suite
 * Copyright (C)2014-2022 Artemio Urbina (PC Engine/TurboGrafx-16)
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
#include <ctype.h>

#define LINESPERPAGE	20

void makeUpper(char *str)
{
	while(*str)
	{
		*str = toupper(*str);
		str++;
	}
}

int main(int argc, char **argv)
{
	int 	c = 0, d = 0;
	int		i = 0, lines = 0, linecount = 0, currpage = 0;
	long 	size = 0;
	FILE 	*fp = NULL;
	char 	*buffer = NULL;
	char	***pages;
	int 	*npages;
	char	**realpages = NULL;
	int 	realnpages = 0;
	char	namebuffer[100];
	int 	dscount = 0;
	int 	hasIFDEF = 0;
	char	ifdef[100];
	
	FILE	*nfile, *hfile;

	nfile = fopen("helpdata.c", "w");
	if(!nfile)
	{
		fprintf(stderr, "Could not open results  file\n");
		return 0;
	}

/*
	hfile = fopen("helpdata.h", "w");
	if(!hfile)
	{
		fprintf(stderr, "Could not open results  file\n");
		return 0;
	}
*/

	pages = &realpages;
	npages = &realnpages;

	if(argc < 2)
	{
		fprintf(stderr, "Please specify a file for %s\n", argv[0]);
		return 0;
	}

/*
	for(c = 1; c < argc; c++)
	{
		int pos = strlen(argv[c]);
		
		strncpy(namebuffer, argv[c], pos);
		makeUpper(namebuffer);
		namebuffer[pos - 4] = '\0';

		fprintf(hfile, "#define\t%s_HELP\t%d\n", namebuffer, c);
	}

	fclose(hfile);
*/

	fprintf(nfile, "/*\n"); 
	fprintf(nfile, " * 240p Test Suite\n"); 
	fprintf(nfile, " * Copyright (C)2014-2022 Artemio Urbina (PC Engine/TurboGrafx-16)\n"); 
	fprintf(nfile, " *\n"); 
	fprintf(nfile, " * This file is part of the 240p Test Suite\n"); 
	fprintf(nfile, " *\n"); 
	fprintf(nfile, " * The 240p Test Suite is free software; you can redistribute it and/or modify\n"); 
	fprintf(nfile, " * it under the terms of the GNU General Public License as published by\n"); 
	fprintf(nfile, " * the Free Software Foundation; either version 2 of the License, or\n"); 
	fprintf(nfile, " * (at your option) any later version.\n"); 
	fprintf(nfile, " *\n"); 
	fprintf(nfile, " * The 240p Test Suite is distributed in the hope that it will be useful,\n"); 
	fprintf(nfile, " * but WITHOUT ANY WARRANTY; without even the implied warranty of\n"); 
	fprintf(nfile, " * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"); 
	fprintf(nfile, " * GNU General Public License for more details.\n"); 
	fprintf(nfile, " *\n"); 
	fprintf(nfile, " * You should have received a copy of the GNU General Public License\n"); 
	fprintf(nfile, " * along with 240p Test Suite; if not, write to the Free Software\n"); 
	fprintf(nfile, " * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA\n"); 
	fprintf(nfile, " */\n\n");
	
	for(c = 1; c < argc; c++)
	{
		int removed = 0;
		int pos = strlen(argv[c]);
		
		lines = 0;
		hasIFDEF = 0;
		strncpy(namebuffer, argv[c], pos);
		namebuffer[pos - 4] = '\0';

		fp = fopen(argv[c], "rb");
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

		// destroy the line feeds 0x0D
		for(i = 0; i < size; i++)
		{
			if(buffer[i] == 0x0D)
			{
				long j = 0;
				for(j = i; j < size - 1; j++)
					buffer[j] = buffer [j+1];
				size --;
				removed ++;
				buffer[size] = '\0';
			}
		}

		// Find IfDef
		for(i = 0; i < size; i++)
		{
			if(buffer[i] == '#')
			{
				int k = 0;
				long j = 0;

				hasIFDEF = 1;
				while(buffer[i] < size && k < 100)
				{
					if(buffer[i] != '\n')
					{
						ifdef[k] = buffer[i];
						k++;
						i++;
					}
					else
						break;
				}
				ifdef[k] = '\0';
				printf("File %s  Found ifdef[%d]: \"%s\"\n", argv[c], k, ifdef);

				for(j = i-k; j < size - k; j++)
					buffer[j] = buffer[j+k+1];
				size = size - k - 1;
				removed += k;
				buffer[size] = '\0';
			}
		}

		// Count lines
		for(i = 0; i < size; i++)
		{
			if(buffer[i] == '\n')
				lines ++;
		}

		if(buffer[size - 2] != '\n')
		{
			int m;
			
			for(m = 0; m < 10; m++)
				printf("%X ", buffer[size - 10 + m]);
			printf("\n");
			printf("File %s doesn't end with \\n, might miss a line\n", argv[c]);
		}

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

		/*	  char	  **realpages = NULL;
			  int	  realnpages = 0;
		*/

		/* here */
	
		for(d = 0; d < realnpages; d++)
		{
			int pos = 0;
			int row = 0;
			int lastpos = 0;
			int hpos = 6;
			int linestart = 1;
			int open = 0;
			int first = 1;
					   
			if(hasIFDEF)
				fprintf(nfile, "%s\n", ifdef);

			fprintf(nfile, "void display_%s_%0.2d()\n{\n", namebuffer, d);

			fprintf(nfile, "#asm\n");
			while(realpages[d][pos] != '\0')
			{
				if(linestart && realpages[d][pos] == ' ')
					hpos++;
				else
					linestart = 0;
				
				if(realpages[d][pos] == '\n')
				{	
					if(pos - lastpos > 1)
						fprintf(nfile, "\tp_string\t%s_%0.2d_%0.2d,%d,%d\n", namebuffer, d, dscount++, hpos, row+4);
					if(first)
					{
						fprintf(nfile, "\t__ldwi\t\t14\n\tcall\t\t_set_font_pal\n");
						first = 0;
					}
					lastpos = pos;
					row++;
					hpos = 6;
					linestart = 1;
				}			
				pos++;
			}
			
			pos = 0;
			lastpos = -1;
			dscount = 0;
			linestart = 1;
			open = 0;

			fprintf(nfile, "\trts\n\n");
			while(realpages[d][pos] != '\0')
			{
				if(linestart && realpages[d][pos] == ' ')
					hpos++;
				else
					linestart = 0;

				if(realpages[d][pos] == '\n')
				{
					if(pos - lastpos > 1)
					{
						fprintf(nfile, "\"\n\t\t\t.db\t0\n");
						open = 0;
					}
					lastpos = pos;
					linestart = 1;
				}
				else
				{
					if(lastpos == pos - 1)
					{
						fprintf(nfile, "%s_%0.2d_%0.2d:\t.db\t\"", namebuffer, d, dscount++);
						open = 1;
					}
					if(realpages[d][pos] == '"')
						fprintf(nfile, "\\");
					if(!(linestart && realpages[d][pos] == ' '))
						fprintf(nfile, "%c", realpages[d][pos]);
				}	  
				pos ++; 		  
			}			 
			if(open)
			{
				fprintf(nfile, "\"\n\t\t\t.db\t0\n");
				open = 0;
			}
			fprintf(nfile, "#endasm\n}\n");	
			if(hasIFDEF)
				fprintf(nfile, "#endif\n");	
			fprintf(nfile, "\n\n");	
			dscount = 0;
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

	return 0;
}
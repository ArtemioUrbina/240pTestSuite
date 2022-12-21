/* 
 * 240p Test Suite
 * Copyright (C)2014-2022 Artemio Urbina (Wii GX)
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
#include <dirent.h>
#include <stdint.h>
#include <math.h>

typedef unsigned int uint;
typedef uint16_t WORD;
typedef uint8_t BYTE;

#define HCFR_COLOR_NAME_MAX		64

typedef struct hcfr_color_st {
	char	name[HCFR_COLOR_NAME_MAX];
	int		r;
	int		g;
	int		b;
} hcfr_color;

typedef struct hcfr_files_st {
	char	*dispname;
	char	*filename;
} hcfr_file;

hcfr_file *listCSVFiles(char *path, int *num_files)
{
	int				filecount = 0, maxlen = 0, i = 0;
	DIR				*d;
	struct dirent	*dir;
	hcfr_file		*filenames = NULL;
	
	if(!num_files)
		return NULL;

	*num_files = 0;
	d = opendir(path);
	if(d)
	{
		dir = readdir(d);
		while(dir)
		{
			int len = 0;
			
			len = strlen(dir->d_name);
			if(len > 4)
			{
				if( dir->d_name[len - 3] == 'c' && 
					dir->d_name[len - 2] == 's' &&
					dir->d_name[len - 1] == 'v')
				{
					filecount++;
					if(len > maxlen)
						maxlen = len;
				}
			}
			dir = readdir(d);
		}
	}
	closedir(d);
	
	if(!filecount)
		return NULL;

	filenames = (hcfr_file*)malloc(sizeof(hcfr_file)*filecount);
	if(!filenames)
		return NULL;

	memset(filenames, 0, sizeof(hcfr_file)*filecount);
	maxlen += 1;	// add NULL
	for(i = 0; i < filecount; i++)
	{
		filenames[i].dispname = (char*)malloc(sizeof(char)*maxlen);
		if(!filenames[i].dispname)
		{
			int pos = 0;
			
			for(pos = 0; pos < i; pos ++)
			{
				free(filenames[pos].dispname);
				free(filenames[pos].filename);
			}
			free(filenames);
			return NULL;
		}
		memset(filenames[i].dispname, 0, sizeof(char)*maxlen);
		
		filenames[i].filename = (char*)malloc(sizeof(char)*maxlen);
		if(!filenames[i].filename)
		{
			int pos = 0;
			
			free(filenames[i].dispname);
			for(pos = 0; pos < i; pos ++)
			{
				free(filenames[pos].dispname);
				free(filenames[pos].filename);
			}
			free(filenames);
			return NULL;
		}
		memset(filenames[i].filename, 0, sizeof(char)*maxlen);
	}
	
	i = 0;
	// store the values
	d = opendir(".");
	if(d)
	{
		dir = readdir(d);
		while(dir)
		{
			int len = 0;
			
			len = strlen(dir->d_name);
			if(len > 7)
			{
				if( dir->d_name[len - 3] == 'c' && 
					dir->d_name[len - 2] == 's' &&
					dir->d_name[len - 1] == 'v')
				{
					int l_len = 0, pos = 0;
					
					// remove leading sort number and extension
					memcpy(filenames[i].dispname, dir->d_name+3, sizeof(char)*(len - 7)); 
					l_len = strlen(filenames[i].dispname);
					for(pos = 0; pos < l_len; pos++)
					{
						if(filenames[i].dispname[pos] == '_')
							filenames[i].dispname[pos] = ' ';
					}
					sprintf(filenames[i].filename, "%s", dir->d_name);
					i++;
				}
			}
			dir = readdir(d);
		}
	}
	closedir(d);
	
	*num_files = filecount;
	return(filenames);
}

void releaseCSVList(hcfr_file **listCSVFiles, int num_files)
{
	int pos = 0;
			
	for(pos = 0; pos < num_files; pos ++)
	{
		free((*listCSVFiles)[pos].dispname);
		free((*listCSVFiles)[pos].filename);
		(*listCSVFiles)[pos].dispname = NULL;
		(*listCSVFiles)[pos].filename = NULL;
	}
	free(*listCSVFiles);
	*listCSVFiles = NULL;
}

hcfr_color *LoadHCFR(char *filename, int *hcfr_num)
{
	uint 		size = 0;
	FILE 		*fp = NULL;
	int			hcfr_count = 0;
	uint		i = 0, lines = 0, current_line = 0;
	uint		max_line_len = 0,line_len = 0, read = 0;
	char		*buffer = NULL, *lineBuffer = NULL, *last_csv = NULL;
	hcfr_color 	*hcfr_colors = NULL;
	
	if(!hcfr_num)
		return NULL;

	*hcfr_num = 0;
	fp = fopen(filename, "rb");
	if(!fp)
	{
		printf("Could not load %s HCFR file\n", filename);
		return NULL;
	}
	fseek(fp, 0L, SEEK_END);
	size = ftell(fp)+1;
	buffer = (char*)malloc(sizeof(char)*size);
	if(!buffer)
	{
		fclose(fp);
		printf("Could not reserve space for %s CSV HCFR in RAM\n", filename);
		return NULL;
	}
	fseek(fp, 0L, SEEK_SET);
	memset(buffer, 0x0, sizeof(char)*size);
	read = fread(buffer, sizeof(char), size-1, fp);
	if(read != size-1)
	{
		free(buffer);
		fclose(fp);
		printf("Could not load %s CSV HCFR file to RAM (size %u/%u)\n", filename, read, size-1);
		return NULL;
	}
	fclose(fp);
	
	for(i = 0; i < size; i++)
	{
		line_len++;
		
		if(buffer[i] == '\n')
		{
			lines ++;
			if(line_len > max_line_len)
				max_line_len = line_len;
			line_len = 0;
		}
	}
	
	hcfr_count = lines - 1; // we remove the header
	if(hcfr_count <= 0)
	{
		printf("File %s has no HCFR color data\n", filename);
		free(buffer);
		buffer = NULL;
		return NULL;
	}
	hcfr_colors = (hcfr_color*)malloc(sizeof(hcfr_color)*hcfr_count);
	if(!hcfr_colors)
	{
		printf("Could not malloc array of hcfr_colors %s\n", filename);
		free(buffer);
		buffer = NULL;
		return NULL;
	}
	memset(hcfr_colors, 0, sizeof(hcfr_color)*hcfr_count);
	
	lineBuffer = (char*)malloc(sizeof(char)*max_line_len);
	if(!lineBuffer)
	{
		free(buffer);
		free(hcfr_colors);
		printf("Could not load %s CSV HCFR file to RAM (%u size)\n", filename, max_line_len);
		return NULL;
	}
	
	// parse the CSV from RAM
	last_csv = buffer;
	for(i = 0; i < size; i++)
	{
		line_len++;
		if(buffer[i] == '\n')
		{
			if(current_line)	// we start at 1, due to header
			{
				uint	last_comma = 0, pos = 0,
						comma_count = 0, str_len = 0;
				
				for(pos = 0; pos < line_len; pos++)
				{
					if(comma_count == 0)
						str_len ++;
					if(last_csv[pos] == ',')
						comma_count ++;
					if(comma_count == 4)
					{
						last_comma = pos;
						break;
					}
				}
				memcpy(lineBuffer, last_csv, sizeof(char)*last_comma);
				lineBuffer[last_comma] = '\0';
				
				if(str_len > HCFR_COLOR_NAME_MAX)
					str_len = HCFR_COLOR_NAME_MAX;
				memcpy(hcfr_colors[current_line-1].name, last_csv, sizeof(char)*(str_len-1));
				if(sscanf(lineBuffer+str_len, "%d,%d,%d",
								&hcfr_colors[current_line-1].r,
								&hcfr_colors[current_line-1].g,
								&hcfr_colors[current_line-1].b) != 3)
					{
						printf("File: %s Invalid structure in line %d: \"%s\"\n", 
								filename, current_line, lineBuffer);
						free(buffer);
						free(hcfr_colors);
						free(lineBuffer);
						return NULL;
					}
			}
			
			current_line ++;
			line_len = 0;
			last_csv = buffer + i + 1;
		}
	}
	
	free(buffer);
	free(lineBuffer);
	*hcfr_num = hcfr_count;
	return hcfr_colors;
}

int main()
{
	int			num_files = 0, i = 0;
	hcfr_color 	*hcfr_colors = NULL;
	hcfr_file	*filenames = NULL;

	filenames = listCSVFiles(".", &num_files);
	if (!filenames)
	{
		printf("Could not find any HCFR CSV files in the current folder\n");
		return 1;	
	}

	for (i = 0; i < num_files; i++)
	{
		int clr = 0, hcfr_num = 0, current_hcfr = 0, count = 0, len = 0;

		hcfr_colors = LoadHCFR(filenames[i].filename, &hcfr_num);
		if (!hcfr_colors)
		{
			printf("Could not load HCFR CSV file");
			releaseCSVList(&filenames, num_files);
			return 1;
		}

		len = strlen(filenames[i].filename);
		if(len > 5)
			filenames[i].filename[len - 4] = '\0';
		printf("const HCFR_DATA %s[] = {\n\t", filenames[i].filename);
		for (current_hcfr = 0; current_hcfr < hcfr_num; current_hcfr++)
		{
			int r, g, b;

			r = hcfr_colors[current_hcfr].r;
			g = hcfr_colors[current_hcfr].g;
			b = hcfr_colors[current_hcfr].b;
			
			printf("{ \"%s\", %d, %d, %d }", 
				hcfr_colors[current_hcfr].name, r, g, b);

			count ++;
			if(current_hcfr != hcfr_num - 1)
			{
				if(count == 1)
				{
					printf(",\n\t");
					count = 0;
				}
				else
					printf(", ");
			}
		}
		printf("\n};\n");

		free(hcfr_colors);
		hcfr_colors = NULL;
	}

	//for(i = 0; i <= 255; i++)
		//printf("%d %g %d 0x%04X\n", i, round(rgb2y(i, i, i)), getDarkBit(i, i, i), PackColor(i, i, i, getDarkBit(i, i, i)));

	releaseCSVList(&filenames, num_files);

	return 0;
}
#include <yaul.h>
#include <svin.h>
#include <stdlib.h>
#include "svin_filelist.h"

int _svin_filelist_size;
char *_svin_filelist_long_filename;
char *_svin_filelist;

bool
_svin_filelist_search_lfn(cdfs_filelist_entry_t entry, char * raw_buffer, int parent_raw_buffer_len, bool bFolder)
{
    char search_buf[16];
    strcpy(search_buf,entry.name);
    assert(strlen(search_buf)<=12);
    if (bFolder)
        strcat(search_buf,";1");
    int len = strlen(search_buf);

    for (int i=0;i<parent_raw_buffer_len-len-1;i++)
    {
        if (0==memcmp(search_buf,raw_buffer+i,len))
        {
            //scanning forward until NM signature
            while ((i<parent_raw_buffer_len)&&(0!=memcmp("NM",raw_buffer+i,2)))
                i++;
            len = raw_buffer[i+2]-5;
            memcpy(_svin_filelist_long_filename,(uint8_t*)&(raw_buffer[i+5]),len);
            _svin_filelist_long_filename[len] = 0;
            return true;
        }
    }

    return false;
}

void 
_svin_filelist_fill_recursive(cdfs_filelist_entry_t entry, char * current_dir, char * parent_raw_buffer, int parent_raw_buffer_len)
{
    cdfs_filelist_t _rec_filelist;
    cdfs_filelist_entry_t _rec_filelist_entries[_SVIN_FILELIST_ENTRIES_PER_DIR_LIMIT];
    _rec_filelist.entries = _rec_filelist_entries;
    _rec_filelist.entries_count = 0;
    _rec_filelist.entries_pooled_count = 0;
    cdfs_filelist_default_init(&_rec_filelist, _rec_filelist_entries, _SVIN_FILELIST_ENTRIES_PER_DIR_LIMIT);
    uint32_t _rec_i;
    char dir[260]; //remember that MAX_PATH horror?
    char * raw_buffer;
    int raw_size;

    fad_t * _rec_pfad = (fad_t*)(_svin_filelist+256*_svin_filelist_size);
    char * _rec_p8 = (char*)(_svin_filelist+256*_svin_filelist_size+8);
    
    if (CDFS_ENTRY_TYPE_FILE == entry.type) {
        _rec_pfad[0] = entry.starting_fad;
        _rec_pfad[1] = entry.size;
        if (strlen(current_dir)>0)
        {
            strcpy(_rec_p8,current_dir);
            strcat(_rec_p8,"/");
        }
        else
            strcpy(_rec_p8,"");
        //we need to figure out long filename
        //using dir buffer for thar purpose
        if (_svin_filelist_search_lfn(entry,parent_raw_buffer,parent_raw_buffer_len,true))
        {
            strcat(_rec_p8,_svin_filelist_long_filename);
        }
        else
        {
            strcat(_rec_p8,entry.name); //using short filename if no LFN is available
        }    
        _svin_filelist_size++;
    }
    else if (CDFS_ENTRY_TYPE_DIRECTORY == entry.type){
        //recursively allocating another buffer
        raw_size = entry.size;//8192;//TODO: detect this automatically
        raw_size = (((raw_size-1)/2048)+1)*2048;
        raw_buffer = malloc(raw_size);
        //now read raw folder content for LFN search
        _svin_cd_block_sectors_read(entry.starting_fad,  (uint8_t*)raw_buffer, raw_size);

        if (strlen(current_dir)>0)
        {
            strcpy(dir,current_dir);
            strcat(dir,"/");
        }
        else
            strcpy(dir,"");

        
        if (_svin_filelist_search_lfn(entry,parent_raw_buffer,parent_raw_buffer_len,false))
        {
            strcat(dir,_svin_filelist_long_filename);
        }
        else
        {
            strcat(dir,entry.name); //using short filename if no LFN is available
        }    
        #ifdef ROM_MODE
            cdfs_rom_filelist_read(entry,&_rec_filelist,_SVIN_FILELIST_ENTRIES_PER_DIR_LIMIT); 
        #else
            cdfs_filelist_read(&_rec_filelist,entry); 
        #endif

        for (_rec_i=0;_rec_i<_rec_filelist.entries_count;_rec_i++)
        {
            _svin_filelist_fill_recursive(_rec_filelist.entries[_rec_i],dir,raw_buffer,raw_size);
        }
        free(raw_buffer);
    }
}

//load entire files list into LWRAM
bool 
_svin_filelist_fill()
{
    cdfs_filelist_t _filelist;
    cdfs_filelist_entry_t _filelist_entries[_SVIN_FILELIST_ENTRIES_PER_DIR_LIMIT];
    _filelist.entries = _filelist_entries;
    _filelist.entries_count = 0;
    _filelist.entries_pooled_count = 0;
    cdfs_filelist_default_init(&_filelist, _filelist_entries, _SVIN_FILELIST_ENTRIES_PER_DIR_LIMIT);

    uint32_t i;
    cdfs_pvd_t * pvd = malloc(sizeof(cdfs_pvd_t));
    cdfs_dirent_t *dirent_root;

	_svin_filelist = (char *)_svin_alloc_lwram(0x40000,0x20200000);

    _svin_filelist_size = 0;

    //reading pvd
    _svin_cd_block_sector_read(LBA2FAD(16), (uint8_t*)pvd);
    dirent_root = (cdfs_dirent_t *)((pvd->root_directory_record)); 
    //getting root size
    int root_length = isonum_733(dirent_root->data_length);
    root_length = (((root_length-1)/2048)+1)*2048;
    int root_start = isonum_733(dirent_root->extent);
    char * raw_buffer = malloc(root_length);
    if (root_start <= 0)
		return false;
    _svin_cd_block_sectors_read(LBA2FAD(root_start), (uint8_t*)raw_buffer,root_length);

    //allocating global  buffer for filename
    _svin_filelist_long_filename = malloc(256);//ought to be enough for everyone

    ///cook up a "root entry"
    cdfs_filelist_entry_t root_dummy_entry;
    root_dummy_entry.type = CDFS_ENTRY_TYPE_DIRECTORY;
    for (int i=0;i<12;i++)
        root_dummy_entry.name[i] = '\0';
    root_dummy_entry.starting_fad = LBA2FAD(root_start);
    root_dummy_entry.size = root_length;
    root_dummy_entry.sector_count = ((root_length-1)/2048 + 1);
    if (root_dummy_entry.starting_fad <= 150)
		return false;

#ifdef ROM_MODE
    cdfs_rom_filelist_read(root_dummy_entry,&_filelist,_SVIN_FILELIST_ENTRIES_PER_DIR_LIMIT);
#else
    cdfs_filelist_read(&_filelist,root_dummy_entry);
#endif

    for (i=0;i<_filelist.entries_count;i++)
        _svin_filelist_fill_recursive(_filelist.entries[i],"",raw_buffer,root_length);
    
    free (raw_buffer);
    free (_svin_filelist_long_filename);
    free (pvd);
	return true;
}

//searches the filelist and returns fad (-1 if not found)
bool 
_svin_filelist_search(char * filename, fad_t * fad, int * size)
{
    fad_t * pfad = (fad_t *)(_svin_filelist);
    //doing a simple linear search for now
    for (int i=0;i<_svin_filelist_size;i++)
    {
        if (strcmp(filename,(char*)(_svin_filelist+i*256+8)) == 0)
        {
            fad[0] = pfad[i*64];
            size[0] = pfad[i*64+1];
            return true;
        }
    }
    return false;
}
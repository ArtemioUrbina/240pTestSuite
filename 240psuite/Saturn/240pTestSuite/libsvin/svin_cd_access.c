#include <cd-block.h>
#include <cpu/instructions.h>
#include <smpc/smc.h>
//#include <assert.h>
#include "svin.h"

#ifdef ROM_MODE
#include <string.h>

static uint8_t _sector_buffer[SECTOR_LENGTH_2048] __aligned(SECTOR_LENGTH_2048); //UGLY!

int
_svin_cd_block_sectors_read(uint32_t fad, uint8_t *output_buffer, uint32_t length)
{
        memcpy(output_buffer,(uint8_t *)CS0((FAD2LBA(fad)+512)*2048),length);
        return 0;
}

int
_svin_cd_block_sector_read(uint32_t fad, uint8_t *output_buffer)
{
        memcpy(output_buffer,(uint8_t *)CS0((FAD2LBA(fad)+512)*2048),2048);
        return 0;
}
#else
int
_svin_cd_block_sectors_read(uint32_t fad, uint8_t *output_buffer, uint32_t length)
{
        return cd_block_sectors_read(fad,output_buffer,length);
}

int
_svin_cd_block_sector_read(uint32_t fad, uint8_t *output_buffer)
{
        return cd_block_sector_read(fad,output_buffer);
}
#endif

int
_svin_cd_block_async_read_request_sectors(uint32_t fad,  uint32_t sector_count)
{
        assert(fad >= 150);
        assert(sector_count > 0);

        int ret;

        if ((ret = cd_block_cmd_selector_reset(0, 0)) != 0) {
                return ret;
        }

        if ((ret = cd_block_cmd_cd_dev_connection_set(0)) != 0) {
                return ret;
        }

        if ((ret = cd_block_cmd_disk_play(0, fad, sector_count)) != 0) {
                return ret;
        }

}

int
_svin_cd_block_async_read_fetch_sectors(uint8_t *output_buffer, uint32_t buff_size_in_sectors)
{
        assert(output_buffer != NULL);
        assert(buff_size_in_sectors > 0);

        uint32_t sectors_to_read = cd_block_cmd_sector_number_get(0);
        
        if (sectors_to_read == 0)
        {
                //no data available, return 0
                return 0;
        }

        if (sectors_to_read > buff_size_in_sectors) 
                sectors_to_read = buff_size_in_sectors;
        
        int ret = cd_block_transfer_data(0, 0, output_buffer, sectors_to_read * CDFS_SECTOR_SIZE);

        if (ret == 0)
                return sectors_to_read;
        else
                return -1; //some error

}

/*----------------------- ROM cd emulation section -----------------------*/
#ifdef ROM_MODE

static struct {
        cdfs_pvd_t pvd;
} _state;

static inline uint32_t __always_inline
_length_sector_round(uint32_t length)
{
        return ((length + 0x07FF) >> 11);
}

static bool __unused
_dirent_interleave(const cdfs_dirent_t *dirent)
{
        return ((isonum_711(dirent->interleave)) != 0x00);
}

static void
_filelist_entry_populate(const cdfs_dirent_t *dirent, cdfs_entry_type_t type,
    cdfs_filelist_entry_t *filelist_entry)
{
        filelist_entry->type = type;
        filelist_entry->size = isonum_733(dirent->data_length);
        filelist_entry->starting_fad = LBA2FAD(isonum_733(dirent->extent));
        filelist_entry->sector_count = _length_sector_round(filelist_entry->size);

        uint8_t name_len;
        name_len = isonum_711(dirent->file_id_len);

        if (type == CDFS_ENTRY_TYPE_FILE) {
                /* Minus the ';1' */
                name_len -= 2;

                /* Empty extension */
                if ((name_len > 0) && (dirent->name[name_len - 1] == '.')) {
                        name_len--;
                }
        }

        (void)memcpy(filelist_entry->name, dirent->name, name_len);
        filelist_entry->name[name_len] = '\0';
}

static void
_filelist_read_walker(const cdfs_filelist_entry_t *entry, void *args)
{
        cdfs_filelist_t *filelist;
        filelist = args;

        if (filelist->entries_count >= filelist->entries_pooled_count) {
                return;
        }

        cdfs_filelist_entry_t *this_entry;
        this_entry = &filelist->entries[filelist->entries_count];

        (void)memcpy(this_entry, entry, sizeof(cdfs_filelist_entry_t));

        filelist->entries_count++;
}

static void
_filelist_initialize(cdfs_filelist_t *filelist, int32_t count)
{
        int32_t clamped_count;
        clamped_count = count;

        if (clamped_count <= 0) {
                clamped_count = cdfs_FILELIST_ENTRIES_COUNT;
        } else if (clamped_count > cdfs_FILELIST_ENTRIES_COUNT) {
                clamped_count = cdfs_FILELIST_ENTRIES_COUNT;
        }

        if (filelist->entries == NULL) {
                filelist->entries = malloc(sizeof(cdfs_filelist_entry_t) * clamped_count);
        }

        assert(filelist->entries != NULL);

        filelist->entries_pooled_count = (uint32_t)clamped_count;
        filelist->entries_count = 0;
}

static void
_bread_rom(uint32_t sector, void *ptr)
{
        int ret __unused;
        ret = _svin_cd_block_sector_read(LBA2FAD(sector), ptr);
        assert(ret == 0);
}

static void
_dirent_rom_walk(cdfs_filelist_walk_t walker, uint32_t sector, void *args)
{
        const cdfs_dirent_t *dirent;
        dirent = NULL;

        int32_t dirent_sectors;
        dirent_sectors = INT32_MAX;

        /* Keep track of where we are within the sector */
        uint32_t dirent_offset;
        dirent_offset = 0;

        while (true) {
                uint8_t dirent_length =
                    (dirent != NULL) ? isonum_711(dirent->length) : 0;

                if ((dirent == NULL) ||
                    (dirent_length == 0) ||
                    ((dirent_offset + dirent_length) >= SECTOR_LENGTH_2048)) {
                        dirent_sectors--;

                        if (dirent_sectors == 0) {
                                break;
                        }

                        dirent_offset = 0;

                        _bread_rom(sector, _sector_buffer);

                        dirent = (const cdfs_dirent_t *)&_sector_buffer[0];
                        dirent_length = isonum_711(dirent->length);

                        if (dirent->name[0] == '\0') {
                                uint32_t data_length;
                                data_length = isonum_733(dirent->data_length);

                                dirent_sectors = _length_sector_round(data_length);
                        }

                        /* Interleave mode must be disabled */
                        assert(!(_dirent_interleave(dirent)));

                        /* Next sector */
                        sector++;
                }

                /* Check for Current directory ('\0') or parent directory ('\1') */
                if ((dirent->name[0] != '\0') && (dirent->name[0] != '\1')) {
                        /* Interleave mode must be disabled */
                        assert(!(_dirent_interleave(dirent)));

                        if (walker != NULL) {
                                const uint8_t file_flags = isonum_711(dirent->file_flags);

                                cdfs_entry_type_t type;
                                type = CDFS_ENTRY_TYPE_FILE;

                                if ((file_flags & DIRENT_FILE_FLAGS_DIRECTORY) == DIRENT_FILE_FLAGS_DIRECTORY) {
                                        type = CDFS_ENTRY_TYPE_DIRECTORY;
                                }

                                cdfs_filelist_entry_t filelist_entry;

                                _filelist_entry_populate(dirent, type, &filelist_entry);

                                walker(&filelist_entry, args);
                        }
                }

                if (dirent != NULL) {
                        const uintptr_t p = (uintptr_t)dirent + dirent_length;

                        dirent = (const cdfs_dirent_t *)p;

                        dirent_offset += dirent_length;
                }
        }
}

static void
_dirent_rom_root_walk(cdfs_filelist_walk_t walker, void *args)
{
        cdfs_dirent_t dirent_root;

        /* Populate filesystem mount structure */
        /* Copy of directory record */
        (void)memcpy(&dirent_root, _state.pvd.root_directory_record, sizeof(dirent_root));

        /* Start walking the root directory */
        const uint32_t sector = isonum_733(dirent_root.extent);

        _dirent_rom_walk(walker, sector, args);
}

void
cdfs_rom_filelist_walk(const cdfs_filelist_entry_t *root_entry,
    cdfs_filelist_walk_t walker, void *args)
{
        if (root_entry == NULL) {
                /* Skip IP.BIN (16 sectors) */
                _bread_rom(16, &_state.pvd);

                /* We are interested in the Primary Volume Descriptor, which
                 * points us to the root directory and path tables, which both
                 * allow us to find any file on the CD */

                /* Must be a "Primary Volume Descriptor" */
                assert(isonum_711(_state.pvd.type) == ISO_VD_PRIMARY);

                /* Must match 'CD001' */
#ifdef DEBUG
                const char *cd001_str;
                cd001_str = (const char *)_state.pvd.id;
                size_t cd001_len;
                cd001_len = sizeof(_state.pvd.id) + 1;

                assert((strncmp(cd001_str, ISO_STANDARD_ID, cd001_len)) != 0);
#endif /* DEBUG */

                /* Logical block size must be SECTOR_LENGTH_2048 bytes */
                assert(isonum_723(_state.pvd.logical_block_size) == SECTOR_LENGTH_2048);

                _dirent_rom_root_walk(walker, args);
        } else {
                const uint32_t sector = FAD2LBA(root_entry->starting_fad);

                _dirent_rom_walk(walker, sector, args);
        }
}

void
cdfs_rom_filelist_read(const cdfs_filelist_entry_t root_entry,
    cdfs_filelist_t *filelist, int32_t count)
{
        _filelist_initialize(filelist, count);

        cdfs_rom_filelist_walk(&root_entry, _filelist_read_walker, filelist);
}

#endif
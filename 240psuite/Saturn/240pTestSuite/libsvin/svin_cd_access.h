#ifndef _SVIN_CD_ACCESS_H_
#define _SVIN_CD_ACCESS_H_

#include <stdint.h>
#include <cd-block/cmd.h>

extern int _svin_cd_block_sectors_read(uint32_t fad, uint8_t *output_buffer, uint32_t length);
extern int _svin_cd_block_sector_read(uint32_t fad, uint8_t *output_buffer);

extern int _svin_cd_block_async_read_request_sectors(uint32_t fad,  uint32_t sector_count);
extern int _svin_cd_block_async_read_fetch_sectors(uint8_t *output_buffer, uint32_t buff_size_in_sectors);

extern void cdfs_rom_filelist_read(const cdfs_filelist_entry_t root_entry,
    cdfs_filelist_t *filelist, int32_t count);

#endif /* !_SVIN_CD_ACCESS_H_ */

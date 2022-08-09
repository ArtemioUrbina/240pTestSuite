#ifndef _SVIN_DEBUG_H_
#define _SVIN_DEBUG_H_

void _svin_debug_init(void);
void _svin_debug_send_buf(uint8_t * buf);

void _svin_debug_dump_to_lwram(uint8_t * buf, int offset, int size);

#endif

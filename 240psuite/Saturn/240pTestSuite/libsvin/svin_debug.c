#include <yaul.h>

#include <stdio.h>
#include <stdlib.h>
#include <cpu/dmac.h>

uint8_t * SCI_BUFFER_SEND;
uint8_t * SCI_BUFFER_RECV;

#define TEST_PATTERN_LENGTH 2048
#define TEST_PATTERN_START 0xA1 

static void _dmac_handler(void *);
static void __unused _sci_handler(void *);

static volatile bool _dma_done = false;
static volatile bool _sci_done = false;

void _svin_debug_init(void)
{
/*
        SCI_BUFFER_SEND = malloc(TEST_PATTERN_LENGTH);
        SCI_BUFFER_RECV = malloc(TEST_PATTERN_LENGTH);
        
        //For a first part, let's setup SCI in a "tandem" mode, where it works together with DMAC

        cpu_dmac_interrupt_priority_set(8);
        cpu_sci_interrupt_priority_set(0); //disable interrupts from SCI, because they will be served by DMAC, not CPU
 
        cpu_dmac_cfg_t cfg0 __unused = {
                .channel= 0,
                .src = (uint32_t)SCI_BUFFER_SEND,
                .src_mode = CPU_DMAC_SOURCE_INCREMENT,
                .dst = CPU(TDR),
                .dst_mode = CPU_DMAC_DESTINATION_FIXED,
                .len = TEST_PATTERN_LENGTH, 
                .stride = CPU_DMAC_STRIDE_1_BYTE,
                .request_mode = CPU_DMAC_REQUEST_MODE_MODULE,
                .detect_mode = CPU_DMAC_DETECT_MODE_EDGE,
                .bus_mode = CPU_DMAC_BUS_MODE_CYCLE_STEAL,
                .resource_select = CPU_DMAC_RESOURCE_SELECT_TXI,
                .nondefault = true,
                .ihr = _dmac_handler,
                .ihr_work = NULL
        };

        cpu_dmac_cfg_t cfg1 __unused = {
                .channel= 1,
                .src = CPU(RDR),
                .src_mode = CPU_DMAC_SOURCE_FIXED,
                .dst = (uint32_t)SCI_BUFFER_RECV,
                .dst_mode = CPU_DMAC_DESTINATION_INCREMENT,
                .len = TEST_PATTERN_LENGTH,
                .stride = CPU_DMAC_STRIDE_1_BYTE,
                .request_mode = CPU_DMAC_REQUEST_MODE_MODULE,
                .detect_mode = CPU_DMAC_DETECT_MODE_EDGE,
                .bus_mode = CPU_DMAC_BUS_MODE_CYCLE_STEAL,
                .resource_select = CPU_DMAC_RESOURCE_SELECT_RXI,
                .nondefault = true,
                .ihr = NULL,
                .ihr_work = NULL
        };

        //interrupt handlers are never called, because SCI interrupts are masked for CPU
        //but we need to pass some non-null handlers to enable interrupts because DMAC needs them
        cpu_sci_cfg_t cfg_sci __unused = {
                .mode = CPU_SCI_MODE_SYNC,
                .ihr_rxi = _sci_handler,
                .ihr_txi = _sci_handler,
                .sck_config = CPU_SCI_SCK_OUTPUT
        };

      
        //sci_setup();
        cpu_sci_config_set(&cfg_sci);

        //apply config to DMA channels
        cpu_dmac_channel_config_set(&cfg0);
        cpu_dmac_channel_config_set(&cfg1);
        */
}

void _svin_debug_send_buf(uint8_t * buf __unused)
{
/*
        cpu_sci_cfg_t cfg_sci __unused = {
                .mode = CPU_SCI_MODE_SYNC,
                .ihr_rxi = _sci_handler,
                .ihr_txi = _sci_handler,
                .sck_config = CPU_SCI_SCK_OUTPUT
        };
        
        //prepare write buffer
        memcpy(SCI_BUFFER_SEND,buf,TEST_PATTERN_LENGTH);

        //clear read buffer
        memset((uint8_t*)SCI_BUFFER_RECV,0x00,TEST_PATTERN_LENGTH);

        //set dmac priority to round-robin
        cpu_dmac_priority_mode_set(CPU_DMAC_PRIORITY_MODE_ROUND_ROBIN);
        //enable dmac, because priority mode set disables it
        cpu_dmac_enable();
        
        //start SCI with DMAC, it requires disabling and enabling it in a dmac mode
        _dma_done = false;
        cpu_sci_reset_status();
        cpu_sci_disable();
        cpu_dmac_channel_start(1); //start the read channel first, so it's with sync with write channel
        cpu_dmac_channel_start(0); //start the write channel
        cpu_sci_enable_with_dmac(&cfg_sci); //enable SCI back, the requests to DMAC should start automatically
        
        //wait for DMA
        while (!_dma_done) ;
 */
}

void 
_svin_debug_dump_to_lwram(uint8_t * buf, int offset, int size)
{
	uint8_t * p = (uint8_t *)0x20200000;
	for (int i =0; i<size; i++)
		p[i+offset] = buf[i];
}

static void __unused
_dmac_handler(void *work __unused)
{
        _dma_done = true;
}

static void __unused
_sci_handler(void *work __unused)
{
        _sci_done = true;
}

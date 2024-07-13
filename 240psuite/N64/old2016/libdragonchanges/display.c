/**
 * @file display.c
 * @brief Display Subsystem
 * @ingroup display
 */
#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include "libdragon.h"

/**
 * @defgroup display Display Subsystem
 * @ingroup libdragon
 * @brief Video interface system for configuring video output modes and displaying rendered
 *        graphics.
 *
 * The display subsystem handles interfacing with the video interface (VI)
 * and the hardware rasterizer (RDP) to allow software and hardware graphics
 * operations.  It consists of the @ref display, the @ref graphics and the
 * @ref rdp modules.  A separate module, the @ref console, provides a rudimentary
 * console for developers.  Only the display subsystem or the console can be
 * used at the same time.  However, commands to draw console text to the display
 * subsystem are available.
 *
 * The display subsystem module is responsible for initializing the proper video
 * mode for displaying 2D, 3D and softward graphics.  To set up video on the N64,
 * code should call #display_init with the appropriate options.  Once the display
 * has been set, a display context can be requested from the display subsystem using
 * #display_lock.  To draw to the acquired display context, code should use functions
 * present in the @ref graphics and the @ref rdp modules.  Once drawing to a display
 * context is complete, the rendered graphic can be displayed to the screen using
 * #display_show.  Once code has finished rendering all graphics, #display_close can
 * be used to shut down the display subsystem.
 *
 * @{
 */

/** @brief Maximum number of video backbuffers */
#define NUM_BUFFERS         3

/** @brief Register location in memory of VI */
#define REGISTER_BASE       0xA4400000
/** @brief Number of 32-bit registers at the register base */
#define REGISTER_COUNT      14

/** @brief Memory location to read which determines the TV type. */
#define TV_TYPE_LOC         0x80000300

/**
 * @brief Return the uncached memory address of a cached address
 *
 * @param[in] x
 *            The cached address
 *uint32_t
 * @return The uncached address
 */
#define UNCACHED_ADDR(x)    ((void *)(((uint32_t)(x)) | 0xA0000000))

/**
 * @brief Align a memory address to 16 byte offset
 *
 * @param[in] x
 *            Unaligned memory address
 *
 * @return An aligned address guaranteed to be >= the unaligned address
 */
#define ALIGN_16BYTE(x)     ((void *)(((uint32_t)(x)+15) & 0xFFFFFFF0))


/** @brief Video buffer pointers */
static void *buffer[NUM_BUFFERS];
/** @brief Currently active bit depth */
uint32_t __bitdepth;
/** @brief Currently active video width (calculated) */
uint32_t __width;
/** @brief Currently active video height (calculated) */
uint32_t __height;
/** @brief Number of active buffers */
uint32_t __buffers = NUM_BUFFERS;
/** @brief Pointer to uncached 16-bit aligned version of buffers */
void *__safe_buffer[NUM_BUFFERS];
/** @brief Current VI register settings (for two fields) */
uint32_t __registers[REGISTER_COUNT*2];

/** @brief Currently displayed buffer */
static int now_showing = -1;

/** @brief Complete drawn buffer to display next */
static int show_next = -1;

/** @brief Buffer currently being drawn on */
static int now_drawing = -1;

/**
 * @brief Write a set of video registers to the VI
 *
 * @param[in] registers
 *            A pointer to a set of register values to be written
 */
static void __write_registers( uint32_t const * const registers )
{
    uint32_t *reg_base = (uint32_t *)REGISTER_BASE;

    /* This should never happen */
    if( !registers ) { return; }

    /* Just straight copy */
    for( int i = 0; i < REGISTER_COUNT; i++ )
    {
        /* Don't set frame buffer base */
        if( i == 1 ) { continue; }
        /* Don't clear interrupts */
        if( i == 3 ) { continue; }
        if( i == 4 ) { continue; }

        reg_base[i] = __registers[i];
        MEMORY_BARRIER();
        //MEMORY_BARRIER(); //removed in latest download
    }
}

/**
 * @brief Update the framebuffer pointer in the VI
 *
 * @param[in] dram_val
 *            The new framebuffer to use for display.  Should be aligned and uncached.
 */
static void __write_dram_register( void const * const dram_val )
{
    uint32_t *reg_base = (uint32_t *)REGISTER_BASE;

    if ( reg_base[4] & 1 )
    {
        /* field 2 */
        reg_base[1] = (uint32_t)dram_val + __registers[1 + REGISTER_COUNT];
        MEMORY_BARRIER();
    }
    else
    {
        /* field 1 */
        reg_base[1] = (uint32_t)dram_val + __registers[1];
        MEMORY_BARRIER();
    }
    //MEMORY_BARRIER();  // Removed in latest download
}

/**
 * @brief Interrupt handler for vertical blank
 *
 * If there is another frame to display, display the frame
 */
static void __display_callback()
{
    uint32_t *reg_base = (uint32_t *)REGISTER_BASE;

    /* Only swap frames if we have a new frame to swap */
    if(show_next >= 0 && show_next != now_drawing)
    {
        now_showing = show_next;
        show_next = -1;
    }
    __write_dram_register( __safe_buffer[now_showing] ); /* call every time for field offset */

    /* write field dependent values */
    if ( reg_base[4] & 1 )
    {
        /* field 2 */
        reg_base[10] = __registers[10 + REGISTER_COUNT];
        MEMORY_BARRIER();
        reg_base[11] = __registers[11 + REGISTER_COUNT];
        MEMORY_BARRIER();
    }
    else
    {
        /* field 1 */
        reg_base[10] = __registers[10];
        MEMORY_BARRIER();
        reg_base[11] = __registers[11];
        MEMORY_BARRIER();
    }
}

/**
 * @brief Initialize the display to a particular resolution and bit depth
 *
 * Initialize video system.  This sets up a double or triple buffered drawing surface which can
 * be blitted or rendered to using software or hardware. Uses the default tv type for region.
 *
 * @param[in] res
 *            The requested resolution
 * @param[in] bit
 *            The requested bit depth
 * @param[in] num_buffers
 *            Number of buffers (2 or 3)
 * @param[in] gamma
 *            The requested gamma setting
 * @param[in] aa
 *            The requested anti-aliasing setting
 */
void display_init( resolution_t res, bitdepth_t bit, uint32_t num_buffers, gamma_t gamma, antialias_t aa )
{
    tvtype_t tv = (tvtype_t)*((uint32_t *)TV_TYPE_LOC);
    display_init_ex( tv, res, bit, num_buffers, gamma, aa );
}

/**
 * @brief Initialize the display to a particular tv type, resolution, and bit depth
 *
 * Initialize video system.  This sets up a double or triple buffered drawing surface which can
 * be blitted or rendered to using software or hardware.
 *
 * @param[in] tv
 *            The requested tv type
 * @param[in] res
 *            The requested resolution
 * @param[in] bit
 *            The requested bit depth
 * @param[in] num_buffers
 *            Number of buffers (2 or 3)
 * @param[in] gamma
 *            The requested gamma setting
 * @param[in] aa
 *            The requested anti-aliasing setting
 */
void display_init_ex( tvtype_t tv, resolution_t res, bitdepth_t bit, uint32_t num_buffers, gamma_t gamma, antialias_t aa )
{
    /* Can't have the video interrupt happening here */
    disable_interrupts();

    /* Figure out control register based on input given */
    __registers[0] = 0x3000;
	/* Enable Dither Filter */
	if(bit == DEPTH_16_BPP_DITHER)
	{
		__registers[0] |= 0x10000;
		bit = DEPTH_16_BPP;
	}
    if ( bit == DEPTH_16_BPP )
        __registers[0] |= 0x0002; /* 16-bit color mode */
    else if ( bit == DEPTH_32_BPP )
        __registers[0] |= 0x0003; /* 32-bit color mode */
    if ( gamma == GAMMA_CORRECT )
        __registers[0] |= 0x0008; /* enable gamma correction */
    else if ( gamma == GAMMA_CORRECT_DITHER )
        __registers[0] |= 0x000C; /* enable gamma correction and dithering */
    if ( (res == RESOLUTION_640x480) || (res == RESOLUTION_512x480) )
        __registers[0] |= 0x0040; /* enable hsync serration for interlaced modes */
    if ( aa == ANTIALIAS_OFF )
        __registers[0] |= 0x0300; /* aa off */
    else if ( aa == ANTIALIAS_RESAMPLE )
        __registers[0] |= 0x0210; /* resample only, enable divot */
    else if ( aa == ANTIALIAS_RESAMPLE_FETCH_NEEDED )
        __registers[0] |= 0x0110; /* aa & resample (fetch lines as needed), enable divot */
    else if ( aa == ANTIALIAS_RESAMPLE_FETCH_ALWAYS )
        __registers[0] |= 0x0010; /* aa & resample (fetch lines always), enable divot */
	else if ( aa == ANTIALIAS_RESAMPLE_NODIVOT )
        __registers[0] |= 0x0200; /* resample only*/
    else if ( aa == ANTIALIAS_RESAMPLE_FETCH_NEEDED_NODIVOT )
        __registers[0] |= 0x0100; /* aa & resample (fetch lines as needed)*/

    /* Figure out origin, width, and x-scale registers based on input given */
    switch ( res )
    {
        case RESOLUTION_320x240:
            __width = 320;
            __height = 240;
            __registers[1] = 320; /* base offset for field 1 */
            __registers[1 + REGISTER_COUNT] = 320; /* base offset for field 2 */
            __registers[2] = 320; /* width */
            __registers[12] = 0x00000200; /* x-scale */
            break;
        case RESOLUTION_640x480:
            __width = 640;
            __height = 480;
            __registers[1] = 640; /* base offset for field 1 */
            __registers[1 + REGISTER_COUNT] = 1280; /* base offset for field 2 */
            __registers[2] = 640; /* width */
            __registers[12] = 0x00000400; /* x-scale */
            break;
        case RESOLUTION_256x240:
            __width = 256;
            __height = 240;
            __registers[1] = 256; /* base offset for field 1 */
            __registers[1 + REGISTER_COUNT] = 256; /* base offset for field 2 */
            __registers[2] = 256; /* width */
            __registers[12] = 0x0000019A; /* x-scale */
            break;
        case RESOLUTION_512x480:
            __width = 512;
            __height = 480;
            __registers[1] = 512; /* base offset for field 1 */
            __registers[1 + REGISTER_COUNT] = 1024; /* base offset for field 2 */
            __registers[2] = 512; /* width */
            __registers[12] = 0x00000334; /* x-scale */
            break;
    }
    if ( bit == DEPTH_16_BPP )
    {
        __registers[1] <<= 1;
        __registers[1 + REGISTER_COUNT] <<= 1;
        __bitdepth = 2;
    }
    else
    {
        __registers[1] <<= 2;
        __registers[1 + REGISTER_COUNT] <<= 2;
        __bitdepth = 4;
    }

    /* Interrupt on first line (2nd half-line) */
    __registers[3] = 2;

    /* Figure out burst, vsync, hsync, leap, hstart, vstart,
       vburst, and y-scale registers based on input given */
    switch ( tv )
    {
        case TV_TYPE_PAL:
            __registers[5] = 0x0404233A; /* burst */
            __registers[7] = 0x00150C69; /* hsync */
            __registers[8] = 0x0C6F0C6E; /* leap */
            __registers[9] = 0x00800300; /* hstart */
            if ( (res == RESOLUTION_640x480) || (res == RESOLUTION_512x480) )
            {
                /* hi-res interlaced display */
                __registers[6] = 0x270; /* vsync */
                __registers[10] = 0x005D0237; /* vstart for field 1 */
                __registers[10 + REGISTER_COUNT] = 0x005F0239; /* vstart for field 2 */
                __registers[11] = 0x0009026B; /* vburst for field 1 */
                __registers[11 + REGISTER_COUNT] = 0x000D0269; /* vburst for field 2 */
                __registers[13] = 0x02000800; /* y-scale */
            }
            else
            {
                /* lo-res non-interlaced display */
                __registers[6] = 0x271; /* vsync */
                __registers[10] = 0x005F0239; /* vstart for field 1 */
                __registers[10 + REGISTER_COUNT] = 0x005F0239; /* vstart for field 2 */
                __registers[11] = 0x0009026B; /* vburst for field 1 */
                __registers[11 + REGISTER_COUNT] = 0x0009026B; /* vburst for field 2 */
                __registers[13] = 0x00000400; /* y-scale */
            }
            break;
        case TV_TYPE_NTSC:
            __registers[5] = 0x03E52239; /* burst */
            __registers[7] = 0x00000C15; /* hsync */
            __registers[8] = 0x0C150C15; /* leap */
            __registers[9] = 0x006C02EC; /* hstart */
            if ( (res == RESOLUTION_640x480) || (res == RESOLUTION_512x480) )
            {
                /* hi-res interlaced display */
                __registers[6] = 0x20C; /* vsync */
                __registers[10] = 0x002301FD; /* vstart for field 1 */
                __registers[10 + REGISTER_COUNT] = 0x002501FF; /* vstart for field 2 */
                __registers[11] = 0x000E0204; /* vburst for field 1 */
                __registers[11 + REGISTER_COUNT] = 0x000E0204; /* vburst for field 2 */
                __registers[13] = 0x02000800; /* y-scale */
            }
            else
            {
                /* lo-res non-interlaced display */
                __registers[6] = 0x20C; /* vsync */
                __registers[10] = 0x002301FD; /* vstart for field 1 */
                __registers[10 + REGISTER_COUNT] = 0x002501FF; /* vstart for field 2 */
                __registers[11] = 0x000E0204; /* vburst for field 1 */
                __registers[11 + REGISTER_COUNT] = 0x000E0204; /* vburst for field 2 */
                __registers[13] = 0x0000400; /* y-scale */
            }
            break;
        case TV_TYPE_MPAL:
            __registers[5] = 0x04651E39; /* burst */
            __registers[7] = 0x00040C11; /* hsync */
            __registers[8] = 0x0C190C1A; /* leap */
            __registers[9] = 0x006C02EC; /* hstart */
            if ( (res == RESOLUTION_640x480) || (res == RESOLUTION_512x480) )
            {
                /* hi-res interlaced display */
                __registers[6] = 0x20C; /* vsync */
                __registers[10] = 0x002301FD; /* vstart for field 1 */
                __registers[10 + REGISTER_COUNT] = 0x002501FF; /* vstart for field 2 */
                __registers[11] = 0x000B0202; /* vburst for field 1 */
                __registers[11 + REGISTER_COUNT] = 0x000E0204; /* vburst for field 2 */
                __registers[13] = 0x02000800; /* y-scale */
            }
            else
            {
                /* lo-res non-interlaced display */
                __registers[6] = 0x20D; /* vsync */
                __registers[10] = 0x002501FF; /* vstart for field 1 */
                __registers[10 + REGISTER_COUNT] = 0x002501FF; /* vstart for field 2 */
                __registers[11] = 0x000E0204; /* vburst for field 1 */
                __registers[11 + REGISTER_COUNT] = 0x000E0204; /* vburst for field 2 */
                __registers[13] = 0x00000400; /* y-scale */
            }
            break;
    }

    /* Set up initial registers */
    __write_registers( __registers );

    /* Ensure that buffering is either double or triple */
    if( num_buffers != 2 && num_buffers != 3 )
    {
        __buffers = NUM_BUFFERS;
    }
    else
    {
        __buffers = num_buffers;
    }

    /* Initialize buffers and set parameters */
    for( int i = 0; i < __buffers; i++ )
    {
        /* Set parameters necessary for drawing */
        /* Grab a location to render to */
        buffer[i] = malloc( __width * __height * __bitdepth + 15 );
        __safe_buffer[i] = ALIGN_16BYTE( UNCACHED_ADDR( buffer[i] ) );

        /* Baseline is blank */
        memset( __safe_buffer[i], 0, __width * __height * __bitdepth );
    }

    /* Set the first buffer as the displaying buffer */
    __write_dram_register( __safe_buffer[0] );

    now_showing = 0;
    now_drawing = -1;
    show_next = -1;

    enable_interrupts();

    /* Set which line to call back on in order to flip screens */
    register_VI_handler( __display_callback );
    set_VI_interrupt( 1, 2 ); /* interrupt on line 1 (2nd half-line) */
}

/**
 * @brief Close the display
 *
 * Close a display and free buffer memory associated with it.
 */
void display_close()
{
    /* Can't have the video interrupt happening here */
    disable_interrupts();

    set_VI_interrupt( 0, 0 );
    unregister_VI_handler( __display_callback );

    now_showing = -1;
    now_drawing = -1;
    show_next = -1;

    __width = 0;
    __height = 0;

    __write_dram_register( 0 );

    for( int i = 0; i < __buffers; i++ )
    {
        /* Free framebuffer memory */
        if( buffer[i] )
        {
            free( buffer[i]);
        }

        buffer[i] = 0;
        __safe_buffer[i] = 0;
    }

    enable_interrupts();
}

/**
 * @brief Lock a display buffer for rendering
 *
 * Grab a display context that is safe for drawing.  If none is available
 * then this will return 0.  Do not check out more than one display
 * context at a time.
 *
 * @return A valid display context to render to or 0 if none is available.
 */
display_context_t display_lock()
{
    display_context_t retval = 0;

    /* Can't have the video interrupt happening here */
    disable_interrupts();

    for( int i = 0; i < __buffers; i++ )
    {
        if( i != now_showing && i != now_drawing && i != show_next )
        {
            /* This screen should be returned */
            now_drawing = i;
            retval = i + 1;

            break;
        }
    }

    enable_interrupts();

    /* Possibility of returning nothing, or a valid display context */
    return retval;
}

/**
 * @brief Display a previously locked buffer
 *
 * Display a valid display context to the screen on the next vblank.  Display
 * contexts should be locked via #display_lock.
 *
 * @param[in] disp
 *            A display context retrieved using #display_lock
 */
void display_show( display_context_t disp )
{
    /* They tried drawing on a bad context */
    if( disp == 0 ) { return; }

    /* Can't have the video interrupt screwing this up */
    disable_interrupts();

    /* Correct to ensure we are handling the right screen */
    int i = disp - 1;

    /* This should match, or something went awry */
    if( i == now_drawing )
    {
        /* Ensure we display this next time */
        now_drawing = -1;
        show_next = i;
    }

    enable_interrupts();
}

/** @} */ /* display */

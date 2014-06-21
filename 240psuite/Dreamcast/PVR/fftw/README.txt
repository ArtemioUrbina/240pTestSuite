How to compile fftw-3.3.4 for Dreamcast with KallistiOS:
http://www.fftw.org/download.html

############### Change kernel/cycles.h and add: ############### 

/*----------------------------------------------------------------*/
/* Dreamcast/KallistiOS */
#include <kos.h>

typedef uint64 ticks;

static inline ticks getticks(void)
{
		 return(timer_us_gettime64());
}

INLINE_ELAPSED(inline)

#define HAVE_TICK_COUNTER


############### Compile fftw-3.3.2 with: ############### 

./configure --disable-fortran --host=sh-elf CC="$KOS_CC" CFLAGS="$KOS_CFLAGS" LDFLAGS="$KOS_LDFLAGS" LIBS="$KOS_LIBS"
make

############### Copy include dir to kos-ports ############### 
############### Copy lib: ############### 

cp .libs/libfftw3.a /opt/toolchains/KallistiOS/addons/lib/dreamcast/


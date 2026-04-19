Follow the instructions at:
https://dreamcast.wiki/Getting_Started_with_Dreamcast_development#Setting_up_and_compiling_the_toolchain_with_the_dc-chain_script
Suite is built with gcc 14 and the master branch of kallistiOs

How to compile fftw-3.3.10 for Dreamcast w1ith KallistiOS:
http://www.fftw.org/download.html

############### Change kernel/cycle.h and add: ############### 

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


############### Compile fftw-3.3.10 with: ############### 

./configure --disable-fortran --host=sh-elf CC="$KOS_CC" CFLAGS="$KOS_CFLAGS" LDFLAGS="$KOS_LDFLAGS" LIBS="$KOS_LIBS" --prefix=/opt/toolchains/dc/kos-ports/
make
make install

NOTE: (tests won't build)

############### headers should end in kos-ports/include after make install ############### 


############### Copy lib: ############### 

#linux
cp .libs/libfftw3.a /opt/toolchains/dc/kos/addons/lib/dreamcast/

#DreamSDK
cp .libs/libfftw3.a /opt/toolchains/dc/kos-ports/lib


makeip source:
https://github.com/ArtemioUrbina/makeip

img4dc (cdi4dc) source:
https://github.com/ArtemioUrbina/img4dc

cp cdi4dc /opt/toolchains/dc/bin
cp makeip /opt/toolchains/dc/bin

Build /opt/toolchains/dc/kos/utils/scramble and copy it:
/opt/toolchains/dc/kos/utils/scramble

install mkisofs

For kmgenc install libjpeg-dev

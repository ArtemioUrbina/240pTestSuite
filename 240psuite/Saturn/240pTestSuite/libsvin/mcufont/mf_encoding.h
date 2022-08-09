/* Simple UTF-8 decoder. Also implements the much simpler ASCII and UTF16
 * input encodings.
 */

#ifndef _MF_ENCODING_H_
#define _MF_ENCODING_H_

#include "mf_config.h"
#include <stdint.h>

/* Type used to represent characters internally. */
#if MF_ENCODING == MF_ENCODING_ASCII
typedef char mf_char;
#else
typedef uint16_t mf_char;
#endif

/* Type used to represent input strings. */
#if MF_ENCODING == MF_ENCODING_ASCII
typedef const char * mf_str;
#elif MF_ENCODING == MF_ENCODING_UTF8
typedef const char * mf_str;
#elif MF_ENCODING == MF_ENCODING_UTF16
typedef const uint16_t * mf_str;
#elif MF_ENCODING == MF_ENCODING_WCHAR
#include <stddef.h>
typedef const wchar_t * mf_str;
#endif

/* Returns the next character in the string and advances the pointer.
 * When the string ends, returns 0 and leaves the pointer at the 0 byte.
 * 
 * str: Pointer to variable holding current location in string.
 *      Initialize it to the start of the string.
 * 
 * Returns: The next character, as unicode codepoint.
 */
MF_EXTERN mf_char mf_getchar(mf_str *str);

/* Moves back the pointer to the beginning of the previous character.
 * Be careful not to go beyond the start of the string.
 */
MF_EXTERN void mf_rewind(mf_str *str);

#endif

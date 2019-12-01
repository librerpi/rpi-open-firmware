/*------------------------------------------------------------------------*/
/* Universal string handler for user console interface  (C)ChaN, 2011     */
/*------------------------------------------------------------------------*/

#ifdef __arm__
	#include <stdarg.h>
#else
	#include "stdarg.h"
#endif


#ifndef _STRFUNC
#define _STRFUNC

#define _USE_XFUNC_OUT	1	/* 1: Use output functions */
#define	_CR_CRLF		1	/* 1: Convert \n ==> \r\n in the output char */

#ifdef __cplusplus
extern "C" {
#endif

int putchar (int c);
int puts (const char* str);
int printf (const char* fmt, ...) __attribute__ ((format (printf, 1, 2)));
void put_dump (const void* buff, unsigned long addr, int len, int width);


int vprintf (
	const char*	fmt,	/* Pointer to the format string */
	va_list arp			/* Pointer to arguments */
);

#define DW_CHAR		sizeof(char)
#define DW_SHORT	sizeof(short)
#define DW_LONG		sizeof(long)

#ifdef __cplusplus
}
#endif

#endif

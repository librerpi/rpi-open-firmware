#include "xprintf.h"

int printf (			/* Put a formatted string to the default device */
	const char*	fmt,	/* Pointer to the format string */
	...					/* Optional arguments */
)
{
	va_list arp;

	va_start(arp, fmt);
	vprintf(fmt, arp);
	va_end(arp);
}

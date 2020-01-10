#include "xprintf.h"

/*----------------------------------------------*/
/* Put a null-terminated string                 */
/*----------------------------------------------*/

int __puts (				/* Put a string to the default device */
	const char* str			/* Pointer to the string */
)
{
	while (*str)
		putchar(*str++);
}

int puts (					/* Put a string to the default device */
	const char* str				/* Pointer to the string */
)
{
	__puts(str);
	putchar('\n');
}

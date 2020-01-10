#include "xprintf.h"

/*----------------------------------------------*/
/* Put a character                              */
/*----------------------------------------------*/

int putchar (int c)
{
	if (_CR_CRLF && c == '\n') putchar('\r');		/* CR -> CRLF */

	uart_putc(c);
}

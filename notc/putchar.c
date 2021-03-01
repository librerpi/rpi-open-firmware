#include "xprintf.h"

/*----------------------------------------------*/
/* Put a character                              */
/*----------------------------------------------*/

int putchar (int c) {
  pl011_putchar(c);
}

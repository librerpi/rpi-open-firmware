#include <stdint.h>
#include <stdio.h>
#include "aux.h"
#include <hardware.h>

void main_entry() {
  uint32_t t = GP_FSEL1;
  t &= ~(7<<12) & ~(7<<15); // clear FSEL for gpio 14&15
  t |= (2<<12) |  (2<<15); // set mode 2 on 14&15
  GP_FSEL1 = t;

  setup_aux_uart(115200);

  puts("hello from c");
}

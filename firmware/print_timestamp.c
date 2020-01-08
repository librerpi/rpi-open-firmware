#include <stdio.h>

#include "hardware.h"

void print_timestamp() {
  uint32_t clock_lo = ST_CLO;

  printf("%3ld.%06ld ", clock_lo / 1000000, clock_lo % 1000000);
}

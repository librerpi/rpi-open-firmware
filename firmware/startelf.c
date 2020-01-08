#include <stdint.h>
#include <stdio.h>
#include <hardware.h>
#include <stdbool.h>
#include "aux.h"
#include "pll_read.h"
#include "interrupt.h"

extern int print_debug;

void main_entry() {
  uint32_t t = GP_FSEL1;
  t &= ~(7<<12) & ~(7<<15); // clear FSEL for gpio 14&15
  t |= (2<<12) |  (2<<15); // set mode 2 on 14&15
  GP_FSEL1 = t;

  setup_aux_uart(115200);

  print_debug = 1;

  puts("hello from c");
  printf("CM_VPUDIV is 0x%08lx\n", CM_VPUDIV);
  printf("PLLA is %lu hz\n", plla());
  printf("PLLB is %lu hz\n", pllb());
  printf("PLLC is %lu hz\n", pllc());
  printf("PLLD is %lu hz\n", plld());
  printf("PLLH is %lu hz\n", pllh());

  setup_irq_handlers();

  puts("vector table installed");
  __asm__ volatile("ei");
  ST_C0 = 10 * 1000 * 1000;

  for(;;) {
    __asm__ __volatile__ ("sleep" :::);
    print_timestamp();
    puts("sleep interrupted!");
  }
}

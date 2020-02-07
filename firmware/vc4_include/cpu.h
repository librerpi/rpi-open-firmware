#pragma once

#include <hardware.h>
#include <stdbool.h>

static inline void __attribute__((noreturn)) _hang_cpu() {
#if 0
	/* disable interrupts  */
	__asm__ __volatile__ ("di");

	/* loop */
	for (;;) {
		__asm__ __volatile__ ("nop");
	}
#else
  __asm__ __volatile__ ("di");
  // disable every external interrupt
  volatile uint32_t *maskreg = (volatile uint32_t*)(IC0_BASE + 0x10);
  for (int i=0; i<8; i++) maskreg[i] = 0;
  maskreg = (volatile uint32_t*)(IC1_BASE + 0x10);
  for (int i=0; i<8; i++) maskreg[i] = 0;
  // go to sleep forever (shouldnt need a loop, but just to be safe)
  while (true) {
    __asm__ __volatile__ ("sleep");
  }
#endif
}

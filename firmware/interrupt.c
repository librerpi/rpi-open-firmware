#include <stdbool.h>
#include <hardware.h>
#include <stdio.h>
#include "traps.h"
#include "interrupt.h"
#include <panic.h>

typedef void (*irqType)();

irqType __attribute__ ((aligned (512))) vectorTable[144];

void setup_irq_handlers() {
  vectorTable[0] = fleh_zero;
  vectorTable[1] = fleh_misaligned;
  vectorTable[2] = fleh_dividebyzero;
  vectorTable[3] = fleh_undefinedinstruction;
  vectorTable[4] = fleh_forbiddeninstruction;
  vectorTable[5] = fleh_illegalmemory;
  vectorTable[6] = fleh_buserror;
  vectorTable[7] = fleh_floatingpoint;
  vectorTable[8] = fleh_isp;
  vectorTable[9] = fleh_dummy;
  vectorTable[10] = fleh_icache;
  vectorTable[11] = fleh_veccore;
  vectorTable[12] = fleh_badl2alias;
  vectorTable[13] = fleh_breakpoint;
  for (int i=14; i<=31; i++) {
    vectorTable[i] = fleh_unknown;
  }
  for (int i=32; i<=143; i++) {
    vectorTable[i] = fleh_irq;
  }

  // enable all irq's on core 0
  // disable all irq's on core 1
  for (int i=0; i<64; ++i) {
    set_interrupt(i, true, 0);
    set_interrupt(i, false, 1);
  }
  // disable a few select irqs on core 0
  set_interrupt(73 - 64, false, 0); // 9
  set_interrupt(96 - 64, false, 0); // 32
  set_interrupt(120 - 64, false, 0); // 56
  set_interrupt(121 - 64, false, 0); // 57
  set_interrupt(125 - 64, false, 0); // 61

  IC0_VADDR = (uint32_t)vectorTable;
  IC1_VADDR = (uint32_t)vectorTable;

  print_timestamp();
  printf("vector table now at 0x%08lx 0x%08lx\n", IC0_VADDR, (uint32_t)vectorTable);

  if (IC0_VADDR != ((uint32_t)vectorTable)) panic("vector table not accepted");
}

void set_interrupt(int intno, bool enable, int core) {
    uint32_t base = (core == 0) ? IC0_BASE : IC1_BASE;

    int offset = 0x10 + ((intno >> 3) << 2);
    uint32_t slot = 0xF << ((intno & 7) << 2);

    uint32_t v = mmio_read32(base + offset) & ~slot;
    mmio_write32(base + offset, enable ? v | slot : v);
}


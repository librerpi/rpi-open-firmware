#include <stdint.h>
#include <hardware.h>
#include <stdio.h>

#define UART_DR 0x3f201000
#define UART_FR 0x3f201018

#define REG32(x) ( (volatile uint32_t*)(x) )

#define logf(fmt, ...) { print_timestamp(); printf("[BRINGUP:%s]: " fmt, __FUNCTION__, ##__VA_ARGS__); }

int boot_count = 0;

void drop_privs(uint64_t link, uint64_t spsr, uint64_t scr);
void reentry();

void uart_putc(char c) {
  while (*REG32(UART_FR) & 0x20);

  *REG32(UART_DR) = c;
}

#define showreg(reg) { uint64_t t; __asm__ volatile ("mrs %0," #reg : "=r"(t)); printf(#reg ": 0x%016x\n", t); }

void dump_things() {
  showreg(CurrentEL);
  showreg(S3_1_C15_C3_0);
  uint64_t currentel;
  __asm__ volatile ("mrs %0, CurrentEL" : "=r"(currentel));
  switch (currentel) {
  case 0xc:
    logf("EL3 regs:\n");
    showreg(RVBAR_EL3);
    showreg(VBAR_EL3);
    showreg(SCR_EL3);
  case 0x8:
    logf("EL2 regs:\n");
    showreg(VBAR_EL2);
    showreg(SCR_EL2);
  case 0x4:
    logf("EL1 regs:\n");
    showreg(RVBAR_EL1);
    showreg(SCR_EL1);
  }
}

#define SPSR_EL_M_EL2H 0x9

void handle_exception200(uint64_t CurrentEL, uint64_t elr, uint64_t esr) {
  puts("exception handler 0x200");
  printf("CurrentEL: %x\nELR: %x\nESR: %x\n", CurrentEL, elr, esr);
  showreg(FAR_EL3);
  showreg(SP_EL0);
  showreg(SP_EL1);
  showreg(SP_EL2);
  showreg(SPSR_EL3);
  uint64_t spsr;
  __asm__("mrs %0, SPSR_EL3": "=r"(spsr));
  printf("M: 0x%x\nM[4]:%d\nIL:%d\n", spsr & 0xf, (spsr >> 4) & 1, (spsr >> 20) & 1);
  showreg(ESR_EL3);
  printf("EC: 0x%x\n", (esr >> 26) & 0x3f);
}

void main() {
  while(ARM_ID != ARM_IDVAL);
  puts("hello world");
  uint8_t c = 'A';
  asm volatile("msr DBGDTRTX_EL0, %0":: "r" (c));
  logf("Started on ARM, continuing boot from here ...\n");
  logf("boot_count: %d\n", boot_count++);
  dump_things();
  puts("dropping privs");
  // TODO: set the A/I/F bits to 1
  drop_privs(reentry, ((0b1111 << 6) | 9), 1<<10 | 0xf);
}

void el2_main() {
  logf("now in el2?\n");
  dump_things();
}

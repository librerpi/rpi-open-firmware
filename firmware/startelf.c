#include <stdint.h>
#include <stdio.h>
#include <hardware.h>
#include <stdbool.h>
#include "aux.h"
#include "pl011.h"
#include "pll_read.h"
#include "interrupt.h"
#include "utils.hh"

#include <stdlib.h>

uint32_t g_CPUID;

enum RamSize g_RAMSize = kRamSize1GB; // FIXME

void test_caches();
void poke_the_dog();
void spin_the_gpio_wheel();
void gpclk0_test();
void hexdump_ram(uint32_t addr, uint32_t count);

int tprintf(const char *format, ...) __attribute__ ((format (printf, 1, 2)));

int tprintf(const char *format, ...) {
  va_list va;

  print_timestamp();

  va_start(va, format);
  int ret = vprintf(format, va);
  va_end(va);
  return ret;
}

void main_entry() {
  bool gpio_level[64];
  enum BCM2708PinmuxSetting functions[64];

  gpio_snapshot(gpio_level, functions);
  poke_the_dog();

  __asm__("version %0" : "=r"(g_CPUID));

  pl011_uart_init(115200);
  set_pl011_funcs();

  tprintf("hello from c\n");

  //gpio_print_snapshot(gpio_level, functions);

  tprintf("vpu freq is %lu\n", get_vpu_per_freq());
  tprintf("uart freq is %lu\n", get_uart_base_freq());

  setup_irq_handlers();

  puts("vector table installed");

  __asm__ volatile("ei");
  __cxx_init();
  hexdump_ram(0x7e100000, 512);
  //PEStartPlatform();
  //            11112222
  //hexdump_ram(0x7e200000, 512);
  //ST_C0 = 10 * 1000 * 1000;

  //gpclk0_test();
  //spin_the_gpio_wheel();

  for(;;) {
    __asm__ __volatile__ ("sleep" :::);
    print_timestamp();
    puts("sleep interrupted!");
  }
}

void gpclk0_test() {
  // outputs a 20mhz clock on gpio4
  uint32_t t = GP_FSEL0;
  t &= ~(7<<12); // clear FSEL for gpio 4
  t |= (4<<12); // set mode 2 on 14&15
  GP_FSEL0 = t;

  CM_GP0DIV = CM_PASSWORD | 0x1e000; // divisor times 0x1000
  CM_GP0CTL = CM_PASSWORD | 5;
  CM_GP0CTL = CM_PASSWORD | 5 | (1 << 4);
}

void gpio_test(uint32_t n, uint32_t *samples);

uint32_t samples[64000];

void spin_the_gpio_wheel() {
  uint32_t start = ST_CLO;
  gpio_test(64000, samples);
  uint32_t stop = ST_CLO;
  printf("64,000 samples done in %lu usec\n", stop - start);
}

void poke_the_dog() {
  PM_WDOG = PM_PASSWORD | ((15 << 16) & PM_WDOG_MASK); // seconds to watchdog
  uint32_t t = PM_RSTC;
  t &= PM_RSTC_WRCFG_CLR;
  t |= 0x20;
  PM_RSTC = PM_PASSWORD | t;
}

// count must be a multiple of 4bytes
void fill_range_numbered(uint32_t addr, uint32_t count) {
  volatile uint32_t *ram = 0;
  for (uint32_t i = addr; i < (addr + count); i += 4) {
    ram[i/4] =i;
  }
}

void test_caches() {
  uint32_t size = 32;
  fill_range_numbered(0x80000000, size);
  hexdump_ram(0x00000000, size);
  hexdump_ram(0x40000000, size);
  hexdump_ram(0x80000000, size);
  hexdump_ram(0xc0000000, size);
}

void do_arm_dump() {
  volatile uint32_t *test = &ARM_CONTROL0;
  for (int i=0; i < 16; i++) {
    printf("%p == %lx\n", &test[i], test[i]);
  }
}

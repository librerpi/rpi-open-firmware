#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>

#include "map_peripherals.h"
#include "hexdump.h"

const char *pretty_alt_mode(uint8_t mode) {
	switch (mode) {
	case 0: return "input";
	case 1: return "output";
	case 2: return "alt5";
	case 3: return "alt4";
	case 4: return "alt0";
	case 5: return "alt1";
	case 6: return "alt2";
	case 7: return "alt3";
	default: return "error";
	}
}

void print_gpclk(volatile uint8_t *base, uint8_t number) {
	volatile uint32_t *control = reinterpret_cast<volatile uint32_t*>(base + 0x70 + (number * 8));
	volatile uint32_t *divisor = reinterpret_cast<volatile uint32_t*>(base + 0x74 + (number * 8));
	uint32_t actual_control = *control;
	uint32_t actual_divisor = *divisor;

	uint8_t source = actual_control & 0xf;
	bool enabled = actual_control & (1 << 4);
	bool kill = actual_control & (1 << 5);
	bool busy = actual_control & (1 << 7);
	bool flip = actual_control & (1 << 8);
	uint8_t mash = (actual_control >> 9) & 3;

	double real_divisor = ((double)actual_divisor) / 0x1000;

	printf("GPCLK%d: source:%d enable:%c kill:%c busy:%c flip:%c mash:%d divisor:%f (0x%x)\n", number, source, enabled?'Y':'N', kill?'Y':'N', busy?'Y':'N', flip?'Y':'N', mash, real_divisor, actual_divisor);
}

int main_crystal = 54000000; // 54MHz

// base: the current address of A2W_BASE (to allow mmap)
// offset: given a A2W_PLL?_DIG0 register (relative to A2W_BASE)
void print_pll(volatile uint8_t *base, const char *name, uint32_t offset) {
  volatile uint32_t *dig = reinterpret_cast<volatile uint32_t*>(base + offset);
  volatile uint32_t *ana = reinterpret_cast<volatile uint32_t*>(base + 0x10 + offset);
  uint32_t control = *reinterpret_cast<volatile uint32_t*>(base + 0x100 + offset);
  uint32_t frac = *reinterpret_cast<volatile uint32_t*>(base + 0x200 + offset);
  uint32_t kaip = *reinterpret_cast<volatile uint32_t*>(base + 0x310 + offset);
  uint32_t multi = *reinterpret_cast<volatile uint32_t*>(base + 0xf00 + offset);
  uint16_t ndiv = control & 0x000003ff;
  uint16_t pdiv = (control & 0x00007000) >> 12;
  bool power_down = control & 0x00010000;
  bool prstn = control & 0x00020000;
  printf("\n%4s: 0x0__\t0x1__\t0x2__\t0x3__\t0xf__\n", name);
  printf("0x_00 %6x\t%x\t%x\t\t%x\n", dig[0], control, frac, multi);
  printf("0x_04 %6x\n", dig[1]);
  printf("0x_08 %6x\n", dig[2]);
  printf("0x_0c %6x\n", dig[3]);
  printf("0x_10 %6x\t\t\t%x\n", ana[0], kaip);
  printf("0x_14 %6x\n", ana[1]);
  printf("0x_18 %6x\n", ana[2]);
  printf("0x_1c %6x\n", ana[3]);
  printf("nvid: %d pdiv: %d power-down: %c prstn: %c control: 0x%x\n", ndiv, pdiv, power_down?'1':'0', prstn?'1':'0', control);
  printf("FRAC: %d (0x%x)\n", frac, frac);
  float divisor = (float)ndiv + ((float)frac / (1<<20));
  printf("freq: %f\n", (main_crystal / pdiv) * divisor);
}

void print_pll_subdivider(volatile uint8_t *base, const char *name, uint32_t offset) {
	uint32_t control = *reinterpret_cast<volatile uint32_t*>(base + offset);
	uint8_t div = control & 0xff;
	bool channel_enable = control & 0x00000100;
	bool bypass_enable = control & 0x00000200;

	printf("%7s: divisor:%d enable:%c bypass:%c\n", name, div, channel_enable?'1':'0', bypass_enable?'1':'0');
}

void print_2nd_divider(volatile uint8_t *base, const char *name, uint32_t offset) {
  volatile uint32_t *regs = reinterpret_cast<volatile uint32_t*>(base + offset);
  //regs[0] &= ~0x10;
  //regs[1] = 0x5600;
  printf("%sCTL: 0x%x\n%sDIV: 0x%x\n", name, regs[0], name, regs[1]);
  uint32_t s = regs[0];
  int src = s & 0xf;
  int divisor = regs[1] >> 4;
  printf("  src: %d\n  enabled: %d\n  kill: %d\n  busy: %d\n  busyd: %d\n  frac: %d\n", s & 0xf, (s >> 4)&1, (s>>5)&1, (s>>7)&1, (s>>8)&1, (s >> 9)&1);
  printf("  divisor: %f\n", (float)divisor / 0x100);
  switch (src) {
  case 1:
    printf("crystal/(0x%x>>8) == %fMHz\n", divisor, main_crystal / ( ((float)divisor) / 0x100) / 1000 / 1000 );
    break;
  }
}

int main(int argc, char **argv) {
  struct peripherals handle;
  open_peripherals(handle);
  volatile uint8_t *addr = static_cast<volatile uint8_t*>(handle.peripherals_start);
  volatile uint8_t *gpio = addr + 0x200000;
  volatile uint32_t *fsel = reinterpret_cast<volatile uint32_t*>(gpio);
  for (int i=0; i<6; i++) {
    uint32_t bank_mode = fsel[i];
    for (int j=0; j<10; j++) {
      uint8_t mode = bank_mode >> (j*3) & 7;
      printf("GPIO %d%d mode %s\n", i, j, pretty_alt_mode(mode));
    }
  }

  volatile uint8_t *clkman_base = addr + 0x101000;
  print_gpclk(clkman_base, 0);
  print_gpclk(clkman_base, 1);
  print_gpclk(clkman_base, 2);

  volatile uint8_t *pll_base = addr + 0x102000;
  print_pll(pll_base, "PLLA", 0x00);
  print_pll_subdivider(pll_base, "A_DSI0", 0x300);
  print_pll_subdivider(pll_base, "A_CORE", 0x400);
  print_pll_subdivider(pll_base, "A_PER", 0x500);
  print_pll_subdivider(pll_base, "A_CCP2", 0x600);
  print_pll(pll_base, "PLLB", 0xe0);
  print_pll_subdivider(pll_base, "B_ARM", 0x3e0);
  print_pll_subdivider(pll_base, "B_SP0", 0x4e0);
  print_pll_subdivider(pll_base, "B_SP1", 0x5e0);
  print_pll_subdivider(pll_base, "B_SP2", 0x6e0);
  print_pll(pll_base, "PLLC", 0x20);
  print_pll_subdivider(pll_base, "C_CORE2", 0x320);
  print_pll_subdivider(pll_base, "C_CORE1", 0x420);
  print_pll_subdivider(pll_base, "C_PER", 0x520);
  print_pll_subdivider(pll_base, "C_CORE0", 0x620);
  print_pll(pll_base, "PLLD", 0x40);
  print_pll_subdivider(pll_base, "D_DSI0", 0x340);
  print_pll_subdivider(pll_base, "D_CORE", 0x440);
  print_pll_subdivider(pll_base, "D_PER", 0x540);
  print_pll_subdivider(pll_base, "D_DSI1", 0x640);
  if (0) {
    print_pll(pll_base, "PLLH", 0x60);
    print_pll_subdivider(pll_base, "H_AUX", 0x360);
    print_pll_subdivider(pll_base, "H_RCAL", 0x460);
    print_pll_subdivider(pll_base, "H_PIX", 0x560);
  }
  print_2nd_divider(addr, "CM_DPI", 0x101068);
  //hexdump_ram(((uint32_t)handle.peripherals_start) + 0x102000, 0x7e102000, 0x400);
  return 0;
}

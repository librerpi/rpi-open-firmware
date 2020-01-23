#include "xprintf.h"

#include <drivers/BCM2708Gpio.hpp>
#include "utils.hh"
#include "drivers/BCM2708ClockDomains.hpp"
#include <pcb.h>
#include "interrupt.h"

const char *function_names[] = {
  "IN",
  "OUT",
  "ALT5",
  "ALT4",
  "ALT0",
  "ALT1",
  "ALT2",
  "ALT3"
};

void dump_all_gpio() {
  BCM2708Gpio *gpio = static_cast<BCM2708Gpio*>(IODevice::findByTag(GPIO_TAG));
  bool gpio_level[64];
  BCM2708PinmuxSetting functions[64];
  gpio_snapshot(gpio_level, functions);
  gpio_print_snapshot(gpio_level, functions);
}

void set_pl011_funcs() {
  BCM2708Gpio *gpio = static_cast<BCM2708Gpio*>(IODevice::findByTag(GPIO_TAG));
  gpio->setFunction(14, kBCM2708Pinmux_ALT0);
  gpio->setFunction(15, kBCM2708Pinmux_ALT0);
}

void gpio_print_snapshot(const bool gpio_level[64], const BCM2708PinmuxSetting functions[64]) {
  for (int i=0; i<32; i++) {
    printf("GPIO%02d %4s %s | %s %4s GPIO%02d\n", i, function_names[functions[i]], gpio_level[i] ? "HIGH" : " LOW", gpio_level[i+32] ? "HIGH" : "LOW ", function_names[functions[i+32]], i + 32);
  }
}

void gpio_snapshot(bool gpio_level[64], BCM2708PinmuxSetting functions[64]) {
  BCM2708Gpio *gpio = static_cast<BCM2708Gpio*>(IODevice::findByTag(GPIO_TAG));
  for (uint8_t bank = 0; bank <2; bank++) {
    uint32_t state = gpio->getBank(bank);
    for (uint8_t pin = 0; pin < 32; pin++) {
      gpio_level[(bank * 32) + pin] = state & (1 << pin);
    };
  }
  gpio->getAllFunctions(functions);
}

void setup_eth_clock(uint8_t pin) {
  pllc_per.configure(2);
  BCM2708Gpio *gpio = static_cast<BCM2708Gpio*>(IODevice::findByTag(GPIO_TAG));
  gpio->setFunction(pin, kBCM2708Pinmux_ALT0);

  // GP0 for testing on header
  CM_GP0DIV = CM_PASSWORD | 0x28000; // divisor times 0x1000
  CM_GP0CTL = CM_PASSWORD | (2 << 9) | 5;
  CM_GP0CTL = CM_PASSWORD | (2 << 9) | 5 | (1 << 4);

  // GP1 routed to GPIO42 to drive ethernet/usb chip
  CM_GP1DIV = CM_PASSWORD | 0x28000; // divisor times 0x1000
  CM_GP1CTL = CM_PASSWORD | (2 << 9) | 5;
  CM_GP1CTL = CM_PASSWORD | (2 << 9) | 5 | (1 << 4);

  gpio->setFunction(42, kBCM2708Pinmux_ALT0);

  //gpio->setFunction(28, kBCM2708Pinmux_ALT0);
  //gpio->setFunction(29, kBCM2708Pinmux_ALT0);

  //gpio->setFunction(46, kBCM2708Pinmux_ALT2);
  //gpio->setFunction(47, kBCM2708Pinmux_ALT2);

  // something usb related
  gpio->setFunction(29, kBCM2708PinmuxOut);
  gpio->clearPin(29);
  udelay(1000);
  gpio->setFunction(29, kBCM2708PinmuxIn);
  // 46&47 not i2c?

  gpio->setFunction(2, kBCM2708Pinmux_ALT0);
  gpio->setFunction(3, kBCM2708Pinmux_ALT0);
}

void safe_putchar(unsigned char c) {
  if ((c >= ' ') && (c <= '~')) {
    printf("%c", c);
  } else {
    printf(".");
  }
}

void peripheral_scan() {
  uint32_t *ram32 = 0;
  for (uint32_t i = 0x7e000; i < 0x7ffff; i++) {
    if (i == 0x7e9c) continue; // hang
    if (i == 0x7e9d) continue; // hang
    if (i == 0x7e9e) continue; // hang
    if (i == 0x7e9f) continue; // hang
    if ((i & 0xfff0) == 0x7ea0) continue; // exception
    if ((i & 0xfff0) == 0x7eb0) continue; // exception
    if ((i & 0xfff0) == 0x7ec0) continue; // exception
    if ((i & 0xfff0) == 0x7ef0) continue; // exception
    if ((i & 0xff00) == 0x7f00) continue; // exception
    uint32_t id = ram32[ ( (i << 12) | 0xfff) / 4];
    if (id == 0) continue;
    printf("0x%04lxFFF == 0x%08lx \"", i, id);
    uint8_t a,b,c,d;
    a = id & 0xff;
    b = (id >> 8) & 0xff;
    c = (id >> 16) & 0xff;
    d = (id >> 24) & 0xff;
    safe_putchar(d);
    safe_putchar(c);
    safe_putchar(b);
    safe_putchar(a);
    printf("\"\n");
  }
  puts("scan done");
}

// addr must be 16 aligned
// count must be a multiple of 16 bytes
void hexdump_ram(uint32_t addr, uint32_t count) {
  volatile uint32_t *ram32 = 0;
  volatile uint8_t *ram8 = 0;
  for (uint32_t i = addr; i < (addr + count); i += 16) {
    uint32_t fragment;
    printf("0x%08lx ", i);
    for (int j=0; j<4; j++) {
      fragment = ram32[(i/4)+j];
      uint8_t a,b,c,d;
      a = fragment & 0xff;
      b = (fragment >> 8) & 0xff;
      c = (fragment >> 16) & 0xff;
      d = (fragment >> 24) & 0xff;
      printf("%02x %02x %02x %02x ", a,b,c,d);
      if (j == 1) printf(" ");
    }
    printf(" |");
    for (int j=0; j<4; j++) {
      fragment = ram32[(i/4)+j];
      uint8_t a,b,c,d;
      a = fragment & 0xff;
      b = (fragment >> 8) & 0xff;
      c = (fragment >> 16) & 0xff;
      d = (fragment >> 24) & 0xff;
      safe_putchar(a);
      safe_putchar(b);
      safe_putchar(c);
      safe_putchar(d);
    }
    printf("|\n");
  }
}

void test_matrix1() {
  uint32_t a[16], b[16];
  for (int i=0; i<16; i++) {
    a[i] = i;
  }
  uint32_t d = 10;
  __asm__ volatile (
  // load 16 uint32_t's from a into row 0,0
      "v32ld HY(0++,0),(%0)\n"
  // multiply all 16 of them by d, save results into a row starting at 1,0
      "vmull.ss HY(1,0), HY(0,0), %1\n"
  // store the row at 1,0 into b
      "v32st HY(1,0), (%2)"
      :
      : "r"(a), "r"(d), "r"(b)
      :"memory");
  for (int i=0; i<16; i++) {
    printf("%d * %ld is %lu\n", i, d, b[i]);
  }
}

extern "C" void dump_all_state(void (*)(vc4_saved_state_t*));
extern "C" void print_vpu_state(vc4_saved_state_t* pcb);
extern "C" void test_rti(void (*)(), uint32_t sr);
extern "C" char *decode_reg(uint32_t reg);

void rti_target() {
  dump_all_state(print_vpu_state);
}

void test_things() {
  uint32_t a;
  __asm__ volatile ("mov %0, 0x400f\n"
                    "add %0,-16399"
                    : "=r"(a));
  printf("0x%lx\n", a);
  dump_all_state(print_vpu_state);
  __asm__ volatile("di");
  test_rti(rti_target, 0xf0);
  dump_all_state(print_vpu_state);
  for (uint32_t i=0; i <= 0xffffffff; i++) {
    char *str = decode_reg(i);
    if (str) printf("0x%lx -> %s\n", i, str);
    if ((i % 0x10000000) == 0) printf("0x%lx\n", i);
  }
  panic("done testing");
}

extern "C" void swi1_test();

void do_irq_test() {
  puts("in irq test");
  printf("IC0_VADDR 0x%08lx\n", IC0_VADDR);
  dump_all_state(print_vpu_state);
  vectorTable[33] = swi1_test;
  __asm__ volatile("mov r0, %0\nswi 1"::"r"(print_vpu_state));
  puts("done irq test");
  ST_C0 = ST_CLO + (10 * 1000 * 1000);
}

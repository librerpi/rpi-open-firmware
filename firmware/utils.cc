#include "xprintf.h"

#include <drivers/BCM2708Gpio.hpp>
#include "utils.hh"
#include "drivers/BCM2708ClockDomains.hpp"
#include <pcb.h>

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
  BCM2708Gpio *gpio = static_cast<BCM2708Gpio*>(IODevice::findByTag('GPIO'));
  bool gpio_level[64];
  BCM2708PinmuxSetting functions[64];
  gpio_snapshot(gpio_level, functions);
  gpio_print_snapshot(gpio_level, functions);
}

void set_pl011_funcs() {
  BCM2708Gpio *gpio = static_cast<BCM2708Gpio*>(IODevice::findByTag('GPIO'));
  gpio->setFunction(14, kBCM2708Pinmux_ALT0);
  gpio->setFunction(15, kBCM2708Pinmux_ALT0);
}

void gpio_print_snapshot(const bool gpio_level[64], const BCM2708PinmuxSetting functions[64]) {
  for (int i=0; i<32; i++) {
    printf("GPIO%02d %4s %s | %s %4s GPIO%02d\n", i, function_names[functions[i]], gpio_level[i] ? "HIGH" : " LOW", gpio_level[i+32] ? "HIGH" : "LOW ", function_names[functions[i+32]], i + 32);
  }
}

void gpio_snapshot(bool gpio_level[64], BCM2708PinmuxSetting functions[64]) {
  BCM2708Gpio *gpio = static_cast<BCM2708Gpio*>(IODevice::findByTag('GPIO'));
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
  BCM2708Gpio *gpio = static_cast<BCM2708Gpio*>(IODevice::findByTag('GPIO'));
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
}

void safe_putchar(unsigned char c) {
  if ((c >= ' ') && (c <= '~')) {
    printf("%c", c);
  } else {
    printf(".");
  }
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

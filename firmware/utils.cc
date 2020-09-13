#include "xprintf.h"

#include <drivers/BCM2708Gpio.hpp>
#include "utils.hh"
#include "drivers/BCM2708ClockDomains.hpp"
#include <pcb.h>
#include "interrupt.h"
#include "otp.h"
#include <hexdump.h>

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

static __thread int foo;

void dump_all_gpio() {
  bool gpio_level[64];
  BCM2708PinmuxSetting functions[64];
  gpio_snapshot(gpio_level, functions);
  gpio_print_snapshot(gpio_level, functions);
}

void set_pl011_funcs() {
  gGPIO.setFunction(14, kBCM2708Pinmux_ALT0);
  gGPIO.setFunction(15, kBCM2708Pinmux_ALT0);
}

void enable_jtag() {
  gGPIO.setFunction(22, kBCM2708Pinmux_ALT4); // TRST
  gGPIO.setFunction(24, kBCM2708Pinmux_ALT4); // TDO
  gGPIO.setFunction(25, kBCM2708Pinmux_ALT4); // TCK
  gGPIO.setFunction(26, kBCM2708Pinmux_ALT4); // TDI
  gGPIO.setFunction(27, kBCM2708Pinmux_ALT4); // TMS
}

void gpio_print_snapshot(const bool gpio_level[64], const BCM2708PinmuxSetting functions[64]) {
  for (int i=0; i<32; i++) {
    printf("GPIO%02d %4s %s | %s %4s GPIO%02d\n", i, function_names[functions[i]], gpio_level[i] ? "HIGH" : " LOW", gpio_level[i+32] ? "HIGH" : "LOW ", function_names[functions[i+32]], i + 32);
  }
}

void gpio_snapshot(bool gpio_level[64], BCM2708PinmuxSetting functions[64]) {
  for (uint8_t bank = 0; bank <2; bank++) {
    uint32_t state = gGPIO.getBank(bank);
    for (uint8_t pin = 0; pin < 32; pin++) {
      gpio_level[(bank * 32) + pin] = state & (1 << pin);
    };
  }
  gGPIO.getAllFunctions(functions);
}

void set_gp_mode(int mash, int source) {
  uint32_t before = ST_CLO;
  CM_GP0CTL = CM_PASSWORD | (CM_GP0CTL & CM_GP0CTL_ENAB_CLR); // disable clock
  while (CM_GP0CTL & CM_GP0CTL_BUSY_SET) {} // wait for it to halt
  CM_GP0CTL = CM_PASSWORD | (mash << 9) | source;
  CM_GP0CTL = CM_PASSWORD | (mash << 9) | source | CM_GP0CTL_ENAB_SET;
  uint32_t after = ST_CLO;
  printf("switch to %d took %ld\n", source, after - before);
}

void report_sdram_usage() {
  uint32_t idle = SD_IDL;
  uint32_t total = SD_CYC;
  SD_IDL = 0;
  float idle_percent = ((float)idle) / ((float)total);
  printf("sdram usage: %ld %ld, %f\t", idle, total, idle_percent);
}

void setup_eth_clock(struct OtpInfo *info) {
  int lan_run = 0;
  int ethclk_pin = 0;

  switch (info->type) {
  case 4: // 2B
    lan_run = 31;
    ethclk_pin = 44;
    break;
  case 8: // 3B
    lan_run = 29;
    ethclk_pin = 42;
    break;
  case 0xd: // 3B+
    lan_run = 30;
    ethclk_pin = 42;
    break;
  }

  pllc_per.configure(2);

  if (0) {
    // GP0 for testing on header
    CM_GP0DIV = CM_PASSWORD | 0x28000; // divisor times 0x1000
    CM_GP0CTL = CM_PASSWORD | (2 << 9) | 5;
    CM_GP0CTL = CM_PASSWORD | (2 << 9) | 5 | (1 << 4);
    gGPIO.setFunction(4, kBCM2708Pinmux_ALT0);
  }

  if (ethclk_pin > 0) {
    // GP1 routed to GPIO42 to drive ethernet/usb chip
    CM_GP1DIV = CM_PASSWORD | 0x28000; // divisor times 0x1000
    CM_GP1CTL = CM_PASSWORD | (2 << 9) | 5;
    CM_GP1CTL = CM_PASSWORD | (2 << 9) | 5 | (1 << 4);

    gGPIO.setFunction(ethclk_pin, kBCM2708Pinmux_ALT0);
  }

  //gpio->setFunction(46, kBCM2708Pinmux_ALT2);
  //gpio->setFunction(47, kBCM2708Pinmux_ALT2);

  if (lan_run > 0) {
    // toggle the reset line to the usb chip
    gGPIO.setFunction(lan_run, kBCM2708PinmuxOut);
    gGPIO.clearPin(lan_run);
    udelay(1000);
    gGPIO.setPin(lan_run);
    //gpio->setFunction(29, kBCM2708PinmuxIn);
  }
  // 46&47 not i2c?

  // i2c bus exploration
  //gGPIO.setFunction(2, kBCM2708Pinmux_ALT0);
  //gGPIO.setFunction(3, kBCM2708Pinmux_ALT0);
}

/*void setup_tec_clock() {
  CM_TECCTL = (CM_TECCTL & CM_TECCTL_ENAB_CLR) | CM_PASSWORD; // disable the TEC clock
  while (CM_TECCTL & CM_TECCTL_BUSY_SET) {} // wait for it to stop
  CM_TECCTL = CM_PASSWORD; // clear all config
  CM_TECDIV = CM_PASSWORD | 0x1000; // set divisor
  CM_TECCTL = CM_PASSWORD | 1; // set clock source
  CM_TECCTL = CM_PASSWORD | CM_TECCTL_ENAB_SET | 1; // enable
}*/

uint32_t get_raw_temp() {
  return TS_TSENSSTAT;
}

void print_temp() {
  uint32_t raw = get_raw_temp();
  float converted = 412 - (raw & 0x3ff) * 0.538;
  printf("Temp: %f\t", converted);
}

void setup_tsens() {
  CM_TSENSCTL = (CM_TSENSCTL & CM_TSENSCTL_ENAB_CLR) | CM_PASSWORD; // disable TSENS
  while (CM_TSENSCTL & CM_TSENSCTL_BUSY_SET) {} // wait for it to stop
  CM_TSENSCTL = CM_PASSWORD; // clear all config
  CM_TSENSDIV = CM_PASSWORD | 0x5000; // set divisor
  CM_TSENSCTL = CM_PASSWORD | 1; // set clock source
  CM_TSENSCTL = CM_PASSWORD | CM_TSENSCTL_ENAB_SET | 1; // enable

  TS_TSENSCTL = 0x4380004;
  TS_TSENSCTL |= 2;
}

void peripheral_scan() {
  uint32_t *ram32 = 0;
  for (uint32_t i = 0x7e000; i < 0x7ffff; i++) {
    if ((i & 0xfffff) == 0x7e981) continue; // hang
    if (i == 0x7e9c) continue; // hang
    if (i == 0x7e9d) continue; // hang
    if (i == 0x7e9e) continue; // hang
    if (i == 0x7e9f) continue; // hang
    if ((i & 0xfff00) == 0x7ea00) continue; // exception
    if ((i & 0xfff0) == 0x7eb0) continue; // exception
    if ((i & 0xfff00) == 0x7ec00) continue; // exception
    if ((i & 0xfff0) == 0x7ef0) continue; // exception
    if ((i & 0xff000) == 0x7f000) continue; // exception
    printf("0x%lx\n", ( (i << 12) | 0xfff));
    uint32_t id = ram32[ ( (i << 12) | 0xffc) / 4];
    if (id == 0) continue;
    printf("0x%04lxFFC == 0x%08lx \"", i, id);
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

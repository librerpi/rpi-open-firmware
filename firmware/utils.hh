#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "drivers/BCM2708Gpio.hpp"
  void dump_all_gpio();
  void setup_eth_clock(uint8_t pin);
  void gpio_print_snapshot(const bool gpio_level[64], const enum BCM2708PinmuxSetting functions[64]);
  void gpio_snapshot(bool gpio_level[64], enum BCM2708PinmuxSetting functions[64]);
  void set_pl011_funcs();
  void hexdump_ram(uint32_t addr, uint32_t count);
  void peripheral_scan();
  void test_things();
  void do_irq_test(void);
#ifdef __cplusplus
};
#endif

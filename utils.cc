#include <drivers/BCM2708Gpio.hpp>
#include "utils.hh"
#include "drivers/BCM2708ClockDomains.hpp"

void dump_all_gpio() {
  BCM2708Gpio *gpio = static_cast<BCM2708Gpio*>(IODevice::findByTag('GPIO'));
  for (uint8_t bank = 0; bank <2; bank++) {
    uint32_t state = gpio->getBank(bank);
    for (uint8_t pin = 0; pin < 32; pin++) {
      printf("GPIO%02d == %s\n", (bank * 32) + pin, (state & (1 << pin)) ? "HIGH" : " LOW");
    };
  }
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

#include <drivers/BCM2708Gpio.hpp>

void BCM2708Gpio::setFunction(uint8_t pin_num, BCM2708PinmuxSetting function) {
  volatile uint32_t* fsel = reinterpret_cast<volatile uint32_t*>(&GP_FSEL0);
  uint8_t regnum = pin_num / 10;

  uint8_t pin_shift = (pin_num % 10) * 3;

  fsel[regnum] = (fsel[regnum] & ~(0x7 << pin_shift)) | (function << pin_shift);
}

void BCM2708Gpio::getAllFunctions(BCM2708PinmuxSetting *functions) {
  volatile uint32_t* fsel = reinterpret_cast<volatile uint32_t*>(&GP_FSEL0);
  for (int bank=0; bank<7; bank++) {
    int pins_in_bank = (bank == 6) ? 4 : 10;
    for (int pin=0; pin < pins_in_bank; pin++) {
      functions[(bank * 10) + pin] = static_cast<BCM2708PinmuxSetting>((fsel[bank] >> (pin * 3)) & 0x7);
    }
  }
}

void BCM2708Gpio::clearPin(uint8_t pin_num) {
  volatile uint32_t *clear = reinterpret_cast<volatile uint32_t*>(&GP_CLR0);
  uint8_t regnum = pin_num / 32;
  uint8_t pin_shift = pin_num % 32;

  clear[regnum] = 1 << pin_shift;
}

void BCM2708Gpio::setPin(uint8_t pin_num) {
  volatile uint32_t *clear = reinterpret_cast<volatile uint32_t*>(&GP_SET0);
  uint8_t regnum = pin_num / 32;
  uint8_t pin_shift = pin_num % 32;

  clear[regnum] = 1 << pin_shift;
}

bool BCM2708Gpio::getPin(uint8_t pin_num) {
  const volatile uint32_t *level = reinterpret_cast<const volatile uint32_t*>(&GP_LEV0);
  uint8_t regnum = pin_num / 32;
  uint8_t pin_shift = pin_num % 32;

  return level[regnum] & (1 << pin_shift);
}

uint32_t BCM2708Gpio::getBank(uint8_t bank) {
  const volatile uint32_t *level = reinterpret_cast<const volatile uint32_t*>(&GP_LEV0);
  return level[bank];
}

void BCM2708Gpio::init() {
	setTag(GPIO_TAG);
}

BCM2708Gpio gGPIO {};

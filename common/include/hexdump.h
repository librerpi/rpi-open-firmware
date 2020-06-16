#include <stdint.h>

extern "C" void hexdump_ram(volatile void *realaddr, uint32_t reportaddr, uint32_t count);
extern "C" void safe_putchar(unsigned char c);

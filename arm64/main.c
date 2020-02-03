#include <stdint.h>

#define UART_DR 0x3f201000
#define UART_FR 0x3f201018

#define REG32(x) ( (volatile uint32_t*)(x) )

void uart_putc(char c) {
  while (*REG32(UART_FR) & 0x20);

  *REG32(UART_DR) = c;
}

void puts(const char *str) {
  while (*str) uart_putc(*str++);
}

void main() {
  puts("hello world\n");
}

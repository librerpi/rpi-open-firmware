#include <stdio.h>
#include <hardware.h>
#include "pl011.h"
#include "pll_read.h"

#define UART_IBRD *((volatile uint32_t*)(0x7e201024))
#define UART_FBRD *((volatile uint32_t*)(0x7e201028))

static void pl011_putchar(unsigned char c) {
  if (c == '\n') pl011_putchar('\r');
  while(UART_MSR & 0x20);
  UART_RBRTHRDLL = c;
}

static int uart_write(void *uart, const char *str, int length) {
  for (int i=0; i<length; i++) {
    pl011_putchar(str[i]);
  }
  return length;
}

static void pl011_flush() {
  while(UART_MSR & 0x20);
}

void pl011_uart_init(uint32_t baud) {
  uint32_t uart_freq = get_uart_base_freq();
  uint32_t divisor = (uart_freq << 6) / baud / 16;
  pl011_flush();
  UART_IBRD = (divisor >> 6) & 0xffff;
  UART_FBRD = divisor & 0x3f;
  stdout = funopen(0, 0, uart_write, 0, 0);
  setvbuf(stdout, 0, _IONBF, 0);
}

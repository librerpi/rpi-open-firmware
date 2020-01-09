#pragma once

void setup_aux_uart(uint32_t baud);
extern struct uart_device aux_uart;
void uart_putchar(struct uart_device *uart, char c);
int uart_write(struct uart_device *uart, const char *str, int length);

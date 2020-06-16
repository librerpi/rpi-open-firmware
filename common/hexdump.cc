#include <stdint.h>
#include <stdio.h>
#include <hexdump.h>

void safe_putchar(unsigned char c) {
  if ((c >= ' ') && (c <= '~')) {
    printf("%c", c);
  } else {
    printf(".");
  }
}

// realaddr must be 16 aligned
// reads from realaddr, but claims to be from reportaddr, to allow mmap usage
// count must be a multiple of 16 bytes
void hexdump_ram(volatile void *realaddr, uint32_t reportaddr, uint32_t count) {
  volatile uint32_t *buffer_start = reinterpret_cast<volatile uint32_t*>(realaddr);
  for (uint32_t i = 0, fakeaddr = reportaddr; i < count; i += 16, fakeaddr += 16) {
    uint32_t fragment;
    printf("0x%08lx ", fakeaddr);
    for (int j=0; j<4; j++) {
      fragment = buffer_start[(i+j)/4];
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
      fragment = buffer_start[(i+j)/4];
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

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <hexdump.h>

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "usage: %s <addr>\n", argv[0]);
    return 1;
  }
  uint32_t addr = strtoll(argv[1], NULL, 0);
  addr = (addr >> 12) << 12;
  int length = 0x1000;

  if (argc >= 3) {
    length = strtol(argv[2], NULL, 0);
  }

  int fd = open("/dev/mem", O_RDWR);
  if (fd < 0) {
    perror("unable to open /dev/mem");
    return 2;
  }
  volatile void *rawaddr = (uint32_t*)mmap(NULL, length, PROT_READ, MAP_SHARED, fd, addr);
  if (rawaddr == MAP_FAILED) {
    perror("unable to mmap");
    return 3;
  }
  printf("starting at 0x%x (%dMB)\n", addr, addr/1024/1024);
  hexdump_ram(rawaddr, addr, length);
  return 0;
}

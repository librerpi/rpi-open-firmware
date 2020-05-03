#include <stdio.h>
#include <sys/mman.h>

void *c_mmap_helper(int fd) {
  return mmap(NULL, 0x1800000, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
}

#include <stdio.h>

int mmap_helper(void *addr, size_t len, int prot, int flags, int fildes, off_t off) {
  printf("mmap(%p, %x, %d, %d, %d, %ld)\n", addr, len, prot, flags, fildes, off);
  return 42;
}

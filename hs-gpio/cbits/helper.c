#include <stdio.h>
#include <sys/mman.h>
#include <stdint.h>
#include <errno.h>

void *c_mmap_helper(uint32_t offset, uint32_t size, int fd) {
  void *addr = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, offset);
  //printf("%p %d\n", addr, errno);
  //uint32_t *addr2 = addr;
  //for (int i=0; i<64; i++) {
  //  printf("%d %x\n", i, addr2[i]);
  //}
  return addr;
}

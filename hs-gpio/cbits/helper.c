#include <stdio.h>

int mmap_helper(int a, int b, int c, int d, int e, off_t off) {
  printf("mmap(%x, %x, %d, %d, %d, %lx)\n", a, b, c, d, e, off);
  printf("sizeof(off_t) == %d\n", sizeof(off_t));
  return 42;
}
int mmap_helper6(int a, int b, int c, int d, int e, off_t f) {
  printf("mmap6(%x, %x, %d, %d, %d, %lx)\n", a, b, c, d, e, f);
  return 42;
}
int mmap_helper5(int a, int b, int c, int d, off_t e) {
  printf("mmap5(%x, %x, %d, %d, %lx)\n", a, b, c, d, e);
  return 42;
}

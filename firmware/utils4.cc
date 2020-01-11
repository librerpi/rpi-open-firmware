#include <stdint.h>
#include <hardware.h>
#include <stdio.h>
#include "pll_read.h"
#include "utils.hh"

// at a vpu freq of 500mhz
// repeating a 16x mult 64 * 128 times took 17uS
// 64 * 64 times took 9uS
// 64 * 256 times took 33uS
// 64 * 1024 times took 131uS
// ~0.1279296875 uS to do a single `vmull.ss HX(0++,0), HX(0++,0), 2 REP64 ; addcmpbne %5, -1, 0, loop`
// which comes to about 64 clock cycles
void test_matrix_speed() {
  uint16_t a[1024];
  for (int i=0; i<1024; i++) a[i] = i;

  uint32_t start, stop, count = 2048;
#if 1
  __asm__ volatile (
      // load 1024 int16's into 0,0 to 65,15
      "v16ld HX(0++,0), (%2+=%3) REP64\n"
      // load start time
      "ld %0, (%4)\n"
      "loop:"
      // multiply all 1024 by 2
      "vmull.ss HX(0++,0), HX(0++,0), 2 REP64\n"
      // repeat count times
      "addcmpbne %5, -1, 0, loop\n"
      // save stop time
      "ld %2, (%4)\n"
      : "=&r"(start), "=r"(stop)
      : "r"(a), "r"(2*16), "r"(&ST_CLO), "r"(count)
      : "memory");
#endif
  printf("16 * 64 * %ld mult + %ld addcmpbne took time delta %lu uS\n", count, count, stop - start);
  uint32_t vpu_freq = get_vpu_per_freq();
  printf("vpu freq is %lu\n", vpu_freq);
  // > ((64 * 1024) / 500000015) * 1000 * 1000
  // 131.07199606784013
  // > ((64 * 2048) / 500000015) * 1000 * 1000
  // 262.14399213568026
  // 64 * count / vpu_freq == total time spent in loop
}

void test_matrix_big_mult() {
  uint16_t a[1024];
  for (int i=0; i<1024; i++) a[i] = i;

#if 1
  __asm__ volatile (
      // load 1024 int16's into 0,0 to 65,15
      "v16ld HX(0++,0), (%0+=%1) REP64\n"
      // multiply all 1024 by 2
      "vmull.ss HX(0++,0), HX(0++,0), 2 REP64\n"
      // save all 1024 back to ram
      "v16st HX(0++,0), (%0+=%1) REP64"
      :
      : "r"(a), "r"(2*16)
      : "memory");
#endif
  for (int i=0; i<64; i++) {
    for (int j=0; j<16; j++) {
      int index = (i*16) + j;
      printf("%5d*2==%5u", index, a[index]);
    }
    printf("\n");
  }
}

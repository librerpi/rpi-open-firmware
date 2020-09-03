#include <stdint.h>
#include <hardware.h>
#include <hexdump.h>
#include <stdio.h>
#include <stdbool.h>

#define logf(fmt, ...) { print_timestamp(); printf("[MMU:%s]: " fmt, __FUNCTION__, ##__VA_ARGS__); }
void arm7_dcache_off();
void arm_mmu_off();

uint32_t __attribute__ ((aligned (16384))) level1[4096] __attribute__ ((section(".bss.page_table")));

#define B (1<<2)
#define C (1<<3)
#define TEX(tex) ((tex & 0x7) << 12)

// SCTLR
#define M 1

void init_page_tables(uint32_t ram_size) {
  // identity maps ${ram_size}mb of ram with full caching as sections (1mb pages, 1 level of table)
  for (int i=0; i<ram_size; i++) {
    level1[i] = (i<<20) // section base addr
          | (1<<15) // AP[2]
          | TEX(1) // TEX=1
          | (3<<10) // AP[1:0]
          | C
          | B
          | 1<<1;
  }
  // also identity map 16mb of mmio, starting at ARM_PERIPH_BASE, no cache/prefetch
  for (int i=ARM_PERIPH_BASE >> 20; i < ((ARM_PERIPH_BASE >> 20)+16); i++) {
    level1[i] = (i<<20) | 3<<10 | 2;
  }

  // based on a peek at uboot src
  __asm__ volatile ("mcr p15, 0, %0, c2, c0, 0" : : "r" (((uint32_t)level1) | 1<<1) : "memory"); // TTBR0
  __asm__ volatile ("mcr p15, 0, %0, c2, c0, 2" : : "r" (1<<5 | 0<<4));       // TTBCR

  /* Set the access control to all-supervisor */
  __asm__ volatile("mcr p15, 0, %0, c3, c0, 0" : : "r" (~0));

  uint32_t sctlr;
  __asm__ volatile ("mrc p15, 0, %0, c1, c0, 0" : "=r"(sctlr));
  sctlr |= 1<<29 | 1<<2 | M;
  __asm__ volatile ("mcr p15, 0, %0, c1, c0, 0" : : "r" (sctlr) : "cc");
  logf("MMU is now on\n");
}

void mmu_off(void) {
  arm7_dcache_off();
  arm_mmu_off();
  logf("MMU now off\n");
}

static uint32_t get_cache_shape(int level, bool instruction) {
  uint32_t shape;
  __asm__ volatile ("mcr p15, 2, %0, c0, c0, 0": : "r"( ((level-1) & 7) << 1 | (instruction ? 1 : 0)));
  __asm__ volatile ("mrc p15, 1, %0, c0, c0, 0": "=r"(shape));
  return shape;
}

static void print_shape(const char *name, uint32_t shape) {
  int wt = (shape & 1<<31) != 0;
  int wb = (shape & 1<<30) != 0;
  int ra = (shape & 1<<29) != 0;
  int wa = (shape & 1<<28) != 0;
  int sets = (shape >> 13) & 0x7fff;
  int associativity = (shape >> 3) & 0x3ff;
  int lines = shape & 7;
  int line_size = 1 << (lines+2); // in words (32bit values)
  printf("%s\t%d KB\t0x%lx\t%d\t%d\t%d\t%d\t0x%x\t0x%x\t0x%d(%d bytes)\n",
        name, ((sets+1) * (associativity+1) * (line_size*4)) / 1024, shape, wt, wb, ra, wa, sets, associativity, lines, line_size * 4);
}

void print_cache_tree(void) {
  printf("name\tsize\tencoding\tWT\tWB\tRA\tWA\tSets\tAssoc\tLineSize\n");
  print_shape("L1d", get_cache_shape(1, false));
  print_shape("L1i", get_cache_shape(1, true));
  print_shape("L2", get_cache_shape(2, false));
}

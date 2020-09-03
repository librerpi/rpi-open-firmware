#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct level1_entry_t {
  uint32_t addr : 20;
} level1_entry;

void init_page_tables(uint32_t ram_size);
void mmu_off(void);
void print_cache_tree(void);

#ifdef __cplusplus
}
#endif

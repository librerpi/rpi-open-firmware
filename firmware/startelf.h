#pragma once

void main_entry(vc4_saved_state_t* pcb);
void do_arm_dump(void);
void gpclk0_test(void);
void spin_the_gpio_wheel(void);
void poke_the_dog(void);
void fill_range_numbered(uint32_t addr, uint32_t count);
void test_caches(void);

#pragma once

void asm_drop_secure();
void asm_enable_fpu();
void asm_set_ACTLR(uint32_t);
void set_CPUECTLR();
void enable_icache();
void disable_icache();
void debug_stall();

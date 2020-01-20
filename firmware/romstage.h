#pragma once

#include "pcb.h"

extern uint32_t g_CPUID;

int _main(unsigned int cpuid, uint32_t load_address, vc4_saved_state_t* pcb);

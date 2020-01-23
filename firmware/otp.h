#pragma once

#include <stdint.h>

void dump_otp(void);
void otp_open(void);
void otp_close(void);
uint32_t otp_read_reg(uint32_t addr);

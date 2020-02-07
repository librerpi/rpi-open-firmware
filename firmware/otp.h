#pragma once

#include <stdint.h>

struct OtpInfo {
  int type;
  int rev;
  int proc;
};

void dump_otp(void);
void otp_open(void);
void otp_close(void);
uint32_t otp_read_reg(uint32_t addr);

uint32_t otp_read(uint8_t addr);
void dump_all_otp(void);
void pretty_print_otp(void);

void parse_otp_into(struct OtpInfo *dest);

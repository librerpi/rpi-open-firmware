#include <stdio.h>
#include "otp.h"
#include "hardware.h"

#define RPI3

void otp_open() {
  OTP_CONFIG_REG = 0x3;
  OTP_CTRL_HI_REG = 0;
  OTP_CTRL_LO_REG = 0;

  OTP_CONFIG_REG = 0x2;
}

uint32_t otp_read_reg(uint32_t addr) {
  OTP_ADDR_REG = addr;
  uint32_t a = OTP_ADDR_REG;
  OTP_CTRL_HI_REG = 0;
  OTP_CTRL_LO_REG = 0;

  a = OTP_CTRL_LO_REG;
  printf("dummy 0x%lx\n", a);
  OTP_CTRL_LO_REG = 1;
  //a = OTP_CTRL_LO_REG;
  //printf("dummy 0x%lx\n", a);

  for (int i=0; i<1; i++) {
    a = OTP_STATUS_REG;
    printf("status is 0x%lx\n", a);

#ifdef RPI3
    //if (a & 0x1) continue;
#endif
#ifdef RPI4
    //if (a & 0x2) continue;
#endif
    //break;
  }
  return OTP_DATA_REG;
}

void otp_close() {
  OTP_CTRL_HI_REG = 0;
  OTP_CTRL_LO_REG = 0;
  OTP_CONFIG_REG = 0;
}

void dump_otp() {
  printf("dumping OTP\n");
  printf("BOOTMODE: 0x%08lx\n", OTP_BOOTMODE_REG);
  for (int otp_reg = 0; otp_reg < 67; otp_reg++) {
    otp_open();
    uint32_t a = otp_read_reg(otp_reg);
    printf("%d:%lx\n", otp_reg, a);
    otp_close();
  }
}

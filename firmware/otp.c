#include <stdio.h>
#include <string.h>
#include "otp.h"
#include "hardware.h"

uint32_t otp_read_internal(uint32_t addr);
uint32_t otp_read(uint8_t addr) {
  return otp_read_internal(addr);
}

void dump_all_otp() {
  printf("full otp dump\n");
  for (uint8_t addr=0; addr < 67; addr++) {
    uint32_t value = otp_read(addr);
    printf("%02d:%08lx\n", addr, value);
  }
}

void parse_otp_into(struct OtpInfo *dest) {
  memset(dest, 0, sizeof(struct OtpInfo));
  uint32_t revision = otp_read(30);
  if (revision & (1 <<23)) { // new style revision
    dest->rev = revision & 0xf;
    dest->type = (revision >> 4) & 0xff;
    dest->proc = (revision >> 12) & 0xf;
    //printf("  Manufacturer: %ld\n", (revision >> 16) & 0xf);
    //printf("  Ram: %ld\n", (revision >> 20) & 0x7);
  }
  uint32_t serial = otp_read(28);
  if (~serial != otp_read(29)) printf("WARNING: serial# duplicate doesnt match\n");
  dest->serial = serial;
  dest->revision = otp_read(30);
}

void pretty_print_otp() {
  // https://github.com/raspberrypi/firmware/issues/974
  // addr 32 contains warrenty flag
  uint32_t bootmode = otp_read(17);
  printf("OTP17: 0x%08lx\n", bootmode);
  if (bootmode & (1<< 1)) printf("  19.2mhz crystal present\n");
  if (bootmode & (1<< 3)) printf("  SDIO pullups should be on?\n");
  if (bootmode & (1<<19)) printf("  GPIO boot mode\n");
  if (bootmode & (1<<20)) printf("  GPIO boot mode bank bit is set\n");
  if (bootmode & (1<<21)) printf("  SD card boot enabled\n");
  if (bootmode & (1<<22)) printf("  bank to boot from set\n");
  if (bootmode & (1<<28)) printf("  USB device boot enabled\n");
  if (bootmode & (1<<29)) printf("  USB host boot enabled (ethernet/mass-storage)\n");
  if (bootmode != otp_read(18)) printf("WARNING: boot mode duplicate doesnt match\n");
  uint32_t serial = otp_read(28);
  printf("\nSoC serial# 0x%08lx\n", serial);
  if (~serial != otp_read(29)) printf("WARNING: serial# duplicate doesnt match\n");
  uint32_t revision = otp_read(30);
  // https://www.raspberrypi.org/documentation/hardware/raspberrypi/revision-codes/README.md
  printf("\nHW revision: 0x%08lx\n", revision);
  if (revision & (1 <<23)) { // new style revision
    printf("  Type: %ld\n", (revision >> 4) & 0xff);
    printf("  Rev: %ld\n", revision & 0xf);
    printf("  Proc: %ld\n", (revision >> 12) & 0xf);
    printf("  Manufacturer: %ld\n", (revision >> 16) & 0xf);
    printf("  Ram: %ld\n", (revision >> 20) & 0x7);
  }
  uint32_t maclow = otp_read(64);
  uint32_t machi = otp_read(65);
  printf("\nOptional Mac Override: %08lx%lx\n", maclow, machi);

  uint32_t advanced_boot = otp_read(66);
  printf("\nOTP66: 0x%08lx\n", advanced_boot);
  if (advanced_boot & (1<<7)) {
    printf("  ETH_CLK on GPIO%ld\n", advanced_boot & 0x7f);
    if (advanced_boot & (1<<25)) printf("  ETH_CLK 24mhz\n");
    else printf("  ETH_CLK 25mhz\n");
  }
  if (advanced_boot & (1<<15)) {
    printf("  LAN_RUN on GPIO%ld\n", (advanced_boot >> 8) & 0x7f);
  }
  if (advanced_boot & (1<<24)) printf("  extended USB timeout\n");
}

#if 0
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
#endif
